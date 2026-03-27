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

#ifndef _UDIS_DEFINE_H_
#define _UDIS_DEFINE_H_

#include "ka_list_pub.h"
#include "ka_common_pub.h"
#include "ka_memory_pub.h"
#include "comm_kernel_interface.h"
#include "udis.h"

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

#define UDIS_SEGMENT_MAX_LEN 128

struct udis_info_stu {
    char name[UDIS_MAX_NAME_LEN];
    unsigned int acc_ctrl;
    UDIS_UPDATE_TYPE update_type;
    unsigned long last_update_time;
    char reserved[12]; // reserved for head
    unsigned int data_len;
    char data[UDIS_MAX_DATA_LEN];
};

struct udis_node {
    ka_list_head_t list;
    ka_dma_addr_t host_dma_addr;
    ka_dma_addr_t dev_dma_addr;
    va_addr_t host_va_addr;
    va_addr_t dev_va_addr;
    UDIS_ADDR_ATTR dev_va_addr_attr;
    void *host_segment;
    void *device_segment_import;
    char device_segment[UDIS_SEGMENT_MAX_LEN];
    size_t host_segment_len;
    size_t device_segment_import_len;
    u32 token_value;
    size_t device_segment_len;
    unsigned int acc_ctrl;
    unsigned int data_len;
    UDIS_UPDATE_TYPE update_type;
    UDIS_MODULE_TYPE module_type;
    char name[UDIS_MAX_NAME_LEN];
};

struct udis_link_ub_node {
    void *host_segment;
    void *device_segment_import;
    size_t host_segment_len;
    size_t device_segment_import_len;
    va_addr_t host_va_addr;
    u32 data_len;
};

struct udis_link_nodes {
    union {
        struct devdrv_dma_node *dma_nodes;
        struct udis_link_ub_node *ub_nodes;
    } node;
    unsigned int node_num;
    unsigned int capacity;
};

enum udis_dev_state {
    UDIS_DEV_UNINIT = 0,
    UDIS_DEV_CHANREADY,
    UDIS_DEV_READY,
    UDIS_DEV_HOTRESET,
    UDIS_DEV_PREHOTRESET,
    UDIS_DEV_HEART_BEAT_LOSS,
    UDIS_DEV_STATE_MAX
};

enum udis_data_status {
    UDIS_DATA_VALID = 0,
    UDIS_DATA_NEEDS_UPDATE,
    UDIS_DATA_UPDATE_IMMEDIATELY,
    UDIS_DATA_INVALID
};

enum udis_search_scope {
    UDIS_INFO_ALL_SPACE = 0,
    UDIS_INFO_DISCRE_SPACE = 1
};

struct udis_ctrl_block {
    ka_list_head_t addr_list[UPDATE_TYPE_MAX];
    ka_rw_semaphore_t addr_list_lock;
    struct udis_info_stu *udis_info_buf;
    ka_rw_semaphore_t udis_info_lock;
    ka_dma_addr_t udis_info_buf_dma;
    enum udis_dev_state state;
};

void udis_release_node(unsigned int udevid, struct udis_node *addr_node);
int period_link_task_init(unsigned int udevid);
void period_link_task_uninit(unsigned int udevid);
int udis_sync_copy(unsigned int udevid, struct udis_ctrl_block *udis_cb, const struct udis_node *node);
int udis_update_info(unsigned int udevid, UDIS_MODULE_TYPE module_id, const char *name);
#endif
