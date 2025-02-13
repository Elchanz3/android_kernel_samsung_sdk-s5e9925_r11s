/*
 * linux/drivers/video/fbdev/exynos/panel/s6e3fc3/s6e3fc3_r11s_panel.h
 *
 * Header file for S6E3FC3 Dimming Driver
 *
 * Copyright (c) 2016 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __S6E3FC3_R11S_PANEL_H__
#define __S6E3FC3_R11S_PANEL_H__

#include "../panel.h"
#include "../panel_drv.h"
#include "../panel_debug.h"
#include "oled_function.h"
#include "oled_property.h"
#include "s6e3fc3.h"
#include "s6e3fc3_r11s.h"
#include "s6e3fc3_dimming.h"
#ifdef CONFIG_USDM_MDNIE
#include "s6e3fc3_r11s_panel_mdnie.h"
#endif
#include "s6e3fc3_r11s_panel_dimming.h"
#ifdef CONFIG_USDM_PANEL_AOD_BL
#include "s6e3fc3_r11s_panel_aod_dimming.h"
#endif
#ifdef CONFIG_USDM_PANEL_SELF_DISPLAY
#include "s6e3fc3_r11s_aod_panel.h"
#include "../aod/aod_drv.h"
#endif

#include "s6e3fc3_r11s_resol.h"

#undef __pn_name__
#define __pn_name__	r11s

#undef __PN_NAME__
#define __PN_NAME__

/* ===================================================================================== */
/* ============================= [S6E3FC3 READ INFO TABLE] ============================= */
/* ===================================================================================== */
/* <READINFO TABLE> IS DEPENDENT ON LDI. IF YOU NEED, DEFINE PANEL's RESOURCE TABLE */


/* ===================================================================================== */
/* ============================= [S6E3FC3 RESOURCE TABLE] ============================== */
/* ===================================================================================== */
/* <RESOURCE TABLE> IS DEPENDENT ON LDI. IF YOU NEED, DEFINE PANEL's RESOURCE TABLE */


/* ===================================================================================== */
/* ============================== [S6E3FC3 MAPPING TABLE] ============================== */
/* ===================================================================================== */

