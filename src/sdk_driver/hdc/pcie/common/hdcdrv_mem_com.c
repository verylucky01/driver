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
#include "ka_compiler_pub.h"
#include "ka_driver_pub.h"
#include "ka_list_pub.h"
#include "ka_memory_pub.h"
#include "pbl_ka_memory.h"
#include "hdcdrv_core_com.h"
#include "comm_kernel_interface.h"
#include "kernel_version_adapt.h"
#include "hdcdrv_mem_com.h"

STATIC u32 g_mem_type = HDCDRV_NORMAL_MEM;
STATIC u32 g_mem_work_flag = 0;
STATIC u64 g_mem_work_cnt = 0;
STATIC struct hdcdrv_init_stamp g_hdcdrv_init_stamp = {0};
STATIC struct hdcdrv_alloc_pool_stamp g_alloc_pool_stamp = {0};
#define HDCDRV_HUGETLB_FOLIO_SIZE (2 * 1024 * 1024UL)
#define HDCDRV_HUGEPAGE_2M_ORDER 9

#define HDC_RESERVE_MEM_NUMA 31
void hdcdrv_init_stamp_init(void)
{
    g_hdcdrv_init_stamp.wait_mutex_start = 0;
    g_hdcdrv_init_stamp.wait_mutex_end = 0;
    g_hdcdrv_init_stamp.chan_alloc_start = 0;
    g_hdcdrv_init_stamp.chan_alloc_end = 0;
    g_hdcdrv_init_stamp.chan_memset_end = 0;
    g_hdcdrv_init_stamp.init_pool_start = 0;
    g_hdcdrv_init_stamp.alloc_mem_pool0 = 0;
    g_hdcdrv_init_stamp.alloc_mem_pool1 = 0;
    g_hdcdrv_init_stamp.alloc_mem_pool2 = 0;
    g_hdcdrv_init_stamp.alloc_mem_pool3 = 0;
    g_hdcdrv_init_stamp.init_pool_end = 0;
    g_hdcdrv_init_stamp.tasklet_start = 0;
    g_hdcdrv_init_stamp.tasklet_end = 0;
    g_hdcdrv_init_stamp.end = 0;
}
void hdcdrv_alloc_pool_stamp_init(void)
{
    g_alloc_pool_stamp.ring_alloc_start = 0;
    g_alloc_pool_stamp.ring_alloc_end = 0;
    g_alloc_pool_stamp.list_init_end = 0;
    g_alloc_pool_stamp.dma_alloc_start = 0;
    g_alloc_pool_stamp.dma_alloc_end = 0;
    g_alloc_pool_stamp.head_alloc_end = 0;
    g_alloc_pool_stamp.dma_alloc_max = 0;
    g_alloc_pool_stamp.head_alloc_max = 0;
    g_alloc_pool_stamp.dma_alloc_total = 0;
    g_alloc_pool_stamp.head_alloc_total = 0;
    g_alloc_pool_stamp.end = 0;
}
void hdcdrv_set_time_stamp(u64 *stamp)
{
    *stamp = ka_jiffies;
}
struct hdcdrv_init_stamp *hdcdrv_get_init_stamp_info(void)
{
    return &g_hdcdrv_init_stamp;
}
STATIC void hdcdrv_calc_alloc_pool_stamp(void)
{
    u64 dma_alloc_diff = g_alloc_pool_stamp.dma_alloc_end - g_alloc_pool_stamp.dma_alloc_start;
    u64 head_alloc_diff = g_alloc_pool_stamp.head_alloc_end - g_alloc_pool_stamp.dma_alloc_end;
    g_alloc_pool_stamp.dma_alloc_max = (g_alloc_pool_stamp.dma_alloc_max > dma_alloc_diff ?
                                        g_alloc_pool_stamp.dma_alloc_max :
                                        dma_alloc_diff);
    g_alloc_pool_stamp.head_alloc_max = (g_alloc_pool_stamp.head_alloc_max > head_alloc_diff ?
                                         g_alloc_pool_stamp.head_alloc_max :
                                         head_alloc_diff);
    g_alloc_pool_stamp.dma_alloc_total += dma_alloc_diff;
    g_alloc_pool_stamp.head_alloc_total += head_alloc_diff;
}
STATIC u32 hdcdrv_calc_time_cost(u64 end, u64 start)
{
    if (end <= start) {
        return 0;
    }
    return ka_system_jiffies_to_msecs(end - start);
}
STATIC void hdcdrv_alloc_pool_stamp_record(void)
{
    if (hdcdrv_calc_time_cost(g_alloc_pool_stamp.end, g_alloc_pool_stamp.ring_alloc_start) >
            HDCDRV_ALLOC_POOL_MAX_TIME) {
        hdcdrv_warn("alloc pool time. (total=%ums, alloc_ring=%ums, list_init=%ums, dma_total=%ums, "
                    "head_total=%ums, dma_max=%ums, head_max=%ums)\n",
                    hdcdrv_calc_time_cost(g_alloc_pool_stamp.end, g_alloc_pool_stamp.ring_alloc_start),
                    hdcdrv_calc_time_cost(g_alloc_pool_stamp.ring_alloc_end, g_alloc_pool_stamp.ring_alloc_start),
                    hdcdrv_calc_time_cost(g_alloc_pool_stamp.list_init_end, g_alloc_pool_stamp.ring_alloc_end),
                    ka_system_jiffies_to_msecs(g_alloc_pool_stamp.dma_alloc_total),
                    ka_system_jiffies_to_msecs(g_alloc_pool_stamp.head_alloc_total),
                    ka_system_jiffies_to_msecs(g_alloc_pool_stamp.dma_alloc_max),
                    ka_system_jiffies_to_msecs(g_alloc_pool_stamp.head_alloc_max));
    }
}
void hdcdrv_init_stamp_record()
{
    if (hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.end, g_hdcdrv_init_stamp.wait_mutex_start) >
            HDCDRV_INIT_TRANS_MAX_TIME) {
        hdcdrv_warn("init time. (total=%ums, wait_mutex=%ums, alloc_chan=%ums, memset=%ums, start_init_pool=%ums, "
                    "alloc_pool0=%ums ,alloc_pool1=%ums, alloc_pool2=%ums, alloc_pool3=%ums, total_alloc_pool=%ums, "
                    "tasklet_start=%ums, ka_system_tasklet_init=%ums, end=%ums)\n",
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.end, g_hdcdrv_init_stamp.wait_mutex_start),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.wait_mutex_end, g_hdcdrv_init_stamp.wait_mutex_start),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.chan_alloc_end, g_hdcdrv_init_stamp.chan_alloc_start),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.chan_memset_end, g_hdcdrv_init_stamp.chan_alloc_end),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.init_pool_start, g_hdcdrv_init_stamp.chan_memset_end),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.alloc_mem_pool0, g_hdcdrv_init_stamp.init_pool_start),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.alloc_mem_pool1, g_hdcdrv_init_stamp.alloc_mem_pool0),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.alloc_mem_pool2, g_hdcdrv_init_stamp.alloc_mem_pool1),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.alloc_mem_pool3, g_hdcdrv_init_stamp.alloc_mem_pool2),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.init_pool_end, g_hdcdrv_init_stamp.init_pool_start),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.tasklet_start, g_hdcdrv_init_stamp.init_pool_end),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.tasklet_end, g_hdcdrv_init_stamp.tasklet_start),
                    hdcdrv_calc_time_cost(g_hdcdrv_init_stamp.end, g_hdcdrv_init_stamp.tasklet_end));
    }
}

ka_page_t *hdcdrv_alloc_pages_node(u32 dev_id, ka_gfp_t gfp_mask, u32 order)
{
    return hdcdrv_alloc_pages_node_inner(dev_id, gfp_mask, order);
}

void *hdcdrv_kzalloc_mem_node(u32 dev_id, ka_gfp_t gfp_mask, u32 size, u32 level)
{
    return hdcdrv_kzalloc_mem_node_inner(dev_id, gfp_mask, size, level);
}

void *hdcdrv_kvmalloc(size_t size, int level)
{
    void *addr = NULL;

    if (size == 0) {
        return NULL;
    }

    addr = hdcdrv_kzalloc(size, KA_GFP_NOWAIT | __KA_GFP_NOWARN | __KA_GFP_ACCOUNT | KA_GFP_KERNEL, level);
    if (addr == NULL) {
        addr = hdcdrv_vmalloc(size, KA_GFP_KERNEL | __KA_GFP_ACCOUNT | __KA_GFP_ZERO, KA_PAGE_KERNEL, level);
        if (addr == NULL) {
            return NULL;
        }
    }

    return addr;
}

void hdcdrv_kvfree(void **addr, int level)
{
    if ((addr == NULL) || (*addr == NULL)) {
        return;
    }
    if (ka_mm_is_vmalloc_addr(*addr)) {
        hdcdrv_vfree(*addr, level);
    } else {
        hdcdrv_kfree(*addr, level);
    }
    *addr = NULL;
}

static inline u32 hdcdrv_mem_block_head_crc32(const struct hdcdrv_mem_block_head *block_head)
{
    return ka_base_crc32_le(~0u, (unsigned char *)block_head, HDCDRV_BLOCK_CRC_LEN);
}

STATIC void hdcdrv_mem_block_head_init(void *buf, ka_dma_addr_t addr, u32 offset, struct hdccom_mem_init *init_mem, u32 segment)
{
    struct hdcdrv_mem_block_head *block_head = (struct hdcdrv_mem_block_head *)buf;

    block_head->magic = HDCDRV_MEM_BLOCK_MAGIC;
    block_head->devid  = (u32)init_mem->dev_id;
    block_head->type = (u32)init_mem->pool_type;
    block_head->size = segment;
    block_head->offset = offset;
    block_head->dma_addr = addr;
    block_head->head_crc = hdcdrv_mem_block_head_crc32(block_head);
    block_head->ref_count = 0;
}

STATIC void hdcdrv_mem_block_head_dump(struct hdcdrv_mem_block_head *block_head)
{
    hdcdrv_err_spinlock("Critical error, memory block head is corrupted.\n"
                        "(magic=%x; "
                        "devid=%x; "
                        "type=%x; "
                        "size=%x; "
                        "dma_addr=(no print); "
                        "head_crc=%x; "
                        "ref_count=%x; "
                        "current crc=%x)\n",
                        block_head->magic, block_head->devid, block_head->type,
                        block_head->size, block_head->head_crc, block_head->ref_count,
                        hdcdrv_mem_block_head_crc32(block_head));
#ifdef CFG_BUILD_DEBUG
    ka_base_dump_stack();
#endif
}

int hdcdrv_mem_block_head_check(void *buf)
{
    struct hdcdrv_mem_block_head *block_head = NULL;

    if (buf == NULL) {
        hdcdrv_err_spinlock("Input parameter is error.\n");
        return HDCDRV_ERR;
    }

    block_head = HDCDRV_BLOCK_HEAD(buf);
    if (block_head == NULL) {
        hdcdrv_err_spinlock("Calling HDCDRV_BLOCK_HEAD failed.\n");
        return HDCDRV_ERR;
    }

    if ((block_head->magic != HDCDRV_MEM_BLOCK_MAGIC) ||
        (block_head->head_crc != hdcdrv_mem_block_head_crc32(block_head))) {
        hdcdrv_mem_block_head_dump(block_head);
        hdcdrv_err_spinlock("Memory block head check failed.\n");
        return HDCDRV_ERR;
    }

    return HDCDRV_OK;
}

#ifdef CFG_FEATURE_SMALL_HUGE_POOL
STATIC bool is_alloc_mirror_mem(u32 dev_id, u32 segment)
{
    u32 chip_type;

    if (devdrv_get_pfvf_type_by_devid(dev_id) == DEVDRV_SRIOV_TYPE_VF) {
        return false;
    }

    chip_type = uda_get_chip_type(dev_id);
    /* from v4/v5, huge mem pool not use mirror mem */
    if ((chip_type >= HISI_CLOUD_V4) && (chip_type < HISI_CHIP_UNKNOWN) && (segment > HDCDRV_SMALL_PACKET_SEGMENT)) {
        return false;
    }

    return true;
}
#endif

void free_mem_pool_single(ka_device_t *dev, u32 segment, struct hdcdrv_mem_block_head *buf, ka_dma_addr_t addr)
{
    if ((buf == NULL) || (buf->dma_buf == NULL) || (addr == KA_DMA_MAPPING_ERROR) || (dev == NULL)) {
        return;
    }

    if ((buf->type == HDCDRV_RESERVE_MEM_POOL_TYPE_TX) || (buf->type == HDCDRV_RESERVE_MEM_POOL_TYPE_RX)) {
        buf->dma_buf = NULL;
        addr = 0;
    } else {
#ifdef CFG_FEATURE_SMALL_HUGE_POOL
        if ((is_alloc_mirror_mem(buf->devid, segment) == false) && (buf->pool_page != NULL)) {
            hal_kernel_devdrv_dma_unmap_page(dev, addr, segment, KA_DMA_BIDIRECTIONAL);
            hdcdrv_free_pages_ex(buf->pool_page, ka_mm_get_order(segment), KA_SUB_MODULE_TYPE_2);
            buf->pool_page = NULL;
        } else {
            hal_kernel_devdrv_dma_free_coherent(dev, segment, buf->dma_buf, addr);
        }
#else
        hal_kernel_devdrv_dma_free_coherent(dev, segment, buf->dma_buf, addr);
#endif
    }
    buf->dma_buf = NULL;
    hdcdrv_kvfree((void **)&buf, KA_SUB_MODULE_TYPE_0);
    buf = NULL;
}

static inline u32 hdccom_calc_ring_id(u64 ring_cnt, u32 mask, u32 size)
{
    if (size == 0) {
        hdcdrv_err_spinlock("Input parameter is error.\n");
        return 0;
    }

    return (mask != 0) ? (ring_cnt & mask) : (u32)(ring_cnt % size);
}

