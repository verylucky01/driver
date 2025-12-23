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
#ifndef PROF_H2D_KERNEL_MSG_H
#define PROF_H2D_KERNEL_MSG_H
#include "ascend_hal_type.h"
#include "pbl_prof_interface_cmd.h"
#include "prof_command_base.h"


#define PROF_EID_SIZE (16) /* #define UBCORE_EID_SIZE (16), #define URMA_EID_SIZE (16) */

enum prof_hdc_msg_type {
    PROF_HDC_CMD_GET_CHANNEL,
    PROF_HDC_CMD_START,
    PROF_HDC_CMD_STOP,
    PROF_HDC_DATA,
    PROF_HDC_CLOSE_SESSION,
    PROF_HDC_DATA_FLUSH,
    PROF_HDC_CMD_MAX
};

struct prof_channel_list_sync_msg {
    struct prof_channel_list channel_list;
};

struct prof_start_urma_msg {
    unsigned int jetty_id;
    unsigned int token_id;
    unsigned int token_val;
    unsigned int channel_id;
    uint8_t eid_raw[PROF_EID_SIZE];
    unsigned int eid_index;
    char *data_buff;
    unsigned int data_buff_len;
    unsigned int sample_period;
    char user_data[PROF_USER_DATA_LEN];
    unsigned int user_data_size;
};

struct prof_start_sync_msg {
    unsigned int jetty_id;
    unsigned int eid;
    uint8_t eid_raw[PROF_EID_SIZE];
    unsigned int token_id;
    unsigned int token_val;
    unsigned int *r_ptr; // This value is the virtual addr in kernel mode, which needs to be mapped out using mmap.
};

struct prof_stop_urma_msg {
    unsigned int channel_id;
    char report[PROF_USER_DATA_LEN];
    unsigned int user_data_size;
};

struct prof_kernel_dfx_info {
    uint32_t buf_len;
    uint32_t r_ptr;
    uint32_t w_ptr;
    UINT64 data_in;
    UINT64 data_out;
    uint32_t cq1_cnt;
    uint32_t poll_enque_cnt;
    uint32_t read_cnt;
    uint32_t sample_fail_cnt;
    uint32_t data_dequeue_fail_cnt;
    uint32_t data_enqueue_fail_cnt;
};

struct prof_stop_sync_msg {
    struct prof_kernel_dfx_info dfx_info;
};

struct prof_flush_urma_msg {
    unsigned int channel_id;
};

struct prof_flush_sync_msg {
    unsigned int data_len;
};

struct prof_hdc_msg {
    int msg_type;
    int ret_val;
    uint32_t cmd_verify;
    uint32_t channel_id;
    uint32_t data_len;
    uint32_t rsv;
    unsigned char data[];
};

struct prof_hdc_start_para {
    uint32_t channel_type;          /* for ts and other device */
    uint32_t buf_len;               /* buffer size */
    uint32_t sample_period;
    char user_data[PROF_USER_DATA_LEN]; /* ts data */
    uint32_t user_data_size;        /* user data's size */
};

#endif
