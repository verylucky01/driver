/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_URMA_COMM_H
#define PROF_URMA_COMM_H
#include "prof_common.h"
#include "urma_types.h"
#include "prof_h2d_kernel_msg.h"

#define TRS_JFS_MAX_DEPTH 2048
#define TRS_JFC_MAX_DEPTH 2048
#define TRS_JFR_MAX_DEPTH 2048

#define PROF_URMA_MAX_DEVNUM     DEV_NUM
#define PROF_URMA_DEVID_INVALID  PROF_URMA_MAX_DEVNUM
#define PROF_URMA_PRIORITY_LOW 13U

struct prof_urma_chan_info {
    uint32_t dev_id;
    uint32_t chan_id;
    urma_jfs_cfg_t jfs_cfg;
    urma_jfs_t *jfs;
    urma_jfr_cfg_t jfr_cfg;
    urma_jfr_t *jfr;
    urma_jetty_t *jetty;
    char *local_r_ptr;
    char *user_buff;
    uint32_t user_buff_len;
    urma_target_seg_t *local_r_ptr_tseg;
    urma_target_seg_t *user_buff_tseg;
    urma_target_jetty_t *tjetty;
    urma_target_seg_t *r_ptr_tseg;
};

struct prof_urma_info {
    uint32_t init_flag;
    pthread_t recv_thread;
    int recv_thread_status;

    urma_jfce_t *jfce;
    urma_device_t *urma_dev;
    urma_context_t *urma_ctx;
    urma_token_id_t *token_id;
    urma_token_t token;
    uint32_t eid_index;
};

struct prof_urma_start_para {
    uint64_t data_buff_addr;
    uint32_t data_buff_len;
    uint64_t rptr_addr;
    uint32_t rptr_size;
};

struct prof_urma_info *prof_get_urma_info(uint32_t dev_id);

struct prof_urma_chan_info *prof_urma_chan_info_creat(uint32_t dev_id, uint32_t chan_id);
void prof_urma_chan_info_destroy(struct prof_urma_chan_info *urma_chan_info);

drvError_t prof_urma_remote_info_import(struct prof_urma_info *urma_info,struct prof_urma_chan_info *urma_chan_info,
    struct prof_start_sync_msg *sync_msg);
void prof_urma_remote_info_unimport(struct prof_urma_info *urma_info, struct prof_urma_chan_info *urma_chan_info);

drvError_t prof_urma_local_seg_register(struct prof_urma_start_para *urma_start_para, struct prof_urma_info *urma_info,
    struct prof_urma_chan_info *urma_chan_info);
void prof_urma_local_seg_unregister(struct prof_urma_chan_info *urma_chan_info);

drvError_t prof_urma_write_remote_rptr(uint32_t dev_id, uint32_t chan_id, struct prof_urma_chan_info *urma_chan_info);
drvError_t prof_urma_post_jetty_recv_wr(urma_jetty_t *jetty, urma_target_seg_t *tseg, uint64_t user_ctx, uint32_t depth);
drvError_t prof_urma_post_recv(struct prof_urma_chan_info *urma_chan_info, uint32_t depth);
#endif
