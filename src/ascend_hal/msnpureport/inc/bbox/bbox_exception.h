/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_EXCEPTION_H
#define BBOX_EXCEPTION_H

#include "bbox_int.h"
#include "bbox_product.h"

// for mini&cloud
#define MODULE_MAGIC                0xbaba0514U
#define RDR_EXCEPTIONDESC_MAXLEN 48
#define RDR_MODULE_NAME_LEN  16
#define THREAD_SIZE     0x2000

struct exc_description_s {
    u32 e_excep_id;                              /* 异常id */
    u8 e_process_level;                          /* 异常处理级别:PROCESS_PRI */
    u8 e_reboot_priority;                        /* 异常重启级别:REBOOT_PRI */
    u8 e_excep_type;                             /* 异常类型 */
    u8 e_reentrant;                             /* 异常是否可重入 */
    u64 e_notify_core_mask;                       /* 异常联动掩码 */
    u8 e_desc[RDR_EXCEPTIONDESC_MAXLEN];        /* 异常描述 */
};

struct exc_info_s {
    bbox_time_t e_clock;                          /* 模块触发异常时间 */
    u32 e_excep_id;                              /* 模块触发的异常id */
    u16 e_dump_status;                           /* 模块将异常信息存预留内存的控制状态 */
    u16 e_save_status;                           /* 代理将异常信息从预留内存导出的控制状态 */
};

/* 通过共享内存注册异常 */
struct exc_module_info_s {
    u32 magic;                                 /* 使用宏MODULE_MAGIC */
    u16 e_excep_valid;                           /* 模块写完注册的异常，则设置1 */
    u16 e_excep_num;                             /* 模块注册异常个数 */
    u8 e_from_module[RDR_MODULE_NAME_LEN];       /* 模块名 */
    struct exc_info_s cur_info;                   /* 模块dump信息控制状态 */
    u32 e_mini_offset;                           /* 模块最小集异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_mini_len;                              /* 模块最小集异常信息长度 */
    u32 e_info_offset;                           /* 模块全部异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_info_len;                              /* 模块全部异常信息长度 */
    struct exc_description_s e_description[1];    /* 模块异常注册信息 */
};

/* 通过注册函数注册异常 */
struct rdr_ddr_module_infos {
    u32 magic;                                 /* 使用宏MODULE_MAGIC */
    u32 e_mini_offset;                           /* 模块最小集异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_mini_len;                              /* 模块最小集异常信息长度 */
    u32 e_info_offset;                           /* 模块全部异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_info_len;                              /* 模块全部异常信息长度 */
}; // for mini&cloud -- end

// for new structure
enum BBOX_PROXY_BLOCK_TYPE {
    BLOCK_TYPE_NORMAL        = 1 << 0,    /* 普通数据 */
    BLOCK_TYPE_STARTUP       = 1 << 1     /* 启动异常数据 */
};

#define BBOX_PROXY_MAGIC                0x56312e31
#define BBOX_PROXY_CTRL_PAD             3

static inline u32 bbox_get_magic(const buff *buffer)
{
    return (*(const u32 *)buffer);
}

struct bbox_proxy_exception_ctrl {
    bbox_time_t e_clock;           /* 模块触发异常时间 */
    u32 e_main_excepid;           /* 模块触发的异常id */
    u32 e_sub_excepid;            /* 模块触发的异常id */
    u32 e_info_offset;            /* 模块全部异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_info_len;               /* 模块全部异常信息长度 */
    u16 e_dump_status;            /* 模块将异常信息存预留内存的控制状态 */
    u16 e_save_status;            /* 代理将异常信息从预留内存导出的控制状态 */
    u32 e_reserved;              /* 结构对齐预留 */
};

struct bbox_proxy_block_info {
    u32 ctrl_type : 16;
    u32 ctrl_flag : 16;
    u32 info_offset;
    u32 info_block_len;
};

struct bbox_proxy_ctrl_info {
    u8 e_block_num;                                               /* 需要使用的控制块个数，最多BBOX_PROXY_CTRL_NUM个 */
    u8 padding[BBOX_PROXY_CTRL_PAD];                            /* 占位字节 */
    struct bbox_proxy_block_info block_info[BBOX_PROXY_CTRL_NUM];   /* 控制块配置 */
};

struct bbox_proxy_module_ctrl {
    u32 magic;                                                  /* 使用宏BBOX_PROXY_MAGIC */
    struct bbox_proxy_ctrl_info config;                            /* ctrl块配置 */
    struct bbox_proxy_exception_ctrl block[BBOX_PROXY_CTRL_NUM];   /* 模块dump信息控制状态 */
    u8 reserved[BBOX_PROXY_CTRL_RESERV];                        /* 预留空间，用于后续扩展 */
};

struct bbox_module_exception_ctrl {
    bbox_time_t e_clock;   /* 模块触发异常时间 */
    u32 e_excep_id;       /* 模块触发的异常id */
    u32 e_block_offset;   /* 模块异常信息划分块起始偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_block_len;      /* 模块异常信息划分块长度 */
    u32 e_info_len;       /* 模块异常信息实际长度 */
};

#define BBOX_MODULE_MAGIC                0x56312e32
#define BBOX_MODULE_CTRL_PAD             3
#define BBOX_MODULE_CTRL_NUM             6
#define BBOX_MODULE_CTRL_RESERV          312

struct bbox_module_ctrl {
    u32 magic;                                                  /* 使用宏BBOX_MAGIC */
    u8 e_block_num;                                               /* 需要使用的控制块个数，最多BBOX_PROXY_CTRL_NUM个 */
    u8 padding[BBOX_MODULE_CTRL_PAD];                           /* 占位字节 */
    struct bbox_module_exception_ctrl block[BBOX_MODULE_CTRL_NUM]; /* 模块dump信息控制状态 */
    u8 reserved[BBOX_MODULE_CTRL_RESERV];                       /* 预留空间，用于后续扩展 */
}; // for new structure --end

#define BBOX_MODULE_CTRL_BLOCK_SIZE sizeof(struct bbox_module_ctrl) // total 512 byte

#endif /* BBOX_EXCEPTION_H */
