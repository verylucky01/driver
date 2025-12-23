/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TRS_DEV_DRV_H__
#define TRS_DEV_DRV_H__

#include <stddef.h>
#include "ascend_hal_define.h"

#include "drv_type.h"

#define SVM_DEV_NAME                "/dev/svm0"
#define SVM_IOCTL_PROCESS_BIND      0xffff

struct trs_svm_process_info {
    pid_t vpid;
    unsigned long long ttbr;
    unsigned long long tcr;
    int pasid;
    unsigned int flags;
};

struct trs_dev_init_ops {
    int (*dev_init)(uint32_t dev_id);
    void (*dev_uninit)(uint32_t dev_id);
};

int trs_svm_proc_bind(void);
void trs_register_dev_init_ops(struct trs_dev_init_ops *ops);
void *trs_dev_mmap(uint32_t dev_id, void *addr, size_t length, int prot, int flags);
int trs_dev_munmap(void *addr, size_t length);
drvError_t trs_urma_proc_ctx_init_by_devid(uint32_t dev_id);
void _trs_urma_proc_ctx_uninit_by_devid(uint32_t dev_id);
int trs_dev_io_ctrl(uint32_t dev_id, uint32_t cmd, void *para);
#endif

