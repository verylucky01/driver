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
#include "ka_common_pub.h"
#include "ka_memory_pub.h"
#include "trs_chan.h"
#include "trs_rsv_mem.h"
#include "trs_chan_mem.h"

static void trs_chan_mem_make_exact(ka_page_t *p, u32 order, size_t size)
{
#ifndef CFG_FEATURE_HOST_ENV
    ka_page_t *cur_page = NULL;
    u64 alloced_num, page_num, i;

    if (order == 0) {
        return;
    }
    ka_mm_split_page(p, order);
    alloced_num = 1ull << order;
    page_num = KA_MM_PAGE_ALIGN(size) / KA_MM_PAGE_SIZE;

    for (i = page_num; i < alloced_num; i++) {
        cur_page = ka_mm_nth_page(p, i);
        __ka_mm_free_page(cur_page);
    }
#endif
}

void *trs_chan_mem_alloc_ddr(struct trs_id_inst *inst, int nid, size_t size, phys_addr_t *paddr)
{
    u32 order = ka_mm_get_order(size);
    ka_page_t *p = NULL;

    if (nid == NUMA_NO_NODE) {
        p = ka_mm_alloc_pages(__KA_GFP_ZERO | __KA_GFP_ACCOUNT | KA_GFP_HIGHUSER_MOVABLE | __KA_GFP_NOWARN, order);
    } else {
        p = ka_mm_alloc_pages_node(nid, __KA_GFP_THISNODE | __KA_GFP_ZERO | __KA_GFP_ACCOUNT | KA_GFP_HIGHUSER_MOVABLE | __KA_GFP_NOWARN,
            order);
    }
    if (p == NULL) {
        return NULL;
    }
    trs_chan_mem_make_exact(p, order, size);
    *paddr = ka_mm_page_to_phys(p);

    return (void *)ka_mm_page_address(p);
}

void trs_chan_mem_free_ddr(struct trs_id_inst *inst, void *vaddr, size_t size)
{
    if (vaddr != NULL) {
#ifdef CFG_FEATURE_HOST_ENV
        ka_mm_free_pages((unsigned long)(uintptr_t)vaddr, ka_mm_get_order(size));
#else
        ka_mm_free_pages_exact(vaddr, size);
#endif
    }
}

void *trs_chan_mem_alloc_rsv(struct trs_id_inst *inst, int type, size_t size, phys_addr_t *paddr, u32 flag)
{
    void *vaddr = NULL;
    int ret;

    vaddr = trs_rsv_mem_alloc(inst, type, size, flag);
    if (vaddr == NULL) {
#ifndef EMU_ST
        trs_debug("No rsv mem. (devid=%u; tsid=%u; type=%d; size=0x%lx)\n", inst->devid, inst->tsid, type, size);
#endif
        return NULL;
    }
    ret = trs_rsv_mem_v2p(inst, type, vaddr, paddr);
    if (ret != 0) {
        trs_rsv_mem_free(inst, type, vaddr, size);
#ifndef EMU_ST
        trs_err("Rcv mem v2p fail. (devid=%u; tsid=%u; type=%d; size=%lu; ret=%d)\n",
                inst->devid, inst->tsid, type, size, ret);
#endif
        return NULL;
    }

    return vaddr;
}

void trs_chan_mem_free_rsv(struct trs_id_inst *inst, int type, void *sq_addr, size_t size)
{
    trs_rsv_mem_free(inst, type, sq_addr, size);
}
