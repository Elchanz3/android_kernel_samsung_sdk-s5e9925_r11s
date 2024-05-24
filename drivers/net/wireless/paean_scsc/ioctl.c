/****************************************************************************
 *
 * Copyright (c) 2012 - 2022 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#include "ioctl.h"
#include "debug.h"
#include "mlme.h"
#include "mgt.h"
#include "cac.h"
#include "hip.h"
#include "netif.h"
#include "cfg80211_ops.h"
#include <net/netlink.h>
#include <linux/netdevice.h>
#include <linux/ieee80211.h>
#include <linux/inet.h>
#include <linux/igmp.h>
#include "mib.h"
#include <paean_scsc/scsc_mx.h>
#include <paean_scsc/scsc_log_collector.h>
#include "dev.h"
#include "fapi.h"
#include <paean_scsc/scsc_warn.h>
#include <linux/string.h>

#define CMD_RXFILTERADD         "RXFILTER-ADD"
#define CMD_RXFILTERREMOVE              "RXFILTER-REMOVE"
#define CMD_RXFILTERSTART               "RXFILTER-START"
#define CMD_RXFILTERSTOP                "RXFILTER-STOP"
#define CMD_SETCOUNTRYREV               "SETCOUNTRYREV"
#define CMD_GETCOUNTRYREV               "GETCOUNTRYREV"
#define CMD_SETROAMTRIGGER_LEGACY       "SETROAMTRIGGER_LEGACY"
#define CMD_GETROAMTRIGGER_LEGACY       "GETROAMTRIGGER_LEGACY"
#define CMD_REASSOC_LEGACY              "REASSOC_LEGACY"
#define CMD_REASSOC_FREQUENCY_LEGACY    "REASSOC_FREQUENCY_LEGACY"
#define CMD_SETROAMTRIGGER              "SETROAMTRIGGER"
#define CMD_GETROAMTRIGGER              "GETROAMTRIGGER"
#define CMD_ADDROAMSCANCHANNELS_LEGACY  "ADDROAMSCANCHANNELS_LEGACY"
#define CMD_ADDROAMSCANFREQUENCIES_LEGACY   "ADDROAMSCANFREQUENCIES_LEGACY"
#define CMD_SETSUSPENDMODE              "SETSUSPENDMODE"
#define CMD_SETROAMDELTA                "SETROAMDELTA"
#define CMD_GETROAMDELTA                "GETROAMDELTA"
#define CMD_SETROAMSCANPERIOD           "SETROAMSCANPERIOD"
#define CMD_GETROAMSCANPERIOD           "GETROAMSCANPERIOD"
#define CMD_SETFULLROAMSCANPERIOD               "SETFULLROAMSCANPERIOD"
#define CMD_GETFULLROAMSCANPERIOD               "GETFULLROAMSCANPERIOD"
#define CMD_SETSCANCHANNELTIME          "SETSCANCHANNELTIME"
#define CMD_GETSCANCHANNELTIME          "GETSCANCHANNELTIME"
#define CMD_SETSCANNPROBES              "SETSCANNPROBES"
#define CMD_GETSCANNPROBES              "GETSCANNPROBES"
#define CMD_SETROAMMODE         "SETROAMMODE"
#define CMD_GETROAMMODE         "GETROAMMODE"
#define CMD_SETROAMINTRABAND            "SETROAMINTRABAND"
#define CMD_GETROAMINTRABAND            "GETROAMINTRABAND"
#define CMD_SETROAMBAND         "SETROAMBAND"
#define CMD_GETROAMBAND         "GETROAMBAND"
#define CMD_SETROAMSCANCONTROL          "SETROAMSCANCONTROL"
#define CMD_GETROAMSCANCONTROL          "GETROAMSCANCONTROL"
#define CMD_SETSCANHOMETIME             "SETSCANHOMETIME"
#define CMD_GETSCANHOMETIME             "GETSCANHOMETIME"
#define CMD_SETSCANHOMEAWAYTIME         "SETSCANHOMEAWAYTIME"
#define CMD_GETSCANHOMEAWAYTIME         "GETSCANHOMEAWAYTIME"
#define CMD_SETSCANHOMEAWAYTIME_LEGACY  "SETSCANHOMEAWAYTIME_LEGACY"
#define CMD_SETSCANHOMETIME_LEGACY      "SETSCANHOMETIME_LEGACY"
#define CMD_SETSCANCHANNELTIME_LEGACY   "SETSCANCHANNELTIME_LEGACY"
#define CMD_SETSCANPASSIVETIME_LEGACY   "SETSCANPASSIVETIME_LEGACY"
#define CMD_SET_TID                     "SET_TID"
#define CMD_SETOKCMODE          "SETOKCMODE"
#define CMD_GETOKCMODE          "GETOKCMODE"
#define CMD_SETWESMODE          "SETWESMODE"
#define CMD_GETWESMODE          "GETWESMODE"
#define CMD_SET_PMK             "SET_PMK"
#define CMD_HAPD_GET_CHANNEL			"HAPD_GET_CHANNEL"
#define CMD_SET_SAP_CHANNEL_LIST                "SET_SAP_CHANNEL_LIST"
#define CMD_REASSOC             "REASSOC"
#define CMD_REASSOC_FREQUENCY   "REASSOC_FREQUENCY"
#define CMD_SETROAMSCANCHANNELS         "SETROAMSCANCHANNELS"
#define CMD_SETROAMSCANFREQUENCIES      "SETROAMSCANFREQUENCIES"
#define CMD_GETROAMSCANCHANNELS         "GETROAMSCANCHANNELS"
#define CMD_GETROAMSCANFREQUENCIES      "GETROAMSCANFREQUENCIES"
#define CMD_GETROAMSCANCHANNELS_LEGACY  "GETROAMSCANCHANNELS_LEGACY"
#define CMD_SETWTCMODE                  "SETWTCMODE"
#define CMD_GETROAMSCANFREQUENCIES_LEGACY   "GETROAMSCANFREQUENCIES_LEGACY"
#define CMD_ADDROAMSCANCHANNELS         "ADDROAMSCANCHANNELS"
#define CMD_ADDROAMSCANFREQUENCIES      "ADDROAMSCANFREQUENCIES"
#define CMD_SENDACTIONFRAME             "SENDACTIONFRAME"
#define CMD_CERTSENDACTIONFRAME         "CERTSENDACTIONFRAME"
#ifdef SLSI_TEST_DEV
#define CMD_GASSENDACTIONFRAME          "GASSENDACTIONFRAME"
#endif
#define CMD_GETNCHOMODE                 "GETNCHOMODE"
#define CMD_SETNCHOMODE                 "SETNCHOMODE"
#define CMD_GETDFSSCANMODE                 "GETDFSSCANMODE"
#define CMD_SETDFSSCANMODE                 "SETDFSSCANMODE"
#define CMD_HAPD_MAX_NUM_STA            "HAPD_MAX_NUM_STA"
#define CMD_HAPD_SET_AX_MODE            "HAPD_SET_AX_MODE"
#define CMD_COUNTRY            "COUNTRY"
#define CMD_SEND_GK                               "SEND_GK"
#define CMD_SETAPP2PWPSIE "SET_AP_P2P_WPS_IE"
#define CMD_P2PSETPS "P2P_SET_PS"
#define CMD_P2PSETNOA "P2P_SET_NOA"
#define CMD_P2PECSA "P2P_ECSA"
#define CMD_P2PLOSTART "P2P_LO_START"
#define CMD_P2PLOSTOP "P2P_LO_STOP"
#define CMD_TDLSCHANNELSWITCH  "TDLS_CHANNEL_SWITCH"
#define CMD_SETROAMOFFLOAD     "SETROAMOFFLOAD"
#define CMD_SETROAMOFFLAPLIST  "SETROAMOFFLAPLIST"
#ifdef CONFIG_SCSC_WLAN_LOW_LATENCY_MODE
#define CMD_SET_LATENCY_MODE "SET_LATENCY_MODE"
#define CMD_SET_POWER_MGMT "SET_POWER_MGMT"
#endif
#define CMD_SET_LATENCY_CRT_DATA "SET_LATENCY_CRT_DATA"
#define CMD_SET_DISCONNECT_IES "SET_DISCONNECT_IES"
#define CMD_SETBANDWIDTH "SETBANDWIDTH"
#define CMD_SET_BSS_CHANNEL_WIDTH "SET_BSS_CHANNEL_WIDTH"

#define CMD_SETBAND "SETBAND"
#define CMD_GETBAND "GETBAND"
#define CMD_FACTORY_SETBAND "FACTORY_SETBAND"
#define CMD_SET_FCC_CHANNEL "SET_FCC_CHANNEL"

#define CMD_FAKEMAC "FAKEMAC"

#define CMD_GETBSSRSSI "GET_BSS_RSSI"
#define CMD_GETBSSINFO "GETBSSINFO"
#define CMD_GETSTAINFO "GETSTAINFO"
#define CMD_GETASSOCREJECTINFO "GETASSOCREJECTINFO"
#define CMD_SET_DWELL_TIME "SET_DWELL_TIME"

#if defined(CONFIG_SLSI_WLAN_STA_FWD_BEACON) && (defined(SCSC_SEP_VERSION) && SCSC_SEP_VERSION >= 10)
#define CMD_BEACON_RECV "BEACON_RECV"
#endif
#ifdef CONFIG_SCSC_WLAN_STA_ENHANCED_ARP_DETECT
#define CMD_SET_ENHANCED_ARP_TARGET "SET_ENHANCED_ARP_TARGET"
#define CMD_GET_ENHANCED_ARP_COUNTS "GET_ENHANCED_ARP_COUNTS"
#endif

/* Known commands from framework for which no handlers */
#define CMD_AMPDU_MPDU "AMPDU_MPDU"
#define CMD_BTCOEXMODE "BTCOEXMODE"
#define CMD_BTCOEXSCAN_START "BTCOEXSCAN-START"
#define CMD_BTCOEXSCAN_STOP "BTCOEXSCAN-STOP"
#define CMD_CHANGE_RL "CHANGE_RL"
#define CMD_INTERFACE_CREATE "INTERFACE_CREATE"
#define CMD_INTERFACE_DELETE "INTERFACE_DELETE"
#define CMD_SET_INDOOR_CHANNELS "SET_INDOOR_CHANNELS"
#define CMD_GET_INDOOR_CHANNELS "GET_INDOOR_CHANNELS"
#define CMD_LTECOEX "LTECOEX"
#define CMD_MIRACAST "MIRACAST"
#define CMD_RESTORE_RL "RESTORE_RL"
#define CMD_RPSMODE "RPSMODE"
#define CMD_SETCCXMODE "SETCCXMODE"
#define CMD_SETDFSSCANMODE "SETDFSSCANMODE"
#define CMD_SETJOINPREFER "SETJOINPREFER"
#define CMD_SETSINGLEANT "SETSINGLEANT"
#define CMD_SET_TX_POWER_CALLING "SET_TX_POWER_CALLING"
#define CMD_GET_CU "GET_CU"

#define CMD_DRIVERDEBUGDUMP "DEBUG_DUMP"
#define CMD_DRIVERDEBUGCOMMAND "DEBUG_COMMAND"
#define CMD_TESTFORCEHANG "SLSI_TEST_FORCE_HANG"
#define CMD_GETREGULATORY "GETREGULATORY"

#define CMD_SET_TX_POWER_SAR "SET_TX_POWER_SAR"
#define CMD_GET_TX_POWER_SAR "GET_TX_POWER_SAR"
#define CMD_SET_TX_POWER_SUB6_BAND "SET_TX_POWER_SUB6_BAND"

#define CMD_POWER_MEASUREMENT_START "POWER_MEASUREMENT_START"

#ifdef CONFIG_SCSC_WLAN_ENHANCED_PKT_FILTER
#define CMD_ENHANCED_PKT_FILTER "ENHANCED_PKT_FILTER"
#endif

#ifdef CONFIG_SCSC_WLAN_MAX_LINK_SPEED
#define CMD_GET_MAX_LINK_SPEED "GET_MAX_LINK_SPEED"
#endif

#ifdef CONFIG_SCSC_WLAN_NUM_ANTENNAS
#define CMD_SET_NUM_ANTENNAS "SET_NUM_ANTENNAS"
#define CMD_GET_NUM_ANTENNAS "GET_NUM_ANTENNAS"
#endif

#ifdef CONFIG_SCSC_WLAN_DYNAMIC_ITO
#define CMD_SET_ITO "SET_ITO"
#define CMD_ENABLE_ITO "ENABLE_ITO"
#endif

#define CMD_ELNA_BYPASS              "ELNA_BYPASS"
#define CMD_ELNA_BYPASS_INT          "ELNA_BYPASS_INT"
#define CMD_MAX_DTIM_IN_SUSPEND      "MAX_DTIM_IN_SUSPEND"
#define CMD_SET_DTIM_IN_SUSPEND      "SET_DTIM_IN_SUSPEND"
#define CMD_FORCE_ROAMING_BSSID      "FORCE_ROAMING_BSSID"
#define CMD_ROAMING_BLACKLIST_ADD    "ROAMING_BLACKLIST_ADD"
#define CMD_ROAMING_BLACKLIST_REMOVE "ROAMING_BLACKLIST_REMOVE"

#define CMD_GET_PS_DISABLED_DURATION "GET_PS_DISABLED_DURATION"
#define CMD_GET_PS_ENTRY_COUNTER     "GET_PS_ENTRY_COUNTER"

#define CMD_TWT_SETUP                "TWT_SETUP"
#define CMD_TWT_TEARDOWN             "TWT_TEARDOWN"
#define CMD_TWT_SUSPEND              "TWT_INFO_FRM"
#define CMD_GET_TWT_STATUS           "GET_TWT_STATUS"
#define CMD_GET_TWT_CAP              "GET_TWT_CAP"
#define CMD_GET_TWT_STATISTICS       "GET_TWT_STATISTICS"
#define CMD_CLEAR_TWT_STATISTICS     "CLEAR_TWT_STATISTICS"
#define CMD_ADPS_ENABLE              "ADPS_ENABLE"
#ifdef CONFIG_SCSC_WLAN_SAP_POWER_SAVE
#define CMD_SET_AP_SUSPEND			"SET_AP_SUSPEND"
#define CMD_SET_AP_RPS_PARAMS		"SET_AP_RPS_PARAMS"
#define CMD_SET_AP_RPS				"SET_AP_RPS"
#define CMD_SET_HOSTAPD_TX_POWER	"SET_HOSTAPD_TX_POWER"
#define CMD_GET_HOSTAPD_TX_POWER	"GET_HOSTAPD_TX_POWER"
#endif

#define CMD_SCHED_PM_SETUP           "SCHED_PM_SETUP"
#define CMD_SCHED_PM_TEARDOWN        "SCHED_PM_TEARDOWN"
#define CMD_GET_SCHED_PM_STATUS      "GET_SCHED_PM_STATUS"

#define CMD_SET_DELAYED_WAKEUP       "SET_DELAYED_WAKEUP"
#define CMD_SET_DELAYED_WAKEUP_TYPE  "SET_DELAYED_WAKEUP_TYPE"

#define CMD_LEAKY_AP_ACTIVE_DETECTION        "LEAKY_AP_ACTIVE_DETECTION"
#define CMD_LEAKY_AP_PASSIVE_DETECTION_START "LEAKY_AP_PASSIVE_DETECTION_START"
#define CMD_LEAKY_AP_PASSIVE_DETECTION_END   "LEAKY_AP_PASSIVE_DETECTION_END"
#define CMD_SET_GRACE_PERIOD                 "SET_GRACE_PERIOD"

#define CMD_GET_TDLS_MAX_SESSION	"GET_TDLS_MAX_SESSION"
#define CMD_GET_TDLS_NUM_OF_SESSION "GET_TDLS_NUM_OF_SESSION"
#define CMD_GET_TDLS_WIDER_BW       "GET_TDLS_WIDER_BW"
#define CMD_GET_TDLS_AVAILABLE      "GET_TDLS_AVAILABLE"
#define CMD_SET_TDLS_ENABLED        "SET_TDLS_ENABLED"

#define CMD_TWT_SOFTAP_ENABLE "TWT_SOFTAP_ENABLE"

#define CMD_HAPD_TWT_TEARDOWN "HAPD_TWT_TEARDOWN"

#define CMD_HAPD_GET_TWT_STATUS "HAPD_GET_TWT_STATUS"

#define CMD_SET_ELNABYPASS_THRESHOLD "SET_ELNABYPASS_THRESHOLD"
#define CMD_GET_ELNA_STATUS "GET_ELNA_STATUS"
#define CMD_GET_TX_ANT_CONFIG   "GET_TX_ANT_CONFIG"
#define CMD_SET_TX_ANT_CONFIG   "SET_TX_ANT_CONFIG"
#define CMD_GET_STA_DUMP "GET_STA_DUMP"

#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
#define CMD_GET_WIFI6E_CHANNELS "GET_WIFI6E_CHANNELS"
#define CMD_SETWSECINFO "SETWSECINFO"
#endif

#define ROAMOFFLAPLIST_MIN 1
#define ROAMOFFLAPLIST_MAX 100

#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
#define SLSI_MAX_BAND_VALUE SLSI_FREQ_BAND_2_4GHZ_6GHZ
#define SLSI_MAX_ROAMBAND_VALUE FAPI_BAND_2_4GHZ_5GHZ_6GHZ
#else
#define SLSI_MAX_BAND_VALUE SLSI_FREQ_BAND_2GHZ
#define SLSI_MAX_ROAMBAND_VALUE FAPI_BAND_2_4GHZ_5GHZ
#endif

#define TWT_D_MIN_WAKE_DURATION 4000
#define TWT_D_MAX_WAKE_DURATION 63000

#define TWT_D_MIN_WAKE_INTERVAL 8000
#define TWT_D_MAX_WAKE_INTERVAL 1000000

#define SLSI_SCHED_PM_MIN_DESIRED_DURATION 8000
#define SLSI_SCHED_PM_MAX_DESIRED_DURATION 63000

#define SLSI_SCHED_PM_MIN_DESIRED_INTERVAL 16000
#define SLSI_SCHED_PM_MAX_DESIRED_INTERVAL 1000000

#define SLSI_SCHED_PM_MIN_ADDITIONAL_DURATION 0
#define SLSI_SCHED_PM_MAX_ADDITIONAL_DURATION 6144

#define SLSI_SCHED_PM_LEAKY_AP_ACTIVE_DETECTION_MIN_DURATION 3072
#define SLSI_SCHED_PM_LEAKY_AP_ACTIVE_DETECTION_MAX_DURATION 10240

#define SLSI_SCHED_PM_MIN_GRACE_PERIOD 0
#define SLSI_SCHED_PM_MAX_GRACE_PERIOD 6144

enum slsi_scheduled_pm_error_code {
	SLSI_SCHED_PM_TRANSIENT_ERROR = -5,
	SLSI_SCHED_PM_INVALID_ARGUMENT = -10,
	SLSI_SCHED_PM_TEMPORARILY_BUSY = -15,
	SLSI_SCHED_PM_NOT_SUPPORTED_REQUEST = -20,
	SLSI_SCHED_PM_NOT_AVAILABLE_TO_PROCESS_CMD = -25
};

#ifdef CONFIG_SCSC_WLAN_SAP_POWER_SAVE
#define AP_RPS_PHASE_MIN 1
#define AP_RPS_PHASE_MAX 10
#define AP_TX_POWER_MIN 0
#define AP_TX_POWER_MAX 31
#endif

struct slsi_ioctl_args *slsi_get_private_command_args(char *buffer, int buf_len, int max_arg_count)
{
	struct slsi_ioctl_args *ioctl_args = NULL;
	char *pos                          = buffer;

	ioctl_args = kmalloc(sizeof(*ioctl_args) + sizeof(u8 *) * max_arg_count, GFP_KERNEL);
	if (!ioctl_args)
		return NULL;
	memset(ioctl_args->args, '\0', sizeof(u8 *) * max_arg_count);

	ioctl_args->arg_count = 0;
	while (buf_len > 0 && ioctl_args->arg_count < max_arg_count) {
		pos = strchr(pos, ' ');
		if (!pos)
			break;
		buf_len = buf_len - (pos - buffer + 1);
		if (buf_len <= 0)
			break;
		*pos = '\0';
		pos++;
		while (*pos == ' ')
			pos++;
		buffer = pos;
		ioctl_args->args[ioctl_args->arg_count++] = pos;
	}
	return ioctl_args;
}

int slsi_verify_ioctl_args(struct slsi_dev *sdev, struct slsi_ioctl_args *ioctl_args)
{
	if (!ioctl_args) {
		SLSI_ERR(sdev, "Malloc of ioctl_args failed.\n");
		return -ENOMEM;
	}

	if (!ioctl_args->arg_count) {
		kfree(ioctl_args);
		return -EINVAL;
	}

	return 0;
}

static int slsi_parse_hex(unsigned char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

void slsi_convert_space_seperation(char *buf, int buf_len)
{
	int i = 0;

	while (buf[i] != '\0' && i < buf_len) {
		if (buf[i] == ',' || buf[i] == '=')
			buf[i] = ' ';
		i++;
	}
}

static int slsi_is_bw_valid(char *str)
{
	if ((slsi_str_cmp(str, "20") == 0) || (slsi_str_cmp(str, "40") == 0) ||
	    (slsi_str_cmp(str, "80") == 0))
		return 1;

	return 0;
}

static void slsi_machexstring_to_macarray(char *mac_str, u8 *mac_arr)
{
	mac_arr[0] = slsi_parse_hex(mac_str[0]) << 4 | slsi_parse_hex(mac_str[1]);
	mac_arr[1] = slsi_parse_hex(mac_str[3]) << 4 | slsi_parse_hex(mac_str[4]);
	mac_arr[2] = slsi_parse_hex(mac_str[6]) << 4 | slsi_parse_hex(mac_str[7]);
	mac_arr[3] = slsi_parse_hex(mac_str[9]) << 4 | slsi_parse_hex(mac_str[10]);
	mac_arr[4] = slsi_parse_hex(mac_str[12]) << 4 | slsi_parse_hex(mac_str[13]);
	mac_arr[5] = slsi_parse_hex(mac_str[15]) << 4 | slsi_parse_hex(mac_str[16]);
}

static int slsi_get_rcl_freq_list(struct slsi_dev *sdev, struct sk_buff *skb, int *freq_list)
{
	int    i = 7; /* 1byte (id) + 1byte(length) + 3byte (oui) + 2byte */
	int    ie_len = 0, sig_data_len = 0;
	u8     *ptr;
	u16    freq_val = 0;
	__le16 *le16_ptr = NULL;
	u32    freq_count = 0;

	SLSI_DBG3(sdev, SLSI_MLME, "RCL Channel List Indication received\n");
	ptr =  fapi_get_data(skb);
	sig_data_len = fapi_get_datalen(skb);
	if (sig_data_len)
		ie_len = ptr[1];

	while (i < sig_data_len) {
		le16_ptr = (__le16 *)&ptr[i];
		freq_val = le16_to_cpu(*le16_ptr);
		freq_list[freq_count] = freq_val / 2;

		if (freq_list[freq_count] < SLSI_2P4GHZ_MIN_FREQ || freq_list[freq_count] > SLSI_6GHZ_MAX_FREQ) {
			SLSI_ERR(sdev, "ERR: Invalid freq received %d\n", freq_list[freq_count]);
			kfree_skb(skb);
			return 0;
		}

		i += SLSI_SCAN_CHANNEL_DESCRIPTOR_SIZE;
		freq_count += 1;
		if (freq_count >= MAX_FREQUENCY_COUNT) {
			SLSI_ERR(sdev, "ERR: Frequency list received >= %d\n", MAX_FREQUENCY_COUNT);
			kfree_skb(skb);
			return 0;
		}
		if (i >= ie_len && i < sig_data_len - 7) {
			ie_len = ptr[i + 1];
			i += 7;
		}
	}
	kfree_skb(skb);
	return freq_count;
}

static int slsi_get_rcl_channel_list(struct slsi_dev *sdev, struct sk_buff *skb, u16 *channel_list)
{
	u32    channel_count = 0;
	int    i = 7; /* 1byte (id) + 1byte(length) + 3byte (oui) + 2byte */
	int    ie_len = 0;
	u8     *ptr;
	u16    channel_val = 0;
	__le16 *le16_ptr = NULL;

	SLSI_DBG3(sdev, SLSI_MLME, "RCL Channel List Indication received\n");
	ptr =  fapi_get_data(skb);
	ie_len = ptr[1];
	while (i < ie_len) {
		le16_ptr = (__le16 *)&ptr[i];
		channel_val = le16_to_cpu(*le16_ptr);
		channel_list[channel_count] = ieee80211_frequency_to_channel(channel_val / 2);
		if (channel_list[channel_count] < 1 || channel_list[channel_count] > 196) {
			SLSI_ERR(sdev, "ERR: Invalid channel received %d\n", channel_list[channel_count]);
			kfree_skb(skb);
			return 0;
		}
		i += SLSI_SCAN_CHANNEL_DESCRIPTOR_SIZE;
		channel_count += 1;
		if (channel_count >= MAX_CHANNEL_COUNT) {
			SLSI_ERR(sdev, "ERR: Channel list received >= %d\n", MAX_CHANNEL_COUNT);
			kfree_skb(skb);
			return 0;
		}
	}
	kfree_skb(skb);
	return channel_count;
}

void slsi_update_multicast_addr(struct slsi_dev *sdev, struct net_device *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct in_device *in_dev = NULL;
	struct ip_mc_list *im = NULL;
	static __be32 multicast_ip_list[65] = {0};
	int size = 0, i = 0, ip_found = 0;

	WLBT_WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));
	WLBT_WARN_ON(ndev_vif->vif_type != FAPI_VIFTYPE_STATION);
	in_dev = __in_dev_get_rtnl(dev);
	if (!in_dev)
		return;

	for (im = rtnl_dereference(in_dev->mc_list); im != NULL; im = rtnl_dereference(im->next_rcu)) {
		ip_found = 0;
		for (i = 0; i < size; i++) {
			if (!memcmp(&im->multiaddr, &multicast_ip_list[i], sizeof(__be32))) {
				ip_found = 1;
				break;
			}
		}
		if (!ip_found) {
			memcpy(&multicast_ip_list[size], &im->multiaddr, sizeof(__be32));
			size++;
		}
		if (size >= 65)
			break;
	}
	slsi_mlme_set_multicast_ip(sdev, dev, multicast_ip_list, size);
}

static int slsi_set_suspend_mode(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *netdev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = netdev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               user_suspend_mode;
	int               previous_suspend_mode;
	u16               host_state;
	int               ret = 0;
	int vif;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	user_suspend_mode = *ioctl_args->args[0] - '0';
	if (user_suspend_mode != 0 && user_suspend_mode != 1) {
		SLSI_ERR(sdev, "Invalid value of user_suspend_mode %d\n", user_suspend_mode);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	previous_suspend_mode = sdev->device_config.user_suspend_mode;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	if (user_suspend_mode != previous_suspend_mode) {
		SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
		for (vif = 1; vif <= CONFIG_SCSC_WLAN_MAX_INTERFACES; vif++) {
			struct net_device *dev = slsi_get_netdev_locked(sdev, vif);
			struct netdev_vif *ndev_vif;

			if (!dev)
				continue;

			ndev_vif = netdev_priv(dev);
			if (!user_suspend_mode)
				cancel_work_sync(&ndev_vif->update_pkt_filter_work);

			SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
			if (ndev_vif->activated &&
			    ndev_vif->vif_type == FAPI_VIFTYPE_STATION &&
			    ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED) {
				if (user_suspend_mode) {
					ret = slsi_update_packet_filters(sdev, dev);
					if (sdev->igmp_offload_activated)
						slsi_update_multicast_addr(sdev, dev);
				} else {
					ret = slsi_clear_packet_filters(sdev, dev);
				}
				if (ret != 0)
					SLSI_NET_ERR(dev, "Error in updating /clearing the packet filters,ret=%d", ret);
			}

			SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		}
		SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
	} else {
		SLSI_NET_INFO(dev, "Current suspend mode (%d) and requested mode(%d) are same\n", previous_suspend_mode, user_suspend_mode);
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	sdev->device_config.user_suspend_mode = user_suspend_mode;
	host_state = sdev->device_config.host_state;

	if (!sdev->device_config.user_suspend_mode)
		host_state = host_state | SLSI_HOSTSTATE_LCD_ACTIVE;
	else
		host_state = host_state & ~SLSI_HOSTSTATE_LCD_ACTIVE;
	sdev->device_config.host_state = host_state;

	ret = slsi_mlme_set_host_state(sdev, dev, host_state);
	if (ret != 0)
		SLSI_NET_ERR(dev, "Error in setting the Host State, ret=%d", ret);

	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_set_p2p_oppps(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	unsigned int      ct_param;
	unsigned int      legacy_ps;
	unsigned int      opp_ps;
	int               result = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 3);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (ioctl_args->arg_count < 3) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		result = -EINVAL;
		goto exit;
	}

	/* The NOA param shall be added only after P2P-VIF is active */
	if (!ndev_vif->activated || ndev_vif->iftype != NL80211_IFTYPE_P2P_GO) {
		SLSI_ERR_NODEV("P2P GO vif not activated\n");
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &legacy_ps)) {
		SLSI_ERR(sdev, "legacy_ps: failed to read from string: '%s'\n", ioctl_args->args[0]);
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[1], &opp_ps)) {
		SLSI_ERR(sdev, "opp_ps: failed to read from string: '%s'\n", ioctl_args->args[1]);
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[2], &ct_param)) {
		SLSI_ERR(sdev, "ct_param: failed to read from string: '%s'\n", ioctl_args->args[2]);
		result = -EINVAL;
		goto exit;
	}

	if (opp_ps == 0)
		result = slsi_mlme_set_ctwindow(sdev, dev, opp_ps);
	else if (ct_param < (unsigned int)ndev_vif->ap.beacon_interval)
		result = slsi_mlme_set_ctwindow(sdev, dev, ct_param);
	else
		SLSI_DBG1(sdev, SLSI_CFG80211, "p2p ct window = %d is out of range for beacon interval(%d)\n", ct_param, ndev_vif->ap.beacon_interval);
exit:
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return result;
}

static int slsi_p2p_set_noa_params(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                  result = 0;
	unsigned int         noa_count;
	unsigned int         duration;
	unsigned int         interval;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 3);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (ioctl_args->arg_count < 3) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		result = -EINVAL;
		goto exit;
	}

	/* The NOA param shall be added only after P2P-VIF is active */
	if (!ndev_vif->activated || ndev_vif->iftype != NL80211_IFTYPE_P2P_GO) {
		SLSI_ERR_NODEV("P2P GO vif not activated\n");
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &noa_count)) {
		SLSI_ERR(sdev, "noa_count: failed to read string: '%s'\n", ioctl_args->args[0]);
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[1], &interval)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[1]);
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[2], &duration)) {
		SLSI_ERR(sdev, "duration: failed to read string: '%s'\n", ioctl_args->args[2]);
		result = -EINVAL;
		goto exit;
	}

	/* Skip start time */
	result = slsi_mlme_set_p2p_noa(sdev, dev, noa_count, interval, duration);

exit:
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return result;
}

