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

#ifndef CASM_CS_DST_H
#define CASM_CS_DST_H

#include "svm_pub.h"
#include "svm_addr_desc.h"

int casm_cs_dst_init_task(u32 udevid, int tgid, void *start_time);
void casm_cs_dst_uninit_task(u32 udevid, int tgid, void *start_time);
void casm_cs_dst_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int casm_cs_dst_init(void);
void svm_casm_cs_dst_uninit(void);

#endif

