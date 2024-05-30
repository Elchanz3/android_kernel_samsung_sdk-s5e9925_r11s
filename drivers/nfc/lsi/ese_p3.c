// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2015 Samsung Electronics. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/spi/spi.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/regulator/consumer.h>
#include <linux/ioctl.h>
#ifdef CONFIG_OF
#include <linux/of_gpio.h>
#endif

#include <linux/platform_data/spi-s3c64xx.h>
#include <linux/pm_runtime.h>
#include <linux/spi/spidev.h>
#include <linux/clk.h>
#ifdef CONFIG_SPI_QCOM_GENI
#include <linux/spi/spi-geni-qcom.h>
#endif

#include "nfc_wakelock.h"
#include "ese_p3.h"

#ifdef CONFIG_SEC_NFC_LOGGER
#include "../nfc_logger/nfc_logger.h"
#endif

#define P3GPIO_IS_VALID(x) (gpio_is_valid(x) && x)

#define SPI_DEFAULT_SPEED 6500000L

static int ese_param_support = -1;
module_param(ese_param_support, int, 0440);

enum pm_mode {
	PM_SUSPEND,
	PM_RESUME,
	SECURE_CHECK,
};

enum secure_state {
	NOT_CHECKED,
	ESE_SECURED,
	ESE_NOT_SECURED,
};

/* size of maximum read/write buffer supported by driver */
#define MAX_BUFFER_SIZE   260U

/* Different driver debug lever */
enum P3_DEBUG_LEVEL {
	P3_DEBUG_OFF,
	P3_FULL_DEBUG
};

#ifdef CONFIG_SEC_NFC_LOGGER
/* Variable to store current debug level request by ioctl */
static unsigned char debug_level = P3_FULL_DEBUG;

#define P3_DBG_MSG(msg...) do { \
		switch (debug_level) { \
		case P3_DEBUG_OFF: \
			break; \
		case P3_FULL_DEBUG: \
			NFC_LOG_INFO("S: "msg); \
			break; \
		default: \
			NFC_LOG_ERR("S: debug level %d", debug_level);\
			break; \
		}; \
	} while (0)

#define P3_ERR_MSG(msg...) NFC_LOG_ERR("S: " msg)
#define P3_INFO_MSG(msg...) NFC_LOG_INFO("S: " msg)
#else
/* Variable to store current debug level request by ioctl */
static unsigned char debug_level = P3_FULL_DEBUG;

#define P3_DBG_MSG(msg...) do { \
		switch (debug_level) { \
		case P3_DEBUG_OFF: \
			break; \
		case P3_FULL_DEBUG: \
			pr_info("ESE: "msg); \
			break; \
			 /*fallthrough*/ \
		default: \
			pr_err("ESE: debug level %d", debug_level);\
			break; \
		}; \
	} while (0)

#define P3_ERR_MSG(msg...) pr_err("ESE: "msg)
#define P3_INFO_MSG(msg...) pr_info("ESE: " msg)
#endif

static DEFINE_MUTEX(device_list_lock);

/* Device specific macro and structure */
struct p3_data {
	wait_queue_head_t read_wq; /* wait queue for read interrupt */
	struct mutex buffer_mutex; /* buffer mutex */
	struct spi_device *spi;  /* spi device structure */
	struct miscdevice p3_device; /* char device as misc driver */

	unsigned int users;

	bool device_opened;
#ifdef FEATURE_ESE_WAKELOCK
	struct nfc_wake_lock ese_lock;
#endif
	unsigned long speed;
	const char *vdd_1p8;
	int cs_gpio;
	struct regulator *ese_pvdd;
	unsigned char *tx_buffer;
	unsigned char *rx_buffer;

	bool pwr_always_on;
	enum coldrst_type coldrst_type;

	char opened_task[TASK_COMM_LEN + 1];
	pid_t opened_pid;

	u32 read_1byte_cnt;
};

#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
struct p3_data *g_p3_dev;
#endif