static u8 r11s_brt_table[S6E3FC3_TOTAL_STEP][2] = {
	{ 0x00, 0x08 }, { 0x00, 0x0A }, { 0x00, 0x0C }, { 0x00, 0x0F }, { 0x00, 0x12 },
	{ 0x00, 0x15 }, { 0x00, 0x18 }, { 0x00, 0x1C }, { 0x00, 0x1F }, { 0x00, 0x23 },
	{ 0x00, 0x27 }, { 0x00, 0x2B }, { 0x00, 0x30 }, { 0x00, 0x34 }, { 0x00, 0x39 },
	{ 0x00, 0x3D }, { 0x00, 0x42 }, { 0x00, 0x46 }, { 0x00, 0x4B }, { 0x00, 0x50 },
	{ 0x00, 0x55 }, { 0x00, 0x5A }, { 0x00, 0x5F }, { 0x00, 0x64 }, { 0x00, 0x6A },
	{ 0x00, 0x6F }, { 0x00, 0x74 }, { 0x00, 0x7A }, { 0x00, 0x7F }, { 0x00, 0x85 },
	{ 0x00, 0x8B }, { 0x00, 0x90 }, { 0x00, 0x96 }, { 0x00, 0x9C }, { 0x00, 0xA2 },
	{ 0x00, 0xA8 }, { 0x00, 0xAE }, { 0x00, 0xB4 }, { 0x00, 0xBA }, { 0x00, 0xC0 },
	{ 0x00, 0xC6 }, { 0x00, 0xCC }, { 0x00, 0xD2 }, { 0x00, 0xD9 }, { 0x00, 0xDF },
	{ 0x00, 0xE5 }, { 0x00, 0xEC }, { 0x00, 0xF2 }, { 0x00, 0xF9 }, { 0x00, 0xFF },
	{ 0x01, 0x06 }, { 0x01, 0x0C }, { 0x01, 0x13 }, { 0x01, 0x1A }, { 0x01, 0x21 },
	{ 0x01, 0x27 }, { 0x01, 0x2E }, { 0x01, 0x35 }, { 0x01, 0x3C }, { 0x01, 0x43 },
	{ 0x01, 0x4A }, { 0x01, 0x51 }, { 0x01, 0x58 }, { 0x01, 0x5B }, { 0x01, 0x66 },
	{ 0x01, 0x6D }, { 0x01, 0x74 }, { 0x01, 0x7B }, { 0x01, 0x82 }, { 0x01, 0x89 },
	{ 0x01, 0x90 }, { 0x01, 0x97 }, { 0x01, 0x9E }, { 0x01, 0xA6 }, { 0x01, 0xAD },
	{ 0x01, 0xB4 }, { 0x01, 0xBC }, { 0x01, 0xC3 }, { 0x01, 0xCA }, { 0x01, 0xD2 },
	{ 0x01, 0xD9 }, { 0x01, 0xE1 }, { 0x01, 0xE8 }, { 0x01, 0xF0 }, { 0x01, 0xF7 },
	{ 0x01, 0xFF }, { 0x02, 0x06 }, { 0x02, 0x0E }, { 0x02, 0x16 }, { 0x02, 0x1D },
	{ 0x02, 0x25 }, { 0x02, 0x2D }, { 0x02, 0x35 }, { 0x02, 0x3C }, { 0x02, 0x44 },
	{ 0x02, 0x4C }, { 0x02, 0x54 }, { 0x02, 0x5C }, { 0x02, 0x64 }, { 0x02, 0x6C },
	{ 0x02, 0x74 }, { 0x02, 0x7C }, { 0x02, 0x84 }, { 0x02, 0x8C }, { 0x02, 0x94 },
	{ 0x02, 0x9C }, { 0x02, 0xA4 }, { 0x02, 0xAC }, { 0x02, 0xB4 }, { 0x02, 0xBC },
	{ 0x02, 0xC4 }, { 0x02, 0xCD }, { 0x02, 0xD5 }, { 0x02, 0xDD }, { 0x02, 0xE5 },
	{ 0x02, 0xEE }, { 0x02, 0xF6 }, { 0x02, 0xFE }, { 0x02, 0xFF }, { 0x03, 0x0F },
	{ 0x03, 0x17 }, { 0x03, 0x1F }, { 0x03, 0x27 }, { 0x03, 0x30 }, { 0x03, 0x38 },
	{ 0x03, 0x40 }, { 0x03, 0x48 }, { 0x03, 0x51 }, { 0x03, 0x59 }, { 0x03, 0x61 },
	{ 0x03, 0x6A }, { 0x03, 0x72 }, { 0x03, 0x7A }, { 0x03, 0x83 }, { 0x03, 0x8B },
	{ 0x03, 0x94 }, { 0x03, 0x9C }, { 0x03, 0xA5 }, { 0x03, 0xAD }, { 0x03, 0xB6 },
	{ 0x03, 0xBE }, { 0x03, 0xC7 }, { 0x03, 0xD0 }, { 0x03, 0xD8 }, { 0x03, 0xE1 },
	{ 0x03, 0xE9 }, { 0x03, 0xF2 }, { 0x03, 0xFB }, { 0x04, 0x03 }, { 0x04, 0x0C },
	{ 0x04, 0x15 }, { 0x04, 0x1E }, { 0x04, 0x26 }, { 0x04, 0x2F }, { 0x04, 0x38 },
	{ 0x04, 0x41 }, { 0x04, 0x4A }, { 0x04, 0x53 }, { 0x04, 0x5B }, { 0x04, 0x64 },
	{ 0x04, 0x6D }, { 0x04, 0x76 }, { 0x04, 0x7F }, { 0x04, 0x88 }, { 0x04, 0x91 },
	{ 0x04, 0x9A }, { 0x04, 0xA3 }, { 0x04, 0xAC }, { 0x04, 0xB5 }, { 0x04, 0xBE },
	{ 0x04, 0xC7 }, { 0x04, 0xD0 }, { 0x04, 0xD9 }, { 0x04, 0xE2 }, { 0x04, 0xEC },
	{ 0x04, 0xF5 }, { 0x04, 0xFE }, { 0x05, 0x07 }, { 0x05, 0x10 }, { 0x05, 0x1A },
	{ 0x05, 0x23 }, { 0x05, 0x2C }, { 0x05, 0x35 }, { 0x05, 0x3F }, { 0x05, 0x48 },
	{ 0x05, 0x51 }, { 0x05, 0x5A }, { 0x05, 0x64 }, { 0x05, 0x6D }, { 0x05, 0x76 },
	{ 0x05, 0x80 }, { 0x05, 0x89 }, { 0x05, 0x93 }, { 0x05, 0x9C }, { 0x05, 0xA6 },
	{ 0x05, 0xAF }, { 0x05, 0xB8 }, { 0x05, 0xC2 }, { 0x05, 0xCB }, { 0x05, 0xD5 },
	{ 0x05, 0xDE }, { 0x05, 0xE8 }, { 0x05, 0xF2 }, { 0x05, 0xFB }, { 0x06, 0x05 },
	{ 0x06, 0x0E }, { 0x06, 0x18 }, { 0x06, 0x21 }, { 0x06, 0x2B }, { 0x06, 0x35 },
	{ 0x06, 0x3E }, { 0x06, 0x48 }, { 0x06, 0x52 }, { 0x06, 0x5C }, { 0x06, 0x65 },
	{ 0x06, 0x6F }, { 0x06, 0x79 }, { 0x06, 0x82 }, { 0x06, 0x8C }, { 0x06, 0x96 },
	{ 0x06, 0xA0 }, { 0x06, 0xAA }, { 0x06, 0xB3 }, { 0x06, 0xBD }, { 0x06, 0xC7 },
	{ 0x06, 0xD1 }, { 0x06, 0xDB }, { 0x06, 0xE5 }, { 0x06, 0xEF }, { 0x06, 0xF9 },
	{ 0x07, 0x03 }, { 0x07, 0x0D }, { 0x07, 0x16 }, { 0x07, 0x20 }, { 0x07, 0x2A },
	{ 0x07, 0x34 }, { 0x07, 0x3E }, { 0x07, 0x48 }, { 0x07, 0x52 }, { 0x07, 0x5D },
	{ 0x07, 0x67 }, { 0x07, 0x71 }, { 0x07, 0x7B }, { 0x07, 0x85 }, { 0x07, 0x8F },
	{ 0x07, 0x99 }, { 0x07, 0xA3 }, { 0x07, 0xAD }, { 0x07, 0xB8 }, { 0x07, 0xC2 },
	{ 0x07, 0xCC }, { 0x07, 0xD6 }, { 0x07, 0xE0 }, { 0x07, 0xEB }, { 0x07, 0xF5 },
	{ 0x07, 0xFF },
	{ 0x09, 0x33 }, { 0x09, 0x36 }, { 0x09, 0x39 }, { 0x09, 0x3C }, { 0x09, 0x3F },
	{ 0x09, 0x42 }, { 0x09, 0x44 }, { 0x09, 0x47 }, { 0x09, 0x4A }, { 0x09, 0x4D },
	{ 0x09, 0x50 }, { 0x09, 0x53 }, { 0x09, 0x56 }, { 0x09, 0x59 }, { 0x09, 0x5C },
	{ 0x09, 0x5F }, { 0x09, 0x61 }, { 0x09, 0x64 }, { 0x09, 0x67 }, { 0x09, 0x6A },
	{ 0x09, 0x6D }, { 0x09, 0x70 }, { 0x09, 0x73 }, { 0x09, 0x76 }, { 0x09, 0x79 },
	{ 0x09, 0x7C }, { 0x09, 0x7E }, { 0x09, 0x81 }, { 0x09, 0x84 }, { 0x09, 0x87 },
	{ 0x09, 0x8A }, { 0x09, 0x8D }, { 0x09, 0x90 }, { 0x09, 0x93 }, { 0x09, 0x96 },
	{ 0x09, 0x99 }, { 0x09, 0x9B }, { 0x09, 0x9E }, { 0x09, 0xA1 }, { 0x09, 0xA4 },
	{ 0x09, 0xA7 }, { 0x09, 0xAA }, { 0x09, 0xAD }, { 0x09, 0xB0 }, { 0x09, 0xB3 },
	{ 0x09, 0xB5 }, { 0x09, 0xB8 }, { 0x09, 0xBB }, { 0x09, 0xBE }, { 0x09, 0xC1 },
	{ 0x09, 0xC4 }, { 0x09, 0xC7 }, { 0x09, 0xCA }, { 0x09, 0xCD }, { 0x09, 0xD0 },
	{ 0x09, 0xD2 }, { 0x09, 0xD5 }, { 0x09, 0xD8 }, { 0x09, 0xDB }, { 0x09, 0xDE },
	{ 0x09, 0xE1 }, { 0x09, 0xE4 }, { 0x09, 0xE7 }, { 0x09, 0xEA }, { 0x09, 0xED },
	{ 0x09, 0xEF }, { 0x09, 0xF2 }, { 0x09, 0xF5 }, { 0x09, 0xF8 }, { 0x09, 0xFB },
	{ 0x09, 0xFE }, { 0x0A, 0x01 }, { 0x0A, 0x04 }, { 0x0A, 0x07 }, { 0x0A, 0x0A },
	{ 0x0A, 0x0C }, { 0x0A, 0x0F }, { 0x0A, 0x12 }, { 0x0A, 0x15 }, { 0x0A, 0x18 },
	{ 0x0A, 0x1B }, { 0x0A, 0x1E }, { 0x0A, 0x21 }, { 0x0A, 0x24 }, { 0x0A, 0x27 },
	{ 0x0A, 0x29 }, { 0x0A, 0x2C }, { 0x0A, 0x2F }, { 0x0A, 0x32 }, { 0x0A, 0x35 },
	{ 0x0A, 0x38 }, { 0x0A, 0x3B }, { 0x0A, 0x3E }, { 0x0A, 0x41 }, { 0x0A, 0x44 },
	{ 0x0A, 0x46 }, { 0x0A, 0x49 }, { 0x0A, 0x4C }, { 0x0A, 0x4F }, { 0x0A, 0x52 },
	{ 0x0A, 0x55 }, { 0x0A, 0x58 }, { 0x0A, 0x5B }, { 0x0A, 0x5E }, { 0x0A, 0x61 },
	{ 0x0A, 0x63 }, { 0x0A, 0x66 }, { 0x0A, 0x69 }, { 0x0A, 0x6C }, { 0x0A, 0x6F },
	{ 0x0A, 0x72 }, { 0x0A, 0x75 }, { 0x0A, 0x78 }, { 0x0A, 0x7B }, { 0x0A, 0x7D },
	{ 0x0A, 0x80 }, { 0x0A, 0x83 }, { 0x0A, 0x86 }, { 0x0A, 0x89 }, { 0x0A, 0x8C },
	{ 0x0A, 0x8F }, { 0x0A, 0x92 }, { 0x0A, 0x95 }, { 0x0A, 0x98 }, { 0x0A, 0x9A },
	{ 0x0A, 0x9D }, { 0x0A, 0xA0 }, { 0x0A, 0xA3 }, { 0x0A, 0xA6 }, { 0x0A, 0xA9 },
	{ 0x0A, 0xAC }, { 0x0A, 0xAF }, { 0x0A, 0xB2 }, { 0x0A, 0xB5 }, { 0x0A, 0xB7 },
	{ 0x0A, 0xBA }, { 0x0A, 0xBD }, { 0x0A, 0xC0 }, { 0x0A, 0xC3 }, { 0x0A, 0xC6 },
	{ 0x0A, 0xC9 }, { 0x0A, 0xCC }, { 0x0A, 0xCF }, { 0x0A, 0xD2 }, { 0x0A, 0xD4 },
	{ 0x0A, 0xD7 }, { 0x0A, 0xDA }, { 0x0A, 0xDD }, { 0x0A, 0xE0 }, { 0x0A, 0xE3 },
	{ 0x0A, 0xE6 }, { 0x0A, 0xE9 }, { 0x0A, 0xEC }, { 0x0A, 0xEF }, { 0x0A, 0xF1 },
	{ 0x0A, 0xF4 }, { 0x0A, 0xF7 }, { 0x0A, 0xFA }, { 0x0A, 0xFD }, { 0x0B, 0x00 },
	{ 0x0B, 0x03 }, { 0x0B, 0x06 }, { 0x0B, 0x09 }, { 0x0B, 0x0C }, { 0x0B, 0x0E },
	{ 0x0B, 0x11 }, { 0x0B, 0x14 }, { 0x0B, 0x17 }, { 0x0B, 0x1A }, { 0x0B, 0x1D },
	{ 0x0B, 0x20 }, { 0x0B, 0x23 }, { 0x0B, 0x26 }, { 0x0B, 0x29 }, { 0x0B, 0x2B },
	{ 0x0B, 0x2E }, { 0x0B, 0x31 }, { 0x0B, 0x34 }, { 0x0B, 0x37 }, { 0x0B, 0x3A },
	{ 0x0B, 0x3D }, { 0x0B, 0x40 }, { 0x0B, 0x43 }, { 0x0B, 0x45 }, { 0x0B, 0x48 },
	{ 0x0B, 0x4B }, { 0x0B, 0x4E }, { 0x0B, 0x51 }, { 0x0B, 0x54 }, { 0x0B, 0x57 },
	{ 0x0B, 0x5A }, { 0x0B, 0x5D }, { 0x0B, 0x60 }, { 0x0B, 0x62 }, { 0x0B, 0x65 },
	{ 0x0B, 0x68 }, { 0x0B, 0x6B }, { 0x0B, 0x6E }, { 0x0B, 0x71 }, { 0x0B, 0x74 },
	{ 0x0B, 0x77 }, { 0x0B, 0x7A }, { 0x0B, 0x7D }, { 0x0B, 0x7F }, { 0x0B, 0x82 },
	{ 0x0B, 0x85 }, { 0x0B, 0x88 }, { 0x0B, 0x8B }, { 0x0B, 0x8E }, { 0x0B, 0x91 },
	{ 0x0B, 0x94 }, { 0x0B, 0x97 }, { 0x0B, 0x9A }, { 0x0B, 0x9C }, { 0x0B, 0x9F },
	{ 0x0B, 0xA2 }, { 0x0B, 0xA5 }, { 0x0B, 0xA8 }, { 0x0B, 0xAB }, { 0x0B, 0xAE },
	{ 0x0B, 0xB1 }, { 0x0B, 0xB4 }, { 0x0B, 0xB7 }, { 0x0B, 0xB9 }, { 0x0B, 0xBC },
	{ 0x0B, 0xBF }, { 0x0B, 0xC2 }, { 0x0B, 0xC5 }, { 0x0B, 0xC8 }, { 0x0B, 0xCB },
	{ 0x0B, 0xCE }, { 0x0B, 0xD1 }, { 0x0B, 0xD4 }, { 0x0B, 0xD6 }, { 0x0B, 0xD9 },
	{ 0x0B, 0xDC }, { 0x0B, 0xDF }, { 0x0B, 0xE2 }, { 0x0B, 0xE5 }, { 0x0B, 0xE8 },
	{ 0x0B, 0xEB }, { 0x0B, 0xEE }, { 0x0B, 0xF1 }, { 0x0B, 0xF3 }, { 0x0B, 0xF6 },
	{ 0x0B, 0xF9 }, { 0x0B, 0xFC }, { 0x0B, 0xFF },
};

static u8 r11s_hbm_transition_table[SMOOTH_TRANS_MAX][MAX_S6E3FC3_NORMAL_HBM_TRANSITION][1] = {
	[SMOOTH_TRANS_OFF] = {
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_NORMAL] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_HBM] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_NORMAL] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_HBM] = { 0x20 },
	},
	[SMOOTH_TRANS_ON] = {
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_NORMAL] = { 0x28 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_HBM] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_NORMAL] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_HBM] = { 0x20 },
	},
};

static u8 r11s_acl_frame_avg_table[][1] = {
	{ 0x44 }, /* 16 Frame Avg */
	{ 0x55 }, /* 32 Frame Avg */
};

static u8 r11s_acl_start_point_table[][2] = {
	[OLED_BR_HBM_OFF] = { 0x01, 0xB0 }, /* 50 Percent */
	[OLED_BR_HBM_ON] = { 0x41, 0x28 }, /* 60 Percent */
};

static u8 r11s_acl_dim_speed_table[MAX_S6E3FC3_ACL_DIM][1] = {
	[S6E3FC3_ACL_DIM_OFF] = { 0x00 }, /* 0x00 : ACL Dimming Off */
	[S6E3FC3_ACL_DIM_ON] = { 0x20 }, /* 0x20 : ACL Dimming 32 Frames */
};

