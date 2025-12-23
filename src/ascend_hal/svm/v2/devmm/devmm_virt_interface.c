/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdint.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include "devmm_virt_comm.h"
#include "svm_ioctl.h"
#include "devmm_svm_init.h"
#include "devmm_svm.h"
#include "devmm_virt_base_heap.h"
#include "devmm_virt_com_heap.h"
#include "devmm_virt_dvpp_heap.h"
#include "devmm_cache_coherence.h"
#include "devmm_virt_interface.h"
#include "devmm_dynamic_addr.h"

STATIC THREAD struct devmm_virt_heap_mgmt *g_heap_mgmt = NULL;
static THREAD devmm_virt_lock_t g_lock_heap_mgmt = PTHREAD_MUTEX_INITIALIZER;
STATIC THREAD struct devmm_virt_heap_mgmt *g_tmp_heap_mgmt = NULL; /* Corresponding g_heap_mgmt address */
STATIC THREAD struct devmm_virt_heap_mgmt *g_backup_heap_mgmt = NULL; /* Corresponding g_heap_mgmt content */

DVresult devmm_get_heap_list_by_type(struct devmm_virt_heap_mgmt *p_heap_mgmt,
    struct devmm_virt_heap_type *heap_type, struct devmm_heap_list **heap_list)
{
    uint32_t page_type = heap_type->heap_type;
    uint32_t heap_lis_type = heap_type->heap_list_type;
    uint32_t heap_sub_type = heap_type->heap_sub_type;
    uint32_t heap_mem_type = heap_type->heap_mem_type;

    if ((heap_lis_type >= HEAP_MAX_LIST) ||
        (heap_sub_type >= SUB_MAX_TYPE) ||
        (heap_mem_type >= DEVMM_MEM_TYPE_MAX)) {
        DEVMM_DRV_ERR("Heap type error. (list_type=0x%x; sub_type=0x%x; mem_type=0x%x)\n",
            heap_lis_type, heap_sub_type, heap_mem_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (page_type == DEVMM_HEAP_CHUNK_PAGE) {
        *heap_list = &p_heap_mgmt->normal_list[heap_lis_type][heap_sub_type][heap_mem_type];
        return DRV_ERROR_NONE;
    } else if (page_type == DEVMM_HEAP_HUGE_PAGE) {
        *heap_list = &p_heap_mgmt->huge_list[heap_lis_type][heap_sub_type][heap_mem_type];
        return DRV_ERROR_NONE;
    } else if (page_type == DEVMM_HEAP_PINNED_HOST) {
        *heap_list = &p_heap_mgmt->normal_list[HOST_LIST][SUB_HOST_TYPE][heap_mem_type];
        return DRV_ERROR_NONE;
    }

    DEVMM_DRV_ERR("Get heap list error. (Heap_type=0x%x; list_type=%d)\n", page_type, heap_lis_type);

    return DRV_ERROR_INVALID_VALUE;
}

uint32_t devmm_virt_heap_size_to_order(uint64_t step, size_t size)
{
    size_t alloc_size = size;
    uint32_t order = 0;
    uint64_t devmm_step = step;

    while (alloc_size > devmm_step) {
        devmm_step = devmm_step << 1;
        order++;
    }

    return order;
}

void *devmm_virt_init_get_heap_mgmt(void)
{
    DVresult result = devmm_virt_init_heap_mgmt();
    if (result != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Devmm_virt_init_heap_mgmt error.\n");
        return NULL;
    }
    return g_heap_mgmt;
}

void *devmm_virt_get_heap_mgmt(void)
{
    if (g_heap_mgmt != NULL) {
        return g_heap_mgmt;
    }
    return devmm_virt_init_get_heap_mgmt();
}

void devmm_virt_uninit_heap_mgmt(void)
{
    g_tmp_heap_mgmt = g_heap_mgmt;
    g_heap_mgmt = NULL;
}

DVresult devmm_virt_backup_heap_mgmt(void)
{
    uint64_t size = sizeof(struct devmm_virt_heap_mgmt);

    if (g_heap_mgmt == NULL) {
        return DRV_ERROR_NONE;
    }

    if (g_backup_heap_mgmt == NULL) {
        g_backup_heap_mgmt = (struct devmm_virt_heap_mgmt *)malloc(size);
        if (g_backup_heap_mgmt == NULL) {
            DEVMM_DRV_ERR("Malloc failed.\n");
            return DRV_ERROR_OUT_OF_MEMORY;
        }
    }

    (void)memcpy_s(g_backup_heap_mgmt, size, g_heap_mgmt, size);

    return DRV_ERROR_NONE;
}

static DVresult devmm_restore_enable_heap(struct devmm_virt_heap_mgmt *backup_mgmt)
{
    struct devmm_virt_com_heap *heap = NULL;
    uint32_t i;
    int ret;

    for (i = 0; i < DEVMM_MAX_HEAP_NUM;) {
        heap = backup_mgmt->heap_queue.heaps[i];
        if (heap == NULL) {
            i++;
            continue;
        }
        ret = devmm_ioctl_enable_heap(heap->heap_idx, heap->heap_type, heap->heap_sub_type, heap->heap_size,
            heap->heap_list_type);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_RUN_INFO("Heap may had been enable. (idx=%u; size=%lu; i=%d; ret=%d)\n",
                heap->heap_idx, heap->heap_size, i, ret);
        } else {
            DEVMM_RUN_INFO("Restore heap. (idx=%u; size=%lu; i=%d)\n", heap->heap_idx, heap->heap_size, i);
        }
        i = (uint32_t)(i + heap->heap_size / DEVMM_HEAP_SIZE);
    }

    DEVMM_RUN_INFO("Restore enable heap succ.\n");
    return DRV_ERROR_NONE;
}

static DVresult devmm_restore_per_rbtree_node_mem(struct devmm_virt_com_heap *heap,
    struct devmm_rbtree_node *rbtree_node)
{
    virt_addr_t ret_ptr;

    if (!devmm_node_is_need_restore(rbtree_node)) {
        return DRV_ERROR_NONE;
    }

    ret_ptr = heap->ops->heap_alloc(heap, rbtree_node->data.va, rbtree_node->data.total, rbtree_node->data.advise);
    if (ret_ptr < DEVMM_SVM_MEM_START) {
        DEVMM_DRV_ERR("Can not alloc ptr. (out=0x%lx; alloc_ptr=0x%lx; alloc_size=%lu; advise=%u; sub_type=%u)\n",
            ret_ptr, rbtree_node->data.va, rbtree_node->data.total, rbtree_node->data.advise, heap->heap_sub_type);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    DEVMM_RUN_INFO("Restore ptr succ. (ret_ptr=0x%lx; alloc_ptr=0x%lx; alloc_size=%lu; advise=%u)\n",
        ret_ptr, rbtree_node->data.va, rbtree_node->data.total, rbtree_node->data.advise);

    return DRV_ERROR_NONE;
}

static DVresult devmm_restore_no_primary_heap_rebree_mem(struct rbtree_root *tree_root,
    struct devmm_virt_com_heap *heap, uint32_t tree_type)
{
    struct devmm_rbtree_node *rbtree_node = NULL;
    struct multi_rb_node *rb_node_list = NULL;
    struct multi_rb_node *rb_node = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *tmp = NULL;
    struct list_node *tmp_head = NULL;
    struct list_node *head = NULL;
    DVresult ret;

    if (devmm_rbtree_is_empty(tree_root) || (heap->ops == NULL)) {
        return DRV_ERROR_NONE;
    }

