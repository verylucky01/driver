/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef TRS_SHR_ID_USER_H
#define TRS_SHR_ID_USER_H

#include "ascend_hal_define.h"

struct trs_shrid_remote_ops {
    int (*shrid_config)(uint32_t dev_id, struct drvShrIdInfo *info);
    int (*shrid_deconfig)(uint32_t dev_id, struct drvShrIdInfo *info);
};

void trs_register_shrid_remote_ops(struct trs_shrid_remote_ops *ops);

#endif