static u8 r11s_acl_opr_table[MAX_S6E3FC3_ACL_OPR][1] = {
	[S6E3FC3_ACL_OPR_0] = { 0x00 }, /* ACL OFF, OPR 0% */
	[S6E3FC3_ACL_OPR_1] = { 0x01 }, /* ACL ON, OPR 8% */
	[S6E3FC3_ACL_OPR_2] = { 0x02 }, /* ACL ON, OPR 15% */
	[S6E3FC3_ACL_OPR_3] = { 0x02 }, /* ACL ON, OPR 15% */
};

static u8 r11s_tset_table[256][1] = {
	/* 0 ~ 127 */
	{ 0x00 }, { 0x01 }, { 0x02 }, { 0x03 }, { 0x04 }, { 0x05 }, { 0x06 }, { 0x07 },
	{ 0x08 }, { 0x09 }, { 0x0A }, { 0x0B }, { 0x0C }, { 0x0D }, { 0x0E }, { 0x0F },
	{ 0x10 }, { 0x11 }, { 0x12 }, { 0x13 }, { 0x14 }, { 0x15 }, { 0x16 }, { 0x17 },
	{ 0x18 }, { 0x19 }, { 0x1A }, { 0x1B }, { 0x1C }, { 0x1D }, { 0x1E }, { 0x1F },
	{ 0x20 }, { 0x21 }, { 0x22 }, { 0x23 }, { 0x24 }, { 0x25 }, { 0x26 }, { 0x27 },
	{ 0x28 }, { 0x29 }, { 0x2A }, { 0x2B }, { 0x2C }, { 0x2D }, { 0x2E }, { 0x2F },
	{ 0x30 }, { 0x31 }, { 0x32 }, { 0x33 }, { 0x34 }, { 0x35 }, { 0x36 }, { 0x37 },
	{ 0x38 }, { 0x39 }, { 0x3A }, { 0x3B }, { 0x3C }, { 0x3D }, { 0x3E }, { 0x3F },
	{ 0x40 }, { 0x41 }, { 0x42 }, { 0x43 }, { 0x44 }, { 0x45 }, { 0x46 }, { 0x47 },
	{ 0x48 }, { 0x49 }, { 0x4A }, { 0x4B }, { 0x4C }, { 0x4D }, { 0x4E }, { 0x4F },
	{ 0x50 }, { 0x51 }, { 0x52 }, { 0x53 }, { 0x54 }, { 0x55 }, { 0x56 }, { 0x57 },
	{ 0x58 }, { 0x59 }, { 0x5A }, { 0x5B }, { 0x5C }, { 0x5D }, { 0x5E }, { 0x5F },
	{ 0x60 }, { 0x61 }, { 0x62 }, { 0x63 }, { 0x64 }, { 0x65 }, { 0x66 }, { 0x67 },
	{ 0x68 }, { 0x69 }, { 0x6A }, { 0x6B }, { 0x6C }, { 0x6D }, { 0x6E }, { 0x6F },
	{ 0x70 }, { 0x71 }, { 0x72 }, { 0x73 }, { 0x74 }, { 0x75 }, { 0x76 }, { 0x77 },
	{ 0x78 }, { 0x79 }, { 0x7A }, { 0x7B }, { 0x7C }, { 0x7D }, { 0x7E }, { 0x7F },
	/* 0 ~ -127 */
	{ 0x80 }, { 0x81 }, { 0x82 }, { 0x83 }, { 0x84 }, { 0x85 }, { 0x86 }, { 0x87 },
	{ 0x88 }, { 0x89 }, { 0x8A }, { 0x8B }, { 0x8C }, { 0x8D }, { 0x8E }, { 0x8F },
	{ 0x90 }, { 0x91 }, { 0x92 }, { 0x93 }, { 0x94 }, { 0x95 }, { 0x96 }, { 0x97 },
	{ 0x98 }, { 0x99 }, { 0x9A }, { 0x9B }, { 0x9C }, { 0x9D }, { 0x9E }, { 0x9F },
	{ 0xA0 }, { 0xA1 }, { 0xA2 }, { 0xA3 }, { 0xA4 }, { 0xA5 }, { 0xA6 }, { 0xA7 },
	{ 0xA8 }, { 0xA9 }, { 0xAA }, { 0xAB }, { 0xAC }, { 0xAD }, { 0xAE }, { 0xAF },
	{ 0xB0 }, { 0xB1 }, { 0xB2 }, { 0xB3 }, { 0xB4 }, { 0xB5 }, { 0xB6 }, { 0xB7 },
	{ 0xB8 }, { 0xB9 }, { 0xBA }, { 0xBB }, { 0xBC }, { 0xBD }, { 0xBE }, { 0xBF },
	{ 0xC0 }, { 0xC1 }, { 0xC2 }, { 0xC3 }, { 0xC4 }, { 0xC5 }, { 0xC6 }, { 0xC7 },
	{ 0xC8 }, { 0xC9 }, { 0xCA }, { 0xCB }, { 0xCC }, { 0xCD }, { 0xCE }, { 0xCF },
	{ 0xD0 }, { 0xD1 }, { 0xD2 }, { 0xD3 }, { 0xD4 }, { 0xD5 }, { 0xD6 }, { 0xD7 },
	{ 0xD8 }, { 0xD9 }, { 0xDA }, { 0xDB }, { 0xDC }, { 0xDD }, { 0xDE }, { 0xDF },
	{ 0xE0 }, { 0xE1 }, { 0xE2 }, { 0xE3 }, { 0xE4 }, { 0xE5 }, { 0xE6 }, { 0xE7 },
	{ 0xE8 }, { 0xE9 }, { 0xEA }, { 0xEB }, { 0xEC }, { 0xED }, { 0xEE }, { 0xEF },
	{ 0xF0 }, { 0xF1 }, { 0xF2 }, { 0xF3 }, { 0xF4 }, { 0xF5 }, { 0xF6 }, { 0xF7 },
	{ 0xF8 }, { 0xF9 }, { 0xFA }, { 0xFB }, { 0xFC }, { 0xFD }, { 0xFE }, { 0xFF },
};

static u8 r11s_lpm_nit_table[4][2] = {
	/* LPM 2NIT: */
	{ 0x08, 0xF3 },
	/* LPM 10NIT */
	{ 0x06, 0x4A },
	/* LPM 30NIT */
	{ 0x04, 0xCF },
	/* LPM 60NIT */
	{ 0x00, 0x80 },
};

static u8 r11s_lpm_mode_table[4][2] = {
	/* LPM 2NIT: */
	{ 0x00, 0x08 },
	/* LPM 10NIT */
	{ 0x00, 0x2B },
	/* LPM 30NIT */
	{ 0x00, 0x7F },
	/* LPM 60NIT */
	{ 0x00, 0xFF },
};

static u8 r11s_ffc_table[MAX_S6E3FC3_R11S_HS_CLK][2] = {
	[S6E3FC3_R11S_HS_CLK_1328] = {0x45, 0xE6}, // FFC for HS: 1328
	[S6E3FC3_R11S_HS_CLK_1362] = {0x44, 0x27}, // FFC for HS: 1362
	[S6E3FC3_R11S_HS_CLK_1368] = {0x43, 0xDB}, // FFC for HS: 1368
};

static u8 r11s_fps_table_1[MAX_S6E3FC3_VRR][2] = {
	[S6E3FC3_VRR_120HS] = { 0x08, 0x00 },
	[S6E3FC3_VRR_60HS_120HS_TE_HW_SKIP_1] = { 0x08, 0x00 },
	[S6E3FC3_VRR_60HS] = { 0x00, 0x00 },
};

static u8 r11s_fps_table_2[MAX_S6E3FC3_VRR][1] = {
	[S6E3FC3_VRR_120HS] = { 0x01 },
	[S6E3FC3_VRR_60HS_120HS_TE_HW_SKIP_1] = { 0x11 },
	[S6E3FC3_VRR_60HS] = { 0x01 },
};

static u8 r11s_dimming_speed_table_1[SMOOTH_TRANS_MAX][MAX_S6E3FC3_NORMAL_HBM_TRANSITION][1] = {
	[SMOOTH_TRANS_OFF] = {
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_NORMAL] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_HBM] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_NORMAL] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_HBM] = { 0x20 },
	},
	[SMOOTH_TRANS_ON] = {
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_NORMAL] = { 0x60 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_NORMAL_TO_HBM] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_NORMAL] = { 0x20 },
		[S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_HBM] = { 0x60 },
	},
};

static u8 r11s_irc_mode_table[MAX_IRC_MODE][4] = {
	[IRC_MODE_MODERATO] = { 0x65, 0xFD, 0x03, 0x00 },		/* moderato */
	[IRC_MODE_FLAT_GAMMA] = { 0x25, 0xFD, 0x03, 0x81 },		/* flat gamma */
};

static struct maptbl r11s_maptbl[MAX_MAPTBL] = {
	[GAMMA_MODE2_MAPTBL] = __OLED_MAPTBL_OVERRIDE_INIT_INITIALIZER(r11s_brt_table, &DDI_FUNC(S6E3FC3_MAPTBL_INIT_GAMMA_MODE2_BRT), S6E3FC3_R11S_BR_INDEX_PROPERTY),
	[HBM_ONOFF_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_hbm_transition_table, PANEL_BL_PROPERTY_SMOOTH_TRANSITION, S6E3FC3_NORMAL_HBM_TRANSITION_PROPERTY),
	[ACL_FRAME_AVG_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_acl_frame_avg_table, PANEL_BL_PROPERTY_ACL_PWRSAVE),
	[ACL_START_POINT_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_acl_start_point_table, OLED_BR_HBM_PROPERTY),
	[ACL_DIM_SPEED_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_acl_dim_speed_table, S6E3FC3_ACL_DIM_PROPERTY),
	[ACL_OPR_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_acl_opr_table, S6E3FC3_ACL_OPR_PROPERTY),
	[TSET_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_tset_table, OLED_TSET_PROPERTY),
	[LPM_NIT_MAPTBL] = __OLED_MAPTBL_OVERRIDE_INIT_INITIALIZER(r11s_lpm_nit_table, &DDI_FUNC(S6E3FC3_MAPTBL_INIT_LPM_BRT), S6E3FC3_R11S_LPM_BR_INDEX_PROPERTY),
	[LPM_MODE_MAPTBL] = __OLED_MAPTBL_OVERRIDE_INIT_INITIALIZER(r11s_lpm_mode_table, &DDI_FUNC(S6E3FC3_MAPTBL_INIT_LPM_BRT), S6E3FC3_R11S_LPM_BR_INDEX_PROPERTY),
	[FPS_MAPTBL_1] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_fps_table_1, S6E3FC3_VRR_PROPERTY),
	[FPS_MAPTBL_2] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_fps_table_2, S6E3FC3_VRR_PROPERTY),
	[DIMMING_SPEED] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_dimming_speed_table_1, PANEL_BL_PROPERTY_SMOOTH_TRANSITION, S6E3FC3_NORMAL_HBM_TRANSITION_PROPERTY),
	[SET_FFC_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_ffc_table, S6E3FC3_R11S_HS_CLK_PROPERTY),
	[IRC_MODE_MAPTBL] = __OLED_MAPTBL_DEFAULT_INITIALIZER(r11s_irc_mode_table, PANEL_PROPERTY_IRC_MODE),
};

