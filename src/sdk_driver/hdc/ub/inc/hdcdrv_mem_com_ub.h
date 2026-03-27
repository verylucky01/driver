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
#ifndef _HDCDRV_MEM_COM_H_
#define _HDCDRV_MEM_COM_H_

#include "ka_type.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "hdcdrv_adapt_ub.h"

enum hdcdrv_mem_pool_state {
    HDCDRV_MEM_POOL_DISABLE = 0, //  pool can not be used
    HDCDRV_MEM_POOL_IDLE = 1,   //  pool is not used
    HDCDRV_MEM_POOL_BUSY = 2,   //  pool is in use
};

#define HDCDRV_MEM_POOL_BLOCK_NUM 16

#define HDCDRV_MEM_POOL_NUM 24

// If the value of this macro needs to be changed, the macro with the same name in user mode also needs to be changed.
#define HDCDRV_UB_MEM_POOL_LEN (4 * 1024 * 16 * 2) // 4K(block_size) * 16(block_num) * 2(pool num)

struct hdcdrv_mem_pool {
    ka_page_t *page;
    unsigned long long va;      // user va, mmap from user
    unsigned long long pid;
    ka_pid_t vnr;
};

struct hdcdrv_mem_pool_list_node {
    int valid;
    struct hdcdrv_mem_pool pool;
    int session_id;
    void *ctx;
};

int hdcdrv_check_va(const void *ctx, ka_vm_area_struct_t *vma, unsigned long long user_va, u32 dev_id);
int hdcdrv_remap_mem_pool_va(const void *ctx, struct hdcdrv_mem_pool *mem_pool, int dev_id, unsigned long long user_va);
int hdcdrv_unmap_mem_pool_va(const void *ctx, struct hdcdrv_mem_pool *mem_pool, u32 dev_id);

// used for emu_st
#ifdef EMU_ST
ka_vm_area_struct_t *hdc_find_vma_from_stub(ka_mm_struct_t *mm, unsigned long addr);
int hdc_remap_pfn_range_stub(ka_vm_area_struct_t *vma, unsigned long from, unsigned long pfn,
    unsigned long size, ka_pgprot_t prot);
#endif

#endif // _HDCDRV_MEM_COM_H_