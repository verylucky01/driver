/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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

#ifndef DEVDRV_SHM_INFO_H
#define DEVDRV_SHM_INFO_H

#include "devdrv_common.h"

int devmng_shm_init(struct devdrv_info *dev_info);
void devmng_shm_uninit(struct devdrv_info *dev_info);
#ifdef CFG_FEATURE_SHM_DEVMNG
int devdrv_manager_get_pf_vf_id(u32 udevid, u32 *pf_id, u32 *vf_id);
#endif

#endif /* DEVDRV_SHM_INFO_H */
