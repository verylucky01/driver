/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_MESSAGE_H
#define BBOX_MESSAGE_H

// enum message type for bbox_msg_header_t.type
enum bbox_msg_type {
    BBOX_MSG_NULL = 0x0,
    // hand shake msg 1 ~ 15
    BBOX_MSG_HELLO = 0x1,
    // exception msg 16 ~ 63
    BBOX_MSG_EXCEPTION = 0x10,
    BBOX_MSG_OOM,
    BBOX_MSG_EXCEPTION_MAX = 0x3F,
    // device notify msg 64 ~ 79
    BBOX_MSG_REBOOT = 0x40,
    BBOX_MSG_NOTIFY_MAX = 0x4F,
    // ack reply msg 80 ~ 95
    BBOX_MSG_ACK = 0x50,
    BBOX_MSG_NAK,
    BBOX_MSG_RPL_MAX = 0x5F,
    // host control msg 96 ~ xxx
    BBOX_MSG_CTRL = 0x60,
    // max msg type
    BBOX_MSG_MAX = 0xFF,
};

// enum message data type for bbox_except_msg_t.d_type
enum bbox_msg_data_type {
    DATA_MSG_T_NULL     = 0, // empty set
    DATA_MSG_T_FULL     = 1, // full set, BBOX_DATASET_FULL
    DATA_MSG_T_MIN      = 2, // mini set, BBOX_DATASET_MIN
    DATA_MSG_T_DDR      = 3, // ddr dump, BBOX_DATASET_DDR
    DATA_MSG_T_KLOG     = 4, // kernel log, BBOX_DATASET_KLOG
    DATA_MSG_T_NODDR    = 5, // no ddr data, BBOX_DATASET_NODDR
    DATA_MSG_T_DMA      = 6, // dma dump, BBOX_DATASET_DMA
    DATA_MSG_T_PMU      = 7, // BBOX_DATASET_PMU
    DATA_MSG_T_SRAM     = 8, // BBOX_DATASET_SRAM
    DATA_MSG_T_OOM      = 9, // BBOX_DATASET_OOM
    DATA_MSG_T_LOG      = 10, // BBOX_DATASET_LOG
    DATA_MSG_T_HDR      = 11, // BBOX_DATASET_HDR
    DATA_MSG_T_TSENSOR  = 12, // BBOX_DATASET_TSENSOR
    DATA_MSG_T_CDR      = 13, // BBOX_DATASET_CDR
    DATA_MSG_T_RECOVER  = 14, // BBOX_DATASET_RECOVER
    DATA_MSG_T_MAX      = 64,
};

#endif