int hdccom_alloc_mem(struct hdcdrv_mem_pool *pool, void **buf, ka_dma_addr_t *addr, u32 *offset)
{
    struct hdcdrv_mem_block_head *block_head = NULL;
    u32 ring_id;

    ka_task_spin_lock_bh(&pool->mem_lock);

    if (pool->valid != HDCDRV_VALID) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        hdcdrv_err("pool is invalid. (devid=%u)\n", pool->dev_id);
        return HDCDRV_ERR;
    }

    if (pool->head == pool->tail) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        return HDCDRV_DMA_MEM_ALLOC_FAIL;
    }

    ring_id = hdccom_calc_ring_id(pool->head, pool->mask, pool->size);
    if (hdcdrv_mem_block_head_check(pool->ring[ring_id].buf) != HDCDRV_OK) {
        pool->head++;
        block_head = pool->ring[ring_id].buf;
        ka_task_spin_unlock_bh(&pool->mem_lock);
        if (block_head != NULL) {
            hdcdrv_err("Block head is corrupted. (magic=%u; devid=%u; type=%u; size=%u; head_crc=%u; current crc=%u)\n",
                block_head->magic, block_head->devid, block_head->type, block_head->size, block_head->head_crc,
                hdcdrv_mem_block_head_crc32(block_head));
        }
        hdcdrv_err("Block head check failed. (devid=%u; ring_id=%d)\n", pool->dev_id, ring_id);
        return HDCDRV_MEM_NOT_MATCH;
    }

    block_head = pool->ring[ring_id].buf;
    if (block_head->ref_count != 0) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        hdcdrv_mem_block_head_dump(block_head);
        hdcdrv_err("Memory block ref_count error. (devid=%u; ref_count=%x)\n", pool->dev_id, block_head->ref_count);
        return HDCDRV_MEM_NOT_MATCH;
    }
    block_head->ref_count++;
    ka_base_atomic_set(&block_head->status, HDCDRV_BLOCK_STATE_NORMAL);

    *buf = pool->ring[ring_id].buf;
    *addr = block_head->dma_addr;
    *offset = block_head->offset;

    pool->ring[ring_id].buf = NULL;

    pool->head++;
    ka_task_spin_unlock_bh(&pool->mem_lock);

    return HDCDRV_OK;
}

int hdccom_free_mem(struct hdcdrv_mem_pool *pool, void *buf)
{
    struct hdcdrv_mem_block_head *block_head = NULL;
    u32 ring_id;

    ka_task_spin_lock_bh(&pool->mem_lock);

    block_head = (struct hdcdrv_mem_block_head *)buf;
    if (block_head->ref_count != 1) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        hdcdrv_mem_block_head_dump(block_head);
        hdcdrv_err_spinlock("Memory block ref_count error. (current=%x)\n", block_head->ref_count);
        return HDCDRV_MEM_NOT_MATCH;
    }
    block_head->ref_count--;

    ring_id = hdccom_calc_ring_id(pool->tail, pool->mask, pool->size);
    pool->ring[ring_id].buf = buf;
    pool->tail++;

    ka_task_spin_unlock_bh(&pool->mem_lock);
    return HDCDRV_OK;
}

#ifdef CFG_FEATURE_MIRROR
void hdcdrv_page_head_init(struct hdcdrv_huge_page *page_head, ka_page_t * page_addr, void *buf, int valid_flag,
    int alloc_flag)
{
    int i = 0;
    page_head->page_addr = page_addr;
    page_head->used_block_num = 0;
    page_head->buf = buf;
    for (i = 0; i < HDCDRV_PAGE_BLOCK_NUM; i++) {
        page_head->used_block[i] = HDCDRV_BLOCK_IS_IDLE;
    }
    page_head->valid = valid_flag;
    page_head->alloc_flag = alloc_flag;
    return;
}

void hdccom_get_page_index_and_block_id(int mem_id, int *page_index, int *block_id)
{
    *page_index = mem_id / HDCDRV_PAGE_BLOCK_NUM;
    *block_id = mem_id % HDCDRV_PAGE_BLOCK_NUM;
}

int hdccom_get_mem_id(int page_index, int block_id)
{
    return (page_index * HDCDRV_PAGE_BLOCK_NUM + block_id);
}

void hdcdrv_free_single_page(ka_page_t *page_addr, int alloc_flag)
{
    ka_free_single_page(page_addr, HDCDRV_HUGEPAGE_2M_ORDER, alloc_flag, HAL_MODULE_TYPE_HDC, KA_SUB_MODULE_TYPE_1);
}

STATIC u32 g_alloc_huge_page_print_cnt = 0;
STATIC u64 g_alloc_huge_page_jiffies = 0;
int hdcdrv_alloc_huge_page(struct hdcdrv_mem_pool *pool, int page_index)
{
    ka_page_t *page = NULL;
    int i;
    int nids[HDC_NID_ID_MAX_NUM] = {0};
    int node_num;
    void* buf;
    ka_dma_addr_t addr;
    ka_device_t* pdev_dev = hdcdrv_get_pdev_dev(pool->dev_id);
    int alloc_flag;

    if (pdev_dev == NULL) {
        hdcdrv_err("pdev_dev is invalid.\n");
        pool->page_list[page_index].valid = HDCDRV_PAGE_NOT_ALLOC;
        return HDCDRV_ERR;
    }

    if (pool->page_list[page_index].valid != HDCDRV_PAGE_PRE_STATUS) {
        HDC_LOG_ERR_LIMIT(&g_alloc_huge_page_print_cnt, &g_alloc_huge_page_jiffies,
            "page status is not correct, now page_statu is %d.\n", pool->page_list[page_index].valid);
        pool->page_list[page_index].valid = HDCDRV_PAGE_NOT_ALLOC;
        return HDCDRV_PARA_ERR;
    }

    // get memory from numa id
    node_num = hal_kernel_dbl_get_ai_nid(pool->dev_id, nids, HDC_NID_ID_MAX_NUM);
    if (node_num <= 0) {
        HDC_LOG_ERR_LIMIT(&g_alloc_huge_page_print_cnt, &g_alloc_huge_page_jiffies,
            "Failed to get node_num. (device_id=%u;node_num=%d)\n", pool->dev_id, node_num);
        pool->page_list[page_index].valid = HDCDRV_PAGE_NOT_ALLOC;
        return HDCDRV_GET_NUMA_ID_FAILED;
    }
    // use NORECLAIM flag for the first memory request, which has better performance
    for (i = 0; i < node_num; i++) {
        page = ka_alloc_hugetlb(nids[i], HDCDRV_HUGETLB_FOLIO_SIZE, HUGETLB_ALLOC_NORECLAIM,
                                HAL_MODULE_TYPE_HDC, KA_SUB_MODULE_TYPE_1);
        if (page != NULL) {
            alloc_flag = HDCDRV_ALLOC_MEM_BY_HUGE_PAGE;
            break;
        }
    }

    if (page == NULL) {
    // use NONE flag for the second memory request, if page == NULL means the memory is exhausted
        for (i = 0; i < node_num; i++) {
            page = ka_alloc_hugepage(nids[i], (KA_GFP_KERNEL) | (__KA_GFP_COMP) | (__KA_GFP_ACCOUNT & ~__KA_GFP_RECLAIM) |
                                    (__KA_GFP_THISNODE) | (KA_GFP_HIGHUSER_MOVABLE), HDCDRV_HUGEPAGE_2M_ORDER,
                                    &alloc_flag, HUGETLB_ALLOC_NONE, HAL_MODULE_TYPE_HDC, KA_SUB_MODULE_TYPE_1);
            if (page != NULL) {
                break;
            }
        }

        if (page == NULL) {
            HDC_LOG_ERR_LIMIT(&g_alloc_huge_page_print_cnt, &g_alloc_huge_page_jiffies,
                "No enough huge page memory.\n");
            pool->page_list[page_index].valid = HDCDRV_PAGE_NOT_ALLOC;
            return HDCDRV_MEM_ALLOC_FAIL;
        }
    }
    // huge page init, fill the page index and head of each hdc_mem_block
    buf = ka_mm_page_address(page);
    addr = hal_kernel_devdrv_dma_map_page(pdev_dev, page, 0, KA_HPAGE_SIZE, KA_DMA_BIDIRECTIONAL);
    if (addr == KA_DMA_MAPPING_ERROR) {
        hdcdrv_free_single_page(page, alloc_flag);
        HDC_LOG_ERR_LIMIT(&g_alloc_huge_page_print_cnt, &g_alloc_huge_page_jiffies, "dma map failed.\n");
        pool->page_list[page_index].valid = HDCDRV_PAGE_NOT_ALLOC;
        return HDCDRV_DMA_MPA_FAIL;
    }

    hdcdrv_page_head_init(&pool->page_list[page_index], page, buf, HDCDRV_PAGE_HAS_ALLOC, alloc_flag);

    return HDCDRV_ALLOC_LATER_WITH_INDEX;
}

void hdccom_page_status_change(struct hdcdrv_mem_pool *pool, int page_index, int block_id, int flag)
{
    if (flag == HDCDRV_PAGE_BLOCK_ALLOC) {
        pool->page_list[page_index].used_block_num++;
        pool->page_list[page_index].used_block[block_id] = HDCDRV_BLOCK_IS_ALLOC;
        pool->used_block_all++;
    } else {
        pool->page_list[page_index].used_block_num--;
        pool->page_list[page_index].used_block[block_id] = HDCDRV_BLOCK_IS_IDLE;
        pool->used_block_all--;
    }
}

int hdccom_get_free_block_id(struct hdcdrv_mem_pool *pool, int page_index)
{
    int j, b_id = -1;

    for (j = 0; j < HDCDRV_PAGE_BLOCK_NUM; j++) {
        if (pool->page_list[page_index].used_block[j] == HDCDRV_BLOCK_IS_IDLE) {
            b_id = j;
            break;
        }
    }
    return b_id;
}

int hdccom_get_page_mem(struct hdcdrv_mem_pool *pool, int *page_index, int *block_id, void** alloc_buf)
{
    int i, p_index = -1, b_id = -1, pre_id = -1, idle_id = -1;
    void* buf = NULL;
    int ret;

    for (i = 0; i < HDCDRV_HUGE_PAGE_NUM ; i++) {
        if ((pool->page_list[i].valid == HDCDRV_PAGE_HAS_ALLOC) &&
            (pool->page_list[i].used_block_num < HDCDRV_PAGE_BLOCK_NUM)) {
            p_index = i;
            b_id = hdccom_get_free_block_id(pool, i);
            break;
        }
        if ((pool->page_list[i].valid == HDCDRV_PAGE_PRE_STATUS) && (pre_id == -1)) {
            pre_id = i;
        } else if ((pool->page_list[i].valid == HDCDRV_PAGE_NOT_ALLOC) && (idle_id == -1)) {
            idle_id = i;
        }
    }

    if (p_index != -1) { // has free block
        buf = HDCDRV_BLOCK_BUFFER(pool->page_list[p_index].buf + b_id * HDCDRV_HUGE_PACKET_SEGMENT);
        hdccom_page_status_change(pool, p_index, b_id, HDCDRV_PAGE_BLOCK_ALLOC);
        *page_index = p_index;
        *block_id = b_id;
        ret = HDCDRV_OK;
    } else if (pre_id != -1) { // this page prepare to alloc, just wait and retry
        ret = HDCDRV_ALLOC_AGAIN;
    } else if (idle_id != -1) { // need to allocate new page, recore page_index then alloc out of spinlock
        pool->page_list[idle_id].valid = HDCDRV_PAGE_PRE_STATUS;
        *page_index = idle_id;
        ret = HDCDRV_ALLOC_LATER_WITH_INDEX;
    } else {
        ret = HDCDRV_DMA_MEM_ALLOC_FAIL;
    }

    *alloc_buf = buf;
    return ret;
}

void free_mem_pool_single_page(ka_device_t *dev, struct hdcdrv_mem_block_head *buf,
    ka_dma_addr_t addr, ka_page_t *page_addr, int alloc_flag)
{
    if ((buf != NULL) && (addr != KA_DMA_MAPPING_ERROR) && (page_addr != NULL) && (dev != NULL)) {
        hal_kernel_devdrv_dma_unmap_page(dev, addr, KA_HPAGE_SIZE, KA_DMA_BIDIRECTIONAL);
        hdcdrv_free_single_page(page_addr, alloc_flag);
    }
}

STATIC u32 g_alloc_mem_print_cnt = 0;
STATIC u64 g_alloc_mem_jiffies = 0;
int hdccom_alloc_mem_page(struct hdcdrv_mem_pool *pool, void **buf, ka_dma_addr_t *addr, int *mem_id)
{
    struct hdcdrv_mem_block_head *block_head = NULL;
    ka_page_t* page_addr = NULL;
    int page_index = -1, block_id = -1, alloc_flag;
    void* alloc_buf = NULL;
    int ret;

    ka_task_spin_lock_bh(&pool->mem_lock);

    if (pool->valid != HDCDRV_VALID) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        hdcdrv_err("pool is invalid. (devid=%u)\n", pool->dev_id);
        return HDCDRV_ERR;
    }

    if (pool->used_block_all == HDCDRV_PAGE_BLOCK_NUM_MAX) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        hdcdrv_warn("pool is full when alloc mem. (devid=%u)\n", pool->dev_id);
        return HDCDRV_DMA_MEM_ALLOC_FAIL;
    }

    ret = hdccom_get_page_mem(pool, &page_index, &block_id, &alloc_buf);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        if (ret == HDCDRV_ALLOC_LATER_WITH_INDEX) {
            ret = hdcdrv_alloc_huge_page(pool, page_index);
            if ((pool->valid != HDCDRV_VALID) && (ret == HDCDRV_ALLOC_AGAIN)) {
                /* To avoid alloc mem when dev remove, pool invalid and free mem immediately */
                block_head = pool->page_list[page_index].buf;
                page_addr = pool->page_list[page_index].page_addr;
                alloc_flag = pool->page_list[page_index].alloc_flag;
                hdcdrv_page_head_init(&pool->page_list[page_index], NULL, NULL, HDCDRV_PAGE_NOT_ALLOC,
                    HDCDRV_ALLOC_MEM_BY_HUGE_PAGE);
                free_mem_pool_single_page(hdcdrv_get_pdev_dev(pool->dev_id), block_head,
                    HDCDRV_BLOCK_DMA_HEAD(block_head->dma_addr), page_addr, alloc_flag);
                ret = HDCDRV_ERR;
            }
        }
        return ret;
    }

    *mem_id = hdccom_get_mem_id(page_index, block_id);

    if (hdcdrv_mem_block_head_check(alloc_buf) != HDCDRV_OK) {
        hdccom_page_status_change(pool, page_index, block_id, HDCDRV_PAGE_BLOCK_FREE);
        ka_task_spin_unlock_bh(&pool->mem_lock);
        HDC_LOG_ERR_LIMIT(&g_alloc_mem_print_cnt, &g_alloc_mem_jiffies,
            "Block head check failed. (mem_id=%d)\n", *mem_id);
        *mem_id = -1;
        return HDCDRV_MEM_NOT_MATCH;
    }

    block_head = HDCDRV_BLOCK_HEAD(alloc_buf);
    if (block_head->ref_count != 0) {
        hdccom_page_status_change(pool, page_index, block_id, HDCDRV_PAGE_BLOCK_FREE);
        ka_task_spin_unlock_bh(&pool->mem_lock);
        hdcdrv_mem_block_head_dump(block_head);
        HDC_LOG_ERR_LIMIT(&g_alloc_mem_print_cnt, &g_alloc_mem_jiffies, "Memory block ref_count error.\n");
        return HDCDRV_MEM_NOT_MATCH;
    }
    block_head->ref_count++;
    ka_base_atomic_set(&block_head->status, HDCDRV_BLOCK_STATE_NORMAL);

    *buf = alloc_buf;
    *addr = block_head->dma_addr;

    ka_task_spin_unlock_bh(&pool->mem_lock);

    return HDCDRV_OK;
}