/* ===================================================================================== */
/* ============================== [S6E3FC3 COMMAND TABLE] ============================== */
/* ===================================================================================== */
static u8 R11S_KEY1_ENABLE[] = { 0x9F, 0xA5, 0xA5 };
static u8 R11S_KEY2_ENABLE[] = { 0xF0, 0x5A, 0x5A };
static u8 R11S_KEY3_ENABLE[] = { 0xFC, 0x5A, 0x5A };

static u8 R11S_KEY1_DISABLE[] = { 0x9F, 0x5A, 0x5A };
static u8 R11S_KEY2_DISABLE[] = { 0xF0, 0xA5, 0xA5 };
static u8 R11S_KEY3_DISABLE[] = { 0xFC, 0xA5, 0xA5 };
static u8 R11S_SLEEP_OUT[] = { 0x11 };
static u8 R11S_SLEEP_IN[] = { 0x10 };
static u8 R11S_DISPLAY_OFF[] = { 0x28 };
static u8 R11S_DISPLAY_ON[] = { 0x29 };

static u8 R11S_MULTI_CMD_ENABLE[] = { 0x72, 0x2C, 0x21 };
static u8 R11S_MULTI_CMD_DISABLE[] = { 0x72, 0x2C, 0x01 };
static u8 R11S_MULTI_CMD_DUMMY[] = { 0x0A, 0x00 };

static u8 R11S_TE_ON[] = { 0x35, 0x00, 0x00 };
static u8 R11S_TE_SETTING[] = { 0xB9, 0x01, 0x09, 0x0F, 0x00, 0x0F };

static DEFINE_STATIC_PACKET(r11s_level1_key_enable, DSI_PKT_TYPE_WR, R11S_KEY1_ENABLE, 0);
static DEFINE_STATIC_PACKET(r11s_level2_key_enable, DSI_PKT_TYPE_WR, R11S_KEY2_ENABLE, 0);
static DEFINE_STATIC_PACKET(r11s_level3_key_enable, DSI_PKT_TYPE_WR, R11S_KEY3_ENABLE, 0);

static DEFINE_STATIC_PACKET(r11s_level1_key_disable, DSI_PKT_TYPE_WR, R11S_KEY1_DISABLE, 0);
static DEFINE_STATIC_PACKET(r11s_level2_key_disable, DSI_PKT_TYPE_WR, R11S_KEY2_DISABLE, 0);
static DEFINE_STATIC_PACKET(r11s_level3_key_disable, DSI_PKT_TYPE_WR, R11S_KEY3_DISABLE, 0);

static DEFINE_STATIC_PACKET(r11s_multi_cmd_enable, DSI_PKT_TYPE_WR, R11S_MULTI_CMD_ENABLE, 0);
static DEFINE_STATIC_PACKET(r11s_multi_cmd_disable, DSI_PKT_TYPE_WR, R11S_MULTI_CMD_DISABLE, 0);
static DEFINE_STATIC_PACKET(r11s_multi_cmd_dummy, DSI_PKT_TYPE_WR, R11S_MULTI_CMD_DUMMY, 0);

static DEFINE_STATIC_PACKET(r11s_sleep_out, DSI_PKT_TYPE_WR, R11S_SLEEP_OUT, 0);
static DEFINE_STATIC_PACKET(r11s_sleep_in, DSI_PKT_TYPE_WR, R11S_SLEEP_IN, 0);
static DEFINE_STATIC_PACKET(r11s_display_on, DSI_PKT_TYPE_WR, R11S_DISPLAY_ON, 0);
static DEFINE_STATIC_PACKET(r11s_display_off, DSI_PKT_TYPE_WR, R11S_DISPLAY_OFF, 0);

static DEFINE_STATIC_PACKET(r11s_te_on, DSI_PKT_TYPE_WR, R11S_TE_ON, 0);
static DEFINE_PKTUI(r11s_te_setting, &r11s_maptbl[FPS_MAPTBL_2], 1);
static DEFINE_VARIABLE_PACKET(r11s_te_setting, DSI_PKT_TYPE_WR, R11S_TE_SETTING, 0);

