/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef _HDC_EPOLL_H_
#define _HDC_EPOLL_H_

#include "hdc_cmn.h"
#include "ascend_hal.h"

#define HDC_EPOLL_FD_INVALID ((mmProcess)-1)

struct hdc_epoll_ops {
    drvError_t (*hdc_epoll_create)(struct hdc_epoll_head *epoll_head, signed int size);
    drvError_t (*hdc_epoll_ctl)(struct hdc_epoll_head *epoll_head,
        signed int op, void *target, const struct drvHdcEvent *event);
    drvError_t (*hdc_epoll_wait)(const struct hdc_epoll_head *epoll_head,
        struct drvHdcEvent *events, signed int maxevents, signed int timeout, signed int *eventnum);
    drvError_t (*hdc_epoll_close)(struct hdc_epoll_head *epoll_head);
};

struct hdc_epoll_ops *drv_get_hdc_pcie_epoll_ops(void);
struct hdc_epoll_ops *drv_get_hdc_sock_epoll_ops(void);
struct hdc_epoll_ops *drv_get_hdc_ub_epoll_ops(void);

#endif

