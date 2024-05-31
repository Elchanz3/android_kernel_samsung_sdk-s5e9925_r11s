// SPDX-License-Identifier: GPL-2.0-only
/*
 *  drivers/cpufreq/exynos-now.c
 *
 *            (C) 2024 Chanz22.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cpu.h>
#include <linux/percpu-defs.h>
#include <linux/slab.h>
#include <linux/tick.h>
#include <linux/sched/cpufreq.h>

#include "exynos-now.h"

static struct exynos_now_ops exynos_now_ops;

static void dbs_freq_increase(struct cpufreq_policy *policy, unsigned int freq)
{
    if (policy->cur == policy->max)
        return;

    __cpufreq_driver_target(policy, freq, CPUFREQ_RELATION_H);
}

static void exynos_now_update(struct cpufreq_policy *policy)
{
    struct policy_dbs_info *policy_dbs = policy->governor_data;
    struct dbs_data *dbs_data = policy_dbs->dbs_data;
    unsigned int load = dbs_update(policy);

    if (load > dbs_data->up_threshold) {
        if (policy->cur < policy->max)
            policy_dbs->rate_mult = dbs_data->sampling_down_factor;
        dbs_freq_increase(policy, policy->max);
    } else {
        unsigned int freq_next, min_f, max_f;

        min_f = policy->cpuinfo.min_freq;
        max_f = policy->cpuinfo.max_freq;
        freq_next = min_f + load * (max_f - min_f) / 100;

        policy_dbs->rate_mult = 1;

        __cpufreq_driver_target(policy, freq_next, CPUFREQ_RELATION_C);
    }
}

static unsigned int exynos_now_dbs_update(struct cpufreq_policy *policy)
{
    struct policy_dbs_info *policy_dbs = policy->governor_data;
    struct dbs_data *dbs_data = policy_dbs->dbs_data;

    exynos_now_update(policy);

    return dbs_data->sampling_rate * policy_dbs->rate_mult;
}

/************************** sysfs interface ************************/
static struct dbs_governor exynos_now_dbs_gov;

static ssize_t store_io_is_busy(struct gov_attr_set *attr_set, const char *buf,
                                size_t count)
{
    struct dbs_data *dbs_data = to_dbs_data(attr_set);
    unsigned int input;
    int ret;

    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    dbs_data->io_is_busy = !!input;

    gov_update_cpu_data(dbs_data);

    return count;
}

static ssize_t store_up_threshold(struct gov_attr_set *attr_set,
                                  const char *buf, size_t count)
{
    struct dbs_data *dbs_data = to_dbs_data(attr_set);
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);

    if (ret != 1 || input > MAX_FREQUENCY_UP_THRESHOLD ||
            input < MIN_FREQUENCY_UP_THRESHOLD) {
        return -EINVAL;
    }

    dbs_data->up_threshold = input;
    return count;
}

static ssize_t store_sampling_down_factor(struct gov_attr_set *attr_set,
                                          const char *buf, size_t count)
{
    struct dbs_data *dbs_data = to_dbs_data(attr_set);
    struct policy_dbs_info *policy_dbs;
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);

    if (ret != 1 || input > MAX_SAMPLING_DOWN_FACTOR || input < 1)
        return -EINVAL;

    dbs_data->sampling_down_factor = input;

    for_each_dbs_info(policy_dbs, dbs_data)
    policy_dbs->rate_mult = 1;

    return count;
}

gov_attr_rw(io_is_busy);
gov_attr_rw(up_threshold);
gov_attr_rw(sampling_down_factor);

static struct attribute *dbs_attributes[] = {
    &io_is_busy.attr,
    &up_threshold.attr,
    &sampling_down_factor.attr,
    NULL
};

static struct gov_attr_set exynos_now_attr_set = {
    .attr_set = {
        .attrs = dbs_attributes,
        .name = "exynos-now"
    }
};

static struct dbs_governor exynos_now_dbs_gov = {
    .gov = CPUFREQ_DBS_GOVERNOR_INITIALIZER("exynos-now"),
    .gov.attr_set = &exynos_now_attr_set,
    .gov.init = cpufreq_gov_dbs_init,
    .gov.exit = cpufreq_gov_dbs_exit,
    .gov.start = cpufreq_gov_dbs_start,
    .gov.stop = cpufreq_gov_dbs_stop,
    .gov.sampling_rate = MICRO_FREQUENCY_MIN_SAMPLE_RATE,
    .gov.sampling_down_factor = DEF_SAMPLING_DOWN_FACTOR,
    .gov.dbs_update = exynos_now_dbs_update,
};

static int __init cpufreq_gov_exynos_now_init(void)
{
    return cpufreq_register_governor(&exynos_now_dbs_gov.gov);
}

static void __exit cpufreq_gov_exynos_now_exit(void)
{
    cpufreq_unregister_governor(&exynos_now_dbs_gov.gov);
}

module_init(cpufreq_gov_exynos_now_init);
module_exit(cpufreq_gov_exynos_now_exit);

MODULE_AUTHOR("Chanz22");
MODULE_DESCRIPTION("EXYNOS-NOW CPUfreq policy governor");
MODULE_LICENSE("GPL");