int hdccom_free_mem_page(struct hdcdrv_mem_pool *pool, void *buf, int mem_id)
{
    struct hdcdrv_mem_block_head *block_head = NULL;
    ka_device_t* pdev_dev = hdcdrv_get_pdev_dev(pool->dev_id);
    int page_index, block_id;
    void* free_buf = NULL;
    ka_page_t* free_page = NULL;
    int alloc_flag;

    if (pdev_dev == NULL) {
        hdcdrv_err("pdev_dev is invalid.\n");
        return HDCDRV_ERR;
    }

    ka_task_spin_lock_bh(&pool->mem_lock);

    block_head = HDCDRV_BLOCK_HEAD(buf);
    if (block_head->ref_count != 1) {
        ka_task_spin_unlock_bh(&pool->mem_lock);
        hdcdrv_mem_block_head_dump(block_head);
        hdcdrv_err_spinlock("Memory block ref_count error.\n");
        return HDCDRV_MEM_NOT_MATCH;
    }
    block_head->ref_count--;

    hdccom_get_page_index_and_block_id(mem_id, &page_index, &block_id);

    hdccom_page_status_change(pool, page_index, block_id, HDCDRV_PAGE_BLOCK_FREE);

    if (pool->page_list[page_index].used_block_num == 0) {  // this page need to free
        free_buf = pool->page_list[page_index].buf; // get buf addr, this addr contains block head
        free_page = pool->page_list[page_index].page_addr;
        alloc_flag = pool->page_list[page_index].alloc_flag;
        pool->page_list[page_index].buf = NULL; // avoid to be overwrite by other process
        pool->page_list[page_index].page_addr = NULL;
        pool->page_list[page_index].valid = HDCDRV_PAGE_NOT_ALLOC;  // change page_index status
    }

    ka_task_spin_unlock_bh(&pool->mem_lock);

    if ((free_buf != NULL) && (free_page != NULL)) {
        block_head = free_buf;
        free_mem_pool_single_page(pdev_dev, block_head, HDCDRV_BLOCK_DMA_HEAD(block_head->dma_addr),
            free_page, alloc_flag);
    }
    return HDCDRV_OK;
}

int hdccom_init_page_pool(struct hdcdrv_mem_pool *pool, struct hdccom_mem_init *init_mem)
{
    u32 i;

    KA_INIT_LIST_HEAD(&pool->wait_list);
    ka_task_spin_lock_init(&pool->mem_lock);

    for (i = 0; i < HDCDRV_HUGE_PAGE_NUM; i++) {
        hdcdrv_page_head_init(&pool->page_list[i], NULL, NULL, HDCDRV_PAGE_NOT_ALLOC, HDCDRV_ALLOC_MEM_BY_HUGE_PAGE);
    }

    pool->dev_id = (u32)init_mem->dev_id;
    pool->valid = HDCDRV_VALID;
    pool->type = init_mem->pool_type;

    return HDCDRV_OK;
}

int hdccom_free_page_pool(struct hdcdrv_mem_pool *pool)
{
    u32 i;
    int retry_time = 0;

    pool->valid = HDCDRV_INVALID;
    for (i = 0; i < HDCDRV_HUGE_PAGE_NUM; i++) {
        retry_time = 0;
free_pool_retry:
        if (pool->page_list[i].valid == HDCDRV_PAGE_PRE_STATUS) {
            retry_time++;
            ka_system_msleep(HDCDRV_RETRY_SLEEP_TIME);
            if (retry_time < HDCDRV_SEND_ALLOC_MEM_RETRY_TIME) {
                goto free_pool_retry;
            } else {
                hdcdrv_info("Wait alloc mem finish too long, exit.\n");
            }
        }
        if ((pool->page_list[i].used_block_num != 0) || (pool->page_list[i].buf != NULL)
            || (pool->page_list[i].page_addr != NULL)) {
            /* Entering this branch means HDC memory is overwritten, will not directly release. Only show the log */
            hdcdrv_info("page_pool has memory abnormal. type=%d, block_num=%d, page_index=%d\n",
                pool->type, pool->page_list[i].used_block_num, i);
        }
    }

    pool->dev_id = 0;

    return HDCDRV_OK;
}
#endif

STATIC void *hdccom_alloc_mem_pool(struct hdccom_mem_init *init_mem, u32 segment,
                                   ka_dma_addr_t *addr, u32 *offset, ka_gfp_t gfp)
{
    struct hdcdrv_mem_block_head *block_head = NULL;
    ka_page_t *page = NULL;
    void *dma_buf = NULL;

    hdcdrv_set_time_stamp(&g_alloc_pool_stamp.dma_alloc_start);
    /* alloc data dma addr */
    if ((init_mem->pool_type == HDCDRV_RESERVE_MEM_POOL_TYPE_TX) ||
        (init_mem->pool_type == HDCDRV_RESERVE_MEM_POOL_TYPE_RX)) {
        *addr = init_mem->reserve_mem_dma_addr;
        dma_buf = init_mem->reserve_mem_va;
        *offset = init_mem->reserve_mem_offset;
        init_mem->reserve_mem_dma_addr += segment;
        init_mem->reserve_mem_va += segment;
        init_mem->reserve_mem_offset += segment;
    } else {
        /* alloc data dma addr */
#ifdef CFG_FEATURE_SMALL_HUGE_POOL
        if (is_alloc_mirror_mem((u32)init_mem->dev_id, segment) == false) {
            page = hdcdrv_alloc_pages_node((u32)init_mem->dev_id, gfp, ka_mm_get_order(segment));
            if (ka_unlikely(page == NULL)) {
                return NULL;
            }
            dma_buf = ka_mm_page_address(page);
            *addr = hal_kernel_devdrv_dma_map_page(init_mem->dev, page, 0, segment, KA_DMA_BIDIRECTIONAL);
            if (ka_mm_dma_mapping_error(init_mem->dev, *addr) != 0) {
                hdcdrv_free_pages_ex(page, ka_mm_get_order(segment), KA_SUB_MODULE_TYPE_2);
                return NULL;
            }
        } else {
            dma_buf = hal_kernel_devdrv_dma_alloc_coherent(init_mem->dev, segment, addr, gfp);
        }
#else
        dma_buf = hal_kernel_devdrv_dma_alloc_coherent(init_mem->dev, segment, addr, gfp);
#endif
    }
    if (ka_unlikely(dma_buf == NULL)) {
        return NULL;
    }
    hdcdrv_set_time_stamp(&g_alloc_pool_stamp.dma_alloc_end);
    /* alloc management structure addr */
    block_head = (struct hdcdrv_mem_block_head *)hdcdrv_kvmalloc(sizeof(struct hdcdrv_mem_block_head), KA_SUB_MODULE_TYPE_0);
    if (ka_unlikely(block_head == NULL)) {
        #ifndef DRV_UT
        if ((init_mem->pool_type == HDCDRV_RESERVE_MEM_POOL_TYPE_TX) ||
            (init_mem->pool_type == HDCDRV_RESERVE_MEM_POOL_TYPE_RX)) {
            dma_buf = NULL;
            *addr = 0;
        } else {
#ifdef CFG_FEATURE_SMALL_HUGE_POOL
            if (is_alloc_mirror_mem(init_mem->dev_id, segment) == false) {
                hal_kernel_devdrv_dma_unmap_page(init_mem->dev, *addr, segment, KA_DMA_BIDIRECTIONAL);
                hdcdrv_free_pages_ex(page, ka_mm_get_order(segment), KA_SUB_MODULE_TYPE_2);
            } else {
                hal_kernel_devdrv_dma_free_coherent(init_mem->dev, segment, dma_buf, *addr);
            }
#else
            hal_kernel_devdrv_dma_free_coherent(init_mem->dev, segment, dma_buf, *addr);
#endif
        }
        #endif
        return NULL;
    }
    hdcdrv_set_time_stamp(&g_alloc_pool_stamp.head_alloc_end);
    hdcdrv_calc_alloc_pool_stamp();
    block_head->dma_buf = dma_buf;
    block_head->pool_page = page;

    return (void *)block_head;
}

int hdccom_init_mem_pool(struct hdcdrv_mem_pool *pool, struct hdccom_mem_init *init_mem)
{
    ka_dma_addr_t addr = 0;
    void *buf = NULL;
    u32 offset = 0;
    ka_gfp_t gfp;
    u32 i;

    hdcdrv_alloc_pool_stamp_init();
    pool->segment = init_mem->segment;
    pool->mask = init_mem->num - 1;
    pool->head = 0;
    pool->size = 0;
    hdcdrv_set_time_stamp(&g_alloc_pool_stamp.ring_alloc_start);
    pool->ring = (struct hdcdrv_mem *)hdcdrv_kzalloc(sizeof(struct hdcdrv_mem) * init_mem->num, KA_GFP_KERNEL,
        KA_SUB_MODULE_TYPE_0);
    if (pool->ring == NULL) {
        pool->tail = 0;
        return HDCDRV_DMA_MEM_ALLOC_FAIL;
    }
    hdcdrv_set_time_stamp(&g_alloc_pool_stamp.ring_alloc_end);

    gfp = hdcdrv_init_mem_pool_get_gfp();

    KA_INIT_LIST_HEAD(&pool->wait_list);
    ka_task_spin_lock_init(&pool->mem_lock);
    hdcdrv_set_time_stamp(&g_alloc_pool_stamp.list_init_end);
    for (i = 0; i < init_mem->num; i++) {
        buf = hdccom_alloc_mem_pool(init_mem, pool->segment, &addr, &offset, gfp);
        if (ka_unlikely(buf == NULL)) {
            goto mem_alloc_fail;
        }

        hdcdrv_mem_block_head_init(buf, addr, offset, init_mem, pool->segment);
        pool->ring[i].buf = buf;
        pool->size++;
    }
    hdcdrv_set_time_stamp(&g_alloc_pool_stamp.end);
    hdcdrv_alloc_pool_stamp_record();
    pool->tail = pool->size;
    pool->dev_id = (u32)init_mem->dev_id;
    pool->valid = HDCDRV_VALID;

    return HDCDRV_OK;

mem_alloc_fail:
    hdcdrv_err("Calling alloc failed. (dev_id=%d; mem_pool=%x; num=%u; i=%u)\n", init_mem->dev_id, pool->segment, init_mem->num, i);
    pool->tail = pool->size;
    return HDCDRV_DMA_MEM_ALLOC_FAIL;
}

int hdccom_free_mem_pool(struct hdcdrv_mem_pool *pool, ka_device_t *dev, u32 segment)
{
    struct hdcdrv_mem_block_head *block_head = NULL;
    int ring_id;
    u32 i;

    if (pool->ring == NULL) {
        hdcdrv_err("Input parameter is error.\n");
        return HDCDRV_ERR;
    }

    if ((pool->tail - pool->head) != pool->size) {
        hdcdrv_info("Get pool value. (pool_segment=%d; alloc_size=%d; head=%lld; tail=%lld)\n", pool->segment,
            pool->size, pool->head, pool->tail);
    }

    for (i = 0; i < (pool->tail - pool->head); i++) {
        ring_id = (int)((pool->head + i) % pool->size);
        if (hdcdrv_mem_block_head_check(pool->ring[ring_id].buf) == HDCDRV_OK) {
            block_head = pool->ring[ring_id].buf;
            free_mem_pool_single(dev, segment, block_head, block_head->dma_addr);
        }
        pool->ring[ring_id].buf = NULL;
    }

    hdcdrv_kfree(pool->ring, KA_SUB_MODULE_TYPE_0);
    pool->ring = NULL;
    pool->dev_id = 0;
    pool->valid = HDCDRV_INVALID;

    return HDCDRV_OK;
}

long hdcdrv_get_page_size(struct hdcdrv_cmd_get_page_size *cmd)
{
    cmd->page_size = KA_MM_PAGE_SIZE;
    cmd->hpage_size = KA_HPAGE_SIZE;
    cmd->page_bit = KA_MM_PAGE_SHIFT;

    return HDCDRV_OK;
}