static void p3_pinctrl_config(struct p3_data *data, bool onoff)
{
	struct spi_device *spi = data->spi;
	struct device *spi_dev = spi->dev.parent->parent;
	struct pinctrl *pinctrl = NULL;

	P3_INFO_MSG("%s: %s\n", __func__, onoff ? "on" : "off");

	if (onoff) {
		/* ON */
		pinctrl = devm_pinctrl_get_select(spi_dev, "ese_active");
		if (IS_ERR_OR_NULL(pinctrl))
			P3_ERR_MSG("no ese_active pinctrl %ld\n", PTR_ERR(pinctrl));
		else
			devm_pinctrl_put(pinctrl);
	} else {
		/* OFF */
		pinctrl = devm_pinctrl_get_select(spi_dev, "ese_suspend");
		if (IS_ERR_OR_NULL(pinctrl))
			P3_ERR_MSG("no ese_suspend pinctrl %ld\n", PTR_ERR(pinctrl));
		else
			devm_pinctrl_put(pinctrl);
	}
}

static int p3_regulator_onoff(struct p3_data *data, int onoff)
{
	int rc = 0;
	struct regulator *regulator_ese_pvdd = data->ese_pvdd;

#ifndef CONFIG_ESE_USE_REGULATOR
	return 0;
#endif

	if (!regulator_ese_pvdd) {
		P3_ERR_MSG("no regulator!\n");
		goto done;
	}

	P3_DBG_MSG("%s - onoff = %d\n", __func__, onoff);
	if (onoff == 1) {
		regulator_set_voltage(regulator_ese_pvdd, 1800000, 1800000);
		rc = regulator_enable(regulator_ese_pvdd);
		if (rc) {
			P3_ERR_MSG("%s - enable vdd_1p8 failed, rc=%d\n",
				__func__, rc);
			goto done;
		}
		msleep(20);
	} else {
		rc = regulator_disable(regulator_ese_pvdd);
		if (rc) {
			P3_ERR_MSG("%s - disable vdd_1p8 failed, rc=%d\n",
				__func__, rc);
			goto done;
		}
	}

	P3_DBG_MSG("success\n");
done:
	return rc;
}

static int p3_xfer(struct p3_data *p3_device, struct p3_ioctl_transfer *tr)
{
	int status = 0;
	struct spi_message m;
	struct spi_transfer t;
	unsigned char *tx_buffer;
	unsigned char *rx_buffer;

	P3_DBG_MSG("%s\n", __func__);

	if (p3_device == NULL || tr == NULL)
		return -EFAULT;

	tx_buffer = p3_device->tx_buffer;
	rx_buffer = p3_device->rx_buffer;
	memset(tx_buffer, 0x0, sizeof(char)*MAX_BUFFER_SIZE);
	memset(rx_buffer, 0x0, sizeof(char)*MAX_BUFFER_SIZE);

	if (tr->len > MAX_BUFFER_SIZE || !tr->len) {
		P3_ERR_MSG("%s invalid size\n", __func__);
		return -EMSGSIZE;
	}

	if (tr->tx_buffer != NULL) {
		if (copy_from_user(tx_buffer,
				tr->tx_buffer, tr->len) != 0)
			return -EFAULT;
	}

	spi_message_init(&m);
	memset(&t, 0, sizeof(t));

	t.tx_buf = tx_buffer;
	t.rx_buf = rx_buffer;
	t.len = tr->len;

	spi_message_add_tail(&t, &m);

	status = spi_sync(p3_device->spi, &m);
	if (status == 0) {
		if (tr->rx_buffer != NULL) {
			unsigned int missing = 0;

			missing = (unsigned int)copy_to_user(tr->rx_buffer,
					       rx_buffer, tr->len);

			if (missing != 0)
				tr->len = tr->len - missing;
		}
	}
	P3_INFO_MSG("xfer%d\n", tr->len);

	return status;

} /* vfsspi_xfer */

