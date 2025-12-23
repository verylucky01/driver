/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TRS_RES_H__
#define TRS_RES_H__

#include "ascend_hal.h"
#include "ascend_hal_define.h"

#include "trs_ioctl.h"
#include "trs_user_msg.h"

struct res_id_usr_info {
    uint32_t valid;
    uint32_t res_len;
    uint64_t res_addr;
};

int trs_dev_res_id_init(uint32_t dev_id);
void trs_dev_res_id_uninit(uint32_t dev_id);
struct res_id_usr_info *trs_get_res_id_info(uint32_t dev_id, uint32_t ts_id, drvIdType_t type, uint32_t id);
uint32_t trs_get_res_id_num(uint32_t dev_id, uint32_t ts_id, drvIdType_t type);
int trs_id_query(uint32_t dev_id, uint32_t cmd, struct trs_res_id_para *para, uint32_t *value);

drvError_t _halResourceIdAlloc(uint32_t dev_id, struct halResourceIdInputInfo *in, struct halResourceIdOutputInfo *out);
drvError_t _halResourceIdFree(uint32_t dev_id, struct halResourceIdInputInfo *in);
drvError_t _halResourceConfig(uint32_t dev_id, struct halResourceIdInputInfo *in, struct halResourceConfigInfo *para);
drvError_t trs_res_config_para_check(uint32_t dev_id, struct halResourceIdInputInfo *in,
    struct halResourceConfigInfo *para);
drvError_t trs_local_res_config(uint32_t dev_id, struct halResourceIdInputInfo *in,
    struct halResourceConfigInfo *para);
drvError_t trs_stream_task_fill(uint32_t dev_id, uint32_t stream_id, void *task_addr, void *task_info, uint32_t size);

struct trs_res_remote_ops {
    drvError_t (*resid_alloc)(uint32_t dev_id, struct halResourceIdInputInfo *in, struct halResourceIdOutputInfo *out);
    drvError_t (*resid_free)(uint32_t dev_id, struct halResourceIdInputInfo *in);
    drvError_t (*resid_config)(uint32_t dev_id, struct halResourceIdInputInfo *in, struct halResourceConfigInfo *para);
};
drvError_t trs_remote_res_config(uint32_t dev_id, struct halResourceIdInputInfo *in,
    struct halResourceConfigInfo *para);

void trs_register_res_remote_ops(struct trs_res_remote_ops *ops);

#endif

