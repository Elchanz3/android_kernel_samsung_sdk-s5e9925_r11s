/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Header file for CPUFreq exynos-now governor and related code.
 *
 * Author: Chanz22 <inutilidades639@gmail.com>
 */

#include "cpufreq_governor.h"

struct exynos_now_policy_dbs_info {
    struct policy_dbs_info policy_dbs;
    unsigned int sample_type:1;
};

static inline struct exynos_now_policy_dbs_info *to_dbs_info(struct policy_dbs_info *policy_dbs)
{
    return container_of(policy_dbs, struct exynos_now_policy_dbs_info, policy_dbs);
}

