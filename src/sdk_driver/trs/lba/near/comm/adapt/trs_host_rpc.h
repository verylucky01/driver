/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#ifndef TRS_HOST_RPC_H
#define TRS_HOST_RPC_H

int trs_urd_rpc_msg_set(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int trs_urd_rpc_msg_get(void *feature, char *in, u32 in_len, char *out, u32 out_len);

int trs_urd_rpc_msg_ctrl_init(void);
void trs_urd_rpc_msg_ctrl_uninit(void);
#endif
