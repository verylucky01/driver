/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MMS_DEF_H
#define MMS_DEF_H

#include "svm_pub.h"

enum { MMS_TYPE_NPAGE = 0U, MMS_TYPE_HPAGE, MMS_TYPE_P2P_NPAGE, MMS_TYPE_P2P_HPAGE, MMS_TYPE_MAX };

static inline const char *get_mms_type_name(u32 type)
{
    const char *type_names[] = {
        [MMS_TYPE_NPAGE] = "MMS_TYPE_NPAGE",
        [MMS_TYPE_HPAGE] = "MMS_TYPE_HPAGE",
        [MMS_TYPE_P2P_NPAGE] = "MMS_TYPE_P2P_NPAGE",
        [MMS_TYPE_P2P_HPAGE] = "MMS_TYPE_P2P_HPAGE"
    };

    if (type >= MMS_TYPE_MAX) {
        return "UNKNOWN";
    }

    return type_names[type];
}

#define MMS_MODULE_ID_MAX MAX_MODULE_ID

struct mms_type_stats {
    u64 alloced_size;
    u64 alloced_peak_size;
    u64 alloc_cnt;
    u64 free_cnt;
};

struct mms_module_stats {
    struct mms_type_stats type_stats[MMS_TYPE_MAX];
};

struct mms_stats {
    struct mms_module_stats module_stats[MMS_MODULE_ID_MAX];
};

#endif
