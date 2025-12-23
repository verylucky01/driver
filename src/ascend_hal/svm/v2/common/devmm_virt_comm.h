/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_COMM_H
#define SVM_COMM_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "devmm_virt_list.h"

typedef unsigned long virt_addr_t;

#define DEVMM_SVM_PG_BUDDY 0x01
#define DEVMM_SVM_PG_SLAB 0x02
#define DEVMM_SVM_PG_CACHE 0x04
#define DEVMM_SVM_PG_FIRST 0x08
#define DEVMM_SVM_PG_MEMTYPE_DDR 0x10
#define DEVMM_SVM_PG_MEMTYPE_HBM 0x20
#define DEVMM_SVM_PG_MEMTYPE_P2P_DDR 0x40
#define DEVMM_SVM_PG_MEMTYPE_P2P_HBM 0x80
#define DEVMM_SVM_PG_MEMTYPE_TS_DDR 0x100
#define DEVMM_SVM_PG_MEMTYPE_HOST_DDR 0x200

typedef pthread_mutex_t devmm_virt_lock_t;
#define devmm_virt_lock_init(lock) pthread_mutex_init(lock, NULL)
#define devmm_virt_try_lock pthread_mutex_trylock
#define devmm_virt_lock pthread_mutex_lock
#define devmm_virt_unlock pthread_mutex_unlock

#define IS_ERR_OR_NULL(ptr) ((ptr) == NULL)
#endif /* _SVM_COMM_H_ */
