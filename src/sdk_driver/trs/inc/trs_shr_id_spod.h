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
#ifndef TRS_SHR_ID_SPOD_H
#define TRS_SHR_ID_SPOD_H
#include <stdbool.h>

#ifdef EMU_ST
#define MAX_LOOP_TIME 30
#else
#define MAX_LOOP_TIME 30000000000
#endif

bool hal_kernel_trs_is_belong_to_pod_proc(unsigned int sdid, unsigned int tsid,
                                          int pid, int res_type, unsigned int res_id);
int trs_pod_msg_recv_async(u32 devid, u32 sdid, void *msg, size_t size, int *cmd_result, u32 mode);
#endif
