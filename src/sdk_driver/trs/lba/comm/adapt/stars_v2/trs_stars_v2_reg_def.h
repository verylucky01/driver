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
#ifndef TRS_STARS_V2_REG_DEF_V2_H
#define TRS_STARS_V2_REG_DEF_V2_H

#include <linux/types.h>

#define TRS_STARS_V2_SCHED_SQ_TAIL_OFFSET              0x0
#define TRS_STARS_V2_SCHED_SQ_HEAD_OFFSET              0x10
#define TRS_STARS_V2_SCHED_SQ_STATUS_OFFSET            0x14

#define TRS_STARS_V2_SCHED_CQ_HEAD_OFFSET              0x888
#define TRS_STARS_V2_SCHED_CQ_TAIL_OFFSET              0x890

#define TRS_STARS_V2_CQINT_L1_MASK_OFFSET              0x2020
#define TRS_STARS_V2_CQINT_L1_STATUS_OFFSET            0x2024

#define TRS_STARS_V2_CQINT_MID_REG_NUM       2

#define TRS_STARS_V2_CQINT_MID_STATUS_OFFSET(mid_index)        (0x2040 + (mid_index) * 4)

#ifdef CFG_MEMORY_OPTIMIZE
#define TRS_STARS_V2_CQINT_MID_STATUS_WIDTH                    8
#endif

#define TRS_STARS_V2_CQINT_L2_STATUS_OFFSET(l2_index)          (0x2180 + (l2_index) * 4)
#ifdef CFG_SOC_PLATFORM_FPGA
#define TRS_STARS_V2_CQINT_L2_STATUS_WIDTH                     8 /* sq_num_per_slice / rtsq_reg_ps */
#else
#define TRS_STARS_V2_CQINT_L2_STATUS_WIDTH                     32
#endif

#define TRS_STARS_V2_CQINT_L2_CTRL_OFFSET(l2_index)            (0x2280 + (l2_index) * 4)

#define TRS_STARS_V2_HOST_CQINT_L1_STATUS_OFFSET(group)    (((group) * 0x10000) + 0x10)
#define TRS_STARS_V2_HOST_CQINT_L1_CTRL_OFFSET(group)      (((group) * 0x10000) + 0x14)
#define TRS_STARS_V2_HOST_CQINT_L1_MASK_OFFSET(group)      (((group) * 0x10000) + 0x20)

#define TRS_STARS_V2_HOST_CQINT_MID_STATUS_OFFSET(group)   (((group) * 0x10000) + 0x40)

#define TRS_STARS_V2_HOST_CQINT_L2_STATUS_OFFSET(group, mid_bit)   (((group) * 0x10000) + ((mid_bit) * 0x4) + 0x90)
#define TRS_STARS_V2_HOST_CQINT_L2_CTRL_OFFSET(group, mid_bit)     (((group) * 0x10000) + ((mid_bit) * 0x4) + 0xA0)
#endif /* TRS_STARS_V2_REG_DEF_V1_H */