STATIC u32 hdcdrv_get_node_status(struct hdcdrv_fast_node *fast_node, struct hdcdrv_node_search_info *node_search_info)
{
#ifndef CFG_FEATURE_HDC_REG_MEM
    if (hdcdrv_node_is_busy(fast_node) && (node_search_info->search_type == HDCDRV_SEARCH_WITH_HASH)) {
        return HDCDRV_NODE_BUSY;
    } else {
        hdcdrv_node_status_busy(fast_node);
        return HDCDRV_NODE_IDLE;
    }
#else
    if (hdcdrv_node_is_busy(fast_node)) {
        return HDCDRV_NODE_BUSY;
    } else {
        hdcdrv_node_status_busy(fast_node);
        return HDCDRV_NODE_IDLE;
    }
#endif
}

ka_rb_root_t* hdcdrv_get_rbtree(struct hdcdrv_dev_fmem *dev_fmem, u32 side)
{
    if (side == HDCDRV_RBTREE_SIDE_LOCAL) {
        return &(dev_fmem->rbtree);
    } else {
        return &(dev_fmem->rbtree_re);
    }
}

STATIC u64 hdcdrv_get_hash_va(struct hdcdrv_fast_node *fast_node, u64 search_type)
{
    if (search_type == HDCDRV_SEARCH_WITH_HASH) {
        return fast_node->hash_va;
    } else {
        return fast_node->fast_mem.user_va;
    }
}

struct hdcdrv_fast_node *hdcdrv_fast_node_search(ka_task_spinlock_t *lock, ka_rb_root_t *root,
    u64 new_node_hash, struct hdcdrv_node_status *node_status, struct hdcdrv_node_search_info *node_search_info)
{
    u64 tree_hash;
    ka_rb_node_t *node = NULL;
    struct hdcdrv_fast_node *fast_node = NULL;

    ka_task_spin_lock_bh(lock);

    node = ka_base_get_rb_root_node(root);
    while (node != NULL) {
        fast_node = ka_base_rb_entry(node, struct hdcdrv_fast_node, node);

        tree_hash = hdcdrv_get_hash_va(fast_node, node_search_info->search_type);
        if (new_node_hash < tree_hash) {
            node = node->rb_left;
        } else if (new_node_hash > tree_hash) {
            node = node->rb_right;
        } else {
            node_status->status = hdcdrv_get_node_status(fast_node, node_search_info);
            node_status->stamp = fast_node->stamp;
            if (fast_node->unregister_flag == 0) {
                node_status->hold_flag = 1;
            }
            fast_node->unregister_flag = 1;
            ka_task_spin_unlock_bh(lock);
            return fast_node;
        }
    }

    ka_task_spin_unlock_bh(lock);
    return NULL;
}

struct hdcdrv_fast_node *hdcdrv_fast_node_search_timeout(ka_task_spinlock_t *lock,
    ka_rb_root_t *root, u64 hash_va, int timeout)
{
    int loop_cnt = timeout;
    struct hdcdrv_fast_node *fast_node = NULL;
    struct hdcdrv_node_status node_status = {0};
    struct hdcdrv_node_search_info node_search_info = {0};

    node_search_info.search_type = HDCDRV_SEARCH_WITH_HASH;
    do {
        fast_node = hdcdrv_fast_node_search(lock, root, hash_va, &node_status, &node_search_info);
        if (fast_node == NULL) {
            return NULL;
        }

        if (node_status.status == HDCDRV_NODE_IDLE) {
            return fast_node;
        }

        /* node busy */
        if (hdcdrv_node_is_timeout(node_status.stamp)) {
            hdcdrv_err_limit("Fast node timeout. (time=%dms, loop_cnt=%d)\n",
                (u32)ka_system_jiffies_to_msecs(ka_jiffies - node_status.stamp), loop_cnt);
            return NULL;
        }

        if (loop_cnt > 0) {
            ka_system_msleep(1);
        }
    } while (loop_cnt--);

    hdcdrv_err_limit("Fast node search failed for busy. (timeout=%d)\n", timeout);
    return NULL;
}

int hdcdrv_fast_node_insert(ka_task_spinlock_t *lock, ka_rb_root_t *root, struct hdcdrv_fast_node *fast_node, u64 search_type)
{
    u64 new_node_hash, tree_hash;
    ka_rb_node_t *parent = NULL;
    ka_rb_node_t **new_node = NULL;

    ka_task_spin_lock_bh(lock);
    new_node = ka_base_get_rb_root_node_addr(root);
    new_node_hash = hdcdrv_get_hash_va(fast_node, search_type);

    /* Figure out where to put new node */
    while (*new_node) {
        struct hdcdrv_fast_node *this = ka_base_rb_entry(*new_node, struct hdcdrv_fast_node, node);

        parent = *new_node;
        tree_hash = hdcdrv_get_hash_va(this, search_type);
        if (new_node_hash < tree_hash) {
            new_node = &((*new_node)->rb_left);
        } else if (new_node_hash > tree_hash) {
            new_node = &((*new_node)->rb_right);
        } else {
            ka_task_spin_unlock_bh(lock);
            return HDCDRV_F_NODE_SEARCH_FAIL;
        }
    }

    /* Add new node and rebalance tree. */
    ka_base_rb_link_node(&fast_node->node, parent, new_node);
    ka_base_rb_insert_color(&fast_node->node, root);

    hdcdrv_node_status_init(fast_node);

    ka_task_spin_unlock_bh(lock);

    return HDCDRV_OK;
}

void hdcdrv_fast_node_erase(ka_task_spinlock_t *lock, ka_rb_root_t *root, struct hdcdrv_fast_node *fast_node)
{
    if (lock != NULL) {
        ka_task_spin_lock_bh(lock);
    }
    ka_base_rb_erase(&fast_node->node, root);
    if (lock != NULL) {
        ka_task_spin_unlock_bh(lock);
    }
}

void hdcdrv_fast_node_free(const struct hdcdrv_fast_node *fast_node)
{
    hdcdrv_kvfree((void **)&fast_node, KA_SUB_MODULE_TYPE_2);
    fast_node = NULL;
}

struct hdcdrv_fast_node* __attribute__((weak)) hdcdrv_fast_node_search_from_new_tree(u32 rb_side,
    int timeout, struct hdcdrv_fast_node_msg_info *node_info)
{
    return NULL;
}

int __attribute__((weak)) hdcdrv_fast_node_insert_new_tree(int devid, u64 pid, u32 fid, u32 rb_side,
    struct hdcdrv_fast_node *new_node)
{
    hdcdrv_err("hdcdrv_fast_node_insert_arry not support\n");
    return HDCDRV_OK;
}

void __attribute__((weak)) hdcdrv_fast_node_erase_from_new_tree(u64 pid, u32 fid, int devid, u32 rb_side,
    struct hdcdrv_fast_node *fast_node)
{
    hdcdrv_err("hdcdrv_fast_node_erase_from_arry not support\n");
    return;
}

STATIC int hdcdrv_get_fast_mem_check(const struct hdcdrv_fast_node *f_node, int type, u32 len)
{
    if ((f_node->fast_mem.mem_type != type) && (f_node->fast_mem.mem_type != HDCDRV_FAST_MEM_TYPE_DVPP) &&
        (f_node->fast_mem.mem_type != HDCDRV_FAST_MEM_TYPE_ANY)) {
        hdcdrv_err("mem_type not match. (mem_type=%d; type=%d)\n", f_node->fast_mem.mem_type, type);
        return HDCDRV_PARA_ERR;
    }

    if (f_node->fast_mem.alloc_len < len) {
        hdcdrv_err("Fast memory check failed. (need_len=%d; alloc_len=%u)\n", len, f_node->fast_mem.alloc_len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

STATIC u32 hdcdrv_get_rb_side(int type)
{
    if ((type == HDCDRV_FAST_MEM_TYPE_TX_CTRL) || (type == HDCDRV_FAST_MEM_TYPE_TX_DATA)) {
        return HDCDRV_RBTREE_SIDE_REMOTE;
    } else {
        return HDCDRV_RBTREE_SIDE_LOCAL;
    }
}

int __attribute__((weak)) hdcdrv_save_fast_mem_info(struct hdcdrv_fast_mem *fast_mem,
    u64 send_addr_va, struct hdcdrv_fast_addr_info *addr_info)
{
    return HDCDRV_OK;
}

void hdcdrv_get_fast_mem(struct hdcdrv_dev_fmem *dev_fmem, int type,
    struct hdcdrv_fast_node_msg_info *node_msg, struct hdcdrv_fast_addr_info *addr_info)
{
    int ret;
    u32 rb_side;
    ka_task_spinlock_t *lock = NULL;
    ka_rb_root_t *root = NULL;
    struct hdcdrv_fast_node *f_node = NULL;
    addr_info->f_mem = NULL;
#ifdef CFG_FEATURE_HDC_REG_MEM
    if ((node_msg->len == 0) || (node_msg->va_addr == 0)) {
        return;
    }
#endif
    rb_side = hdcdrv_get_rb_side(type);
    f_node = hdcdrv_fast_node_search_from_new_tree(rb_side, HDCDRV_NODE_WAIT_TIME_MIN, node_msg);
    if (f_node == NULL) {
        lock = &dev_fmem->rb_lock;
        if ((type == HDCDRV_FAST_MEM_TYPE_TX_DATA) || (type == HDCDRV_FAST_MEM_TYPE_TX_CTRL)) {
            root = &dev_fmem->rbtree_re;
        } else {
            root = &dev_fmem->rbtree;
        }
        f_node = hdcdrv_fast_node_search_timeout(lock, root, node_msg->hash_val, HDCDRV_NODE_WAIT_TIME_MIN);
    }
    if (f_node == NULL) {
        hdcdrv_warn("Node not found. (type=%d; hash_va=0x%llx len=%d)\n", type, node_msg->hash_val, node_msg->len);
        return;
    }

    ret = hdcdrv_get_fast_mem_check(f_node, type, node_msg->len);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("Calling hdcdrv_get_fast_mem_check failed.(f_type=%d, msg type=%d, f_pid=%lld, msg pid=%lld)\n",
            f_node->fast_mem.mem_type, type, f_node->pid, node_msg->pid);
        return;
    }

    ret = hdcdrv_save_fast_mem_info(&f_node->fast_mem, node_msg->va_addr, addr_info);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        return;
    }
    addr_info->f_mem = &f_node->fast_mem;

    return;
}

struct hdcdrv_fast_mem *hdcdrv_get_fast_mem_timeout(int dev_id, int type,
    int len, u64 hash_va, u64 user_va)
{
    int ret;
    u32 fid, pid;
    struct hdcdrv_dev_fmem *dev_fmem = NULL;
    ka_task_spinlock_t *lock  = NULL;
    ka_rb_root_t *root = NULL;
    struct hdcdrv_fast_node *f_node = NULL;
    struct hdcdrv_fast_node_msg_info node_msg;

    fid = (u32)((hash_va >> HDCDRV_FRBTREE_FID_BEG) & HDCDRV_FRBTREE_FID_MASK);
    node_msg.dev_id = (u32)dev_id;
    pid = (u32)(hash_va & HDCDRV_FRBTREE_PID_MASK);
    hdcdrv_node_msg_info_fill(pid, fid, len, user_va, HDCDRV_SEARCH_NODE_SENDRECV, &node_msg);
    f_node = hdcdrv_fast_node_search_from_new_tree(HDCDRV_RBTREE_SIDE_LOCAL, HDCDRV_NODE_WAIT_TIME_MAX,
                                                   &node_msg);
    if (f_node == NULL) {
        dev_fmem = hdcdrv_get_dev_fmem_ex(dev_id, fid, HDCDRV_RBTREE_SIDE_LOCAL);
        lock = &dev_fmem->rb_lock;
        root = &dev_fmem->rbtree;
        f_node = hdcdrv_fast_node_search_timeout(lock, root, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    }
    if (f_node == NULL) {
        hdcdrv_warn("Node not found. (type=%d; hash_va=0x%llx len=%d)\n", type, hash_va, len);
        return NULL;
    }

    ret = hdcdrv_get_fast_mem_check(f_node, type, (u32)len);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("Calling hdcdrv_get_fast_mem_check failed.\n");
        return NULL;
    }

    return &f_node->fast_mem;
}

STATIC unsigned int hdcdrv_fast_get_alloc_pages_segment(unsigned int len, unsigned int *max_len_bit)
{
    unsigned int j;
    unsigned int segment = 0;

    /* max segment is 4M */
    if ((len >> *max_len_bit) != 0) {
        return (unsigned int)(0x1u << *max_len_bit);
    }

    for (j = *max_len_bit - 1; j >= HDCDRV_MEM_MIN_PAGE_LEN_BIT; j--) {
        if ((len & (0x1u << j)) != 0) {
            *max_len_bit = j;
            segment = 0x1u << j;
            break;
        }
    }

    return segment;
}

int __attribute__((weak)) hdcdrv_dma_map(struct hdcdrv_fast_mem *f_mem, int devid, int flag)
{
    ka_device_t* pdev_dev = hdcdrv_get_pdev_dev(devid);
    u32 stamp, cost_time;
    int ret, i, j;

    /* devid has checked outside */
    if (pdev_dev == NULL) {
        hdcdrv_err("pdev_dev is invalid. (dev_id=%d)\n", devid);
        return HDCDRV_DEVICE_NOT_READY;
    }

    if (f_mem->dma_map != 0) {
        ret = ((f_mem->devid == devid) ? HDCDRV_OK : HDCDRV_PARA_ERR);
        hdcdrv_info("Hdcdrv dma has already been mapped. (dev=%d; now_dev=%d)\n", f_mem->devid, devid);
        return ret;
    }

    stamp = (u32)ka_jiffies;
    for (i = 0; i < f_mem->phy_addr_num; i++) {
        cost_time = ka_system_jiffies_to_msecs(ka_jiffies - stamp);
        if (cost_time > HDCDRV_THRESHOLD_COST_TIME) {
            ka_task_cond_resched();
            stamp = (u32)ka_jiffies;
        }

        f_mem->mem[i].addr = hal_kernel_devdrv_dma_map_page(pdev_dev, f_mem->mem[i].page,
            f_mem->mem[i].page_inner_offset, f_mem->mem[i].len, KA_DMA_BIDIRECTIONAL);
        if (ka_mm_dma_mapping_error(pdev_dev, f_mem->mem[i].addr) != 0) {
            hdcdrv_err("Calling ka_mm_dma_mapping_error error.\n");
            goto dma_unmap;
        }
    }
    f_mem->devid = devid;

    ret = hdcdrv_send_mem_info(f_mem, devid, flag);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Calling hdcdrv_send_mem_info failed. (ret=%d)\n", ret);
        goto dma_unmap;
    }

    f_mem->dma_map = 1;
    return ret;

dma_unmap:
    for (j = 0; j < i; j++) {
        hal_kernel_devdrv_dma_unmap_page(pdev_dev, f_mem->mem[j].addr, f_mem->mem[j].len, KA_DMA_BIDIRECTIONAL);
        f_mem->mem[j].addr = 0;
    }
    return HDCDRV_DMA_MPA_FAIL;
}

