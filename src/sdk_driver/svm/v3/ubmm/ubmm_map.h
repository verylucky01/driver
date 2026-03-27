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

#ifndef UBMM_MAP_H
#define UBMM_MAP_H

#include "svm_pub.h"
#include "svm_addr_desc.h"

int ubmm_init_map(u32 udevid);
void ubmm_uninit_map(u32 udevid);

int ubmm_map(u32 udevid, u64 uba, u64 size, struct svm_pa_seg *pa_seg, u64 seg_num);
int ubmm_unmap(u32 udevid, u64 uba, u64 size);

#endif
