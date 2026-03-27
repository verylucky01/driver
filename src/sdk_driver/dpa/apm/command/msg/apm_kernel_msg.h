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

#ifndef APM_KERNEL_MSG_H
#define APM_KERNEL_MSG_H

#include "ka_base_pub.h"

#include "drv_type.h"
#include "ascend_hal_define.h"
#include "apm_kernel_ioctl.h"

/* apm_msg.h */
enum apm_msg_type {
    APM_MSG_TYPE_MASTER_DESTROY,
    APM_MSG_TYPE_SLAVE_DESTROY,
    APM_MSG_TYPE_BIND,
    APM_MSG_TYPE_UNBIND,
    APM_MSG_TYPE_QUERY_MASTER,
    APM_MSG_TYPE_QUERY_SLAVE_SSID,
    APM_MSG_TYPE_SET_SLAVE_STATUS,
    APM_MSG_TYPE_QUERY_TASK_GROUP_EXIT_STAGE,
    APM_MSG_TYPE_MAP,
    APM_MSG_TYPE_UNMAP,
    APM_MSG_TYPE_MAP_QUERY,
    APM_MSG_TYPE_QUERY_SLAVE_MEMINFO,
    APM_MSG_TYPE_NOTICE_HOST_OS_ID,    
    APM_MSG_TYPE_MAX
};

/* apm_msg.h */
struct apm_msg_header {
    enum apm_msg_type msg_type;
    int result;
};

/* apm_task_group_def */
struct apm_task_group_cfg {
    int master_tgid;
    int slave_tgid;
    int local_flag;
    struct apm_cmd_bind para;
};

/* apm_task_group_msg.h */
struct apm_msg_bind_unbind {
    struct apm_msg_header header;
    struct apm_task_group_cfg cfg;
    unsigned int rsv[4];    /* reserve */
};

/* apm_task_group_msg.h */
struct apm_msg_master_destroy {
    struct apm_msg_header header;
    int tgid;
    unsigned int rsv[4];    /* reserve */
};

/* apm_task_group_msg.h */
struct apm_msg_slave_destroy {
    struct apm_msg_header header;
    int tgid;
    unsigned int rsv[4];    /* reserve */
};

/* apm_task_group_msg.h */
struct apm_msg_query_master {
    struct apm_msg_header header;
    u32 udevid;
    int slave_tgid;
    int master_tgid;
    unsigned int rsv[4];    /* reserve */
};

/* apm_slave_ssid.h */
struct apm_msg_query_slave_ssid {
    struct apm_msg_header header;
    int master_tgid;
    processType_t proc_type;
    int ssid;
    unsigned int rsv[4];    /* reserve */
};

/* apm_task_group_msg.h */
struct apm_msg_set_slave_status {
    struct apm_msg_header header;
    int master_tgid;
    int slave_tgid;
    int type;
    int status;
    unsigned int rsv[4];    /* reserve */
};

/* apm_task_group_msg.h */
struct apm_msg_query_task_group_exit_stage {
    struct apm_msg_header header;
    int master_tgid;
    int slave_tgid;
    u32 proc_type_bitmap;
    int exit_stage;
    unsigned int rsv[4];    /* reserve */
};

/* apm_res_map_ctx.h */
struct apm_res_map_info {
    struct res_map_info_in res_info;
    int slave_tgid;
    u32 udevid;
    unsigned long va;
    u64 *pa_array;
    u64 pa;
    u32 len;
    ka_atomic_t ref;
};

#define APM_RES_MAP_INFO_PRIV_LEN_MAX   128U
/* apm_task_group_msg.h */
struct apm_msg_map_unmap {
    struct apm_msg_header header;
    struct apm_res_map_info para;
    char res_map_priv[APM_RES_MAP_INFO_PRIV_LEN_MAX];
    unsigned int rsv[4];    /* reserve */
};

/* apm_task_group_msg.h */
struct apm_msg_query_slave_meminfo {
    struct apm_msg_header header;
    u32 udevid;
    int slave_tgid;
    u32 type;
    u64 size;
    unsigned int rsv[4];    /* reserve */
};

struct apm_msg_notice_host_os_id {
    struct apm_msg_header header;
    u32 phy_devid;
    u32 sub_devid;
    int host_os_id;
    unsigned int rsv[4];    /* reserve */
};

#endif