STATIC void hdcdrv_mem_stat_info_show(void)
{
    u64 alloc_len;
    u64 alloc_normal_len;
    u64 alloc_dma_len;
    u64 alloc_cnt;
    u64 free_len;
    u64 free_cnt;
    struct hdcdrv_dev_fmem *dev_fmem = NULL;

    dev_fmem = hdcdrv_get_dev_fmem_uni();
    alloc_cnt = dev_fmem->mem_dfx_stat.alloc_cnt;
    alloc_len = dev_fmem->mem_dfx_stat.alloc_size;
    alloc_normal_len = dev_fmem->mem_dfx_stat.alloc_normal_size;
    alloc_dma_len = dev_fmem->mem_dfx_stat.alloc_dma_size;
    free_cnt = dev_fmem->mem_dfx_stat.free_cnt;
    free_len = dev_fmem->mem_dfx_stat.free_size;

    hdcdrv_info("HDC memory stat information. (alloc_cnt=%llu; alloc_size=0x%llx; alloc_normal_len=0x%llx;"
        "alloc_dma_len=0x%llx; free_cnt=%llu; free_size=0x%llx)\n",
        alloc_cnt, alloc_len, alloc_normal_len, alloc_dma_len, free_cnt, free_len);
}

int __attribute__((weak)) hdcdrv_dma_unmap(struct hdcdrv_fast_mem *f_mem, u32 devid, int sync, int flag)
{
    ka_device_t* pdev_dev = NULL;
    u32 stamp, cost_time;
    int ret = HDCDRV_OK;
    int i;

    if (f_mem->dma_map == 0) {
        hdcdrv_limit_exclusive(info, HDCDRV_LIMIT_LOG_0x10, "Dma memory has not been mapped, no need unmap.\n");
        return HDCDRV_OK;
    }

    pdev_dev = hdcdrv_get_pdev_dev((int)devid);
    if (pdev_dev == NULL) {
        hdcdrv_err("pdev_dev is invalid.\n");
        return HDCDRV_ERR;
    }

    if (hdcdrv_mem_is_notify(f_mem)) {
        ret = hdcdrv_send_mem_info(f_mem, (int)devid, flag);
        if (ret != HDCDRV_OK) {
            hdcdrv_err_limit("Calling hdcdrv_send_mem_info failed. (ret=%d)\n", ret);
            /* ignore the sync msg */
            if (sync == HDCDRV_SYNC_CHECK) {
                return ret;
            }
        }
    }

    stamp = (u32)ka_jiffies;
    for (i = 0; i < f_mem->phy_addr_num; i++) {
        cost_time = ka_system_jiffies_to_msecs(ka_jiffies - stamp);
        if (cost_time > HDCDRV_THRESHOLD_COST_TIME) {
            ka_task_cond_resched();
            stamp = (u32)ka_jiffies;
        }

        hal_kernel_devdrv_dma_unmap_page(pdev_dev, f_mem->mem[i].addr, f_mem->mem[i].len, KA_DMA_BIDIRECTIONAL);
    }
    f_mem->dma_map = 0;
    f_mem->devid = 0;

    return HDCDRV_OK;
}

void hdcdrv_fast_mem_continuity_check(u32 alloc_len, u32 addr_num, const int segment_mem_num[], u32 segment_num)
{
    int score, ret, offset;
    u32 i, expect_num, segment;
    char buf[HDCDRV_BUF_LEN] = {0};

    expect_num = 0;
    for (i = HDCDRV_MEM_MIN_PAGE_LEN_BIT; ; i++) {
        segment = (0x1u << i);
        if (alloc_len < segment) {
            break;
        }

        if ((alloc_len & segment) != 0) {
            /* 256kb is the critical point of performance */
            if (i > HDCDRV_MEM_MIN_LEN_BIT) {
                expect_num += 0x1u << (i - HDCDRV_MEM_MIN_LEN_BIT);
            } else {
                expect_num++;
            }
        }
    }

    /* Percentage, more than 100 points is excellence */
    score = 0;
    if (addr_num != 0) {
        score = (int)(expect_num * HDCDRV_MEM_SCORE_SCALE / addr_num);
    }

    offset = 0;
    ret = snprintf_s(buf, HDCDRV_BUF_LEN, HDCDRV_BUF_LEN - 1, "order:num");
    if (ret >= 0) {
        offset += ret;
    }

    for (i = 0; i < segment_num; i++) {
        if (segment_mem_num[i] > 0) {
            ret = snprintf_s(buf + offset, (size_t)(HDCDRV_BUF_LEN - offset), (size_t)(HDCDRV_BUF_LEN - offset - 1),
                ",%u:%d", i, segment_mem_num[i]);
            if (ret >= 0) {
                offset += ret;
            }
        }
    }

    if (score < HDCDRV_MEM_SCORE_SCALE) {
        hdcdrv_warn_limit("score is invalid. (alloc_len=0x%x; expect_num=%u; actual=%u; score=%d; addr_info=\"%s\")\n",
            alloc_len, expect_num, addr_num, score, buf);
    }
}

static inline void hdcdrv_fast_init_segment_mem_num(int segment_mem_num[], u32 segment_num)
{
    u32 i;
    for (i = 0; i < segment_num; i++) {
        segment_mem_num[i] = 0;
    }
}

static inline void hdcdrv_fast_inc_segment_mem_num(int segment_mem_num[], u32 segment_num, u32 power)
{
    if (power < segment_num) {
        segment_mem_num[power]++;
    }
}

void hdcdrv_fill_fast_mem_info(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type)
{
    f_mem->user_va = va;
    f_mem->alloc_len = len;
    f_mem->mem_type = (int)type;
}

int hdcdrv_pin_user_page(unsigned long start, int nr_pages, unsigned int gup_flags, ka_page_t **pages)
{
#ifdef CFG_FEATURE_HDC_REG_MEM
    return ka_mm_get_user_pages_fast(start, nr_pages, gup_flags, pages);
#else
    return ka_mm_pin_user_pages_fast(start, nr_pages, gup_flags, pages);
#endif
}

void hdcdrv_unpin_user_page(ka_page_t *page)
{
#ifdef CFG_FEATURE_HDC_REG_MEM
    ka_mm_put_page(page);
#else
    ka_mm_unpin_user_page(page);
#endif
}

void hdcdrv_huge_put_page(struct hdcdrv_fast_mem *f_mem)
{
    int i;

    for (i = 0; i < f_mem->phy_addr_num; i++) {
        if (f_mem->mem[i].page != NULL) {
            hdcdrv_unpin_user_page(f_mem->mem[i].page);
            f_mem->mem[i].page = NULL;
        }
    }
}

void hdcdrv_fast_free_mem_node(struct hdcdrv_fast_mem *f_mem)
{
    hdcdrv_kvfree((void **)&f_mem->mem, KA_SUB_MODULE_TYPE_2);
    f_mem->mem = NULL;
}

void hdcdrv_fast_free_huge_page_mem(struct hdcdrv_fast_mem *f_mem)
{
    hdcdrv_huge_put_page(f_mem);
    hdcdrv_fast_free_mem_node(f_mem);
    f_mem->phy_addr_num = 0;
}

