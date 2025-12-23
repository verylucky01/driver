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

#ifndef MB_STRUCT_CALLBACK_H
#define MB_STRUCT_CALLBACK_H
#include "tsch_defines.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    volatile uint64_t sq_addr;
    volatile uint64_t cq_addr;
    volatile uint32_t sq_index;  // sq id
    volatile uint32_t cq_index;  // cq id
    volatile uint16_t sqe_size;
    volatile uint16_t cqe_size;
    volatile uint16_t sq_depth;
    volatile uint16_t cq_depth;
    volatile uint8_t app_flag;
    volatile uint8_t reserved[3];
    volatile uint32_t cq_irq;  // cq id
} ts_create_callback_sqcq_t;

typedef struct {
    volatile uint32_t sq_index;  // sq id
    volatile uint32_t cq_index;  // cq id
    volatile uint8_t app_flag;
    volatile uint8_t reserved[3];
} ts_release_callback_sqcq_t;

typedef struct {
    volatile uint32_t vpid;
    volatile uint32_t group_id;
    volatile uint32_t logic_cqid;
    volatile uint32_t phy_cqid;
    volatile uint32_t phy_sqid;
    volatile uint32_t cq_irq;
    volatile uint8_t app_flag;
    volatile uint8_t reserved[3];
} ts_create_callback_logic_cq_t;

typedef struct {
    volatile uint32_t vpid;
    volatile uint32_t group_id;
    volatile uint32_t logic_cqid;
    volatile uint32_t phy_cqid;
    volatile uint32_t phy_sqid;
} ts_release_callback_logic_cq_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MB_STRUCT_CALLBACK_H */