static int p3_rw_spi_message(struct p3_data *p3_device,
				 unsigned long arg)
{
	struct p3_ioctl_transfer   *dup = NULL;
	int err = 0;

	dup = kmalloc(sizeof(struct p3_ioctl_transfer), GFP_KERNEL);
	if (dup == NULL)
		return -ENOMEM;

	if (copy_from_user(dup, (void *)arg,
			   sizeof(struct p3_ioctl_transfer)) != 0) {
		kfree(dup);
		return -EFAULT;
	}

	err = p3_xfer(p3_device, dup);
	if (err != 0) {
		kfree(dup);
		P3_ERR_MSG("%s xfer failed!\n", __func__);
		return err;
	}

	/*P3_ERR_MSG("%s len:%u\n", __func__, dup->len);*/
	if (copy_to_user((void *)arg, dup,
			 sizeof(struct p3_ioctl_transfer)) != 0) {
		kfree(dup);
		return -EFAULT;
	}

	kfree(dup);
	return 0;
}

#ifdef CONFIG_ESE_COLDRESET
#if !IS_ENABLED(CONFIG_SAMSUNG_ESE_ONLY)
extern int trig_nfc_wakeup(void);
extern int trig_nfc_sleep(void);
#endif
#endif

static int spip3_open(struct inode *inode, struct file *filp)
{
#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	struct p3_data *p3_dev = g_p3_dev;
#else
	struct p3_data *p3_dev = container_of(filp->private_data, struct p3_data, p3_device);
#endif
	struct task_struct *task = current;
	int ret = 0;

	if (p3_dev == NULL) {
		P3_ERR_MSG("%s: spi probe is not called\n", __func__);
		return -EAGAIN;
	}

	/* for defence MULTI-OPEN */
	if (p3_dev->device_opened) {
		P3_ERR_MSG("ALREADY opened! try(%d, %s), opened(%d, %s)\n",
			task->pid, task->comm, p3_dev->opened_pid, p3_dev->opened_task);
		return -EBUSY;
	}
#ifdef CONFIG_ESE_COLDRESET
#if !IS_ENABLED(CONFIG_SAMSUNG_ESE_ONLY)
	trig_nfc_wakeup();
#endif
#endif
	mutex_lock(&device_list_lock);
	p3_dev->device_opened = true;
	memcpy(p3_dev->opened_task, task->comm, TASK_COMM_LEN);
	p3_dev->opened_pid = task->pid;
	P3_INFO_MSG("open(%d, %s)\n", task->pid, task->comm);

#ifdef FEATURE_ESE_WAKELOCK
	wake_lock(&p3_dev->ese_lock);
#endif
	p3_pinctrl_config(p3_dev, true);
	if (!p3_dev->pwr_always_on) {
		ret = p3_regulator_onoff(p3_dev, 1);
		if (ret < 0)
			P3_ERR_MSG("%s : failed to turn on LDO()\n", __func__);
		usleep_range(2000, 2500);
	}

	filp->private_data = p3_dev;

	p3_dev->users++;
	mutex_unlock(&device_list_lock);

	return 0;
}

static int spip3_release(struct inode *inode, struct file *filp)
{
	struct p3_data *p3_dev = filp->private_data;
	struct task_struct *task = current;
	int ret = 0;

	if (!p3_dev->device_opened) {
		P3_ERR_MSG("close(%d, %s) - NOT opened\n", task->pid, task->comm);
		return 0;
	}

	P3_INFO_MSG("close(%d, %s)\n", task->pid, task->comm);
	mutex_lock(&device_list_lock);

#ifdef FEATURE_ESE_WAKELOCK
	if (wake_lock_active(&p3_dev->ese_lock))
		wake_unlock(&p3_dev->ese_lock);
#endif

	filp->private_data = p3_dev;

	p3_dev->users--;
	if (!p3_dev->users) {
		p3_dev->device_opened = false;
		p3_pinctrl_config(p3_dev, false);
		if (!p3_dev->pwr_always_on) {
			ret = p3_regulator_onoff(p3_dev, 0);
			if (ret < 0)
				P3_ERR_MSG("test: failed to turn off LDO()\n");
		}
	}
	usleep_range(10000, 15000);

	mutex_unlock(&device_list_lock);
#ifdef CONFIG_ESE_COLDRESET
#if !IS_ENABLED(CONFIG_SAMSUNG_ESE_ONLY)
	trig_nfc_sleep();
#endif
#endif

	return 0;
}