STATIC int hdcdrv_fast_alloc_huge_page_mem(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type, u32 devid)
{
    const int nr_page = 1;
    int ret;
    u32 i;

    f_mem->phy_addr_num = (int)(len >> HDCDRV_MEM_MIN_HUGE_PAGE_LEN_BIT);

    f_mem->mem = (struct hdcdrv_mem_f *)hdcdrv_kvmalloc((u64)(unsigned int)f_mem->phy_addr_num *
        sizeof(struct hdcdrv_mem_f), KA_SUB_MODULE_TYPE_2);
    if (f_mem->mem == NULL) {
        hdcdrv_err("Calling ka_mm_kmalloc error.\n");
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    for (i = 0; i < (u32)f_mem->phy_addr_num; i++) {
        ret = hdcdrv_pin_user_page(va + (u64)i * KA_HPAGE_SIZE, nr_page, KA_FOLL_WRITE, &f_mem->mem[i].page);
        if (ret != nr_page) {
            /* In the exception branch, the f_mem->mem[i].page may not be NULL,
                which causes problem when free huge pages */
            f_mem->mem[i].page = NULL;
            hdcdrv_err("Calling get_user_pages failed. (ret=%d)\n", ret);
            goto free_put_page;
        }

        f_mem->mem[i].len = (u32)KA_HPAGE_SIZE;
        f_mem->mem[i].page_inner_offset = 0;
    }

    return HDCDRV_OK;

free_put_page:
    hdcdrv_fast_free_huge_page_mem(f_mem);
    return HDCDRV_MEM_ALLOC_FAIL;
}

STATIC void hdcdrv_fast_normal_mem_alloc_dfx(const struct hdcdrv_mem_f *mem, int phy_num, u32 len, u32 devid)
{
    int i;
    struct hdcdrv_dev_fmem *dev_fmem = hdcdrv_get_dev_fmem_uni();

    ka_task_spin_lock_bh(&dev_fmem->mem_dfx_stat.lock);
    dev_fmem->mem_dfx_stat.alloc_cnt++;
    dev_fmem->mem_dfx_stat.alloc_size += len;
    for (i = 0; i < phy_num; i++) {
        if (mem[i].type == HDCDRV_NORMAL_MEM) {
            dev_fmem->mem_dfx_stat.alloc_normal_size += mem[i].len;
        } else {
            dev_fmem->mem_dfx_stat.alloc_dma_size += mem[i].len;
        }
    }
    ka_task_spin_unlock_bh(&dev_fmem->mem_dfx_stat.lock);
}

STATIC void hdcdrv_fast_normal_mem_free_dfx(u32 len)
{
    struct hdcdrv_dev_fmem *dev_fmem = hdcdrv_get_dev_fmem_uni();

    ka_task_spin_lock_bh(&dev_fmem->mem_dfx_stat.lock);
    dev_fmem->mem_dfx_stat.free_cnt++;
    dev_fmem->mem_dfx_stat.free_size += len;
    ka_task_spin_unlock_bh(&dev_fmem->mem_dfx_stat.lock);
}

STATIC void hdcdrv_fast_free_pages(struct hdcdrv_mem_f *mem, int phy_addr_num)
{
    int i;

    for (i = 0; i < phy_addr_num; i++) {
        if (mem[i].page != NULL) {
            hdcdrv_free_pages_ex(mem[i].page, mem[i].power, KA_SUB_MODULE_TYPE_2);
            mem[i].buf = NULL;
            mem[i].page = NULL;
            mem[i].len = 0;
            mem[i].power = 0;
        }
    }
}
STATIC ka_gfp_t hdcdrv_get_mem_work_mask(u32 type)
{
    ka_gfp_t gfp_mask;

    if (type == HDCDRV_DMA_MEM) {
        gfp_mask = KA_GFP_KERNEL | __KA_GFP_NOWARN | __KA_GFP_DMA32;
    } else {
        gfp_mask = KA_GFP_KERNEL | __KA_GFP_NOWARN;
    }

    return gfp_mask;
}

void hdcdrv_recycle_mem_work(ka_work_struct_t *p_work)
{
    int i;
    u32 stamp;
    u32 cost_time;
    struct hdcdrv_mem_f *mem =  NULL;
    ka_gfp_t gfp_mask;
    u64 work_cnt = g_mem_work_cnt;

    stamp = (u32)ka_jiffies;
    gfp_mask = hdcdrv_get_mem_work_mask(g_mem_type);

    mem = (struct hdcdrv_mem_f *)hdcdrv_kvmalloc((u64)HDCDRV_LIST_MEM_NUM * sizeof(struct hdcdrv_mem_f), KA_SUB_MODULE_TYPE_2);
    if (mem == NULL) {
        hdcdrv_warn("Calling ka_mm_kmalloc no success. (mem_type=%u; work_cnt=%llu)\n", g_mem_type, work_cnt);
        return;
    }

    for (i = 0; i < HDCDRV_LIST_MEM_NUM; i++) {
        if (g_mem_work_flag != 0) {
            hdcdrv_info("Work exit. (mem_type=%d; i=%i; work_cnt=%lld)\n", g_mem_type, i, work_cnt);
            g_mem_work_flag = 0;
            goto out;
        }
        mem[i].page = hdcdrv_alloc_pages(gfp_mask, HDCDRV_MEM_ORDER_1MB, KA_SUB_MODULE_TYPE_2);
        if (mem[i].page == NULL) {
            hdcdrv_warn("Calling ka_mm_alloc_pages no success. (i=%d; gfp_mask=0x%x; mem_type=%d; work_cnt=%lld)\n",
                i, gfp_mask, g_mem_type, work_cnt);
            goto out;
        }
        mem[i].power = HDCDRV_MEM_ORDER_1MB;
    }

    cost_time = ka_system_jiffies_to_msecs(ka_jiffies - stamp);
    hdcdrv_info("Get memory work cost_time. (cost_time=%d; mask=0x%x; mem_type=%d; work_cnt=%lld)\n",
        cost_time, gfp_mask, g_mem_type, work_cnt);

out:
    hdcdrv_fast_free_pages(mem, i);
    hdcdrv_kvfree((void **)&mem, KA_SUB_MODULE_TYPE_2);
    mem = NULL;
    cost_time = ka_system_jiffies_to_msecs(ka_jiffies - stamp);
    hdcdrv_info("Get memory work cost_time. (cost_time=%d; i=%d; mem_type=%d; work_cnt=%lld)\n",
        cost_time, i, g_mem_type, work_cnt);
    hdcdrv_mem_stat_info_show();
}

STATIC void hdcdrv_alloc_pages_switch(u32 *max_len_bit)
{
    ka_delayed_work_t *rec_work;
    rec_work = hdcdrv_get_recycle_mem();
    if (rec_work == NULL) {
        hdcdrv_info("rec_work is invalid.\n");
        return;
    }

    g_mem_work_flag = 1;
    (void)ka_task_cancel_delayed_work_sync(rec_work);
    g_mem_work_flag = 0;
    (void)ka_task_schedule_delayed_work(rec_work, 0);
    *max_len_bit = HDCDRV_MEM_MAX_LEN_BIT;
    g_mem_type = ((g_mem_type == HDCDRV_DMA_MEM) ?  HDCDRV_NORMAL_MEM : HDCDRV_DMA_MEM);
    g_mem_work_cnt++;
    hdcdrv_info("Schedule mem work. (mem_type=%u; cnt=%llu)\n", g_mem_type, g_mem_work_cnt);
}

STATIC ka_gfp_t hdcdrv_get_mask(u32 type, u32 max_len_bit)
{
    ka_gfp_t gfp_mask;

    if (type == HDCDRV_DMA_MEM) {
        gfp_mask = KA_GFP_NOWAIT | __KA_GFP_NOWARN | __KA_GFP_DMA32 | __KA_GFP_ACCOUNT | __KA_GFP_ZERO;
    } else {
        gfp_mask = KA_GFP_NOWAIT | __KA_GFP_NOWARN | __KA_GFP_ACCOUNT | __KA_GFP_ZERO;
    }

    if (max_len_bit <= HDCDRV_MEM_64KB_LEN_BIT) {
        gfp_mask = KA_GFP_KERNEL | __KA_GFP_NOWARN | __KA_GFP_ACCOUNT | __KA_GFP_ZERO;
    }

    return gfp_mask;
}

STATIC int hdcdrv_fast_alloc_pages(struct hdcdrv_mem_f *mem, u64 va, u32 len, u32 type, struct hdcdrv_fast_mem *f_mem)
{
    int segment_mem_num[HDCDRV_MEM_ORDER_NUM] = {0};
    int i = 0;
    u32 segment, power;
    u32 max_len_bit = HDCDRV_MEM_MAX_LEN_BIT;
    u32 alloc_len = len;
    ka_gfp_t gfp_mask;
    u32 switch_time = 0;
    struct hdcdrv_dev_fmem *dev_fmem = NULL;

    dev_fmem = hdcdrv_get_dev_fmem_uni();

    hdcdrv_fast_init_segment_mem_num(segment_mem_num, HDCDRV_MEM_ORDER_NUM);

    while (alloc_len > 0) {
        segment = hdcdrv_fast_get_alloc_pages_segment(alloc_len, &max_len_bit);
        power = (u32)ka_mm_get_order(segment);

        gfp_mask = hdcdrv_get_mask(g_mem_type, max_len_bit);

        mem[i].page = hdcdrv_alloc_pages_node((u32)f_mem->devid, gfp_mask, power);
        if (mem[i].page == NULL) {
            max_len_bit -= 1;
            hdcdrv_info_limit("Get memory length. (total_len=0x%x; remain_alloc_len=0x%x; max_len_bit=%u;"
                "mem_work_cnt=%llu; mem_type=%u; gfp_mask=0x%x; alloc_cnt=%llu; alloc_size=%llu)\n",
                len, alloc_len, max_len_bit, g_mem_work_cnt, g_mem_type, gfp_mask,
                dev_fmem->mem_dfx_stat.alloc_cnt, dev_fmem->mem_dfx_stat.alloc_size);
            if ((max_len_bit == HDCDRV_MEM_64KB_LEN_BIT) && (switch_time == 0)) {
                hdcdrv_info("ka_task_cond_resched for system task and switch alloc_mem_type.\n");
                ka_task_cond_resched();
                hdcdrv_alloc_pages_switch(&max_len_bit);
                switch_time++;
            } else if ((max_len_bit < HDCDRV_MEM_MIN_PAGE_LEN_BIT)) {
                f_mem->phy_addr_num = i;
                hdcdrv_fast_free_pages(mem, i);
                return HDCDRV_DMA_MEM_ALLOC_FAIL;
            }
            continue;
        }

        mem[i].len = segment;
        mem[i].power = power;
        mem[i].type = g_mem_type;
        mem[i].page_inner_offset = 0;
        hdcdrv_fast_inc_segment_mem_num(segment_mem_num, HDCDRV_MEM_ORDER_NUM, power);

        i++;
        if (segment > 0) {
            alloc_len -= segment;
        }
    }

    f_mem->phy_addr_num = i;

    hdcdrv_fast_mem_continuity_check(len, (u32)i, segment_mem_num, HDCDRV_MEM_ORDER_NUM);
    return HDCDRV_OK;
}

STATIC int hdcdrv_fast_alloc_normal_page_mem(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type, u32 devid)
{
    struct hdcdrv_mem_f *mem = NULL;
    u32 stamp, cost_time;
    int i, ret;
    u64 size;

    size = (u64)(len >> HDCDRV_MEM_MIN_PAGE_LEN_BIT) * sizeof(struct hdcdrv_mem_f);
    mem = (struct hdcdrv_mem_f *)hdcdrv_kvmalloc(size, KA_SUB_MODULE_TYPE_2);
    if (mem == NULL) {
        hdcdrv_err("Calling ka_mm_kvmalloc error. (len=%u; size=%lld)\n", len, size);
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    stamp = (u32)ka_jiffies;

    ret = hdcdrv_fast_alloc_pages(mem, va, len, type, f_mem);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Calling hdcdrv_fast_alloc_pages failed. (dev=%u; addr_bum=%d)\n", devid, f_mem->phy_addr_num);
        goto fail;
    }

    cost_time = ka_system_jiffies_to_msecs(ka_jiffies - stamp);
    if (cost_time > HDCDRV_MAX_COST_TIME) {
        ka_task_cond_resched();
        hdcdrv_warn_limit("cost_time is longer than expected. (devid=%d; type=%d; phy_num=%d; va=0x%pK; "
            "len=0x%x; cost_time=%dms)\n", devid, type, f_mem->phy_addr_num, (void *)(uintptr_t)va, len, cost_time);
    }

    if (f_mem->phy_addr_num > HDCDRV_MEM_MAX_PHY_NUM) {
        hdcdrv_err("phy_addr_num is bigger than expected. (dev=%d; phy_addr_num=%d; max_addr_num=%d)\n",
            devid, f_mem->phy_addr_num, HDCDRV_MEM_MAX_PHY_NUM);
        goto fail;
    }

    size = (u64)(unsigned int)f_mem->phy_addr_num * sizeof(struct hdcdrv_mem_f);
    f_mem->mem = (struct hdcdrv_mem_f *)hdcdrv_kvmalloc(size, KA_SUB_MODULE_TYPE_2);
    if (f_mem->mem == NULL) {
        hdcdrv_err("Calling ka_mm_kvmalloc error. (phy_addr_num=%d; size=%lld)\n", f_mem->phy_addr_num, size);
        goto fail;
    }

    for (i = 0; i < f_mem->phy_addr_num; i++) {
        f_mem->mem[i] = mem[i];
    }

    hdcdrv_kvfree((void **)&mem, KA_SUB_MODULE_TYPE_2);
    mem = NULL;

    hdcdrv_fast_normal_mem_alloc_dfx(f_mem->mem, f_mem->phy_addr_num, len, devid);
    return HDCDRV_OK;

fail:
    hdcdrv_fast_free_pages(mem, f_mem->phy_addr_num);
    hdcdrv_kvfree((void **)&mem, KA_SUB_MODULE_TYPE_2);
    mem = NULL;
    hdcdrv_err("Memory alloc failed. (dev=%u; type=%d; total_len=0x%x)\n", devid, type, len);
    hdcdrv_mem_stat_info_show();
    return HDCDRV_DMA_MEM_ALLOC_FAIL;
}

STATIC void hdcdrv_fast_free_normal_page_mem(struct hdcdrv_fast_mem *f_mem)
{
    hdcdrv_fast_free_pages(f_mem->mem, f_mem->phy_addr_num);
    hdcdrv_fast_free_mem_node(f_mem);
    f_mem->phy_addr_num = 0;
    hdcdrv_fast_normal_mem_free_dfx(f_mem->alloc_len);
}

STATIC int hdcdrv_fast_alloc_phy_mem(struct hdcdrv_fast_mem *f_mem, u64 va, u32 len, u32 type, u32 devid)
{
    hdcdrv_fill_fast_mem_info(f_mem, va, len, type);

    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        return hdcdrv_fast_alloc_huge_page_mem(f_mem, va, len, type, devid);
    } else {
        return hdcdrv_fast_alloc_normal_page_mem(f_mem, va, len, type, devid);
    }
}

void hdcdrv_fast_free_phy_mem(struct hdcdrv_fast_mem *f_mem)
{
#ifdef CFG_FEATURE_HDC_REG_MEM
    if (f_mem->page_type == HDCDRV_PAGE_TYPE_REGISTER) {
        hdcdrv_fast_unpin_mem_normal(f_mem);
        return;
    }
#endif
    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        hdcdrv_fast_free_huge_page_mem(f_mem);
    } else if (f_mem->page_type == HDCDRV_PAGE_TYPE_NORMAL) {
        hdcdrv_fast_free_normal_page_mem(f_mem);
    } else {
        hdcdrv_fast_free_mem_node(f_mem);
    }

    hdcdrv_fill_fast_mem_info(f_mem, 0, 0, HDCDRV_FAST_MEM_TYPE_MAX);
    f_mem->hash_va = 0;
    return;
}

STATIC int hdcdrv_check_va(const void *ctx, ka_vm_area_struct_t *vma, const struct hdcdrv_fast_mem *f_mem)
{
    unsigned long size = f_mem->alloc_len;
    unsigned long addr = f_mem->user_va;
    unsigned long end = addr + KA_MM_PAGE_ALIGN(size);

    if ((ka_mm_get_vm_flags(vma) & KA_VM_HUGETLB) != 0) {
        hdcdrv_err("Input parameter is error. (addr=%pK)\n", (void *)(uintptr_t)f_mem->user_va);
        return HDCDRV_PARA_ERR;
    }

    if (ka_mm_get_vm_private_data(vma) != ctx) {
        hdcdrv_err("addr %pK ka_mm_get_vm_private_data() %pK ctx %pK\n",
            (void *)(uintptr_t)f_mem->user_va, ka_mm_get_vm_private_data(vma), ctx);
        return HDCDRV_PARA_ERR;
    }

    if ((f_mem->user_va & (KA_MM_PAGE_SIZE - 1)) != 0) {
        hdcdrv_err("Input parameter is error. (addr=%pK)\n", (void *)(uintptr_t)f_mem->user_va);
        return HDCDRV_PARA_ERR;
    }

    if ((addr < ka_mm_get_vm_start(vma)) || (addr > ka_mm_get_vm_end(vma)) || (end > ka_mm_get_vm_end(vma)) || (addr >= end)) {
        hdcdrv_err("Input parameter is error. (vma_user_addr=%pK; len=%x)\n",
            (void *)(uintptr_t)f_mem->user_va, f_mem->alloc_len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

STATIC void hdcdrv_zap_vma_ptes(const struct hdcdrv_fast_mem *f_mem, ka_vm_area_struct_t *vma, int phy_addr_num)
{
    int i;
    u32 len;
    int ret;
    u32 offset = 0;

    for (i = 0; i < phy_addr_num; i++) {
        len = f_mem->mem[i].len;
        ret = ka_mm_zap_vma_ptes(vma, f_mem->user_va + offset, f_mem->mem[i].len);
        if (ret != 0) {
            hdcdrv_err("Calling ka_mm_zap_vma_ptes failed. (va=0x%pK)\n", (void *)(uintptr_t)(f_mem->user_va + offset));
        }
        offset += len;
    }
}

STATIC int hdcdrv_follow_pfn_check(ka_vm_area_struct_t *vma, const struct hdcdrv_fast_mem *f_mem)
{
    unsigned long size = f_mem->alloc_len;
    unsigned long addr = f_mem->user_va;
    unsigned long end = addr + KA_MM_PAGE_ALIGN(size);
    unsigned long va_check;
    unsigned long pfn;

    for (va_check = addr; va_check < end; va_check += KA_MM_PAGE_SIZE) {
        if (ka_mm_follow_pfn(vma, va_check, &pfn) == 0) {
            hdcdrv_err("va_check is invalid. (ddr=%pK; size=%lu; va_check=%lx)\n",
                (void *)(uintptr_t)f_mem->user_va, size, va_check);
            return HDCDRV_PARA_ERR;
        }
    }

    return HDCDRV_OK;
}

STATIC int hdcdrv_remap_va(void *ctx, struct hdcdrv_fast_mem *f_mem)
{
    int i, ret;
    unsigned int len;
    unsigned int offset = 0;
    ka_vm_area_struct_t *vma = NULL;

    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        return HDCDRV_OK;
    }

    ka_task_down_write(get_mmap_sem(ka_task_get_current_mm()));

    vma = ka_mm_find_vma(ka_task_get_current_mm(), f_mem->user_va);
    if (vma == NULL) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        hdcdrv_err("Find vma failed. (devid=%d; va=0x%pK)\n", f_mem->devid, (void *)(uintptr_t)f_mem->user_va);
        return HDCDRV_FIND_VMA_FAIL;
    }

    ret = hdcdrv_check_va(ctx, vma, f_mem);
    if (ret != HDCDRV_OK) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        return ret;
    }

    ret = hdcdrv_follow_pfn_check(vma, f_mem);
    if (ret != HDCDRV_OK) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        return ret;
    }

    if (hdcdrv_get_running_env() == HDCDRV_RUNNING_ENV_ARM_3559) {
        ka_mm_set_vm_flags(vma, KA_VM_IO | KA_VM_SHARED);
        /*lint -e446 */
        ka_mm_set_vm_pgprot(vma);
        /*lint +e446 */
    }

    for (i = 0; i < f_mem->phy_addr_num; i++) {
        len = f_mem->mem[i].len;
        if (len > 0) {
            /*lint -e648 */
            ret = ka_mm_remap_pfn_range(vma, f_mem->user_va + offset, ka_mm_page_to_pfn(f_mem->mem[i].page), len,
                *(ka_mm_get_vm_pgprot(vma)));
            /*lint +e648 */
        }
        offset += len;
        if (ret != 0) {
            break;
        }
    }
    if (i == f_mem->phy_addr_num) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        return HDCDRV_OK;
    }

    hdcdrv_zap_vma_ptes(f_mem, vma, i);

    hdcdrv_err("Remap va failed. (dev=%d; vma_start=%pK; end=%pK; addr=%pK; len=%x)\n", f_mem->devid,
        (void *)(uintptr_t)ka_mm_get_vm_start(vma), (void *)(uintptr_t)ka_mm_get_vm_end(vma),
        (void *)(uintptr_t)f_mem->user_va, f_mem->alloc_len);

    ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));

    return HDCDRV_DMA_MPA_FAIL;
}

