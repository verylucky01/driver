/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_SHARE_TYPE_H
#define SVM_SHARE_TYPE_H

enum svm_share_type {
    SVM_SHARE_TYPE_VMM, /* vmm rsv va */
    SVM_SHARE_TYPE_VMM_ACCESS, /* vmm mapped va */
    SVM_SHARE_TYPE_IPC_OPENED, /* ipc opened va */
    SVM_SHARE_TYPE_PREFETCH,
    SVM_SHARE_TYPE_REGISTER,
    SVM_SHARE_TYPE_MAX
};

struct svm_share_priv_head {
    enum svm_share_type type;
};

#endif

