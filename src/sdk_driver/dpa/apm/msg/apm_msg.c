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

#include "apm_msg.h"

static int (* apm_msg_handler[APM_MSG_TYPE_MAX])(u32 udevid, struct apm_msg_header *header) = {NULL, };
static u32 apm_msg_len[APM_MSG_TYPE_MAX] = {0, };

void apm_register_msg_handle(enum apm_msg_type msg_type, u32 msg_len,
    int (*fn)(u32 udevid, struct apm_msg_header *header))
{
    apm_msg_handler[msg_type] = fn;
    apm_msg_len[msg_type] = msg_len;
}

int apm_msg_recv(u32 udevid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    struct apm_msg_header *header = (struct apm_msg_header *)data;

    if ((data == NULL) || (real_out_len == NULL)) {
        apm_err("Null ptr. (udevid=%d)\n", udevid);
        return -EINVAL;
    }

    /* check header can be access */
    if (in_data_len < sizeof(*header)) {
        apm_err("Invalid len. (udevid=%d; in_data_len=%d; out_data_len=%d)\n", udevid, in_data_len, out_data_len);
        return -EINVAL;
    }

    if ((header->msg_type < 0) || (header->msg_type >= APM_MSG_TYPE_MAX)) {
        apm_err("Invalid msg_type. (udevid=%d; msg_type=%d)\n", udevid, header->msg_type);
        return -EINVAL;
    }

    if (apm_msg_handler[header->msg_type] == NULL) {
        apm_err("No msg handle. (udevid=%d; msg_type=%d)\n", udevid, header->msg_type);
        return -EINVAL;
    }

    if ((in_data_len != apm_msg_len[header->msg_type]) || (out_data_len != apm_msg_len[header->msg_type])) {
        apm_err("Invalid len. (udevid=%d; msg_type=%d; in_data_len=%d; out_data_len=%d; msg_len=%d)\n",
            udevid, header->msg_type, in_data_len, out_data_len, apm_msg_len[header->msg_type]);
        return -EINVAL;
    }

    *real_out_len = out_data_len;
    header->result = apm_msg_handler[header->msg_type](udevid, header);

    return 0;
}