static u8 R11S_TSET_SET[] = {
	0xB5,
	0x00,
};
static DEFINE_PKTUI(r11s_tset_set, &r11s_maptbl[TSET_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_tset_set, DSI_PKT_TYPE_WR, R11S_TSET_SET, 0x00);

static u8 R11S_AOD_SETTING[] = { 0x91, 0x01 };
static DEFINE_STATIC_PACKET(r11s_aod_setting, DSI_PKT_TYPE_WR, R11S_AOD_SETTING, 0x0);

static u8 R11S_LPM_PORCH_ON[] = { 0xF2, 0x25, 0x60 };
static DEFINE_STATIC_PACKET(r11s_lpm_porch_on, DSI_PKT_TYPE_WR, R11S_LPM_PORCH_ON, 0x10);

static u8 R11S_SWIRE_NO_PULSE[] = { 0xB5, 0x00 };
static DEFINE_STATIC_PACKET(r11s_swire_no_pulse, DSI_PKT_TYPE_WR, R11S_SWIRE_NO_PULSE, 0x0A);

static u8 R11S_SWIRE_NO_PULSE_LPM_OFF[] = { 0xB5, 0x00, 0x00, 0x00 };
static DEFINE_STATIC_PACKET(r11s_swire_no_pulse_lpm_off, DSI_PKT_TYPE_WR, R11S_SWIRE_NO_PULSE_LPM_OFF, 0x07);

static u8 R11S_LPM_OFF_SYNC_CTRL[] = {
	0x63, 0x20
};
static DEFINE_STATIC_PACKET(r11s_lpm_off_sync_ctrl, DSI_PKT_TYPE_WR, R11S_LPM_OFF_SYNC_CTRL, 0x91);

static u8 R11S_LPM_PORCH_OFF[] = { 0xF2, 0x27, 0xE0 };
static DEFINE_STATIC_PACKET(r11s_lpm_porch_off, DSI_PKT_TYPE_WR, R11S_LPM_PORCH_OFF, 0x10);

static u8 R11S_HW_CODE[] = { 0xF2, 0x26, 0xF0 };
static DEFINE_STATIC_PACKET(r11s_hw_code, DSI_PKT_TYPE_WR, R11S_HW_CODE, 0x10);

static u8 R11S_NORMAL_SETTING[] = { 0x91, 0x02 };
static DEFINE_STATIC_PACKET(r11s_normal_setting, DSI_PKT_TYPE_WR, R11S_NORMAL_SETTING, 0);

static u8 R11S_LPM_TE_SETTING[] = { 0xB9, 0x00, 0x09, 0x34, 0x00, 0x0F  };
static DEFINE_STATIC_PACKET(r11s_lpm_te_setting, DSI_PKT_TYPE_WR, R11S_LPM_TE_SETTING, 0);

static u8 R11S_LPM_CTRL[] = { 0x63, 0x01, 0x09, 0x40, 0x02, 0x00, 0x80 };
static DEFINE_STATIC_PACKET(r11s_lpm_ctrl, DSI_PKT_TYPE_WR, R11S_LPM_CTRL, 0x76);

static u8 R11S_LPM_CTRL_OFF[] = { 0x63, 0x00 };
static DEFINE_STATIC_PACKET(r11s_lpm_ctrl_off, DSI_PKT_TYPE_WR, R11S_LPM_CTRL_OFF, 0x76);

static u8 R11S_LPM_FPS[] = { 0x60, 0x08, 0x00 };
static DEFINE_STATIC_PACKET(r11s_lpm_fps, DSI_PKT_TYPE_WR, R11S_LPM_FPS, 0);

static u8 R11S_LPM_DISPLAY_SET[] = { 0xCB, 0x02 };
static DEFINE_STATIC_PACKET(r11s_lpm_display_set, DSI_PKT_TYPE_WR, R11S_LPM_DISPLAY_SET, 0x200);

static u8 R11S_LPM_BLACK_FRAME_ENTER[] = { 0xBB, 0x39 };
static DEFINE_STATIC_PACKET(r11s_lpm_black_frame_enter, DSI_PKT_TYPE_WR, R11S_LPM_BLACK_FRAME_ENTER, 0);

static u8 R11S_LPM_BLACK_FRAME_EXIT[] = { 0xBB, 0x31 };
static DEFINE_STATIC_PACKET(r11s_lpm_black_frame_exit, DSI_PKT_TYPE_WR, R11S_LPM_BLACK_FRAME_EXIT, 0);

static u8 R11S_LPM_AOR_NIT[] = { 0x63, 0x08, 0xF3 };
static DEFINE_PKTUI(r11s_lpm_aor_nit, &r11s_maptbl[LPM_NIT_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_lpm_aor_nit, DSI_PKT_TYPE_WR, R11S_LPM_AOR_NIT, 0x77);

static u8 R11S_LPM_ON[] = { 0x53, 0x24 };
static DEFINE_STATIC_PACKET(r11s_lpm_on, DSI_PKT_TYPE_WR, R11S_LPM_ON, 0);

static u8 R11S_LPM_WRDISBV[] = { 0x51, 0x00, 0xFF };
static DEFINE_PKTUI(r11s_lpm_wrdisbv, &r11s_maptbl[LPM_MODE_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_lpm_wrdisbv, DSI_PKT_TYPE_WR, R11S_LPM_WRDISBV, 0);


#ifdef CONFIG_USDM_FACTORY_DSC_CRC_TEST
static u8 R11S_DECODER_TEST_CASET[] = { 0x2A, 0x00, 0x00, 0x04, 0x37 };
static DEFINE_STATIC_PACKET(r11s_decoder_test_caset, DSI_PKT_TYPE_WR, R11S_DECODER_TEST_CASET, 0x00);

static u8 R11S_DECODER_TEST_PASET[] = { 0x2B, 0x00, 0x00, 0x09, 0x23 };
static DEFINE_STATIC_PACKET(r11s_decoder_test_paset, DSI_PKT_TYPE_WR, R11S_DECODER_TEST_PASET, 0x00);

static u8 R11S_DECODER_TEST_2C[] = { 0x2C, 0x00 };
static DEFINE_STATIC_PACKET(r11s_decoder_test_2c, DSI_PKT_TYPE_WR, R11S_DECODER_TEST_2C, 0x00);

static u8 R11S_DECODER_CRC_PATTERN_ENABLE[] = { 0xBE, 0x07 };
static DEFINE_STATIC_PACKET(r11s_decoder_crc_pattern_enable, DSI_PKT_TYPE_WR, R11S_DECODER_CRC_PATTERN_ENABLE, 0x00);

static u8 R11S_DECODER_READ_SET_1[] = { 0xD8, 0x11 };
static DEFINE_STATIC_PACKET(r11s_decoder_read_set_1, DSI_PKT_TYPE_WR, R11S_DECODER_READ_SET_1, 0x27);

static u8 R11S_DECODER_READ_SET_2[] = { 0xD8, 0x20 };
static DEFINE_STATIC_PACKET(r11s_decoder_read_set_2, DSI_PKT_TYPE_WR, R11S_DECODER_READ_SET_2, 0x27);

static u8 R11S_DECODER_VDDM_LOW_SET_1[] = { 0xD7, 0x04 };
static DEFINE_STATIC_PACKET(r11s_decoder_vddm_low_set_1, DSI_PKT_TYPE_WR, R11S_DECODER_VDDM_LOW_SET_1, 0x02);

static u8 R11S_DECODER_VDDM_LOW_SET_2[] = { 0xF4, 0x07 };
static DEFINE_STATIC_PACKET(r11s_decoder_vddm_low_set_2, DSI_PKT_TYPE_WR, R11S_DECODER_VDDM_LOW_SET_2, 0x0F);

static u8 R11S_DECODER_FUSING_UPDATE_1[] = { 0xFE, 0xB0 };
static DEFINE_STATIC_PACKET(r11s_decoder_fusing_update_1, DSI_PKT_TYPE_WR, R11S_DECODER_FUSING_UPDATE_1, 0x00);

static u8 R11S_DECODER_FUSING_UPDATE_2[] = { 0xFE, 0x30 };
static DEFINE_STATIC_PACKET(r11s_decoder_fusing_update_2, DSI_PKT_TYPE_WR, R11S_DECODER_FUSING_UPDATE_2, 0x0F);

static u8 R11S_DECODER_CRC_PATERN_DISABLE[] = { 0xBE, 0x00 };
static DEFINE_STATIC_PACKET(r11s_decoder_crc_pattern_disable, DSI_PKT_TYPE_WR, R11S_DECODER_CRC_PATERN_DISABLE, 0x00);

static u8 R11S_DECODER_VDDM_RETURN_SET_1[] = { 0xD7, 0x00 };
static DEFINE_STATIC_PACKET(r11s_decoder_vddm_return_set_1, DSI_PKT_TYPE_WR, R11S_DECODER_VDDM_RETURN_SET_1, 0x02);

static u8 R11S_DECODER_VDDM_RETURN_SET_2[] = { 0xF4, 0x00 };
static DEFINE_STATIC_PACKET(r11s_decoder_vddm_return_set_2, DSI_PKT_TYPE_WR, R11S_DECODER_VDDM_RETURN_SET_2, 0x0F);
#endif

#ifdef CONFIG_USDM_PANEL_MASK_LAYER
static DEFINE_PANEL_MDELAY(r11s_wait_2msec, 2);
static DEFINE_PANEL_MDELAY(r11s_wait_3msec, 3);
static DEFINE_PANEL_MDELAY(r11s_wait_7msec, 7);
static DEFINE_PANEL_MDELAY(r11s_wait_9msec, 9);
static DEFINE_PANEL_VSYNC_DELAY(r11s_wait_1_vsync, 1);
#endif

static DEFINE_PANEL_MDELAY(r11s_wait_1msec, 1);
static DEFINE_PANEL_MDELAY(r11s_wait_6msec, 6);
static DEFINE_PANEL_MDELAY(r11s_wait_10msec, 10);
static DEFINE_PANEL_MDELAY(r11s_wait_30msec, 30);
static DEFINE_PANEL_MDELAY(r11s_wait_17msec, 17);
static DEFINE_PANEL_MDELAY(r11s_wait_20msec, 20);

static DEFINE_PANEL_MDELAY(r11s_wait_dsc_test_100msec, 100);
static DEFINE_PANEL_MDELAY(r11s_wait_dsc_test_20msec, 20);

static DEFINE_PANEL_MDELAY(r11s_wait_34msec, 34);

static DEFINE_PANEL_MDELAY(r11s_wait_sleep_out_30msec, 30);
static DEFINE_PANEL_MDELAY(r11s_wait_display_off, 20);

static DEFINE_PANEL_MDELAY(r11s_wait_sleep_in, 120);
static DEFINE_PANEL_UDELAY(r11s_wait_1usec, 1);

static DEFINE_PANEL_FRAME_DELAY(r11s_wait_1_frame, 1);
static DEFINE_PANEL_FRAME_DELAY(r11s_wait_2_frame, 2);

static DEFINE_PANEL_KEY(r11s_level1_key_enable, CMD_LEVEL_1, KEY_ENABLE, &PKTINFO(r11s_level1_key_enable));
static DEFINE_PANEL_KEY(r11s_level2_key_enable, CMD_LEVEL_2, KEY_ENABLE, &PKTINFO(r11s_level2_key_enable));
static DEFINE_PANEL_KEY(r11s_level3_key_enable, CMD_LEVEL_3, KEY_ENABLE, &PKTINFO(r11s_level3_key_enable));

static DEFINE_PANEL_KEY(r11s_level1_key_disable, CMD_LEVEL_1, KEY_DISABLE, &PKTINFO(r11s_level1_key_disable));
static DEFINE_PANEL_KEY(r11s_level2_key_disable, CMD_LEVEL_2, KEY_DISABLE, &PKTINFO(r11s_level2_key_disable));
static DEFINE_PANEL_KEY(r11s_level3_key_disable, CMD_LEVEL_3, KEY_DISABLE, &PKTINFO(r11s_level3_key_disable));

static u8 R11S_HBM_TRANSITION[] = {
	0x53, 0x20
};
static DEFINE_PKTUI(r11s_hbm_transition, &r11s_maptbl[HBM_ONOFF_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_hbm_transition, DSI_PKT_TYPE_WR, R11S_HBM_TRANSITION, 0);

static u8 R11S_ACL_SET[] = {
	0x65,
	0x55, 0x01, 0xB0, 0x56, 0xA0, 0x37, 0x15, 0x55,
	0x55, 0x55, 0x08, 0xF1, 0xC6, 0x48, 0x40, 0x00,
	0x20, 0x10, 0x09
};
static DECLARE_PKTUI(r11s_acl_set) = {
	//{ .offset = 1, .maptbl = &r11s_maptbl[ACL_FRAME_AVG_MAPTBL] },
	{ .offset = 2, .maptbl = &r11s_maptbl[ACL_START_POINT_MAPTBL] },
	{ .offset = 17, .maptbl = &r11s_maptbl[ACL_DIM_SPEED_MAPTBL] },
};
static DEFINE_VARIABLE_PACKET(r11s_acl_set, DSI_PKT_TYPE_WR, R11S_ACL_SET, 0x3B3);

static u8 R11S_ACL[] = {
	0x55, 0x01
};
static DEFINE_PKTUI(r11s_acl_control, &r11s_maptbl[ACL_OPR_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_acl_control, DSI_PKT_TYPE_WR, R11S_ACL, 0);

static u8 R11S_ACL_DIM_OFF[] = {
	0x55,
	0x00
};
static DEFINE_STATIC_PACKET(r11s_acl_dim_off, DSI_PKT_TYPE_WR, R11S_ACL_DIM_OFF, 0);

static u8 R11S_WRDISBV[] = {
	0x51, 0x03, 0xFF
};
static DEFINE_PKTUI(r11s_wrdisbv, &r11s_maptbl[GAMMA_MODE2_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_wrdisbv, DSI_PKT_TYPE_WR, R11S_WRDISBV, 0);

static u8 R11S_IRC_MDOE[] = {
	0x8F,
	0x65, 0xFD, 0x03, 0x00,
};
static DEFINE_PKTUI(r11s_irc_mode, &r11s_maptbl[IRC_MODE_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_irc_mode, DSI_PKT_TYPE_WR, R11S_IRC_MDOE, 0x03);

static u8 R11S_IRC_MDOE_MODERATO[] = {
	0x8F,
	0x65, 0xFD, 0x03, 0x00,
};
static DEFINE_STATIC_PACKET(r11s_irc_mode_moderato, DSI_PKT_TYPE_WR, R11S_IRC_MDOE_MODERATO, 0x03);

static u8 R11S_CASET[] = { 0x2A, 0x00, 0x00, 0x04, 0x37 };
static u8 R11S_PASET[] = { 0x2B, 0x00, 0x00, 0x09, 0x23 };
static DEFINE_STATIC_PACKET(r11s_caset, DSI_PKT_TYPE_WR, R11S_CASET, 0);
static DEFINE_STATIC_PACKET(r11s_paset, DSI_PKT_TYPE_WR, R11S_PASET, 0);

static u8 R11S_PCD_SET_DET_LOW[] = {
	0xCC,
	0x5C, 0x51	/* 1st 0x5C: default high, 2nd 0x51 : Enable SW RESET */
};
static DEFINE_STATIC_PACKET(r11s_pcd_setting, DSI_PKT_TYPE_WR, R11S_PCD_SET_DET_LOW, 0);

static u8 R11S_ERR_FG_ON[] = {
	0xE5, 0x15
};
static DEFINE_STATIC_PACKET(r11s_err_fg_on, DSI_PKT_TYPE_WR, R11S_ERR_FG_ON, 0);

static u8 R11S_ERR_FG_OFF[] = {
	0xE5, 0x05
};
static DEFINE_STATIC_PACKET(r11s_err_fg_off, DSI_PKT_TYPE_WR, R11S_ERR_FG_OFF, 0);

static u8 R11S_ERR_FG_SETTING[] = {
	0xED,
	0x04, 0x4C, 0x20
	/* Vlin1, ELVDD, Vlin3 Monitor On */
	/* Defalut LOW */
};
static DEFINE_STATIC_PACKET(r11s_err_fg_setting, DSI_PKT_TYPE_WR, R11S_ERR_FG_SETTING, 0);

static u8 R11S_VSYNC_SET[] = {
	0xF2,
	0x54
};
static DEFINE_STATIC_PACKET(r11s_vsync_set, DSI_PKT_TYPE_WR, R11S_VSYNC_SET, 0x04);

static u8 R11S_FREQ_SET[] = {
	0xF2,
	0x00
};
static DEFINE_STATIC_PACKET(r11s_freq_set, DSI_PKT_TYPE_WR, R11S_FREQ_SET, 0x27);

static u8 R11S_PORCH_SET[] = {
	0xF2,
	0x55
};
static DEFINE_STATIC_PACKET(r11s_porch_set, DSI_PKT_TYPE_WR, R11S_PORCH_SET, 0x2E);

static u8 R11S_FPS_1[] = { 0x60, 0x08, 0x00 };
static DEFINE_PKTUI(r11s_fps_1, &r11s_maptbl[FPS_MAPTBL_1], 1);
static DEFINE_VARIABLE_PACKET(r11s_fps_1, DSI_PKT_TYPE_WR, R11S_FPS_1, 0);

static u8 R11S_120HZ_FREQ_1[] = { 0x60, 0x08, 0x00 };
static DEFINE_STATIC_PACKET(r11s_120hz_freq_1, DSI_PKT_TYPE_WR, R11S_120HZ_FREQ_1, 0);

static u8 R11S_DIMMING_SPEED[] = { 0x63, 0x60 };
static DEFINE_PKTUI(r11s_dimming_speed, &r11s_maptbl[DIMMING_SPEED], 1);
static DEFINE_VARIABLE_PACKET(r11s_dimming_speed, DSI_PKT_TYPE_WR, R11S_DIMMING_SPEED, 0x91);

static u8 R11S_DIMMING_1_FRAME[] = { 0x63, 0x01 };
static DEFINE_STATIC_PACKET(r11s_dimming_1_frame, DSI_PKT_TYPE_WR, R11S_DIMMING_1_FRAME, 0x92);

static u8 R11S_DIMMING_15_FRAME[] = { 0x63, 0x0F };
static DEFINE_STATIC_PACKET(r11s_dimming_15_frame, DSI_PKT_TYPE_WR, R11S_DIMMING_15_FRAME, 0x92);

static u8 R11S_PANEL_UPDATE[] = {
	0xF7,
	0x0F
};
static DEFINE_STATIC_PACKET(r11s_panel_update, DSI_PKT_TYPE_WR, R11S_PANEL_UPDATE, 0);

static u8 R11S_GLOBAL_PARAM_OFFSET_SETTING[] = {
	0xB0,
	0x28, 0xF2,
};

static DEFINE_STATIC_PACKET(r11s_global_param_offset_setting, DSI_PKT_TYPE_WR, R11S_GLOBAL_PARAM_OFFSET_SETTING, 0);

static u8 R11S_GLOBAL_PARAM_SETTING[] = {
	0xF2,
	0xCE
};
static DEFINE_STATIC_PACKET(r11s_global_param_setting, DSI_PKT_TYPE_WR, R11S_GLOBAL_PARAM_SETTING, 0);

static u8 R11S_FOD_ENTER[] = {
	0xB5, 0x14
};
static DEFINE_STATIC_PACKET(r11s_fod_enter, DSI_PKT_TYPE_WR, R11S_FOD_ENTER, 0);

static u8 R11S_FOD_EXIT[] = {
	0xB5, 0x14
};
static DEFINE_STATIC_PACKET(r11s_fod_exit, DSI_PKT_TYPE_WR, R11S_FOD_EXIT, 0);

static u8 R11S_DSC[] = { 0x01 };
static DEFINE_STATIC_PACKET(r11s_dsc, DSI_PKT_TYPE_WR_COMP, R11S_DSC, 0);

static u8 R11S_PPS[] = {
	//1080x2340 Slice Info : 540x30
	0x11, 0x00, 0x00, 0x89, 0x30, 0x80, 0x09, 0x24,
	0x04, 0x38, 0x00, 0x1E, 0x02, 0x1C, 0x02, 0x1C,
	0x02, 0x00, 0x02, 0x0E, 0x00, 0x20, 0x02, 0xE3,
	0x00, 0x07, 0x00, 0x0C, 0x03, 0x50, 0x03, 0x64,
	0x18, 0x00, 0x10, 0xF0, 0x03, 0x0C, 0x20, 0x00,
	0x06, 0x0B, 0x0B, 0x33, 0x0E, 0x1C, 0x2A, 0x38,
	0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7B,
	0x7D, 0x7E, 0x01, 0x02, 0x01, 0x00, 0x09, 0x40,
	0x09, 0xBE, 0x19, 0xFC, 0x19, 0xFA, 0x19, 0xF8,
	0x1A, 0x38, 0x1A, 0x78, 0x1A, 0xB6, 0x2A, 0xF6,
	0x2B, 0x34, 0x2B, 0x74, 0x3B, 0x74, 0x6B, 0xF4,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static DEFINE_STATIC_PACKET(r11s_pps, DSI_PKT_TYPE_WR_PPS, R11S_PPS, 0);

static u8 R11S_NORMAL_MODE[] = {
	0x53, 0x20

};
static DEFINE_STATIC_PACKET(r11s_normal_mode, DSI_PKT_TYPE_WR, R11S_NORMAL_MODE, 0);

static u8 R11S_FFC_SETTING_1[] = {
	0xC5,
	0x0D, 0x10, 0x80, 0x05
};
static DEFINE_PKTUI(r11s_ffc_setting_1, &r11s_maptbl[SET_FFC_MAPTBL], 1);
static DEFINE_STATIC_PACKET(r11s_ffc_setting_1, DSI_PKT_TYPE_WR, R11S_FFC_SETTING_1, 0x2A);

static u8 R11S_FFC_SETTING_2[] = {
	0xC5,
	0x44, 0x27
};
static DEFINE_PKTUI(r11s_ffc_setting_2, &r11s_maptbl[SET_FFC_MAPTBL], 1);
static DEFINE_VARIABLE_PACKET(r11s_ffc_setting_2, DSI_PKT_TYPE_WR, R11S_FFC_SETTING_2, 0x2E);


static u8 SEQ_S6E3FC3_R11S_120HZ_FREQ_2[] = {
	0xF2, 0x0C
};
static DEFINE_STATIC_PACKET(r11s_120hz_freq_2, DSI_PKT_TYPE_WR, SEQ_S6E3FC3_R11S_120HZ_FREQ_2, 0x8);

static u8 SEQ_S6E3FC3_R11S_120HZ_FREQ_3[] = {
	0x6A,
	0x00, 0x00, 0x00
};
static DEFINE_STATIC_PACKET(r11s_120hz_freq_3, DSI_PKT_TYPE_WR, SEQ_S6E3FC3_R11S_120HZ_FREQ_3, 0x2A);

static u8 SEQ_S6E3FC3_R11S_120HZ_FREQ_4[] = {
	0x68, 0x02
};
static DEFINE_STATIC_PACKET(r11s_120hz_freq_4, DSI_PKT_TYPE_WR, SEQ_S6E3FC3_R11S_120HZ_FREQ_4, 0x28);

static u8 SEQ_S6E3FC3_FD_ENABLE[] = {
	0xB5, 0x40, 0x60
};
static DEFINE_STATIC_PACKET(r11s_fd_enable, DSI_PKT_TYPE_WR, SEQ_S6E3FC3_FD_ENABLE, 0x0A);

#ifdef CONFIG_USDM_FACTORY_CCD_TEST
static u8 R11S_CCD_ENABLE[] = {
	0xCC,
	0x5C, 0x51, 0x01
};

static u8 R11S_CCD_DISABLE[] = {
	0xCC,
	0x5C, 0x51, 0x02
};
static DEFINE_STATIC_PACKET(r11s_ccd_test_enable, DSI_PKT_TYPE_WR, R11S_CCD_ENABLE, 0x00);
static DEFINE_STATIC_PACKET(r11s_ccd_test_disable, DSI_PKT_TYPE_WR, R11S_CCD_DISABLE, 0x00);
#endif

static u8 R11S_AOD_CRC_ENABLE[] = { 0x6A, 0x01 };
static DEFINE_STATIC_PACKET(r11s_aod_crc_enable, DSI_PKT_TYPE_WR, R11S_AOD_CRC_ENABLE, 0x06);

static DEFINE_PANEL_TIMER_MDELAY(r11s_init_complete_delay, 90);
static DEFINE_PANEL_TIMER_BEGIN(r11s_init_complete_delay,
		TIMER_DLYINFO(&r11s_init_complete_delay));

static struct seqinfo SEQINFO(r11s_set_bl_param_seq);
static struct seqinfo SEQINFO(r11s_set_fps_param_seq);
static struct seqinfo SEQINFO(r11s_res_init_seq);

static DEFINE_PNOBJ_CONFIG(r11s_set_separate_tx_off, PANEL_PROPERTY_SEPARATE_TX, SEPARATE_TX_OFF);
static DEFINE_PNOBJ_CONFIG(r11s_set_separate_tx_on, PANEL_PROPERTY_SEPARATE_TX, SEPARATE_TX_ON);

static DEFINE_RULE_BASED_COND(r11s_cond_is_factory_mode,
		PANEL_PROPERTY_IS_FACTORY_MODE, EQ, 1);

static DEFINE_RULE_BASED_COND(r11s_cond_is_hbm_to_normal_transition,
		S6E3FC3_NORMAL_HBM_TRANSITION_PROPERTY, EQ,
		S6E3FC3_NORMAL_HBM_TRANSITION_HBM_TO_NORMAL);

static void *r11s_common_setting_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),

	&PKTINFO(r11s_global_param_offset_setting),
	&PKTINFO(r11s_global_param_setting),
	&PKTINFO(r11s_panel_update),

	&PKTINFO(r11s_te_on),
	&PKTINFO(r11s_te_setting),

	&PKTINFO(r11s_aod_crc_enable),

	&PKTINFO(r11s_caset),
	&PKTINFO(r11s_paset),

	&PKTINFO(r11s_ffc_setting_1),
	&PKTINFO(r11s_ffc_setting_2),

	&PKTINFO(r11s_err_fg_on),
	&PKTINFO(r11s_err_fg_setting),

	&PKTINFO(r11s_vsync_set),
	&PKTINFO(r11s_pcd_setting),

	&PKTINFO(r11s_acl_set),
	&PKTINFO(r11s_panel_update),

	&PKTINFO(r11s_dsc),
	&PKTINFO(r11s_pps),

	&PKTINFO(r11s_freq_set),
	&PKTINFO(r11s_panel_update),

	&PKTINFO(r11s_porch_set),
	&PKTINFO(r11s_dimming_1_frame),
	&PKTINFO(r11s_panel_update),

	&CONDINFO_IF(r11s_cond_is_factory_mode),
		&PKTINFO(r11s_fd_enable),
	&CONDINFO_EL(r11s_cond_is_factory_mode),
		&PKTINFO(r11s_swire_no_pulse),
	&CONDINFO_FI(r11s_cond_is_factory_mode),
	&PKTINFO(r11s_panel_update),

	&SEQINFO(r11s_set_bl_param_seq), /* includes FPS setting also */

	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
};

static DEFINE_SEQINFO(r11s_common_setting_seq,
		r11s_common_setting_cmdtbl);

static void *r11s_init_cmdtbl[] = {
	&DLYINFO(r11s_wait_1msec),

	&PKTINFO(r11s_sleep_out),
	&DLYINFO(r11s_wait_sleep_out_30msec),

	&SEQINFO(r11s_common_setting_seq),

	&TIMER_DLYINFO_BEGIN(r11s_init_complete_delay),
	&CONDINFO_IF(r11s_cond_is_factory_mode),
		&SEQINFO(r11s_res_init_seq),
	&CONDINFO_FI(r11s_cond_is_factory_mode),
	&TIMER_DLYINFO(r11s_init_complete_delay),
};

static void *r11s_res_init_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
	&s6e3fc3_restbl[RES_ID],
	&s6e3fc3_restbl[RES_COORDINATE],
	&s6e3fc3_restbl[RES_CODE],
	&s6e3fc3_restbl[RES_DATE],
	&s6e3fc3_restbl[RES_OCTA_ID],
#ifdef CONFIG_USDM_PANEL_DPUI
	&s6e3fc3_restbl[RES_SELF_DIAG],
	&s6e3fc3_restbl[RES_ERR_FG],
	&s6e3fc3_restbl[RES_DSI_ERR],
#endif
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
};

static void *r11s_pcd_dump_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
	&s6e3fc3_dmptbl[DUMP_PCD],
	&s6e3fc3_dmptbl[DUMP_RDDPM],
	&s6e3fc3_dmptbl[DUMP_RDDSM],
	&s6e3fc3_dmptbl[DUMP_ERR],
	&s6e3fc3_dmptbl[DUMP_ERR_FG],
	&s6e3fc3_dmptbl[DUMP_DSI_ERR],
	&s6e3fc3_dmptbl[DUMP_SELF_DIAG],
	&s6e3fc3_dmptbl[DUMP_PCD2],
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
};

static DEFINE_SEQINFO(r11s_res_init_seq, r11s_res_init_cmdtbl);

/* bl for normal */
static void *r11s_set_bl_param_cmdtbl[] = {
	&PKTINFO(r11s_fps_1),
	&PKTINFO(r11s_panel_update),
	&DLYINFO(r11s_wait_6msec),
	&PKTINFO(r11s_te_setting),
	&PKTINFO(r11s_dimming_speed),
	&PKTINFO(r11s_tset_set),
	&PKTINFO(r11s_panel_update),
	&PKTINFO(r11s_acl_control),
	&PKTINFO(r11s_hbm_transition),
	&PKTINFO(r11s_wrdisbv),
	&PKTINFO(r11s_irc_mode),
	&PKTINFO(r11s_panel_update),
	/*
	 * workaround to turn smooth dimming off
	 * until the first normal brightness is applied.
	 */
	&CONDINFO_IF(r11s_cond_is_hbm_to_normal_transition),
		&DLYINFO(r11s_wait_17msec),
	&CONDINFO_FI(r11s_cond_is_hbm_to_normal_transition),
};

static DEFINE_SEQINFO(r11s_set_bl_param_seq, r11s_set_bl_param_cmdtbl);

static void *r11s_set_bl_cmdtbl[] = {
	&KEYINFO(r11s_level2_key_enable),
	&SEQINFO(r11s_set_bl_param_seq),
	&KEYINFO(r11s_level2_key_disable),
};

static DEFINE_RULE_BASED_COND(r11s_cond_is_panel_state_not_lpm,
		PANEL_PROPERTY_PANEL_STATE, NE, PANEL_STATE_ALPM);
static DEFINE_RULE_BASED_COND(r11s_cond_is_panel_state_lpm,
		PANEL_PROPERTY_PANEL_STATE, EQ, PANEL_STATE_ALPM);
static DEFINE_RULE_BASED_COND(r11s_cond_is_120hz,
		PANEL_PROPERTY_PANEL_REFRESH_RATE, EQ, 120);
static DEFINE_RULE_BASED_COND(r11s_cond_is_60hz,
		PANEL_PROPERTY_PANEL_REFRESH_RATE, EQ, 60);

static void *r11s_set_fps_cmdtbl[] = {
	&CONDINFO_IF(r11s_cond_is_panel_state_not_lpm),
		&KEYINFO(r11s_level2_key_enable),
		&SEQINFO(r11s_set_bl_param_seq),
		&KEYINFO(r11s_level2_key_disable),
	&CONDINFO_FI(r11s_cond_is_panel_state_not_lpm),
};

static void *r11s_display_on_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&PKTINFO(r11s_display_on),
	&KEYINFO(r11s_level1_key_disable),
	&KEYINFO(r11s_level2_key_enable),
	&DLYINFO(r11s_wait_1_frame),
	&PKTINFO(r11s_dimming_15_frame),
	&KEYINFO(r11s_level2_key_disable),
};

static void *r11s_display_off_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&PKTINFO(r11s_display_off),
	&PKTINFO(r11s_err_fg_off),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
	&DLYINFO(r11s_wait_display_off),
};

static void *r11s_exit_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
#ifdef CONFIG_USDM_PANEL_DPUI
	&s6e3fc3_dmptbl[DUMP_RDDPM_SLEEP_IN],
	&s6e3fc3_dmptbl[DUMP_RDDSM],
	&s6e3fc3_dmptbl[DUMP_ERR],
	&s6e3fc3_dmptbl[DUMP_DSI_ERR],
	&s6e3fc3_dmptbl[DUMP_SELF_DIAG],
#endif
	&PKTINFO(r11s_sleep_in),
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
	&DLYINFO(r11s_wait_sleep_in),
};

static void *r11s_alpm_init_cmdtbl[] = {
	&KEYINFO(r11s_level2_key_enable),
	&PKTINFO(r11s_lpm_te_setting),
	&PKTINFO(r11s_lpm_ctrl),
	&PKTINFO(r11s_lpm_fps),
	&PKTINFO(r11s_lpm_display_set),
	&PKTINFO(r11s_panel_update),
	&KEYINFO(r11s_level2_key_disable),
	&DLYINFO(r11s_wait_17msec),
};

static void *r11s_alpm_set_bl_cmdtbl[] = {
	&KEYINFO(r11s_level2_key_enable),
	&PKTINFO(r11s_lpm_black_frame_enter),
	&PKTINFO(r11s_aod_setting),
	&PKTINFO(r11s_lpm_porch_on),
	&PKTINFO(r11s_lpm_on),
	&PKTINFO(r11s_lpm_wrdisbv),
	&PKTINFO(r11s_lpm_aor_nit),
	&PKTINFO(r11s_panel_update),
	&DLYINFO(r11s_wait_1usec),
	&KEYINFO(r11s_level2_key_disable),
	&DLYINFO(r11s_wait_17msec),
};

static void *r11s_alpm_exit_cmdtbl[] = {
	&KEYINFO(r11s_level2_key_enable),
	&PKTINFO(r11s_lpm_black_frame_exit),
	&PKTINFO(r11s_swire_no_pulse_lpm_off),
	&PKTINFO(r11s_lpm_porch_off),
	&PKTINFO(r11s_lpm_off_sync_ctrl),
	&PKTINFO(r11s_normal_mode),
	&DLYINFO(r11s_wait_34msec),
	&PKTINFO(r11s_normal_setting),
	&PKTINFO(r11s_lpm_ctrl_off),
	&PKTINFO(r11s_panel_update),
	&KEYINFO(r11s_level2_key_disable),
	&DLYINFO(r11s_wait_34msec),
};

static void *r11s_alpm_exit_after_cmdtbl[] = {
	&KEYINFO(r11s_level2_key_enable),
	&SEQINFO(r11s_set_bl_param_seq),
	&DLYINFO(r11s_wait_1usec),
	&KEYINFO(r11s_level2_key_disable),
};

static void *r11s_dump_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
	&s6e3fc3_dmptbl[DUMP_RDDPM],
	&s6e3fc3_dmptbl[DUMP_RDDSM],
	&s6e3fc3_dmptbl[DUMP_DSI_ERR],
	&s6e3fc3_dmptbl[DUMP_SELF_DIAG],
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
};

