/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_INI_PROC_H
#define QUE_INI_PROC_H

#include "urma_api.h"
#include "ascend_hal_define.h"
#include "que_uma.h"
#include "que_comm_agent.h"

int que_get_tgt_log_level(void);
uint64_t que_get_ini_basetime(unsigned int devid);
void que_update_ini_basetime(unsigned int devid);
void que_ini_time_stamp(struct que_ini_proc *ini_proc, QUE_TRACE_INI_TIMESTAMP type);
struct que_ini_proc *que_ini_proc_create();
void que_ini_proc_destroy(struct que_ini_proc *ini_proc);
struct que_pkt *que_tx_get_first_pkt(struct que_tx *tx);
struct que_pkt *que_tx_get_other_pkts(struct que_tx *tx);
int que_ini_pkt_send(struct que_ini_proc *ini_proc, struct buff_iovec *vector);
int que_ini_ack_wait(struct que_ini_proc *ini_proc, int timeout);
void que_ini_proc_done(struct que_ini_proc *ini_proc);
#endif