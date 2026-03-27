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
#ifndef APM_MSG_H
#define APM_MSG_H

#include "apm_auto_init.h"
#include "apm_kernel_msg.h"
#include "apm_kern_log.h"

static inline void apm_msg_fill_header(struct apm_msg_header *header, enum apm_msg_type msg_type)
{
    header->msg_type = msg_type;
    header->result = 0;
}

void apm_register_msg_handle(enum apm_msg_type msg_type, u32 msg_len,
    int (*fn)(u32 udevid, struct apm_msg_header *header));
int apm_msg_send(u32 udevid, struct apm_msg_header *header, u32 size);
int apm_msg_recv(u32 udevid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);

#endif
