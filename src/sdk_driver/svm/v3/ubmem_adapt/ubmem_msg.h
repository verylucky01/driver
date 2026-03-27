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

#ifndef UBMEM_MSG_H
#define UBMEM_MSG_H

#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "kmc_msg.h"

struct ubmem_map_msg {
    struct svm_kmc_msg_head head;
    struct svm_global_va src_va;
    u64 maped_va; /* output */
    u64 rsv;  /* reserve */
};

struct ubmem_unmap_msg {
    struct svm_kmc_msg_head head;
    struct svm_global_va src_va;
    u64 rsv;  /* reserve */
};

#endif

