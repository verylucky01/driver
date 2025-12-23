/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef DMS_VDEV_H
#define DMS_VDEV_H

#include "drv_type.h"
#ifdef CFG_FEATURE_VDEV_MEM
int dms_drv_get_vdevice_info(void *feature, char *in, u32 in_len, char *out, u32 out_len);
#endif

#if defined(CFG_HOST_ENV) && defined(CFG_FEATURE_SRIOV)
int dms_feature_set_sriov_switch(void *feature, char *in, u32 in_len, char *out, u32 out_len);
#endif
#endif