static int slsi_p2p_ecsa(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct netdev_vif *group_dev_vif;
	struct net_device *group_dev = NULL;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                  result = 0;
	unsigned int         channel;
	unsigned int         bandwidth;
	u16 center_freq = 0;
	u16 chan_info = 0;
	struct cfg80211_chan_def chandef;
	enum nl80211_band band;
	enum nl80211_channel_type chan_type = NL80211_CHAN_NO_HT;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (ioctl_args->arg_count < 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	group_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX_SWLAN);
	if (!group_dev) {
		SLSI_INFO(sdev, "No Group net_dev found\n");
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &channel)) {
		SLSI_ERR(sdev, "channel: failed to read string: '%s'\n", ioctl_args->args[0]);
		result = -EINVAL;
		goto exit;
	}

	if (channel < 1 || channel > SLSI_5GHZ_LAST_CHAN) {
		SLSI_ERR(sdev, "Invalid channel : %d\n", channel);
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[1], &bandwidth)) {
		SLSI_ERR(sdev, "bandwidth: failed to read string: '%s'\n", ioctl_args->args[1]);
		result = -EINVAL;
		goto exit;
	}

	band = (channel <= 14) ? NL80211_BAND_2GHZ : NL80211_BAND_5GHZ;
	center_freq = ieee80211_channel_to_frequency(channel, band);
	SLSI_DBG1(sdev, SLSI_CFG80211, "p2p ecsa_params (center_freq)= (%d)\n", center_freq);
	chandef.chan = ieee80211_get_channel(sdev->wiphy, center_freq);
	chandef.width = (band  == NL80211_BAND_2GHZ) ? NL80211_CHAN_WIDTH_20_NOHT : NL80211_CHAN_WIDTH_80;

#ifndef SSB_4963_FIXED
	/* Default HT40 configuration */
	if (sdev->band_5g_supported) {
		if (bandwidth == 80) {
			chandef.width = NL80211_CHAN_WIDTH_40;
			bandwidth = 40;
			if (channel == 36 || channel == 44 || channel == 149 || channel == 157)
				chan_type = NL80211_CHAN_HT40PLUS;
			else
				chan_type = NL80211_CHAN_HT40MINUS;
		}
	}
#endif
	if (channel == 165 && bandwidth != 20) {
		bandwidth = 20;
		chan_type = NL80211_CHAN_HT20;
	}
	cfg80211_chandef_create(&chandef, chandef.chan, chan_type);
	chan_info = slsi_get_chann_info(sdev, &chandef);
	if (bandwidth != 20)
		center_freq = slsi_get_center_freq1(sdev, chan_info, center_freq);
	group_dev_vif = netdev_priv(group_dev);
	SLSI_MUTEX_LOCK(group_dev_vif->vif_mutex);
	result = slsi_mlme_channel_switch(sdev, group_dev, center_freq, chan_info);
	SLSI_MUTEX_UNLOCK(group_dev_vif->vif_mutex);

exit:
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return result;
}

static ssize_t slsi_ap_vendor_ies_write(struct slsi_dev *sdev, struct net_device *dev, u8 *ie,
					size_t ie_len, u16 purpose)
{
	u8                *vendor_ie = NULL;
	int               result = 0;
	struct netdev_vif *ndev_vif;

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	/* During AP start before mlme_start_req, supplicant calls set_ap_wps_ie() to send the vendor IEs for each
	 * beacon, probe response and association response. As we get all of them in mlme_start_req, ignoring the
	 * same which comes before adding GO VIF
	 */
	if (!ndev_vif->activated) {
		SLSI_DBG1(sdev, SLSI_CFG80211, "vif not activated\n");
		result = 0;
		goto exit;
	}
	if (!(ndev_vif->iftype == NL80211_IFTYPE_P2P_GO || ndev_vif->iftype == NL80211_IFTYPE_AP)) {
		SLSI_ERR(sdev, "Not AP or P2P interface. interfaceType:%d\n", ndev_vif->iftype);
		result = -EINVAL;
		goto exit;
	}

	vendor_ie = kmalloc(ie_len, GFP_KERNEL);
	if (!vendor_ie) {
		SLSI_ERR(sdev, "kmalloc failed\n");
		result = -ENOMEM;
		goto exit;
	}
	memcpy(vendor_ie, ie, ie_len);

	slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	result = slsi_ap_prepare_add_info_ies(ndev_vif, vendor_ie, ie_len);

	if (result == 0)
		result = slsi_mlme_add_info_elements(sdev, dev, purpose, ndev_vif->ap.add_info_ies, ndev_vif->ap.add_info_ies_len);

	slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	kfree(vendor_ie);

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return result;
}

static int slsi_set_ap_p2p_wps_ie(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               result = 0;
	u8                *params;
	int               offset = 0;
	enum if_type {
		IF_TYPE_NONE,
		IF_TYPE_P2P_DEVICE,
		IF_TYPE_AP_P2P
	} iftype = IF_TYPE_NONE;
	enum frame_type {
		FRAME_TYPE_NONE,
		FRAME_TYPE_BEACON,
		FRAME_TYPE_PROBE_RESPONSE,
		FRAME_TYPE_ASSOC_RESPONSE
	} frametype = FRAME_TYPE_NONE;
	int params_len = buf_len - strlen(CMD_SETAPP2PWPSIE) - 1;

	params = command + strlen(CMD_SETAPP2PWPSIE) + 1;
	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (ioctl_args->arg_count < 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], (int *)&frametype)) {
		SLSI_ERR(sdev, "Failed to read frame type string: '%s'\n", ioctl_args->args[0]);
		result = -EINVAL;
		goto exit;
	}
	offset = offset + strlen(ioctl_args->args[0]) + 1;

	if (!slsi_str_to_int(ioctl_args->args[1], (int *)&iftype)) {
		SLSI_ERR(sdev, "Failed to read iftype string: '%s'\n", ioctl_args->args[1]);
		result = -EINVAL;
		goto exit;
	}
	offset = offset + strlen(ioctl_args->args[1]) + 1;
	params_len = params_len - offset;

	SLSI_NET_DBG2(dev, SLSI_NETDEV,
		      "command=%s, frametype=%d, iftype=%d, total buf_len=%d, params_len=%d\n",
		      command, frametype, iftype, buf_len, params_len);

	/* check the net device interface type */
	if (iftype == IF_TYPE_P2P_DEVICE) {
		u8                *probe_resp_ie = NULL; /* params+offset; */

		if (frametype != FRAME_TYPE_PROBE_RESPONSE) {
			SLSI_NET_ERR(dev, "Wrong frame type received\n");
			result = -EINVAL;
			goto exit;
		}
		probe_resp_ie = kmalloc(params_len, GFP_KERNEL);
		if (!probe_resp_ie) {
			SLSI_ERR(sdev, "Malloc for IEs failed\n");
			kfree(ioctl_args);
			return -ENOMEM;
		}

		memcpy(probe_resp_ie, params + offset, params_len);

		result = slsi_p2p_dev_probe_rsp_ie(sdev, dev, probe_resp_ie, params_len);
	} else if (iftype == IF_TYPE_AP_P2P) {
		if (frametype == FRAME_TYPE_BEACON)
			result = slsi_ap_vendor_ies_write(sdev, dev, params + offset, params_len, FAPI_PURPOSE_BEACON);
		else if (frametype == FRAME_TYPE_PROBE_RESPONSE)
			result = slsi_ap_vendor_ies_write(sdev, dev, params + offset, params_len, FAPI_PURPOSE_PROBE_RESPONSE);
		else if (frametype == FRAME_TYPE_ASSOC_RESPONSE)
			result = slsi_ap_vendor_ies_write(sdev, dev, params + offset, params_len, FAPI_PURPOSE_ASSOCIATION_RESPONSE);
	}
exit:
	kfree(ioctl_args);
	return result;
}

/**
 * P2P_LO_START handling.
 * Add unsync vif, register for action frames and set the listen channel.
 * The probe response IEs would be configured later.
 */
static int slsi_p2p_lo_start(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct ieee80211_channel *chan = NULL;
	struct slsi_ioctl_args *ioctl_args = NULL;
	unsigned int channel, duration, interval, count;
	int  ret = 0;
	int  freq;
	enum nl80211_band band;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 4);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (ioctl_args->arg_count < 4) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	/* Reject LO if other operations are in progress. Back to back LO can be received.
	 * In such a case, if state is Listening then the listen offload flag should be true else
	 * reject the request as the Listening state would then be due to ROC.
	 */
	if (sdev->p2p_state == P2P_SCANNING || sdev->p2p_state > P2P_LISTENING ||
	    (sdev->p2p_state == P2P_LISTENING && !ndev_vif->unsync.listen_offload)) {
		SLSI_NET_ERR(dev, "Reject LO due to ongoing P2P operation (state: %s)\n", slsi_p2p_state_text(sdev->p2p_state));
		ret = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &channel)) {
		SLSI_ERR(sdev, "channel: failed to read string: '%s'\n", ioctl_args->args[0]);
		ret = -EINVAL;
		goto exit;
	}

	if (channel < 1 || channel > SLSI_5GHZ_LAST_CHAN) {
		SLSI_ERR(sdev, "Invalid channel : %d\n", channel);
		ret = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[1], &duration)) {
		SLSI_ERR(sdev, "duration: failed to read string: '%s'\n", ioctl_args->args[1]);
		ret = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[2], &interval)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[2]);
		ret = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[3], &count)) {
		SLSI_ERR(sdev, "count: failed to read string: '%s'\n", ioctl_args->args[3]);
		ret = -EINVAL;
		goto exit;
	}

	if (!ndev_vif->activated) {
		ret = slsi_mlme_add_vif(sdev, dev, dev->dev_addr, dev->dev_addr);
		if (ret != 0) {
			SLSI_NET_ERR(dev, "Unsync vif addition failed\n");
			goto exit;
		}

		ndev_vif->activated = true;
		ndev_vif->mgmt_tx_data.exp_frame = SLSI_PA_INVALID;
		SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_VIF_ACTIVE);

		ret = slsi_mlme_register_action_frame(sdev, dev, SLSI_ACTION_FRAME_PUBLIC, SLSI_ACTION_FRAME_PUBLIC);
		if (ret != 0) {
			SLSI_NET_ERR(dev, "Action frame registration for unsync vif failed\n");
			goto exit_with_vif_deactivate;
		}
	}

	/* Send set_channel irrespective of the values of LO parameters as they are not cached
	 * in driver to check whether they have changed.
	 */
	band = (channel <= 14) ? NL80211_BAND_2GHZ : NL80211_BAND_5GHZ;
	freq = ieee80211_channel_to_frequency(channel, band);
	chan = ieee80211_get_channel(sdev->wiphy, freq);
	if (!chan) {
		SLSI_NET_ERR(dev, "Incorrect channel: %u - Listen Offload failed\n", channel);
		ret = -EINVAL;
		goto exit_with_vif_deactivate;
	}

	ret = slsi_mlme_set_channel(sdev, dev, chan, duration, interval, count);
	if (ret != 0) {
		SLSI_NET_ERR(dev, "Set channel for unsync vif failed\n");
		goto exit_with_vif_deactivate;
	} else {
		ndev_vif->chan = chan;
		ndev_vif->driver_channel = chan->hw_value;
	}
	/* If framework sends the values for listen offload as 1,500,5000 and 6,
	 * where 5000ms (5 seconds) is the listen interval which needs to be repeated
	 * 6 times(i.e. count). Hence listen_end_ind comes after 30 seconds
	 * (6 * 5000 = 30000ms) Hence host should wait 31 seconds to delete the
	 * unsync VIF for one such P2P listen offload request.
	 */
	slsi_p2p_queue_unsync_vif_del_work(ndev_vif, interval * count + 1000);
	ndev_vif->unsync.listen_offload = true;
	SLSI_P2P_STATE_CHANGE(ndev_vif->sdev, P2P_LISTENING);
	goto exit;

exit_with_vif_deactivate:
	slsi_p2p_vif_deactivate(sdev, dev, true);
exit:
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

/**
 * P2P_LO_STOP handling.
 * Clear listen offload flag.
 * Delete the P2P unsynchronized vif.
 */
static int slsi_p2p_lo_stop(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	WLBT_WARN_ON((!ndev_vif->unsync.listen_offload) || (ndev_vif->sdev->p2p_state != P2P_LISTENING));

	ndev_vif->unsync.listen_offload = false;

	/* Deactivating the p2p unsynchronized vif */
	if (ndev_vif->sdev->p2p_state == P2P_LISTENING)
		slsi_p2p_vif_deactivate(ndev_vif->sdev, ndev_vif->wdev.netdev, true);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return 0;
}

static int slsi_rx_filter_add(struct net_device *dev, char *buffer, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               ret = 0;
	int               filter_num = 0;

	ioctl_args = slsi_get_private_command_args(buffer, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	filter_num = *ioctl_args->args[0] - '0';
	if (filter_num < 0 || filter_num > 3) {
		SLSI_ERR(sdev, "Invalid value of filter_num %d\n", filter_num);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	sdev->device_config.rx_filter_num = filter_num;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return ret;
}

static int slsi_rx_filter_remove(struct net_device *dev, char *buffer, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	sdev->device_config.rx_filter_num = 0;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	return 0;
}

#ifdef CONFIG_SCSC_WLAN_WIFI_SHARING
#if !defined(CONFIG_SCSC_WLAN_MHS_STATIC_INTERFACE) || (defined(SCSC_SEP_VERSION) && SCSC_SEP_VERSION < 9)
static int slsi_create_interface(struct net_device *dev, char *buffer, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	struct net_device *ap_dev;
	char *intf_name = NULL;
	bool is_cfg80211 = true;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(buffer, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	intf_name = ioctl_args->args[0];
	if (strcmp(CONFIG_SCSC_AP_INTERFACE_NAME, intf_name) != 0) {
		SLSI_NET_ERR(dev, "Creation of %s not allowed!\n", intf_name);
		kfree(ioctl_args);
		return -EINVAL;
	}
	ap_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX_SWLAN);
	if (ap_dev && (strcmp(ap_dev->name, intf_name) == 0)) {
		SLSI_NET_ERR(dev, "%s already created\n", intf_name);
		kfree(ioctl_args);
		return -EINVAL;
	}

	ap_dev = slsi_dynamic_interface_create(sdev->wiphy, intf_name, NL80211_IFTYPE_AP, NULL, is_cfg80211);
	if (ap_dev) {
		sdev->netdev_ap = ap_dev;
		kfree(ioctl_args);
		return 0;
	}

	SLSI_NET_ERR(dev, "Failed to create AP interface %s\n", intf_name);
	kfree(ioctl_args);
	return -EINVAL;
}

static int slsi_delete_interface(struct net_device *dev, char *buffer, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	char *intf_name = NULL;
	bool is_cfg80211 = false;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(buffer, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	intf_name = ioctl_args->args[0];
	if (strcmp(CONFIG_SCSC_AP_INTERFACE_NAME, intf_name) != 0) {
		SLSI_NET_ERR(dev, "Deletion of %s not allowed!\n", intf_name);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (strcmp(intf_name, CONFIG_SCSC_AP_INTERFACE_NAME) == 0)
		dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX_SWLAN);

	if (!dev) {
		SLSI_WARN(sdev, "AP dev is NULL");
		kfree(ioctl_args);
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	if (ndev_vif->activated)
		slsi_stop_net_dev(sdev, dev);
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
	slsi_netif_remove_rtlnl_locked(sdev, dev, is_cfg80211);

	sdev->netdev_ap = NULL;
	SLSI_DBG1_NODEV(SLSI_MLME, "Successfully deleted AP interface %s ", intf_name);

	kfree(ioctl_args);
	return 0;
}
#endif

static int slsi_get_indoor_channels(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	char op[150] = "";
	char int_string[30] = "";
	int i;
	int len = 0;

	SLSI_DBG1_NODEV(SLSI_MLME, "GET_INDOOR_CHANNELS : %d\n", sdev->num_5g_restricted_channels);

	for (i = 0; i < sdev->num_5g_restricted_channels; i++) {
		sprintf(int_string, "%d", sdev->wifi_sharing_5g_restricted_channels[i]);
		strcat(op, int_string);
		strcat(op, " ");
	}

	len = snprintf(command, buf_len, "%d %s", sdev->num_5g_restricted_channels, op);

	return len;
}
#endif

static int slsi_legacy_roam_trigger_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               trigger_value = 0;
	int               ret = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &trigger_value)) {
		SLSI_ERR(sdev, "Invalid trigger_value: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (trigger_value > -50 || trigger_value < -100) {
		SLSI_ERR(sdev, "Invalid trigger_value: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
#endif

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EPERM;
	}
	ret = slsi_mlme_set_roaming_parameters(sdev, dev, SLSI_PSID_UNIFI_ROAM_RSSI_SCAN_TRIGGER, trigger_value, 1);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	kfree(ioctl_args);
	return ret;
}

static int slsi_legacy_roam_scan_trigger_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res = 0;

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
#endif

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_RSSI_SCAN_TRIGGER, &mib_value);
	if (res)
		return res;
	res = snprintf(command, buf_len, "%s %d", CMD_GETROAMTRIGGER, mib_value);
	return res;
}

static int slsi_roam_add_scan_frequencies_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	int                    result = 0;
	int                    i, j;
	int                    curr_channel_count = 0;
	int                    found = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;
	const u8               *connected_ssid = NULL;
	u32                    network_map_channels_count = 0;
	u16                    network_map_channels[SLSI_ROAMING_CHANNELS_MAX];
	u16                    merged_channels[SLSI_ROAMING_CHANNELS_MAX * 2];
	u32                    merge_chan_count = 0;
	int		       new_freq_count = 0;
	int 		       new_freqs[SLSI_MAX_FREQUENCY_LIST] = {0};

	ioctl_args = slsi_get_private_command_args(command, buf_len, 21);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_STATION ||
	    ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "Not a STA vif or status is not CONNECTED\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EPERM;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
#endif

	if (sdev->device_config.legacy_roam_scan_list.n == SLSI_MAX_CHANNEL_LIST) {
		SLSI_ERR(sdev, "Roam scan list is already full\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	/* Adding Connected channel to legacy_roam_scan_list */
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (sdev->device_config.legacy_roam_scan_list.n == 0) {
		sdev->device_config.legacy_roam_scan_list.n = 1;
		sdev->device_config.legacy_roam_scan_list.channels[0] =
			SLSI_CONVERT_CH_TO_CACHED_CH(ndev_vif->chan->hw_value, ndev_vif->chan->band);
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &new_freq_count)) {
		SLSI_ERR(sdev, "Invalid freq_count string: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (new_freq_count < 1 || new_freq_count > SLSI_MAX_FREQUENCY_LIST ||
	    (ioctl_args->arg_count - 1) < new_freq_count) {
		SLSI_ERR(sdev, "Invalid value of freq_count %d\n", new_freq_count);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	curr_channel_count = sdev->device_config.legacy_roam_scan_list.n;
	for (i = 0; i < new_freq_count; i++) {
		struct ieee80211_channel *chan = NULL;
		u16 new_channel;

		if (!slsi_str_to_int(ioctl_args->args[i + 1], &new_freqs[i])) {
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}

		chan = ieee80211_get_channel(sdev->wiphy, new_freqs[i]);
		if (!chan) {
			SLSI_ERR(sdev, "Invalid frequency : %d\n", new_freqs[i]);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		new_channel = SLSI_CONVERT_CH_TO_CACHED_CH(chan->hw_value, chan->band);
		for (j = 0; j < curr_channel_count; j++) {
			found = 0;
			if (sdev->device_config.legacy_roam_scan_list.channels[j] == new_channel) {
				found = 1;
				break;
			}
		}
		if (!found) {
			sdev->device_config.legacy_roam_scan_list.channels[curr_channel_count] = new_channel;
			curr_channel_count++;
		}
		if (curr_channel_count >= SLSI_MAX_CHANNEL_LIST) {
			curr_channel_count = SLSI_MAX_CHANNEL_LIST;
			break;
		}
	}

	sdev->device_config.legacy_roam_scan_list.n = curr_channel_count;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	connected_ssid = cfg80211_find_ie(WLAN_EID_SSID, ndev_vif->sta.sta_bss->ies->data,
					  ndev_vif->sta.sta_bss->ies->len);

	network_map_channels_count = slsi_roaming_scan_configure_channels(sdev, dev, connected_ssid, network_map_channels);
	merge_chan_count = slsi_merge_lists(network_map_channels, network_map_channels_count,
					    sdev->device_config.legacy_roam_scan_list.channels,
					    sdev->device_config.legacy_roam_scan_list.n,
					    merged_channels);

	result = slsi_mlme_set_cached_channels(sdev, dev, merge_chan_count, merged_channels);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return result;
}

static int slsi_roam_add_scan_channels_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	int                    result = 0;
	int                    i, j, new_channel_count = 0;
	int                    new_channels[SLSI_MAX_CHANNEL_LIST];
	int                    curr_channel_count = 0;
	int                    found = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;
	const u8               *connected_ssid = NULL;
	u32                    network_map_channels_count = 0;
	u16                    network_map_channels[SLSI_ROAMING_CHANNELS_MAX];
	u16                    merged_channels[SLSI_ROAMING_CHANNELS_MAX * 2];
	u32                    merge_chan_count = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 21);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_STATION ||
	    ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "Not a STA vif or status is not CONNECTED\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EPERM;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled.\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
#endif

	if (sdev->device_config.legacy_roam_scan_list.n == SLSI_MAX_CHANNEL_LIST) {
		SLSI_ERR(sdev, "Roam scan list is already full\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	/* Adding Connected channel to legacy_roam_scan_list */
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (sdev->device_config.legacy_roam_scan_list.n == 0 && ndev_vif->chan) {
		sdev->device_config.legacy_roam_scan_list.n = 1;
		sdev->device_config.legacy_roam_scan_list.channels[0] = ndev_vif->chan->hw_value;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	if (!slsi_str_to_int(ioctl_args->args[0], &new_channel_count)) {
		SLSI_ERR(sdev, "Invalid channel_count string: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (new_channel_count < 1 || new_channel_count > SLSI_MAX_CHANNEL_LIST || (ioctl_args->arg_count - 1) < new_channel_count) {
		SLSI_ERR(sdev, "Invalid value of channel_count %d\n", new_channel_count);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	curr_channel_count = sdev->device_config.legacy_roam_scan_list.n;

	for (i = 0; i < new_channel_count; i++) {
		if (!slsi_str_to_int(ioctl_args->args[i + 1], &new_channels[i])) {
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		if (new_channels[i] < 1 || new_channels[i] > SLSI_5GHZ_LAST_CHAN) {
			SLSI_ERR(sdev, "Invalid channel : %d\n", new_channels[i]);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		for (j = 0; j < curr_channel_count; j++) {
			found = 0;
			if (sdev->device_config.legacy_roam_scan_list.channels[j] == new_channels[i]) {
				found = 1;
				break;
			}
		}
		if (!found) {
			sdev->device_config.legacy_roam_scan_list.channels[curr_channel_count] = new_channels[i];
			curr_channel_count++;
		}
		if (curr_channel_count >= SLSI_MAX_CHANNEL_LIST) {
			curr_channel_count = SLSI_MAX_CHANNEL_LIST;
			break;
		}
	}

	sdev->device_config.legacy_roam_scan_list.n = curr_channel_count;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	connected_ssid = cfg80211_find_ie(WLAN_EID_SSID, ndev_vif->sta.sta_bss->ies->data,
					  ndev_vif->sta.sta_bss->ies->len);

	network_map_channels_count = slsi_roaming_scan_configure_channels(sdev, dev, connected_ssid, network_map_channels);
	merge_chan_count = slsi_merge_lists(network_map_channels, network_map_channels_count,
					    sdev->device_config.legacy_roam_scan_list.channels,
					    sdev->device_config.legacy_roam_scan_list.n,
					    merged_channels);

	result = slsi_mlme_set_cached_channels(sdev, dev, merge_chan_count, merged_channels);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return result;
}

static int slsi_roam_scan_frequencies_read_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               freq_list[MAX_FREQUENCY_COUNT] = {0};
	struct sk_buff    *ind = NULL;
	int               pos = 0;
	int               i;
	int               freq_count = 0;

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
#endif
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "STA is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		freq_count = 0;
		goto output;
	}

	ind = slsi_mlme_roaming_channel_list_req(sdev, dev);
	if (!ind) {
		SLSI_ERR(sdev, "RCL ind is NULL!\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	freq_count = slsi_get_rcl_freq_list(sdev, ind, freq_list);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (!freq_count) {
		SLSI_ERR(sdev, "Frequency count is 0!\n");
		return -EINVAL;
	}

output:
	pos = scnprintf(command, buf_len, "%s %d", CMD_GETROAMSCANFREQUENCIES_LEGACY, freq_count);
	for (i = 0; i < freq_count; i++)
		pos += scnprintf(command + pos, buf_len - pos, " %d", freq_list[i]);

	return pos;
}

static int slsi_roam_scan_channels_read_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	u16               channel_list[MAX_CHANNEL_COUNT] = {0};
	struct sk_buff    *ind = NULL;
	int               pos = 0;
	int               i;
	int               channel_count = 0;

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
#endif
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "STA is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		channel_count = 0;
		goto output;
	}

	ind = slsi_mlme_roaming_channel_list_req(sdev, dev);
	if (!ind) {
		SLSI_ERR(sdev, "RCL ind is NULL!\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	channel_count = slsi_get_rcl_channel_list(sdev, ind, channel_list);
	if (!channel_count) {
		SLSI_ERR(sdev, "Channel count is 0!");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}
output:
	pos = scnprintf(command, buf_len, "%s %d", CMD_GETROAMSCANCHANNELS, channel_count);
	for (i = 0; i < channel_count; i++)
		pos += scnprintf(command + pos, buf_len - pos, " %d", channel_list[i]);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return pos;
}

static int slsi_set_wtc_mode(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif       *ndev_vif   = netdev_priv(dev);
	struct slsi_dev         *sdev       = ndev_vif->sdev;
	struct slsi_ioctl_args  *ioctl_args = NULL;
	int                     wtc_mode = 2;
	int                     scan_mode = 0;
	int                     rssi = 0;
	int                     rssi_th_2g = 0;
	int                     rssi_th_5g = 0;
	int                     rssi_th_6g = 0;
	int                     result = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 6);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (ioctl_args->arg_count < 6) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_STATION ||
	    ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "Not a STA vif or status is not CONNECTED\n");
		result = -EPERM;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &wtc_mode)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[0]);
		result = -EINVAL;
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &scan_mode)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[1]);
		result = -EINVAL;
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[2], &rssi)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[2]);
		result = -EINVAL;
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[3], &rssi_th_2g)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[3]);
		result = -EINVAL;
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[4], &rssi_th_5g)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[4]);
		result = -EINVAL;
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[5], &rssi_th_6g)) {
		SLSI_ERR(sdev, "interval: failed to read string: '%s'\n", ioctl_args->args[5]);
		result = -EINVAL;
		goto exit;
	}
	if (wtc_mode < 0 || wtc_mode > 255) {
		SLSI_NET_ERR(dev, "Incorrect wtc mode: %d\n", wtc_mode);
		result = -EINVAL;
		goto exit;
	}
	if (scan_mode < 0 || scan_mode > 2) {
		SLSI_NET_ERR(dev, "Incorrect scan mode: %d\n", scan_mode);
		result = -EINVAL;
		goto exit;
	}
	if (rssi > 0 || rssi_th_2g > 0 || rssi_th_5g > 0 || rssi_th_6g > 0) {
		SLSI_NET_ERR(dev, "Incorrect rssi value\n");
		result = -EINVAL;
		goto exit;
	}

	result = slsi_mlme_wtc_mode_req(sdev, dev, wtc_mode, scan_mode, rssi, rssi_th_2g, rssi_th_5g, rssi_th_6g);

exit:
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return result;
}

static int slsi_reassoc_frequency_write_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif   *ndev_vif = netdev_priv(dev);
	struct slsi_dev     *sdev = ndev_vif->sdev;
	u8                  bssid[6] = { 0 };
	int                 freq;
	int                 r = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled.\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
