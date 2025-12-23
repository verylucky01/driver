/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_DUMP_PROCESS_MILAN_H
#define BBOX_DUMP_PROCESS_MILAN_H

// hdr reserve ddr: 0x36C00000-0x36C78800
#define BBOX_DDR_HDR_OFFSET             0x0UL
#define BBOX_DDR_HDR_LEN                0x80000UL // 512K

// kernel log redirect: 0x3620100-0x36400000
#define BBOX_DDR_KLOG_OFFSET            0x00001000UL
#define BBOX_DDR_KLOG_LEN               0x001FF000UL // 2M - 4k

// bbox reserve ddr: 0x36400000-0x36D00000
#define BBOX_DDR_BASE_OFFSET            0
#define BBOX_DDR_BASE_LEN               0x900000UL // 9M

#define BBOX_BIOS_KEYPOINT_LEN          4
#define BBOX_BIOS_KEYPOINT_OFFSET       0x00029004UL
#define BOOTUP_STAGE_BIOS_SUCC  99U

#define BBOX_DDR_CHIP_DFX_LEN           0x800000UL  // 8M
#define BBOX_DDR_CHIP_DFX_OFFSET        0x0UL
#define BBOX_SRAM_CHIP_DFX_LEN          0xC000UL    // 48k
#define BBOX_SRAM_CHIP_DFX_OFFSET       0x1C000UL
#define BBOX_SRAM_HBM_DFX_LEN          0x4000UL    // 16k
#define BBOX_SRAM_HBM_DFX_OFFSET       0x14000UL


/* dump sram binary data before boot */
#define BBOX_SRAM_SNAPSHOT_LEN         0x12000UL    // 72k
#define BBOX_SRAM_SNAPSHOT_OFFSET      0x8000UL
#define BBOX_SRAM_BIOS_HISS_LEN        0x1000UL    // 4k
#define BBOX_SRAM_BIOS_HISS_OFFSET     0x29000UL
#define BBOX_SRAM_HBOOT_LEN            0x200000UL    // 2M
#define BBOX_SRAM_HBOOT_OFFSET         0x0UL

// ts log
#define BBOX_DDR_TS_LOG_LEN             0x00100000UL // 1M
#define BBOX_DDR_TS_LOG_OFFSET          0x0UL

// device slog
#define BBOX_DDR_RUN_DEVICE_OS_LOG_LEN    0x00300000UL // 3M
#define BBOX_DDR_DEBUG_DEVICE_OS_LOG_LEN  0x00300000UL // 3M
#define BBOX_DDR_DEBUG_DEVICE_FW_LOG_LEN  0x01400000UL // 20M
#define BBOX_DDR_RUN_EVENT_LOG_LEN        0x00200000UL // 2M
#define BBOX_DDR_SEC_DEVICE_OS_LOG_LEN    0x00200000UL // 2M
#define BBOX_DDR_SLOG_OFFSET              0x0UL

/* vmcore file*/
#define BBOX_HBM_VMCORE_STAT_LEN       (2 * sizeof(unsigned int))
#define BBOX_HBM_VMCORE_STAT_OFFSET    0x0UL
#define BBOX_HBM_VMCORE_LEN            (0xFFFFFFFFUL)
#define BBOX_HBM_VMCORE_OFFSET         0x0UL

#endif