static void *r11s_ffc_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
	&PKTINFO(r11s_ffc_setting_1),
	&PKTINFO(r11s_ffc_setting_2),
	&PKTINFO(r11s_panel_update),
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
};

static void *r11s_check_condition_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
	&s6e3fc3_dmptbl[DUMP_RDDPM],
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
};

#ifdef CONFIG_USDM_PANEL_MASK_LAYER
static void *r11s_mask_layer_workaround_cmdtbl[] = {
	&PKTINFO(r11s_wrdisbv),
	&PKTINFO(r11s_hbm_transition),
	&DLYINFO(r11s_wait_2_frame),
};

static void *r11s_mask_layer_enter_br_cmdtbl[] = {
	&DLYINFO(r11s_wait_1_vsync),
	&DLYINFO(r11s_wait_2msec),
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),

	&PKTINFO(r11s_irc_mode_moderato),
	&PKTINFO(r11s_panel_update),

	&PKTINFO(r11s_acl_dim_off),
	&PKTINFO(r11s_lpm_off_sync_ctrl),
	&PKTINFO(r11s_hbm_transition),
	&PKTINFO(r11s_wrdisbv),

	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),

	&CONDINFO_IF(r11s_cond_is_120hz),
		&DLYINFO(r11s_wait_9msec),
	&CONDINFO_FI(r11s_cond_is_120hz),
};