#endif
	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[0]));
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);

	if (!slsi_str_to_int(ioctl_args->args[1], &freq)) {
		SLSI_ERR(sdev, "Invalid frequency string: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	r = slsi_mlme_roam(sdev, dev, bssid, freq);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return r;
}

static int slsi_reassoc_write_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif   *ndev_vif = netdev_priv(dev);
	struct slsi_dev     *sdev = ndev_vif->sdev;
	u8                  bssid[6] = { 0 };
	int                 channel;
	int                 freq;
	enum nl80211_band band = NL80211_BAND_2GHZ;
	int                 r = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is enabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
#endif
	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[0]));
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);
	if (!slsi_str_to_int(ioctl_args->args[1], &channel)) {
		SLSI_ERR(sdev, "Invalid channel string: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (channel < 1 || channel > SLSI_5GHZ_LAST_CHAN) {
		SLSI_ERR(sdev, "Invalid channel : %d\n", channel);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (channel > 14)
		band = NL80211_BAND_5GHZ;
	freq = (u16)ieee80211_channel_to_frequency(channel, band);

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	r = slsi_mlme_roam(sdev, dev, bssid, freq);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return r;
}

static int slsi_set_country_rev(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	char              alpha2_rev[] = {0, 0, 0, 0};
	int               status = 0;
	char *country_code = NULL;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	status = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (status)
		return status;

	country_code = ioctl_args->args[0];
	memcpy(alpha2_rev, country_code, strlen(country_code) < 4 ? strlen(country_code) : 4);

	status = slsi_set_country_update_regd(sdev, alpha2_rev, strlen(country_code) < 4 ? strlen(country_code) : 4);

	kfree(ioctl_args);
	return status;
}

static int slsi_get_country_rev(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	u8                buf[5];
	int               len = 0;

	memset(buf, 0, sizeof(buf));

	len = snprintf(command, buf_len, "%s %c%c %d", CMD_GETCOUNTRYREV,
		       sdev->device_config.domain_info.regdomain->alpha2[0],
		       sdev->device_config.domain_info.regdomain->alpha2[1],
		       sdev->device_config.domain_info.regdomain->dfs_region);

	return len;
}

static int slsi_ioctl_set_roam_band(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	uint roam_band = 0;
	int ret = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	roam_band = *ioctl_args->args[0] - '0';
	if (roam_band > SLSI_MAX_ROAMBAND_VALUE) {
		SLSI_ERR(sdev, "Invalid value for Roam Band %d\n", roam_band);
		kfree(ioctl_args);
		return -EINVAL;
	}
	/* If SETROAMBAND 0 is received, then roam band will be set to 2GHZ,
	 * if 5GHZ supported then 2GHZ+5GHZ, if 6GHZ supported then 2GHZ+5GHZ+6GHZ
	 */
	if (roam_band == FAPI_BAND_AUTO) {
		roam_band = FAPI_BAND_2_4GHZ;
		if (sdev->band_5g_supported)
			roam_band |= FAPI_BAND_5GHZ;
#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
		if (sdev->band_6g_supported)
			roam_band |= FAPI_BAND_6GHZ;
#endif
	}

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (sdev->device_config.supported_roam_band == roam_band) {
		SLSI_DBG1_NODEV(SLSI_MLME, "roam band is already %d\n", roam_band);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return ret;
	}

	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_set_band_req(sdev, dev, roam_band, 1);
	if (ret == -EIO) {
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return ret;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	sdev->device_config.supported_roam_band = roam_band;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	kfree(ioctl_args);
	return ret;
#else
	SLSI_ERR(sdev, "NCHO is not supported\n");

	kfree(ioctl_args);
	return -EINVAL;
#endif
}

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
uint slsi_band_to_fw_band_maping(uint band)
{
	switch (band) {
	case SLSI_FREQ_BAND_AUTO:
		return FAPI_BAND_2_4GHZ_5GHZ;
	case SLSI_FREQ_BAND_5GHZ:
		return FAPI_BAND_5GHZ;
	case SLSI_FREQ_BAND_2GHZ:
		return FAPI_BAND_2_4GHZ;
	case SLSI_FREQ_BAND_2_4GHZ_5GHZ_6GHZ:
		return  FAPI_BAND_2_4GHZ_5GHZ_6GHZ;
	case SLSI_FREQ_BAND_6GHZ:
		return FAPI_BAND_6GHZ;
	case SLSI_FREQ_BAND_5GHZ_6GHZ:
		return FAPI_BAND_5GHZ_6GHZ;
	case SLSI_FREQ_BAND_2_4GHZ_6GHZ:
		return FAPI_BAND_2_4GHZ_6GHZ;
	default:
		return -1;
	}
}
#endif

static int slsi_ioctl_set_band(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	uint band = 0;
	uint internal_band = 0; /* used for driver and firmware mapping */
	int ret = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	band = *ioctl_args->args[0] - '0';
	if (band > SLSI_MAX_BAND_VALUE) {
		SLSI_ERR(sdev, "Invalid value for Band %d\n", band);
		kfree(ioctl_args);
		return -EINVAL;
	}

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (sdev->device_config.supported_band == band) {
		SLSI_DBG1_NODEV(SLSI_MLME, "band is already %d\n", band);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return ret;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	internal_band = slsi_band_to_fw_band_maping(band);
	if (internal_band < 0) {
		SLSI_ERR(sdev, "Invalid value for Band = %d\n", band);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_set_band_req(sdev, dev, internal_band, 0);
	if (ret == -EIO) {
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return ret;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	sdev->device_config.supported_band = band;
	slsi_band_cfg_update(sdev, band);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return ret;
#else
	SLSI_ERR(sdev, "NCHO is not supported\n");

	kfree(ioctl_args);
	return -EINVAL;
#endif
}
static int slsi_ioctl_get_roam_band(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               pos = 0;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	if (slsi_is_test_mode_enabled() || sdev->device_config.ncho_mode) {
#else
	if (sdev->device_config.ncho_mode) {
#endif
		pos = scnprintf(command, buf_len, "%s %d", CMD_GETROAMBAND, sdev->device_config.supported_roam_band);
	} else {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		pos = -EINVAL;
	}

	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	return pos;
}

static int slsi_ioctl_get_band(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               pos = 0;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	if (slsi_is_test_mode_enabled() || sdev->device_config.ncho_mode) {
#else
	if (sdev->device_config.ncho_mode) {
#endif
		pos = scnprintf(command, buf_len, "%s %d", CMD_GETBAND, sdev->device_config.supported_band);
	} else {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		pos = -EINVAL;
	}

	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	return pos;
}

static int slsi_factory_freq_band_write(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	uint band = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	band = *ioctl_args->args[0] - '0';
	if (band > 2) {
		SLSI_ERR(sdev, "Invalid value : Band Must be 0/1/2 band %d\n", band);
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_band_update(sdev, band);
	/* Convert to correct Mib value (intra_band:1, all_band:2) */
	kfree(ioctl_args);
	return slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_SCAN_BAND, (band == SLSI_FREQ_BAND_AUTO) ? 2 : 1);
}

#ifdef CONFIG_SCSC_WLAN_WES_NCHO
static int slsi_roam_scan_trigger_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               mib_value = 0;
	int               ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mib_value > -50 || mib_value < -100) {
		SLSI_ERR(sdev, "Invalid trigger_value: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_set_roaming_parameters(sdev, dev, SLSI_PSID_UNIFI_ROAM_NCHO_RSSI_TRIGGER, mib_value, 1);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_roam_scan_trigger_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_RSSI_TRIGGER, &mib_value);
	if (res)
		return res;
	res = snprintf(command, buf_len, "%s %d", CMD_GETROAMTRIGGER, mib_value);
	return res;
}

static int slsi_roam_delta_trigger_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               mib_value = 0;
	int               ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mib_value > 100 || mib_value < 0) {
		SLSI_ERR(sdev, "Invalid delta trigger_value: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_set_roaming_parameters(sdev, dev, SLSI_PSID_UNIFI_ROAM_NCHO_RSSI_DELTA, mib_value, 1);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_roam_delta_trigger_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_RSSI_DELTA, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETROAMDELTA, mib_value);
	return res;
}

static int slsi_reassoc_frequency_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif   *ndev_vif = netdev_priv(dev);
	struct slsi_dev     *sdev = ndev_vif->sdev;
	u8                  bssid[6] = { 0 };
	int                 freq = 0;
	int                 r = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	if (ioctl_args->arg_count != 2) {
		SLSI_ERR(sdev, "Not enough arguments\n");
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[0]));
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);

	if (!slsi_str_to_int(ioctl_args->args[1], &freq)) {
		SLSI_ERR(sdev, "Invalid freq string: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	r = slsi_mlme_roam(sdev, dev, bssid, freq);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	kfree(ioctl_args);
	return r;
}

static int slsi_reassoc_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif   *ndev_vif = netdev_priv(dev);
	struct slsi_dev     *sdev = ndev_vif->sdev;
	u8                  bssid[6] = { 0 };
	int                 channel = 0;
	int                 freq = 0;
	enum nl80211_band band = NL80211_BAND_2GHZ;
	int                 r = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	if (ioctl_args->arg_count != 2) {
		SLSI_ERR(sdev, "Not enough arguments\n");
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[0]));
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);

	if (!slsi_str_to_int(ioctl_args->args[1], &channel)) {
		SLSI_ERR(sdev, "Invalid channel string: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (channel < 1 || channel > SLSI_5GHZ_LAST_CHAN) {
		SLSI_ERR(sdev, "Invalid channel : %d\n", channel);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (channel > 14)
		band = NL80211_BAND_5GHZ;
	freq = (u16)ieee80211_channel_to_frequency(channel, band);

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	r = slsi_mlme_roam(sdev, dev, bssid, freq);

	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

static int slsi_cached_channel_scan_period_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               mib_value = 0;
	int               ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mib_value > 60 || mib_value < 0) {
		SLSI_ERR(sdev, "Invalid roam scan period: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_set_roaming_parameters(sdev, dev, SLSI_PSID_UNIFI_ROAM_NCHO_CACHED_SCAN_PERIOD, mib_value * 1000000, 4);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_cached_channel_scan_period_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_CACHED_SCAN_PERIOD, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETROAMSCANPERIOD, mib_value / 1000000);

	return res;
}

static int slsi_full_roam_scan_period_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               mib_value = 0;
	int               ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mib_value > 600 || mib_value < 0) {
		SLSI_ERR(sdev, "Invalid full roam scan period: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_set_roaming_parameters(sdev, dev, SLSI_PSID_UNIFI_ROAM_NCHO_FULL_SCAN_PERIOD, mib_value * 1000000, 4);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_full_roam_scan_period_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_FULL_SCAN_PERIOD, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETFULLROAMSCANPERIOD, mib_value / 1000000);

	return res;
}

static int slsi_roam_scan_max_active_channel_time_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mib_value > 300 || mib_value < 3) {
		SLSI_ERR(sdev, "Invalid scan channel time: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	return slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_MAX_ACTIVE_CHANNEL_TIME, mib_value);
}

static int slsi_roam_scan_max_active_channel_time_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_MAX_ACTIVE_CHANNEL_TIME, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETSCANCHANNELTIME, mib_value);

	return res;
}

static int slsi_roam_scan_probe_interval_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mib_value > 10 || mib_value < 1) {
		SLSI_ERR(sdev, "Invalid scan probes: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	return slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_NPROBE, mib_value);
}

static int slsi_roam_scan_probe_interval_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_NPROBE, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETSCANNPROBES, mib_value);

	return res;
}

static int slsi_roam_mode_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (slsi_is_rf_test_mode_enabled()) {
		SLSI_DBG1_NODEV(SLSI_MLME, "SLSI_PSID_UNIFI_ROAM_MODE is not supported because of rf test mode.\n");
		kfree(ioctl_args);
		return -EOPNOTSUPP;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mib_value < 0 || mib_value > 2) {
		SLSI_ERR(sdev, "Invalid roam mode value: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	return slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_MODE, mib_value);
}

static int slsi_roam_mode_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_MODE, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETROAMMODE, mib_value);

	return res;
}

static int slsi_roam_offload_ap_list(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif          *ndev_vif   = netdev_priv(dev);
	struct slsi_dev            *sdev       = ndev_vif->sdev;
	struct cfg80211_acl_data   *mac_acl;
	struct slsi_ioctl_args     *ioctl_args = NULL;
	int                        ap_count    = 0;
	int                        i, r;
	int                        malloc_len;

	slsi_convert_space_seperation(command, buf_len);
	ioctl_args = slsi_get_private_command_args(command, buf_len, 101);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

	/* command format:
	 *     x,aa:bb:cc:dd:ee:ff,xx:yy:zz:qq:ww:ee...
	 *     x = 1 to 100
	 *     each mac address id 17 bytes and every mac address is separated by ','
	 */
	if (!slsi_str_to_int(ioctl_args->args[0], &ap_count)) {
			SLSI_ERR(sdev, "Invalid ap_count string: '%s'\n", ioctl_args->args[0]);
			kfree(ioctl_args);
			return -EINVAL;
	}
	if (ap_count < ROAMOFFLAPLIST_MIN || ap_count > ROAMOFFLAPLIST_MAX) {
		SLSI_ERR(sdev, "Invalid ap_count: %d\n", ap_count);
		kfree(ioctl_args);
		return -EINVAL;
	}
	/* each mac address takes 18 bytes(17 for mac address and 1 for ',') except the last one.
	 * the last mac address is just 17 bytes(without a coma)
	 */
	if ((ioctl_args->arg_count - 1) < ap_count) {
		SLSI_ERR(sdev, "Buffer doesn't have enough fields ap_count: %d\n", ap_count);
		kfree(ioctl_args);
		return -EINVAL;
	}
	malloc_len = sizeof(struct cfg80211_acl_data) + sizeof(struct mac_address) * ap_count;
	mac_acl = kmalloc(malloc_len, GFP_KERNEL);
	if (!mac_acl) {
		SLSI_ERR(sdev, "MEM fail for size:%ld\n", sizeof(struct cfg80211_acl_data) + sizeof(struct mac_address) * ap_count);
		kfree(ioctl_args);
		return -ENOMEM;
	}

	for (i = 1; i <= ap_count; i++) {
		if (strlen(ioctl_args->args[i]) != 17) {
			SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[i]));
			kfree(ioctl_args);
			kfree(mac_acl);
			return -EINVAL;
		}
		slsi_machexstring_to_macarray(ioctl_args->args[i], mac_acl->mac_addrs[i].addr);
		SLSI_DBG3_NODEV(SLSI_MLME, "[%pM]", mac_acl->mac_addrs[i].addr);
	}
	mac_acl->acl_policy = NL80211_ACL_POLICY_DENY_UNLESS_LISTED;
	mac_acl->n_acl_entries = ap_count;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	r = slsi_mlme_set_acl(sdev, dev, ndev_vif->ifnum, mac_acl->acl_policy, mac_acl->n_acl_entries, mac_acl->mac_addrs);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(mac_acl);
	kfree(ioctl_args);
	return r;
}

static int slsi_roam_scan_band_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mib_value != 1 && mib_value != 2) {
		SLSI_ERR(sdev, "Invalid roam scan band value: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	return slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_SCAN_BAND, mib_value);
}

static int slsi_roam_scan_band_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_SCAN_BAND, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETROAMINTRABAND, mib_value);

	return res;
}

static int slsi_roam_scan_control_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif   = netdev_priv(dev);
	struct slsi_dev        *sdev       = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    mode        = 0;
	int                    res         = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	res = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (res)
		return res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		res = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		res = -EINVAL;
		goto exit;
	}

	if (mode == 0 || mode == 1) {
		sdev->device_config.roam_scan_mode = mode;
	} else {
		SLSI_ERR(sdev, "Invalid roam Mode: Must be 0 or, 1 Not '%d'\n", mode);
		res = -EINVAL;
		goto exit;
	}

	res = slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_CONTROL, sdev->device_config.roam_scan_mode);
	if (res)
		goto exit;

	/* If the mode is 0, Clear the roam cache */
	if (!mode) {
		memset(&sdev->device_config.wes_roam_scan_list, 0, sizeof(struct slsi_roam_scan_channels));
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
		res = slsi_mlme_set_cached_channels(sdev, dev, 0, NULL);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return res;
	}

exit:
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	kfree(ioctl_args);
	return res;
}

static int slsi_roam_scan_control_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_CONTROL, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETROAMSCANCONTROL, mib_value);

	return res;
}

static int slsi_roam_scan_home_time_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mib_value > 300 || mib_value < 3) {
		SLSI_ERR(sdev, "Invalid scan home time: '%d'\n", mib_value);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	return slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_HOME_TIME, mib_value);
}

static int slsi_roam_scan_home_time_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_HOME_TIME, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETSCANHOMETIME, mib_value);

	return res;
}

static int slsi_roam_scan_home_away_time_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mib_value > 300 || mib_value < 3) {
		SLSI_ERR(sdev, "Invalid scan home away time: '%d'\n", mib_value);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	return slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_HOME_AWAY_TIME, mib_value);
}

static int slsi_roam_scan_home_away_time_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_HOME_AWAY_TIME, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETSCANHOMEAWAYTIME, mib_value);

	return res;
}

static ssize_t slsi_validate_low_latency_params(struct net_device *dev, char *command, int buf_len, int *latency_param)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], latency_param)) {
		SLSI_ERR(sdev, "Invalid string latency_param: '%s'\n", ioctl_args->args[0]);
		ret = -EINVAL;
	} else if (*latency_param < 0 || *latency_param > 0xFFFF) {
		SLSI_ERR(sdev, "Invalid latency_param value: '%d'\n", *latency_param);
		ret = -EINVAL;
	}

	kfree(ioctl_args);
	return ret;
}

static ssize_t slsi_set_low_latency_params(struct net_device *dev, int latency_param)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int ret = 0;

	if ((sdev->latency_param_mask & LATENCY_ALL_SET_MASK) == LATENCY_ALL_SET_MASK) {
		SLSI_INFO(sdev, "Scan Mode = %d, Home Away Time = %d, Home time = %d, Max Channel Time = %d Passive Time = %d\n",
			  sdev->scan_mode, sdev->home_away_time, sdev->home_time,
			  sdev->max_channel_time, sdev->max_channel_passive_time);
		ret = slsi_mlme_set_scan_mode_req(sdev, dev, sdev->scan_mode, sdev->max_channel_time,
						  sdev->home_away_time, sdev->home_time, sdev->max_channel_passive_time);
		sdev->latency_param_mask = 0;
	} else if (latency_param == 0) {
		ret = slsi_mlme_set_scan_mode_req(sdev, dev, FAPI_SCANMODE_LEGACY, 0, 0, 0, 0);
		sdev->latency_param_mask = 0;
	}

	return ret;
}

static int slsi_set_home_away_time_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int home_away_time = 0;
	int ret = 0;

	ret = slsi_validate_low_latency_params(dev, command, buf_len, &home_away_time);
	if (ret != 0)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	sdev->home_away_time = home_away_time;
	if (home_away_time != 0)
		sdev->latency_param_mask |= HOME_AWAY_TIME_BIT;
	ret = slsi_set_low_latency_params(dev, home_away_time);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return ret;
}

static int slsi_set_home_time_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int home_time = 0;
	int ret = 0;

	ret = slsi_validate_low_latency_params(dev, command, buf_len, &home_time);
	if (ret != 0)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	sdev->home_time = home_time;
	if (home_time != 0)
		sdev->latency_param_mask |= HOME_TIME_BIT;
	ret = slsi_set_low_latency_params(dev, home_time);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

static int slsi_set_channel_time_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int max_channel_time = 0;
	int ret = 0;

	ret = slsi_validate_low_latency_params(dev, command, buf_len, &max_channel_time);
	if (ret != 0)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	sdev->max_channel_time = max_channel_time;
	if (max_channel_time != 0)
		sdev->latency_param_mask |= MAX_CHANNEL_TIME_BIT;
	ret = slsi_set_low_latency_params(dev, max_channel_time);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return ret;
}

static int slsi_set_passive_time_legacy(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int max_channel_passive_time = 0;
	int ret = 0;

	ret = slsi_validate_low_latency_params(dev, command, buf_len, &max_channel_passive_time);
	if (ret != 0)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	sdev->max_channel_passive_time = max_channel_passive_time;
	if (max_channel_passive_time != 0)
		sdev->latency_param_mask |= MAX_CHANNEL_PASSIVE_TIME_BIT;
	ret = slsi_set_low_latency_params(dev, max_channel_passive_time);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return ret;
}