#ifdef CONFIG_ESE_COLDRESET
#if !IS_ENABLED(CONFIG_SAMSUNG_ESE_ONLY)
extern int trig_cold_reset(void);
#endif
static void p3_power_reset(struct p3_data *data)
{
	/*Add Reset Sequence here*/

	P3_INFO_MSG("%s: start\n", __func__);

#if IS_ENABLED(CONFIG_SAMSUNG_ESE_ONLY)
	if (data->coldrst_type == COLDRST_POWER_ONOFF) {
		p3_regulator_onoff(data, 0);
		msleep(100);
		p3_regulator_onoff(data, 1);
	}
#else
	trig_cold_reset();
#endif
	P3_DBG_MSG("%s: end\n", __func__);

}
#endif /* CONFIG_ESE_COLDRESET */

static long spip3_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	int ret = 0;
	struct p3_data *data = NULL;

	if (_IOC_TYPE(cmd) != P3_MAGIC) {
		P3_ERR_MSG("%s invalid magic. cmd=0x%X Received=0x%X Expected=0x%X\n",
				__func__, cmd, _IOC_TYPE(cmd), P3_MAGIC);
		return -ENOTTY;
	}

	data = filp->private_data;

	mutex_lock(&data->buffer_mutex);
	switch (cmd) {
	case P3_SET_DBG:
		debug_level = (unsigned char)arg;
		P3_INFO_MSG(KERN_INFO"[NXP-P3] -  Debug level %d", debug_level);
		break;
	case P3_ENABLE_SPI_CLK:
		P3_INFO_MSG("%s P3_ENABLE_SPI_CLK\n", __func__);
		break;
	case P3_DISABLE_SPI_CLK:
		P3_INFO_MSG("%s P3_DISABLE_SPI_CLK\n", __func__);
		break;
	case P3_RW_SPI_DATA:
		ret = p3_rw_spi_message(data, arg);
		if (ret < 0)
			P3_ERR_MSG("%s P3_RW_SPI_DATA failed [%d].\n",
					__func__, ret);
		break;
	case P3_SET_PWR:
	case P3_SET_POLL:
	case P3_SET_SPI_CLK:
	case P3_ENABLE_SPI_CS:
	case P3_DISABLE_SPI_CS:
	case P3_ENABLE_CLK_CS:
	case P3_DISABLE_CLK_CS:
	case P3_SWING_CS:
		P3_ERR_MSG("%s deprecated IOCTL:0x%X\n", __func__, cmd);
		break;

#ifdef CONFIG_ESE_COLDRESET
	case P3_WR_RESET:
		P3_DBG_MSG("ese_ioctl (cmd: %d)\n", cmd);
		p3_power_reset(data);
		break;
#endif
	default:
		P3_DBG_MSG("no matching ioctl! 0x%X\n", cmd);
		ret = -EINVAL;
	}
	mutex_unlock(&data->buffer_mutex);

	return ret;
}

static int spip3_transfer(struct p3_data *p3_dev, struct spi_message *m)
{
	int ret;

	if (P3GPIO_IS_VALID(p3_dev->cs_gpio)) {
		gpio_set_value(p3_dev->cs_gpio, 0);
		udelay(20);
	}

	ret = spi_sync(p3_dev->spi, m);

	if (P3GPIO_IS_VALID(p3_dev->cs_gpio))
		gpio_set_value(p3_dev->cs_gpio, 1);

	return ret;
}

