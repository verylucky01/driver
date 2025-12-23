/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_RDR_PUB_H
#define BBOX_RDR_PUB_H

#include <stdbool.h>
#include "bbox_proc_types.h"

/**********************************************************************/
/*                            exception id                            */
/*                                                                    */
/*  31  30 29  28 27     25 24       17 16       12 11             0  */
/*  +-----+------+---------+-----------+-----------+---------------+  */
/*  | H/D | type |  level  | module id | submod id | function code |  */
/*  +--2--+--2---+----3----+-----8-----+-----5-----+-------12------+  */
/*          H/D             type                level                 */
/*      0b01 host       0b01 error          0b100 emergency           */
/*      0b10 device     0b10 exception      0b011 critical            */
/*                                          0b010 warning             */
/*                                          0b001 notice              */
/*                                          0b000 no-level            */
/**********************************************************************/
// exceptions report from device hdc
#define ATF_PANIC                  0xA8340000U

#define RDR_EXCEPID_COREID_BIT          17U
#define BBOX_EXCEPID_POSITION_BIT       30U
#define BBOX_EXCEPID_POSITION_MASK      0x3U
#define BBOX_EXCEPID_POSITION_DEVICE    0x2U
#define BBOX_EXCEPID_COREID_MASK        0xFFU

enum bbox_rdr_rb_reason {
    REBOOT_REASON_LABEL0 = 0x0,                     /* label0: bios阶段模式重启 */
    DEVICE_COLDBOOT = 0x0,                              /* 冷启动，如关机后第一次开机；掉电后第一次开机 */
    BIOS_EXCEPTION = 0x1,                               /* bios异常，前一次启动bios异常 */
    DEVICE_HOTBOOT = 0x2,                               /* 热复位，如按键复位，芯片硬复位等 */
    REBOOT_REASON_LABEL1 = 0x10,                    /* label1:硬件原因复位 */
    ABNORMAL_EXCEPTION = 0x10,                          /* 未检测到的异常 */
    TSENSOR_EXCEPTION = 0x1F,                           /* soc温保复位 */
    PMU_EXCEPTION = 0x20,                               /* 过流、欠压、PMU过温引起的硬件复位 */
    DDR_CRITICAL_EXCEPTION = 0x22,                      /* ddr critical异常复位,如:ddr颗粒超温复位 */
    REBOOT_REASON_LABEL2 = 0x24,                    /* label2:OS软件原因复位 */
    OS_PANIC = 0x24,                                    /* os panic，如访问非法地址 */
    OS_OOM = 0x2A,                                      /* OOM 异常 */
    OS_COMM = 0x2B,                                     /* 通信异常 */
    REBOOT_REASON_LABEL3 = 0x2C,                    /* label3:模块复位 */
    STARTUP_EXCEPTION = 0x2C,                           /* 模块启动异常 */
    HEARTBEAT_EXCEPTION = 0x2D,                         /* 模块心跳异常 */
    RUN_EXCEPTION = 0x2E,                               /* 模块运行异常 */
    HDR_EXCEPTION = 0x2F,                               /* 模块快照异常 */
    LPM_EXCEPTION = 0x32,                               /* LPM3子系统检测到的各种异常 */
    TS_EXCEPTION = 0x33,                                /* TS子系统检测到的各种异常 */
    MICROWATT_EXCEPTION = 0x34,                         /* MICROWATT异常 */
    DVPP_EXCEPTION = 0x35,                              /* DVPP异常 */
    DRIVER_EXCEPTION = 0x36,                            /* DRIVER异常 */
    TEE_EXCEPTION = 0x38,                               /* teeos异常 */
    LPFW_EXCEPTION = 0x39,                              /* LPFW异常 */
    NETWORK_EXCEPTION = 0x3A,                           /* NETWORK异常 */
    HSM_EXCEPTION = 0x3B,                               /* HSM异常 */
    ATF_EXCEPTION = 0x3C,                               /* ATF异常 */
    ISP_EXCEPTION = 0x3D,                               /* ISP异常 */
    SAFETYISLAND_EXCEPTION = 0x3E,                      /* SAFETYISLAND异常 */
    TOOLCHAIN_EXCEPTION = 0x3F,                         /* TOOLCHAIN异常 */
    DSS_EXCEPTION = 0x40,                               /* DSS异常 */
    COMISOLATOR_EXCEPTION = 0x41,                       /* COMISOLATOR异常 */
    SD_EXCEPTION = 0x42,                                /* SD异常 */
    DP_EXCEPTION = 0x43,                                /* DP异常 */
    REBOOT_REASON_LABEL4 = 0x60,                    /* label4: */
    SUSPEND_FAIL = REBOOT_REASON_LABEL4,                /* 休眠失败LP发起的复位 */
    RESUME_FAIL  = 0x61,                                /* 唤醒失败LP发起的复位 */
    CPUCORE_EXCEPTION = 0x62,                           /* 核挂死异常 */
    REBOOT_REASON_LABEL8 = 0x89,                    /* label5: host侧异常 */
    DEVICE_LTO_EXCEPTION = 0x8A,                        /* 设备启动超时 */
    DEVICE_HBL_EXCEPTION = 0x8B,                        /* 设备心跳丢失 */
    DEVICE_HDC_EXCEPTION = 0x8E,                        /* 设备HDC异常 */
    BOOT_DOT_INFO = 0xFE,                           // 启动打点信息，启动后无条件导出打点日志.
    BBOX_EXCEPTION_REASON_INVALID = 0xFF,
};