static int slsi_set_tid(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mode = 0, uid = 0, tid = 0, ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 3);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string mode : '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (mode < 0 || mode > 3) {
		SLSI_ERR(sdev, "Invalid mode: '%d'\n", mode);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &uid)) {
		SLSI_ERR(sdev, "Invalid string uid: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (!slsi_str_to_int(ioctl_args->args[2], &tid)) {
		SLSI_ERR(sdev, "Invalid string tid: '%s'\n", ioctl_args->args[2]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	return slsi_netif_set_tid_config(sdev, dev, mode, uid, tid);
}

static int slsi_roam_scan_frequencies_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    result = 0;
	int                    i = 0;
	int                    ret = 0;
	int                    freq_count = 0;
	int                    freqs[SLSI_NCHO_MAX_FREQUENCY_LIST] = { 0 };
	const u8               *connected_ssid = NULL;
	u32                    network_map_channels_count = 0;
	u16                    network_map_channels[SLSI_ROAMING_CHANNELS_MAX];
	u16                    merged_channels[SLSI_ROAMING_CHANNELS_MAX * 2];
	u32                    merge_chan_count = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 21);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &freq_count)) {
		SLSI_ERR(sdev, "Invalid freq_count string: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (freq_count < 0) {
		SLSI_ERR(sdev, "Invalid frequency count : %d\n", freq_count);
		kfree(ioctl_args);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	if (freq_count > SLSI_NCHO_MAX_FREQUENCY_LIST)
		freq_count = SLSI_NCHO_MAX_FREQUENCY_LIST;

	if ((ioctl_args->arg_count - 1) < freq_count) {
		SLSI_ERR(sdev, "Buffer doesn't have enough fields freq_count: %d\n", freq_count);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	sdev->device_config.wes_roam_scan_list.n = freq_count;

	for (i = 0; i < freq_count; i++) {
		struct ieee80211_channel *chan = NULL;

		if (!slsi_str_to_int(ioctl_args->args[i + 1], &freqs[i])) {
			SLSI_ERR(sdev, "failed to read a numeric value\n");
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		chan = ieee80211_get_channel(sdev->wiphy, freqs[i]);
		if (!chan) {
			SLSI_ERR(sdev, "Invalid frequency : %d\n", freqs[i]);
			kfree(ioctl_args);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			return -EINVAL;
		}
		sdev->device_config.wes_roam_scan_list.channels[i] =
			SLSI_CONVERT_CH_TO_CACHED_CH(chan->hw_value, chan->band);
	}

	if (!sdev->device_config.roam_scan_mode) {
		ret = slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_CONTROL, 1);
		if (ret != SLSI_MIB_STATUS_SUCCESS) {
			kfree(ioctl_args);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			return -EINVAL;
		}
		sdev->device_config.roam_scan_mode = 1;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	connected_ssid = cfg80211_find_ie(WLAN_EID_SSID, ndev_vif->sta.sta_bss->ies->data,
					  ndev_vif->sta.sta_bss->ies->len);

	network_map_channels_count = slsi_roaming_scan_configure_channels(sdev, dev, connected_ssid, network_map_channels);
	merge_chan_count = slsi_merge_lists(network_map_channels, network_map_channels_count,
					    sdev->device_config.wes_roam_scan_list.channels,
					    sdev->device_config.wes_roam_scan_list.n,
					    merged_channels);

	result = slsi_mlme_set_cached_channels(sdev, dev, merge_chan_count, merged_channels);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return result;
}

static int slsi_roam_scan_channels_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    result = 0;
	int                    i, channel_count = 0;
	int                    channels[SLSI_NCHO_MAX_CHANNEL_LIST];
	int                    ret = 0;
	const u8               *connected_ssid = NULL;
	u32                    network_map_channels_count = 0;
	u16                    network_map_channels[SLSI_ROAMING_CHANNELS_MAX];
	u16                    merged_channels[SLSI_ROAMING_CHANNELS_MAX * 2];
	u32                    merge_chan_count = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 21);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &channel_count)) {
		SLSI_ERR(sdev, "Invalid channel_count string: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (channel_count < 0) {
		SLSI_ERR(sdev, "Invalid channel count : %d\n", channel_count);
		kfree(ioctl_args);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	if (channel_count > SLSI_NCHO_MAX_CHANNEL_LIST)
		channel_count = SLSI_NCHO_MAX_CHANNEL_LIST;

	if ((ioctl_args->arg_count - 1) < channel_count) {
		SLSI_ERR(sdev, "Buffer doesn't have enough fields channel_count: %d\n", channel_count);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	sdev->device_config.wes_roam_scan_list.n = channel_count;

	for (i = 0; i < channel_count; i++) {
		if (!slsi_str_to_int(ioctl_args->args[i + 1], &channels[i])) {
			SLSI_ERR(sdev, "failed to read a numeric value\n");
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		if (channels[i] < 1 || channels[i] > SLSI_5GHZ_LAST_CHAN) {
			SLSI_ERR(sdev, "Invalid channel : %d\n", channels[i]);
			kfree(ioctl_args);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			return -EINVAL;
		}
		sdev->device_config.wes_roam_scan_list.channels[i] = channels[i];
	}

	if (!sdev->device_config.roam_scan_mode) {
		ret = slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_SCAN_CONTROL, 1);
		if (ret != SLSI_MIB_STATUS_SUCCESS) {
			kfree(ioctl_args);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			return -EINVAL;
		}
		sdev->device_config.roam_scan_mode = 1;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	connected_ssid = cfg80211_find_ie(WLAN_EID_SSID, ndev_vif->sta.sta_bss->ies->data,
					  ndev_vif->sta.sta_bss->ies->len);

	network_map_channels_count = slsi_roaming_scan_configure_channels(sdev, dev, connected_ssid, network_map_channels);
	merge_chan_count = slsi_merge_lists(network_map_channels, network_map_channels_count,
					    sdev->device_config.wes_roam_scan_list.channels,
					    sdev->device_config.wes_roam_scan_list.n,
					    merged_channels);

	result = slsi_mlme_set_cached_channels(sdev, dev, merge_chan_count, merged_channels);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return result;
}

static int slsi_roam_scan_frequencies_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct sk_buff    *ind = NULL;
	int		  freq_list[MAX_FREQUENCY_COUNT] = {0};
	int               pos = 0;
	int               i;
	int               freq_count = 0;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "STA is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		freq_count = 0;
		goto output;
	}

	ind = slsi_mlme_roaming_channel_list_req(sdev, dev);
	if (!ind) {
		SLSI_ERR(sdev, "RCL ind is NULL!\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	freq_count = slsi_get_rcl_freq_list(sdev, ind, freq_list);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (!freq_count) {
		SLSI_ERR(sdev, "Frequency count is 0!\n");
		return -EINVAL;
	}

output:
	pos = scnprintf(command, buf_len, "%s %d", CMD_GETROAMSCANFREQUENCIES, freq_count);
	for (i = 0; i < freq_count; i++)
		pos += scnprintf(command + pos, buf_len - pos, " %d", freq_list[i]);

	return pos;
}

static int slsi_roam_scan_channels_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct sk_buff    *ind = NULL;
	u16               channel_list[MAX_CHANNEL_COUNT] = {0};
	int               pos = 0;
	int               i;
	int               channel_count = 0;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ind = slsi_mlme_roaming_channel_list_req(sdev, dev);
	if (!ind) {
		SLSI_ERR(sdev, "RCL ind is NULL!\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	channel_count = slsi_get_rcl_channel_list(sdev, ind, channel_list);
	if (!channel_count) {
		SLSI_ERR(sdev, "Channel count is 0!\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}
	pos = scnprintf(command, buf_len, "%s %d", CMD_GETROAMSCANCHANNELS, channel_count);
	for (i = 0; i < channel_count; i++)
		pos += scnprintf(command + pos, buf_len - pos, " %d", channel_list[i]);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return pos;
}

static int slsi_roam_add_scan_frequencies(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    result = 0;
	int                    i, j;
	int                    curr_channel_count = 0;
	int                    found = 0;
	int 		           new_freq_count = 0;
	int 		           new_freqs[SLSI_NCHO_MAX_FREQUENCY_LIST] = { 0 };
	const u8               *connected_ssid = NULL;
	u32                    network_map_channels_count = 0;
	u16                    network_map_channels[SLSI_ROAMING_CHANNELS_MAX];
	u16                    merged_channels[SLSI_ROAMING_CHANNELS_MAX * 2];
	u32                    merge_chan_count = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 21);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (sdev->device_config.roam_scan_mode) {
		SLSI_ERR(sdev, "ROAM Scan Control must be 0, roam mode = %d\n", sdev->device_config.roam_scan_mode);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (sdev->device_config.wes_roam_scan_list.n == SLSI_NCHO_MAX_CHANNEL_LIST) {
		SLSI_ERR(sdev, "Roam scan list is already full\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION && ndev_vif->sta.vif_status ==
	    SLSI_VIF_STATUS_CONNECTED && sdev->device_config.wes_roam_scan_list.n == 0) {
		sdev->device_config.wes_roam_scan_list.n = 1;
		sdev->device_config.wes_roam_scan_list.channels[0] =
			SLSI_CONVERT_CH_TO_CACHED_CH(ndev_vif->chan->hw_value, ndev_vif->chan->band);
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (!slsi_str_to_int(ioctl_args->args[0], &new_freq_count)) {
		SLSI_ERR(sdev, "Invalid freq_count string: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (new_freq_count < 0 || new_freq_count > 20 || ((ioctl_args->arg_count - 1) < new_freq_count)) {
		SLSI_ERR(sdev, "Invalid frequency count : %d Range: [0,20]\n", new_freq_count);
		kfree(ioctl_args);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}

	curr_channel_count = sdev->device_config.wes_roam_scan_list.n;

	for (i = 0; i < new_freq_count; i++) {
		struct ieee80211_channel *chan = NULL;
		u16 new_channel;

		if (!slsi_str_to_int(ioctl_args->args[i + 1], &new_freqs[i])) {
			SLSI_ERR(sdev, "failed to read a numeric value\n");
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}

		chan = ieee80211_get_channel(sdev->wiphy, new_freqs[i]);
		if (!chan) {
			SLSI_ERR(sdev, "Invalid frequency : %d\n", new_freqs[i]);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		new_channel = SLSI_CONVERT_CH_TO_CACHED_CH(chan->hw_value, chan->band);
		for (j = 0; j < curr_channel_count; j++) {
			found = 0;
			if (sdev->device_config.wes_roam_scan_list.channels[j] == new_channel) {
				found = 1;
				break;
			}
		}
		if (!found) {
			sdev->device_config.wes_roam_scan_list.channels[curr_channel_count] = new_channel;
			curr_channel_count++;
		}
		if (curr_channel_count >= SLSI_NCHO_MAX_CHANNEL_LIST) {
			curr_channel_count = SLSI_NCHO_MAX_CHANNEL_LIST;
			break;
		}
	}

	sdev->device_config.wes_roam_scan_list.n = curr_channel_count;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	connected_ssid = cfg80211_find_ie(WLAN_EID_SSID, ndev_vif->sta.sta_bss->ies->data,
					  ndev_vif->sta.sta_bss->ies->len);

	network_map_channels_count = slsi_roaming_scan_configure_channels(sdev, dev, connected_ssid, network_map_channels);
	merge_chan_count = slsi_merge_lists(network_map_channels, network_map_channels_count,
					    sdev->device_config.wes_roam_scan_list.channels,
					    sdev->device_config.wes_roam_scan_list.n,
					    merged_channels);

	result = slsi_mlme_set_cached_channels(sdev, dev, merge_chan_count, merged_channels);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return result;
}

static int slsi_roam_add_scan_channels(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    result = 0;
	int                    i, j, new_channel_count = 0;
	int                    new_channels[SLSI_NCHO_MAX_CHANNEL_LIST];
	int                    curr_channel_count = 0;
	int                    found = 0;
	const u8               *connected_ssid = NULL;
	u32                    network_map_channels_count = 0;
	u16                    network_map_channels[SLSI_ROAMING_CHANNELS_MAX];
	u16                    merged_channels[SLSI_ROAMING_CHANNELS_MAX * 2];
	u32                    merge_chan_count = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 21);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (sdev->device_config.roam_scan_mode) {
		SLSI_ERR(sdev, "ROAM Scan Control must be 0, roam mode = %d\n", sdev->device_config.roam_scan_mode);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (sdev->device_config.wes_roam_scan_list.n == SLSI_NCHO_MAX_CHANNEL_LIST) {
		SLSI_ERR(sdev, "Roam scan list is already full\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	/* Adding Connected channel to wes_roam_scan_list */
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION && ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED &&
	    sdev->device_config.wes_roam_scan_list.n == 0 && ndev_vif->chan) {
		sdev->device_config.wes_roam_scan_list.n = 1;
		sdev->device_config.wes_roam_scan_list.channels[0] = ndev_vif->chan->hw_value;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	if (!slsi_str_to_int(ioctl_args->args[0], &new_channel_count)) {
		SLSI_ERR(sdev, "Invalid channel_count string: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (new_channel_count < 0 || new_channel_count > 20) {
		SLSI_ERR(sdev, "Invalid channel count : %d Range: [0,20]\n", new_channel_count);
		kfree(ioctl_args);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}

	if ((ioctl_args->arg_count - 1) < new_channel_count) {
		SLSI_ERR(sdev, "Buffer doesn't have enough fields channel_count: %d\n", new_channel_count);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	curr_channel_count = sdev->device_config.wes_roam_scan_list.n;

	for (i = 0; i < new_channel_count; i++) {
		if (!slsi_str_to_int(ioctl_args->args[i + 1], &new_channels[i])) {
			SLSI_ERR(sdev, "failed to read a numeric value\n");
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		if (new_channels[i] < 1 || new_channels[i] > SLSI_5GHZ_LAST_CHAN) {
			SLSI_ERR(sdev, "Invalid channel : %d\n", new_channels[i]);
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		for (j = 0; j < curr_channel_count; j++) {
			found = 0;
			if (sdev->device_config.wes_roam_scan_list.channels[j] == new_channels[i]) {
				found = 1;
				break;
			}
		}
		if (!found) {
			sdev->device_config.wes_roam_scan_list.channels[curr_channel_count] = new_channels[i];
			curr_channel_count++;
		}
		if (curr_channel_count >= SLSI_NCHO_MAX_CHANNEL_LIST) {
			curr_channel_count = SLSI_NCHO_MAX_CHANNEL_LIST;
			break;
		}
	}

	sdev->device_config.wes_roam_scan_list.n = curr_channel_count;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	connected_ssid = cfg80211_find_ie(WLAN_EID_SSID, ndev_vif->sta.sta_bss->ies->data,
					  ndev_vif->sta.sta_bss->ies->len);

	network_map_channels_count = slsi_roaming_scan_configure_channels(sdev, dev, connected_ssid, network_map_channels);
	merge_chan_count = slsi_merge_lists(network_map_channels, network_map_channels_count,
					    sdev->device_config.wes_roam_scan_list.channels,
					    sdev->device_config.wes_roam_scan_list.n,
					    merged_channels);

	result = slsi_mlme_set_cached_channels(sdev, dev, merge_chan_count, merged_channels);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return result;
}

static int slsi_okc_mode_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mode = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mode == 0 || mode == 1) {
		sdev->device_config.okc_mode = mode;
	} else {
		SLSI_ERR(sdev, "Invalid OKC Mode: Must be 0 or, 1 Not '%d'\n", mode);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	return 0;
}

static int slsi_okc_mode_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               okc_mode;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}

	okc_mode = sdev->device_config.okc_mode;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = snprintf(command, buf_len, "%s %d", CMD_GETOKCMODE, okc_mode);

	return res;
}

static int slsi_wes_mode_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               result = -EINVAL;
	u32               action_frame_bmap = SLSI_STA_ACTION_FRAME_BITMAP;
	int mode = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}

	if (mode == 0 || mode == 1) {
		sdev->device_config.wes_mode = mode;
	} else {
		SLSI_ERR(sdev, "Invalid WES Mode: Must be 0 or 1 Not '%d'\n", mode);
		goto exit;
	}

	result = 0;
	if (ndev_vif->activated && ndev_vif->vif_type == FAPI_VIFTYPE_STATION &&
	    ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED) {
		if (sdev->device_config.wes_mode)
			action_frame_bmap |= SLSI_ACTION_FRAME_VENDOR_SPEC;

		result = slsi_mlme_register_action_frame(sdev, dev, action_frame_bmap, action_frame_bmap);
	}
exit:
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return result;
}

static int slsi_wes_mode_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               wes_mode;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}

	wes_mode = sdev->device_config.wes_mode;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = snprintf(command, buf_len, "%s %d", CMD_GETWESMODE, wes_mode);

	return res;
}

static int slsi_set_tx_ant_config(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               ret = 0;
	int               cfg = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &cfg)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (cfg < 0 || cfg > 2) {
		SLSI_ERR(sdev, "Invalid Tx antenna config: Must be between 0 and 2, cfg = %d\n", cfg);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (sdev->device_config.tx_ant_config == cfg) {
		SLSI_INFO(sdev, "Tx antenna config is already %d\n", cfg);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return ret;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EPERM;
	}

	ret = slsi_mlme_set_tx_num_ant(sdev, dev, cfg);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (!ret) {
		SLSI_MUTEX_LOCK(sdev->device_config_mutex);
		sdev->device_config.tx_ant_config = cfg;
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	}

	kfree(ioctl_args);
	return ret;
}

static int slsi_set_ncho_mode(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	struct sk_buff    *req;
	struct sk_buff    *cfm;
	int               ret = 0;
	int               mode = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mode != 0 && mode != 1) {
		SLSI_ERR(sdev, "Invalid NCHO Mode: Must be 0 or 1, mode = %d\n", mode);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	if (sdev->device_config.ncho_mode == mode) {
		SLSI_INFO(sdev, "ncho_mode is already %d\n", mode);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return ret;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EPERM;
	}

	SLSI_DBG1_NODEV(SLSI_MLME, "mlme_set_roaming_type_req(vif:%u mode:%u)\n", ndev_vif->ifnum, mode);

	req = fapi_alloc(mlme_set_roaming_type_req, MLME_SET_ROAMING_TYPE_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EIO;
	}
	fapi_set_u16(req, u.mlme_set_roaming_type_req.vif, ndev_vif->ifnum);
	fapi_set_u16(req, u.mlme_set_roaming_type_req.roaming_type, mode);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_ROAMING_TYPE_CFM);
	if (!cfm) {
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_set_roaming_type_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_set_roaming_type_cfm(result:0x%04x) ERROR\n",
			     fapi_get_u16(cfm, u.mlme_set_roaming_type_cfm.result_code));
		ret = -EINVAL;
	}

	kfree_skb(cfm);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_conn_log2us_ncho_mode(sdev, dev, mode);
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	sdev->device_config.ncho_mode = mode;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return ret;
}

static int slsi_get_tx_ant_config(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	ret = snprintf(command, buf_len, "%d", sdev->device_config.tx_ant_config);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	return ret;
}

static int slsi_get_ncho_mode(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               ret;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	ret = snprintf(command, buf_len, "%s %d", CMD_GETNCHOMODE, sdev->device_config.ncho_mode);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	return ret;
}

static int slsi_set_dfs_scan_mode(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mode = 0;
	int res = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	res = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (res)
		return res;

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mode >= 0 && mode <= 2) {
		sdev->device_config.dfs_scan_mode = mode;
	} else {
		SLSI_ERR(sdev, "Invalid dfs scan mode: Must be 0/1 or 2, Not '%d'\n", mode);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	kfree(ioctl_args);
	res = slsi_set_mib_roam(sdev, NULL, SLSI_PSID_UNIFI_ROAM_NCHO_DFS_SCAN_MODE, sdev->device_config.dfs_scan_mode);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	return res;
}

static int slsi_get_dfs_scan_mode(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               mib_value = 0;
	int               res;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.ncho_mode) {
		SLSI_INFO(sdev, "Command not allowed, NCHO is disabled\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	res = slsi_get_mib_roam(sdev, SLSI_PSID_UNIFI_ROAM_NCHO_DFS_SCAN_MODE, &mib_value);
	if (res)
		return res;

	res = snprintf(command, buf_len, "%s %d", CMD_GETDFSSCANMODE, mib_value);

	return res;
}

#endif

static int slsi_set_pmk(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	u8                pmk[33] = {0};
	int               result = 0;
	struct key_params params;
	u8                mac_addr[ETH_ALEN] = {0};

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (strlen(ioctl_args->args[0]) < 32) {
		kfree(ioctl_args);
		return -EINVAL;
	}

	memcpy((u8 *)pmk, ioctl_args->args[0], 32);
	memset(mac_addr, 0x00, ETH_ALEN);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	memset(&params, 0, sizeof(params));
	params.key = pmk;
	params.key_len = 32;
	params.cipher = WLAN_CIPHER_SUITE_PMK;
	result = slsi_mlme_set_key(sdev, dev, 0, FAPI_KEYTYPE_PMK, mac_addr, &params);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return result;
}

static int slsi_auto_chan_read(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int             ap_auto_chan;
	int result = 0;

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	ap_auto_chan = sdev->device_config.ap_auto_chan;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	result = snprintf(command, buf_len, "%d\n", ap_auto_chan);
	return result;
}

static ssize_t slsi_auto_chan_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif        *ndev_vif = netdev_priv(dev);
	struct slsi_dev          *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args   *ioctl_args = NULL;
	int                      n_channels = 0;
	struct ieee80211_channel *channels[SLSI_NO_OF_SCAN_CHANLS_FOR_AUTO_CHAN_MAX] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	int                      count_channels;
	int                      chan;
	int                      ret = 0;
#ifdef CONFIG_SCSC_WLAN_WIFI_SHARING
	struct net_device *sta_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	struct netdev_vif *ndev_sta_vif  = netdev_priv(sta_dev);
	int sta_frequency;
#endif

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &n_channels)) {
		SLSI_ERR(sdev, "channel count: failed to read a numeric value");
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (n_channels < 1 || n_channels > SLSI_NO_OF_SCAN_CHANLS_FOR_AUTO_CHAN_MAX) {
		SLSI_ERR(sdev, "Invalid channel count:%d", n_channels);
		kfree(ioctl_args);
		return -EINVAL;
	}

	/* If "1 6 11" are passed, scan all "1 - 14" channels. If "1 6" are passed, scan "1 - 9" channels */
	if (n_channels == 3)
		n_channels = 14;
	else if (n_channels == 2)
		n_channels = 9;
	count_channels = 0;
	for (chan = 1; chan <= n_channels; chan++) {
		int center_freq;

		center_freq = ieee80211_channel_to_frequency(chan, NL80211_BAND_2GHZ);
		channels[count_channels] = ieee80211_get_channel(sdev->wiphy, center_freq);
		if (!channels[count_channels])
			SLSI_WARN(sdev, "channel number:%d invalid\n", chan);
		else
			count_channels++;
	}

	SLSI_DBG3(sdev, SLSI_INIT_DEINIT, "Number of channels for autochannel selection= %d\n", count_channels);

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	sdev->device_config.ap_auto_chan = 0;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

#ifdef CONFIG_SCSC_WLAN_WIFI_SHARING
	if (ndev_sta_vif->activated && ndev_sta_vif->vif_type == FAPI_VIFTYPE_STATION &&
	    (ndev_sta_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTING ||
	     ndev_sta_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED)) {
		sta_frequency = ndev_sta_vif->chan->center_freq;
		SLSI_MUTEX_LOCK(sdev->device_config_mutex);
		if ((sta_frequency / 1000) == 2)
			sdev->device_config.ap_auto_chan = ieee80211_frequency_to_channel(sta_frequency);
		else
			sdev->device_config.ap_auto_chan = 1;
		SLSI_INFO(sdev, "Channel selected = %d", sdev->device_config.ap_auto_chan);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return 0;
	}
#endif /*wifi sharing*/
	kfree(ioctl_args);
	return slsi_auto_chan_select_scan(sdev, count_channels, channels);
}

static int slsi_send_action_frame(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	char                 *temp;
	u8                   bssid[6] = { 0 };
	int                  channel = 0;
	int                  freq = 0;
	enum nl80211_band    band = NL80211_BAND_2GHZ;
	int                  r = 0;
	u16                  host_tag = slsi_tx_mgmt_host_tag(sdev);
	u32                  dwell_time;
	struct ieee80211_hdr *hdr;
	u8                   *buf = NULL;
	u8                   *final_buf = NULL;
	u8                   temp_byte;
	int                  len = 0;
	int                  final_length = 0;
	int                  i = 0, j = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_STATION ||
	    ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_ERR(sdev, "Not a STA vif or status is not CONNECTED\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	ioctl_args = slsi_get_private_command_args(command, buf_len, 5);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

	if (ioctl_args->arg_count < 5) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid mac address: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!SLSI_ETHER_EQUAL(bssid, ndev_vif->sta.bssid)) {
		SLSI_ERR(sdev, "Wrong Bssid = " MACSTR " Connected Bssid = " MACSTR "\n", MAC2STR(bssid), MAC2STR(ndev_vif->sta.bssid));
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (!slsi_str_to_int(ioctl_args->args[1], &channel)) {
		SLSI_ERR(sdev, "Invalid channel string: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (channel < 1 || channel > SLSI_5GHZ_LAST_CHAN) {
		SLSI_ERR(sdev, "Invalid channel : %d\n", channel);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (channel > 14)
		band = NL80211_BAND_5GHZ;
	freq = (u16)ieee80211_channel_to_frequency(channel, band);
	if (!freq) {
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[2], &dwell_time)) {
		SLSI_ERR(sdev, "Invalid dwell time string: '%s'\n", ioctl_args->args[2]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[3], &len)) {
		SLSI_ERR(sdev, "Invalid length string: '%s'\n", ioctl_args->args[3]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	/* if length is less than 512 or greater than 1024 driver will ignore the command */
	if (len < 512 || len > 1024) {
		if (len < 0 || len > 1024) {
			SLSI_ERR(sdev, "Invalid buffer length:%d\n", len);
			kfree(ioctl_args);
			return -EINVAL;
		}
		SLSI_INFO(sdev, "Frame Body of Vendor Specific buffer length = %d so ignoring the command\n", len);
		kfree(ioctl_args);
		return 0;
	}

	buf = kmalloc((len + 1) / 2, GFP_KERNEL);

	if (!buf) {
		SLSI_ERR(sdev, "Malloc  failed\n");
		kfree(ioctl_args);
		return -ENOMEM;
	}

	/*We receive a char buffer, convert to hex*/
	temp = ioctl_args->args[4];
	for (i = 0, j = 0; j < len; j += 2) {
		if (j + 1 == len)
			temp_byte = slsi_parse_hex(temp[j]);
		else
			temp_byte = slsi_parse_hex(temp[j]) << 4 | slsi_parse_hex(temp[j + 1]);
		buf[i++] = temp_byte;
	}
	len = i;

	final_length = len + IEEE80211_HEADER_SIZE;
	final_buf = kmalloc(final_length, GFP_KERNEL);
	if (!final_buf) {
		SLSI_ERR(sdev, "Malloc  failed\n");
		kfree(ioctl_args);
		kfree(buf);
		return -ENOMEM;
	}

	hdr = (struct ieee80211_hdr *)final_buf;
	hdr->frame_control = IEEE80211_FC(IEEE80211_FTYPE_MGMT, IEEE80211_STYPE_ACTION);
	SLSI_ETHER_COPY(hdr->addr1, bssid);
	SLSI_ETHER_COPY(hdr->addr2, dev->dev_addr);
	SLSI_ETHER_COPY(hdr->addr3, bssid);
	memcpy(final_buf + IEEE80211_HEADER_SIZE, buf, len);

	kfree(buf);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	r = slsi_mlme_send_frame_mgmt(sdev, dev, final_buf, final_length, FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME, FAPI_MESSAGETYPE_IEEE80211_ACTION, host_tag, SLSI_FREQ_HOST_TO_FW(freq), dwell_time * 1000, 0);

	kfree(final_buf);
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

static int slsi_send_action_frame_cert(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	char                 *temp;
	int                  r = 0;
	u16                  host_tag = slsi_tx_mgmt_host_tag(sdev);
	struct ieee80211_hdr *hdr;
	u8                   *buf = NULL;
	u8                   *final_buf = NULL;
	u8                   temp_byte;
	int                  len = 0;
	int                  final_length = 0;
	int                  i = 0, j = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_STATION ||
	    ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_ERR(sdev, "Not a STA vif or status is not CONNECTED\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

	if (ioctl_args->arg_count < 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &len)) {
		SLSI_ERR(sdev, "Invalid length string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	buf = kmalloc((len + 1) / 2, GFP_KERNEL);

	if (!buf) {
		SLSI_ERR(sdev, "Malloc failed\n");
		kfree(ioctl_args);
		return -ENOMEM;
	}

	/*We receive a char buffer, convert to hex*/
	temp = ioctl_args->args[1];
	for (i = 0, j = 0; j < len; j += 2) {
		if (j + 1 == len)
			temp_byte = slsi_parse_hex(temp[j]);
		else
			temp_byte = slsi_parse_hex(temp[j]) << 4 | slsi_parse_hex(temp[j + 1]);
		buf[i++] = temp_byte;
	}
	len = i;

	final_length = len + IEEE80211_HEADER_SIZE;
	final_buf = kmalloc(final_length, GFP_KERNEL);
	if (!final_buf) {
		SLSI_ERR(sdev, "Malloc failed\n");
		kfree(ioctl_args);
		kfree(buf);
		return -ENOMEM;
	}

	hdr = (struct ieee80211_hdr *)final_buf;
	hdr->frame_control = IEEE80211_FC(IEEE80211_FTYPE_MGMT, IEEE80211_STYPE_ACTION);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	SLSI_ETHER_COPY(hdr->addr1, ndev_vif->sta.bssid);
	SLSI_ETHER_COPY(hdr->addr3, ndev_vif->sta.bssid);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_ETHER_COPY(hdr->addr2, sdev->hw_addr);
	memcpy(final_buf + IEEE80211_HEADER_SIZE, buf, len);

	kfree(buf);

	r = slsi_mlme_send_frame_mgmt(sdev, dev, final_buf, final_length, FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME, FAPI_MESSAGETYPE_IEEE80211_ACTION, host_tag, 0, 0, 0);

	kfree(final_buf);
	kfree(ioctl_args);
	return r;
}

#ifdef SLSI_TEST_DEV
static int slsi_send_action_frame_ut(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	char                 *hex_str;
	u8                   bssid[6] = { 0 };
	u8                   transmitteraddr[6] = { 0 };
	int                  channel = 0;
	int                  r = 0;
	u32                  dwell_time;
	struct ieee80211_mgmt *hdr;
	u8                   *buf = NULL;
	u8                   temp_byte;
	int                  len = 0;
	int                  i = 0, j = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;
	struct ieee80211_channel chan;
	u64 cookie = 1;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 6);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;

	if (ioctl_args->arg_count < 6) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid mac address: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], transmitteraddr);

	if (strlen(ioctl_args->args[1]) != 17) {
		SLSI_ERR(sdev, "Invalid mac address: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[1], bssid);

	if (!slsi_str_to_int(ioctl_args->args[2], &channel)) {
		SLSI_ERR(sdev, "Invalid channel string: '%s'\n", ioctl_args->args[2]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (channel < 1 || channel > SLSI_5GHZ_LAST_CHAN) {
		SLSI_ERR(sdev, "Invalid channel : %d\n", channel);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_INFO(sdev, "channel: '%d'\n", channel);

	if (channel > 14)
		chan.band = NL80211_BAND_5GHZ;
	else
		chan.band = NL80211_BAND_2GHZ;

	SLSI_ERR(sdev, "channel: '%d'\n", chan.band);

	chan.center_freq = (u16)ieee80211_channel_to_frequency(channel, chan.band);
	if (!chan.center_freq) {
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[3], &dwell_time)) {
		SLSI_ERR(sdev, "Invalid dwell time string: '%s'\n", ioctl_args->args[3]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[4], &len)) {
		SLSI_ERR(sdev, "Invalid length string: '%s'\n", ioctl_args->args[4]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	/* if length is less than 0 or greater than 1024 driver will ignore the command */
	if (len < 0 || len > 1024) {
		SLSI_ERR(sdev, "Invalid buffer length:%d\n", len);
		kfree(ioctl_args);
		return -EINVAL;
	}

	buf = kmalloc((IEEE80211_HEADER_SIZE + ((len + 1) / 2)), GFP_KERNEL);

	if (!buf) {
		SLSI_ERR(sdev, "Malloc  failed\n");
		kfree(ioctl_args);
		return -ENOMEM;
	}
	hdr = (struct ieee80211_mgmt *)buf;
	hdr->frame_control = IEEE80211_FC(IEEE80211_FTYPE_MGMT, IEEE80211_STYPE_ACTION);
	SLSI_ETHER_COPY(hdr->da, bssid);
	SLSI_ETHER_COPY(hdr->sa, transmitteraddr);
	SLSI_ETHER_COPY(hdr->bssid, bssid);

	/*We receive a char buffer, convert to hex*/
	hex_str = ioctl_args->args[5];
	for (i = IEEE80211_HEADER_SIZE, j = 0; j < len; j += 2) {
		if (j + 1 == len)
			temp_byte = slsi_parse_hex(hex_str[j]);
		else
			temp_byte = slsi_parse_hex(hex_str[j]) << 4 | slsi_parse_hex(hex_str[j + 1]);
		buf[i++] = temp_byte;
	}
	len = i;
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	r = slsi_wlan_mgmt_tx(sdev, dev, &chan, dwell_time, buf, len, 1, &cookie);

	kfree(buf);
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}
#endif

static int slsi_setting_max_sta_write(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_mib_data mib_data = { 0, NULL };
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                  sta_number = 0;
	int                  result = 0;

	slsi_convert_space_seperation(command, cmd_len);
	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (!slsi_str_to_int(ioctl_args->args[0], &sta_number)) {
		SLSI_ERR(sdev, "Invalid max num sta sting: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (sta_number > 10 || sta_number < 1) {
		kfree(ioctl_args);
		return -EINVAL;
	}

	result = slsi_mib_encode_uint(&mib_data, SLSI_PSID_UNIFI_MAX_CLIENT, sta_number, 0);
	if (result != SLSI_MIB_STATUS_SUCCESS || mib_data.dataLength == 0) {
		kfree(ioctl_args);
		return -ENOMEM;
	}
	result = slsi_mlme_set(sdev, dev, mib_data.data, mib_data.dataLength);
	if (result != 0)
		SLSI_ERR(sdev, "max_sta: mlme_set_req failed: Result code: %d\n", result);
	kfree(ioctl_args);
	kfree(mib_data.data);

	return result;
}

static int slsi_setting_sap_ax_mode(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev         *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args  *ioctl_args = NULL;
	int                     ax_mode = 0;
	int                     result = 0;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (!slsi_str_to_int(ioctl_args->args[0], &ax_mode)) {
		SLSI_ERR(sdev, "Invalid ax mode string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!(ax_mode == 0 || ax_mode == 1)) {
		SLSI_ERR(sdev, "Invalid ax mode:%d\n", ax_mode);
		kfree(ioctl_args);
		return -EINVAL;
	}
	if (!sdev->ap_cert_11ax_enabled)
		ndev_vif->sap_ax_mode = ax_mode;
	else
		ndev_vif->sap_ax_mode = sdev->ap_cert_11ax_enabled;
	kfree(ioctl_args);
	return result;
}

static int slsi_country_write(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *netdev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = netdev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	char              alpha2_code[SLSI_COUNTRY_CODE_LEN];
	int               status;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	status = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (status)
		return status;

	if (strlen(ioctl_args->args[0]) < 2) {
		kfree(ioctl_args);
		return -EINVAL;
	}

	memcpy(alpha2_code, ioctl_args->args[0], 2);
	alpha2_code[2] = ' '; /* set 3rd byte of countrycode to ASCII space */

	status = slsi_set_country_update_regd(sdev, alpha2_code, SLSI_COUNTRY_CODE_LEN);

	kfree(ioctl_args);
	return status;
}

#if defined(CONFIG_SLSI_WLAN_STA_FWD_BEACON) && (defined(SCSC_SEP_VERSION) && SCSC_SEP_VERSION >= 10)
static int slsi_forward_beacon(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *netdev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = netdev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int               intended_action = 0;
	int               ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (strlen(ioctl_args->args[0]) == 4 && strncasecmp(ioctl_args->args[0], "stop", 4) == 0) {
		intended_action = FAPI_ACTION_STOP;
	} else if (strlen(ioctl_args->args[0]) == 5 && strncasecmp(ioctl_args->args[0], "start", 5) == 0) {
		intended_action = FAPI_ACTION_START;
	} else {
		SLSI_NET_ERR(dev, "BEACON_RECV should be used with start or stop\n");
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "BEACON_RECV %s!!\n", intended_action ? "START" : "STOP");
	SLSI_MUTEX_LOCK(netdev_vif->vif_mutex);

	if (!netdev_vif->activated || netdev_vif->vif_type != FAPI_VIFTYPE_STATION ||
	    netdev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_ERR(sdev, "Not a STA vif or status is not CONNECTED\n");
		ret = -EINVAL;
		goto exit_vif_mutex;
	}

	if ((intended_action == FAPI_ACTION_START && netdev_vif->is_wips_running) ||
	    (intended_action == FAPI_ACTION_STOP && !netdev_vif->is_wips_running)) {
		SLSI_NET_INFO(dev, "Forwarding beacon is already %s!!\n",
			      netdev_vif->is_wips_running ? "running" : "stopped");
		ret = 0;
		goto exit_vif_mutex;
	}

	SLSI_MUTEX_LOCK(netdev_vif->scan_mutex);
	if (intended_action == FAPI_ACTION_START &&
	    (netdev_vif->scan[SLSI_SCAN_HW_ID].scan_req || netdev_vif->sta.roam_in_progress)) {
		SLSI_NET_ERR(dev, "Rejecting BEACON_RECV start as scan/roam is running\n");
		ret = -EBUSY;
		goto exit_scan_mutex;
	}

	ret = slsi_mlme_set_forward_beacon(sdev, dev, intended_action);
exit_scan_mutex:
	SLSI_MUTEX_UNLOCK(netdev_vif->scan_mutex);
exit_vif_mutex:
	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(netdev_vif->vif_mutex);
	return ret;
}
#endif

static int slsi_update_rssi_boost(struct net_device *dev, char *buffer, int buf_len)
{
	struct netdev_vif *netdev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = netdev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int digit1, digit2, band, lendigit1, lendigit2;
	int boost = 0, length = 0, i = 0, ret = 0;
	char *rssi_boost_string = NULL;

	ioctl_args = slsi_get_private_command_args(buffer, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	rssi_boost_string = ioctl_args->args[0];
	if (strlen(rssi_boost_string) < 8) {
		kfree(ioctl_args);
		return -EINVAL;
	}
	for (i = 0; i < (strlen(rssi_boost_string) - 4);) {
		if (rssi_boost_string[i] == '0' &&
		    rssi_boost_string[i + 1] == '4') {
			if (rssi_boost_string[i + 2] == '0' &&
			    rssi_boost_string[i + 3] == '2' &&
			    ((i + 7) < strlen(rssi_boost_string))) {
				i = i + 4;
			} else {
				kfree(ioctl_args);
				return -EINVAL;
			}
			digit1 = slsi_parse_hex(rssi_boost_string[i]);
			digit2 = slsi_parse_hex(rssi_boost_string[i + 1]);
			boost = (digit1 * 16) + digit2;
			band = rssi_boost_string[i + 3] - '0';
			SLSI_MUTEX_LOCK(sdev->device_config_mutex);
			if (band == 0) {
				sdev->device_config.rssi_boost_2g = 0;
				sdev->device_config.rssi_boost_5g = 0;
			} else if (band == 1) {
				sdev->device_config.rssi_boost_2g = 0;
				sdev->device_config.rssi_boost_5g = boost;
			} else if (band == 2) {
				sdev->device_config.rssi_boost_2g = boost;
				sdev->device_config.rssi_boost_5g = 0;
			} else {
				SLSI_ERR(sdev, "Invalid band value %d\n", band);
				SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
				kfree(ioctl_args);
				return -EINVAL;
			}
			SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
			if (netdev_vif->activated &&
			    netdev_vif->vif_type == FAPI_VIFTYPE_STATION) {
				kfree(ioctl_args);
				return slsi_set_boost(sdev, dev);
			}
			kfree(ioctl_args);
			return 0;
		}
		i = i + 2;
		lendigit1 = slsi_parse_hex(rssi_boost_string[i]);
		lendigit2 = slsi_parse_hex(rssi_boost_string[i + 1]);
		length = (lendigit1 * 16) + lendigit2;
		i = i + (length * 2) + 2;
	}
	kfree(ioctl_args);
	return -EINVAL;
}

int slsi_set_tx_power_calling(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                  mode = 0;
	int                  error = 0;
	u16                  host_state;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	error = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (error)
		return error;

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	host_state = sdev->device_config.host_state;

	switch (mode) {
	case HEAD_SAR_BACKOFF_DISABLED:
		host_state &= ~SLSI_HOSTSTATE_HEAD_SAR_ACTIVE;
		break;
	case HEAD_SAR_BACKOFF_ENABLED:
		host_state |= SLSI_HOSTSTATE_HEAD_SAR_ACTIVE;
		break;
	case BODY_SAR_BACKOFF_DISABLED:
		host_state &= ~SLSI_HOSTSTATE_GRIP_SAR_ACTIVE;
		break;
	case BODY_SAR_BACKOFF_ENABLED:
		host_state |= SLSI_HOSTSTATE_GRIP_SAR_ACTIVE;
		break;
	case NR_MMWAVE_SAR_BACKOFF_DISABLED:
		host_state &= ~(SLSI_HOSTSTATE_BASE_MMW << SLSI_HOSTSTATE_BASE_POS);
		break;
	case NR_MMWAVE_SAR_BACKOFF_ENABLED:
		host_state &= ~SLSI_HOSTSTATE_BASE_MASK;
		host_state |= SLSI_HOSTSTATE_BASE_MMW << SLSI_HOSTSTATE_BASE_POS;
		break;
	case NR_SUB6_SAR_BACKOFF_DISABLED:
		sdev->device_config.host_state_sub6_band = false;
		host_state &= ~(SLSI_HOSTSTATE_SUB6_BAND_MASK | SLSI_HOSTSTATE_BASE_MASK);
		break;
	case NR_SUB6_SAR_BACKOFF_ENABLED:
		sdev->device_config.host_state_sub6_band = true;
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return error;
	case SAR_BACKOFF_DISABLE_ALL:
		host_state &= SLSI_HOSTSTATE_SAR_INIT_MASK;
		break;
	case MHS_SAR_BACKOFF_DISABLED:
		host_state &= ~SLSI_HOSTSTATE_MHS_SAR_ACTIVE;
		break;
	case MHS_SAR_BACKOFF_ENABLED:
		host_state |= SLSI_HOSTSTATE_MHS_SAR_ACTIVE;
		break;
	default:
		SLSI_ERR(sdev, "Invalid mode: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	error = slsi_mlme_set_host_state(sdev, dev, host_state);
	if (!error)
		sdev->device_config.host_state = host_state;

	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	kfree(ioctl_args);
	return error;
}

int slsi_set_tx_power_sub6_band(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                  mode = 0;
	int                  error = 0;
	u16                  host_state;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	error = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (error)
		return error;

	if (!sdev->device_config.host_state_sub6_band) {
		SLSI_ERR(sdev, "'SET_TX_POWER_CALLING 6' must be called first.\n");
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	host_state = sdev->device_config.host_state;

	host_state &= ~(SLSI_HOSTSTATE_SUB6_BAND_MASK | SLSI_HOSTSTATE_BASE_MASK);
	host_state |= SLSI_HOSTSTATE_BASE_SUB6 << SLSI_HOSTSTATE_BASE_POS;

	switch (mode) {
	case SUB6_SAR_1_BAND:
		host_state |= (SUB6_SAR_1 << SLSI_HOSTSTATE_SUB6_BAND_POS);
		break;
	case SUB6_SAR_2_BAND:
		host_state |= (SUB6_SAR_2 << SLSI_HOSTSTATE_SUB6_BAND_POS);
		break;
	case SUB6_SAR_3_BAND:
		host_state |= (SUB6_SAR_3 << SLSI_HOSTSTATE_SUB6_BAND_POS);
		break;
	case SUB6_SAR_4_BAND:
		host_state |= (SUB6_SAR_4 << SLSI_HOSTSTATE_SUB6_BAND_POS);
		break;
	case SUB6_SAR_5_BAND:
		host_state |= (SUB6_SAR_5 << SLSI_HOSTSTATE_SUB6_BAND_POS);
		break;
	case SUB6_SAR_6_BAND:
		host_state |= (SUB6_SAR_6 << SLSI_HOSTSTATE_SUB6_BAND_POS);
		break;
	default:
		SLSI_ERR(sdev, "Invalid mode: '%s'\n", ioctl_args->args[0]);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	error = slsi_mlme_set_host_state(sdev, dev, host_state);
	if (!error)
		sdev->device_config.host_state = host_state;

	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return error;
}

static int slsi_print_regulatory(struct slsi_802_11d_reg_domain *domain_info, char *buf, int buf_len,
				 struct slsi_sub_band *sub_bands, int sub_band_count)
{
	int  cur_pos = 0;
	int  i, j, k;
	char *dfs_region_str[] = {"unknown", "FCC", "ETSI", "JAPAN", "GLOBAL", "CHINA"};
	u8   dfs_region_index;
	struct ieee80211_reg_rule *reg_rule;
	int  channel_start_freq = 0;
	int  channel_end_freq = 0;
	int  channel_start_num = 0;
	int  channel_end_num = 0;
	int  channel_count = 0;
	int  channel_increment = 0;
	int  channel_band = 0;
	bool display_pattern = false;

	cur_pos = snprintf(buf, buf_len, "country %c%c:", domain_info->regdomain->alpha2[0],
			   domain_info->regdomain->alpha2[1]);
	dfs_region_index = domain_info->regdomain->dfs_region <= 5 ? domain_info->regdomain->dfs_region : 0;
	cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, "DFS-%s\n", dfs_region_str[dfs_region_index]);
	for (i = 0; i < domain_info->regdomain->n_reg_rules; i++) {
		reg_rule = &domain_info->regdomain->reg_rules[i];
		if (reg_rule->freq_range.start_freq_khz / 1000 >= 57000)
			continue;
		cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, "\t(%d-%d @ %d), (N/A, %d)",
					reg_rule->freq_range.start_freq_khz / 1000,
					reg_rule->freq_range.end_freq_khz / 1000,
					reg_rule->freq_range.max_bandwidth_khz / 1000,
					MBM_TO_DBM(reg_rule->power_rule.max_eirp));
		if (reg_rule->flags) {
			if (reg_rule->flags & NL80211_RRF_DFS)
				cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", DFS");
			if (reg_rule->flags & NL80211_RRF_NO_OFDM)
				cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", NO_OFDM");
			if (reg_rule->flags & (NL80211_RRF_NO_IR))
				cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", NO_IR");
			if (reg_rule->flags & NL80211_RRF_NO_INDOOR)
				cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", NO_INDOOR");
			if (reg_rule->flags & NL80211_RRF_NO_OUTDOOR)
				cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", NO_OUTDOOR");
			if (reg_rule->flags & NL80211_RRF_AUTO_BW)
				cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", AUTO_BW");
		}
		cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, "\n");
	}

	/* Display of Supported Channels for 2.4GHz and 5GHz */
	cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, "Channels:");

	for (i = 0; i < sub_band_count; i++) {
		channel_start_num = sub_bands[i].start_chan_num;
		channel_count = sub_bands[i].channel_count;
		channel_increment = sub_bands[i].increment;
		channel_band = sub_bands[i].band;
		channel_end_num = channel_start_num + ((channel_count - 1) * channel_increment);
		for (j = channel_start_num; j <= channel_end_num; j += channel_increment) {
			channel_start_freq = (ieee80211_channel_to_frequency(j, channel_band) * 1000) - 10000;
			channel_end_freq = (ieee80211_channel_to_frequency(j, channel_band) * 1000) + 10000;
			for (k = 0; k < domain_info->regdomain->n_reg_rules; k++) {
				reg_rule = &domain_info->regdomain->reg_rules[k];
				if (reg_rule->freq_range.start_freq_khz <= channel_start_freq &&
				    reg_rule->freq_range.end_freq_khz >= channel_end_freq) {
					if (display_pattern)
						cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", %d", j);
					else
						cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, " %d", j);
					display_pattern = true;
					break;
				}
			}
		}
	}
	cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, "\n");
	return cur_pos;
}

static int slsi_get_supported_channels(struct slsi_dev *sdev, struct net_device *dev,
				       struct slsi_sub_band *sub_bands, int sub_bands_max)
{
	struct slsi_mib_data      mibrsp = { 0, NULL };
	struct slsi_mib_data      supported_chan_mib;
	struct slsi_mib_value     *values = NULL;
	struct slsi_mib_get_entry get_values[] = {{SLSI_PSID_UNIFI_SUPPORTED_CHANNELS, { 1, 0 } },
						  {SLSI_PSID_UNIFI_SUPPORTED_CHANNELS, { 2, 0 } } };
	int                       i, chan_count, chan_start;
	int                       sub_bands_count = 0;

	mibrsp.dataLength = 50;
	mibrsp.data = kmalloc(mibrsp.dataLength, GFP_KERNEL);
	if (!mibrsp.data) {
		SLSI_ERR(sdev, "Cannot kmalloc %d bytes\n", mibrsp.dataLength);
		return 0;
	}
	values = slsi_read_mibs(sdev, dev, get_values, ARRAY_SIZE(get_values), &mibrsp);
	if (!values)
		goto exit_with_mibrsp;

	if (values[0].type != SLSI_MIB_TYPE_OCTET) {
		SLSI_ERR(sdev, "Supported_Chan invalid type.");
		goto exit_with_values;
	}

	supported_chan_mib = values[0].u.octetValue;
	for (i = 0; i < supported_chan_mib.dataLength / 2; i++) {
		chan_start = supported_chan_mib.data[i * 2];
		chan_count = supported_chan_mib.data[i * 2 + 1];
		if (chan_start == 1) { /* for 2.4GHz */
			if (!(sdev->device_config.host_state & SLSI_HOSTSTATE_CELLULAR_ACTIVE) &&
			    chan_count > 11 && sdev->device_config.disable_ch12_ch13) {
				chan_count = 11;
				SLSI_DBG1(sdev, SLSI_CFG80211, "Channels 12 and 13 have been disabled");
			}
			sub_bands[sub_bands_count].increment = 1;
			sub_bands[sub_bands_count].band = NL80211_BAND_2GHZ;
		} else { /* for 5GHz */
			sub_bands[sub_bands_count].increment = 4;
			sub_bands[sub_bands_count].band = NL80211_BAND_5GHZ;
		}
		sub_bands[sub_bands_count].start_chan_num = chan_start;
		sub_bands[sub_bands_count].channel_count = chan_count;
		sub_bands_count++;
		if (sub_bands_count == sub_bands_max)
			break;
	}
#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
	if (values[1].type != SLSI_MIB_TYPE_OCTET) {
		SLSI_ERR(sdev, "6GHz: Supported_Chan invalid type %d", values[1].type);
		goto exit_with_values;
	}
	supported_chan_mib = values[1].u.octetValue;
	for (i = 0; i < supported_chan_mib.dataLength / 2; i++) {
		chan_start = supported_chan_mib.data[i * 2];
		chan_count = supported_chan_mib.data[i * 2 + 1];
		sub_bands[sub_bands_count].increment = 4;
		sub_bands[sub_bands_count].band = NL80211_BAND_6GHZ;
		sub_bands[sub_bands_count].start_chan_num = chan_start;
		sub_bands[sub_bands_count].channel_count = chan_count;
		sub_bands_count++;
		if (sub_bands_count == sub_bands_max)
			break;
	}
#endif
exit_with_values:
	kfree(values);
exit_with_mibrsp:
	kfree(mibrsp.data);
	return sub_bands_count;
}

static int slsi_get_regulatory(struct net_device *dev, char *buf, int buf_len)
{
	struct netdev_vif              *ndev_vif = netdev_priv(dev);
	struct slsi_dev                *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                            mode = 0;
	int                            cur_pos = 0;
	int                            status;
	u8                             alpha2[3] = {0};
	struct slsi_sub_band           sub_bands[SLSI_MAX_SUB_BANDS];
	int                            sub_band_count;

	ioctl_args = slsi_get_private_command_args(buf, buf_len, 1);
	status = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (status)
		return status;

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mode != 0 && mode != 1) {
		SLSI_ERR(sdev, "Invalid mode: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mode == 1) {
		struct slsi_802_11d_reg_domain domain_info;

		memset(&domain_info, 0, sizeof(struct slsi_802_11d_reg_domain));
		SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
		if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_STATION || !ndev_vif->sta.sta_bss) {
			snprintf(buf, buf_len, "Station not connected");
			SLSI_ERR(sdev, "sta not connected. vif[activated:%d type:%d bss:%s]\n",
				 ndev_vif->activated, ndev_vif->vif_type,
				 ndev_vif->sta.sta_bss ? "yes" : "no");
			SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

		/* max 20 rules */
		domain_info.regdomain = kmalloc(sizeof(*domain_info.regdomain) + sizeof(struct ieee80211_reg_rule) * 20,
						GFP_KERNEL);
		if (!domain_info.regdomain) {
			SLSI_ERR(sdev, "no memory size:%lu\n",
				 sizeof(struct ieee80211_regdomain) + sizeof(struct ieee80211_reg_rule) * 20);
			kfree(ioctl_args);
			return -ENOMEM;
		}

		/* get regulatory rules based on country code */
		domain_info.countrylist = sdev->device_config.domain_info.countrylist;
		domain_info.country_len = sdev->device_config.domain_info.country_len;
		status = slsi_read_regulatory_rules(sdev, &domain_info, alpha2);
		if (status) {
			kfree(domain_info.regdomain);
			kfree(ioctl_args);
			return status;
		}

		sub_band_count = slsi_get_supported_channels(sdev, dev, &sub_bands[0], ARRAY_SIZE(sub_bands));
		cur_pos += slsi_print_regulatory(&domain_info, buf + cur_pos, buf_len - cur_pos, &sub_bands[0],
						 sub_band_count);
		kfree(domain_info.regdomain);
	} else if (mode == 0) {
		SLSI_MUTEX_LOCK(sdev->device_config_mutex);
		sub_band_count = slsi_get_supported_channels(sdev, dev, &sub_bands[0], ARRAY_SIZE(sub_bands));
		cur_pos += slsi_print_regulatory(&sdev->device_config.domain_info, buf + cur_pos, buf_len - cur_pos,
						 &sub_bands[0], sub_band_count);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	}
	/* Buf is somewhere close to 4Kbytes. so expect some spare space. If there is no spare
	 * space we might have missed printing some text in buf.
	 */
	kfree(ioctl_args);
	if (buf_len - cur_pos)
		return cur_pos;
	else
		return -ENOMEM;
}

void slsi_disable_ch12_13(struct slsi_dev *sdev)
{
	struct wiphy *wiphy = sdev->wiphy;
	struct ieee80211_channel *chan;

	if (wiphy->bands[0]) {
		chan = &wiphy->bands[0]->channels[11];
		chan->flags |= IEEE80211_CHAN_DISABLED;
		chan = &wiphy->bands[0]->channels[12];
		chan->flags |= IEEE80211_CHAN_DISABLED;
	}

	SLSI_DBG1(sdev, SLSI_CFG80211, "Channels 12 and 13 have been disabled");
}

#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
static int slsi_configure_6ghz_support(struct slsi_dev *sdev, bool value)
{
	struct slsi_mib_data mib_data = { 0, NULL };
	int                  status;

	if (!sdev->fw_6g_supported)
		return 0;

	status = slsi_mib_encode_bool(&mib_data, SLSI_PSID_UNIFI6_GHZ_ENABLED, value, 0);
	if (status != SLSI_MIB_STATUS_SUCCESS) {
		SLSI_ERR(sdev, "6GHz: no mem for MIB\n");
		return -ENOMEM;
	}

	status = slsi_mlme_set(sdev, NULL, mib_data.data, mib_data.dataLength);
	if (status)
		SLSI_ERR(sdev, "Enable/Disable 6GHz failed, err: %d\n", status);

	sdev->band_6g_supported = value;

	kfree(mib_data.data);
	return status;
}
#endif

int slsi_set_fcc_channel(struct net_device *dev, char *cmd, int cmd_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    status;
	int                    fcc_channel_value;
	u16                    host_state;

	ioctl_args = slsi_get_private_command_args(cmd, cmd_len, 1);
	status = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (status)
		return status;

	if (!slsi_str_to_int(ioctl_args->args[0], &fcc_channel_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!(fcc_channel_value >= -1 && fcc_channel_value <= 2)) {
		SLSI_ERR(sdev, "Invalid value of flight_mode_ena: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	host_state = sdev->device_config.host_state;

	/* SET_FCC_CHANNEL 0/1 indicates flight mode is enabled */
	/* SET_FCC_CHANNEL -1/2 indicates flight mode is disabled */
	if (fcc_channel_value == 0 || fcc_channel_value == 1)
		host_state = host_state & ~SLSI_HOSTSTATE_CELLULAR_ACTIVE;
	else
		host_state = host_state | SLSI_HOSTSTATE_CELLULAR_ACTIVE;
	sdev->device_config.host_state = host_state;

	status = slsi_mlme_set_host_state(sdev, dev, host_state);
	if (status) {
		SLSI_ERR(sdev, "Err setting MMaxPowerEna. error = %d\n", status);
	} else {
		if ((fcc_channel_value == 0 || fcc_channel_value == 1) && sdev->device_config.disable_ch12_ch13)
			slsi_disable_ch12_13(sdev);
#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
		if (!slsi_is_test_mode_enabled()) {
			if (sdev->band_6g_supported && (fcc_channel_value == 0 || fcc_channel_value == 2))
				slsi_configure_6ghz_support(sdev, false);
			if (!sdev->band_6g_supported && (fcc_channel_value == 1 || fcc_channel_value == -1))
				slsi_configure_6ghz_support(sdev, true);
		}
#endif
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	kfree(ioctl_args);
	return status;
}

int slsi_fake_mac_write(struct net_device *dev, char *cmd, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_mib_data mib_data = { 0, NULL };
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                  status;
	bool                enable;

	ioctl_args = slsi_get_private_command_args(cmd, buf_len, 1);
	status = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (status)
		return status;

	if (strlen(ioctl_args->args[0]) == 2 && strncmp(ioctl_args->args[0], "ON", strlen("ON")) == 0) {
		enable = 1;
	} else if (strlen(ioctl_args->args[0]) == 3 && strncmp(ioctl_args->args[0], "OFF", strlen("OFF")) == 0) {
		enable = 0;
	} else {
		SLSI_ERR(sdev, "Invalid parameter: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	status = slsi_mib_encode_bool(&mib_data, SLSI_PSID_UNIFI_MAC_ADDRESS_RANDOMISATION, enable, 0);
	if (status != SLSI_MIB_STATUS_SUCCESS) {
		SLSI_ERR(sdev, "FAKE MAC FAIL: no mem for MIB\n");
		kfree(ioctl_args);
		return -ENOMEM;
	}

	status = slsi_mlme_set(sdev, NULL, mib_data.data, mib_data.dataLength);

	kfree(mib_data.data);

	if (status)
		SLSI_ERR(sdev, "Err setting unifiMacAddrRandomistaion MIB. error = %d\n", status);

	kfree(ioctl_args);
	return status;
}

static char *slsi_get_assoc_status(u16 fw_result_code)
{
	char *assoc_status_label = "unspecified_failure";

	switch (fw_result_code) {
	case FAPI_RESULTCODE_SUCCESS:
		assoc_status_label = "success";
		break;
	case FAPI_RESULTCODE_TRANSMISSION_FAILURE:
		assoc_status_label = "transmission_failure";
		break;
	case FAPI_RESULTCODE_HOST_REQUEST_SUCCESS:
		assoc_status_label = "host_request_success";
		break;
	case FAPI_RESULTCODE_HOST_REQUEST_FAILED:
		assoc_status_label = "host_request_failed";
		break;
	case FAPI_RESULTCODE_PROBE_TIMEOUT:
		assoc_status_label = "probe_timeout";
		break;
	case FAPI_RESULTCODE_AUTH_TIMEOUT:
		assoc_status_label = "auth_timeout";
		break;
	case FAPI_RESULTCODE_ASSOC_TIMEOUT:
		assoc_status_label = "assoc_timeout";
		break;
	case FAPI_RESULTCODE_ASSOC_ABORT:
		assoc_status_label = "assoc_abort";
		break;
	}
	return assoc_status_label;
}

static int slsi_copy_ap_sta_info_stats_to_buf(char *command, int buf_len, int len, int support_mode, u32 *rate_stats)
{
	if (buf_len <= len)
		return len;

	switch (support_mode) {
	case SLSI_MODE_HE:
	   len += snprintf(&command[len], (buf_len - len),
			   "11=%d,10=%d,9=%d,8=%d,7=%d,6=%d,5=%d,4=%d,3=%d,2=%d,1=%d,0=%d ",
			   rate_stats[35] + rate_stats[34],
			   rate_stats[33] + rate_stats[32],
			   rate_stats[31] + rate_stats[30],
			   rate_stats[29] + rate_stats[28],
			   rate_stats[27] + rate_stats[26],
			   rate_stats[25] + rate_stats[24],
			   rate_stats[23] + rate_stats[22],
			   rate_stats[21] + rate_stats[20],
			   rate_stats[19] + rate_stats[18],
			   rate_stats[17] + rate_stats[16],
			   rate_stats[15] + rate_stats[14],
			   rate_stats[13] + rate_stats[12]);
		break;
	case SLSI_MODE_VHT:
	   len += snprintf(&command[len], (buf_len - len),
			   "9=%d,8=%d,7=%d,6=%d,5=%d,4=%d,3=%d,2=%d,1=%d,0=%d ",
			   rate_stats[31] + rate_stats[30],
			   rate_stats[29] + rate_stats[28],
			   rate_stats[27] + rate_stats[26],
			   rate_stats[25] + rate_stats[24],
			   rate_stats[23] + rate_stats[22],
			   rate_stats[21] + rate_stats[20],
			   rate_stats[19] + rate_stats[18],
			   rate_stats[17] + rate_stats[16],
			   rate_stats[15] + rate_stats[14],
			   rate_stats[13] + rate_stats[12]);
		break;

	default:/* SLSI_MODE_LEGACY_HT */
	   len += snprintf(&command[len], (buf_len - len),
			   "15=%d,14=%d,13=%d,12=%d,11=%d,10=%d,9=%d,8=%d,7=%d,6=%d,5=%d,4=%d,3=%d,2=%d,1=%d,0=%d ",
			   rate_stats[27], rate_stats[25],
			   rate_stats[23], rate_stats[21],
			   rate_stats[19], rate_stats[17],
			   rate_stats[15], rate_stats[13],
			   rate_stats[26], rate_stats[24],
			   rate_stats[22], rate_stats[20],
			   rate_stats[18], rate_stats[16],
			   rate_stats[14], rate_stats[12]);
		break;
	}

	return len;
}

static int slsi_copy_ap_sta_info_to_buf(struct netdev_vif *ndev_vif, char *command, int buf_len,
					int len, struct slsi_ap_sta_info *peer_info)
{
	if (buf_len <= len)
		return len;

	len += snprintf(&command[len], (buf_len - len),
		   "GETSTAINFO %pM Rx_Retry_Pkts=%d Rx_BcMc_Pkts=%d CAP=%04x %02x:%02x:%02x ",
		   peer_info->address,
		   peer_info->rx_retry_packets,
		   peer_info->rx_bc_mc_packets,
		   peer_info->capabilities,
		   peer_info->address[0],
		   peer_info->address[1],
		   peer_info->address[2]);

	if (buf_len <= len)
		return len;

	len += snprintf(&command[len], (buf_len - len), "%d %d %d %d %d %d %d %u %d %d ",
		   ieee80211_frequency_to_channel(ndev_vif->ap.channel_freq),
		   peer_info->bandwidth, peer_info->rssi,
		   peer_info->tx_data_rate, peer_info->support_mode,
		   peer_info->antenna_mode,
		   peer_info->mimo_used, peer_info->reason,
		   peer_info->supported_band,
		   peer_info->avg_antenna_rssi);

	len = slsi_copy_ap_sta_info_stats_to_buf(command, buf_len, len, peer_info->support_mode,
						 peer_info->tx_rate_stats);

	if (buf_len <= len)
		return len;

	len += snprintf(&command[len], (buf_len - len), "%d %d %d ",
		   peer_info->tx_success_packets, peer_info->tx_failure_packets,
		   peer_info->tx_retry_packets);

	len = slsi_copy_ap_sta_info_stats_to_buf(command, buf_len, len, peer_info->support_mode,
						 peer_info->rx_rate_stats);

	if (buf_len <= len)
		return len;

	len += snprintf(&command[len], (buf_len - len), "%d %d %d\n",
		   peer_info->rx_success_packets, peer_info->rx_failure_packets,
		   peer_info->rx_retry_packets);

	return len;
}

int slsi_get_sta_info(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif   *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	int                     len = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;
	struct slsi_ap_sta_info sta_info;
#ifdef CONFIG_SCSC_WLAN_WIFI_SHARING
	struct net_device *ap_dev;
	struct netdev_vif *ndev_ap_vif;

	ap_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX_SWLAN);

	if (ap_dev) {
		ndev_ap_vif = netdev_priv(ap_dev);
		SLSI_MUTEX_LOCK(ndev_ap_vif->vif_mutex);
		if (SLSI_IS_VIF_INDEX_MHS(sdev, ndev_ap_vif))
			ndev_vif = ndev_ap_vif;
		SLSI_MUTEX_UNLOCK(ndev_ap_vif->vif_mutex);
	}
#endif

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_AP) {
		SLSI_ERR(sdev, "slsi_get_sta_info: AP is not up.Command not allowed vif.activated:%d, vif.type:%d\n",
			 ndev_vif->activated, ndev_vif->vif_type);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);

	if (!ioctl_args) {
		SLSI_ERR(sdev, "Malloc of ioctl_args failed.\n");
		return -ENOMEM;
	}

	if (!ioctl_args->arg_count) {
		/* If hotstapd send AP-STA-REMOVE to upper layer(framework),
		 * then framework will call GETSTAINFO.
		 * Then log the info of the last disconnected station that inclue disconnection reason.
		 */
		kfree(ioctl_args);
		SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

		len += slsi_copy_ap_sta_info_to_buf(ndev_vif, command, buf_len, len,
						    &ndev_vif->ap.last_disconnected_sta);

		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

		return len;
	}

	if (ioctl_args->arg_count != 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	memset(&sta_info, 0, sizeof(sta_info));
	if (strcmp("ALL", ioctl_args->args[0]) == 0) {
		int i = 0;

		for (i = 0; i < SLSI_PEER_INDEX_MAX; i++) {
			SLSI_INFO(sdev, "%02X:%02X:%02X:%02X:%02X:%02X - valid:%d, buf_len:%d, len:%d\n",
				  ndev_vif->peer_sta_record[i]->address[0], ndev_vif->peer_sta_record[i]->address[1],
				  ndev_vif->peer_sta_record[i]->address[2], ndev_vif->peer_sta_record[i]->address[3],
				  ndev_vif->peer_sta_record[i]->address[4], ndev_vif->peer_sta_record[i]->address[5],
				  ndev_vif->peer_sta_record[i]->valid, buf_len, len);
			if (ndev_vif->peer_sta_record[i] && ndev_vif->peer_sta_record[i]->valid &&
			    slsi_fill_ap_sta_info(sdev, dev, ndev_vif->peer_sta_record[i]->address,
						  &sta_info, 0) == 0) {
				len += slsi_copy_ap_sta_info_to_buf(ndev_vif, command, buf_len, len, &sta_info);
				if (buf_len <= len)
					break;
				memset(&sta_info, 0, sizeof(sta_info));
			}
		}
	} else {
		int r = 0;
		u8 u[ETH_ALEN];

		r = sscanf(ioctl_args->args[0], "%c:%c:%c:%c:%c:%c",
			   &u[0], &u[1], &u[2], &u[3], &u[4], &u[5]);
		if (r != ETH_ALEN) {
			SLSI_ERR(sdev, "mac_addr format is incorrect (should be e.g. xx:xx:xx:xx:xx:xx)\n");
			SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
		if (slsi_fill_ap_sta_info(sdev, dev, u, &sta_info, 0) == 0)
			len += slsi_copy_ap_sta_info_to_buf(ndev_vif, command, buf_len, len, &sta_info);
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	kfree(ioctl_args);
	return len;
}

static int slsi_get_bss_rssi(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif        *ndev_vif = netdev_priv(dev);
	int len = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	len = snprintf(command, buf_len, "%d", ndev_vif->sta.last_connected_bss.rssi);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return len;
}

static int slsi_get_bss_info(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif        *ndev_vif = netdev_priv(dev);
	int len = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	len = snprintf(command, buf_len, "%02x:%02x:%02x %u %u %d %u %u %u %u %u %d %d %u %u %u %u",
		       ndev_vif->sta.last_connected_bss.address[0],  ndev_vif->sta.last_connected_bss.address[1],
		       ndev_vif->sta.last_connected_bss.address[2],
		       ndev_vif->sta.last_connected_bss.channel_freq, ndev_vif->sta.last_connected_bss.bandwidth,
		       ndev_vif->sta.last_connected_bss.rssi, ndev_vif->sta.last_connected_bss.tx_data_rate,
		       ndev_vif->sta.last_connected_bss.mode, ndev_vif->sta.last_connected_bss.antenna_mode,
		       ndev_vif->sta.last_connected_bss.mimo_used,
		       ndev_vif->sta.last_connected_bss.passpoint_version, ndev_vif->sta.last_connected_bss.snr,
		       ndev_vif->sta.last_connected_bss.noise_level, ndev_vif->sta.last_connected_bss.roaming_akm,
		       ndev_vif->sta.last_connected_bss.roaming_count, ndev_vif->sta.last_connected_bss.kv,
		       ndev_vif->sta.last_connected_bss.kvie);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return len;
}

static int slsi_get_cu(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int mib_value = 0;
	int res = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not Activated or Not STA mode or STA is not in Connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		res = snprintf(command, buf_len, "-1\n");
		return res;
	}

	res = slsi_get_beacon_cu(sdev, dev, &mib_value);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (res)
		return res;

	res = snprintf(command, buf_len, "%d\n", mib_value);
	return res;
}

#ifdef CONFIG_SCSC_WLAN_MAX_LINK_SPEED
static int slsi_get_linkspeed(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	int len = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated) {
		SLSI_ERR(sdev, "Not Activated, Command not allowed vif.activated:%d, vif.type:%d\n",
			 ndev_vif->activated, ndev_vif->vif_type);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	if (ndev_vif->vif_type != FAPI_VIFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EPERM;
	}

	len = snprintf(command, buf_len, "MAX_SPEED %u CURRENT_LINK_SPEED %u",
		       ndev_vif->sta.max_rate_mbps, ndev_vif->sta.data_rate_mbps);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return len;
}
#endif

static int slsi_get_assoc_reject_info(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	int len = 0;

	len = snprintf(command, buf_len, "assoc_reject.status : %d %s", sdev->assoc_result_code,
		       slsi_get_assoc_status(sdev->assoc_result_code));

	return len;
}

#ifdef CONFIG_SCSC_WLAN_LOW_LATENCY_MODE
static int slsi_set_power_mode(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                  status;
	u16                  power_mode;
	int                  mode;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	status = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (status)
		return status;

	if (!slsi_str_to_int(ioctl_args->args[0], &mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (mode != 0 && mode != 1) {
		SLSI_ERR(sdev, "Invalid power_mode: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_NET_INFO(dev, "PS MODE mode:%d, vif_type:%d, vif_index:%d\n", mode, ndev_vif->vif_type,
		      ndev_vif->ifnum);

	power_mode = (mode == 0) ? FAPI_POWERMANAGEMENTMODE_ACTIVE_MODE : FAPI_POWERMANAGEMENTMODE_POWER_SAVE;
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_STATION ||
	    !(ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_ERR(sdev, "Command not allowed vif.activated:%d, vif.type:%d, ndev_vif->sta.vif_status:%d\n",
			 ndev_vif->activated, ndev_vif->vif_type, ndev_vif->sta.vif_status);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}
	status = slsi_mlme_powermgt(sdev, dev, power_mode);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	kfree(ioctl_args);
	return status;
}
#endif

int slsi_set_bandwidth(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int bandwidth = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &bandwidth)) {
		SLSI_ERR(sdev, "Invalid bandwidth string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	SLSI_DBG1(sdev, SLSI_CFG80211, "Bandwidth = %d\n", bandwidth);

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (bandwidth == 20 || bandwidth == 40 || bandwidth == 80 || bandwidth == 160) {
		sdev->forced_bandwidth = bandwidth;
	} else {
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -EINVAL;
	}

	ret = slsi_mlme_set_country(sdev, sdev->device_config.domain_info.regdomain->alpha2);
	sdev->forced_bandwidth = 0;
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	if (ret != 0)
		SLSI_NET_ERR(dev, "Error in setting the Country, ret=%d", ret);

	kfree(ioctl_args);
	return ret;
}

int slsi_set_bss_bandwidth(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	struct sk_buff         *req;
	struct sk_buff         *cfm;
	int                    bandwidth = 0;
	int                    protection_scope = 0;
	int                    ret = 0;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 2);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_is_bw_valid(ioctl_args->args[0])) {
		SLSI_ERR(sdev, "Unexpected string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &bandwidth)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[1], &protection_scope)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	if (protection_scope != 0 && protection_scope != 1) {
		SLSI_ERR(sdev, "Invalid value of protection_scope: '%d'\n", protection_scope);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Must be STA interface and connected, iftype = %d\n", ndev_vif->iftype);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EPERM;
	}

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_set_bss_max_channel_width_req(vif:%u, bw:%d, protection_scope:%x)\n",
		      ndev_vif->ifnum, bandwidth, protection_scope);

	req = fapi_alloc(mlme_set_bss_max_channel_width_req, MLME_SET_BSS_MAX_CHANNEL_WIDTH_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EIO;
	}

	fapi_set_u16(req, u.mlme_set_bss_max_channel_width_req.protection_scope, protection_scope);
	fapi_set_u16(req, u.mlme_set_bss_max_channel_width_req.channel_information, bandwidth);

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_BSS_MAX_CHANNEL_WIDTH_CFM);
	if (!cfm) {
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_set_bss_max_channel_width_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_set_bss_max_channel_width_cfm(result:0x%04x) ERROR\n",
			     fapi_get_u16(cfm, u.mlme_set_bss_max_channel_width_cfm.result_code));
		ret = -EINVAL;
	}

	kfree_skb(cfm);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	kfree(ioctl_args);
	return ret;
}

int slsi_set_disconnect_ies(struct net_device *dev, char *cmd, int cmd_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev         *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args  *ioctl_args = NULL;
	char                    *disconnect_ies = cmd + strlen(CMD_SET_DISCONNECT_IES) + 1;
	int                     ie_len = 0;
	u8                      *disconnect_ies_bin;
	u8                      temp_byte;
	int                     i;
	int                     j;
	int                     len;
	int                     ret = 0;

	SLSI_DBG1(sdev, SLSI_CFG80211, "Setting disconnect IE's\n");
	ioctl_args = slsi_get_private_command_args(cmd, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	disconnect_ies = ioctl_args->args[0];
	ie_len = strlen(ioctl_args->args[0]);

	/* ie_len has been trimmed to even, as odd length would mean that ie is invalid */
	ie_len &= (~0x01);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ndev_vif->sta.vendor_disconnect_ies_len = (ie_len / 2);
	disconnect_ies_bin = kmalloc(ndev_vif->sta.vendor_disconnect_ies_len, GFP_KERNEL);
	if (!disconnect_ies_bin) {
		SLSI_ERR(sdev, "Malloc  failed\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -ENOMEM;
	}

	for (i = 0, j = 0; j < ie_len; j += 2) {
		temp_byte = slsi_parse_hex(disconnect_ies[j]) << 4 | slsi_parse_hex(disconnect_ies[j + 1]);
		disconnect_ies_bin[i++] = temp_byte;
	}

	/* check if the IE is valid */
	for (i = 0; i < ndev_vif->sta.vendor_disconnect_ies_len;) {
		if (disconnect_ies_bin[i] == 0xdd) {
			len = disconnect_ies_bin[i + 1];
			if ((ndev_vif->sta.vendor_disconnect_ies_len - (i + 2)) < len) {
				SLSI_WARN(sdev, "The length of disconnect IE's is not proper\n");
				ndev_vif->sta.vendor_disconnect_ies_len = 0;
				kfree(disconnect_ies_bin);
				SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
				kfree(ioctl_args);
				return -EINVAL;
			}

			i = i + 2 + len;
		} else {
			SLSI_WARN(sdev, "The tag of disconnect IE's is not proper\n");
			ndev_vif->sta.vendor_disconnect_ies_len = 0;
			kfree(disconnect_ies_bin);
			SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
			kfree(ioctl_args);
			return -EINVAL;
		}
	}
	ndev_vif->sta.vendor_disconnect_ies = disconnect_ies_bin;
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	kfree(ioctl_args);
	return 0;
}

static int slsi_start_power_measurement_detection(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	u8                   device_address[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int                  r = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (sdev->detect_vif_active) {
		SLSI_DBG1(sdev, SLSI_CFG80211, "Detect vif already active\n");
		r = -EINVAL;
		goto exit_with_vif_mutex;
	}

	if (slsi_mlme_add_detect_vif(sdev, dev, dev->dev_addr, device_address) != 0) {
		SLSI_NET_ERR(dev, "slsi_mlme_add_vif for detect vif failed\n");
		r = -EINVAL;
		goto exit_with_vif_mutex;
	}

	sdev->detect_vif_active = true;
	SLSI_DBG1(sdev, SLSI_CFG80211, "Starting Power Measurement Detection\n");
	r = slsi_mlme_start_detect_request(sdev, dev);
	if (r) {
		r = -EINVAL;
		if (slsi_mlme_del_detect_vif(sdev, dev) != 0)
			SLSI_NET_ERR(dev, "slsi_mlme_del_vif failed for detect vif\n");
		sdev->detect_vif_active = false;
	}

exit_with_vif_mutex:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

#ifdef CONFIG_SCSC_WLAN_STA_ENHANCED_ARP_DETECT
static int slsi_enhanced_arp_start_stop(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif    *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int result = 0;
	int readbyte = 0;
	int readvalue = 0;
	int i = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (!slsi_str_to_int(ioctl_args->args[0], &readvalue)) {
		SLSI_ERR(sdev, "Invalid start/stop string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	if (!sdev->device_config.fw_enhanced_arp_detect_supported) {
		SLSI_ERR(sdev, "Enhanced ARP Detect Feature is not supported.\n");
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		kfree(ioctl_args);
		return -ENOTSUPP;
	}
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->vif_type != FAPI_VIFTYPE_STATION) {
		SLSI_ERR(sdev, "Not in STA mode\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		kfree(ioctl_args);
		return -EPERM;
	}

	SLSI_DBG1(sdev, SLSI_CFG80211, "Enhanced ARP Start/Stop\n");

	memset(ndev_vif->target_ip_addr, 0, sizeof(ndev_vif->target_ip_addr));
	if (readvalue != 0) { /* parse IP address */
		for (i = 0; i < 4 ; i++) {
			readbyte = slsi_str_to_int(ioctl_args->args[0], &readvalue);
			if (!readbyte) {
				SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
				memset(ndev_vif->target_ip_addr, 0, sizeof(ndev_vif->target_ip_addr));
				SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
				kfree(ioctl_args);
				return -EINVAL;
			}
			if (readvalue < 0 || readvalue > 255) {
				SLSI_ERR(sdev, "Invalid value of IP address byte\n");
				memset(ndev_vif->target_ip_addr, 0, sizeof(ndev_vif->target_ip_addr));
				SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
				kfree(ioctl_args);
				return -EINVAL;
			}
			ndev_vif->target_ip_addr[i] = readvalue;
			if (i == 3)
				break;
			if (strlen(ioctl_args->args[0]) - readbyte - 1 <= 0) {
				SLSI_ERR(sdev, "Invalid IP address\n");
				memset(ndev_vif->target_ip_addr, 0, sizeof(ndev_vif->target_ip_addr));
				SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
				kfree(ioctl_args);
				return -EINVAL;
			}
			ioctl_args->args[0] = ioctl_args->args[0] + readbyte + 1;
		}
	}

	if (ndev_vif->target_ip_addr[0] != 0) { /* start enhanced arp detect */
		/* reset all the counters in host and firmware */
		slsi_read_enhanced_arp_rx_count_by_lower_mac(sdev, dev, SLSI_PSID_UNIFI_ARP_DETECT_RESPONSE_COUNTER);
		memset(&ndev_vif->enhanced_arp_stats, 0, sizeof(ndev_vif->enhanced_arp_stats));
		memset(ndev_vif->enhanced_arp_host_tag, 0, sizeof(ndev_vif->enhanced_arp_host_tag));
		ndev_vif->enhanced_arp_detect_enabled = true;
		result = slsi_mlme_arp_detect_request(sdev, dev, FAPI_ACTION_START, ndev_vif->target_ip_addr);
	} else { /* stop enhanced arp detect */
		ndev_vif->enhanced_arp_detect_enabled = false;
		result = slsi_mlme_arp_detect_request(sdev, dev, FAPI_ACTION_STOP, ndev_vif->target_ip_addr);
	}

	kfree(ioctl_args);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return result;
}

static int slsi_enhanced_arp_get_stats(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif                      *ndev_vif = netdev_priv(dev);
	struct slsi_dev      *sdev = ndev_vif->sdev;
	int                                    r = 0;
	int                                    len = 0;

	r = slsi_read_enhanced_arp_rx_count_by_lower_mac(sdev, dev, SLSI_PSID_UNIFI_ARP_DETECT_RESPONSE_COUNTER);

	if (r == 0) {
		len = snprintf(command, buf_len, "%d %d %d %d %d %d %d %d %d %d",
			       ndev_vif->enhanced_arp_stats.arp_req_count_from_netdev,
			       ndev_vif->enhanced_arp_stats.arp_req_count_to_lower_mac,
			       ndev_vif->enhanced_arp_stats.arp_req_rx_count_by_lower_mac,
			       ndev_vif->enhanced_arp_stats.arp_req_count_tx_success,
			       ndev_vif->enhanced_arp_stats.arp_rsp_rx_count_by_lower_mac,
			       ndev_vif->enhanced_arp_stats.arp_rsp_rx_count_by_upper_mac,
			       ndev_vif->enhanced_arp_stats.arp_rsp_count_to_netdev,
			       ndev_vif->enhanced_arp_stats.arp_rsp_count_out_of_order_drop,
			       0, /*ap_link_active not supported, set as 0*/
			       ndev_vif->enhanced_arp_stats.is_duplicate_addr_detected);
		SLSI_DBG2(sdev, SLSI_CFG80211, "Enhanced ARP Stats: %s\n", command);
	}

	/*clear all the counters*/
	memset(&ndev_vif->enhanced_arp_stats, 0, sizeof(ndev_vif->enhanced_arp_stats));

	return len;
}
#endif

static int slsi_ioctl_auto_chan_write(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int ret = 0;

	ret = slsi_auto_chan_write(dev, command, cmd_len);
	ndev_vif->acs = true;
	return ret;
}

#ifdef CONFIG_SCSC_WLAN_HANG_TEST
static int slsi_ioctl_test_force_hang(struct net_device *dev, char *command, int cmd_len)
{
	return slsi_test_send_hanged_vendor_event(dev);
}
#endif

#ifdef CONFIG_SCSC_WLAN_LOW_LATENCY_MODE
static int slsi_ioctl_set_latency_mode(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int latency_mode = 0;
	int ret = 0;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &latency_mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		ret = -EINVAL;
	} else {
		if (latency_mode < 0 || latency_mode > 2) {
			SLSI_ERR(sdev, "Invalid latency_mode: '%s'\n", ioctl_args->args[0]);
			ret = -EINVAL;
		} else {
			ret = slsi_set_latency_mode(dev, latency_mode, cmd_len);
		}
	}
	kfree(ioctl_args);
	return ret;
}
#endif

static int slsi_ioctl_set_latency_crt_data(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    latency_mode = 0;
	int                    ret = 0;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &latency_mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		ret = -EINVAL;
	} else {
		if (latency_mode < 0 || latency_mode > 3) {
			SLSI_ERR(sdev, "Invalid latency_crt_data: '%s'\n", ioctl_args->args[0]);
			ret = -EINVAL;
		} else {
			ret = slsi_set_latency_crt_data(dev, latency_mode);
		}
	}
	kfree(ioctl_args);
	return ret;
}

#ifndef SLSI_TEST_DEV
static int slsi_ioctl_driver_bug_dump(struct net_device *dev, char *command, int cmd_len)
{
	int ret = 0;

	slsi_dump_stats(dev);
#ifdef CONFIG_SCSC_LOG_COLLECTION
	scsc_log_collector_schedule_collection(SCSC_LOG_DUMPSTATE, SCSC_LOG_DUMPSTATE_REASON_DRIVERDEBUGDUMP);
#else
#ifndef SLSI_TEST_DEV
#if IS_ENABLED(CONFIG_SCSC_INDEPENDENT_SUBSYSTEM)
	SLSI_NET_INFO(dev, "SCSC_LOG_COLLECTION not enabled. Sable will not be triggered\n");
#else
	ret = mx140_log_dump();
#endif
#endif
#endif
	return ret;
}
#endif

#ifdef CONFIG_SCSC_WLAN_NUM_ANTENNAS
static int slsi_ioctl_get_num_antennas(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               num_antennas = 0;
	int               res = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	res = slsi_mlme_get_num_antennas(sdev, dev, &num_antennas);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (res)
		return res;

	res = snprintf(command, buf_len, "%d\n", num_antennas);
	return res;
}

static int slsi_ioctl_set_num_antennas(struct net_device *dev, char *command, int cmd_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int num_of_antennas;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int ret = 0;
	int frame_type = 0;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 2);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &num_of_antennas)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		ret = -EINVAL;
	} else {
		/* We cannot lock in slsi_set_num_antennas as
		 * this is also called in slsi_start_ap with netdev_vif lock.
		 */
		if (ioctl_args->arg_count == 2) {
			if (!slsi_str_to_int(ioctl_args->args[1], &frame_type)) {
				SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[1]);
				ret = -EINVAL;
				goto exit;
			}
			frame_type += 1;
		}
		SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
		ret = slsi_mlme_set_num_antennas(dev, num_of_antennas, frame_type);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	}
exit:
	kfree(ioctl_args);
	return ret;
}
#endif

static int slsi_elna_bypass(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif        = netdev_priv(dev);
	struct slsi_dev   *sdev            = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value                      = 0;
	int ret                            = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	kfree(ioctl_args);
	if (mib_value != 0 && mib_value != 1) {
		SLSI_ERR(sdev, "Invalid LNA control: '%d'\n", mib_value);
		return -EINVAL;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EPERM;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_LNA_CONTROL_ENABLED, mib_value);
}

static int slsi_elna_bypass_int(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif        = netdev_priv(dev);
	struct slsi_dev   *sdev            = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value                      = 0;
	int ret                            = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	kfree(ioctl_args);
	if (mib_value < 0 || mib_value > 65535) {
		SLSI_ERR(sdev, "Invalid LNA Eval Intervel: '%d'\n", mib_value);
		return -EINVAL;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EPERM;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_LNA_CONTROL_EVALUATION_INTERVAL, mib_value);
}

static int slsi_set_dwell_time(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int passive_time = 0;
	int home_time = 0;
	int scan_channel_time = 0;
	int home_away_time = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int ret = 0;
	int reset = 1;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 4);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &passive_time)) {
		SLSI_ERR(sdev, "Invalid string passive_time: '%s'\n", ioctl_args->args[0]);
		ret = -EINVAL;
	} else if (passive_time < 0 || passive_time > 0xFFFF) {
		SLSI_ERR(sdev, "Invalid passive_time value: '%d'\n", passive_time);
		ret = -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[1], &home_time)) {
		SLSI_ERR(sdev, "Invalid string home_time: '%s'\n", ioctl_args->args[1]);
		ret = -EINVAL;
	} else if (home_time < 0 || home_time > 0xFFFF) {
		SLSI_ERR(sdev, "Invalid home_time value: '%d'\n", home_time);
		ret = -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[2], &scan_channel_time)) {
		SLSI_ERR(sdev, "Invalid string scan_channel_time: '%s'\n", ioctl_args->args[2]);
		ret = -EINVAL;
	} else if (scan_channel_time < 0 || scan_channel_time > 0xFFFF) {
		SLSI_ERR(sdev, "Invalid scan_channel_time value: '%d'\n", scan_channel_time);
		ret = -EINVAL;
	}

	if (!slsi_str_to_int(ioctl_args->args[3], &home_away_time)) {
		SLSI_ERR(sdev, "Invalid string scan_channel_time: '%s'\n", ioctl_args->args[3]);
		ret = -EINVAL;
	} else if (home_away_time < 0 || home_away_time > 0xFFFF) {
		SLSI_ERR(sdev, "Invalid scan_channel_time value: '%d'\n", home_away_time);
		ret = -EINVAL;
	}

	if (ret != 0)
		goto exit;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	sdev->home_away_time = home_away_time;
	sdev->home_time = home_time;
	sdev->max_channel_time = scan_channel_time;
	sdev->max_channel_passive_time = passive_time;
	/* Set all 4 bits*/
	if (home_away_time != 0 && home_time != 0  && scan_channel_time != 0 && passive_time != 0)
		sdev->latency_param_mask = LATENCY_ALL_SET_MASK;
	else
		reset = 0;
	ret = slsi_set_low_latency_params(dev, reset);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
exit:
	kfree(ioctl_args);
	return ret;
}

static int slsi_max_dtim_suspend(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif        = netdev_priv(dev);
	struct slsi_dev   *sdev            = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value                      = 0;
	int ret                            = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	kfree(ioctl_args);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EPERM;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (mib_value < 0 || mib_value > 1) {
		SLSI_ERR(sdev, "Invalid Max DTIM Suspend: '%d'\n", mib_value);
		return -EINVAL;
	}
	if (mib_value == 0) {
		sdev->max_dtim_recv = true;
	} else if (mib_value == 1) {
		sdev->max_dtim_recv = false;
		ret = slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_USE_HOST_LISTEN_INTERVAL, 0);
	}

	return ret;
}

static int slsi_set_dtim_suspend(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif        = netdev_priv(dev);
	struct slsi_dev   *sdev            = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int mib_value                      = 0;
	int ret                            = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (!slsi_str_to_int(ioctl_args->args[0], &mib_value)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	kfree(ioctl_args);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EPERM;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (!sdev->max_dtim_recv) {
		SLSI_ERR(sdev, "Max DTIM Suspend = 0 not received\n");
		return -EPERM;
	}

	sdev->max_dtim_recv = false;

	if (mib_value < 0) {
		SLSI_ERR(sdev, "Invalid Set DTIM Suspend val: '%d'\n", mib_value);
		return -EINVAL;
	}

	ret = slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_USE_HOST_LISTEN_INTERVAL, mib_value);
	return ret;
}

static int slsi_force_roaming_bssid(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	u8                     bssid[6] = { 0 };
	int                    channel;
	int                    freq;
	enum nl80211_band      band = NL80211_BAND_2GHZ;
	int                    ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[0]));
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);
	if (!slsi_str_to_int(ioctl_args->args[1], &channel)) {
		SLSI_ERR(sdev, "Invalid channel string: '%s'\n", ioctl_args->args[1]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	kfree(ioctl_args);
	SLSI_NET_DBG1(dev, SLSI_NETDEV, "Force Roam: " MACSTR " Chan: %d\n",
		      MAC2STR(bssid), channel);

	/* Check in 4 blacklists */
	if (slsi_is_bssid_in_blacklist(sdev, dev, bssid) ||
	    slsi_is_bssid_in_hal_blacklist(dev, bssid) ||
	    slsi_is_bssid_in_ioctl_blacklist(dev, bssid)) {
		SLSI_ERR(sdev, "Requested BSSID is in blacklist\n");
		return -EINVAL;
	}

	if (channel < 1 || channel > SLSI_5GHZ_LAST_CHAN) {
		SLSI_ERR(sdev, "Invalid channel : %d\n", channel);
		return -EINVAL;
	}

	if (channel > 14)
		band = NL80211_BAND_5GHZ;
	freq = (u16)ieee80211_channel_to_frequency(channel, band);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_roam(sdev, dev, bssid, freq);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

static int slsi_roaming_blacklist_add(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	u8                     bssid[6] = { 0 };
	int                    ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[0]));
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);
	kfree(ioctl_args);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	/* Check if BSSID is already present in list */
	if (slsi_is_bssid_in_ioctl_blacklist(dev, bssid)) {
		SLSI_ERR(sdev, "Requested BSSID already in blacklist\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return ret;
	}
	ret = slsi_add_ioctl_blacklist(sdev, dev, bssid);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

static int slsi_roaming_blacklist_remove(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;
	u8                     bssid[6] = { 0 };
	int                    ret = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	if (strlen(ioctl_args->args[0]) != 17) {
		SLSI_ERR(sdev, "Invalid MAC address length :%d\n", (int)strlen(ioctl_args->args[0]));
		kfree(ioctl_args);
		return -EINVAL;
	}

	slsi_machexstring_to_macarray(ioctl_args->args[0], bssid);
	kfree(ioctl_args);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_remove_bssid_blacklist(sdev, dev, bssid);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

static int slsi_ioctl_get_ps_disabled_duration(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev         *sdev = ndev_vif->sdev;
	int                     mib_value = 0;
	int                     res = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not Activated or Not STA mode or STA is not in Connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	res = slsi_get_ps_disabled_duration(sdev, dev, &mib_value);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (res)
		return res;

	res = snprintf(command, buf_len, "%d\n", mib_value);
	return res;
}

static int slsi_ioctl_get_ps_entry_counter(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev         *sdev = ndev_vif->sdev;
	int                     mib_value = 0;
	int                     res = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not Activated or Not STA mode or STA is not in Connected state\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	res = slsi_get_ps_entry_counter(sdev, dev, &mib_value);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	if (res)
		return res;

	res = snprintf(command, buf_len, "%d\n", mib_value);
	return res;
}

static int slsi_ioctl_cmd_success(struct net_device *dev, char *command, int cmd_len)
{
	return 0;
}

struct slsi_ioctl_fn {
	char *name;
	int (*fn)(struct net_device *dev, char *command, int cmd_len);
};

static int slsi_twt_setup(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_vif_sta    *sta;
	struct twt_setup       tsetup = {0};
	int                    r, ret = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    i, setup_id_idx = SLSI_MAX_NUMBER_SETUP_ID;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 13);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r) {
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
		return ret;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype == NL80211_IFTYPE_UNSPECIFIED ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		ret = SLSI_TWT_WIFI_ERROR_NOT_AVAILABLE;
		goto exit;
	}
	if (ioctl_args->arg_count < 13) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &tsetup.setup_id) || tsetup.setup_id <= 0) {
		SLSI_ERR(sdev, "Invalid set up string: '%s' value %d\n", ioctl_args->args[0], tsetup.setup_id);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &tsetup.negotiation_type) ||
	    tsetup.negotiation_type < 0 || tsetup.negotiation_type > 1) {
		SLSI_ERR(sdev, "Invalid negotiation type string: '%s'value %d\n",
			 ioctl_args->args[1], tsetup.negotiation_type);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[2], &tsetup.flow_type)) {
		SLSI_ERR(sdev, "Invalid flow type string: '%s'\n", ioctl_args->args[2]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[3], &tsetup.trigger_type)) {
		SLSI_ERR(sdev, "Invalid trigger_type string: '%s'\n", ioctl_args->args[3]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[4], &tsetup.d_wake_duration) ||
	    tsetup.d_wake_duration < TWT_D_MIN_WAKE_DURATION || tsetup.d_wake_duration > TWT_D_MAX_WAKE_DURATION) {
		SLSI_ERR(sdev, "Invalid desired wake duration string: '%s' value %d\n",
			 ioctl_args->args[4], tsetup.d_wake_duration);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[5], &tsetup.d_wake_interval) ||
	    tsetup.d_wake_interval < TWT_D_MIN_WAKE_INTERVAL || tsetup.d_wake_interval > TWT_D_MAX_WAKE_INTERVAL) {
		SLSI_ERR(sdev, "Invalid desired wake interval string: '%s' value %d\n",
			 ioctl_args->args[5], tsetup.d_wake_interval);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[6], &tsetup.d_wake_time)) {
		SLSI_ERR(sdev, "Invalid desired wake time string: '%s'\n", ioctl_args->args[6]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[7], &tsetup.min_wake_interval)) {
		SLSI_ERR(sdev, "Invalid desired wake interval string: '%s'\n", ioctl_args->args[7]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[8], &tsetup.max_wake_interval)) {
		SLSI_ERR(sdev, "Invalid desired wake interval string: '%s'\n", ioctl_args->args[8]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[9], &tsetup.min_wake_duration)) {
		SLSI_ERR(sdev, "Invalid desired wake interval string: '%s'\n", ioctl_args->args[9]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[10], &tsetup.max_wake_duration)) {
		SLSI_ERR(sdev, "Invalid desired wake interval string: '%s'\n", ioctl_args->args[10]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[11], &tsetup.avg_pkt_num)) {
		SLSI_ERR(sdev, "Invalid Average packet number string: '%s'\n", ioctl_args->args[11]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[12], &tsetup.avg_pkt_size)) {
		SLSI_ERR(sdev, "Invalid Average packet size string: '%s'\n", ioctl_args->args[12]);
		goto exit;
	}
	sta = &ndev_vif->sta;
	if (!sta->twt_allowed) {
		SLSI_ERR(sdev, "HE Capability not supported\n");
		ret = SLSI_TWT_WIFI_ERROR_NOT_SUPPORTED;
		goto exit;
	}
	if ((tsetup.min_wake_duration != -1 && tsetup.d_wake_duration < tsetup.min_wake_duration) ||
	    (tsetup.max_wake_duration != -1 && tsetup.d_wake_duration > tsetup.max_wake_duration)) {
		SLSI_ERR(sdev, "Desired wake duration is not within expected range.\n");
		goto exit;
	}
	if ((tsetup.min_wake_interval != -1 && tsetup.d_wake_interval < tsetup.min_wake_interval) ||
	    (tsetup.max_wake_interval != -1 && tsetup.d_wake_interval > tsetup.max_wake_interval)) {
		SLSI_ERR(sdev, "Desired wake interval is not within expected range.\n");
		goto exit;
	}
	for (i = 0; i < SLSI_MAX_NUMBER_SETUP_ID; i++) {
		if (sdev->twt_setup_id[i] == tsetup.setup_id) {
			setup_id_idx = i;
			break;
		} else if (!sdev->twt_setup_id[i]) {
			setup_id_idx = i;
		}
	}
	if (setup_id_idx >= SLSI_MAX_NUMBER_SETUP_ID) {
		SLSI_NET_ERR(dev, "Number of twt setup session is more than %d.\n", SLSI_MAX_NUMBER_SETUP_ID);
		ret = -ENOMEM;
		goto exit;
	}
	sdev->twt_setup_id[setup_id_idx] = tsetup.setup_id;
	ret = slsi_mlme_twt_setup(sdev, dev, &tsetup);
	if (ret != 0)
		sdev->twt_setup_id[setup_id_idx] = 0;
