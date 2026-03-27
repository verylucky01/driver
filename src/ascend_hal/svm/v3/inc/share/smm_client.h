/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SMM_CLIENT_H
#define SMM_CLIENT_H

#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "smm.h"

/*
   share memory map client interface: support cross app share mem and in single app share mem
   cross app case: src_info->va is 0, we will update src info by dst info in kernel,
              the src info is stored in svm_casm_mem_map process by parse key.
   in single app case: src_info->va is fill in user, we will pin/unpin src mem in kernel (how to safe unpin?)
*/

int svm_smm_client_map(struct svm_dst_va *dst_info, struct svm_global_va *src_info, u32 flag);
int svm_smm_client_unmap(struct svm_dst_va *dst_info, struct svm_global_va *src_info, u32 flag);

#endif

