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

#ifndef _HDCDRV_MEM_H_
#define _HDCDRV_MEM_H_

#include "hdcdrv_mem_com.h"

struct hdcdrv_mem_pool *find_mem_pool(int pool_type, int devid, int len);
int alloc_mem(struct hdcdrv_mem_pool *pool, void **buf, dma_addr_t *addr, u32 *offset, struct list_head *wait_head);
#ifdef CFG_FEATURE_MIRROR
struct mirror_alloc_para {
    int dev_id;
    int len;
    struct list_head *wait_head;
    int service_type;
};
extern int alloc_mem_page(int pool_type, struct mirror_alloc_para para, void **buf, dma_addr_t *addr, int *mem_id);
extern int get_pool_type(int data_len);
extern void free_mem_page(void *buf, int mem_id);
#endif
extern void free_mem(void *buf);
extern int alloc_mem_pool(int pool_type, int dev_id, u32 segment, u32 num);
extern void free_mem_pool(int pool_type, int dev_id, u32 segment);

extern void hdcdrv_mem_block_head_init(void *buf, dma_addr_t addr, int pool_type, int dev_id, u32 segment);
extern int hdcdrv_mem_block_head_check(void *buf);
extern int hdcdrv_init_mem_pool(u32 dev_id);
extern void hdcdrv_uninit_mem_pool(u32 dev_id);
extern int hdcdrv_mem_block_capacity(void);
extern void hdcdrv_recycle_mem_work(struct work_struct *p_work);

struct device* hdcdrv_get_pdev_dev(int dev_id);
void *hdcdrv_kvmalloc(size_t size, int level);
int hdcdrv_map_reserve_mem(struct hdccom_mem_init *init_mem, int pool_type, u32 segment, u32 num);
void hdcdrv_unmap_reserve_mem(struct hdcdrv_mem_pool *pool, struct device *dev);
int hdcdrv_init_alloc_mem_pool(u32 dev_id, u32 small_packet_num, u32 huge_packet_num);
void hdcdrv_uninit_alloc_mem_pool(u32 dev_id);
int hdcdrv_init_reserve_mem_pool(u32 dev_id, u32 small_packet_num, u32 huge_packet_num);
void hdcdrv_uninit_reserve_mem_pool(u32 dev_id);

#endif
