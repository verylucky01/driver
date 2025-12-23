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
 
#ifndef PEER_MEM_H
#define PEER_MEM_H
 
#define IB_PEER_MEMORY_NAME_MAX 64
#define IB_PEER_MEMORY_VER_MAX 16
#define NPU_SMALL_PAGE_SIZE    4096        // 4K 小页
#define NPU_LARGE_PAGE_SIZE    2097152     // 2M 大页
 
struct peer_memory_client {
    char name[IB_PEER_MEMORY_NAME_MAX];
    char version[IB_PEER_MEMORY_VER_MAX];
 
    int (*acquire)(unsigned long, size_t, void *, char *, void **); 
    int (*get_pages)(unsigned long, size_t, int, int, struct sg_table *, void *, u64); 
    int (*dma_map)(struct sg_table *, void *, struct device *, int, int *); 
    int (*dma_unmap)(struct sg_table *, void *, struct device *dma_device); 
    void (*put_pages)(struct sg_table *, void *); 
    unsigned long (*get_page_size)(void *); 
    void (*release)(void *);
};
 
struct peer_memory_client_ex {
        struct peer_memory_client client;
        size_t ex_size;
        u32 flags;
};
 
struct npu_mem_context {
    u64 pad1;
    spinlock_t lock;                    // 保护 page_table, sg_allocated 等
    struct p2p_page_table *page_table;
    u64 core_context;
    u64 page_virt_start;
    u64 page_virt_end;
    size_t mapped_size;
    unsigned long npages;
    unsigned long page_size;             // 原有字段
    struct task_struct *callback_task;
    int sg_allocated;
    struct sg_table sg_head;
    struct device *dma_device;
    u64 pad2;
 
    // 新增：替代原全局变量
    unsigned int page_shift;
    u64 page_offset;
    u64 page_mask;
};
 
typedef int (*invalidate_peer_memory)(void *reg_handle, u64 core_context);

void *
ib_register_peer_memory_client(const struct peer_memory_client *peer_client,
                   invalidate_peer_memory *invalidate_callback);
void ib_unregister_peer_memory_client(void *reg_handle);
 
#endif