static ssize_t spip3_write(struct file *filp, const char *buf, size_t count,
		loff_t *offset)
{
	int ret = -1;
	struct p3_data *p3_dev;
	struct spi_message m;
	struct spi_transfer t;
	unsigned char *tx_buffer;
	unsigned char *rx_buffer;
#ifdef FEATURE_ESE_SPI_DUMMY_ENABLE
	int dummy = 0;
#endif

	p3_dev = filp->private_data;

	if (count > MAX_BUFFER_SIZE) {
		P3_ERR_MSG("%s invalid size\n", __func__);
		return -EMSGSIZE;
	}

	mutex_lock(&p3_dev->buffer_mutex);

	tx_buffer = p3_dev->tx_buffer;
	rx_buffer = p3_dev->rx_buffer;

	memset(tx_buffer, 0, MAX_BUFFER_SIZE);
	memset(rx_buffer, 0, MAX_BUFFER_SIZE);

	if (copy_from_user(&tx_buffer[0], &buf[0], count)) {
		P3_ERR_MSG("%s : failed to copy from user space\n", __func__);
		mutex_unlock(&p3_dev->buffer_mutex);
		return -EFAULT;
	}

#ifdef FEATURE_ESE_SPI_DUMMY_ENABLE
	/* if data size is not aligned, it makes spi clk gap.
	 * and it leads spi read failure.
	 * so, if data size is not aligned, add dummy data.
	 */
	{
		int SPI_MAX_BYTES_PER_WORD = 4;

		if (count % SPI_MAX_BYTES_PER_WORD) {
			dummy = SPI_MAX_BYTES_PER_WORD - count % SPI_MAX_BYTES_PER_WORD;
			if (count + dummy < MAX_BUFFER_SIZE) {
				memset(tx_buffer + count, 0, dummy);
				count +=  dummy;

				P3_INFO_MSG("%s: %d byte dummy is added. count is changed to %zu\n",
						__func__, dummy, count);
			}
		}
	}
#endif

	/* Write data */
	spi_message_init(&m);
	memset(&t, 0, sizeof(t));

	t.tx_buf = tx_buffer;
	t.rx_buf = rx_buffer;
	t.len = count;

	spi_message_add_tail(&t, &m);

	ret = spip3_transfer(p3_dev, &m);

#ifdef FEATURE_ESE_SPI_DUMMY_ENABLE
	count -= dummy;
#endif
	if (ret < 0) {
		P3_ERR_MSG("%s error %d\n", __func__, ret);
		ret = -EIO;
	} else {
		ret = count;
		if (p3_dev->read_1byte_cnt)
			P3_INFO_MSG("w%zu 1r:%u\n", count, p3_dev->read_1byte_cnt);
		else
			P3_INFO_MSG("w%zu\n", count);
	}

	mutex_unlock(&p3_dev->buffer_mutex);

	return ret;
}

static ssize_t spip3_read(struct file *filp, char *buf, size_t count,
		loff_t *offset)
{
	int ret = -EIO;
	struct spi_message m;
	struct spi_transfer t;
	struct p3_data *p3_dev = filp->private_data;
	unsigned char *tx_buffer;
	unsigned char *rx_buffer;

	if (count > MAX_BUFFER_SIZE) {
		P3_ERR_MSG("%s invalid size\n", __func__);
		return -EMSGSIZE;
	}

	mutex_lock(&p3_dev->buffer_mutex);

	tx_buffer = p3_dev->tx_buffer;
	rx_buffer = p3_dev->rx_buffer;

	memset(tx_buffer, 0, MAX_BUFFER_SIZE);
	memset(rx_buffer, 0, MAX_BUFFER_SIZE);

	spi_message_init(&m);
	memset(&t, 0, sizeof(t));

	t.tx_buf = tx_buffer;
	t.rx_buf = rx_buffer;
	t.len = count;

	spi_message_add_tail(&t, &m);

	ret = spip3_transfer(p3_dev, &m);
	if (ret < 0)
		P3_ERR_MSG("%s error %d\n", __func__, ret);

	if (copy_to_user(buf, &rx_buffer[0], count)) {
		P3_ERR_MSG("%s : failed to copy to user space\n", __func__);
		ret = -EFAULT;
		goto fail;
	}
	if (count == 1 && (rx_buffer[0] == 0 || rx_buffer[0] == 0xFF)) {
		p3_dev->read_1byte_cnt++;
	} else {
		if (p3_dev->read_1byte_cnt) {
			P3_INFO_MSG("r%zu t%u\n", count, p3_dev->read_1byte_cnt);
			p3_dev->read_1byte_cnt = 0;
		} else {
			P3_INFO_MSG("r%zu\n", count);
		}
	}
	ret = count;

	mutex_unlock(&p3_dev->buffer_mutex);

	return ret;

fail:
	mutex_unlock(&p3_dev->buffer_mutex);
	return ret;
}