    rbtree_node_for_each_prev_safe(cur, tmp, tree_root) {
        rb_node = multi_rbtree_get_node_from_rb_node(cur);
        if (list_empty(&rb_node->list) == 0) {
            list_for_each_node_safe(head, tmp_head, &rb_node->list) {
                rb_node_list = rb_entry(head, struct multi_rb_node, list);
                rbtree_node = devmm_get_rbtree_node_by_type(rb_node_list, tree_type);
                ret = devmm_restore_per_rbtree_node_mem(heap, rbtree_node);
                if (ret != DRV_ERROR_NONE) {
                    return ret;
                }
            }
        }

        rbtree_node = devmm_get_rbtree_node_by_type(rb_node, tree_type);
        ret = devmm_restore_per_rbtree_node_mem(heap, rbtree_node);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

static DVresult devmm_restore_no_primary_heap_mem(struct devmm_virt_com_heap *heap)
{
    struct rbtree_root *alloced_tree = NULL;
    struct rbtree_root *cache_tree = NULL;
    DVresult ret;
    int i;

    alloced_tree = heap->rbtree_queue.alloced_tree;
    ret = devmm_restore_no_primary_heap_rebree_mem(alloced_tree, heap, DEVMM_ALLOCED_TREE);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("restore alloc_tree mem fail. (heap_idx=%u; heap_size=%lu; ret=%d)\n",
            heap->heap_idx, heap->heap_size, ret);
        return ret;
    }
    DEVMM_RUN_INFO("restore alloc_tree mem succ. (idx=%u; size=%lu)\n", heap->heap_idx, heap->heap_size);

    for (i = 0; i < DEVMM_MAPPED_TREE_TYPE_MAX; i++) {
        cache_tree = heap->rbtree_queue.idle_mapped_cache_tree[i];
        ret = devmm_restore_no_primary_heap_rebree_mem(cache_tree, heap, DEVMM_IDLE_MAPPED_TREE);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Restore map_tree_mem fail. (heap_idx=%u; heap_size=%lu; ret=%d)\n",
                heap->heap_idx, heap->heap_size, ret);
            return ret;
        }
        DEVMM_RUN_INFO("Restore map_tree_mem succ. (idx=%u; size=%lu; type=%d)\n", heap->heap_idx, heap->heap_size, i);
    }

    return DRV_ERROR_NONE;
}

static DVresult devmm_restore_primary_heap_mem(struct devmm_virt_com_heap *heap)
{
    virt_addr_t ret_ptr;

    ret_ptr = devmm_virt_heap_alloc_ops(heap, heap->start, heap->mapped_size, heap->advise);
    if (ret_ptr < DEVMM_SVM_MEM_START) {
        DEVMM_DRV_ERR("Can not alloc ptr. (out=0x%lx; alloc_ptr=0x%lx; alloc_size=%lu; advise=%u; heap_sub_type=%u)\n",
            ret_ptr, heap->start, heap->mapped_size, heap->advise, heap->heap_sub_type);
        return DRV_ERROR_OUT_OF_MEMORY;
    } else {
        DEVMM_RUN_INFO("Restore primary_heap_mem succ. (ptr=0x%lx; alloc_size=%lu; advise=%u; heap_sub_type=%u)\n",
            heap->start, heap->mapped_size, heap->advise, heap->heap_sub_type);
    }
    return DRV_ERROR_NONE;
}

static DVresult devmm_virt_restore_heap_alloc_mem(struct devmm_virt_heap_mgmt *backup_mgmt)
{
    struct devmm_virt_com_heap *heap = NULL;
    DVresult ret;
    uint32_t i;

    for (i = 0; i < DEVMM_MAX_HEAP_NUM;) {
        heap = backup_mgmt->heap_queue.heaps[i];
        if (heap == NULL) {
            i++;
            continue;
        }

        if (heap->heap_sub_type == SUB_SVM_TYPE) {
            i = i + (uint32_t)(heap->heap_size / DEVMM_HEAP_SIZE);
            continue;
        }

        if (devmm_virt_heap_is_primary(heap)) {
            ret = devmm_restore_primary_heap_mem(heap);
        } else {
            ret = devmm_restore_no_primary_heap_mem(heap);
        }

        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
        i = i + (uint32_t)(heap->heap_size / DEVMM_HEAP_SIZE);
    }

    DEVMM_RUN_INFO("Restore heap mem succ.\n");
    return DRV_ERROR_NONE;
}


static void devmm_virt_clear_sub_svm_type_heap(struct devmm_virt_heap_mgmt *mgmt)
{
    struct devmm_virt_heap_type heap_type = {0};
    struct devmm_heap_list *heap_list = NULL;
    struct devmm_virt_com_heap *heap = NULL;
    uint32_t i;

    for (i = 0; i < DEVMM_MAX_HEAP_NUM;) {
        heap = mgmt->heap_queue.heaps[i];
        if (heap == NULL) {
            i++;
            continue;
        }

        i = i + (uint32_t)(heap->heap_size / DEVMM_HEAP_SIZE);
        if (heap->heap_sub_type == SUB_SVM_TYPE) {
            heap_type.heap_type = heap->heap_type;
            heap_type.heap_list_type = heap->heap_list_type;
            heap_type.heap_sub_type = heap->heap_sub_type;
            heap_type.heap_mem_type = heap->heap_mem_type;
            if (devmm_get_heap_list_by_type(mgmt, &heap_type, &heap_list) != DRV_ERROR_NONE) {
                continue;
            }

            devmm_virt_list_del_init(&(heap->list));
            (void)devmm_virt_destroy_heap(mgmt, heap, true); /* will free heap, Cannot be accessed anymore */
            heap_list->heap_cnt--;
        }
    }

    DEVMM_RUN_INFO("Clear sub_svm_type heap succ.\n");
    return;
}

DVresult devmm_virt_restore_heap_mgmt(void)
{
    DVresult ret;
    uint64_t size;

    if (g_heap_mgmt == NULL) {
        return DRV_ERROR_NONE;
    }

    if (g_backup_heap_mgmt == NULL) {
        DEVMM_DRV_ERR("Not call backup, oper not permitted.\n");
        return DRV_ERROR_OPER_NOT_PERMITTED;
    }

    ret = devmm_restore_enable_heap(g_backup_heap_mgmt);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Restort enable heap failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = devmm_virt_restore_heap_alloc_mem(g_backup_heap_mgmt);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Restort devmm_virt_restore_heap_alloc_mem failed. (ret=%d)\n", ret);
        return ret;
    }

    size = sizeof(struct devmm_heap_queue);
    (void)memcpy_s(&g_heap_mgmt->heap_queue, size, &g_backup_heap_mgmt->heap_queue, size);
    size = sizeof(struct devmm_heap_list) * HEAP_MAX_LIST * SUB_MAX_TYPE * DEVMM_MEM_TYPE_MAX;
    (void)memcpy_s(g_heap_mgmt->huge_list, size, g_backup_heap_mgmt->huge_list, size);
    (void)memcpy_s(g_heap_mgmt->normal_list, size, g_backup_heap_mgmt->normal_list, size);

    devmm_virt_clear_sub_svm_type_heap(g_heap_mgmt);

    return DRV_ERROR_NONE;
}

struct devmm_virt_com_heap *devmm_virt_get_heap_mgmt_virt_heap(uint32_t heap_idx)
{
    if (heap_idx >= DEVMM_MAX_HEAP_NUM) {
        DEVMM_DRV_ERR("Heap id error.\n");
        return NULL;
    }
    if (g_heap_mgmt != NULL && g_heap_mgmt->heap_queue.heaps[heap_idx] != NULL) {
        return g_heap_mgmt->heap_queue.heaps[heap_idx];
    } else {
        return NULL;
    }
}

STATIC DVresult devmm_ioctl_advise(DVdeviceptr p, size_t size, uint32_t dev_id, DVmem_advise advise)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.head.devid = dev_id;
    arg.data.advise_para.ptr = p;
    arg.data.advise_para.count = size;
    arg.data.advise_para.advise = advise;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_ADVISE, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_INFO("Can not memory advise. (ptr=0x%llx; count=%lu; advise=0x%llx; device=%u)\n",
            p, size, advise, dev_id);
        return ret;
    }

    return DRV_ERROR_NONE;
}