exit:
	if (ret < 0)
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_twt_teardown(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_vif_sta    *sta;
	int                    setup_id;
	int                    negotiation_type;
	int                    r, ret = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    setup_id_idx, teardown_count = 0;
	bool                   teardown_success = false;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r) {
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
		return ret;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype == NL80211_IFTYPE_UNSPECIFIED ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "sta is not in connected state\n");
		ret = SLSI_TWT_WIFI_ERROR_NOT_AVAILABLE;
		goto exit;
	}
	if (ioctl_args->arg_count < 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &setup_id)) {
		SLSI_ERR(sdev, "Invalid set up string: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (setup_id < 0) {
		SLSI_ERR(sdev, "Invalid setup id : %d\n", setup_id);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &negotiation_type)) {
		SLSI_ERR(sdev, "Invalid negotiation type string: '%s'\n", ioctl_args->args[1]);
		goto exit;
	}
	if (negotiation_type < 0 || negotiation_type > 1) {
		SLSI_ERR(sdev, "Invalid negotiation type : %d\n", negotiation_type);
		goto exit;
	}
	sta = &ndev_vif->sta;
	if (!sta->twt_allowed) {
		SLSI_ERR(sdev, "HE Capability not supported\n");
		ret = SLSI_TWT_WIFI_ERROR_NOT_SUPPORTED;
		goto exit;
	}
	for (setup_id_idx = 0; setup_id_idx < SLSI_MAX_NUMBER_SETUP_ID; setup_id_idx++) {
		if ((!setup_id && sdev->twt_setup_id[setup_id_idx]) ||
		    (setup_id && sdev->twt_setup_id[setup_id_idx] == setup_id)) {
			ret = slsi_mlme_twt_teardown(sdev, dev, sdev->twt_setup_id[setup_id_idx], negotiation_type);
			if (ret == 0)
				teardown_success = true;
			sdev->twt_setup_id[setup_id_idx] = 0;
			teardown_count++;
		}
	}
	if (teardown_success == true)
		ret = 0;
	if (!teardown_count)
		ret = SLSI_TWT_WIFI_ERROR_NOT_AVAILABLE;
