/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_DDR_AP_ADAPTER_H
#define BBOX_DDR_AP_ADAPTER_H

#include <stddef.h>
#include "bbox_ddr_int.h"

#define AP_DUMP_MAGIC           0x19283746
#define AP_DUMP_END_MAGIC       0x1F2E3D4C
#define REGS_DUMP_MAX_NUM       10U
#define REGS_NAME_LEN           12
#define AP_LOG_BUFFER_NUM       10U

#define FILE_AP_EH_ROOT "os_info.txt"
#define REG_NAME_PMU    "pmu"

#define DEFAULT_DEVICE_ID               0
#define DEVICE_OS_MAX_NUM               4

struct ap_regs_info {
    char reg_name[REGS_NAME_LEN];
    u32 reg_size;
    u64 reg_base;
    u64 reg_map_addr;
    u64 reg_dump_addr;
};

struct ap_top_head {
    u32 dump_magic;
    u32 version;         // ap version
    u32 device_num;
    u32 cpu_num;
    u32 reserve;
    u32 end_magic;
};

struct ap_current_info {
    u32 devid;
    u32 excepid;
    u8  e_type;
    u8  coreid;
    u16 reserve;
    struct bbox_time tm;
};

struct ap_log_record {
    u32 devid;
    u32 excepid;
    u8  e_type;
    u8  coreid;
    u16 reserve;
    char date[DATATIME_MAXLEN];
};

struct ap_log_info {
    u16 event_flag;
    u8 log_num;
    u8 next;
    struct ap_log_record log_buffer[AP_LOG_BUFFER_NUM];
};

struct ap_area_info {
    u64 ap_map_addr;          // reserved virtual addr
    u64 ap_area_addr;         // reserved physical addr
    u32 ap_area_len;          // reserved physical addr length
    u32 reg_regions_num;
    struct ap_regs_info dump_regs_info[DEVICE_OS_MAX_NUM][REGS_DUMP_MAX_NUM]; // regs info for all deivce
    u32 core_reg_regions_num;
    struct ap_regs_info core_dump_regs_info[DEVICE_OS_MAX_NUM][REGS_DUMP_MAX_NUM]; // regs info for all deivce
};

struct vmcore_area_info {
    u64 vmcore_phy_addr;
};
 
struct ap_root_info {
#if defined(CFG_SOC_PLATFORM_CLOUD_V2) || defined(CFG_SOC_PLATFORM_CLOUD_V4)
    struct vmcore_area_info vmcore_info;  // for kdump, must be the first element
#endif
    struct ap_top_head top_head;
    struct ap_current_info current_info;
    struct ap_log_info log_info;
    struct ap_area_info area_info;
    struct ap_top_head coretop_head;
    struct ap_current_info corecurrent_info;
    struct ap_log_info corelog_info;
};

static inline void *bbox_rdr_ap_vaddr_convert(const struct ap_root_info *ap, u64 vaddr, u32 *len)
{
    u64 map_addr = ap->area_info.ap_map_addr;
    u64 area_len = ap->area_info.ap_area_len;
    u64 offset = 0;

    if (vaddr > map_addr) {
        offset = vaddr - map_addr;
        if (offset <= area_len) {
            *len = (u32)area_len - (u32)offset;
            return (u8 *)(uintptr_t)ap + offset;
        }
    }
    return NULL;
}

void bbox_ddr_dump_ap(const struct ap_root_info *ap, u32 len, const char *log_path);

#endif