/* possible fops on the p3 device */
static const struct file_operations spip3_dev_fops = {
	.owner = THIS_MODULE,
	.read = spip3_read,
	.write = spip3_write,
	.open = spip3_open,
	.release = spip3_release,
	.unlocked_ioctl = spip3_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = spip3_ioctl,
#endif
};

#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
static struct miscdevice p3_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "p3",
	.fops = &spip3_dev_fops,
};
#endif

static int p3_parse_dt(struct device *dev, struct p3_data *data)
{
	struct device_node *np = dev->of_node;
	const char *coldrst_type_str;
	int ret = 0;

	data->cs_gpio = of_get_named_gpio(np, "ese_p3,cs-gpio", 0);
	P3_INFO_MSG("cs-gpio : %d\n", data->cs_gpio);

	if (!of_property_read_string(np, "ese_p3,coldrst_type", &coldrst_type_str)) {
		if (!strcmp(coldrst_type_str, "gpio"))
			data->coldrst_type = COLDRST_GPIO;
		else if (!strcmp(coldrst_type_str, "power_onoff"))
			data->coldrst_type = COLDRST_POWER_ONOFF;
		else
			data->coldrst_type = COLDRST_NONE;
		P3_INFO_MSG("coldrst type is %d\n", data->coldrst_type);
	} else {
		P3_INFO_MSG("coldrst type is not set\n");
	}
#ifdef CONFIG_ESE_USE_REGULATOR
	data->ese_pvdd = regulator_get(dev, "p3-vdd");
	if (IS_ERR_OR_NULL(data->ese_pvdd)) {
		P3_ERR_MSG("get ese_pvdd error\n");
		data->ese_pvdd = NULL;
	} else
		P3_INFO_MSG("LDO ese_pvdd: %pK\n", data->ese_pvdd);
#else
	P3_INFO_MSG("ESE power follows NFC PVDD\n");
#endif

	data->pwr_always_on = of_property_read_bool(np, "ese_p3,pwr_always_on");

	return ret;
}

