/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ascend_hal.h"
#include "ascend_hal_external.h"

#include "trs_user_pub_def.h"
#include "trs_sqcq.h"
#include "trs_user_interface.h"

drvError_t trs_sq_svm_mem_alloc(uint32_t dev_id, void **va, uint64_t size)
{
    void *sq_que_va = NULL;
    unsigned long long mem_flag = 0;
    int ret;

    ret = halMemAgentOpen(dev_id, SVM_AGENT_DEVICE);
    if (ret != DRV_ERROR_NONE) {
        (void)halMemFree(sq_que_va);
        trs_err("Register sq mem fail. (devid=%u; size=%llu; ret=%d)\n", dev_id, size, ret);
        return ret;
    }

    if (trs_get_sq_send_mode(dev_id) == TRS_MODE_TYPE_SQ_SEND_HIGH_PERFORMANCE) {
        mem_flag = (MEM_DEV | MEM_ADVISE_P2P | dev_id);
    } else {
#ifndef EMU_ST
        mem_flag = (MEM_DEV | MEM_ADVISE_P2P | MEM_DEV_CP_ONLY | dev_id);
#endif
    }

    ret = halMemAlloc(&sq_que_va, size, mem_flag | MEM_CONTIGUOUS_PHY);
    if (ret != DRV_ERROR_NONE) {
        ret = halMemAlloc(&sq_que_va, size, mem_flag);
        if (ret != DRV_ERROR_NONE) {
            trs_err("Alloc sq mem fail. (devid=%u; size=%llu; ret=%d)\n", dev_id, size, ret);
            return ret;
        }
    }

    if (trs_get_sq_send_mode(dev_id) == TRS_MODE_TYPE_SQ_SEND_HIGH_PERFORMANCE) { /* only uio mode need host register va */
        ret = halHostRegister(sq_que_va, size, DEV_SVM_MAP_HOST, dev_id, va);
        if (ret != DRV_ERROR_NONE) {
            (void)halMemFree(sq_que_va);
            trs_err("Register sq mem fail. (devid=%u; size=%llu; ret=%d)\n", dev_id, size, ret);
            return ret;
        }

        if (sq_que_va != *va) {
            (void)halHostUnregisterEx(*va, dev_id, DEV_SVM_MAP_HOST);
            (void)halMemFree(sq_que_va);
            trs_err("Svm dev va not equal to host va. (devid=%u; size=%llu; ret=%d)\n", dev_id, size, ret);
            return DRV_ERROR_INNER_ERR;
        }
    } else {
        *va = sq_que_va;
    }

    return 0;
}

void trs_sq_svm_mem_free(uint32_t dev_id, void *va)
{
    if (trs_get_sq_send_mode(dev_id) == TRS_MODE_TYPE_SQ_SEND_HIGH_PERFORMANCE) {
        (void)halHostUnregisterEx(va, dev_id, DEV_SVM_MAP_HOST);
    }

    (void)halMemFree(va);
}

#ifndef EMU_ST
static int __attribute__((constructor)) trs_sqcq_mem_construct(void)
{
    struct trs_sqcq_mem_ops sqcq_mem_ops = {NULL};

    sqcq_mem_ops.mem_alloc = trs_sq_svm_mem_alloc;
    sqcq_mem_ops.mem_free = trs_sq_svm_mem_free;
    trs_register_sqcq_mem_ops(&sqcq_mem_ops);

    trs_info("Register Sqcq mem ops successed.\n");
    return 0;
}
#else
int trs_sqcq_mem_construct(void)
{
    struct trs_sqcq_mem_ops sqcq_mem_ops = {NULL};

    sqcq_mem_ops.mem_alloc = trs_sq_svm_mem_alloc;
    sqcq_mem_ops.mem_free = trs_sq_svm_mem_free;
    trs_register_sqcq_mem_ops(&sqcq_mem_ops);

    trs_info("Register Sqcq mem ops successed.\n");
    return 0;
}

void trs_sqcq_mem_un_construct(void)
{
    struct trs_sqcq_mem_ops sqcq_mem_ops = {NULL};
    trs_register_sqcq_mem_ops(&sqcq_mem_ops);
    trs_info("Unregister Sqcq mem ops successed.\n");
}
#endif