DVresult devmm_ioctl_free_pages(virt_addr_t ptr)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.data.free_pages_para.va = ptr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_FREE_PAGES, &arg);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC DVresult devmm_ioctl_alloc(DVdeviceptr p, size_t size)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.data.alloc_svm_para.p = p;
    arg.data.alloc_svm_para.size = size;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_ALLOC, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Ioctl alloc error. (ret=%d; size=%lu; ptr=0x%llx)\n",
            ret, size, p);
        return ret;
    }

    return DRV_ERROR_NONE;
}

void devmm_virt_status_init(struct devmm_virt_com_heap *heap)
{
    /* init monitoring information */
    (void)heap;
}

STATIC DVresult devmm_ioctl_alloc_and_advise(DVdeviceptr p, size_t size, uint32_t devid, DVmem_advise advise)
{
    DVresult ret;

    ret = devmm_ioctl_alloc(p, size);
    if (ret != 0) {
        DEVMM_DRV_ERR("Alloc memory error. (ret=%d)\n", ret);
        return ret;
    }

    ret = devmm_ioctl_advise(p, size, devid, advise);
    if (ret != DRV_ERROR_NONE) {
        (void)devmm_ioctl_free_pages(p);
        return ret;
    }
    return DRV_ERROR_NONE;
}

int devmm_ioctl_alloc_dev(DVdeviceptr p, size_t size, uint32_t devid, DVmem_advise advise)
{
    return (int)devmm_ioctl_alloc_and_advise(p, size, devid, advise | DV_ADVISE_POPULATE | DV_ADVISE_LOCK_DEV);
}

STATIC int devmm_ioctl_alloc_host(DVdeviceptr p, size_t size, DVmem_advise advise)
{
    return (int)devmm_ioctl_alloc_and_advise(p, size, 0, advise | DV_ADVISE_HOST);
}

uint32_t devmm_va_to_heap_idx(const struct devmm_virt_heap_mgmt *mgmt, virt_addr_t va)
{
    return (uint32_t)((va - mgmt->start) / DEVMM_HEAP_SIZE);
}

struct devmm_virt_com_heap *devmm_va_to_heap(virt_addr_t va)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;

    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        /* do not printf, p_heap_mgmt == NULL is allowable here, will be judged in other process */
        return NULL;
    }
    if ((va < p_heap_mgmt->start) || (va >= p_heap_mgmt->end)) {
        /* do not printf ,printf too much */
        DEVMM_DRV_SWITCH("Address is not reserved. (addr=0x%lx; start=0x%lx; end=0x%lx)\n",
            va, p_heap_mgmt->start, p_heap_mgmt->end);
        return NULL;
    }

    return devmm_virt_get_heap_mgmt_virt_heap(devmm_va_to_heap_idx(p_heap_mgmt, va));
}

bool devmm_va_is_svm(virt_addr_t va)
{
    struct devmm_virt_com_heap *heap = NULL;

    if (va >= DEVMM_MAX_DYN_ALLOC_BASE) {
        return svm_is_dyn_addr(va);
    }

    heap = devmm_va_to_heap(va);
    if (IS_ERR_OR_NULL(heap)) {
        /* do not printf ,printf too much */
        return DEVMM_FALSE;
    }
    if ((heap->heap_type != DEVMM_HEAP_PINNED_HOST) && (heap->heap_type != DEVMM_HEAP_IDLE)) {
        return DEVMM_TRUE;
    }
    return DEVMM_FALSE;
}

static void devmm_host_pin_pre_register(virt_addr_t ret_val, size_t alloc_size)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    uint32_t i;

    mgmt = devmm_virt_get_heap_mgmt();
    if ((mgmt != NULL) && (mgmt->support_host_pin_pre_register)) {
        for (i = 0; i < DEVMM_MAX_PHY_DEVICE_NUM; ++i) {
            if (mgmt->is_dev_inited[i]) {
                (void)devmm_register_mem_to_dma((void *)(uintptr_t)ret_val, alloc_size, i);
            }
        }
    }
}

void devmm_host_pin_pre_register_release(virt_addr_t ptr)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    uint32_t i;

    mgmt = devmm_virt_get_heap_mgmt();
    if ((mgmt != NULL) && (mgmt->support_host_pin_pre_register)) {
        for (i = 0; i < DEVMM_MAX_PHY_DEVICE_NUM; ++i) {
            if (mgmt->is_dev_inited[i]) {
                (void)devmm_unregister_mem_to_dma((void *)(uintptr_t)ptr, i);
            }
        }
    }
}

STATIC virt_addr_t devmm_virt_heap_alloc_host(struct devmm_virt_com_heap *heap,
    virt_addr_t ret_val, size_t alloc_size, DVmem_advise advise)
{
    if (devmm_ioctl_alloc_host(ret_val, alloc_size, advise) != 0) {
        return DEVMM_INVALID_STOP;
    }

    devmm_mem_mapped_size_inc(heap, alloc_size);
    devmm_host_pin_pre_register(ret_val, alloc_size);

    return ret_val;
}

virt_addr_t devmm_virt_heap_alloc_device(struct devmm_virt_com_heap *heap,
    virt_addr_t ret_val, size_t alloc_size, DVmem_advise advise)
{
    uint32_t devid = devmm_heap_device_by_list_type(heap->heap_list_type);
    int ret;

    ret = devmm_ioctl_alloc_dev(ret_val, alloc_size, devid, advise);
    if (ret != 0) {
        return errcode_to_ptr(ret, DEVMM_INVALID_STOP);
    }

    devmm_mem_mapped_size_inc(heap, alloc_size);

    return ret_val;
}

STATIC virt_addr_t devmm_virt_heap_alloc_svm(struct devmm_virt_com_heap *heap, virt_addr_t ret_val,
    size_t alloc_size, DVmem_advise advise)
{
    if (devmm_ioctl_alloc_and_advise(ret_val, alloc_size, 0, advise) != DRV_ERROR_NONE) {
        return DEVMM_INVALID_STOP;
    }

    devmm_mem_mapped_size_inc(heap, alloc_size);
    return ret_val;
}

STATIC virt_addr_t devmm_virt_heap_alloc_reserve(struct devmm_virt_com_heap *heap, virt_addr_t ret_val,
    size_t alloc_size, DVmem_advise advise)
{
    (void)heap;
    if (devmm_ioctl_alloc_and_advise(ret_val, alloc_size, 0, advise) != DRV_ERROR_NONE) {
        return DEVMM_INVALID_STOP;
    }

    return ret_val;
}

DVresult devmm_virt_heap_free_pages(struct devmm_virt_com_heap *heap, virt_addr_t ptr)
{
    DVresult ret;
    /*
     * call ioctl first to ensure clear bitmap first
     */
    if (heap->heap_sub_type == SUB_HOST_TYPE) {
        devmm_host_pin_pre_register_release(ptr);
    }

    ret = devmm_ioctl_free_pages(ptr);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Devmm_ioctl_free failed. (ptr=0x%llx; heap_type=%u)\n", ptr, heap->heap_type);
        return ret;
    }
    return DRV_ERROR_NONE;
}

/* primary heap means that the heap is allocated from base heap and can not be allocated further. */
bool devmm_virt_heap_is_primary(struct devmm_virt_com_heap *heap)
{
    struct devmm_heap_rbtree *queue = &heap->rbtree_queue;

    if (queue->idle_size_tree == NULL) {
        return DEVMM_TRUE;
    }

    return DEVMM_FALSE;
}

static void _devmm_virt_free_idle_heap(struct devmm_virt_heap_mgmt *mgmt, struct devmm_heap_list *heap_list,
    struct devmm_virt_com_heap *heap)
{
    if (!devmm_virt_heap_is_primary(heap) && devmm_virt_check_idle_heap(heap)) {
        devmm_virt_list_del_init(&(heap->list));
        (void)devmm_virt_destroy_heap(mgmt, heap, true);
        heap_list->heap_cnt--;
    }
}