STATIC int hdcdrv_unmap_va(struct hdcdrv_fast_mem *f_mem, const void *ctx)
{
    ka_vm_area_struct_t *vma = NULL;
    int ret;

    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        return HDCDRV_OK;
    }

    ka_task_down_read(get_mmap_sem(ka_task_get_current_mm()));

    vma = ka_mm_find_vma(ka_task_get_current_mm(), f_mem->user_va);
    if (vma == NULL) {
        hdcdrv_err("Find vma failed. (devid=%d; va=0x%pK)\n", f_mem->devid, (void *)(uintptr_t)f_mem->user_va);
        ka_task_up_read(get_mmap_sem(ka_task_get_current_mm()));
        return HDCDRV_FIND_VMA_FAIL;
    }

    ret = hdcdrv_check_va(ctx, vma, f_mem);
    if (ret != HDCDRV_OK) {
        ka_task_up_read(get_mmap_sem(ka_task_get_current_mm()));
        return HDCDRV_FIND_VMA_FAIL;
    }

    hdcdrv_zap_vma_ptes(f_mem, vma, f_mem->phy_addr_num);

    ka_task_up_read(get_mmap_sem(ka_task_get_current_mm()));

    return HDCDRV_OK;
}

u64 hdcdrv_get_hash(u64 user_va, u64 pid, u32 fid)
{
    /*
     * 1. virtual address max 48 bits
     * 2. virtual address aligned by 4k at last, so there are 28 (16+12) bits to store pid
     * 3. 64 bits linux system pid max support 4194304 (23 bits)
     * 4. fid use 60~63 bits
     */
    u64 tfid;
    u64 tpid = (u64)pid;
    u64 hash_va;

    tfid = hdcdrv_get_hash_fid(fid);

    hash_va = ((tfid & HDCDRV_FRBTREE_FID_MASK) << HDCDRV_FRBTREE_FID_BEG) |
        ((user_va & HDCDRV_FRBTREE_ADDR_MASK) << (HDCDRV_FRBTREE_ADDR_BEG - HDCDRV_FRBTREE_ADDR_DEL)) |
        (tpid & HDCDRV_FRBTREE_PID_MASK);

    return hash_va;
}

STATIC int hdcdrv_unmap_va_for_kernel(struct hdcdrv_fast_node *fast_node, struct hdcdrv_fast_mem *f_mem)
{
    ka_vm_area_struct_t *vma = NULL;
    ka_struct_pid_t *pro_pid = NULL;
    ka_task_struct_t *task = NULL;
    ka_mm_struct_t *task_mm = NULL;
    int ret = HDCDRV_OK;

    if (f_mem->page_type == HDCDRV_PAGE_TYPE_HUGE) {
        return ret;
    }

    pro_pid = ka_task_find_get_pid((int)fast_node->pid);
    if (pro_pid == NULL) {
        return ret;
    }

    task = ka_task_get_pid_task(pro_pid, KA_PIDTYPE_PID);
    if (task == NULL) {
        goto unmap_put_pid;
    }

    task_mm = ka_task_get_task_mm(task);
    if (task_mm == NULL) {
        goto unmap_put_task_struct;
    }

    ka_task_down_read(get_mmap_sem(task_mm));
    vma = ka_mm_find_vma(task_mm, f_mem->user_va);
    if (vma == NULL) {
        goto unmap_up_read;
    }

    ret = hdcdrv_check_va(fast_node->ctx, vma, f_mem);
    if (ret != HDCDRV_OK) {
        goto unmap_up_read;
    }

    hdcdrv_zap_vma_ptes(f_mem, vma, f_mem->phy_addr_num);

unmap_up_read:
    ka_task_up_read(get_mmap_sem(task_mm));
    ka_mm_mmput(task_mm);
unmap_put_task_struct:
    ka_task_put_task_struct(task);
unmap_put_pid:
    ka_task_put_pid(pro_pid);

    return ret;
}

