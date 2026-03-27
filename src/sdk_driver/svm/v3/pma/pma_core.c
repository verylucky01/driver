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
#include "ka_system_pub.h"
#include "ka_task_pub.h"
#include "ka_common_pub.h"
#include "ka_memory_pub.h"
#include "ka_fs_pub.h"
#include "ka_compiler_pub.h"
#include "ka_sched_pub.h"

#include "pbl_uda.h"
#include "dpa_kernel_interface.h"

#include "svm_kern_log.h"
#include "svm_addr_desc.h"
#include "svm_slab.h"
#include "dbi_kern.h"
#include "pmq_client.h"
#include "pma.h"
#include "pma_ctx.h"
#include "pma_core.h"

static struct pma_mem_node *pma_mem_node_alloc(struct pma_ctx *ctx, u64 va, u64 size,
    void (*free_callback)(void *data), void *data)
{
    struct pma_mem_node *mem_node = NULL;

    mem_node = svm_kvzalloc(sizeof(struct pma_mem_node), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (mem_node == NULL) {
        svm_err("Alloc pma mem node failed. (va=0x%llx; size=%llu)\n", va, size);
        return NULL;
    }

    KA_INIT_LIST_HEAD(&mem_node->node);
    ka_base_kref_init(&mem_node->ref);

    mem_node->udevid = ctx->udevid;
    mem_node->tgid = ctx->tgid;

    mem_node->va = va;
    mem_node->size = size;
    mem_node->free_callback = free_callback;
    mem_node->data = data;

    return mem_node;
}

static void pma_mem_node_free(struct pma_mem_node *mem_node)
{
    svm_kvfree(mem_node);
}

static void pma_mem_node_insert(struct pma_ctx *ctx, struct pma_mem_node *mem_node)
{
    ka_task_down_write(&ctx->rw_sem);
    ka_list_add_tail(&mem_node->node, &ctx->head);
    ctx->mem_node_num++;
    ctx->get_cnt++;
    ka_task_up_write(&ctx->rw_sem);
}

static void _pma_mem_node_erase(struct pma_ctx *ctx, struct pma_mem_node *mem_node)
{
    ctx->mem_node_num--;
    ka_list_del_init(&mem_node->node);
}

int pma_mem_node_erase(struct pma_ctx *ctx, struct pma_mem_node *mem_node)
{
    ka_task_down_write(&ctx->rw_sem);
    ctx->put_cnt++;
    if (ka_list_empty(&mem_node->node) == 1) { /* Check if the node is in the list. */
        ka_task_up_write(&ctx->rw_sem);
        return -ENOENT;
    }

    _pma_mem_node_erase(ctx, mem_node);
    ka_task_up_write(&ctx->rw_sem);
    return 0;
}

static u64 pma_get_dev_pa_seg_page_size(u32 udevid, u64 va, u64 size,
    struct svm_pa_seg *pa_seg, u64 seg_num)
{
    u64 npage_size, hpage_size, gpage_size, i;
    bool is_gpage = true;
    bool is_hpage = true;
    int ret;

    ret = svm_dbi_kern_query_npage_size(udevid, &npage_size);
    if (ret != 0) {
        svm_err("Query npage_size failed. (ret=%d; udevid=%u)\n", ret, udevid);
        return 0;
    }

    ret = svm_dbi_kern_query_hpage_size(udevid, &hpage_size);
    if (ret != 0) {
        svm_err("Query hpage_size failed. (ret=%d; udevid=%u)\n", ret, udevid);
        return 0;
    }

    ret = svm_dbi_kern_query_gpage_size(udevid, &gpage_size);
    if (ret != 0) {
        svm_err("Query gpage_size failed. (ret=%d; udevid=%u)\n", ret, udevid);
        return 0;
    }

    for (i = 0; i < seg_num; i++) {
        if ((pa_seg[i].size % gpage_size) != 0) {
            is_gpage = false;
        }

        if ((pa_seg[i].size % hpage_size) != 0) {
            is_hpage = false;
        }

        if ((pa_seg[i].size % npage_size) != 0) {
            svm_err("Invalid pa seg. (size=%llu)\n", pa_seg[i].size);
            return 0;
        }
    }

    if (is_gpage && SVM_IS_ALIGNED(va, gpage_size) && SVM_IS_ALIGNED(size, gpage_size)) {
        return gpage_size;
    } else if (is_hpage && SVM_IS_ALIGNED(va, hpage_size) && SVM_IS_ALIGNED(size, hpage_size)) {
        return hpage_size;
    } else if (SVM_IS_ALIGNED(va, npage_size) && SVM_IS_ALIGNED(size, npage_size)) {
        return npage_size;
    } else {
        svm_err("Va or size isn't aligned by npage_size. (va=0x%llx; size=%llu; npage_size=%llu)\n",
            va, size, npage_size);
        return 0;
    }
}

static int _pma_page_table_init(u64 page_size, u64 page_num, struct svm_pa_seg pa_seg[], u64 seg_num,
    struct p2p_page_table *page_table)
{
    struct p2p_page_info *pages_info = NULL;
    u64 i, j, num = 0;

    pages_info = svm_kvzalloc(sizeof(struct p2p_page_info) * page_num, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (pages_info == NULL) {
        svm_err("Alloc pma mem node failed. (page_num=%llu)\n", page_num);
        return -ENOMEM;
    }

    for (i = 0; i < seg_num; i++) {
        u64 seg_page_num = pa_seg[i].size / page_size;

        for (j = 0; j < seg_page_num; j++) {
            if (ka_unlikely(num >= page_num)) {
                svm_err("Unlikely, num >= page_num. (num=%llu; page_num=%llu)\n", num, page_num);
                svm_kvfree(pages_info);
                return -EINVAL;
            }
            pages_info[num++].pa = pa_seg[i].pa + j * page_size;
        }
    }

    page_table->pages_info = pages_info;
    page_table->page_num = page_num;
    page_table->page_size = page_size;
    page_table->version = P2P_GET_PAGE_VERSION;
    return 0;
}

static void _pmq_page_table_uninit(struct p2p_page_table *page_table)
{
    svm_kvfree(page_table->pages_info);
    page_table->pages_info = NULL;
}

static int pma_pa_seg_alloc(u32 udevid, u64 va, u64 size, struct svm_pa_seg **pa_seg, u64 *seg_num)
{
    struct svm_pa_seg *segs = NULL;
    u64 page_size, num;
    int ret;

    ret = svm_dbi_kern_query_npage_size(udevid, &page_size);
    if (ret != 0) {
        svm_err("Dbi query device page_size failed. (ret=%d; udevid=%u)\n", ret, udevid);
        return ret;
    }

    num = svm_get_align_up_num(va, size, page_size);
    if (num == 0) {
        svm_err("Invalid size. (size=0x%llx)\n", size);
        return -EINVAL;
    }

    segs = svm_kvzalloc(num * sizeof(struct svm_pa_seg), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (segs == NULL) {
        svm_err("Malloc pa seg failed. (seg_num=%llu)\n", num);
        return -ENOMEM;
    }

    *pa_seg = segs;
    *seg_num = num;
    return 0;
}

static void pma_pa_seg_free(struct svm_pa_seg *pa_seg)
{
    svm_kvfree(pa_seg);
}

static int pma_page_table_init(struct pma_mem_node *mem_node)
{
    struct svm_global_va src_info;
    struct svm_pa_seg *pa_seg = NULL;
    u64 seg_num, page_size, page_num;
    u64 va = mem_node->va;
    u64 size = mem_node->size;
    u32 udevid = mem_node->udevid;
    int ret;

    svm_global_va_pack(udevid, 0, va, size, &src_info);
    ret = hal_kernel_apm_query_slave_tgid_by_master(mem_node->tgid, udevid, PROCESS_CP1, &src_info.tgid);
    if (ret != 0) {
        svm_err("Query cp1 tgid failed. (udevid=%u; ret=%d)\n", udevid, ret);
        return ret;
    }

    ret = pma_pa_seg_alloc(udevid, va, size, &pa_seg, &seg_num);
    if (ret != 0) {
        return ret;
    }

    ret = svm_pmq_client_host_bar_query(uda_get_host_id(), &src_info, pa_seg, &seg_num);
    if (ret != 0) {
        svm_err("Query host bar failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx; size=%llu)\n",
            ret, udevid, src_info.tgid, va, size);
        pma_pa_seg_free(pa_seg);
        return ret;
    }

    page_size = pma_get_dev_pa_seg_page_size(udevid, va, size, pa_seg, seg_num);
    if (page_size == 0ULL) {
        svm_err("Get dev pa seg page_size failed.\n");
        pma_pa_seg_free(pa_seg);
        return -EINVAL;
    }

    page_num = size / page_size;
    ret = _pma_page_table_init(page_size, page_num, pa_seg, seg_num, &mem_node->page_table);
    pma_pa_seg_free(pa_seg);
    return ret;
}

static void pma_page_table_uninit(struct pma_mem_node *mem_node)
{
    _pmq_page_table_uninit(&mem_node->page_table);
}

static int _pma_mem_node_create(struct pma_ctx *ctx, u64 va, u64 size,
    void (*free_callback)(void *data), void *data, struct pma_mem_node **mem_node)
{
    struct pma_mem_node *new_node = NULL;
    int ret;

    new_node = pma_mem_node_alloc(ctx, va, size, free_callback, data);
    if (new_node == NULL) {
        return -ENOMEM;
    }

    ret = pma_page_table_init(new_node);
    if (ret != 0) {
        pma_mem_node_free(new_node);
        return ret;
    }

    pma_mem_node_insert(ctx, new_node);
    *mem_node = new_node;
    return 0;
}

int pma_mem_node_create(struct pma_ctx *ctx, u64 va, u64 size,
    void (*free_callback)(void *data), void *data, struct pma_mem_node **mem_node)
{
    int ret;

    pma_use_pipeline(ctx);
    ret = _pma_mem_node_create(ctx, va, size, free_callback, data, mem_node);
    pma_unuse_pipeline(ctx);

    return ret;
}

static void _pma_mem_node_release(struct pma_mem_node *mem_node)
{
    pma_page_table_uninit(mem_node);
    pma_mem_node_free(mem_node);
}

static void pma_mem_node_release(ka_kref_t *kref)
{
    struct pma_mem_node *mem_node = ka_container_of(kref, struct pma_mem_node, ref);

    _pma_mem_node_release(mem_node);
}

void pma_mem_node_get(struct pma_mem_node *mem_node)
{
    ka_base_kref_get(&mem_node->ref);
}

void pma_mem_node_put(struct pma_mem_node *mem_node)
{
    ka_base_kref_put(&mem_node->ref, pma_mem_node_release);
}

static struct pma_mem_node *pma_mem_node_erase_one_with_get(struct pma_ctx *ctx)
{
    struct pma_mem_node *mem_node = NULL;

    ka_task_down_write(&ctx->rw_sem);
    if (ka_list_empty_careful(&ctx->head) == 1) {
        ka_task_up_write(&ctx->rw_sem);
        return NULL;
    }

    mem_node = ka_list_first_entry(&ctx->head, struct pma_mem_node, node);
    _pma_mem_node_erase(ctx, mem_node);
    pma_mem_node_get(mem_node);
    ka_task_up_write(&ctx->rw_sem);
    return mem_node;
}

static struct pma_mem_node *pma_mem_node_erase_by_range_with_get(struct pma_ctx *ctx, u64 va, u64 size)
{
    struct pma_mem_node *mem_node = NULL;
    struct pma_mem_node *n = NULL;
    unsigned long stamp = (unsigned long)ka_jiffies;

    ka_task_down_write(&ctx->rw_sem);
    ka_list_for_each_entry_safe(mem_node, n, &ctx->head, node) {
        if (!(((va + size) <= mem_node->va) || ((mem_node->va + mem_node->size) <= va))) {
            _pma_mem_node_erase(ctx, mem_node);
            pma_mem_node_get(mem_node);
            ka_task_up_write(&ctx->rw_sem);
            return mem_node;
        }
        ka_try_cond_resched(&stamp);
    }
    ka_task_up_write(&ctx->rw_sem);

    return NULL;
}

void pma_mem_show(struct pma_ctx *ctx, ka_seq_file_t *seq)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    struct pma_mem_node *mem_node = NULL;
    struct pma_mem_node *n = NULL;
    int i = 0;

    ka_task_down_read(&ctx->rw_sem);

    ka_fs_seq_printf(seq, "pma: udevid %u tgid %d mem num %llu get_cnt %llu put cnt %llu\n",
        ctx->udevid, ctx->tgid, ctx->mem_node_num, ctx->get_cnt, ctx->put_cnt);

    ka_list_for_each_entry_safe(mem_node, n, &ctx->head, node) {
        if (i == 0) {
            ka_fs_seq_printf(seq, "   %-5s %-17s %-15s\n", "id", "va", "size(Bytes)");
        }
        ka_fs_seq_printf(seq, "   %-5d 0x%-15llx %-15llu\n", i++, mem_node->va, mem_node->size);
        ka_try_cond_resched(&stamp);
    }

    ka_task_up_read(&ctx->rw_sem);
}

static void pma_mem_node_recycle_notify(struct pma_mem_node *mem_node)
{
    mem_node->free_callback(mem_node->data);
}

static void _pma_mem_recycle(struct pma_ctx *ctx, u64 va, u64 size)
{
    struct pma_mem_node *mem_node = NULL;
    unsigned long stamp = (unsigned long)ka_jiffies;
    bool recycle_by_range = (va != 0);
    int recycle_num = 0;

    do {
        if (recycle_by_range) {
            mem_node = pma_mem_node_erase_by_range_with_get(ctx, va, size);
        } else {
            mem_node = pma_mem_node_erase_one_with_get(ctx);
        }
        if (mem_node == NULL) {
            break;
        }

        /* Notify first, then drop recycle-held ref. */
        pma_mem_node_recycle_notify(mem_node);
        pma_mem_node_put(mem_node);
        recycle_num++;
        ka_try_cond_resched(&stamp);
    } while (1);

    if (!recycle_by_range &&
        ((recycle_num > 0) || (ctx->get_cnt != ctx->put_cnt))) {
        svm_info("Recycle mem. (udevid=%u; tgid=%d; get_cnt=%llu; put_cnt=%llu; recycle_num=%d)\n",
            ctx->udevid, ctx->tgid, ctx->get_cnt, ctx->put_cnt, recycle_num);
    }
}

void pma_mem_recycle(struct pma_ctx *ctx)
{
    _pma_mem_recycle(ctx, 0, 0);
}

void pma_mem_recycle_notify(u32 udevid, int tgid, u64 va, u64 size)
{
    struct pma_ctx *ctx = pma_ctx_get(udevid, tgid);
    if (ctx != NULL) {
        pma_occupy_pipeline(ctx);
        _pma_mem_recycle(ctx, va, size);
        pma_release_pipeline(ctx);
        pma_ctx_put(ctx);
    }
}
