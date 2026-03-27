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
#ifndef APM_RES_MAP_OPS_H
#define APM_RES_MAP_OPS_H

#include "dpa/dpa_apm_kernel.h"

bool apm_res_is_belong_to_proc(int master_tgid, int slave_tgid, u32 udevid, struct res_map_info_in *res_info);
int apm_get_res_addr(u32 udevid, struct res_map_info_in *res_info, u64 pa[], u32 num, u32 *len);
void apm_put_res_addr(u32 udevid, struct res_map_info_in *res_info, u64 pa[], u32 len);
int apm_update_res_info(u32 udevid, struct res_map_info_in *res_info);
#endif
