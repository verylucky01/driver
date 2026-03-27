/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DBI_DEF_H
#define DBI_DEF_H

#include "pair_dev_info.h"

#include "svm_pub.h"

#define SVM_DEV_CAP_SVA                 (0x1 << 0)
#define SVM_DEV_CAP_UBMEM               (0x1 << 1)
#define SVM_DEV_CAP_ASSIGN_GAP          (0x1 << 2)

typedef struct devdrv_pair_info_eid dbi_bus_inst_eid_t;

#define DBI_EID_FMT "%2.2x%2.2x:%2.2x%2.2x:%2.2x%2.2x:%2.2x%2.2x:%2.2x%2.2x:%2.2x%2.2x:%2.2x%2.2x:%2.2x%2.2x"
#define DBI_EID_RAW_ARGS(eid) eid[0], eid[1], eid[2], eid[3], eid[4], eid[5], eid[6], eid[7], eid[8], eid[9], eid[10], \
        eid[11], eid[12], eid[13], eid[14], eid[15]
#define DBI_EID_ARGS(eid) DBI_EID_RAW_ARGS((eid).raw)

struct svm_device_basic_info {
    u64 npage_size;
    u64 hpage_size;
    u64 gpage_size;
    u32 cap_flag;
    u32 bus_inst_eid_flag;
    dbi_bus_inst_eid_t bus_inst_eid;
    u64 d2h_acc_mask;
    u64 rsv; /* reserve */
};

#endif