#ifndef CFG_FEATURE_HDC_REG_MEM
void hdcdrv_fast_mem_uninit(ka_task_spinlock_t *lock, ka_rb_root_t *root, int reset, int flag)
{
    ka_rb_node_t *node = NULL;
    struct hdcdrv_fast_node *fast_node = NULL, *fast_node_tmp = NULL;
    int ret = 0;
    u64 hash_va;

    /* only uninit free, suspend status not free */
    if (hdcdrv_get_running_status() != HDCDRV_RUNNING_NORMAL) {
        return;
    }

    ka_task_spin_lock_bh(lock);

    while ((node = ka_base_rb_first(root)) != NULL) {
        fast_node_tmp = ka_base_rb_entry(node, struct hdcdrv_fast_node, node);
        if ((fast_node_tmp->pid == hdcdrv_get_pid()) || (reset == HDCDRV_TRUE_FLAG)) {
            hash_va = fast_node_tmp->hash_va;
            ka_task_spin_unlock_bh(lock);
            fast_node = hdcdrv_fast_node_search_timeout(lock, root, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
            if (fast_node == NULL) {
                ka_task_spin_lock_bh(lock);
                continue;
            }
            hdcdrv_fast_node_erase(lock, root, fast_node);
            ret = hdcdrv_dma_unmap(&fast_node->fast_mem, (u32)fast_node->fast_mem.devid,
                HDCDRV_SYNC_NO_CHECK, flag);
            if (ret != HDCDRV_OK) {
                hdcdrv_err("Dma unmap failed. (dev=%d; pid=%lld)\n", fast_node->fast_mem.devid, fast_node->pid);
            }
            ret = hdcdrv_unmap_va_for_kernel(fast_node, &fast_node->fast_mem);
            if (ret != 0) {
                hdcdrv_err("Unmap va failed. (dev=%d; pid=%lld)\n", fast_node->fast_mem.devid, fast_node->pid);
                // This VMA isn't part of HDC and won't release physical memory to prevent serious security issues
                hdcdrv_node_status_idle(fast_node);
                hdcdrv_fast_node_free(fast_node);
                fast_node = NULL;
                ka_task_spin_lock_bh(lock);
                continue;
            }
            hdcdrv_fast_free_phy_mem(&fast_node->fast_mem);
            hdcdrv_node_status_idle(fast_node);
            hdcdrv_fast_node_free(fast_node);
            fast_node = NULL;
            ka_task_spin_lock_bh(lock);
        }
    }

    ka_task_spin_unlock_bh(lock);
}
#endif

void hdcdrv_fast_mem_free_abnormal(const struct hdcdrv_mem_node_info *f_info)
{
    u32 fid;
    int flag = HDCDRV_DEL_FLAG;
    struct hdcdrv_fast_node *fast_node = NULL;
    struct hdcdrv_dev_fmem *dev_fmem = NULL;
    struct hdcdrv_fast_node_msg_info node_msg = { 0 };

    dev_fmem = hdcdrv_get_dev_fmem_uni();
    fast_node = hdcdrv_fast_node_search_timeout(&dev_fmem->rb_lock,
        &dev_fmem->rbtree, f_info->hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    fid = (f_info->hash_va >> HDCDRV_FRBTREE_FID_BEG) & HDCDRV_FRBTREE_FID_MASK;
    if (fast_node == NULL) {
        hdcdrv_node_msg_info_fill((u32)f_info->pid, fid, (int)f_info->alloc_len, f_info->user_va,
                                  HDCDRV_SEARCH_NODE_UNREGISTER, &node_msg);
        fast_node = hdcdrv_fast_node_search_from_new_tree(HDCDRV_RBTREE_SIDE_LOCAL, HDCDRV_NODE_RELEASE_TIME_MAX,
                                                          &node_msg);
        flag = HDCDRV_DEL_REGISTER_FLAG;
    }
    if (fast_node == NULL) {
        hdcdrv_err("Fast node search failed when release. (pid=%llx)\n", f_info->pid);
        return;
    }

    (void)hdcdrv_dma_unmap(&fast_node->fast_mem, (u32)fast_node->fast_mem.devid, HDCDRV_SYNC_NO_CHECK, flag);
    if (flag == HDCDRV_DEL_REGISTER_FLAG) {
        hdcdrv_fast_node_erase_from_new_tree((u64)fast_node->pid, fid, 0, HDCDRV_RBTREE_SIDE_LOCAL, fast_node);
    } else {
        (void)hdcdrv_unmap_va_for_kernel(fast_node, &fast_node->fast_mem);
        hdcdrv_fast_node_erase(&dev_fmem->rb_lock, &dev_fmem->rbtree, fast_node);
    }

    fast_node->fast_mem.alloc_len = 0;
    hdcdrv_fast_free_phy_mem(&fast_node->fast_mem);
    hdcdrv_node_status_init(fast_node);
    hdcdrv_fast_node_free(fast_node);
    fast_node = NULL;
}

void __attribute__((weak)) hdcdrv_fast_mem_quick_proc(const struct hdcdrv_mem_node_info *f_info) {}

void hdcdrv_release_unmap_failed_fast_mem(struct hdcdrv_ctx_fmem *ctx_fmem)
{
    struct hdcdrv_mem_fd_list *entry = NULL;
    struct hdcdrv_fast_node *fast_node = NULL;

    ka_task_spin_lock_bh(&ctx_fmem->mem_lock);
    if ((ka_list_empty_careful(&ctx_fmem->mlist.list)) || (ctx_fmem->mem_count == 0)) {
        ka_task_spin_unlock_bh(&ctx_fmem->mem_lock);
        return;
    }
    ka_task_spin_unlock_bh(&ctx_fmem->mem_lock);

    /* memory free */
    hdcdrv_info("Release abnormal memory. (task_pid=%llu; count=%llu)\n", hdcdrv_get_pid(), ctx_fmem->mem_count);
    ka_system_usleep_range(HDCDRV_USLEEP_RANGE_2000, HDCDRV_USLEEP_RANGE_3000);

    while (1) {
        entry = hdcdrv_release_get_free_mem_entry(ctx_fmem);
        if (entry == NULL) {
            return;
        }
        fast_node = entry->f_node;

        hdcdrv_dma_unmap(&fast_node->fast_mem, fast_node->fast_mem.devid, HDCDRV_SYNC_NO_CHECK, HDCDRV_DEL_FLAG);
        hdcdrv_fast_free_phy_mem(&fast_node->fast_mem);
        hdcdrv_fast_node_free(fast_node);
#ifndef DRV_UT
        fast_node = NULL;
        hdcdrv_kfree(entry, KA_SUB_MODULE_TYPE_1);
        entry = NULL;
#endif
    }
}

STATIC long hdcdrv_fast_alloc_addr_check(struct hdcdrv_cmd_alloc_mem *cmd)
{
    if (((cmd->type == HDCDRV_FAST_MEM_TYPE_TX_DATA) || (cmd->type == HDCDRV_FAST_MEM_TYPE_RX_DATA) ||
        (cmd->type == HDCDRV_FAST_MEM_TYPE_DVPP) || (cmd->type == HDCDRV_FAST_MEM_TYPE_ANY)) &&
        (cmd->len > HDCDRV_MEM_MAX_LEN)) {
        hdcdrv_err("Fast alloc address check error. (cmd_type=%d; cmd_len=0x%x)\n", cmd->type, cmd->len);
        return HDCDRV_PARA_ERR;
    }

    if (((cmd->type == HDCDRV_FAST_MEM_TYPE_TX_CTRL) || (cmd->type == HDCDRV_FAST_MEM_TYPE_RX_CTRL)) &&
        (cmd->len > HDCDRV_CTRL_MEM_MAX_LEN)) {
        hdcdrv_err("Fast alloc address check error. (cmd_type=%d; cmd_len=0x%x)\n", cmd->type, cmd->len);
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->page_type != HDCDRV_PAGE_TYPE_HUGE) && (cmd->page_type != HDCDRV_PAGE_TYPE_NORMAL)) {
        hdcdrv_err("Fast alloc address check error. (cmd_page_type=%d)\n", cmd->page_type);
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->page_type == HDCDRV_PAGE_TYPE_HUGE) &&
        ((cmd->va != ka_base_round_down(cmd->va, KA_HPAGE_SIZE)) || (cmd->len % KA_HPAGE_SIZE != 0))) {
        hdcdrv_err("Fast alloc address check error. (page_type=%d; cmd_len=0x%x; KA_HPAGE_SIZE=%ld)\n",
            cmd->page_type, cmd->len, KA_HPAGE_SIZE);
        return HDCDRV_PARA_ERR;
    }

    if (cmd->len < KA_MM_PAGE_SIZE) {
        hdcdrv_info("cmd_len is smaller than KA_MM_PAGE_SIZE. (cmd_len=0x%x; KA_MM_PAGE_SIZE=%ld)\n", cmd->len, KA_MM_PAGE_SIZE);
        cmd->len = KA_MM_PAGE_SIZE;
    }

    if ((cmd->len % KA_MM_PAGE_SIZE) != 0) {
        hdcdrv_err("Fast alloc address check error. (cmd_len=0x%x)\n", cmd->len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

STATIC int hdcdrv_alloc_mem_param_check(int map, int devid, unsigned int type, unsigned int len)
{
    if ((map != 0) && ((devid >= hdcdrv_get_max_support_dev()) || (devid < 0))) {
        hdcdrv_err("Input parameter is error. (devid=%d)\n", devid);
        return HDCDRV_PARA_ERR;
    }

    if ((type >= HDCDRV_FAST_MEM_TYPE_MAX) || (len == 0)) {
        hdcdrv_err("Input parameter is error. (type=%u; len=%u)\n", type, len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

long hdccom_fast_alloc_mem(void *ctx, struct hdcdrv_cmd_alloc_mem *cmd,
    struct hdcdrv_fast_node **f_node_ret)
{
    struct hdcdrv_fast_node *f_node = NULL;
    long ret;
    long res;
    struct hdcdrv_dev_fmem *dev_fmem = hdcdrv_get_dev_fmem_uni();

    ret = hdcdrv_alloc_mem_param_check(cmd->map, cmd->dev_id, cmd->type, cmd->len);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Check parameter error. (devid=%d; type=%u; len=%u)\n", cmd->dev_id, cmd->type, cmd->len);
        return HDCDRV_PARA_ERR;
    }

    if (hdcdrv_fast_alloc_addr_check(cmd) != HDCDRV_OK) {
        hdcdrv_err("Check fast alloc addr failed. (devid=%d; addr=%pK; len=0x%x; type=%u; data_max_len=0x%x; "
            "ctrl_max_len=0x%x; page_type=%u)\n", cmd->dev_id, (void *)(uintptr_t)cmd->va, cmd->len, cmd->type,
            HDCDRV_MEM_MAX_LEN, HDCDRV_CTRL_MEM_MAX_LEN, cmd->page_type);
        return HDCDRV_PARA_ERR;
    }

    f_node = (struct hdcdrv_fast_node *)hdcdrv_kvmalloc(sizeof(struct hdcdrv_fast_node), KA_SUB_MODULE_TYPE_2);
    if (f_node == NULL) {
        hdcdrv_err("Calling ka_mm_kzalloc failed. (dev=%d)\n", cmd->dev_id);
        return HDCDRV_MEM_ALLOC_FAIL;
    }

    f_node->pid = (long long)cmd->pid;
    f_node->fast_mem.page_type = cmd->page_type;
    f_node->fast_mem.hash_va = hdcdrv_get_hash(cmd->va, cmd->pid, 0);
#ifdef CFG_FEATURE_HDC_REG_MEM
    f_node->fast_mem.align_size = 0; // in alloc memory screan, no need use align_size to calc page_index.
#endif

    ret = hdcdrv_fast_alloc_phy_mem(&f_node->fast_mem, cmd->va, cmd->len, cmd->type, (u32)cmd->dev_id);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Calling hdcdrv_fast_alloc_phy_mem failed. (dev=%d)\n", cmd->dev_id);
        goto fast_alloc_fail;
    }

    if (cmd->map != 0) {
        ret = hdcdrv_dma_map(&f_node->fast_mem, cmd->dev_id, HDCDRV_ADD_FLAG);
        if (ret != HDCDRV_OK) {
            hdcdrv_err("Calling hdcdrv_dma_map failed. (dev=%d)\n", cmd->dev_id);
            goto dma_map_fail;
        }
    }

    ret = (long)hdcdrv_remap_va(ctx, &f_node->fast_mem);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Calling hdcdrv_remap_va failed.\n");
        goto map_va_fail;
    }

    f_node->hash_va = f_node->fast_mem.hash_va;
    dev_fmem = hdcdrv_get_dev_fmem_uni();
    ret = hdcdrv_fast_node_insert(&dev_fmem->rb_lock, &dev_fmem->rbtree, f_node, HDCDRV_SEARCH_WITH_HASH);
    if (ret != HDCDRV_OK) {
        hdcdrv_info("Calling hdcdrv_fast_node_insert abnormal. (dev=%d)\n", cmd->dev_id);
        goto node_insert_fail;
    }
    *f_node_ret = f_node;

    return HDCDRV_OK;

node_insert_fail:
    res = hdcdrv_unmap_va(&f_node->fast_mem, ctx);
    if (res != HDCDRV_OK) {
        // fast mem operation can not be used from kernel.
        // If ctx exist (from ioctl), mem can be recorded and free when process release
        // else (from kernel), no need to record and no secure problem will be caused
        hdcdrv_err("Calling hdcdrv_unmap_va failed. (dev=%d)\n", cmd->dev_id);
        if ((ctx != NULL) && (ctx != HDCDRV_KERNEL_WITHOUT_CTX)) {
            *f_node_ret = f_node;
            return HDCDRV_VA_UNMAP_FAILED;
        }
    }
map_va_fail:
    if (cmd->map != 0) {
        res = hdcdrv_dma_unmap(&f_node->fast_mem, (u32)cmd->dev_id, HDCDRV_SYNC_NO_CHECK, HDCDRV_DEL_FLAG);
        if (res != HDCDRV_OK) {
            hdcdrv_err("Calling hdcdrv_dma_unmap failed. (dev=%d)\n", cmd->dev_id);
        }
    }
dma_map_fail:
    hdcdrv_fast_free_phy_mem(&f_node->fast_mem);
fast_alloc_fail:
    hdcdrv_fast_node_free(f_node);
    f_node = NULL;

    return ret;
}

long hdcdrv_fast_free_mem(const void *ctx, struct hdcdrv_cmd_free_mem *cmd)
{
    struct hdcdrv_fast_node *f_node = NULL;
    struct hdcdrv_dev_fmem *dev_fmem = hdcdrv_get_dev_fmem_uni();
    u64 hash_va;
    int ret;

    if ((cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX)) {
        hdcdrv_err("Input parameter is error. (type=%d; va=0x%pK)\n", cmd->type, (void *)(uintptr_t)cmd->va);
        return HDCDRV_PARA_ERR;
    }

    hash_va = hdcdrv_get_hash(cmd->va, cmd->pid, HDCDRV_DEFAULT_LOCAL_FID);
    f_node = hdcdrv_fast_node_search_timeout(&dev_fmem->rb_lock, &dev_fmem->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_err("Fast node search failed. (va=0x%pK; hash_va=0x%016llx)\n", (void *)(uintptr_t)cmd->va, hash_va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (ctx != f_node->ctx) {
        hdcdrv_err("ctx not match. (fast_mem_devid=%d; cmd_type=%u)\n", f_node->fast_mem.devid, cmd->type);
        hdcdrv_node_status_idle(f_node);
        return HDCDRV_PARA_ERR;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_err("cmd_type is invalid. (fast_mem_devid=%d; cmd_type=%u; mem_type=%d)\n",
            f_node->fast_mem.devid, cmd->type, f_node->fast_mem.mem_type);
        hdcdrv_node_status_idle(f_node);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_dma_unmap(&f_node->fast_mem, (u32)f_node->fast_mem.devid, HDCDRV_SYNC_CHECK, HDCDRV_DEL_FLAG);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Calling hdcdrv_dma_unmap failed. (type=%d)\n", cmd->type);
        hdcdrv_node_status_idle(f_node);
        return ret;
    }

    ret = hdcdrv_unmap_va(&f_node->fast_mem, ctx);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Calling hdcdrv_unmap_va failed. (type=%d)\n", cmd->type);
        hdcdrv_node_status_idle(f_node);
        return ret;
    }

    cmd->len = f_node->fast_mem.alloc_len;
    cmd->page_type = f_node->fast_mem.page_type;

    f_node->fast_mem.alloc_len = 0;
    hdcdrv_fast_free_phy_mem(&f_node->fast_mem);

    hdcdrv_unbind_mem_ctx(f_node);

    hdcdrv_fast_node_erase(&dev_fmem->rb_lock, &dev_fmem->rbtree, f_node);
    hdcdrv_node_status_idle(f_node);
    hdcdrv_fast_node_free(f_node);
    f_node = NULL;

    return HDCDRV_OK;
}

long hdcdrv_fast_dma_map(const struct hdcdrv_cmd_dma_map *cmd)
{
    int ret;
    struct hdcdrv_fast_node *f_node = NULL;
    struct hdcdrv_dev_fmem *dev_fmem = hdcdrv_get_dev_fmem_uni();
    u64 hash_va;
    u64 pid;

    if ((cmd->dev_id >= hdcdrv_get_max_support_dev()) || (cmd->dev_id < 0)) {
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX)) {
        hdcdrv_err("Input parameter cmd type is error. (devid=%d; type=%u; va=0x%pK)\n",
            cmd->dev_id, cmd->type, (void *)(uintptr_t)cmd->va);
        return HDCDRV_PARA_ERR;
    }

    pid = cmd->pid;
    hash_va = hdcdrv_get_hash(cmd->va, pid, HDCDRV_DEFAULT_LOCAL_FID);
    f_node = hdcdrv_fast_node_search_timeout(&dev_fmem->rb_lock, &dev_fmem->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_err("Fast node search va failed, NULL or timeout. (va=0x%pK)\n", (void *)(uintptr_t)cmd->va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("Parameter mem_type not match. (type=%u; mem_type=%d)\n", cmd->type, f_node->fast_mem.mem_type);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_dma_map(&f_node->fast_mem, cmd->dev_id, HDCDRV_ADD_FLAG);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_info("Calling hdcdrv_dma_map abnormal. (devid=%d)\n", f_node->fast_mem.devid);
        return ret;
    }

    hdcdrv_node_status_idle(f_node);
    return HDCDRV_OK;
}

long hdcdrv_fast_dma_unmap(const struct hdcdrv_cmd_dma_unmap *cmd)
{
    u64 pid;
    u64 hash_va;
    struct hdcdrv_fast_node *f_node = NULL;
    struct hdcdrv_dev_fmem *dev_fmem = hdcdrv_get_dev_fmem_uni();
    int ret;

    if ((cmd->dev_id >= hdcdrv_get_max_support_dev()) || (cmd->dev_id < 0)) {
        return HDCDRV_PARA_ERR;
    }

    if (cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX) {
        hdcdrv_err("Input parameter is error. (type=%d; va=0x%pK; dev=%d)\n", cmd->type,
            (void *)(uintptr_t)cmd->va, cmd->dev_id);
        return HDCDRV_PARA_ERR;
    }

    pid = cmd->pid;
    hash_va = hdcdrv_get_hash(cmd->va, pid, HDCDRV_DEFAULT_LOCAL_FID);
    f_node = hdcdrv_fast_node_search_timeout(&dev_fmem->rb_lock, &dev_fmem->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_err("Fast node search va failed. (va=0x%pK)\n", (void *)(uintptr_t)cmd->va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("Parameter mem_type not match. (type=%u; mem_type=%d)\n", cmd->type, f_node->fast_mem.mem_type);
        return HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_dma_unmap(&f_node->fast_mem, (u32)f_node->fast_mem.devid, HDCDRV_SYNC_CHECK, HDCDRV_DEL_FLAG);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("Calling hdcdrv_dma_unmap failed. (type=%d)\n", cmd->type);
        return ret;
    }

    hdcdrv_node_status_idle(f_node);

    return HDCDRV_OK;
}

long hdcdrv_fast_dma_remap(const struct hdcdrv_cmd_dma_remap *cmd)
{
    int ret;
    u64 pid;
    u64 hash_va;
    struct hdcdrv_fast_node *f_node = NULL;
    struct hdcdrv_dev_fmem *dev_fmem = hdcdrv_get_dev_fmem_uni();
    int devid;

    if ((cmd->dev_id >= hdcdrv_get_max_support_dev()) || (cmd->dev_id < 0)) {
        return HDCDRV_PARA_ERR;
    }

    if ((cmd->type >= HDCDRV_FAST_MEM_TYPE_MAX)) {
        hdcdrv_err("Input parameter is error. (devid=%d; type=%u; va=0x%pK)\n",
            cmd->dev_id, cmd->type, (void *)(uintptr_t)cmd->va);
        return HDCDRV_PARA_ERR;
    }

    pid = cmd->pid;
    hash_va = hdcdrv_get_hash(cmd->va, pid, HDCDRV_DEFAULT_LOCAL_FID);
    dev_fmem = hdcdrv_get_dev_fmem_uni();
    f_node = hdcdrv_fast_node_search_timeout(&dev_fmem->rb_lock, &dev_fmem->rbtree, hash_va, HDCDRV_NODE_WAIT_TIME_MAX);
    if (f_node == NULL) {
        hdcdrv_err("Fast node search va failed. (va=0x%pK)\n", (void *)(uintptr_t)cmd->va);
        return HDCDRV_F_NODE_SEARCH_FAIL;
    }

    if (cmd->type != f_node->fast_mem.mem_type) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("mem_type not match. (type=%u; mem_type=%d)\n", cmd->type, f_node->fast_mem.mem_type);
        return HDCDRV_PARA_ERR;
    }

    devid = f_node->fast_mem.devid;
    ret = hdcdrv_dma_unmap(&f_node->fast_mem, (u32)f_node->fast_mem.devid, HDCDRV_SYNC_CHECK, HDCDRV_DEL_FLAG);
    if (ret != HDCDRV_OK) {
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("Calling hdcdrv_dma_unmap failed. (devid=%d)\n", f_node->fast_mem.devid);
        return ret;
    }

    ret = hdcdrv_dma_map(&f_node->fast_mem, cmd->dev_id, HDCDRV_ADD_FLAG);
    if (ret != HDCDRV_OK) {
        ret = hdcdrv_dma_map(&f_node->fast_mem, devid, HDCDRV_ADD_FLAG);
        if (ret != HDCDRV_OK) {
            hdcdrv_err("Calling hdcdrv_dma_map failed. (devid=%d)\n", devid);
        }
        hdcdrv_node_status_idle(f_node);
        hdcdrv_err("Calling hdcdrv_dma_map failed. (devid=%d)\n", cmd->dev_id);
        return ret;
    }

    hdcdrv_node_status_idle(f_node);

    return HDCDRV_OK;
}