static void devmm_virt_free_idle_heap(struct devmm_virt_heap_mgmt *mgmt, struct devmm_heap_list *heap_list,
    struct devmm_virt_com_heap *heap)
{
    (void)pthread_rwlock_wrlock(&heap_list->list_lock);
    DEVMM_DRV_SWITCH("Heap info. (heap_type=0x%x; sub_type=0x%x; size=%llu; "
        "page_size=%u; start_addr=0x%lx; end_addr=0x%lx; cache=%u; map_size=%u; is_limited=%u; heap_cnt=%u; "
        "sys_mem_alloced=%llu; sys_mem_freed=%llu; cur_cache_mem=%llu)\n",
        heap->heap_type, heap->heap_sub_type, heap->heap_size, heap->chunk_size, heap->start, heap->end,
        heap->need_cache_thres[DEVMM_MEM_NORMAL], heap->map_size, heap->is_limited, heap_list->heap_cnt,
        heap->sys_mem_alloced, heap->sys_mem_freed, heap->cur_cache_mem[DEVMM_MEM_NORMAL]);
    _devmm_virt_free_idle_heap(mgmt, heap_list, heap);
    (void)pthread_rwlock_unlock(&heap_list->list_lock);
}

STATIC void devmm_virt_free_idle_heaps(struct devmm_virt_heap_mgmt *mgmt, struct devmm_heap_list *heap_list)
{
    struct devmm_virt_list_head *pos = NULL;
    struct devmm_virt_list_head *n = NULL;
    struct devmm_virt_com_heap *heap = NULL;

    (void)pthread_rwlock_wrlock(&heap_list->list_lock);
    devmm_virt_list_for_each_safe(pos, n, &heap_list->heap_list)
    {
        heap = devmm_virt_list_entry(pos, struct devmm_virt_com_heap, list);
        _devmm_virt_free_idle_heap(mgmt, heap_list, heap);
    }
    (void)pthread_rwlock_unlock(&heap_list->list_lock);
}

STATIC void devmm_virt_destory_all_heap(void)
{
    struct devmm_virt_com_heap *heap = NULL;
    uint32_t heap_idx, heap_num;

    for (heap_idx = 0; heap_idx < DEVMM_MAX_HEAP_NUM; heap_idx++) {
        heap = g_heap_mgmt->heap_queue.heaps[heap_idx];
        if (heap != NULL) {
            heap_num = (uint32_t)(heap->heap_size / DEVMM_HEAP_SIZE);
            devmm_rbtree_destory(&heap->rbtree_queue);
            free(heap);
            heap = NULL;
            heap_idx += heap_num - 1;
        }
    }

    devmm_rbtree_destory(&g_heap_mgmt->heap_queue.base_heap.rbtree_queue);
}

STATIC DVresult devmm_virt_alloc_heap_mgmt(void)
{
    int ret;
    if (g_heap_mgmt == NULL) {
        /* thread exit auto free */
        DEVMM_DRV_SWITCH("Svm init. (heap_mgmt_size=%lu)\n", sizeof(struct devmm_virt_heap_mgmt));
        if (devmm_is_snapshot_state() && (g_tmp_heap_mgmt != NULL)) {
            g_heap_mgmt = g_tmp_heap_mgmt;
        } else {
            g_heap_mgmt = (struct devmm_virt_heap_mgmt *)malloc(sizeof(struct devmm_virt_heap_mgmt));
            if (g_heap_mgmt == NULL) {
                DEVMM_DRV_ERR("Svm init error.\n");
                return DRV_ERROR_OUT_OF_MEMORY;
            }
        }

        ret = memset_s(g_heap_mgmt, sizeof(struct devmm_virt_heap_mgmt), 0, sizeof(struct devmm_virt_heap_mgmt));
        if (ret != 0) {
            DEVMM_DRV_ERR("Memset_s return error. (g_heap_mgmt=%p; ret=%d)\n", g_heap_mgmt, ret);
            free(g_heap_mgmt);
            g_heap_mgmt = NULL;
            return DRV_ERROR_MEMORY_OPT_FAIL;
        }
    } else if (g_heap_mgmt->pid != getpid()) {
        /* if heap_mgmt->pid isn't equal to current pid, reset all svm process structures */
        devmm_virt_destory_all_heap();
        ret = memset_s(g_heap_mgmt, sizeof(struct devmm_virt_heap_mgmt),
                       0, sizeof(struct devmm_virt_heap_mgmt));
        if (ret != 0) {
            DEVMM_DRV_ERR("Memset_s return error. (g_heap_mgmt=%p; ret=%d)\n", g_heap_mgmt, ret);
            free(g_heap_mgmt);
            g_heap_mgmt = NULL;
            return DRV_ERROR_MEMORY_OPT_FAIL;
        }
        devmm_svm_master_uninit();
    }
    return DRV_ERROR_NONE;
}

STATIC DVresult devmm_ioctl_init_process(struct devmm_ioctl_arg *arg)
{
    DVresult ret;
    int cnt;

    for (cnt = 0; cnt < DEVMM_MAX_INIT_CNT; cnt++) {
        ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_INIT_PROCESS, (void *)arg);
        if (ret != DRV_ERROR_TRY_AGAIN) {
            break;
        }
    }
    DEVMM_RUN_INFO("Init_process details. (hostpid=%d; ret=%d; cnt=%d)\n", getpid(), ret, cnt);
    return ret;
}

STATIC DVresult devmm_virt_init_mgmt_page_size_and_addr(struct devmm_virt_heap_mgmt *mgmt,
    struct devmm_ioctl_arg *arg)
{
    virt_addr_t start, end;

    start = (virt_addr_t)(DEVMM_SVM_MEM_START);
    end = (virt_addr_t)(DEVMM_SVM_MEM_START + DEVMM_SVM_MEM_SIZE);

    mgmt->svm_page_size = arg->data.init_process_para.svm_page_size;
    mgmt->local_page_size = arg->data.init_process_para.local_page_size;
    mgmt->huge_page_size = arg->data.init_process_para.huge_page_size;
    mgmt->support_host_giant_page = arg->data.init_process_para.is_enable_host_giant_page;

    if ((mgmt->svm_page_size == 0) || (mgmt->local_page_size == 0) || (mgmt->huge_page_size == 0)) {
        DEVMM_DRV_ERR("Init page size fail. (svm_page_size=0x%x; local_page_size=0x%x; "
            "huge_page_size=0x%x)\n", mgmt->svm_page_size, mgmt->local_page_size, mgmt->huge_page_size);
        return DRV_ERROR_INNER_ERR;
    }

    if (!IS_ALIGNED(start, mgmt->svm_page_size) || !IS_ALIGNED(end, mgmt->svm_page_size)) {
        DEVMM_DRV_ERR("Start_addr or end_addr is not aligned. (start=%lu; "
            "end=%lu; svm_page_size=%u)\n", start, end, mgmt->svm_page_size);
        return DRV_ERROR_INNER_ERR;
    }
    mgmt->start = start;
    mgmt->end = end;

    return DRV_ERROR_NONE;
}

STATIC void devmm_virt_init_mgmt_reserve_addr_range(struct devmm_virt_heap_mgmt *mgmt)
{
    mgmt->dvpp_start = mgmt->start;
    mgmt->dvpp_end = mgmt->start +
        DEVMM_DVPP_HEAP_RESERVATION_SIZE * DEVMM_DVPP_HEAP_NUM - 1;

    mgmt->read_only_start = mgmt->dvpp_end + 1;
    mgmt->read_only_end = mgmt->read_only_start + DEVMM_READ_ONLY_HEAP_TOTAL_SIZE +
        DEVMM_DEV_READ_ONLY_HEAP_TOTAL_SIZE - 1;
}

