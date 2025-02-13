// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * DPU profiler file for Samsung EXYNOS DPU driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <exynos_drm_profiler.h>
#include <soc/samsung/exynos-profiler-external.h>
#include <soc/samsung/xperf.h>
#include <exynos_drm_crtc.h>

#define PROFILER_PERIOD_NSEC	100000000UL
#define FPS_UNIT_NSEC		1000000000UL	/* 1sec */

/* only valid for LCD display crtc */
static struct exynos_drm_crtc *primary_exynos_crtc;

static void get_frame_vsync_cnt(u64 *cnt, ktime_t *time)
{
	const struct exynos_profiler *profiler = primary_exynos_crtc->profiler;
	const struct drm_crtc_state *crtc_state = primary_exynos_crtc->base.state;
	u64 lcd_fps;
	u64 vsync_period_nsec;
	static u64 vsync_cnt;
	static ktime_t prev_ktime;
	ktime_t vsync_time;
	ktime_t cur_ktime = ktime_get();
	ktime_t ktime_diff;

	lcd_fps = drm_mode_vrefresh(&crtc_state->mode);
	if (!lcd_fps) {
		pr_info("%s: there is no vrefresh\n", __func__);
		return;
	}
	vsync_period_nsec = FPS_UNIT_NSEC / lcd_fps;

	if (vsync_cnt) {
		if (prev_ktime) {
			ktime_diff = cur_ktime - prev_ktime;
			vsync_cnt += DIV_ROUND_CLOSEST(lcd_fps * ktime_diff, FPS_UNIT_NSEC);
		} else
			vsync_cnt += lcd_fps * PROFILER_PERIOD_NSEC / FPS_UNIT_NSEC;
	} else
		vsync_cnt = profiler->frame_vsync_cnt;

	if ((cur_ktime - profiler->frame_vsync_cnt_time) > vsync_period_nsec)
		vsync_time = profiler->frame_vsync_cnt_time + vsync_period_nsec;
	else
		vsync_time = profiler->frame_vsync_cnt_time;

	*cnt = vsync_cnt;
	*time = vsync_time;
	prev_ktime = cur_ktime;
}

static void get_ems_frame_cnt(u64 *cnt, ktime_t *time)
{
	const struct exynos_profiler *profiler = primary_exynos_crtc->profiler;

	*cnt = profiler->ems_frame_cnt;
	*time = profiler->ems_frame_cnt_time;
}

static void get_ems_fence_cnt(u64 *cnt, ktime_t *time)
{
	const struct exynos_profiler *profiler = primary_exynos_crtc->profiler;

	*cnt = profiler->ems_fence_cnt;
	*time = profiler->ems_fence_cnt_time;
}

#if IS_ENABLED(CONFIG_EXYNOS_GPU_PROFILER)
void exynos_profiler_set_vsync(ktime_t time_real_us);
#endif
void exynos_profiler_update_vsync_cnt(struct exynos_profiler *profiler)
{
	profiler->frame_vsync_cnt++;
	profiler->frame_vsync_cnt_time = ktime_get();
#if IS_ENABLED(CONFIG_EXYNOS_GPU_PROFILER)
	exynos_profiler_set_vsync(ktime_get_real() / 1000UL);
#endif
}

void exynos_profiler_update_ems_frame_cnt(struct exynos_profiler *profiler)
{
	profiler->ems_frame_cnt++;
	profiler->ems_frame_cnt_time = ktime_get();
}

void exynos_profiler_update_ems_fence_cnt(struct exynos_profiler *profiler)
{
	profiler->ems_fence_cnt++;
	profiler->ems_fence_cnt_time = ktime_get();
}

struct exynos_profiler *exynos_profiler_register(struct exynos_drm_crtc *exynos_crtc)
{
	struct exynos_profiler *profiler;
	struct drm_crtc *crtc = &exynos_crtc->base;

	/* profiler support only lcd crtc(index == 0) */
	if (!IS_ENABLED(CONFIG_EXYNOS_GPU_PROFILER) ||	crtc->index)
		return NULL;

	profiler = kzalloc(sizeof(struct exynos_profiler), GFP_KERNEL);

	primary_exynos_crtc = exynos_crtc;

	exynos_profiler_register_vsync_cnt(get_frame_vsync_cnt);
	exynos_profiler_register_frame_cnt(get_ems_frame_cnt);
	exynos_profiler_register_fence_cnt(get_ems_fence_cnt);
	exynos_gmc_register_frame_cnt(get_ems_frame_cnt);

	pr_info("%s[%d]: profiler supported\n", crtc->name, crtc->index);

	return profiler;
}
