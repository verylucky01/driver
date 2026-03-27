/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SMM_MSG_H
#define SMM_MSG_H

#include "svm_pub.h"
#include "svm_addr_desc.h"

struct svm_smm_map_msg {
    u64 dst_va;
    u64 dst_size;
    u32 dst_task_type;
    u32 flag;
    struct svm_global_va src_info;
    u64 rsv;  /* reserve */
};

struct svm_smm_unmap_msg {
    u64 dst_va;
    u64 dst_size;
    u32 dst_task_type;
    u32 flag;
    struct svm_global_va src_info;
    u64 rsv;  /* reserve */
};

#endif

