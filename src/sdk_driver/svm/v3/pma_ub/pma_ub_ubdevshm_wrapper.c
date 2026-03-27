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
#include "ubdevshm.h"

#include "ka_base_pub.h"
#include "ka_system_pub.h"
#include "ka_task_pub.h"

#include "svm_kern_log.h"
#include "framework_task.h"
#include "dbi_kern.h"
#include "va_query.h"
#include "pma_ub_core.h"
#include "pma_ub_ubdevshm_wrapper.h"

static int _pma_ub_ubdevshm_acquire(int master_tgid, u64 va, u64 size,
    int (*invalidate)(u64 invalidate_tag), u64 invalidate_tag,
    u32 *token_id_out, dbi_bus_inst_eid_t *eid_out)
{
    dbi_bus_inst_eid_t eid = {0};
    u32 udevid;
    int ret;

    ret = svm_query_va_udevid(master_tgid, va, size, &udevid);
    if (ret != 0) {
        svm_err("Get va udevid failed. (ret=%d; va=0x%llx; size=%llu)\n", ret, va, size);
        return ret;
    }
    if (udevid == uda_get_host_id()) {
        svm_err("Not support host svm addr. (va=0x%llx; size=%llu)\n", va, size);
        return -EINVAL;
    }

    ret = svm_dbi_kern_query_bus_inst_eid(udevid, &eid);
    if (ret != 0) {
        svm_err("Get eid failed. (ret=%d; udevid=%u; tgid=%d)\n", ret, udevid, master_tgid);
        return ret;
    }

    ret = pma_ub_acquire_seg(udevid, master_tgid, va, size, invalidate, invalidate_tag, token_id_out);
    if (ret != 0) {
        svm_err("Acquire seg token id failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx; size=%llu)\n",
            ret, udevid, master_tgid, va, size);
        return ret;
    }

    *eid_out = eid;
    return 0;
}

static int _pma_ub_ubdevshm_release(int master_tgid, u64 va, u64 size)
{
    u32 udevid;
    int ret;

    ret = svm_query_va_udevid(master_tgid, va, size, &udevid);
    if (ret != 0) {
        svm_err("Get va udevid failed. (ret=%d; va=0x%llx; size=%llu)\n", ret, va, size);
        return ret;
    }
    if (udevid == uda_get_host_id()) {
        svm_err("Not support host svm addr. (va=0x%llx; size=%llu)\n", va, size);
        return -EINVAL;
    }

    return pma_ub_release_seg(udevid, master_tgid, va, size);
}

static int pma_ub_ubdevshm_acquire_para_check(struct mem_uva *uva, union acquire_attr *attr,
    invalidate func, struct mem_uba *uba)
{
    if (uva == NULL) {
        svm_err("Uva is NULL.\n");
        return -EINVAL;
    }

    if (attr == NULL) {
        svm_err("Attr is NULL.\n");
        return -EINVAL;
    }

    if (func == NULL) {
        svm_err("Invalidate func is NULL.\n");
        return -EINVAL;
    }

    if (uba == NULL) {
        svm_err("Uba is NULL.\n");
        return -EINVAL;
    }

    if ((attr->bs.require_pin != 0) || (attr->bs.require_invalidate != 1)) {
        svm_info("Invalid acquire attr. (require_pin=%llu, require_invalidate=%llu)\n",
            attr->bs.require_pin, attr->bs.require_invalidate);
        return -EOPNOTSUPP;
    }

    return 0;
}

static void pma_ub_dbi_to_uba_eid(const dbi_bus_inst_eid_t *src, struct uba_eid *dst)
{
    u64 hi, lo;

    hi = ka_get_unaligned_be64(&src->raw[0]); /* read 64-bit BE from offset 0 */
    lo = ka_get_unaligned_be64(&src->raw[8]); /* read 64-bit BE from offset 8 */

    dst->eid = lo & KA_GENMASK_ULL(19, 0); /* eid = bits [19:0] */
    dst->reserved0 = (lo >> 20) & KA_GENMASK_ULL(43, 0); /* reserved0 = bits [63:20] (shift 20; mask bits [43:0]) */
    dst->reserved1 = hi;
}

static void pma_ub_ubdevshm_uba_pack(u64 va, u64 size, u32 token_id, dbi_bus_inst_eid_t *eid,
    struct mem_uba *uba)
{
    uba->uba = va; /* Device uba is va */
    uba->size = size;
    uba->token_id = token_id;
    uba->attr.bs.readable = 1;
    uba->attr.bs.writeable = 1;
    uba->attr.bs.executeable = 1;

    pma_ub_dbi_to_uba_eid(eid, &uba->eid);
}

