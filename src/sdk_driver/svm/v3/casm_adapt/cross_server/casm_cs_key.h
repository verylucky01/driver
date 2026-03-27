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

#ifndef CASM_CS_KEY_H
#define CASM_CS_KEY_H

#include "svm_pub.h"
#include "svm_addr_desc.h"

bool casm_cs_is_local_key(u64 key);
u32 casm_cs_parse_server_id_from_key(u64 key);

void casm_cs_set_local_server_id(u32 server_id);
int casm_cs_key_init_dev(u32 udevid);
int casm_cs_key_init(void);

#endif

