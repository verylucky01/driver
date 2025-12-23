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

#ifndef _HDCDRV_CMD_MSG_H_
#define _HDCDRV_CMD_MSG_H_

#define HDCDRV_CTRL_MEM_REGISTER_MAX_LEN (2 * 1024 * 1024)  /* ctrl mem register mem len check */
#define HDCDRV_CTRL_MEM_SEND_MAX_LEN (252 * 1024)  /* ctrl mem send len check */
#define HDCDRV_MEM_MIN_LEN (4 * 1024)
#define HDCDRV_MEM_CACHE_LIMIT (64 * 1024)
#define HDCDRV_MEM_MAX_NUM (HDCDRV_MEM_MAX_LEN / HDCDRV_MEM_MIN_LEN)

#define HDCDRV_DEFAULT_PM_FID 0
#define HDCDRV_DEFAULT_LOCAL_FID 0

struct hdcdrv_dma_mem {
    unsigned long long addr;
    unsigned int len;
    unsigned int resv;
};

struct hdcdrv_ctrl_msg_sync_mem_info {
    int type;
    int error_code;
    int flag;
    int phy_addr_num;
    int mem_type;
    unsigned int alloc_len;
    unsigned long long hash_va;
    long long pid;
#ifdef CFG_FEATURE_HDC_REG_MEM
    unsigned int align_size;
    unsigned int register_offset;
    unsigned long long user_va;
#endif
    unsigned int reserved[8];
    struct hdcdrv_dma_mem mem[];
};

enum hdc_dfx_print_type {
    HDC_DFX_PRINT_IN_SYSFS = 0,
    HDC_DFX_PRINT_IN_LOG = 1,
};

struct hdcdrv_sysfs_ctrl_msg_head {
    int type;
    int error_code;
    unsigned int msg_len;
    unsigned int para;
    enum hdc_dfx_print_type print_type;
};

struct hdcdrv_sysfs_ctrl_msg {
    struct hdcdrv_sysfs_ctrl_msg_head head;
    char reserved[4];
    char data[];
};

/* ctrl msg */
struct hdcdrv_ctrl_msg_sync {
    int segment;
    int peer_dev_id;
};

struct hdcdrv_ctrl_msg_connect {
    int service_type;
    int client_session;
    unsigned int fast_chan_id;
    unsigned int normal_chan_id;
    int run_env;
    int euid;
    int uid;
    int root_privilege;
    unsigned int fid;
    unsigned int unique_val;
    unsigned long long local_pid;
    unsigned long long peer_pid;
};

struct hdcdrv_ctrl_msg_connect_reply {
    int client_session;
    int server_session;
    int run_env;
    unsigned int fid;
    unsigned int unique_val;
    unsigned long long local_pid;
    unsigned long long peer_pid;
    unsigned int fast_chan_id;
    unsigned int normal_chan_id;
};

struct hdcdrv_ctrl_msg_close {
    int local_session;
    int remote_session;
    int session_close_state;
    unsigned int fid;
    unsigned int unique_val;
};

struct hdcdrv_ctrl_msg_chan_set {
    unsigned int normal_chan_num;
};

struct hdcdrv_ctrl_msg_get_stat {
    unsigned int msg_len;
    unsigned int para;
};

struct hdcdrv_ctrl_msg {
    int type;
    int error_code;
    union {
        struct hdcdrv_ctrl_msg_sync sync_msg;
        struct hdcdrv_ctrl_msg_connect connect_msg;
        struct hdcdrv_ctrl_msg_connect_reply connect_msg_reply;
        struct hdcdrv_ctrl_msg_close close_msg;
        struct hdcdrv_ctrl_msg_chan_set chan_set_msg;
        struct hdcdrv_ctrl_msg_get_stat stat;
    };
} __attribute__((aligned(128)));

#endif