static int spip3_probe(struct spi_device *spi)
{
	int ret = -1;
	struct p3_data *data = NULL;
	struct device_node *np = spi->dev.of_node;
	struct property *prop;
	int ese_support = 0;

#ifdef CONFIG_SPI_QCOM_GENI /*SDM845 Only*/
	struct spi_geni_qcom_ctrl_data *delay_params = NULL;
#endif

	nfc_logger_init();

	P3_INFO_MSG("%s chip select : %d , bus number = %d\n",
		__func__, spi->chip_select, spi->master->bus_num);

	/*separate NFC / non NFC using GPIO*/
	prop = of_find_property(np, "check_ese", NULL);
	if (prop) {
		ese_support = gpio_get_value(of_get_named_gpio(np, "check_ese", 0));
		if (ese_support > 0) {
			P3_ERR_MSG("%s : ese support model : %d\n", __func__, ese_support);
		} else {
			P3_ERR_MSG("%s : ese not support model : %d\n", __func__, ese_support);
#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
			misc_deregister(&p3_misc_device);
			P3_ERR_MSG("Misc Deregister\n");
#endif
			return -ENXIO;
		}
	}

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (data == NULL) {
		P3_ERR_MSG("failed to allocate memory for module data\n");
		ret = -ENOMEM;
		goto err_exit;
	}

	ret = p3_parse_dt(&spi->dev, data);
	if (ret) {
		P3_ERR_MSG("%s - Failed to parse DT\n", __func__);
		goto p3_parse_dt_failed;
	}

	if (data->pwr_always_on) {
		ret = p3_regulator_onoff(data, 1);
		if (ret) {
			P3_ERR_MSG("%s - Failed to enable regulator\n", __func__);
			goto p3_parse_dt_failed;
		}
	}

#ifdef CONFIG_SPI_QCOM_GENI /*SDM845 Only*/
	delay_params = spi->controller_data;
	if (spi->controller_data)
		pr_err("%s ctrl data is not empty\n", __func__);
	delay_params = devm_kzalloc(&spi->dev, sizeof(struct spi_geni_qcom_ctrl_data),
			GFP_KERNEL);
	pr_info("%s success alloc ctrl_data!\n", __func__);
	delay_params->spi_cs_clk_delay = 133; /*clock cycles*/
	delay_params->spi_inter_words_delay = 0;
	spi->controller_data = delay_params;
#endif
	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_0;
	spi->max_speed_hz = SPI_DEFAULT_SPEED;
	ret = spi_setup(spi);
	if (ret < 0) {
		P3_ERR_MSG("failed to do spi_setup()\n");
		goto p3_parse_dt_failed;
	}

	data->speed = SPI_DEFAULT_SPEED;
	data->spi = spi;
#ifndef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	data->p3_device.minor = MISC_DYNAMIC_MINOR;
	data->p3_device.name = "p3";
	data->p3_device.fops = &spip3_dev_fops;
	data->p3_device.parent = &spi->dev;
#endif

	dev_set_drvdata(&spi->dev, data);

	/* init mutex and queues */
	init_waitqueue_head(&data->read_wq);
	mutex_init(&data->buffer_mutex);
#ifdef FEATURE_ESE_WAKELOCK
	wake_lock_init(&data->ese_lock,
		WAKE_LOCK_SUSPEND, "ese_wake_lock");
#endif

	data->device_opened = false;

#ifndef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	ret = misc_register(&data->p3_device);
	if (ret < 0) {
		P3_ERR_MSG("misc_register failed! %d\n", ret);
		goto err_misc_regi;
	}
#endif

	if (P3GPIO_IS_VALID(data->cs_gpio)) {
		ret = gpio_request(data->cs_gpio, "ese_cs");
		if (ret)
			P3_ERR_MSG("failed to get gpio cs-gpio\n");
	}

	p3_pinctrl_config(data, false);

	data->tx_buffer = kzalloc(sizeof(unsigned char) * MAX_BUFFER_SIZE, GFP_KERNEL);
	if (data->tx_buffer == NULL) {
		P3_ERR_MSG("failed to allocate spi tx buf\n");
		ret = -EINVAL;
		goto err_alloc_tx_buf;
	}

	data->rx_buffer = kzalloc(sizeof(unsigned char) * MAX_BUFFER_SIZE, GFP_KERNEL);
	if (data->rx_buffer == NULL) {
		P3_ERR_MSG("failed to allocate spi rx buf\n");
		ret = -EINVAL;
		goto err_alloc_rx_buf;
	}

#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	g_p3_dev = data;
#endif
	P3_INFO_MSG("%s finished...\n", __func__);
	return ret;
err_alloc_rx_buf:
	kfree(data->tx_buffer);
err_alloc_tx_buf:
#ifdef FEATURE_ESE_POWER_ON_OFF
err_ldo_off:
#ifndef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	misc_deregister(&data->p3_device);
#endif
#endif

#ifndef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
err_misc_regi:
#endif

#ifdef FEATURE_ESE_WAKELOCK
	wake_lock_destroy(&data->ese_lock);
#endif
	mutex_destroy(&data->buffer_mutex);
p3_parse_dt_failed:
	kfree(data);
err_exit:
	P3_ERR_MSG("ERROR: Exit : %s ret %d\n", __func__, ret);
	return ret;
}

