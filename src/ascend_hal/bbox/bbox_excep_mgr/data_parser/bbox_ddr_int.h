/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_DDR_INT_H
#define BBOX_DDR_INT_H

#include "bbox_int.h"
#include "bbox_rdr_pub.h"
#include "bbox_exception.h"

#define MODULE_NAME_LEN     56
#define FILE_MAGIC          0xdead8d8dU
#define RDR_VERSION         ((1 << 16) | (0x1 << 0))
#define RDR_BASEINFO_SIZE   0x2000U
#define RDR_AREA_MAXIMUM    17
#define RDR_LOG_BUFFER_NUM  50U
#define RDR_PRODUCT_RELATION_LEN 16

#define DATA_MAXLEN         15
#define DATATIME_MAXLEN     25 /* 14+9+2, 2: '-'+'\0' */

#define TMP_BUFF_B_LEN 1024U
#define TMP_BUFF_S_LEN 256

#define RDR_MIN(a, b) (((a) < (b)) ? (a) : (b))

enum rdr_record_type_list {
    RDR_RECORD_NORMAL = 0x0,
    RDR_RECORD_DEFINE_EXCEPTION = 0x1,
    RDR_RECORD_UNDEFINE_EXCEPTION,
    RDR_RECORD_RESET_EXCEPTION
};

struct rdr_base_info {
    u32 excepid;
    u32 devid;
    u32 arg;
    u8 e_type;
    u8 coreid;
    u8 reserve[6];              // reserve 6 bytes
    char date[DATATIME_MAXLEN];
    struct bbox_time tm;
    u8 module[RDR_MODULE_NAME_LEN];
    u8 desc[RDR_EXCEPTIONDESC_MAXLEN];
    u32 start_flag;
    u32 reboot_flag;
    u32 comm_flag;
};

struct rdr_top_head {
    u32 magic;
    u32 version;
    u32 area_number;
    u32 reserve;
    u8 product_name[RDR_PRODUCT_RELATION_LEN];
};

struct rdr_area_info {
    u64 offset;     // area addr, unit is bytes(1 bytes)
    u32 length;     // area len, unit is bytes
    u8 coreid;      // module id
    u8 reserve[3];  // reserve 3 bytes
};

struct log_record_data {
    u32 devid;
    u32 excepid;
    u8  e_type;
    u8  coreid;
    u16 reserve;
    u32 arg;
    /* UTC time difference from 1970 to the time when an exception is recorded */
    struct bbox_time tm;
    char date[DATATIME_MAXLEN]; /* UTC time string */
};

struct rdr_log_buffer {
    enum rdr_record_type_list record_type;
    struct log_record_data record_info;
};

struct rdr_log_info {
    u16 event_flag;
    u8 log_num;
    u8 next_valid_index;
    struct rdr_log_buffer log_buffer[RDR_LOG_BUFFER_NUM];
};

struct rdr_head {
    struct rdr_top_head top_head;
    struct rdr_base_info base_info;
    struct rdr_log_info log_info;
    struct rdr_area_info area_info[RDR_AREA_MAXIMUM];
    struct rdr_base_info corebase_info;
    struct rdr_log_info corelog_info;
};

#endif
