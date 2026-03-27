/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_CHAN_H
#define QUE_CHAN_H

#include "urma_api.h"
#include "que_comm_agent.h"
#include "ascend_hal_define.h"
#include "ascend_hal_external.h"

int que_chan_pkt_send(unsigned int devid, unsigned int qid, QUEUE_AGENT_TYPE que_type, struct buff_iovec *vector, uint64_t *stamp);
int que_chan_wait(unsigned int devid, unsigned int qid, QUEUE_AGENT_TYPE que_type, int timeout);
int que_chan_ini_update(unsigned int devid, unsigned int qid, struct que_jfs_pool_info *jfs_info, struct que_jfr_pool_info *jfr_info,
    urma_target_jetty_t *tjetty, urma_token_t token, QUEUE_AGENT_TYPE que_type, unsigned int d2d_flag);

#endif