exit:
	if (ret < 0)
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_twt_suspend(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_vif_sta    *sta;
	int                    setup_id;
	int                    suspend_duration;
	int                    r, ret = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r) {
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
		return ret;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ioctl_args->arg_count < 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &setup_id)) {
		SLSI_ERR(sdev, "Invalid set up string: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &suspend_duration)) {
		SLSI_ERR(sdev, "Invalid suspend duration string: '%s'\n", ioctl_args->args[1]);
		goto exit;
	}
	sta = &ndev_vif->sta;
	if (!sta->twt_allowed) {
		SLSI_ERR(sdev, "HE Capability not supported\n");
		ret = SLSI_TWT_WIFI_ERROR_NOT_SUPPORTED;
		goto exit;
	}
	ret = slsi_mlme_twt_suspend(sdev, dev, setup_id, suspend_duration);
exit:
	if (ret < 0)
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_twt_get_status(struct net_device *dev, char *command, int command_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_vif_sta    *sta;
	int                    setup_id;
	int                    r, ret = 0;
	struct slsi_ioctl_args *ioctl_args = NULL;
	int                    setup_id_idx, command_pos = 0, query_count;

	ioctl_args = slsi_get_private_command_args(command, command_len, 1);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r) {
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
		return ret;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ioctl_args->arg_count < 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		ret = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &setup_id)) {
		SLSI_ERR(sdev, "Invalid set up string: '%s'\n", ioctl_args->args[0]);
		ret = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
		goto exit;
	}
	sta = &ndev_vif->sta;
	if (!sta->twt_allowed) {
		SLSI_ERR(sdev, "HE Capability not supported\n");
		ret = SLSI_TWT_WIFI_ERROR_NOT_SUPPORTED;
		goto exit;
	}
	query_count = 0;
	for (setup_id_idx = 0; setup_id_idx < SLSI_MAX_NUMBER_SETUP_ID; setup_id_idx++) {
		if ((!setup_id && sdev->twt_setup_id[setup_id_idx]) ||
		    (setup_id && sdev->twt_setup_id[setup_id_idx] == setup_id)) {
			ret = slsi_mlme_twt_status_query(sdev, dev, command, command_len, &command_pos,
							 sdev->twt_setup_id[setup_id_idx]);
			query_count++;
		}
	}
	if (!query_count)
		command_pos = snprintf(command, command_len, "%d 0 0 0\n", setup_id);
exit:
	if (ret < 0)
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
	else
		ret = command_pos;
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_twt_get_capabilities(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	struct slsi_vif_sta       *sta;
	struct slsi_mib_data      mibrsp = { 0, NULL };
	struct slsi_mib_value     *values = NULL;
	struct slsi_mib_get_entry get_values[] = {{SLSI_PSID_UNIFI_TWT_CONTROL_FLAGS, { 0, 0 } } };
	int                       r = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
	u16                       chipset_cap = 0;
	int                       value;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	sta = &ndev_vif->sta;
	if (!(sta->twt_peer_cap & SLSI_GETCAP_TWT_RESPONDER_SUPPORT)) {
		r = snprintf(command, buf_len, "0x00000000\n");
		goto exit_with_error;
	}
	mibrsp.dataLength = 64;
	mibrsp.data = kmalloc(mibrsp.dataLength, GFP_KERNEL);
	if (!mibrsp.data) {
		SLSI_ERR(sdev, "Cannot kmalloc %d bytes\n", mibrsp.dataLength);
		r = -ENOMEM;
		goto exit_with_error;
	}
	values = slsi_read_mibs(sdev, dev, get_values, 1, &mibrsp);
	if (!values) {
		kfree(mibrsp.data);
		goto exit_with_error;
	}
	if (values[0].type != SLSI_MIB_TYPE_UINT) {
		SLSI_ERR(sdev, "Invalid type (%d) for SLSI_PSID_UNIFI_TWT_CONTROL_FLAGS\n", values[0].type);
		kfree(mibrsp.data);
		kfree(values);
		goto exit_with_error;
	}
	value = values[0].u.uintValue;

	if (value & SLSI_TWT_FW_REQUESTER_SUPPORT)
		chipset_cap |= SLSI_GETCAP_TWT_REQUESTER_SUPPORT;

	if (value & SLSI_TWT_FW_RESPONDER_SUPPORT)
		chipset_cap |= SLSI_GETCAP_TWT_RESPONDER_SUPPORT;

	if (value & SLSI_TWT_FW_BROADCAST_SUPPORT)
		chipset_cap |= SLSI_GETCAP_BROADCAST_TWT_SUPPORT;

	if (value & SLSI_TWT_FW_FLEXIBLE_SUPPORT)
		chipset_cap |= SLSI_GETCAP_FLEXIBLE_TWT_SUPPORT;

	r = snprintf(command, buf_len, "0x%04x%04x\n", chipset_cap, sta->twt_peer_cap);
	kfree(values);
	kfree(mibrsp.data);

exit_with_error:
	if (r < 0)
		SLSI_ERR(sdev, "twt_error_code = %d\n", r);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

static int slsi_twt_get_statistics_query(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	struct slsi_vif_sta       *sta;
	int                       setup_id;
	u32                       data[5];
	int                       r, ret = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
	int                       i, j;
	struct slsi_ioctl_args    *ioctl_args = NULL;
	struct slsi_mib_data      mibrsp = { 0, NULL };
	struct slsi_mib_value     *values = NULL;
	struct slsi_mib_get_entry get_values[5] = {
		{SLSI_PSID_UNIFI_TWT_AVG_EARLY_SP_TERM, { 0, 0 }},
		{SLSI_PSID_UNIFI_TWT_AVG_TX_PKT_NUM, { 0, 0 }},
		{SLSI_PSID_UNIFI_TWT_AVG_RX_PKT_NUM, { 0, 0 }},
		{SLSI_PSID_UNIFI_TWT_AVG_TX_PKT_SIZE, { 0, 0 }},
		{SLSI_PSID_UNIFI_TWT_AVG_RX_PKT_SIZE, { 0, 0 }}
	};

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r) {
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
		return ret;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ioctl_args->arg_count < 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit_with_error;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &setup_id)) {
		SLSI_ERR(sdev, "Invalid set up string: '%s'\n", ioctl_args->args[0]);
		goto exit_with_error;
	}
	sta = &ndev_vif->sta;
	if (!sta->twt_allowed) {
		SLSI_ERR(sdev, "HE Capability not supported\n");
		ret = SLSI_TWT_WIFI_ERROR_NOT_SUPPORTED;
		goto exit_with_error;
	}

	for (j = 0; j < ARRAY_SIZE(get_values); j++)
		get_values[j].index[0] = setup_id;
	mibrsp.dataLength = 32 * ARRAY_SIZE(get_values);
	mibrsp.data = kmalloc(mibrsp.dataLength, GFP_KERNEL);
	if (!mibrsp.data) {
		SLSI_ERR(sdev, "Cannot kmalloc %d bytes\n", mibrsp.dataLength);
		ret = -ENOMEM;
		goto exit_with_error;
	}
	values = slsi_read_mibs(sdev, dev, get_values, ARRAY_SIZE(get_values), &mibrsp);
	if (!values) {
		kfree(mibrsp.data);
		goto exit_with_error;
	}
	for (i = 0; i < ARRAY_SIZE(get_values); i++) {
		if (values[i].type == SLSI_MIB_TYPE_UINT) {
			data[i] = values[i].u.uintValue;
		} else {
			SLSI_ERR(sdev, "invalid type. iter:%d\n", i);
			kfree(mibrsp.data);
			kfree(values);
			goto exit_with_error;
		}
	}
	ret = snprintf(command, buf_len, "%d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
	kfree(values);

exit_with_error:
	if (ret < 0)
		SLSI_ERR(sdev, "twt_error_code = %d\n", ret);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_twt_clear_statistics(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	struct slsi_vif_sta    *sta;
	struct slsi_mib_data   mib_data = { 0, NULL };
	int                    setup_id;
	int                    r, result = SLSI_TWT_WIFI_ERROR_INVALID_ARGUMENT;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r) {
		SLSI_ERR(sdev, "twt_error_code = %d\n", result);
		return result;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ioctl_args->arg_count < 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit_with_error;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &setup_id)) {
		SLSI_ERR(sdev, "Invalid setup string: '%s'\n", ioctl_args->args[0]);
		goto exit_with_error;
	}
	if (setup_id < 0 || setup_id >= 8) {
		SLSI_ERR(sdev, "Invalid setup id : %d\n", setup_id);
		goto exit_with_error;
	}
	sta = &ndev_vif->sta;
	if (!sta->twt_allowed) {
		SLSI_ERR(sdev, "HE Capability not supported\n");
		result = SLSI_TWT_WIFI_ERROR_NOT_SUPPORTED;
		goto exit_with_error;
	}
	result = slsi_mib_encode_uint(&mib_data, SLSI_PSID_UNIFI_TWT_CLEAR_STATS, setup_id, 0);
	if (result != SLSI_MIB_STATUS_SUCCESS || mib_data.dataLength == 0)
		goto exit_with_error;

	result = slsi_mlme_set(sdev, dev, mib_data.data, mib_data.dataLength);
	if (result != 0)
		SLSI_ERR(sdev, "twt_clear_statistics: mlme_set_req failed: Result code: %d\n", result);
	kfree(mib_data.data);