STATIC DVresult devmm_virt_init_mgmt_queue_and_lists(struct devmm_virt_heap_mgmt *mgmt)
{
    uint32_t i, j, k;
    /* init base heap to manage heaps */
    if (devmm_virt_init_base_heap(mgmt) != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Init base heap fail.\n");
        return DRV_ERROR_INNER_ERR;
    }
    for (i = 0; i < DEVMM_MAX_PHY_DEVICE_NUM; i++) {
        mgmt->dvpp_mem_size[i] = DEVMM_MAX_HEAP_MEM_FOR_DVPP_4G;
        mgmt->support_bar_mem[i] = false;
        mgmt->support_dev_read_only[i] = false;
    }
    devmm_virt_init_mgmt_reserve_addr_range(mgmt);

    for (i = 0; i < HEAP_MAX_LIST; i++) {
        for (j = 0; j < SUB_MAX_TYPE; j++) {
            for (k = 0; k < DEVMM_MEM_TYPE_MAX; k++) {
                SVM_INIT_LIST_HEAD(&mgmt->normal_list[i][j][k].heap_list);
                mgmt->normal_list[i][j][k].heap_cnt = 0;
                (void)pthread_rwlock_init(&mgmt->normal_list[i][j][k].list_lock, NULL);

                SVM_INIT_LIST_HEAD(&mgmt->huge_list[i][j][k].heap_list);
                mgmt->huge_list[i][j][k].heap_cnt = 0;
                (void)pthread_rwlock_init(&mgmt->huge_list[i][j][k].list_lock, NULL);
            }
        }
    }
    mgmt->inited = POOL_MGMT_INITED_FLAG;
    return DRV_ERROR_NONE;
}

DVresult devmm_virt_init_heap_mgmt(void)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;
    uint32_t i;

    (void)devmm_virt_lock(&g_lock_heap_mgmt);
    ret = devmm_virt_alloc_heap_mgmt();
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Devmm virt alloc heap_mgmt fail.\n");
        goto init_heap_mgmt_unlock;
    }
    mgmt = g_heap_mgmt;
    /* 1.if has inited, ret DRV_ERROR_NONE. */
    if (mgmt->inited == POOL_MGMT_INITED_FLAG) {
        DEVMM_DRV_SWITCH("Already init...\n");
        ret = DRV_ERROR_NONE;
        goto init_heap_mgmt_unlock;
    }

    for (i = 0; i < SVM_MAX_AGENT_NUM; i++) {
        mgmt->can_init_dev[i] = true;
    }

    share_log_create(HAL_MODULE_TYPE_DEVMM, SHARE_LOG_MAX_SIZE);
    ret = devmm_svm_master_init();
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Svm init error.\n");
        goto init_heap_mgmt_svm_init_fail;
    }

    /* 2.init process and get page_sizes from kernel,
     * if has not exchanged page info, fail.
     */
    if (devmm_ioctl_init_process(&arg) != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Can not get page_size, has not exchanged.\n");
        ret = DRV_ERROR_IOCRL_FAIL;
        goto init_heap_mgmt_init_process_fail;
    }

    ret = devmm_virt_init_mgmt_page_size_and_addr(mgmt, &arg);
    if (ret != DRV_ERROR_NONE) {
        /* do not need print */
        goto init_heap_mgmt_init_process_fail;
    }

    mgmt->max_conti_size = DEVMM_MAX_MAPPED_RANGE;
    mgmt->pid = getpid();

    /* 3.init heap queue and lists
     * heap queue is used for mgmt heaps
     * huge, normal, host list is used for cache diffrent heaps
     */
    ret = devmm_virt_init_mgmt_queue_and_lists(mgmt);
    if (ret != DRV_ERROR_NONE) {
        /* do not need print */
        goto init_heap_mgmt_init_process_fail;
    }

    ret = (DVresult)devmm_init_convert_task_mgmt();
    if (ret != DRV_ERROR_NONE) {
        /* do not need print */
        goto init_heap_mgmt_init_process_fail;
    }

    (void)devmm_virt_unlock(&g_lock_heap_mgmt);

    return DRV_ERROR_NONE;

init_heap_mgmt_init_process_fail:
    devmm_svm_master_uninit();
init_heap_mgmt_svm_init_fail:
    mgmt->inited = 0;
    share_log_destroy(HAL_MODULE_TYPE_DEVMM);
init_heap_mgmt_unlock:
    (void)devmm_virt_unlock(&g_lock_heap_mgmt);
    return ret;
}

DVresult devmm_ioctl_enable_heap(uint32_t heap_idx,
    uint32_t heap_type, uint32_t heap_sub_type, uint64_t heap_size, uint32_t heap_list_type)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.data.update_heap_para.op = DEVMM_HEAP_ENABLE;
    arg.data.update_heap_para.heap_idx = heap_idx;
    arg.data.update_heap_para.heap_type = heap_type;
    arg.data.update_heap_para.heap_sub_type = heap_sub_type;
    /* enable need set heap size, disable get by kernel note */
    arg.data.update_heap_para.heap_size = heap_size;
    if (heap_sub_type == SUB_DEVICE_TYPE) {
        arg.head.devid = devmm_heap_device_by_list_type(heap_list_type);
    }

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_UPDATE_HEAP, &arg);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return DRV_ERROR_NONE;
}

int devmm_ioctl_disable_heap(uint32_t heap_idx,
    uint32_t heap_type, uint32_t heap_sub_type, uint64_t heap_size)
{
    struct devmm_ioctl_arg arg = {0};
    int ret;

    arg.data.update_heap_para.op = DEVMM_HEAP_DISABLE;
    arg.data.update_heap_para.heap_idx = heap_idx;
    arg.data.update_heap_para.heap_type = heap_type;
    arg.data.update_heap_para.heap_sub_type = heap_sub_type;
    /* enable need set heap size, disable get by kernel note */
    arg.data.update_heap_para.heap_size = heap_size;

    ret = (int)devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_UPDATE_HEAP, &arg);
    if (ret != 0) {
        DEVMM_DRV_ERR("Ioctl device error. (ret=%d)\n", ret);
        return ret;
    }

    return 0;
}


STATIC void devmm_get_free_threshold_by_type(struct devmm_virt_heap_mgmt *p_heap_mgmt,
    struct devmm_virt_com_heap *heap, int *free_thresdhold)
{
    (void)p_heap_mgmt;

    if (heap->heap_type == DEVMM_HEAP_PINNED_HOST) {
        *free_thresdhold = DEVMM_POOL_PINNED_MEM_DESTROY_THREDHOLD;
        return;
    }

    if (heap->heap_sub_type == SUB_SVM_TYPE) {
        *free_thresdhold = DEVMM_POOL_DESTROY_THREADHOLD;
        return;
    }

    *free_thresdhold = DEVMM_POOL_DESTROY_THREADHOLD;
}

DVresult devmm_free_to_normal_heap(struct devmm_virt_heap_mgmt *p_heap_mgmt,
    struct devmm_virt_com_heap *heap, DVdeviceptr p, uint64_t *free_len)
{
    struct devmm_heap_list *heap_list = NULL;
    struct devmm_virt_heap_type heap_type;
    int free_thresdhold;
    DVresult ret;

    heap_type.heap_type = heap->heap_type;
    heap_type.heap_list_type = heap->heap_list_type;
    heap_type.heap_sub_type = heap->heap_sub_type;
    heap_type.heap_mem_type = heap->heap_mem_type;

