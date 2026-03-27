/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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

#ifndef _VMNG_CMD_MSG_H_
#define _VMNG_CMD_MSG_H_
#include "vmng_kernel_interface_cmd.h"

struct soc_mia_ub_info {
    u32 eid;
    u32 token;
    u32 jetty_id;
};

struct vmng_ctrl_msg_sync {
    int dev_id;
    unsigned int aicore_num;
    u32 reserved[6];
};

struct vmng_ctrl_msg_info {
    u32 dev_id;
    u32 vfid;
    u32 vfg_type;
    u32 dtype;
    u32 core_num;
    u32 total_core_num;
    u32 reserved[4];
    union {
        struct vmng_vf_res_info vf_cfg;
        struct vmng_soc_resource_enquire enquire;
        struct vmng_soc_resource_refresh refresh;
        int sriov_status;
        struct vmng_mdev_iova_info iova_info;
        struct vmng_vf_sync_remote_id id_info;
        struct soc_mia_ub_info ub_info;
    };
};

struct vmng_ctrl_msg {
    int type;
    int error_code;
    union {
        struct vmng_ctrl_msg_sync sync_msg;
        struct vmng_ctrl_msg_info info_msg;
    };
};
#endif