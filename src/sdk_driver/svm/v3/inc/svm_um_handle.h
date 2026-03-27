/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef SVM_UM_HANDLE_H
#define SVM_UM_HANDLE_H

#include <linux/types.h>

#include "svm_sub_event_type.h"

#define SVM_INVALID_EVENT_GID 63 /* esched max gid is 64 */

void svm_um_register_handle(u32 subevent_id,
    int (*pre_handle)(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len),
    void (*pre_cancel_handle)(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len),
    int (*post_handle)(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len));

int svm_um_handle_init(void);
void svm_um_handle_uninit(void);

#endif