    ret = devmm_get_heap_list_by_type(p_heap_mgmt, &heap_type, &heap_list);
    if (ret != DRV_ERROR_NONE) {
        /* will not return err, heap_type is not pass by user */
        return ret;
    }
    /* use heap list lock to ensure heap donot destory when oper pg */
    (void)pthread_rwlock_rdlock(&heap_list->list_lock);
    ret = devmm_free_mem(p, heap, free_len);
    (void)pthread_rwlock_unlock(&heap_list->list_lock);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Virt_heap_free_mem failed. (ret=%d; va=0x%llx)\n", ret, p);
        return ret;
    }
    DEVMM_DRV_DEBUG_ARG("Free normal heap details. (free_ptr=0x%llx; "
                        "heap_type=0x%x; heap_sub_type=%u)\n", p, heap->heap_type, heap->heap_sub_type);

    devmm_get_free_threshold_by_type(p_heap_mgmt, heap, &free_thresdhold);
    if (heap->is_cache == false) {
        devmm_virt_free_idle_heap(p_heap_mgmt, heap_list, heap);
    }
    if (heap_list->heap_cnt > free_thresdhold) {
        devmm_virt_free_idle_heaps(p_heap_mgmt, heap_list);
    }

    return DRV_ERROR_NONE;
}

STATIC struct devmm_com_heap_ops g_heap_ops[SUB_MAX_TYPE] = {
    [SUB_SVM_TYPE] = {
        devmm_virt_heap_alloc_svm,
        devmm_virt_heap_free_pages
    },
    [SUB_DEVICE_TYPE] = {
        devmm_virt_heap_alloc_device,
        devmm_virt_heap_free_pages
    },
    [SUB_HOST_TYPE] = {
        devmm_virt_heap_alloc_host,
        devmm_virt_heap_free_pages
    },
    [SUB_DVPP_TYPE] = {
        devmm_virt_heap_alloc_device,
        devmm_virt_heap_free_pages
    },
    [SUB_RESERVE_TYPE] = {
        devmm_virt_heap_alloc_reserve,
        devmm_virt_heap_free_pages
    }
};

int devmm_virt_heap_free_ops(struct devmm_virt_com_heap *heap, virt_addr_t ptr)
{
    return g_heap_ops[heap->heap_sub_type].heap_free(heap, ptr);
}

virt_addr_t devmm_virt_heap_alloc_ops(struct devmm_virt_com_heap *heap, virt_addr_t alloc_ptr,
    size_t alloc_size, DVmem_advise advise)
{
    return g_heap_ops[heap->heap_sub_type].heap_alloc(heap, alloc_ptr, alloc_size, advise);
}

uint32_t devmm_virt_get_page_size_by_heap_type(struct devmm_virt_heap_mgmt *mgmt,
    uint32_t heap_type, uint32_t heap_sub_type)
{
    if (heap_sub_type == SUB_RESERVE_TYPE) {
        return mgmt->huge_page_size;
    }

    if (heap_type == DEVMM_HEAP_PINNED_HOST) {
        return DEVMM_DEV_PAGE_SIZE;
    } else {
        /*
         * to more efficient to use huge page ram,
         * alloc device mem kernel manage page size use huge page size,
         * user mode alloc used svm page size, and cache left size for next alloc
         * for details see devmm_virt_heap_alloc_chunk_device.
         * svm mem type kernel and user manage page size use same.
         * kernel will get page size by heap_type.
         */
        if (heap_type == DEVMM_HEAP_HUGE_PAGE) {
            return mgmt->huge_page_size;
        } else {
            /* use 4K to manage user mode memory */
            if (heap_sub_type == SUB_DEVICE_TYPE) {
                return DEVMM_DEV_PAGE_SIZE;
            } else {
                return mgmt->svm_page_size;
            }
        }
    }
}

uint32_t devmm_virt_get_kernel_page_size_by_heap_type(struct devmm_virt_heap_mgmt *mgmt,
    uint32_t heap_type, uint32_t heap_sub_type)
{
    if (heap_sub_type == SUB_RESERVE_TYPE) {
        return DEVMM_MAP_ALIGN_SIZE;
    }

    /* dev/host/dvpp's kernel_page_size is configured as 2M temporarily, otherwise ut:svm_addr_query_test fails */
    if (heap_type == DEVMM_HEAP_PINNED_HOST) {
        return DEVMM_MAP_ALIGN_SIZE;
    } else {
        if (heap_type == DEVMM_HEAP_HUGE_PAGE) {
            return DEVMM_MAP_ALIGN_SIZE;
        } else {
            /* use 4K to manage user mode memory */
            if (heap_sub_type == SUB_DEVICE_TYPE) {
                return DEVMM_MAP_ALIGN_SIZE;
            } else {
                return mgmt->svm_page_size;
            }
        }
    }
}

STATIC uint64_t devmm_virt_get_heap_size(uint32_t heap_type, uint32_t heap_sub_type)
{
    (void)heap_type;
    (void)heap_sub_type;
    return DEVMM_HEAP_SIZE;
}

void devmm_virt_heap_update_info(struct devmm_virt_heap_mgmt *mgmt, struct devmm_virt_com_heap *heap,
    struct devmm_virt_heap_type *heap_type, struct devmm_com_heap_ops *ops,
    struct devmm_virt_heap_para *heap_info)
{
    (void)mgmt;
    heap->chunk_size = heap_info->page_size;
    heap->kernel_page_size = heap_info->kernel_page_size;
    heap->ops = ops;
    heap->heap_type = heap_type->heap_type;
    heap->heap_list_type = heap_type->heap_list_type;
    heap->heap_sub_type = heap_type->heap_sub_type;
    heap->heap_mem_type = heap_type->heap_mem_type;
    heap->start = heap_info->start;
    heap->end = heap->start + heap_info->heap_size - 1;
    heap->heap_size = heap_info->heap_size;
    /* need_cache_thres£¬map_size and other members not used in primary heap */
    DEVMM_DRV_SWITCH("Heap info. (heap_type=0x%x; list_type=%u; sub_type=0x%x; heap_size=%llu; chunk_size=%u)\n",
        heap_type->heap_type, heap_type->heap_list_type, heap_type->heap_sub_type,
        heap->heap_size, heap->chunk_size);
}

void devmm_virt_normal_heap_update_info(struct devmm_virt_heap_mgmt *mgmt, struct devmm_virt_com_heap *heap,
    struct devmm_virt_heap_type *heap_type, struct devmm_com_heap_ops *ops, uint64_t alloc_size)
{
    struct devmm_virt_heap_para heap_info;

    heap_info.start = mgmt->start + heap->heap_idx * DEVMM_HEAP_SIZE;
    heap_info.heap_size = alloc_size;
    heap_info.page_size = devmm_virt_get_page_size_by_heap_type(mgmt,
        heap_type->heap_type, heap_type->heap_sub_type);
    heap_info.kernel_page_size = devmm_virt_get_kernel_page_size_by_heap_type(mgmt,
        heap_type->heap_type, heap_type->heap_sub_type);
    devmm_virt_heap_update_info(mgmt, heap, heap_type, ops, &heap_info);
}

static inline bool devmm_virt_heap_resource_is_reservation(uint32_t heap_sub_type)
{
    if ((heap_sub_type == SUB_DVPP_TYPE) || (heap_sub_type == SUB_READ_ONLY_TYPE) ||
        (heap_sub_type == SUB_DEV_READ_ONLY_TYPE)) {
        return true;
    }

    return false;
}

static inline DVresult devmm_virt_free_heap_to_base(struct devmm_virt_heap_mgmt *mgmt, virt_addr_t heap_index)
{
    virt_addr_t ptr = heap_index * DEVMM_HEAP_SIZE + mgmt->start;
    DEVMM_DRV_SWITCH("Put heap to base heap. (ptr=0x%lx; heap_index=%lu)\n", ptr, heap_index);
    return devmm_virt_free_mem_to_base(mgmt, ptr);
}