exit_with_error:
	if (result < 0)
		SLSI_ERR(sdev, "twt_error_code = %d\n", result);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return result;
}

static int slsi_adps_enable(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	int                    adps_state;
	int                    ret = -EINVAL;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not STA mode or STA is not in Connected state, iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}

	if (ioctl_args->arg_count < 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &adps_state)) {
		SLSI_ERR(sdev, "Invalid adps_state string: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (adps_state < 0 || adps_state > 1) {
		SLSI_ERR(sdev, "Invalid adps_state : %d\n", adps_state);
		goto exit;
	}

	ret = slsi_mlme_set_adps_state(sdev, dev, adps_state);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_get_sched_pm_error_code(struct net_device *dev, int result_code)
{
	switch (result_code) {
	case FAPI_RESULTCODE_INVALID_PARAMETERS:
		result_code = SLSI_SCHED_PM_INVALID_ARGUMENT;
		break;
	case FAPI_RESULTCODE_INVALID_VIRTUAL_INTERFACE_INDEX:
		result_code = SLSI_SCHED_PM_INVALID_ARGUMENT;
		break;
	case FAPI_RESULTCODE_NOT_SUPPORTED:
		result_code = SLSI_SCHED_PM_NOT_SUPPORTED_REQUEST;
		break;
	case FAPI_RESULTCODE_INVALID_STATE:
		result_code = SLSI_SCHED_PM_TEMPORARILY_BUSY;
		break;
	default:
		result_code = SLSI_SCHED_PM_TRANSIENT_ERROR;
	}
	return result_code;
}

static int slsi_sched_pm_setup(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif              *ndev_vif = netdev_priv(dev);
	struct slsi_dev                *sdev = ndev_vif->sdev;
	struct slsi_scheduled_pm_setup sched_pm_setup;
	int                            ret = SLSI_SCHED_PM_INVALID_ARGUMENT;
	struct slsi_ioctl_args         *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 7);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not STA mode or STA is not in Connected state, iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}
	if (ioctl_args->arg_count < 7) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &sched_pm_setup.desired_duration)) {
		SLSI_ERR(sdev, "Invalid Desired Duration String: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (sched_pm_setup.desired_duration < SLSI_SCHED_PM_MIN_DESIRED_DURATION ||
	    sched_pm_setup.desired_duration > SLSI_SCHED_PM_MAX_DESIRED_DURATION) {
		SLSI_ERR(sdev, "Invalid Desired Duration : %d\n", sched_pm_setup.desired_duration);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &sched_pm_setup.desired_interval)) {
		SLSI_ERR(sdev, "Invalid Desired Interval String: '%s'\n", ioctl_args->args[1]);
		goto exit;
	}
	if (sched_pm_setup.desired_interval < SLSI_SCHED_PM_MIN_DESIRED_INTERVAL ||
	    sched_pm_setup.desired_interval > SLSI_SCHED_PM_MAX_DESIRED_INTERVAL) {
		SLSI_ERR(sdev, "Invalid Desired Interval : %d\n", sched_pm_setup.desired_interval);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[2], &sched_pm_setup.additional_duration)) {
		SLSI_ERR(sdev, "Invalid Additional Duration String: '%s'\n", ioctl_args->args[2]);
		goto exit;
	}
	if (sched_pm_setup.additional_duration == -1)
                sched_pm_setup.additional_duration = 0;
	if (sched_pm_setup.additional_duration < SLSI_SCHED_PM_MIN_ADDITIONAL_DURATION ||
	    sched_pm_setup.additional_duration > SLSI_SCHED_PM_MAX_ADDITIONAL_DURATION) {
		SLSI_ERR(sdev, "Invalid Additional Duration : %d\n", sched_pm_setup.additional_duration);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[3], &sched_pm_setup.minimum_interval)) {
		SLSI_ERR(sdev, "Invalid Minimum Interval String: '%s'\n", ioctl_args->args[3]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[4], &sched_pm_setup.maximum_interval)) {
		SLSI_ERR(sdev, "Invalid Maximum Interval String: '%s'\n", ioctl_args->args[4]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[5], &sched_pm_setup.minimum_duration)) {
		SLSI_ERR(sdev, "Invalid Minimum Duration String: '%s'\n", ioctl_args->args[5]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[6], &sched_pm_setup.maximum_duration)) {
		SLSI_ERR(sdev, "Invalid Maximum Duration String: '%s'\n", ioctl_args->args[6]);
		goto exit;
	}
	if (sched_pm_setup.minimum_interval == -1)
		sched_pm_setup.minimum_interval = sched_pm_setup.desired_interval;
	if (sched_pm_setup.maximum_interval == -1)
		sched_pm_setup.maximum_interval = sched_pm_setup.desired_interval;
	if (sched_pm_setup.minimum_duration == -1)
		sched_pm_setup.minimum_duration = sched_pm_setup.desired_duration;
	if (sched_pm_setup.maximum_duration == -1)
		sched_pm_setup.maximum_duration = sched_pm_setup.desired_duration;
	ret = slsi_mlme_sched_pm_setup(sdev, dev, &sched_pm_setup);
	if (ret != FAPI_RESULTCODE_SUCCESS) {
		ret = slsi_get_sched_pm_error_code(dev, ret);
		SLSI_NET_ERR(dev, "mlme_scheduled_pm_setup(result:%d) ERROR\n", ret);
	}
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_sched_pm_teardown(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev   *sdev = ndev_vif->sdev;
	int               ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not STA mode or STA is not in Connected state, iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}
	ret = slsi_mlme_sched_pm_teardown(sdev, dev);
	if (ret != FAPI_RESULTCODE_SUCCESS) {
		ret = slsi_get_sched_pm_error_code(dev, ret);
		SLSI_NET_ERR(dev, "mlme_sched_pm_teardown(result:%d) ERROR\n", ret);
	}
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

static int slsi_sched_pm_status(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif               *ndev_vif = netdev_priv(dev);
	struct slsi_dev                 *sdev = ndev_vif->sdev;
	int                             ret;
	struct slsi_scheduled_pm_status status;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not STA mode or STA is not in Connected state, iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}
	memset(&status, 0, sizeof(struct slsi_scheduled_pm_status));
	ret = slsi_mlme_sched_pm_status(sdev, dev, &status);
	ret = snprintf(command, buf_len, "%d %d %d\n", status.state, status.duration, status.interval);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

static int slsi_set_delayed_wakeup(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	int delayed_wakeup_on;
	int timeout;
	int ret = -EINVAL;
	struct slsi_ioctl_args *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION) {
		SLSI_NET_ERR(dev, "Not STA mode , iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}

	if (ioctl_args->arg_count < 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &delayed_wakeup_on)) {
		SLSI_ERR(sdev, "Invalid delayed wake up state string: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (delayed_wakeup_on < 0 || delayed_wakeup_on > 1) {
		SLSI_ERR(sdev, "Invalid delayed_wakeup state : %d\n", delayed_wakeup_on);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &timeout)) {
		SLSI_ERR(sdev, "Invalid delayed wake up timeout string: '%s'\n", ioctl_args->args[1]);
		goto exit;
	}
	if (timeout < 1 || timeout > 60) {
		SLSI_ERR(sdev, "Invalid delayed wake up timeout'\n");
		goto exit;
	}
	ret = slsi_mlme_set_delayed_wakeup(sdev, dev, delayed_wakeup_on, timeout);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_set_delayed_wakeup_type(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif      *ndev_vif = netdev_priv(dev);
	struct slsi_dev        *sdev = ndev_vif->sdev;
	int                    type, i;
	int                    ret = -EINVAL;
	struct slsi_ioctl_args *ioctl_args = NULL;
	u8 macaddrlist[5][ETH_ALEN];
	u8  ipv4addrlist[5][4];
	u8 ipv6addrlist[5][16];
	int mac_count = 0;
	int ipv4_count = 0;
	int ipv6_count = 0;
	char *token;
	u8 *pos;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 4);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION) {
		SLSI_NET_ERR(dev, "Not STA mode , iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &type)) {
		SLSI_ERR(sdev, "Invalid type string: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (type < 0 || type > 3) {
		SLSI_ERR(sdev, "Invalid type : %d\n", type);
		goto exit;
	}
	if (ioctl_args->arg_count < 2 && type == 2) {
		SLSI_ERR(sdev, "Invalid argcount : %d\n", type);
		goto exit;
	}
	if (type == 2) {
		for (i = 1; i < ioctl_args->arg_count; i++) {
			if (strncmp((char *)&ioctl_args->args[i][2], ":", 1) == 0) {
				pos = &ioctl_args->args[i][0];
				while (pos < (ioctl_args->args[i] + strlen(ioctl_args->args[i]))) {
					slsi_machexstring_to_macarray(pos, macaddrlist[mac_count]);
					mac_count++;
					pos += (2 * ETH_ALEN) + 5 + 1;
				}
			} else if (strncmp((char *)&ioctl_args->args[i][4], ":", 1) == 0) {
				token = strsep((char **)&ioctl_args->args[i], ",");

				while (token) {
					if (in6_pton(token, strlen(token), ipv6addrlist[ipv6_count], -1, NULL) > 0) {
						ipv6_count++;
					} else {
						SLSI_ERR(sdev, "Invalid ipv6 addr\n");
						goto exit;
					}
					token = strsep((char **)&ioctl_args->args[i], ",");
				}
			} else {
				token = strsep((char **)&ioctl_args->args[i], ",");
				while (token) {
					if (in4_pton(token, strlen(token), ipv4addrlist[ipv4_count], -1, NULL) > 0) {
						ipv4_count++;
					} else {
						SLSI_ERR(sdev, "Invalid ipv4 addr\n");
						goto exit;
					}
					token = strsep((char **)&ioctl_args->args[i], ",");
				}
			}
		}
	}

	ret = slsi_mlme_set_delayed_wakeup_type(sdev, dev, type, macaddrlist, mac_count, ipv4addrlist, ipv4_count,
						ipv6addrlist, ipv6_count);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

#ifdef CONFIG_SCSC_WLAN_SAP_POWER_SAVE
static int slsi_set_ap_suspend(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev	*sdev = ndev_vif->sdev;
	struct slsi_ioctl_args *ioctl_args = NULL;

	struct netdev_vif *suspend_vif = NULL;
	struct net_device *suspend_dev = NULL;
	int suspend_enable = 0;
	int result = 0;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);

	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result) {
		SLSI_ERR(sdev, "Malloc Failed or Wrong ioctl_args\n");
		return result;
	}

	if (ioctl_args->arg_count != 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		result = -EINVAL;
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &suspend_enable)) {
		SLSI_ERR(sdev, "Invalid argument: '%s'\n", ioctl_args->args[0]);
		result = -EINVAL;
		goto exit;
	}

	/* Get net_device for requested v-interface */
	suspend_dev = slsi_get_netdev_by_ifname(sdev, ioctl_args->args[1]);
	if (!suspend_dev) {
		SLSI_ERR(sdev, "There is no net_device for %s\n", ioctl_args->args[1]);
		result = -EINVAL;
		goto exit;
	}

	/* Get vif structure */
	suspend_vif = netdev_priv(suspend_dev);
	SLSI_MUTEX_LOCK(suspend_vif->vif_mutex);

	/* Check requested VIF's type and status */
	if (suspend_vif->vif_type == FAPI_VIFTYPE_AP && suspend_vif->activated) {
		/* Check requested VIF's curr suspend_mode */
		if (suspend_vif->softap_suspend_mode == suspend_enable) {
			SLSI_ERR(sdev, "Already working in this mode\n");
			goto exit_with_unlock;
		}
		/* En/disable Transmit Queue */
		if (suspend_enable) {
			suspend_vif->softap_suspend_mode = 1;
			netif_tx_disable(suspend_dev);
		} else {
			suspend_vif->softap_suspend_mode = 0;
			netif_tx_wake_all_queues(suspend_dev);
		}
	} else {
		SLSI_ERR(sdev, "Wrong vif-type or device status\n");
		result = -EINVAL;
		goto exit_with_unlock;
	}

	SLSI_INFO(sdev, "%s %s %d\n", CMD_SET_AP_SUSPEND, ioctl_args->args[1], suspend_enable);
	result = slsi_mlme_ap_suspend(suspend_vif->sdev, suspend_dev, suspend_enable);

exit_with_unlock:
	SLSI_MUTEX_UNLOCK(suspend_vif->vif_mutex);

exit:
	kfree(ioctl_args);
	return result;
}

static int slsi_set_ap_rps_params(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev         *sdev = ndev_vif->sdev;
	struct net_device       *sap_dev;
	struct netdev_vif       *sap_ndev_vif;
	struct slsi_ioctl_args  *ioctl_args = NULL;
	struct ap_rps_params    rps_params;
	int                     result = -EINVAL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 5);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	result = -EINVAL;
	if (ioctl_args->arg_count != 5) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &rps_params.ips)) {
		SLSI_ERR(sdev, "Invalid incoming packet per seconds: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[1], &rps_params.phase)) {
		SLSI_ERR(sdev, "Invalid power saving phase : '%s'\n", ioctl_args->args[1]);
		goto exit;
	}
	if (rps_params.phase < AP_RPS_PHASE_MIN || rps_params.phase > AP_RPS_PHASE_MAX) {
		SLSI_ERR(sdev, "Out of  power saving phase boundary (1~10): %d\n", rps_params.phase);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[2], &rps_params.npktime)) {
		SLSI_ERR(sdev, "Invalid packet duration : '%s'\n", ioctl_args->args[2]);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[3], &rps_params.checkstastatus)) {
		SLSI_ERR(sdev, "Invalid STA connection status : '%s'\n", ioctl_args->args[3]);
		goto exit;
	}
	if (rps_params.checkstastatus != 0 && rps_params.checkstastatus != 1) {
		SLSI_ERR(sdev, "Invalid STA connection status (0|1): '%d'\n", rps_params.checkstastatus);
		goto exit;
	}

	sap_dev = slsi_get_netdev_by_ifname(sdev, ioctl_args->args[4]);
	if (!sap_dev) {
		SLSI_NET_ERR(dev, "no net_device for %s\n", ioctl_args->args[4]);
		goto exit;
	}
	sap_ndev_vif = netdev_priv(sap_dev);

	SLSI_MUTEX_LOCK(sap_ndev_vif->vif_mutex);

	if (!sap_ndev_vif->activated || sap_ndev_vif->vif_type != FAPI_VIFTYPE_AP ||
	    sap_ndev_vif->iftype != NL80211_IFTYPE_AP) {
		SLSI_ERR(sdev,
			 "slsi_set_ap_rps: AP is not up.Command not allowed vif.activated:%d, vif.type:%d, nl80211.vif.type:%d\n",
			 sap_ndev_vif->activated, sap_ndev_vif->vif_type, sap_ndev_vif->iftype);
		SLSI_MUTEX_UNLOCK(sap_ndev_vif->vif_mutex);
		goto exit;
	}

	result = slsi_mlme_set_ap_rps_params(sdev, dev, &rps_params, sap_ndev_vif->ifnum);

	SLSI_MUTEX_UNLOCK(sap_ndev_vif->vif_mutex);

exit:
	kfree(ioctl_args);
	return result;
}

static int slsi_set_ap_rps(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev	        *sdev = ndev_vif->sdev;
	struct net_device       *sap_dev;
	struct netdev_vif       *sap_ndev_vif;
	struct slsi_ioctl_args  *ioctl_args = NULL;
	int                     rps_enable;
	int                     result = -EINVAL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	result = -EINVAL;
	if (ioctl_args->arg_count != 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &rps_enable)) {
		SLSI_ERR(sdev, "Invalid AP RPS enabled: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (rps_enable != 0 && rps_enable != 1) {
		SLSI_ERR(sdev, "Invalid AP RPS enabled (0|1): '%d'\n", rps_enable);
		goto exit;
	}

	sap_dev = slsi_get_netdev_by_ifname(sdev, ioctl_args->args[1]);
	if (!sap_dev) {
		SLSI_NET_ERR(dev, "no net_device for %s\n", ioctl_args->args[1]);
		goto exit;
	}
	sap_ndev_vif = netdev_priv(sap_dev);

	SLSI_MUTEX_LOCK(sap_ndev_vif->vif_mutex);

	if (!sap_ndev_vif->activated || sap_ndev_vif->vif_type != FAPI_VIFTYPE_AP ||
	    sap_ndev_vif->iftype != NL80211_IFTYPE_AP) {
		SLSI_ERR(sdev, "AP is not up.Command not allowed vif.activated:%d, vif.type:%d, nl80211.vif.type:%d\n",
			 sap_ndev_vif->activated, sap_ndev_vif->vif_type, sap_ndev_vif->iftype);
		SLSI_MUTEX_UNLOCK(sap_ndev_vif->vif_mutex);
		goto exit;
	}

	result = slsi_mlme_set_ap_rps(sdev, dev, rps_enable, sap_ndev_vif->ifnum);

	SLSI_MUTEX_UNLOCK(sap_ndev_vif->vif_mutex);

exit:
	kfree(ioctl_args);
	return result;
}

static int slsi_set_ap_tx_power(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev	        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args  *ioctl_args = NULL;
	int                     tx_power = 0;
	int                     result = -EINVAL;
#ifdef CONFIG_SCSC_WLAN_WIFI_SHARING
	struct net_device       *ap_dev;
	struct netdev_vif       *ndev_ap_vif;

	ap_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX_SWLAN);

	if (ap_dev) {
		ndev_ap_vif = netdev_priv(ap_dev);
		SLSI_MUTEX_LOCK(ndev_ap_vif->vif_mutex);
		if (SLSI_IS_VIF_INDEX_MHS(sdev, ndev_ap_vif))
			ndev_vif = ndev_ap_vif;
		SLSI_MUTEX_UNLOCK(ndev_ap_vif->vif_mutex);
	}
#endif

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	result = -EINVAL;
	if (ioctl_args->arg_count != 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}

	if (!slsi_str_to_int(ioctl_args->args[0], &tx_power)) {
		SLSI_ERR(sdev, "Invalid AP TX power: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (tx_power < AP_RPS_PHASE_MIN || tx_power > AP_RPS_PHASE_MAX) {
		SLSI_ERR(sdev, "Out of AP TX power boundary  (0~31dBm): %d\n", tx_power);
		goto exit;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_AP ||
	    ndev_vif->iftype != NL80211_IFTYPE_AP) {
		SLSI_ERR(sdev, "AP is not up.Command not allowed vif.activated:%d, vif.type:%d, nl80211.vif.type:%d\n",
			 ndev_vif->activated, ndev_vif->vif_type, ndev_vif->iftype);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		goto exit;
	}

	result = slsi_mlme_set_ap_tx_power(sdev, dev, tx_power, ndev_vif->ifnum);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

exit:
	kfree(ioctl_args);
	return result;
}

static int slsi_get_ap_tx_power(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif       *ndev_vif = netdev_priv(dev);
	struct slsi_dev	        *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args  *ioctl_args = NULL;
	int                     result = -EINVAL;
#ifdef CONFIG_SCSC_WLAN_WIFI_SHARING
	struct net_device       *ap_dev;
	struct netdev_vif       *ndev_ap_vif;

	ap_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX_SWLAN);

	if (ap_dev) {
		ndev_ap_vif = netdev_priv(ap_dev);
		SLSI_MUTEX_LOCK(ndev_ap_vif->vif_mutex);
		if (SLSI_IS_VIF_INDEX_MHS(sdev, ndev_ap_vif))
			ndev_vif = ndev_ap_vif;
		SLSI_MUTEX_UNLOCK(ndev_ap_vif->vif_mutex);
	}
#endif

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_AP || ndev_vif->iftype != NL80211_IFTYPE_AP) {
		SLSI_ERR(sdev, "AP is not up.Command not allowed vif.activated:%d, vif.type:%d, nl80211.vif.type:%d\n",
			 ndev_vif->activated, ndev_vif->vif_type, ndev_vif->iftype);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}

	result = slsi_mlme_get_ap_tx_power(sdev, dev, command, buf_len, ndev_vif->ifnum);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	kfree(ioctl_args);
	return result;
}
#endif

