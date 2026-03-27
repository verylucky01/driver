/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_REGISTER_TO_MASTER_H
#define SVM_REGISTER_TO_MASTER_H

#include <stdint.h>
#include <stdbool.h>

#include "svm_pub.h"
#include "svm_addr_desc.h"

/*
    register address for later op(access/memcpy)
    case 1: register device apm reg addr to master, register_va is device va register_va.devid == user_devid
    case 2: register master malloc addr to master for op with user device, user_devid is the op device
    case 3: register svm host addr to master for op with user device, user_devid is the op device
    case 4: register svm device addr to master for op with host, user_devid is the host devid

    connect mode:
    ub:
         1. device addr: device register urma segment host import segment
         2. host addr: host register urma segment
    pcie:
         1. device svm addr: if support sva, do nothing; else device dma map or query pa(smmu disable)
         2. host svm addr: host dma map(no need pin because mpl already pined the page)
         3. host malloc addr: pined user page and dma map
*/

#define REGISTER_TO_MASTER_FLAG_APM_MAPED_REG     (1U << 0U)
#define REGISTER_TO_MASTER_FLAG_ACCESS_WRITE      (1U << 1U)
#define REGISTER_TO_MASTER_FLAG_PIN               (1U << 2U)
#define REGISTER_TO_MASTER_FLAG_VA_IO_MAP         (1U << 3U)

static inline bool register_to_master_flag_bit_is_set(u32 flag, u32 bit_mask)
{
    return ((flag & bit_mask) != 0);
}

static inline bool register_to_master_flag_is_apm_maped_reg(u32 flag)
{
    return register_to_master_flag_bit_is_set(flag, REGISTER_TO_MASTER_FLAG_APM_MAPED_REG);
}

static inline bool register_to_master_flag_is_access_write(u32 flag)
{
    return register_to_master_flag_bit_is_set(flag, REGISTER_TO_MASTER_FLAG_ACCESS_WRITE);
}

static inline bool register_to_master_flag_is_pin(u32 flag)
{
    return register_to_master_flag_bit_is_set(flag, REGISTER_TO_MASTER_FLAG_PIN);
}

static inline bool register_to_master_flag_is_va_io_map(u32 flag)
{
    return register_to_master_flag_bit_is_set(flag, REGISTER_TO_MASTER_FLAG_VA_IO_MAP);
}

int svm_register_to_master(u32 user_devid, struct svm_dst_va *register_va, u32 flag);
int svm_unregister_to_master(u32 user_devid, struct svm_dst_va *register_va, u32 flag);

/* register ops */
struct svm_register_to_master_ops {
    int (*register_to_master)(u32 user_devid, struct svm_dst_va *register_va, u32 flag);
    int (*unregister_to_master)(u32 user_devid, struct svm_dst_va *register_va, u32 flag);
};

/* devid is real agent device devid, register h->d devid id access devid, d->h devid id register va devid */
void svm_register_to_master_set_ops(u32 user_devid, u32 devid, struct svm_register_to_master_ops *ops);

#endif