static void *r11s_mask_layer_exit_br_cmdtbl[] = {
	&DLYINFO(r11s_wait_1_vsync),
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),

	&PKTINFO(r11s_irc_mode),
	&PKTINFO(r11s_panel_update),

	&PKTINFO(r11s_acl_control),
	&CONDINFO_IF(r11s_cond_is_120hz),
		&DLYINFO(r11s_wait_3msec),
	&CONDINFO_FI(r11s_cond_is_120hz),

	&PKTINFO(r11s_lpm_off_sync_ctrl),
	&PKTINFO(r11s_hbm_transition),
	&PKTINFO(r11s_wrdisbv),

	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),

	&CONDINFO_IF(r11s_cond_is_120hz),
		&DLYINFO(r11s_wait_9msec),
	&CONDINFO_FI(r11s_cond_is_120hz),
};
#endif

#ifdef CONFIG_USDM_FACTORY_DSC_CRC_TEST
static void *r11s_decoder_test_cmdtbl[] = {
	&KEYINFO(r11s_level1_key_enable),
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
	&PKTINFO(r11s_decoder_test_caset),
	&PKTINFO(r11s_decoder_test_paset),
	&PKTINFO(r11s_decoder_test_2c),
	&PKTINFO(r11s_decoder_crc_pattern_enable),
	&PKTINFO(r11s_decoder_read_set_1),
	&DLYINFO(r11s_wait_dsc_test_100msec),
	&s6e3fc3_restbl[RES_DECODER_TEST1],
	&PKTINFO(r11s_decoder_read_set_2),
	&DLYINFO(r11s_wait_dsc_test_100msec),
	&s6e3fc3_restbl[RES_DECODER_TEST2],
	&PKTINFO(r11s_decoder_vddm_low_set_1),
	&PKTINFO(r11s_decoder_vddm_low_set_2),
	&PKTINFO(r11s_decoder_fusing_update_1),
	&PKTINFO(r11s_decoder_fusing_update_2),
	&PKTINFO(r11s_decoder_crc_pattern_enable),
	&PKTINFO(r11s_decoder_read_set_1),
	&DLYINFO(r11s_wait_dsc_test_20msec),
	&s6e3fc3_restbl[RES_DECODER_TEST3],
	&PKTINFO(r11s_decoder_read_set_2),
	&DLYINFO(r11s_wait_dsc_test_20msec),
	&s6e3fc3_restbl[RES_DECODER_TEST4],
	&PKTINFO(r11s_decoder_crc_pattern_disable),
	&PKTINFO(r11s_decoder_vddm_return_set_1),
	&PKTINFO(r11s_decoder_vddm_return_set_2),
	&PKTINFO(r11s_decoder_fusing_update_1),
	&PKTINFO(r11s_decoder_fusing_update_2),
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
	&KEYINFO(r11s_level1_key_disable),
};
#endif

