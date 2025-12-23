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
#include <linux/kprobes.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/module.h>
#include <linux/types.h>
#include "peer_mem.h"
#include "ndr_log.h"
#include "ascend_kernel_hal.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_kernel_def_pub.h"

#define NPU_MEM_CONTEXT_MAGIC ((u64)0xF1F4F1D0FEF0DAD0ULL)

static void *reg_handle = NULL;

invalidate_peer_memory mem_invalidate_callback;

static bool npu_mem_context_valid(struct npu_mem_context *ctx)
{
    if (!ctx)
        return false;
    if (ctx->pad1 != NPU_MEM_CONTEXT_MAGIC || ctx->pad2 != NPU_MEM_CONTEXT_MAGIC)
        return false;
    return true;
}

// 空实现满足接口回调要求
static void npu_mem_dummy(void *data)
{
    ndr_event("npu_mem_dummy acquire failed callbak\n");
}

static void npu_get_p2p_free_callback(void *data)
{
    struct npu_mem_context *npu_mem_context = (struct npu_mem_context *)data;
    struct p2p_page_table *saved_page_table = NULL;
    struct sg_table *saved_sg_head = NULL;
    struct scatterlist *sg;
    bool need_release = false;
    int i;
    int ret;
    unsigned long flags;
    int unmapped_count = 0;

    if (!npu_mem_context_valid(npu_mem_context)) {
        ndr_err("npu_get_p2p_free_callback invalid context or magic\n");
        return;
    }

    if (npu_mem_context->callback_task == current) {
        ndr_event("npu_get_p2p_free_callback Skipping unmap or put pages already execute \n");
        return;
    }

    ka_task_spin_lock_irqsave(&npu_mem_context->lock, flags);
    // 未在资源释放中，保存变量释放资源
    if (npu_mem_context->callback_task == NULL && npu_mem_context->page_table
        && npu_mem_context->sg_allocated) {
        saved_page_table = npu_mem_context->page_table;
        saved_sg_head = &npu_mem_context->sg_head;
        npu_mem_context->page_table = NULL;  // 在锁内清空
        npu_mem_context->sg_allocated = 0;
        need_release = true;
        npu_mem_context->callback_task = current;
    }
    ka_task_spin_unlock_irqrestore(&npu_mem_context->lock, flags);

    // 资源已经释放，直接返回
    if (need_release == false || saved_page_table == NULL || saved_sg_head == NULL) {
        ndr_info("npu_get_p2p_free_callback  umap and put page already  %llx\n",
                 npu_mem_context->core_context);
        return;
    }
    /* 触发内存无效回调 */
    if (mem_invalidate_callback && reg_handle) {
        ndr_event("npu_get_p2p_free_callback: Invoking invalidate callback for ctx 0x%llx\n",
                  npu_mem_context->core_context);
        // 调用RDMA框架提供的内存失效回调函数，通知上层清理相关的RDMA内存映射资源
        (*mem_invalidate_callback)(reg_handle, npu_mem_context->core_context);
    }

    // 解除DMA映射    
    ka_base_for_each_sg(saved_sg_head->sgl, sg, saved_sg_head->nents, i) {
        dma_addr_t dma_addr = ka_mm_sg_dma_address(sg);
        size_t len = ka_mm_sg_dma_len(sg);
        if (dma_addr) {
            dma_unmap_resource(npu_mem_context->dma_device, dma_addr, len, DMA_BIDIRECTIONAL, 0);
            unmapped_count++;
        }
    }

    ka_base_sg_free_table(&npu_mem_context->sg_head);

    /* 清理 page_table */
    if (saved_page_table) {
        ret = hal_kernel_p2p_put_pages(saved_page_table);
        if (ret) {
            ndr_err("npu_get_p2p_free_callback: hal_kernel_p2p_put_pages failed: %d\n", ret);
        }
    }

    npu_mem_context->callback_task = NULL;
    return;
}



