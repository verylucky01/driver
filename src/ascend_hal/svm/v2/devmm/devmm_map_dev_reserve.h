/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEVMM_MAP_DEV_RESERVE_H
#define DEVMM_MAP_DEV_RESERVE_H

#include <pthread.h>
#include <stdint.h>
#include "svm_ioctl.h"
#include "ascend_hal.h"

struct devmm_map_dev_reserve {
    uint64_t mapped_dev_reserve_addr[DEVMM_MAX_PHY_DEVICE_NUM][ADDR_MAP_TYPE_MAX];
    uint64_t mapped_dev_reserve_size[DEVMM_MAX_PHY_DEVICE_NUM][ADDR_MAP_TYPE_MAX];
    pthread_mutex_t map_dev_lock[DEVMM_MAX_PHY_DEVICE_NUM];
    uint32_t inited;
};

void devmm_init_dev_reserve_addr(uint32_t devid);
void devmm_reset_dev_reserve_addr(uint32_t devid);
void devmm_get_dev_reserve_addr(uint32_t devid, uint32_t addr_type, uint64_t *addr, uint64_t *size);
DVresult devmm_ctrl_map_addr(void *param_value, size_t param_value_size, void *out_value, size_t *out_size_ret);
DVresult devmm_ctrl_unmap_addr(void *param_value, size_t param_value_size, void *out_value, size_t *out_size_ret);
DVresult devmm_ctrl_map_mem_restore(void);

#endif /* DEVMM_MAP_DEV_RESERVE_H */