#ifdef CONFIG_USDM_FACTORY_CCD_TEST
static void *r11s_ccd_test_cmdtbl[] = {
	&KEYINFO(r11s_level2_key_enable),
	&KEYINFO(r11s_level3_key_enable),
	&PKTINFO(r11s_ccd_test_enable),
	&DLYINFO(r11s_wait_20msec),
	&s6e3fc3_restbl[RES_CCD_STATE],
	&PKTINFO(r11s_ccd_test_disable),
	&KEYINFO(r11s_level3_key_disable),
	&KEYINFO(r11s_level2_key_disable),
};
#endif

static void *r11s_dummy_cmdtbl[] = {
	NULL,
};

static struct seqinfo r11s_seqtbl[] = {
	SEQINFO_INIT(PANEL_INIT_SEQ, r11s_init_cmdtbl),
	SEQINFO_INIT(PANEL_RES_INIT_SEQ, r11s_res_init_cmdtbl),
	SEQINFO_INIT(PANEL_SET_BL_SEQ, r11s_set_bl_cmdtbl),
	SEQINFO_INIT(PANEL_DISPLAY_ON_SEQ, r11s_display_on_cmdtbl),
	SEQINFO_INIT(PANEL_DISPLAY_OFF_SEQ, r11s_display_off_cmdtbl),
	SEQINFO_INIT(PANEL_EXIT_SEQ, r11s_exit_cmdtbl),
	SEQINFO_INIT(PANEL_DISPLAY_MODE_SEQ, r11s_set_fps_cmdtbl),
	SEQINFO_INIT(PANEL_ALPM_INIT_SEQ, r11s_alpm_init_cmdtbl),
	SEQINFO_INIT(PANEL_ALPM_SET_BL_SEQ, r11s_alpm_set_bl_cmdtbl),
	SEQINFO_INIT(PANEL_ALPM_EXIT_SEQ, r11s_alpm_exit_cmdtbl),
	SEQINFO_INIT(PANEL_ALPM_EXIT_AFTER_SEQ, r11s_alpm_exit_after_cmdtbl),
	SEQINFO_INIT(PANEL_FFC_SEQ, r11s_ffc_cmdtbl),
	SEQINFO_INIT(PANEL_DUMP_SEQ, r11s_dump_cmdtbl),
	SEQINFO_INIT(PANEL_CHECK_CONDITION_SEQ, r11s_check_condition_cmdtbl),
#ifdef CONFIG_USDM_PANEL_MASK_LAYER
	SEQINFO_INIT(PANEL_MASK_LAYER_STOP_DIMMING_SEQ, r11s_mask_layer_workaround_cmdtbl),
	SEQINFO_INIT(PANEL_MASK_LAYER_ENTER_BR_SEQ, r11s_mask_layer_enter_br_cmdtbl),
	SEQINFO_INIT(PANEL_MASK_LAYER_EXIT_BR_SEQ, r11s_mask_layer_exit_br_cmdtbl),
#endif
#ifdef CONFIG_USDM_FACTORY_DSC_CRC_TEST
	SEQINFO_INIT(PANEL_DECODER_TEST_SEQ, r11s_decoder_test_cmdtbl),
#endif
#ifdef CONFIG_USDM_FACTORY_CCD_TEST
	SEQINFO_INIT(PANEL_CCD_TEST_SEQ, r11s_ccd_test_cmdtbl),
#endif
	SEQINFO_INIT(PANEL_PCD_DUMP_SEQ, r11s_pcd_dump_cmdtbl),
	SEQINFO_INIT(PANEL_DUMMY_SEQ, r11s_dummy_cmdtbl),
};

struct common_panel_info s6e3fc3_r11s_panel_info = {
	.ldi_name = "s6e3fc3",
	.name = "s6e3fc3_r11s",
	.model = "AMS646YB01",
	.vendor = "SDC",
	.id = 0x800042,
	.rev = 0,
	.ddi_props = {
		.gpara = (DDI_SUPPORT_WRITE_GPARA | DDI_SUPPORT_READ_GPARA | DDI_SUPPORT_2BYTE_GPARA | DDI_SUPPORT_POINT_GPARA),
		.err_fg_recovery = false,
		.support_vrr = true,
		.dft_osc_freq = 181300,
		/* Todo the hs_clk must be synchronized with the value of LK,
		 * It must be changed to be read and set in the LK.
		 */
		.dft_dsi_freq = 1362000,
	},
	.ddi_ops = {
		.get_cell_id = s6e3fc3_get_cell_id,
		.get_octa_id = s6e3fc3_get_octa_id,
		.get_manufacture_code = s6e3fc3_get_manufacture_code,
		.get_manufacture_date = s6e3fc3_get_manufacture_date,
#ifdef CONFIG_USDM_FACTORY_DSC_CRC_TEST
		.decoder_test = s6e3fc3_r11s_decoder_test,
#endif
	},
#if defined(CONFIG_USDM_PANEL_DISPLAY_MODE)
	.common_panel_modes = &s6e3fc3_r11s_display_modes,
#endif
	.mres = {
		.nr_resol = ARRAY_SIZE(s6e3fc3_r11s_default_resol),
		.resol = s6e3fc3_r11s_default_resol,
	},
	.vrrtbl = s6e3fc3_r11s_default_vrrtbl,
	.nr_vrrtbl = ARRAY_SIZE(s6e3fc3_r11s_default_vrrtbl),
	.maptbl = r11s_maptbl,
	.nr_maptbl = ARRAY_SIZE(r11s_maptbl),
	.seqtbl = r11s_seqtbl,
	.nr_seqtbl = ARRAY_SIZE(r11s_seqtbl),
	.rditbl = s6e3fc3_rditbl,
	.nr_rditbl = ARRAY_SIZE(s6e3fc3_rditbl),
	.restbl = s6e3fc3_restbl,
	.nr_restbl = ARRAY_SIZE(s6e3fc3_restbl),
	.dumpinfo = s6e3fc3_dmptbl,
	.nr_dumpinfo = ARRAY_SIZE(s6e3fc3_dmptbl),

#ifdef CONFIG_USDM_MDNIE
	.mdnie_tune = &s6e3fc3_r11s_mdnie_tune,
#endif
	.panel_dim_info = {
		[PANEL_BL_SUBDEV_TYPE_DISP] = &s6e3fc3_r11s_panel_dimming_info,
#ifdef CONFIG_USDM_PANEL_AOD_BL
		[PANEL_BL_SUBDEV_TYPE_AOD] = &s6e3fc3_r11s_panel_aod_dimming_info,
#endif
	},
#ifdef CONFIG_USDM_PANEL_SELF_DISPLAY
	.aod_tune = &s6e3fc3_r11s_aod,
#endif
};

#endif /* __S6E3FC3_R11S_PANEL_H__ */