static int __spip3_remove(struct spi_device *spi)
{
	struct p3_data *p3_dev = dev_get_drvdata(&spi->dev);

	P3_DBG_MSG("Entry : %s\n", __func__);
	if (p3_dev == NULL) {
		P3_ERR_MSG("%s p3_dev is null!\n", __func__);
		return 0;
	}

#ifdef FEATURE_ESE_WAKELOCK
	wake_lock_destroy(&p3_dev->ese_lock);
#endif
	mutex_destroy(&p3_dev->buffer_mutex);
#ifndef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	misc_deregister(&p3_dev->p3_device);
#endif
	kfree(p3_dev);
	P3_DBG_MSG("Exit : %s\n", __func__);

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 1, 0)
static int spip3_remove(struct spi_device *spi)
{
	return __spip3_remove(spi);
}
#else
static void spip3_remove(struct spi_device *spi)
{
	__spip3_remove(spi);
}
#endif

#ifdef CONFIG_OF
static const struct of_device_id p3_match_table[] = {
	{ .compatible = "ese_p3",},
	{},
};
#else
#define ese_match_table NULL
#endif

static struct spi_driver spip3_driver = {
	.driver = {
		.name = "p3",
		.bus = &spi_bus_type,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = p3_match_table,
#endif
	},
	.probe =  spip3_probe,
	.remove = spip3_remove,
};

#if IS_MODULE(CONFIG_SAMSUNG_NFC)
#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
static int p3_platform_probe(struct platform_device *pdev)
{
	int ret = -1;

	nfc_logger_init();

	ret = misc_register(&p3_misc_device);
	if (ret < 0)
		P3_INFO_MSG("misc_register failed! %d\n", ret);

	P3_INFO_MSG("%s: finished...\n", __func__);
	return 0;
}

static int p3_platform_remove(struct platform_device *pdev)
{
	P3_INFO_MSG("Entry : %s\n", __func__);
	misc_deregister(&p3_misc_device);

	return 0;
}

static const struct of_device_id p3_platform_match_table[] = {
	{ .compatible = "p3_platform",},
	{},
};

static struct platform_driver p3_platform_driver = {
	.driver = {
		.name = "p3_platform",
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = p3_platform_match_table,
#endif

	},
	.probe =  p3_platform_probe,
	.remove = p3_platform_remove,
};
#endif
int spip3_dev_init(void)
{
	P3_INFO_MSG("Entry : %s\n", __func__);

	if (!ese_param_support) {
		P3_INFO_MSG("%s, support : %d\n", __func__, ese_param_support);
		return 0;
	}
#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	if (platform_driver_register(&p3_platform_driver))
		P3_INFO_MSG("fail to register early\n");
#endif
	return spi_register_driver(&spip3_driver);
}
EXPORT_SYMBOL(spip3_dev_init);

void spip3_dev_exit(void)
{
	P3_INFO_MSG("Entry : %s\n", __func__);

#ifdef CONFIG_MAKE_NODE_USING_PLATFORM_DEVICE
	platform_driver_unregister(&p3_platform_driver);
#endif
	if (ese_param_support)
		spi_unregister_driver(&spip3_driver);
}
EXPORT_SYMBOL(spip3_dev_exit);
#else
static int __init spip3_dev_init(void)
{
	P3_INFO_MSG("Entry : %s\n", __func__);

	if (!ese_param_support) {
		P3_INFO_MSG("%s, support : %d\n", __func__, ese_param_support);
		return 0;
	}

	return spi_register_driver(&spip3_driver);
}

static void __exit spip3_dev_exit(void)
{
	P3_INFO_MSG("Entry : %s\n", __func__);
	if (ese_param_support)
		spi_unregister_driver(&spip3_driver);
}

module_init(spip3_dev_init);
module_exit(spip3_dev_exit);

MODULE_AUTHOR("Sec");
MODULE_DESCRIPTION("ese SPI driver");
MODULE_LICENSE("GPL");
#endif

/** @} */
