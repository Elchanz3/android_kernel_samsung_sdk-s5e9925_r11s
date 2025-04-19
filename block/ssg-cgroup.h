/* SPDX-License-Identifier: GPL-2.0 */

#ifndef SSG_CGROUP_H
#define SSG_CGROUP_H
#include <linux/blk-cgroup.h>
#include <linux/atomic.h>

#if IS_ENABLED(CONFIG_MQ_IOSCHED_SSG_CGROUP)

#define SSG_ATOMIC_ADD_RELAXED(v, i) \
    __atomic_add_fetch(&((v)->counter), (i), __ATOMIC_RELAXED)

struct ssg_blkcg {
        struct blkcg_policy_data cpd __aligned(64); // Sintaxe corrigida
        int max_available_ratio;
};

struct ssg_blkg {
        struct blkg_policy_data pd __aligned(64); // Sintaxe corrigida
        atomic_t current_rqs;
        int max_available_rqs;
        unsigned int shallow_depth;
};

extern int ssg_blkcg_init(void);
extern void ssg_blkcg_exit(void);
extern int ssg_blkcg_activate(struct request_queue *q);
extern void ssg_blkcg_deactivate(struct request_queue *q);
extern unsigned int ssg_blkcg_shallow_depth(struct request_queue *q);
extern void ssg_blkcg_depth_updated(struct blk_mq_hw_ctx *hctx);
extern void ssg_blkcg_inc_rq(struct blkcg_gq *blkg);
extern void ssg_blkcg_dec_rq(struct blkcg_gq *blkg);
#else
int ssg_blkcg_init(void)
{
	return 0;
}
void ssg_blkcg_exit(void)
{
}

int ssg_blkcg_activate(struct request_queue *q)
{
	return 0;
}

void ssg_blkcg_deactivate(struct request_queue *q)
{
}

unsigned int ssg_blkcg_shallow_depth(struct request_queue *q)
{
	return 0;
}

void ssg_blkcg_depth_updated(struct blk_mq_hw_ctx *hctx)
{
}

void ssg_blkcg_inc_rq(struct blkcg_gq *blkg)
{
}

void ssg_blkcg_dec_rq(struct blkcg_gq *blkg)
{
}

#endif

#endif
