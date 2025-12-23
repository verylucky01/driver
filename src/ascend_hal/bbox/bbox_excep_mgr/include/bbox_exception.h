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
    u32 e_excep_id;                              /* Exception ID. */
    u8 e_process_level;                          /* Exception handling priority: PROCESS_PRI. */
    u8 e_reboot_priority;                        /* Exception restart priority: REBOOT_PRI. */
    u8 e_excep_type;                             /* Exception type. */
    u8 e_reentrant;                             /* Whether the exception is reentrant. */
    u64 e_notify_core_mask;                       /* Exception linkage mask. */
    u8 e_desc[RDR_EXCEPTIONDESC_MAXLEN];        /* Exception description. */
};

struct exc_info_s {
    bbox_time_t e_clock;                          /* Time when the module triggers an exception. */
    u32 e_excep_id;                              /* ID of the exception triggered by the module. */
    u16 e_dump_status;                           /* Control status for the module to store exception information in the reserved memory. */
    u16 e_save_status;                           /* Control status for the agent to export exception information from the reserved memory. */
};

/* Registering exceptions through shared memory. */
struct exc_module_info_s {
    u32 magic;                                 /* Using the macro MODULE_MAGIC. */
    u16 e_excep_valid;                           /* After the module writes the registered exceptions, set this to 1. */
    u16 e_excep_num;                             /* Number of exceptions registered by the module. */
    u8 e_from_module[RDR_MODULE_NAME_LEN];       /* Module name. */
    struct exc_info_s cur_info;                   /* Control status for module dump information. */
    u32 e_mini_offset;                           /* Offset of the minimum set of exception information in the module, based on the start addr of the reserved memory, starting from the magic number. */
    u32 e_mini_len;                              /* Length of the minimum set of exception information in the module. */
    u32 e_info_offset;                           /* Offset of all exception information in the module, based on the start addr of the reserved memory, starting from the magic number. */
    u32 e_info_len;                              /* Length of all exception information in the module. */
    struct exc_description_s e_description[1];    /* Module exception registration information. */
};

/* Registering exceptions through the registration function. */
struct rdr_ddr_module_infos {
    u32 magic;                                 /* Using the macro MODULE_MAGIC. */
    u32 e_mini_offset;                           /* Offset of the minimum set of exception information in the module, based on the start addr of the reserved memory, starting from the magic number. */
    u32 e_mini_len;                              /* Length of the minimum set of exception information in the module. */
    u32 e_info_offset;                           /* Offset of all exception information in the module, based on the start addr of the reserved memory, starting from the magic number. */
    u32 e_info_len;                              /* Length of all exception information in the module. */
}; // for mini&cloud -- end

// for new structure
enum BBOX_PROXY_BLOCK_TYPE {
    BLOCK_TYPE_NORMAL        = 1 << 0,    /* Normal data. */
    BLOCK_TYPE_STARTUP       = 1 << 1     /* Startup exception data. */
};

#define BBOX_PROXY_MAGIC                0x56312e31
#define BBOX_PROXY_CTRL_PAD             3

static inline u32 bbox_get_magic(const buff *buffer)
{
    return (*(const u32 *)buffer);
}

struct bbox_proxy_exception_ctrl {
    bbox_time_t e_clock;           /* Time when the module triggers an exception. */
    u32 e_main_excepid;           /* ID of the exception triggered by the module. */
    u32 e_sub_excepid;            /* ID of the exception triggered by the module. */
    u32 e_info_offset;            /* Offset of all exception information in the module, based on the start addr of the reserved memory, starting from the magic number. */
    u32 e_info_len;               /* Length of all exception information in the module. */
    u16 e_dump_status;            /* Control status for the module to store exception information in the reserved memory. */
    u16 e_save_status;            /* Control status for the agent to export exception information from the reserved memory. */
    u32 e_reserved;              /* Structure alignment reservation. */
};

struct bbox_proxy_block_info {
    u32 ctrl_type : 16;
    u32 ctrl_flag : 16;
    u32 info_offset;
    u32 info_block_len;
};

struct bbox_proxy_ctrl_info {
    u8 e_block_num;                                               /* Number of control blocks to be used, up to BBOX_PROXY_CTRL_NUM. */
    u8 padding[BBOX_PROXY_CTRL_PAD];                            /* Placeholder bytes. */
    struct bbox_proxy_block_info block_info[BBOX_PROXY_CTRL_NUM];   /* Control block configuration. */
};

struct bbox_proxy_module_ctrl {
    u32 magic;                                                  /* Using the macro BBOX_PROXY_MAGIC. */
    struct bbox_proxy_ctrl_info config;                            /* ctrl block configuration. */
    struct bbox_proxy_exception_ctrl block[BBOX_PROXY_CTRL_NUM];   /* Control status for module dump information. */
    u8 reserved[BBOX_PROXY_CTRL_RESERV];                        /* Reserved space for future expansion. */
};

struct bbox_module_exception_ctrl {
    bbox_time_t e_clock;   /* Time when the module triggers an exception. */
    u32 e_excep_id;       /* ID of the exception triggered by the module. */
    u32 e_block_offset;   /* Start offset of the exception information block in the module, based on the start addr of the reserved memory, starting from the magic number. */
    u32 e_block_len;      /* Length of the exception information block in the module. */
    u32 e_info_len;       /* Actual length of the exception information in the module. */
};

#define BBOX_MODULE_MAGIC                0x56312e32
#define BBOX_MODULE_CTRL_PAD             3
#define BBOX_MODULE_CTRL_NUM             6
#define BBOX_MODULE_CTRL_RESERV          312

struct bbox_module_ctrl {
    u32 magic;                                                  /* Using the macro BBOX_MAGIC. */
    u8 e_block_num;                                               /* Number of control blocks to be used, up to BBOX_PROXY_CTRL_NUM. */
    u8 padding[BBOX_MODULE_CTRL_PAD];                           /* Placeholder bytes. */
    struct bbox_module_exception_ctrl block[BBOX_MODULE_CTRL_NUM]; /* Control status for module dump information. */
    u8 reserved[BBOX_MODULE_CTRL_RESERV];                       /* Reserved space for future expansion. */
}; // for new structure --end

#define BBOX_MODULE_CTRL_BLOCK_SIZE sizeof(struct bbox_module_ctrl) // total 512 byte

#endif /* BBOX_EXCEPTION_H */