DVresult devmm_virt_set_heap_idle(struct devmm_virt_heap_mgmt *mgmt, struct devmm_virt_com_heap *heap)
{
    uint32_t i, heap_num;

    heap_num = (uint32_t)(heap->heap_size / DEVMM_HEAP_SIZE);
    for (i = 0; i < heap_num; i++) {
        mgmt->heap_queue.heaps[(uint64_t)heap->heap_idx + i] = NULL;
    }
    if (devmm_virt_heap_resource_is_reservation(heap->heap_sub_type) == false) {
        if (devmm_virt_free_heap_to_base(mgmt, heap->heap_idx) != DRV_ERROR_NONE) {
            return DRV_ERROR_INNER_ERR;
        }
    }
    (void)pthread_mutex_destroy(&heap->tree_lock);
    (void)pthread_rwlock_destroy(&heap->heap_rw_lock);

    free(heap);
    heap = NULL;

    return DRV_ERROR_NONE;
}

struct devmm_virt_com_heap *devmm_virt_get_heap_from_queue(struct devmm_virt_heap_mgmt *mgmt,
    uint32_t heap_idx, size_t heap_size)
{
    struct devmm_virt_com_heap *tem_heap = NULL;
    uint32_t heap_num, i;
    int ret;

    if ((heap_size % DEVMM_HEAP_SIZE) != 0) {
        return NULL;
    }
    heap_num = (uint32_t)(heap_size / DEVMM_HEAP_SIZE);
    if ((heap_idx < DEVMM_MAX_HEAP_NUM) && ((heap_idx + heap_num) <= DEVMM_MAX_HEAP_NUM) &&
        (mgmt->heap_queue.heaps[heap_idx] == NULL)) {
        tem_heap = malloc(sizeof(struct devmm_virt_com_heap));
        if (tem_heap == NULL) {
            DEVMM_DRV_ERR("Malloc heap fail. (idx=%u; size=%lu)\n", heap_idx, sizeof(struct devmm_virt_com_heap));
            return NULL;
        }

        ret = memset_s(tem_heap, sizeof(struct devmm_virt_com_heap), 0, sizeof(struct devmm_virt_com_heap));
        if (ret != 0) {
#ifndef EMU_ST
            DEVMM_DRV_ERR("Memset_s return error. (tem_heap=%p; ret=%d)\n", tem_heap, ret);
            free(tem_heap);
            return NULL;
#endif
        }

        tem_heap->heap_idx = heap_idx;
        tem_heap->rbtree_queue = (struct devmm_heap_rbtree){0};
        (void)pthread_mutex_init(&tem_heap->tree_lock, NULL);
        (void)pthread_rwlock_init(&tem_heap->heap_rw_lock, NULL);
    } else {
        DEVMM_DRV_ERR("Input error, or heap is NULL. (idx=%u; size=%lu)\n", heap_idx, heap_size);
        return NULL;
    }
    for (i = 0; i < heap_num; i++) {
        mgmt->heap_queue.heaps[(uint64_t)i + heap_idx] = tem_heap;
    }

    return tem_heap;
}

DVresult devmm_virt_init_heap_customize(struct devmm_virt_heap_mgmt *mgmt,
    struct devmm_virt_heap_type *heap_type,
    struct devmm_virt_heap_para *heap_info,
    struct devmm_com_heap_ops *ops)
{
    struct devmm_heap_list *heap_list = NULL;
    struct devmm_virt_com_heap *heap;
    DVresult ret;

    if (devmm_is_snapshot_state()) {
        return DRV_ERROR_NONE;
    }

    if (devmm_get_heap_list_by_type(mgmt, heap_type, &heap_list) != DRV_ERROR_NONE) {
        return DRV_ERROR_INNER_ERR;
    }
    (void)pthread_rwlock_wrlock(&heap_list->list_lock);
    if (devmm_virt_get_heap_mgmt_virt_heap(devmm_va_to_heap_idx(mgmt, heap_info->start)) != NULL) {
        (void)pthread_rwlock_unlock(&heap_list->list_lock);
        return DRV_ERROR_NONE;
    }
    heap = devmm_virt_get_heap_from_queue(mgmt,
        devmm_va_to_heap_idx(mgmt, heap_info->start), heap_info->heap_size);
    if (heap == NULL) {
        (void)pthread_rwlock_unlock(&heap_list->list_lock);
        return DRV_ERROR_INNER_ERR;
    }

    ret = devmm_virt_init_com_heap(heap, heap_type, ops, heap_info);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Init com heap fail. (ret=%u)\n", ret);
        (void)devmm_virt_set_heap_idle(mgmt, heap);
        (void)pthread_rwlock_unlock(&heap_list->list_lock);
        return ret;
    }

    devmm_virt_list_add(&heap->list, &heap_list->heap_list);
    heap_list->heap_cnt++;
    (void)pthread_rwlock_unlock(&heap_list->list_lock);
    return DRV_ERROR_NONE;
}

STATIC struct devmm_virt_com_heap *devmm_virt_get_heap_from_base(struct devmm_virt_heap_mgmt *mgmt,
    size_t heap_size, uint64_t va)
{
    uint32_t heap_idx;
    virt_addr_t ptr;

    ptr = devmm_virt_alloc_mem_from_base(mgmt, heap_size, 0, va);
    if (ptr < DEVMM_SVM_MEM_START) {
        if (devmm_is_specified_va_alloc(va) == false) {
            DEVMM_DRV_ERR("Alloc heap from base heap error.\n");
        }
        return NULL;
    }
    DEVMM_DRV_SWITCH("Alloc heap from base heap. (ptr=0x%lx; size=%lu)\n", ptr, heap_size);
    heap_idx = devmm_va_to_heap_idx(mgmt, ptr);

    return devmm_virt_get_heap_from_queue(mgmt, heap_idx, heap_size);
}

static struct devmm_virt_com_heap *devmm_virt_get_free_heap(struct devmm_virt_heap_mgmt *mgmt,
    struct devmm_virt_heap_type *heap_type, size_t heap_size, uint64_t va)
{
    (void)heap_type;
    return devmm_virt_get_heap_from_base(mgmt, heap_size, va);
}

DVresult devmm_virt_destroy_heap(struct devmm_virt_heap_mgmt *mgmt, struct devmm_virt_com_heap *heap, bool need_mem_stats_dec)
{
    (void)pthread_rwlock_wrlock(&heap->heap_rw_lock);
    /* host_pin pre_register_dma need to unregister before free pages, and host_pin will not fail in devmm_ioctl_disable_heap */
    if ((heap->heap_sub_type == SUB_HOST_TYPE) && (devmm_virt_heap_is_primary(heap) == false)) {
        devmm_rbtree_destory(&heap->rbtree_queue);
    }
    if (devmm_ioctl_disable_heap(heap->heap_idx, heap->heap_type,
        heap->heap_sub_type, heap->heap_size) != 0) {
        (void)pthread_rwlock_unlock(&heap->heap_rw_lock);
        DEVMM_DRV_ERR("Devmm_virt_ioctl_update_heap error.\n");
        return DRV_ERROR_INVALID_VALUE;
    }
    if ((heap->heap_sub_type != SUB_HOST_TYPE) && (devmm_virt_heap_is_primary(heap) == false)) {
        devmm_rbtree_destory(&heap->rbtree_queue);
    }
    if (need_mem_stats_dec) {
        devmm_primary_heap_module_mem_stats_dec(heap);
        devmm_mem_mapped_size_dec(heap, heap->mapped_size);
    }
    (void)pthread_rwlock_unlock(&heap->heap_rw_lock);

    return devmm_virt_set_heap_idle(mgmt, heap);
}