/*
 * snapshot log for startup not exception
 */
#define BOOT_DOT_ID            0xB2600000


/* 模块id列表 */
enum bbox_rdr_coreid {
    BBOX_DRIVER      = 0x1,
    BBOX_OS          = 0x2,
    BBOX_TS          = 0x3,
    BBOX_AICPU       = 0x5,
    BBOX_DVPP        = 0xa,
    BBOX_AIPP        = 0xb,
    BBOX_LPM         = 0xc,
    BBOX_MICROWATT   = 0xd,
    BBOX_TOOLCHAIN   = 0xf,
    BBOX_BSBC        = 0x13,
    BBOX_BIOS        = 0x14,
    BBOX_TEEOS       = 0x15,
    BBOX_LPFW        = 0x17,
    BBOX_NETWORK     = 0x18,
    BBOX_ATF         = 0x1A,
    BBOX_HSM         = 0x1B,
    BBOX_ISP         = 0x1C,
    BBOX_SIL         = 0x1D,
    BBOX_DSS         = 0x1E,
    BBOX_COMISOLATOR = 0x1F,
    BBOX_AOS_SD      = 0x20,
    BBOX_AOS_DP      = 0x21,
    BBOX_AOS_LINUX   = 0x22,
    BBOX_AOS_CORE    = 0x23,
    BBOX_PEK         = 0x24,
    BBOX_IMU         = 0x25,
    BBOX_QOS         = 0x26,
    BBOX_UB          = 0x27,
    BBOX_COMMON      = 0x30,
    BBOX_REGDUMP     = 0x31,
    BBOX_CORE_MAX    = 0x32
};

static inline u8 bbox_excep_id_get_core_id(u32 excepid)
{
    return (u8)((excepid >> RDR_EXCEPID_COREID_BIT) & BBOX_EXCEPID_COREID_MASK);
}

static inline bool bbox_core_id_valid(u8 coreid)
{
    return ((coreid > 0) && (coreid < (u32)BBOX_CORE_MAX));
}

static inline bool bbox_check_excep_id_postition(u32 excepid)
{
    return (((excepid >> BBOX_EXCEPID_POSITION_BIT) & BBOX_EXCEPID_POSITION_MASK) == BBOX_EXCEPID_POSITION_DEVICE);
}

static inline bool bbox_check_excep_id_core_id(u32 excepid)
{
    return bbox_core_id_valid(bbox_excep_id_get_core_id(excepid));
}

static inline bool bbox_check_excep_id(u32 excepid)
{
    return (bbox_check_excep_id_postition(excepid) && bbox_check_excep_id_core_id(excepid));
}

#endif /* BBOX_RDR_PUB_H */