static int slsi_sched_pm_leaky_ap_active_detection(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif             *ndev_vif = netdev_priv(dev);
	struct slsi_dev               *sdev = ndev_vif->sdev;
	struct slsi_sched_pm_leaky_ap leaky_ap;
	int                           ret = -EINVAL;
	struct slsi_ioctl_args        *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 3);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not STA mode or STA is not in Connected state, iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}
	if (ioctl_args->arg_count != 3) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	memset(&leaky_ap, 0, sizeof(struct slsi_sched_pm_leaky_ap));
	ret = in4_pton(ioctl_args->args[0], -1, leaky_ap.src_ipaddr, -1, NULL);
	if(ret < 0) {
		SLSI_ERR(sdev, "Invalid ipv4 Source IP address\n");
		goto exit;
	}
	ret = in4_pton(ioctl_args->args[1], -1,leaky_ap.target_ipaddr, -1, NULL);
	if(ret < 0) {
		SLSI_ERR(sdev, "Invalid ipv4 Target IP address\n");
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[2], &leaky_ap.duration)) {
		SLSI_ERR(sdev, "Invalid duration String: '%s'\n", ioctl_args->args[2]);
		goto exit;
	}
	if (leaky_ap.duration < SLSI_SCHED_PM_LEAKY_AP_ACTIVE_DETECTION_MIN_DURATION || leaky_ap.duration > SLSI_SCHED_PM_LEAKY_AP_ACTIVE_DETECTION_MAX_DURATION) {
		SLSI_ERR(sdev, "Invalid Duration: %d\n", leaky_ap.duration);
		goto exit;
	}
	leaky_ap.detection_type = FAPI_SCHEDULEDPMLEAKYAPDETECTIONTYPE_ACTIVE_ONESHOT;
	ret = slsi_mlme_scheduled_pm_leaky_ap_detect(sdev, dev, &leaky_ap);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_sched_pm_leaky_ap_passive_detection_start(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif             *ndev_vif = netdev_priv(dev);
	struct slsi_dev               *sdev = ndev_vif->sdev;
	struct slsi_sched_pm_leaky_ap leaky_ap;
	int                           ret = -EINVAL;
	struct slsi_ioctl_args        *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not STA mode or STA is not in Connected state, iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}
	if (ioctl_args->arg_count != 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	memset(&leaky_ap, 0, sizeof(struct slsi_sched_pm_leaky_ap));
	if (!slsi_str_to_int(ioctl_args->args[0], &leaky_ap.duration)) {
		SLSI_ERR(sdev, "Invalid duration String: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (leaky_ap.duration < SLSI_SCHED_PM_LEAKY_AP_ACTIVE_DETECTION_MIN_DURATION || leaky_ap.duration > SLSI_SCHED_PM_LEAKY_AP_ACTIVE_DETECTION_MAX_DURATION) {
		SLSI_ERR(sdev, "Invalid Duration: %d\n", leaky_ap.duration);
		goto exit;
	}
	leaky_ap.grace_period = ndev_vif->grace_period;
	leaky_ap.detection_type = FAPI_SCHEDULEDPMLEAKYAPDETECTIONTYPE_PASSIVE_START;
	ret = slsi_mlme_scheduled_pm_leaky_ap_detect(sdev, dev, &leaky_ap);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_sched_pm_leaky_ap_passive_detection_end(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif             *ndev_vif = netdev_priv(dev);
	struct slsi_dev               *sdev = ndev_vif->sdev;
	struct slsi_sched_pm_leaky_ap leaky_ap;
	int                           ret = -EINVAL;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->iftype != NL80211_IFTYPE_STATION ||
	    (ndev_vif->iftype == NL80211_IFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		SLSI_NET_ERR(dev, "Not STA mode or STA is not in Connected state, iftype = %d\n", ndev_vif->iftype);
		ret = -EPERM;
		goto exit;
	}
	memset(&leaky_ap, 0, sizeof(struct slsi_sched_pm_leaky_ap));
	leaky_ap.detection_type = FAPI_SCHEDULEDPMLEAKYAPDETECTIONTYPE_PASSIVE_END;
	ndev_vif->grace_period = 0;
	ret = slsi_mlme_scheduled_pm_leaky_ap_detect(sdev, dev, &leaky_ap);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

static int slsi_sched_pm_set_grace_period(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif             *ndev_vif = netdev_priv(dev);
	struct slsi_dev               *sdev = ndev_vif->sdev;
	int                           grace_period;
	int                           ret = -EINVAL;
	struct slsi_ioctl_args        *ioctl_args = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ioctl_args->arg_count != 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &grace_period)) {
		SLSI_ERR(sdev, "Invalid Grace Period String: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (grace_period < SLSI_SCHED_PM_MIN_GRACE_PERIOD || grace_period > SLSI_SCHED_PM_MAX_GRACE_PERIOD) {
		SLSI_ERR(sdev, "Invalid Grace Period: %d\n", grace_period);
		goto exit;
	}
	ndev_vif->grace_period = grace_period;
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	kfree(ioctl_args);
	return ret;
}

static int slsi_get_tdls_max_session(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int               len = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	len = snprintf(command, buf_len, "%s %d", CMD_GET_TDLS_MAX_SESSION,
		       ndev_vif->sta.tdls_max_peer);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return len;
}

static int slsi_get_tdls_num_of_session(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int               len = 0;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	len = snprintf(command, buf_len, "%s %d", CMD_GET_TDLS_NUM_OF_SESSION,
		       ndev_vif->sta.tdls_peer_sta_records);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return len;
}

static int slsi_get_tdls_wider_bandwidth(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	int                       res = 0;

	res = snprintf(command, buf_len, "%s %d\n", CMD_GET_TDLS_WIDER_BW,
		       sdev->tdls_wider_bandwidth);
	return res;
}

/*TODO:autogen to be done*/
#define SLSI_PSID_UNIFI_TDLS_AVAILABLE 2571
static int slsi_get_tdls_available_state(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	struct slsi_mib_data      mibrsp = { 0, NULL };
	unsigned int              is_tdls_available;
	struct slsi_mib_value     *values = NULL;
	struct slsi_mib_get_entry get_values[] = {{SLSI_PSID_UNIFI_TDLS_AVAILABLE, { 0, 0 } } };
	int                       res = -EINVAL;

	mibrsp.dataLength = 8;
	mibrsp.data = kmalloc(mibrsp.dataLength, GFP_KERNEL);
	if (!mibrsp.data) {
		SLSI_ERR(sdev, "Cannot kmalloc %d bytes\n", mibrsp.dataLength);
		res = -ENOMEM;
		goto exit_with_error;
	}
	values = slsi_read_mibs(sdev, dev, get_values, 1, &mibrsp);
	if (!values) {
		goto exit_with_mibs;
	}
	if (values[0].type != SLSI_MIB_TYPE_UINT) {
		SLSI_ERR(sdev, "Invalid type (%d) for SLSI_PSID_UNIFI_TDLS_AVAILABLE\n", values[0].type);
		goto exit_with_values;
	}

	is_tdls_available = values[0].u.uintValue;
	SLSI_DBG3(sdev, SLSI_MLME, "TDLS available = %d\n", is_tdls_available);

	res = snprintf(command, buf_len, "%s %d\n", CMD_GET_TDLS_AVAILABLE, is_tdls_available);

exit_with_values:
	kfree(values);
exit_with_mibs:
	kfree(mibrsp.data);
exit_with_error:
	return res;
}

static int slsi_set_tdls_enabled(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args    *ioctl_args = NULL;
	int                       tdls_enabled_flag;
	int                       ret = -EINVAL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;

	ret = -EINVAL;
	if (!slsi_str_to_int(ioctl_args->args[0], &tdls_enabled_flag)) {
		SLSI_ERR(sdev, "Invalid TDLS enabled flag string: '%s'\n", ioctl_args->args[0]);
		goto exit;
	}
	if (tdls_enabled_flag != 0 && tdls_enabled_flag != 1) {
		SLSI_ERR(sdev, "Invalid value of TDLS enabled flag %d\n", tdls_enabled_flag);
		goto exit;
	}
	SLSI_DBG3(sdev, SLSI_MLME, "TDLS enabled flag = %d\n", tdls_enabled_flag);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ret = slsi_mlme_set_tdls_state(sdev, dev, tdls_enabled_flag);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

exit:
	kfree(ioctl_args);
	return ret;
}

int slsi_twt_softap_enable(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	int                       enable;
	struct slsi_ioctl_args    *ioctl_args = NULL;
	int                       r = -EINVAL;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		goto exit_with_mutex;

	if (ioctl_args->arg_count != 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		goto exit_with_mutex;
	}
	if (!slsi_str_to_int(ioctl_args->args[0], &enable)) {
		SLSI_ERR(sdev, "Invalid enable String: '%s'\n", ioctl_args->args[0]);
		goto exit_with_mutex;
	}
	if (sdev->device_state == SLSI_DEVICE_STATE_STARTED)
		r = slsi_twt_update_ctrl_flags(dev, enable);
	else
		sdev->twt_enable_responder = enable;
exit_with_mutex:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

int slsi_hapd_twt_teardown(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	int                       setup_id;
	struct slsi_ioctl_args    *ioctl_args = NULL;
	int                       r = -EINVAL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	r = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (r)
		return r;
	if (!slsi_str_to_int(ioctl_args->args[0], &setup_id)) {
		SLSI_ERR(sdev, "Invalid setup_id String: '%s'\n", ioctl_args->args[0]);
		return r;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_AP || ndev_vif->iftype != NL80211_IFTYPE_AP) {
		SLSI_ERR(sdev, "AP is not up.Command not allowed, vif.type:%d.\n", ndev_vif->iftype);
		r = -EPERM;
	} else {
		r = slsi_mlme_twt_teardown(sdev, dev, setup_id, 0);
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

int slsi_hapd_get_twt_status(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	int                       command_pos = 0;
	int                       r = -EINVAL;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated || ndev_vif->vif_type != FAPI_VIFTYPE_AP || ndev_vif->iftype != NL80211_IFTYPE_AP) {
		SLSI_ERR(sdev, "AP is not up. Command not allowed, vif.type:%d.\n", ndev_vif->iftype);
		r = -EPERM;
	} else {
		r = slsi_mlme_twt_status_query(sdev, dev, command, buf_len, &command_pos, 0);
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

int slsi_set_elnabypass_threshold(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args    *ioctl_args = NULL;
	int                       ret, r = -EINVAL;
	int                       elna_value = 2;
	struct netdev_vif         *elna_vif = NULL;
	struct net_device         *elna_net_device = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 2);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return ret;
	if (ioctl_args->arg_count < 2) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		return r;
	}
	if (strcmp(ioctl_args->args[0], "wlan0") != 0) {
		SLSI_ERR(sdev, "Invalid interface name %s\n", ioctl_args->args[0]);
		return r;
	}
	elna_net_device = slsi_get_netdev_by_ifname(sdev, ioctl_args->args[0]);
	if (!elna_net_device) {
		SLSI_ERR(sdev, "There is no net_device for %s\n", ioctl_args->args[0]);
		return r;
	}
	if (strtoint(ioctl_args->args[1], &elna_value) < 0) {
		SLSI_ERR(sdev, "Conversion error for elna value.\n");
		return -r;
	}
	if ((elna_value < 0 || elna_value > 2)) {
		SLSI_ERR(sdev, "Invalid elna value string: '%s'\n", ioctl_args->args[1]);
		return r;
	}
	elna_vif = netdev_priv(elna_net_device);
	SLSI_MUTEX_LOCK(elna_vif->vif_mutex);
	if (!elna_vif->activated) {
		elna_vif->sta.store_elna_value = elna_value;
		elna_vif->sta.set_elna_after_connect = true;
		r = 0;
	} else if (elna_vif->activated && elna_vif->iftype == NL80211_IFTYPE_STATION) {
		r = slsi_mlme_set_elnabypass_threshold(sdev, elna_net_device, elna_value);
	}
	SLSI_MUTEX_UNLOCK(elna_vif->vif_mutex);
	return r;
}

int slsi_get_elna_status(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif         *ndev_vif = netdev_priv(dev);
	struct slsi_dev           *sdev = ndev_vif->sdev;
	struct slsi_ioctl_args    *ioctl_args = NULL;
	int                       ret, r = -EINVAL;
	struct netdev_vif         *elna_vif = NULL;
	struct net_device         *elna_net_device = NULL;

	ioctl_args = slsi_get_private_command_args(command, buf_len, 1);
	ret = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (ret)
		return r;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ioctl_args->arg_count < 1) {
		SLSI_ERR(sdev, "Invalid argument count = %d\n", ioctl_args->arg_count);
		return r;
	}
	if (strcmp(ioctl_args->args[0], "wlan0") != 0) {
		SLSI_ERR(sdev, "Invalid interface name %s\n", ioctl_args->args[0]);
		return r;
	}

	elna_net_device = slsi_get_netdev_by_ifname(sdev, ioctl_args->args[0]);
	if (!elna_net_device) {
		SLSI_ERR(sdev, "There is no net_device for %s\n", ioctl_args->args[0]);
		return r;
	}
	elna_vif = netdev_priv(elna_net_device);
	r = snprintf(command, buf_len, "%d\n", elna_vif->sta.current_elna_value);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
static void slsi_get_dual_station_cert(struct slsi_dev *sdev, u8 *dual_cert)
{
	struct slsi_mib_value     *values = NULL;
	struct slsi_mib_get_entry get_values[] = {{SLSI_PSID_UNIFI_DUAL_STATION_CERTIFICATE, { 0, 0 } } };
	struct slsi_mib_data      mibrsp = { 0, NULL };

	*dual_cert = 0;
	mibrsp.dataLength = 8;
	mibrsp.data = kmalloc(mibrsp.dataLength, GFP_KERNEL);
	if (!mibrsp.data) {
		SLSI_ERR(sdev, "Cannot kmalloc %d bytes\n", mibrsp.dataLength);
		return;
	}
	values = slsi_read_mibs(sdev, NULL, get_values, 1, &mibrsp);
	if (values && values[0].type != SLSI_MIB_TYPE_NONE)
		*dual_cert = values[0].u.uintValue;

	kfree(values);
	kfree(mibrsp.data);
}

static int slsi_get_wifi6e_channels(struct net_device *dev, char *buf, int buf_len)
{
	struct slsi_dev *sdev = ((struct netdev_vif *)netdev_priv(dev))->sdev;
	struct slsi_ioctl_args *ioctl_args;
	struct slsi_sub_band sub_bands[SLSI_MAX_SUB_BANDS];
	struct regdb_file_reg_rule *reg_rule;
	int pwr_mode, status, sub_band_count;
	int cur_pos = 0;
	int  channel_start_freq, channel_end_freq, channel_start_num, channel_end_num;
	int  channel_count, channel_increment, channel_band;
	int i, j, k;
	int country_index = 0;
	uint32_t reg_rule_flag = 0;
	u8 dual_cert = 0;

	ioctl_args = slsi_get_private_command_args(buf, buf_len, 1);
	status = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (status)
		return status;

	if (!slsi_str_to_int(ioctl_args->args[0], &pwr_mode)) {
		SLSI_ERR(sdev, "Invalid string: '%s'\n", ioctl_args->args[0]);
		kfree(ioctl_args);
		return -EINVAL;
	}
	kfree(ioctl_args);
	ioctl_args = NULL;

	switch(pwr_mode) {
		case 0:
			reg_rule_flag = SLSI_REGULATORY_6GHZ_LPI;
			break;
		case 1:
			reg_rule_flag = SLSI_REGULATORY_6GHZ_VLP;
			break;
		case 2:
			slsi_get_dual_station_cert(sdev, &dual_cert);
			if (!dual_cert) {
				SLSI_INFO(sdev, "Not Dual Station Certified\n");
				return 0;
			}
			reg_rule_flag = SLSI_REGULATORY_6GHZ_SP;
			break;
		default:
			SLSI_ERR(sdev, "Invalid power mode: %d\n", pwr_mode);
			return -EINVAL;
	}

	if (!sdev->band_6g_supported) {
		SLSI_ERR(sdev, "6GHz band not supported\n");
		return 0;
	}
	country_index = sdev->regdb.current_cc_index;
	SLSI_INFO(sdev, "get_wifi6e_channels for power scheme: %d", pwr_mode);

	sub_band_count = slsi_get_supported_channels(sdev, dev, &sub_bands[0], ARRAY_SIZE(sub_bands));
	for (i = 0; i < sub_band_count; i++) {
		if (sub_bands[i].band != NL80211_BAND_6GHZ)
			continue;
		channel_start_num = sub_bands[i].start_chan_num;
		channel_count = sub_bands[i].channel_count;
		channel_increment = sub_bands[i].increment;
		channel_band = sub_bands[i].band;
		channel_end_num = channel_start_num + ((channel_count - 1) * channel_increment);
		for (j = channel_start_num; j <= channel_end_num; j += channel_increment) {
			channel_start_freq = ieee80211_channel_to_frequency(j, channel_band) - 10;
			channel_end_freq = ieee80211_channel_to_frequency(j, channel_band) + 10;
			for (k = 0; k < sdev->regdb.country[country_index].collection->reg_rule_num; k++) {
				reg_rule = sdev->regdb.country[country_index].collection->reg_rule[k];
				if ((reg_rule->flags & reg_rule_flag) &&
				    reg_rule->freq_range->start_freq <= channel_start_freq &&
				    reg_rule->freq_range->end_freq >= channel_end_freq) {
					if (cur_pos)
						cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", %d", j);
					else
						cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, " %d", j);
					break;
				}
			}
			if (j == 1) {
				channel_start_freq = ieee80211_channel_to_frequency(2, channel_band) - 10;
				channel_end_freq = ieee80211_channel_to_frequency(2, channel_band) + 10;
				for (k = 0; k < sdev->regdb.country[country_index].collection->reg_rule_num; k++) {
					reg_rule = sdev->regdb.country[country_index].collection->reg_rule[k];
					if ((reg_rule->flags & reg_rule_flag) &&
					    reg_rule->freq_range->start_freq <= channel_start_freq &&
					    reg_rule->freq_range->end_freq >= channel_end_freq) {
						cur_pos += snprintf(buf + cur_pos, buf_len - cur_pos, ", %d", 2);
						break;
					}
				}
			}
		}
	}
	SLSI_DBG3(sdev, SLSI_MLME, "%s\n", buf);
	return cur_pos;
}

static int slsi_set_wifi_safe_mode(struct net_device *dev, char *command, int cmd_len)
{
	struct slsi_dev *sdev = ((struct netdev_vif *)netdev_priv(dev))->sdev;
	struct slsi_mib_data mib_data = { 0, NULL };
	struct slsi_ioctl_args *ioctl_args = NULL;
	int safe_mode, result, ret;

	ioctl_args = slsi_get_private_command_args(command, cmd_len, 1);
	result = slsi_verify_ioctl_args(sdev, ioctl_args);
	if (result)
		return result;

	if (!slsi_str_to_int(ioctl_args->args[0], &safe_mode)) {
		SLSI_ERR(sdev, "Invalid safe mode sting: '%s'\n", ioctl_args->args[0]);
		ret = -EINVAL;
		goto err;
	}

	if (!((safe_mode == 0) || (safe_mode == 1))) {
		ret = -EINVAL;
		goto err;
	}

	result = slsi_mib_encode_bool(&mib_data, SLSI_PSID_UNIFI6_G_SAFE_MODE, safe_mode, 0);
	if (result != SLSI_MIB_STATUS_SUCCESS) {
		ret = -ENOMEM;
		goto err;
	}

	ret = slsi_mlme_set(sdev, NULL, mib_data.data, mib_data.dataLength);
	if (ret != 0)
		SLSI_ERR(sdev, "safe mode: mlme_set_req failed: Result code: %d\n", ret);
	else
		sdev->wifi_safe_mode = safe_mode;
err:
	kfree(ioctl_args);
	return ret;
}
#endif

static void slsi_get_sta_dump_str(int *dump_param, char *sta_dump) {
	char sta_str[15][15] = {"rssi:", "rxpackets:", "txpackets:",
				"rxbytes:", "txbytes:", "rxbitrate:",
				"txbitrate:", "rxmcs:", "txmcs:",
				"rxbandwidth:", "txbandwidth:", "rxnss:",
				"txnss:", "mode:"};
	char mode[7][7] = {"11B ", "11G ", "11N ", "11A ",
			   "11AC ", "11AX ", "11BE "};
	char unit[15][6] = {"dBm "," "," "," "," ",
			    "Mbps ","Mbps "," "," ",
			    "MHz ","MHz "," "," "," "};
	char int_string[30] = "";
	char bw[4][15] = {"20", "40", "80", "160"};
	u8 i;

	for(i = 0; i < SLSI_STA_DUMP_MAX ; i++) {
		if (i != SLSI_STA_DUMP_MODE && i != SLSI_STA_DUMP_RXBANDWIDTH &&
			i != SLSI_STA_DUMP_TXBANDWIDTH) {
			sta_dump = strcat(sta_dump, sta_str[i]);
			sprintf(int_string, "%d", dump_param[i]);
			sta_dump = strcat(sta_dump, int_string);
		} else if (i == SLSI_STA_DUMP_MODE) {
			sta_dump = strcat(sta_dump, sta_str[i]);
			sta_dump = strcat(sta_dump, mode[dump_param[i]]);
		} else {
			sta_dump = strcat(sta_dump, sta_str[i]);
			sta_dump = strcat(sta_dump, bw[dump_param[i]]);
		}
		sta_dump = strcat(sta_dump, unit[i]);
	}
}

static int slsi_get_sta_dump(struct net_device *dev, char *command, int buf_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int res = 0;
	int dump_params[SLSI_STA_DUMP_MAX] = {0};
	char sta_dump[256] = {" "};

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (ndev_vif->vif_type != FAPI_VIFTYPE_STATION &&
	    ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED) {
		SLSI_NET_ERR(dev, "type %d in %d state\n", ndev_vif->vif_type,
			     ndev_vif->sta.vif_status);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		goto print_res;
	}
	slsi_mlme_get_sta_dump(ndev_vif, dev, dump_params);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_get_sta_dump_str(dump_params, sta_dump);
print_res:
	res = snprintf(command, buf_len, "%s", sta_dump);

	return res;
}

static const struct slsi_ioctl_fn slsi_ioctl_fn_table[] = {
	{ CMD_SETSUSPENDMODE,               slsi_set_suspend_mode },
	{ CMD_SETJOINPREFER,                slsi_update_rssi_boost },
	{ CMD_RXFILTERADD,                  slsi_rx_filter_add },
	{ CMD_RXFILTERREMOVE,               slsi_rx_filter_remove },
	{ CMD_SETBANDWIDTH,                 slsi_set_bandwidth }, /* Changing the BW in regulatory database*/
	{ CMD_SETSCANHOMEAWAYTIME_LEGACY,   slsi_set_home_away_time_legacy },
	{ CMD_SETSCANHOMETIME_LEGACY,       slsi_set_home_time_legacy },
	{ CMD_SETSCANCHANNELTIME_LEGACY,    slsi_set_channel_time_legacy },
	{ CMD_SETSCANPASSIVETIME_LEGACY,    slsi_set_passive_time_legacy },
	{ CMD_SET_TID,                      slsi_set_tid },
	{ CMD_SET_BSS_CHANNEL_WIDTH,        slsi_set_bss_bandwidth }, /* Changing the Tx BW only, without notifying the AP */
#ifdef CONFIG_SCSC_WLAN_WIFI_SHARING
#if !defined(CONFIG_SCSC_WLAN_MHS_STATIC_INTERFACE) || (defined(SCSC_SEP_VERSION) && SCSC_SEP_VERSION < 9)
	{ CMD_INTERFACE_CREATE,             slsi_create_interface },
	{ CMD_INTERFACE_DELETE,             slsi_delete_interface },
#endif
	{ CMD_SET_INDOOR_CHANNELS,          slsi_set_wifisharing_permitted_channels },
	{ CMD_GET_INDOOR_CHANNELS,          slsi_get_indoor_channels },
#endif
	{ CMD_SETCOUNTRYREV,                slsi_set_country_rev },
	{ CMD_GETCOUNTRYREV,                slsi_get_country_rev },
	{ CMD_SETROAMBAND,                  slsi_ioctl_set_roam_band },
	{ CMD_SETBAND,                      slsi_ioctl_set_band },
	{ CMD_GETROAMBAND,                  slsi_ioctl_get_roam_band },
	{ CMD_GETBAND,                      slsi_ioctl_get_band },
	{ CMD_FACTORY_SETBAND,              slsi_factory_freq_band_write },
	{ CMD_SETROAMTRIGGER_LEGACY,        slsi_legacy_roam_trigger_write },
	{ CMD_GETROAMTRIGGER_LEGACY,        slsi_legacy_roam_scan_trigger_read },
	{ CMD_REASSOC_LEGACY,               slsi_reassoc_write_legacy },
	{ CMD_REASSOC_FREQUENCY_LEGACY,     slsi_reassoc_frequency_write_legacy },
	{ CMD_ADDROAMSCANCHANNELS_LEGACY,   slsi_roam_add_scan_channels_legacy },
	{ CMD_ADDROAMSCANFREQUENCIES_LEGACY, slsi_roam_add_scan_frequencies_legacy },
	{ CMD_GETROAMSCANFREQUENCIES_LEGACY, slsi_roam_scan_frequencies_read_legacy },
	{ CMD_GETROAMSCANCHANNELS_LEGACY,   slsi_roam_scan_channels_read_legacy },
	{ CMD_SETWTCMODE,                   slsi_set_wtc_mode },
#ifdef CONFIG_SCSC_WLAN_WES_NCHO
	{ CMD_SETROAMTRIGGER,               slsi_roam_scan_trigger_write },
	{ CMD_GETROAMTRIGGER,               slsi_roam_scan_trigger_read },
	{ CMD_SETROAMDELTA,                 slsi_roam_delta_trigger_write },
	{ CMD_GETROAMDELTA,                 slsi_roam_delta_trigger_read },
	{ CMD_SETROAMSCANPERIOD,            slsi_cached_channel_scan_period_write },
	{ CMD_GETROAMSCANPERIOD,            slsi_cached_channel_scan_period_read },
	{ CMD_SETFULLROAMSCANPERIOD,        slsi_full_roam_scan_period_write },
	{ CMD_GETFULLROAMSCANPERIOD,        slsi_full_roam_scan_period_read },
	{ CMD_SETSCANCHANNELTIME,           slsi_roam_scan_max_active_channel_time_write },
	{ CMD_GETSCANCHANNELTIME,           slsi_roam_scan_max_active_channel_time_read },
	{ CMD_SETSCANNPROBES,               slsi_roam_scan_probe_interval_write },
	{ CMD_GETSCANNPROBES,               slsi_roam_scan_probe_interval_read },
	{ CMD_SETROAMMODE,                  slsi_roam_mode_write },
	{ CMD_GETROAMMODE,                  slsi_roam_mode_read },
	{ CMD_SETROAMINTRABAND,             slsi_roam_scan_band_write },
	{ CMD_GETROAMINTRABAND,             slsi_roam_scan_band_read },
	{ CMD_SETROAMSCANCONTROL,           slsi_roam_scan_control_write },
	{ CMD_GETROAMSCANCONTROL,           slsi_roam_scan_control_read },
	{ CMD_SETSCANHOMETIME,              slsi_roam_scan_home_time_write },
	{ CMD_GETSCANHOMETIME,              slsi_roam_scan_home_time_read },
	{ CMD_SETSCANHOMEAWAYTIME,          slsi_roam_scan_home_away_time_write },
	{ CMD_GETSCANHOMEAWAYTIME,          slsi_roam_scan_home_away_time_read },
	{ CMD_SETOKCMODE,                   slsi_okc_mode_write },
	{ CMD_GETOKCMODE,                   slsi_okc_mode_read },
	{ CMD_SETWESMODE,                   slsi_wes_mode_write },
	{ CMD_GETWESMODE,                   slsi_wes_mode_read },
	{ CMD_SETROAMSCANCHANNELS,          slsi_roam_scan_channels_write },
	{ CMD_SETROAMSCANFREQUENCIES,       slsi_roam_scan_frequencies_write },
	{ CMD_GETROAMSCANCHANNELS,          slsi_roam_scan_channels_read },
	{ CMD_GETROAMSCANFREQUENCIES,       slsi_roam_scan_frequencies_read },
	{ CMD_ADDROAMSCANCHANNELS,          slsi_roam_add_scan_channels },
	{ CMD_ADDROAMSCANFREQUENCIES,       slsi_roam_add_scan_frequencies },
	{ CMD_GETNCHOMODE,                  slsi_get_ncho_mode },
	{ CMD_SETNCHOMODE,                  slsi_set_ncho_mode },
	{ CMD_SETDFSSCANMODE,               slsi_set_dfs_scan_mode },
	{ CMD_GETDFSSCANMODE,               slsi_get_dfs_scan_mode },
	{ CMD_REASSOC_FREQUENCY,            slsi_reassoc_frequency_write },
	{ CMD_REASSOC,                      slsi_reassoc_write },
	{ CMD_SETROAMOFFLOAD,               slsi_roam_mode_write },
	{ CMD_SETROAMOFFLAPLIST,            slsi_roam_offload_ap_list },
#endif
	{ CMD_SET_PMK,                      slsi_set_pmk },
	{ CMD_HAPD_GET_CHANNEL,             slsi_auto_chan_read },
	{ CMD_SET_SAP_CHANNEL_LIST,         slsi_ioctl_auto_chan_write },
	{ CMD_SENDACTIONFRAME,              slsi_send_action_frame },
	{ CMD_CERTSENDACTIONFRAME,          slsi_send_action_frame_cert },
#ifdef SLSI_TEST_DEV
	{ CMD_GASSENDACTIONFRAME,           slsi_send_action_frame_ut },
#endif
	{ CMD_HAPD_MAX_NUM_STA,             slsi_setting_max_sta_write },
	{ CMD_HAPD_SET_AX_MODE,             slsi_setting_sap_ax_mode },
	{ CMD_COUNTRY,                      slsi_country_write },
#if defined(CONFIG_SLSI_WLAN_STA_FWD_BEACON) && (defined(SCSC_SEP_VERSION) && SCSC_SEP_VERSION >= 10)
	{ CMD_BEACON_RECV,                  slsi_forward_beacon },
#endif
	{ CMD_SETAPP2PWPSIE,                slsi_set_ap_p2p_wps_ie },
	{ CMD_P2PSETPS,                     slsi_set_p2p_oppps },
	{ CMD_P2PSETNOA,                    slsi_p2p_set_noa_params },
	{ CMD_P2PECSA,                      slsi_p2p_ecsa },
	{ CMD_P2PLOSTART,                   slsi_p2p_lo_start },
	{ CMD_P2PLOSTOP,                    slsi_p2p_lo_stop },
	{ CMD_SET_TX_POWER_CALLING,         slsi_set_tx_power_calling },
	{ CMD_SET_TX_POWER_SUB6_BAND,       slsi_set_tx_power_sub6_band },
	{ CMD_POWER_MEASUREMENT_START,      slsi_start_power_measurement_detection },
	{ CMD_GETREGULATORY,                slsi_get_regulatory },
#ifdef CONFIG_SCSC_WLAN_HANG_TEST
	{ CMD_TESTFORCEHANG,                slsi_ioctl_test_force_hang },
#endif
	{ CMD_SET_FCC_CHANNEL,              slsi_set_fcc_channel },
	{ CMD_FAKEMAC,                      slsi_fake_mac_write },
	{ CMD_GETBSSRSSI,                   slsi_get_bss_rssi },
	{ CMD_GETBSSINFO,                   slsi_get_bss_info },
	{ CMD_GETSTAINFO,                   slsi_get_sta_info },
	{ CMD_GETASSOCREJECTINFO,           slsi_get_assoc_reject_info },
#ifdef CONFIG_SCSC_WLAN_LOW_LATENCY_MODE
	{ CMD_SET_LATENCY_MODE,             slsi_ioctl_set_latency_mode },
	{ CMD_SET_POWER_MGMT,               slsi_set_power_mode },
#endif
	{ CMD_SET_LATENCY_CRT_DATA,         slsi_ioctl_set_latency_crt_data },
	{ CMD_SET_DISCONNECT_IES,           slsi_set_disconnect_ies },
#ifdef CONFIG_SCSC_WLAN_STA_ENHANCED_ARP_DETECT
	{ CMD_SET_ENHANCED_ARP_TARGET,      slsi_enhanced_arp_start_stop },
	{ CMD_GET_ENHANCED_ARP_COUNTS,      slsi_enhanced_arp_get_stats },
#endif
	{ CMD_RXFILTERSTART,                slsi_ioctl_cmd_success },
	{ CMD_RXFILTERSTOP,                 slsi_ioctl_cmd_success },
	{ CMD_BTCOEXMODE,                   slsi_ioctl_cmd_success },
	{ CMD_BTCOEXSCAN_START,             slsi_ioctl_cmd_success },
	{ CMD_BTCOEXSCAN_STOP,              slsi_ioctl_cmd_success },
	{ CMD_MIRACAST,                     slsi_ioctl_cmd_success },
#ifndef SLSI_TEST_DEV
	{ CMD_DRIVERDEBUGDUMP,              slsi_ioctl_driver_bug_dump },
	{ CMD_DRIVERDEBUGCOMMAND,           slsi_ioctl_driver_bug_dump },
#endif
#ifdef CONFIG_SCSC_WLAN_ENHANCED_PKT_FILTER
	{ CMD_ENHANCED_PKT_FILTER,          slsi_set_enhanced_pkt_filter },
#endif
#ifdef CONFIG_SCSC_WLAN_NUM_ANTENNAS
	{ CMD_SET_NUM_ANTENNAS,             slsi_ioctl_set_num_antennas },
	{ CMD_GET_NUM_ANTENNAS,             slsi_ioctl_get_num_antennas },
#endif
#ifdef CONFIG_SCSC_WLAN_MAX_LINK_SPEED
	{ CMD_GET_MAX_LINK_SPEED,           slsi_get_linkspeed },
#endif

#ifdef CONFIG_SCSC_WLAN_DYNAMIC_ITO
	{ CMD_SET_ITO,                      slsi_set_ito },
	{ CMD_ENABLE_ITO,                   slsi_enable_ito },
#endif
	{ CMD_GET_CU,                       slsi_get_cu },
	{ CMD_ELNA_BYPASS_INT,              slsi_elna_bypass_int },
	{ CMD_ELNA_BYPASS,                  slsi_elna_bypass },
	{ CMD_SET_DWELL_TIME,               slsi_set_dwell_time },
	{ CMD_SET_DTIM_IN_SUSPEND,          slsi_set_dtim_suspend },
	{ CMD_MAX_DTIM_IN_SUSPEND,          slsi_max_dtim_suspend },
	{ CMD_GET_PS_DISABLED_DURATION,     slsi_ioctl_get_ps_disabled_duration},
	{ CMD_GET_PS_ENTRY_COUNTER,         slsi_ioctl_get_ps_entry_counter},
	{ CMD_FORCE_ROAMING_BSSID,          slsi_force_roaming_bssid },
	{ CMD_ROAMING_BLACKLIST_ADD,        slsi_roaming_blacklist_add },
	{ CMD_ROAMING_BLACKLIST_REMOVE,     slsi_roaming_blacklist_remove },
	{ CMD_TWT_SETUP,                    slsi_twt_setup},
	{ CMD_TWT_TEARDOWN,                 slsi_twt_teardown},
	{ CMD_TWT_SUSPEND,                  slsi_twt_suspend},
	{ CMD_GET_TWT_STATUS,               slsi_twt_get_status},
	{ CMD_GET_TWT_CAP,                  slsi_twt_get_capabilities},
	{ CMD_GET_TWT_STATISTICS,           slsi_twt_get_statistics_query},
	{ CMD_CLEAR_TWT_STATISTICS,         slsi_twt_clear_statistics},
	{ CMD_ADPS_ENABLE,                  slsi_adps_enable},
	{ CMD_SCHED_PM_SETUP,               slsi_sched_pm_setup},
	{ CMD_SCHED_PM_TEARDOWN,            slsi_sched_pm_teardown},
	{ CMD_GET_SCHED_PM_STATUS,          slsi_sched_pm_status},
	{CMD_SET_DELAYED_WAKEUP,            slsi_set_delayed_wakeup},
	{CMD_SET_DELAYED_WAKEUP_TYPE,       slsi_set_delayed_wakeup_type},
#ifdef CONFIG_SCSC_WLAN_SAP_POWER_SAVE
	{ CMD_SET_AP_SUSPEND,               slsi_set_ap_suspend},
	{ CMD_SET_AP_RPS_PARAMS,            slsi_set_ap_rps_params},
	{ CMD_SET_AP_RPS,                   slsi_set_ap_rps},
	{ CMD_SET_HOSTAPD_TX_POWER,         slsi_set_ap_tx_power},
	{ CMD_GET_HOSTAPD_TX_POWER,         slsi_get_ap_tx_power},
#endif
	{CMD_LEAKY_AP_ACTIVE_DETECTION,        slsi_sched_pm_leaky_ap_active_detection},
	{CMD_LEAKY_AP_PASSIVE_DETECTION_START, slsi_sched_pm_leaky_ap_passive_detection_start},
	{CMD_LEAKY_AP_PASSIVE_DETECTION_END,   slsi_sched_pm_leaky_ap_passive_detection_end},
	{CMD_SET_GRACE_PERIOD,                 slsi_sched_pm_set_grace_period},
	{CMD_GET_TDLS_MAX_SESSION,             slsi_get_tdls_max_session},
	{CMD_GET_TDLS_NUM_OF_SESSION,          slsi_get_tdls_num_of_session},
	{CMD_GET_TDLS_WIDER_BW,                slsi_get_tdls_wider_bandwidth},
	{CMD_GET_TDLS_AVAILABLE,               slsi_get_tdls_available_state},
	{CMD_SET_TDLS_ENABLED,                 slsi_set_tdls_enabled},
	{CMD_TWT_SOFTAP_ENABLE,                slsi_twt_softap_enable},
	{CMD_HAPD_TWT_TEARDOWN,                slsi_hapd_twt_teardown},
	{CMD_HAPD_GET_TWT_STATUS,              slsi_hapd_get_twt_status},
	{CMD_SET_ELNABYPASS_THRESHOLD,         slsi_set_elnabypass_threshold},
	{CMD_GET_ELNA_STATUS,                  slsi_get_elna_status},
#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
	{CMD_GET_WIFI6E_CHANNELS,           slsi_get_wifi6e_channels},
	{CMD_SETWSECINFO,                   slsi_set_wifi_safe_mode},
#endif
	{CMD_GET_TX_ANT_CONFIG,             slsi_get_tx_ant_config},
	{CMD_SET_TX_ANT_CONFIG,             slsi_set_tx_ant_config},
	{CMD_GET_STA_DUMP,                  slsi_get_sta_dump}
};

static int slsi_ioctl_fn_lookup(char *command, int len)
{
	int i = 0;
	const struct slsi_ioctl_fn *p = NULL;

	for (i = 0; i < ARRAY_SIZE(slsi_ioctl_fn_table); i++) {
		p = &slsi_ioctl_fn_table[i];
		if (p->name) {
			if (strlen(p->name) < len) {
				if (!strncasecmp(command, p->name, len))
					return i;
			} else {
				if (!strncasecmp(command, p->name, strlen(p->name)))
					return i;
			}
		}
	}
	return -1;
}

static int slsi_do_ioctl(struct net_device *dev, char *command, int cmd_len)
{
	int ret = 0;
	int index;
	int len = 0;
	char *pos = command;

	pos = strchr(pos, ' ');
	if (!pos) {
		pos = command;
		pos = strchr(pos, '\0');
		if (!pos)
			return -ENOTSUPP;
	}

	len = pos - command;
	index = slsi_ioctl_fn_lookup(command, len);

	if (index < 0)
		return -ENOTSUPP;

	if (slsi_ioctl_fn_table[index].fn)
		ret = slsi_ioctl_fn_table[index].fn(dev, command, cmd_len);
	else
		ret = -ENOTSUPP;

	return ret;
}

int slsi_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
#define MAX_LEN_PRIV_COMMAND    4096 /*This value is the max reply size set in supplicant*/
	struct android_wifi_priv_cmd priv_cmd;
	int                          ret = 0;
	u8                           *command = NULL;

	if (!dev) {
		ret = -ENODEV;
		goto exit;
	}

	if (!rq->ifr_data) {
		ret = -EINVAL;
		goto exit;
	}
	if (copy_from_user((void *)&priv_cmd, (void *)rq->ifr_data, sizeof(struct android_wifi_priv_cmd))) {
		ret = -EFAULT;
		SLSI_NET_ERR(dev, "ifr data failed\n");
		goto exit;
	}

	if (priv_cmd.total_len > MAX_LEN_PRIV_COMMAND || priv_cmd.total_len < 0) {
		ret = -EINVAL;
		SLSI_NET_ERR(dev, "Length mismatch total_len = %d\n", priv_cmd.total_len);
		goto exit;
	}
	command = kmalloc((priv_cmd.total_len + 1), GFP_KERNEL);
	if (!command) {
		ret = -ENOMEM;
		SLSI_NET_ERR(dev, "No memory\n");
		goto exit;
	}
	if (copy_from_user(command, priv_cmd.buf, priv_cmd.total_len)) {
		ret = -EFAULT;
		SLSI_NET_ERR(dev, "Buffer copy fail\n");
		goto exit;
	}
	command[priv_cmd.total_len] = '\0';

	if (strncasecmp(command, CMD_SET_PMK, strlen(CMD_SET_PMK)) == 0)
		SLSI_INFO_NODEV("command: SET_PMK\n");
	else
		SLSI_INFO_NODEV("command: %.*s\n", priv_cmd.total_len, command);

	ret = slsi_do_ioctl(dev, command, priv_cmd.total_len);

	if (strncasecmp(command, CMD_SETROAMBAND, strlen(CMD_SETROAMBAND)) != 0 && strncasecmp(command, CMD_FACTORY_SETBAND, strlen(CMD_FACTORY_SETBAND)) != 0 && strncasecmp(command, CMD_SETBAND, strlen(CMD_SETBAND)) != 0 && copy_to_user(priv_cmd.buf, command, priv_cmd.total_len)) {
		ret = -EFAULT;
		SLSI_NET_ERR(dev, "Buffer copy fail\n");
	}

exit:
	kfree(command);
	return ret;
}