static int pma_ub_ubdevshm_acquire(struct mem_uva *uva, union acquire_attr *attr, invalidate func, struct mem_uba *uba)
{
    dbi_bus_inst_eid_t eid;
    u32 token_id;
    int master_tgid = ka_task_get_current_tgid();
    int ret;

    ka_task_might_sleep();

    ret = pma_ub_ubdevshm_acquire_para_check(uva, attr, func, uba);
    if (ret != 0) {
        return ret;
    }

    ret = _pma_ub_ubdevshm_acquire(master_tgid, uva->va, uva->size, func, uva->invalidate_tag, &token_id, &eid);
    if (ret == 0) {
        pma_ub_ubdevshm_uba_pack(uva->va, uva->size, token_id, &eid, uba);
        svm_debug("Acquire success. (va=0x%llx; size=%llu; eid=0x%llx)\n", uva->va, uva->size, (u64)uba->eid.eid);
    }
    return ret;
}

static int pma_ub_ubdevshm_release(struct mem_uba *uba)
{
    int master_tgid = ka_task_get_current_tgid();

    ka_task_might_sleep();

    if (uba == NULL) {
        svm_err("Uba is NULL.\n");
        return -EINVAL;
    }

    return _pma_ub_ubdevshm_release(master_tgid, uba->uba, uba->size);
}

static struct ubdevshm_mem_ops g_pma_ub_ubdevshm_ops = {
    .name = "svm_ubdevshm_ops",
    .version = 0,
    .acquire = pma_ub_ubdevshm_acquire,
    .release = pma_ub_ubdevshm_release
};

static unsigned long g_ubdevshm_handle;

static int pma_ub_ubdevshm_register_ops(void)
{
    int ret = ubdevshm_register_ops(&g_pma_ub_ubdevshm_ops, &g_ubdevshm_handle);
    if (ret != 0) {
        svm_err("ubdevshm_register_ops failed. (ret=%d)\n", ret);
    }

    return ret;
}

static void pma_ub_ubdevshm_unregister_ops(void)
{
    int ret = ubdevshm_unregister_ops(&g_ubdevshm_handle);
    if (ret != 0) {
        svm_err("ubdevshm_register_ops failed. (ret=%d)\n", ret);
    }
}

int pma_ub_ubdevshm_wrapper_init(void)
{
    return pma_ub_ubdevshm_register_ops();
}

void pma_ub_ubdevshm_wrapper_uninit(void)
{
    pma_ub_ubdevshm_unregister_ops();
}

static void pma_ub_ubdevshm_uva_pack(int tgid, struct task_start_time *start_time, u64 va, u64 size,
    struct mem_uva *uva)
{
    uva->va = va;
    uva->size = size;
    uva->invalidate_tag = (((u64)tgid << 32ULL) | (u32)start_time->time); /* Ubdevshm use time's low 32bit. */
}

static int _pma_ub_ubdevshm_register_segment(int tgid, struct task_start_time *start_time,
    u64 va, u64 size)
{
    struct mem_uva uva;
    int ret;

    pma_ub_ubdevshm_uva_pack(tgid, start_time, va, size, &uva);
    ret = ubdevshm_register_segment(&g_ubdevshm_handle, &uva);
    if (ret != 0) {
        svm_err("ubdevshm_register_segment failed. (ret=%d; va=0x%llx; size=%llu)\n", ret, va, size);
    }

    return ret;    
}

static int _pma_ub_ubdevshm_unregister_segment(int tgid, struct task_start_time *start_time,
    u64 va, u64 size)
{
    struct mem_uva uva;
    int ret;

    pma_ub_ubdevshm_uva_pack(tgid, start_time, va, size, &uva);
    ret = ubdevshm_unregister_segment(&g_ubdevshm_handle, &uva);
    if (ret != 0) {
        svm_err("ubdevshm_register_segment failed. (ret=%d)\n", ret);
    }

    return ret;  
}

int pma_ub_ubdevshm_register_segment(u32 udevid, int tgid, u64 va, u64 size)
{
    struct task_start_time start_time;
    int ret;

    ret = svm_get_task_start_time(udevid, tgid, &start_time);
    if (ret != 0) {
        svm_err("Get task start time failed. (ret=%d; udevid=%u; tgid=%d)\n", ret, udevid, tgid);
        return ret;
    }

    return _pma_ub_ubdevshm_register_segment(tgid, &start_time, va, size);
}

int pma_ub_ubdevshm_unregister_segment(u32 udevid, int tgid, u64 va, u64 size)
{
    struct task_start_time start_time;
    int ret;

    ret = svm_get_task_start_time(udevid, tgid, &start_time);
    if (ret != 0) {
        svm_err("Get task start time failed. (ret=%d; udevid=%u; tgid=%d)\n", ret, udevid, tgid);
        return ret;
    }

    return _pma_ub_ubdevshm_unregister_segment(tgid, &start_time, va, size);
}

