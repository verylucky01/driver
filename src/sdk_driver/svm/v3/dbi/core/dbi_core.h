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
 
#ifndef DBI_CORE_H
#define DBI_CORE_H

int dbi_init_dev(u32 udevid);
void dbi_uninit_dev(u32 udevid);
int dbi_feature_init(void);
void dbi_show_dev(u32 udevid, int feature_id, ka_seq_file_t *seq);

#endif
