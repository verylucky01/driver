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

#ifndef CASM_KEY_H
#define CASM_KEY_H

#include <linux/types.h>

#include "svm_addr_desc.h"
#include "casm_kernel.h"

bool casm_is_local_key(u64 key);
int casm_destroy_key(u64 key);

int casm_key_init_dev(u32 udevid);
void casm_key_uninit_dev(u32 udevid);
void casm_key_show_dev(u32 udevid, int feature_id, ka_seq_file_t *seq);
int casm_key_init(void);

#endif