static DVresult devmm_alloc_com_heap(struct devmm_virt_heap_type *heap_type, uint64_t va,
    struct devmm_virt_com_heap **ret_heap)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    struct devmm_virt_com_heap *heap = NULL;
    struct devmm_virt_heap_para heap_info;
    uint64_t heap_size;
    DVresult ret_val;

    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Get heap mangement error.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (devmm_virt_heap_resource_is_reservation(heap_type->heap_sub_type) == true) {
        DEVMM_DRV_INFO("Mem type can not new more heap. (sub_type=%u)\n", heap_type->heap_sub_type);
        return DRV_ERROR_INVALID_VALUE;
    }
    heap_size = devmm_virt_get_heap_size(heap_type->heap_type, heap_type->heap_sub_type);
    /* 1. get heap and prepare for heap_info */
    heap = devmm_virt_get_free_heap(mgmt, heap_type, heap_size, va);
    if (heap == NULL) {
        if (devmm_is_specified_va_alloc(va) == false) {
            DEVMM_DRV_ERR("Get free heap fail. (heap_size=%llu)\n", heap_size);
            return DRV_ERROR_INNER_ERR;
        } else {
            return DRV_ERROR_BUSY;
        }
    }

    devmm_virt_status_init(heap);
    heap_info.start = mgmt->start + heap->heap_idx * DEVMM_HEAP_SIZE;
    heap_info.heap_size = heap_size;
    heap_info.page_size = devmm_virt_get_page_size_by_heap_type(mgmt,
        heap_type->heap_type, heap_type->heap_sub_type);
    heap_info.kernel_page_size = devmm_virt_get_kernel_page_size_by_heap_type(mgmt,
        heap_type->heap_type, heap_type->heap_sub_type);
    heap_info.need_cache_thres[DEVMM_MEM_NORMAL] = devmm_virt_get_cache_size_by_heap_type(heap_type);
    heap_info.map_size = 0; /* No extra cache will be generated. */
    heap_info.is_limited = false;
    heap_info.is_base_heap = false;
    /* 2. init heap info, init rbtree queue and enable heap */
    ret_val = devmm_virt_init_com_heap(heap, heap_type, &g_heap_ops[heap_type->heap_sub_type], &heap_info);
    if (ret_val != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Init com heap error. (ret_val=%d)\n", ret_val);
        (void)devmm_virt_set_heap_idle(mgmt, heap);
        return ret_val;
    }

    *ret_heap = heap;
    return DRV_ERROR_NONE;
}

static void devmm_add_heap_to_list(struct devmm_virt_com_heap *heap, struct devmm_heap_list *heap_list)
{
    devmm_virt_list_add(&heap->list, &heap_list->heap_list);
    heap_list->heap_cnt++;
    DEVMM_DRV_SWITCH("Add heap list success. (heap_idx=%d)\n", heap->heap_idx);
}

bool devmm_virt_check_idle_heap(struct devmm_virt_com_heap *heap)
{
    /*
     * accumulative allocated physical(mapped) memory =
     * accumulative released physical(mapped) memory + current remaining cache
    */
    if (heap->sys_mem_alloced == (heap->sys_mem_freed + heap->cur_cache_mem[DEVMM_MEM_NORMAL])) {
        return DEVMM_TRUE;
    }

    return DEVMM_FALSE;
}

static DVdeviceptr devmm_alloc_from_trees(struct devmm_heap_list *heap_list,
    size_t bytesize, DVmem_advise advise, uint32_t tree_type, uint64_t va)
{
    struct devmm_virt_list_head *pos = NULL;
    struct devmm_virt_com_heap *heap = NULL;
    DVdeviceptr ptr = DEVMM_OUT_OF_VIRT_MEM;
    uint32_t memtype = advise_to_memtype(advise);

    devmm_virt_list_for_each(pos, &heap_list->heap_list)
    {
        heap = devmm_virt_list_entry(pos, struct devmm_virt_com_heap, list);
        /* alloc form base heap ptr will add to heap list and rbtree will be null */
        if ((heap == NULL) || devmm_virt_heap_is_primary(heap) || (bytesize > heap->heap_size)) {
            continue;
        }

        ptr = devmm_alloc_from_tree(heap, bytesize, advise, tree_type, va);
        if (ptr_is_valid(ptr)) {
            break;
        }

        if ((get_ptr_err(ptr) == DEVMM_OUT_OF_PHYS_MEM) || (get_ptr_err(ptr) == DEVMM_NO_DEVICE) ||
            (get_ptr_err(ptr) == DEVMM_DEV_PROC_EXIT)) {
            break;
        }

        if ((get_ptr_err(ptr) == DEVMM_OUT_OF_VIRT_MEM) && (heap->is_limited == true) &&
            (tree_type == DEVMM_IDLE_SIZE_TREE)) {
            DEVMM_DRV_INFO("Out of virt mem, check mem usage. (size=%lu; memtype=%u; heap_size=%llu; "
                "need_cache_thres=%lu; heap_mem_alloced=%llu; freed=%llu; cur_cache=%llu; chunk_size=%u)\n",
                bytesize, memtype, heap->heap_size, heap->need_cache_thres[memtype], heap->sys_mem_alloced,
                heap->sys_mem_freed, heap->cur_cache_mem[memtype], heap->chunk_size);
        }
    }
    return ptr;
}

static DVdeviceptr _devmm_alloc_from_heaplist(struct devmm_heap_list *heap_list,
    size_t bytesize, DVmem_advise advise, uint64_t va)
{
    DVdeviceptr ptr;

    if (advise_is_nocache(advise) == false) {
        ptr = devmm_alloc_from_trees(heap_list, bytesize, advise, DEVMM_IDLE_MAPPED_TREE, va);
        if (ptr_is_valid(ptr)) {
            return ptr;
        }
    }

    return devmm_alloc_from_trees(heap_list, bytesize, advise, DEVMM_IDLE_SIZE_TREE, va);
}

/* If heaplist is out of virt mem, new heap to alloc */
static DVdeviceptr devmm_alloc_from_heaplist(struct devmm_heap_list *heap_list,
    struct devmm_virt_heap_type *heap_type, size_t bytesize, DVmem_advise advise, uint64_t va)
{
    struct devmm_virt_com_heap *heap = NULL;
    DVdeviceptr ptr;
    DVresult ret;

    ptr = _devmm_alloc_from_heaplist(heap_list, bytesize, advise, va);
    if (ptr_is_valid(ptr)) {
        return ptr;
    }

    if (get_ptr_err(ptr) == DEVMM_OUT_OF_VIRT_MEM) {
        ret = devmm_alloc_com_heap(heap_type, va, &heap);
        if (ret != DRV_ERROR_NONE) {
            return errcode_to_ptr(ret, DEVMM_INVALID_STOP);
        }

        ptr = devmm_alloc_from_tree(heap, bytesize, advise, DEVMM_IDLE_SIZE_TREE, va);
        devmm_add_heap_to_list(heap, heap_list);
    }
    return ptr;
}

virt_addr_t devmm_alloc_from_normal_heap(struct devmm_virt_heap_mgmt *p_heap_mgmt, size_t bytesize,
    struct devmm_virt_heap_type *heap_type, DVmem_advise advise, DVdeviceptr va)
{
    struct devmm_heap_list *heap_list = NULL;
    DVdeviceptr ptr;
    DVresult ret;

    ret = devmm_get_heap_list_by_type(p_heap_mgmt, heap_type, &heap_list);
    if (ret != DRV_ERROR_NONE) {
        return DEVMM_INVALID_STOP;
    }

    /* To improve the perf of concurrency, hold with read lock. */
    (void)pthread_rwlock_rdlock(&heap_list->list_lock);
    ptr = _devmm_alloc_from_heaplist(heap_list, bytesize, advise, va);
    (void)pthread_rwlock_unlock(&heap_list->list_lock);
    if (ptr_is_valid(ptr)) {
        return ptr;
    }

    if (get_ptr_err(ptr) == DEVMM_OUT_OF_VIRT_MEM) {
        /*
         * Note that the write lock should be held,
         * otherwise alloc svm heap will fail if there are too many concurrent threads.
         */
        (void)pthread_rwlock_wrlock(&heap_list->list_lock);
        ptr = devmm_alloc_from_heaplist(heap_list, heap_type, bytesize, advise, va);
        (void)pthread_rwlock_unlock(&heap_list->list_lock);
    }

    return ptr;
}

