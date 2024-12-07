/*
 *  Copyright (C) 2020, Samsung Electronics Co. Ltd. All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#ifndef __SHUB_LID_ANGLE_FUSION_H__
#define __SHUB_LID_ANGLE_FUSION_H__

#include <linux/types.h>

struct lid_angle_fusion {
	int32_t fusion_mode;
	int32_t angle;
	uint8_t state;
	uint8_t error_code;
	float dtime;
	float acc_main[3];
	float gyro_main[3];
	float acc_sub[3];
	float gyro_sub[3];
} __attribute__((__packed__));


#endif /* __SHUB_LID_ANGLE_FUSION_H__ */