/* acquire return code: 1 mine, 0 - not mine */
static int npu_acquire(unsigned long addr, size_t size, void *peer_mem_private_data, char *peer_mem_name,
    void **client_context)
{
    int ret = 0;
    struct npu_mem_context *npu_mem_context;
    npu_mem_context = kzalloc(sizeof(*npu_mem_context), KA_GFP_KERNEL);
    if (!npu_mem_context) {
        ndr_err("npu_acquire kzalloc failed.\n");
        return 0;
    }

    // 仅用于校验接口可用性
    ret = hal_kernel_p2p_get_pages(addr, size, npu_mem_dummy,
                                       npu_mem_context, &npu_mem_context->page_table);
    if (ret != 0) {
        ndr_err("npu_acquire: failed to get pages\n");
        goto err;
    }

    if (!npu_mem_context->page_table) {
        ndr_err("npu_acquire page_table is NULL after get_pages.\n");
        goto err;
    }

    // 直接使用页表（已知有效）
    npu_mem_context->page_size = npu_mem_context->page_table->page_size;
    npu_mem_context->page_shift = fls64(npu_mem_context->page_size) - 1;
    npu_mem_context->page_offset = npu_mem_context->page_size - 1;
    npu_mem_context->page_mask = ~(npu_mem_context->page_offset);

    // 初始化其余字段
    npu_mem_context->pad1 = NPU_MEM_CONTEXT_MAGIC;
    npu_mem_context->page_virt_start = addr & npu_mem_context->page_mask;
    npu_mem_context->page_virt_end = (addr + size + npu_mem_context->page_offset) & npu_mem_context->page_mask;
    npu_mem_context->mapped_size = npu_mem_context->page_virt_end - npu_mem_context->page_virt_start;
    npu_mem_context->pad2 = NPU_MEM_CONTEXT_MAGIC;
    ka_task_spin_lock_init(&npu_mem_context->lock);

    ret = hal_kernel_p2p_put_pages(npu_mem_context->page_table);
    npu_mem_context->page_table = NULL;  // 置空
    if (ret < 0) {
        ndr_err("npu_acquire hal_kernel_p2p_put_pages failed with error %d\n", ret);
        goto err;
    }

    *client_context = npu_mem_context;
    return 1;

err:
    ka_mm_kfree(npu_mem_context);
    return 0;
}

static int npu_get_pages(unsigned long addr, size_t size, int write, int force, struct sg_table *sg_head,
    void *client_context, u64 off)
{
    int ret;
    struct npu_mem_context *npu_mem_context = (struct npu_mem_context *)client_context;
    if (!npu_mem_context_valid(npu_mem_context)) {
        ndr_err("npu_get_pages npu_mem_context check failed!\n");
        return -EINVAL;
    }

    npu_mem_context->core_context = off;
    ret = hal_kernel_p2p_get_pages(addr, size, npu_get_p2p_free_callback, npu_mem_context,
        &npu_mem_context->page_table);
    if (ret < 0) {
        ndr_err("npu_get_pages error %d while calling hal_kernel_p2p_get_pages()\n", ret);
        return ret;
    }
    return 0;
}

static void fill_sg_entry_failed(struct sg_table *sgt, struct device *dev, int i)
{
    int j;
    struct scatterlist *tmp_sg;

    ka_base_for_each_sg(sgt->sgl, tmp_sg, i, j) {
        if (ka_mm_sg_dma_address(tmp_sg)) {
            dma_unmap_resource(dev, ka_mm_sg_dma_address(tmp_sg), ka_mm_sg_dma_len(tmp_sg), DMA_BIDIRECTIONAL, 0);
        }
    }
    ka_base_sg_free_table(sgt);
    sgt->sgl = NULL;
}


/**
 * 填充 scatterlist 条目：将 NPU 物理地址通过 IOMMU 映射为 IOVA
 */
static int fill_sg_entry(struct sg_table *sgt, struct p2p_page_table *pt,
    struct device *dev, struct npu_mem_context *ctx)
{
    struct scatterlist *sg;
    int i;
    dma_addr_t dma_addr;

    if (!pt || pt->page_num == 0) {
        ndr_err("fill_sg_entry p2p_page_table parameter is null\n");
        return -EINVAL;
    }

    ka_base_for_each_sg(sgt->sgl, sg, pt->page_num, i) {
        u64 npu_pa = pt->pages_info[i].pa;

        // 添加边界检查
        if (i >= pt->page_num) {
            ndr_err("fill_sg_entry index %d exceeds page_num %d\n", i, pt->page_num);
            goto err;
        }

        if (!npu_pa) {
            ndr_err("fill_sg_entry Invalid NPU physical address at index %d\n", i);
            goto err;
        }

        // 检查地址对齐
        if (npu_pa & ~ctx->page_mask) {
            ndr_err("fill_sg_entry Unaligned NPU physical address 0x%llx (mask=0x%llx) at index %d\n",
                    npu_pa, ctx->page_mask, i);
            goto err;
        }

        dma_addr = dma_map_resource(dev, npu_pa, ctx->page_size, DMA_BIDIRECTIONAL, 0);
        if (dma_mapping_error(dev, dma_addr)) {
            ndr_err("fill_sg_entry dma_map_resource failed for npu_pa=0x%llx\n", npu_pa);
            goto err;
        }

        ka_mm_sg_dma_address(sg) = dma_addr;
        ka_mm_sg_dma_len(sg) = ctx->page_size;
    }
    return 0;

err:
    fill_sg_entry_failed(sgt, dev, i);
    return -EIO;
}

