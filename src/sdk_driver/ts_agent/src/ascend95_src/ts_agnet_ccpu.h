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

#ifndef TS_AGENT_CCPU_H
#define TS_AGENT_CCPU_H

#ifdef CFG_SOC_PLATFORM_DAVID
#include <linux/types.h>
#include "tsch/task_struct.h"
#include "trs_adapt.h"

#ifndef TS_AGENT_UT
#define STATIC static
#else
#define STATIC
#endif

#define TS_AGENT_SQE_LENGTH_MAX 4U

typedef struct {
    /* word0-1 */
    ts_stars_sqe_header_t header;
    /* word2 */
    uint32_t res1;
    /* word3 */
    uint16_t res2;
    uint8_t kernel_credit;
    uint8_t res3 : 5;
    uint8_t sqe_length : 3;
    /* word4 */
    uint32_t pcie_dma_sq_addr_low;
    /* word5 */
    uint32_t pcie_dma_sq_addr_high;
    /* word6 */
    uint16_t pcie_dma_sq_tail_ptr;
    uint16_t die_id : 1;
    uint16_t res4 : 15;
    /* word7 */
    uint32_t is_converted : 1;  //use reserved filed
    uint32_t res5 : 31;
    /* word8~11 */
    uint64_t src;  //use reserved filed
    uint64_t dst;  //use reserved filed
    /* word12-15 */
    uint64_t length;  //use reserved filed
    uint32_t pass_id;  //use reserved filed
    uint32_t res6;
} ccpu_stars_pcie_dma_sqe;

int32_t tsagent_sqe_update(uint32_t devid, uint32_t tsid, struct trs_sqe_update_info *update_info);
int32_t tsagent_sqe_update_src_check(uint32_t devid, uint32_t tsid, struct trs_sqe_update_info *update_info);
void init_task_convert_func(void);
#endif
#endif // TS_AGENT_CCPU_H
