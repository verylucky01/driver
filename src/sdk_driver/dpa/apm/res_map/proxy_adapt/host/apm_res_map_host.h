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
#ifndef APM_RES_MAP_HOST_H
#define APM_RES_MAP_HOST_H

int apm_res_map_host_res_map(struct apm_res_map_info *para);
int apm_res_map_host_res_unmap(struct apm_res_map_info *para);
int apm_res_map_host_init(void);
void apm_res_map_host_uninit(void);

#endif