/**
 * DMA 映射：将获取的 NPU 内存页映射为当前设备可访问的 IOVA
 */
static int npu_dma_map(struct sg_table *sg_head, void *client_context,
    struct device *dma_device, int dmasync, int *nmap)
{
    int ret = 0;
    struct npu_mem_context *npu_mem_context = (struct npu_mem_context *)client_context;
    struct p2p_page_table *pt = npu_mem_context ? npu_mem_context->page_table : NULL;
    *nmap = 0;

    if (!npu_mem_context_valid(npu_mem_context)) {
        ndr_err("npu_dma_map npu_mem_context Invalid parameters\n");
        return -EINVAL;
    }

    if (!pt || !pt->pages_info || !sg_head || !dma_device) {
        ndr_err("npu_dma_map page_table Invalid parameters\n");
        return -EINVAL;
    }

    npu_mem_context->dma_device = dma_device;
    npu_mem_context->npages = npu_mem_context->mapped_size >> npu_mem_context->page_shift;

    if (npu_mem_context->npages != pt->page_num) {
        ndr_err("npu_dma_map Page count mismatch\n");
        return -EINVAL;
    }

    ret = sg_alloc_table(sg_head, pt->page_num, KA_GFP_KERNEL);
    if (ret) {
        ndr_err("npu_dma_map sg_alloc_table failed with error %d\n", ret);
        return ret;
    }

    ret = fill_sg_entry(sg_head, pt, dma_device, npu_mem_context);
    if (ret != 0) {
        ndr_err("npu_dma_map: fill_sg_entry failed with error %d\n", ret);
        return ret;
    }

    *nmap = pt->page_num;
    npu_mem_context->sg_allocated = 1;
    npu_mem_context->sg_head = *sg_head;
    return 0;
}

/**
 * DMA 反映射：取消 IOVA 映射
 */
static int npu_dma_unmap(struct sg_table *sg_head, void *client_context, struct device *dma_device)
{
    struct npu_mem_context *npu_mem_context = (struct npu_mem_context *)client_context;
    struct scatterlist *sg;
    int i;
    unsigned long flags;
    bool need_unmap = false;

    if (!npu_mem_context_valid(npu_mem_context)) {
        ndr_err("npu_dma_unmap Invalid parameters: ctx=%p\n", npu_mem_context);
        return -EINVAL;
    }

    if (!sg_head || !dma_device) {
        ndr_err("npu_dma_unmap Invalid parameters: ctx=%p, sg_head=%p, device %s\n",
                npu_mem_context, sg_head, dev_name(dma_device));
        return -EINVAL;
    }

    if (npu_mem_context->callback_task == current) {
        ndr_event("npu_dma_unmap Skipping unmap in callback context\n");
        return 0;
    }

    // 加锁检查是否可进入释放流程
    ka_task_spin_lock_irqsave(&npu_mem_context->lock, flags);
    if (npu_mem_context->callback_task == NULL &&          // 无人正在释放
        npu_mem_context->sg_allocated &&                   // 资源确实已分配
        sg_head->sgl == npu_mem_context->sg_head.sgl) {    // 确认是同一个sg表
        need_unmap = true;
        npu_mem_context->sg_allocated = 0;
        npu_mem_context->callback_task = current;
    }
    ka_task_spin_unlock_irqrestore(&npu_mem_context->lock, flags);
 
    if (!need_unmap) {
        ndr_event("npu_dma_unmap  SG already unmapped or mismatch\n");
        return 0;
    }

    if (sg_head->sgl && sg_head->nents > 0) {
        ka_base_for_each_sg(sg_head->sgl, sg, sg_head->nents, i) {
            dma_addr_t dma_addr = ka_mm_sg_dma_address(sg);
            size_t len = ka_mm_sg_dma_len(sg);
            if (dma_addr) {
                dma_unmap_resource(dma_device, dma_addr, len, DMA_BIDIRECTIONAL, 0);
            }
        }
    }

    ka_base_sg_free_table(&npu_mem_context->sg_head);
    npu_mem_context->callback_task = NULL;
    return 0;
}

