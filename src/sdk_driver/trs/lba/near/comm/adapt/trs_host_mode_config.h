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
#ifndef TRS_HOST_MODE_CONFIG_H
#define TRS_HOST_MODE_CONFIG_H

int trs_get_sq_send_mode(u32 devid);
int trs_mode_config_to_sia(u32 chip_id);
int trs_mode_config_to_mia(u32 chip_id);

int trs_mode_config_by_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int trs_get_sq_send_mode(u32 udevid);
int trs_mode_query_by_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int trs_host_mode_config_init(void);
void trs_host_mode_config_uninit(void);
#endif /* TRS_HOST_MODE_CONFIG_H */

