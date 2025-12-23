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
#ifndef TRS_HOST_INIT_H
#define TRS_HOST_INIT_H

#include "trs_pub_def.h"

int trs_host_msg_chan_recv(void *msg_chan, void *data, u32 in_data_len,
                           u32 out_data_len, u32 *real_out_len);
#endif /* TRS_HOST_INIT_H */