static void npu_put_pages(struct sg_table *sg_head, void *client_context)
{
    struct npu_mem_context *npu_mem_context = (struct npu_mem_context *)client_context;
    struct p2p_page_table *local_page_table = NULL;
    unsigned long flags;
    int ret;

    if (!npu_mem_context_valid(npu_mem_context)) {
        ndr_err("npu_put_pages Invalid context\n");
        return;
    }

    if (npu_mem_context->callback_task == current) {
        ndr_event("npu_put_pages Skipping put_pages in callback context\n");
        return;
    }

    // 加锁保护
    ka_task_spin_lock_irqsave(&npu_mem_context->lock, flags);
    if (npu_mem_context->callback_task == NULL && npu_mem_context->page_table) {
        local_page_table = npu_mem_context->page_table;
        npu_mem_context->page_table = NULL;  // 在锁内清空
        npu_mem_context->callback_task = current;
    }
    ka_task_spin_unlock_irqrestore(&npu_mem_context->lock, flags);

    if (local_page_table == NULL) {
        ndr_err("npu_put_pages Skipping put_pages page table is already null\n");
        return;
    }
 
    ret = hal_kernel_p2p_put_pages(local_page_table);
    if (ret) {
        ndr_err("npu_put_pages: hal_kernel_p2p_put_pages failed: %d\n", ret);
    }

    npu_mem_context->callback_task = NULL;
    return;
}

static unsigned long npu_get_page_size(void *client_context)
{
    struct npu_mem_context *npu_mem_context = (struct npu_mem_context *)client_context;
 
    if (!npu_mem_context_valid(npu_mem_context)) {
        ndr_err("npu_get_page_size: invalid context or magic\n");
        return 0;
    }

    return npu_mem_context->page_size;
}

static void npu_release(void *client_context)
{
    struct npu_mem_context *npu_mem_context = (struct npu_mem_context *)client_context;
    if (!npu_mem_context_valid(npu_mem_context)) {  // Magic 校验
        ndr_err("npu_release invalid context or magic\n");
        return;
    }
    // 如果 page_table 还在，强制释放
    if (npu_mem_context->page_table) {
        hal_kernel_p2p_put_pages(npu_mem_context->page_table);
        npu_mem_context->page_table = NULL;
    }

    if (npu_mem_context->sg_allocated) {
        ka_base_sg_free_table(&npu_mem_context->sg_head);
        npu_mem_context->sg_allocated = 0;
    }

    ka_mm_kfree(npu_mem_context);
    return;
}

// 结构体绑定函数
static struct peer_memory_client_ex __attribute__((unused)) npu_mem_client_ex = {
    .client = {
        .name = "npu_mem",
        .acquire = npu_acquire,
        .get_pages = npu_get_pages,
        .dma_map = npu_dma_map,
        .dma_unmap = npu_dma_unmap,
        .put_pages = npu_put_pages,
        .get_page_size = npu_get_page_size,
        .release = npu_release,
    }
};
static int __init npu_peer_mem_init(void)
{
#ifdef IB_PEER_MEM_SYMBOLS_PRESENT
    reg_handle = ib_register_peer_memory_client(&npu_mem_client_ex.client, &mem_invalidate_callback);
    if (!reg_handle) {
        ndr_err("ib_register_peer_memory_client call failed\n");
        return -EINVAL;
    }
    ndr_info("npu_peer_mem: loaded successfully\n");
    return 0;
#else
    ndr_err("ib_register_peer_memory_client is NULL\n");
    return -EINVAL;
#endif
}
 
static void __exit npu_peer_mem_exit(void)
{
#ifdef IB_PEER_MEM_SYMBOLS_PRESENT
    if (reg_handle) {
        ib_unregister_peer_memory_client(reg_handle);
        ndr_info("npu_peer_mem: unloaded  successfully\n");
    }
#endif
}

ka_module_init(npu_peer_mem_init);
ka_module_exit(npu_peer_mem_exit);

KA_MODULE_LICENSE("GPL");
KA_MODULE_AUTHOR("Huawei Tech. Co., LTD.");
KA_MODULE_DESCRIPTION("PeerMem Module");
KA_MODULE_SOFTDEP("pre: ib_uverbs");  // 确保 ib_uverbs 先加载