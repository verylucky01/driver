/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MPL_FLAG_H
#define MPL_FLAG_H

#define SVM_MPL_FLAG_HPAGE             (1U << 0U)
#define SVM_MPL_FLAG_CONTIGUOUS        (1U << 1U)
#define SVM_MPL_FLAG_P2P               (1U << 2U)
#define SVM_MPL_FLAG_PG_NC             (1U << 3U)
#define SVM_MPL_FLAG_PG_RDONLY         (1U << 4U)
#define SVM_MPL_FLAG_GPAGE             (1U << 5U)
#define SVM_MPL_FLAG_DEV_CP_ONLY       (1U << 6U)
#define SVM_MPL_FLAG_FIXED_NUMA        (1U << 7U)

/* numa id: bit24~31 */
#define SVM_MPL_FLAG_NUMA_ID_BIT       24U
#define SVM_MPL_FLAG_NUMA_ID_WIDTH     8U
#define SVM_MPL_FLAG_NUMA_ID_MASK      ((1U << SVM_MPL_FLAG_NUMA_ID_WIDTH) - 1)

static inline void mpl_flag_set_numa_id(u32 *flag, u32 numa_id)
{
    *flag |= ((numa_id & SVM_MPL_FLAG_NUMA_ID_MASK) << SVM_MPL_FLAG_NUMA_ID_BIT);
}

static inline u32 mpl_flag_get_numa_id(u32 flag)
{
    return ((flag >> SVM_MPL_FLAG_NUMA_ID_BIT) & SVM_MPL_FLAG_NUMA_ID_MASK);
}

#endif
