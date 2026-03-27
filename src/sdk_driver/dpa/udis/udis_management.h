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

#ifndef _UDIS_MANAGEMENT_H_
#define _UDIS_MANAGEMENT_H_

#include "ascend_platform.h"
#include "udis_define.h"
#include "ka_system_pub.h"

#define UDIS_BAD_DMA_ADDR (~0UL)
#define UDIS_BAD_VA_ADDR (~0UL)
#define UDIS_UNIFIED_MODULE_INFO "ALL"
#define UDIS_NAME_OFFSET (256)

#ifdef DRV_HOST
#define UDIS_MAX_NO_UPDATE_MSEC 250
#define UDIS_MODULE_OFFSET (64 * 1024)
#define UDIS_UNIFIED_MODULE_OFFSET (UDIS_MODULE_OFFSET / 2)
#define UDIS_UNIFIED_NAME_MAX (UDIS_UNIFIED_MODULE_OFFSET / UDIS_NAME_OFFSET)
#define UDIS_DEVICE_UDEVID_MAX 1124
#else
#define UDIS_MODULE_OFFSET (32 * 1024)
#define UDIS_DEVICE_UDEVID_MAX 64
#endif

#define UDIS_DEVICE_SPACE_SIZE (512 * 1024)
/*HCCS credit uses the first 40 bytes. To achieve 4K alignment for the DMA address, an offset of 4K is added*/
#define UDIS_INFO_MEM_ADDR        (BASE_DEVMNG_INFO_MEM_ADDR + 0x1000)
#define UDIS_NAME_NUM_MAX (UDIS_MODULE_OFFSET / UDIS_NAME_OFFSET)
#define UDIS_MODULE_LEN UDIS_MODULE_OFFSET

#ifndef KA_NSEC_PER_MSEC
#define KA_NSEC_PER_MSEC 1000000L
#endif
struct udis_ctrl_block *udis_get_ctrl_block(unsigned int udevid);
int udis_set_ctrl_block(unsigned int udevid, struct udis_ctrl_block *ctrl_block);
unsigned long long udis_get_link_timestamp(unsigned int udevid);
void  udis_set_link_timestamp(unsigned int udevid, unsigned long long timestamp);
struct devdrv_common_msg_client *udis_get_common_msg_client(void);
void udis_cb_rwlock_init(unsigned int udevid);
void udis_cb_write_lock(unsigned int udevid);
void udis_cb_write_unlock(unsigned int udevid);
void udis_cb_read_lock(unsigned int udevid);
void udis_cb_read_unlock(unsigned int udevid);
int udis_alloc_info_block(unsigned int udevid, const struct udis_node *addr_node, ka_dma_addr_t *host_addr);
void udis_free_info_block(unsigned int udevid, struct udis_node *addr_node);
struct udis_link_nodes *udis_get_link_nodes(unsigned int udevid);
int udis_link_nodes_init(unsigned int udevid);
void udis_link_nodes_uninit(unsigned int udevid);
int udis_link_nodes_scale_up(unsigned int udevid);
struct udis_node *udis_addr_list_find_node(const struct udis_ctrl_block *udis_cb, UDIS_MODULE_TYPE module_type,
    const char *name);
int udis_addr_list_add_node(unsigned int udevid, struct udis_ctrl_block *udis_cb,
    const struct udis_node *addr_node);
void udis_addr_list_remove_node(unsigned int udevid, struct udis_ctrl_block *udis_cb,
    const struct udis_node *addr_node);
void udis_cmd_init(void);
void udis_cmd_exit(void);
#endif
