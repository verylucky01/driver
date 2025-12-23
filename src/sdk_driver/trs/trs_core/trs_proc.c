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

#include "ka_base_pub.h"
#include "ka_kernel_def_pub.h"
#include "ka_task_pub.h"
#include "ka_common_pub.h"
#include "ka_system_pub.h"
#include "ka_memory_pub.h"

#include "ascend_kernel_hal.h"
#include "devdrv_user_common.h"
#include "kernel_version_adapt.h"
#include "securec.h"
#include "pbl_uda.h"

#include "trs_proc_fs.h"
#include "trs_ts_inst.h"
#include "trs_proc.h"

static bool trs_is_id_pid_match(struct trs_res_ids *id, int pid, s64 task_id)
{
    return ((id->pid == pid) && (id->task_id == task_id) && (id->status == RES_STATUS_NORMAL));
}

static bool trs_is_res_pid_match(struct trs_core_ts_inst *ts_inst, int pid, s64 task_id, int res_type, u32 res_id)
{
    struct trs_res_mng *res_mng = &ts_inst->res_mng[res_type];
    bool ret = false;

    if (res_id >= res_mng->max_id) {
        trs_err("Invalid id. (devid=%u; tsid=%u; res_type=%d; id=%u; max_num=%u)\n",
            ts_inst->inst.devid, ts_inst->inst.tsid, res_type, res_id, res_mng->max_id);
        return false;
    }

    ret = trs_is_id_pid_match(&res_mng->ids[res_id], pid, task_id);
    if (!ret) {
        /*
         * Do not change log level.
         * Shr id will check ipc notify is opend by this proc.
         */
        trs_debug("Match failed. (devid=%u; tsid=%u; res_type=%d; res_id=%u; id_pid=%d; pid=%u; "
            "id_task_id=%lld; task_id=%lld; id_status=%u)\n", ts_inst->inst.devid, ts_inst->inst.tsid,
            res_type, res_id, res_mng->ids[res_id].pid, pid, res_mng->ids[res_id].task_id, task_id,
            res_mng->ids[res_id].status);
    }
    return ret;
}

bool trs_proc_has_res(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id)
{
    return trs_is_res_pid_match(ts_inst, proc_ctx->pid, proc_ctx->task_id, res_type, res_id);
}

static int trs_get_proc_task_id(struct trs_core_ts_inst *ts_inst, int pid, s64 *task_id)
{
    struct trs_core_ts_inst *tmp_ts_inst = ts_inst;
    struct trs_proc_ctx *proc_ctx = NULL;

    if (ts_inst->inst.tsid != 0) {
        tmp_ts_inst = trs_core_inst_get(ts_inst->inst.devid, 0); /* proc info stored in tsid 0 inst */
        if (tmp_ts_inst == NULL) {
            return -EINVAL;
        }
    }

    ka_task_down_read(&ts_inst->sem);
    proc_ctx = trs_proc_ctx_find(tmp_ts_inst, pid);
    if (proc_ctx != NULL) {
        *task_id = proc_ctx->task_id;
    }
    ka_task_up_read(&ts_inst->sem);

    if (tmp_ts_inst != ts_inst) {
        trs_core_inst_put(tmp_ts_inst);
    }

    return (proc_ctx == NULL) ? -EINVAL : 0;
}

int trs_core_get_ssid(struct trs_id_inst *inst, int pid, u32 *passid)
{
    struct trs_core_ts_inst *ts_inst = NULL;
    struct trs_proc_ctx *proc_ctx = NULL;

    ts_inst = trs_core_inst_get(inst->devid, inst->tsid);
    if (ts_inst == NULL) {
        return -EINVAL;
    }

    ka_task_down_read(&ts_inst->sem);
    proc_ctx = trs_proc_ctx_find(ts_inst, pid);
    if (proc_ctx == NULL) {
        ka_task_up_read(&ts_inst->sem);
        trs_core_inst_put(ts_inst);
        return -ENOENT;
    }
    *passid = proc_ctx->cp_ssid;
    ka_task_up_read(&ts_inst->sem);
    trs_core_inst_put(ts_inst);

    return 0;
}
KA_EXPORT_SYMBOL_GPL(trs_core_get_ssid);

bool trs_proc_has_res_with_pid(struct trs_core_ts_inst *ts_inst, int pid, int res_type, u32 res_id)
{
    s64 task_id;
    int ret;

    ret = trs_get_proc_task_id(ts_inst, pid, &task_id);
    if (ret != 0) {
        /*
         * Do not change log level.
         * Shr id will check ipc notify is opend by this proc.
         */
        trs_debug("Invalid value. (pid=%d)\n", pid);
        return false;
    }

    return trs_is_res_pid_match(ts_inst, pid, task_id, res_type, res_id);
}

int trs_res_get(struct trs_core_ts_inst *ts_inst, int pid, int res_type, u32 res_id)
{
    struct trs_res_mng *res_mng = &ts_inst->res_mng[res_type];
    s64 task_id;
    int ret;

    ret = trs_get_proc_task_id(ts_inst, pid, &task_id);
    if (ret != 0) {
        trs_err("Invalid value. (pid=%d)\n", pid);
        return ret;
    }

    ka_task_mutex_lock(&res_mng->mutex);
    if (!trs_is_res_pid_match(ts_inst, pid, task_id, res_type, res_id)) {
        ka_task_mutex_unlock(&res_mng->mutex);
        return -EINVAL;
    }

    res_mng->ids[res_id].ref++;
    ka_task_mutex_unlock(&res_mng->mutex);

    return 0;
}

int trs_res_put(struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id)
{
    struct trs_res_mng *res_mng = &ts_inst->res_mng[res_type];
    int ref;

    if (res_id >= res_mng->max_id) {
        return -EINVAL;
    }

    ka_task_mutex_lock(&res_mng->mutex);
    if (res_mng->ids[res_id].ref == 0) {
        trs_err("Res not valid. (devid=%u; tsid=%u; res_type=%d; id=%u)\n",
            ts_inst->inst.devid, ts_inst->inst.tsid, res_type, res_id);
        ka_task_mutex_unlock(&res_mng->mutex);
        return -EINVAL;
    }

    res_mng->ids[res_id].ref--;
    if (res_mng->ids[res_id].ref <= 0) {
        res_mng->ids[res_id].pid = 0;
        res_mng->use_num--;
    }
    ref = res_mng->ids[res_id].ref;
    ka_task_mutex_unlock(&res_mng->mutex);

    return ref;
}

static int _trs_proc_add_res(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id,
    bool is_agent_res)
{
    struct trs_res_mng *res_mng = &ts_inst->res_mng[res_type];
    struct trs_id_inst *inst = &ts_inst->inst;

    if (res_id >= res_mng->max_id) {
        trs_err("Invalid id. (devid=%u; tsid=%u; res_type=%d; id=%u; max_num=%u)\n",
            inst->devid, inst->tsid, res_type, res_id, res_mng->max_id);
        return -EINVAL;
    }

    ka_task_mutex_lock(&res_mng->mutex);
    if (res_mng->ids[res_id].pid != 0) {
#ifndef EMU_ST /* Don't delete for UB UT */
        trs_err("Repeat add res. (devid=%u; tsid=%u; res_type=%d; id=%u; owner_pid=%u)\n",
            inst->devid, inst->tsid, res_type, res_id, res_mng->ids[res_id].pid);
        ka_task_mutex_unlock(&res_mng->mutex);
        return -EINVAL;
#endif
    }

    res_mng->ids[res_id].task_id = proc_ctx->task_id;
    res_mng->ids[res_id].pid = proc_ctx->pid;
    res_mng->ids[res_id].ref = 1;
    res_mng->ids[res_id].status = RES_STATUS_NORMAL;
    res_mng->ids[res_id].is_agent_res = is_agent_res;
    res_mng->use_num++;
    ka_task_mutex_unlock(&res_mng->mutex);

    trs_debug("Add res. (devid=%u; tsid=%u; res_type=%d; id=%u; owner_pid=%u)\n",
        inst->devid, inst->tsid, res_type, res_id, res_mng->ids[res_id].pid);

    ka_base_atomic_inc(&proc_ctx->ts_ctx[inst->tsid].current_id_num[res_type]);
    if (is_agent_res) {
        ka_base_atomic_inc(&proc_ctx->ts_ctx[inst->tsid].agent_id_num[res_type]);
    }

    return 0;
}

int trs_proc_add_res(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id)
{
    return _trs_proc_add_res(proc_ctx, ts_inst, res_type, res_id, false);
}

int trs_proc_add_res_ex(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id,
    bool is_agent_res)
{
    return _trs_proc_add_res(proc_ctx, ts_inst, res_type, res_id, is_agent_res);
}

bool trs_is_proc_res_limited(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type)
{
    struct trs_res_mng *res_mng = &ts_inst->res_mng[res_type];
    struct trs_id_inst *inst = &ts_inst->inst;
    bool flag = false;

    ka_task_mutex_lock(&res_mng->mutex);
    if ((res_mng->id_num - res_mng->use_num) < ts_inst->support_proc_num) {
        if (trs_get_proc_res_num(proc_ctx, inst->tsid, res_type) > 0) {
            trs_warn("Proc res limited. (devid=%u; tsid=%u; res_type=%d; id_num=%u; use_num=%u; support_proc_num=%u)\n",
                inst->devid, inst->tsid, res_type, res_mng->id_num, res_mng->use_num, ts_inst->support_proc_num);
            flag = true;
        }
    }
    ka_task_mutex_unlock(&res_mng->mutex);

    return flag;
}

int trs_proc_del_res(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id)
{
    struct trs_res_mng *res_mng = &ts_inst->res_mng[res_type];
    struct trs_id_inst *inst = &ts_inst->inst;
    bool is_agent_res = false;
    int ret;

    ka_task_mutex_lock(&res_mng->mutex);
    if (!trs_proc_has_res(proc_ctx, ts_inst, res_type, res_id)) {
        ka_task_mutex_unlock(&res_mng->mutex);
        trs_err("Not proc res. (devid=%u; tsid=%u; res_type=%d; id=%d)\n", inst->devid, inst->tsid, res_type, res_id);
        return -EINVAL;
    }

    ret = trs_res_id_pre_del(proc_ctx, ts_inst, res_type, res_id);
    if (ret != 0) {
        ka_task_mutex_unlock(&res_mng->mutex);
        trs_err("Failed to check. (ret=%d; devid=%u; tsid=%u; res_type=%d; id=%d)\n",
            ret, inst->devid, inst->tsid, res_type, res_id);
        return ret;
    }

    res_mng->ids[res_id].status = RES_STATUS_DEL;
    is_agent_res = res_mng->ids[res_id].is_agent_res;
    ka_task_mutex_unlock(&res_mng->mutex);

    trs_debug("Del res. (devid=%u; tsid=%u; res_type=%d; id=%u\n", inst->devid, inst->tsid, res_type, res_id);

    ka_base_atomic_dec(&proc_ctx->ts_ctx[inst->tsid].current_id_num[res_type]);
    if (is_agent_res) {
        ka_base_atomic_dec(&proc_ctx->ts_ctx[inst->tsid].agent_id_num[res_type]);
    }

    return trs_res_put(ts_inst, res_type, res_id);
}

struct trs_proc_ctx *trs_proc_ctx_find(struct trs_core_ts_inst *ts_inst, int pid)
{
    struct trs_proc_ctx *proc_ctx = NULL;

    ka_list_for_each_entry(proc_ctx, &ts_inst->proc_list_head, node) {
        if (proc_ctx->pid == pid) {
            return proc_ctx;
        }
    }

    return NULL;
}

#ifdef CFG_SOC_PLATFORM_FPGA
#define TRS_MAX_WAIT_PROC_EXIT_CNT 1800000U // 1800s
#else
#define TRS_MAX_WAIT_PROC_EXIT_CNT 3U
#endif
int trs_proc_wait_for_exit(struct trs_core_ts_inst *ts_inst, int pid)
{
    int loop = 0;

    do {
        struct trs_proc_ctx *proc_ctx = NULL;

        ka_task_down_read(&ts_inst->sem);
        proc_ctx = trs_proc_ctx_find(ts_inst, pid);
        ka_task_up_read(&ts_inst->sem);
        if (proc_ctx == NULL) {
            return 0;
        }
        ka_system_usleep_range(USEC_PER_MSEC - 100, USEC_PER_MSEC); /* 1ms, range 100 us */
    } while (++loop < TRS_MAX_WAIT_PROC_EXIT_CNT);
    return -EBUSY;
}

static struct trs_proc_ctx *trs_proc_ctx_get(struct trs_core_ts_inst *ts_inst, int pid)
{
    struct trs_proc_ctx *proc_ctx = NULL;

#ifndef EMU_ST
    ka_list_for_each_entry(proc_ctx, &ts_inst->proc_list_head, node) {
        ka_task_read_lock_bh(&proc_ctx->ctx_rwlock);
        if (proc_ctx->pid == pid && proc_ctx->status == TRS_PROC_STATUS_NORMAL) {
            kref_safe_get(&proc_ctx->ref);
            ka_task_read_unlock_bh(&proc_ctx->ctx_rwlock);
            return proc_ctx;
        }
        ka_task_read_unlock_bh(&proc_ctx->ctx_rwlock);
    }
#endif

    return NULL;
}

void trs_proc_ctx_destroy(struct kref_safe *kref);
void trs_proc_ctx_put(struct trs_proc_ctx *proc_ctx)
{
    kref_safe_put(&proc_ctx->ref, trs_proc_ctx_destroy);
}

struct trs_proc_ctx *trs_get_share_ctx(struct trs_core_ts_inst *ts_inst, u32 share_pid)
{
    struct trs_proc_ctx *proc_ctx = NULL;
#ifndef EMU_ST
    ka_task_down_write(&ts_inst->sem);
    proc_ctx = trs_proc_ctx_get(ts_inst, share_pid);
    if (proc_ctx != NULL) {
        ka_task_up_write(&ts_inst->sem);
        return proc_ctx;
    }
    ka_task_up_write(&ts_inst->sem);

    trs_err("Invalid share ctx. (devid=%u; share_pid=%u)\n", ts_inst->inst.devid, share_pid);
#endif

    return NULL;
}

struct trs_proc_ctx *trs_proc_ctx_create(struct trs_core_ts_inst *all_ts_inst[], u32 ts_num)
{
    struct trs_core_ts_inst *ts_inst = all_ts_inst[0];
    struct trs_proc_ctx *proc_ctx = NULL;
    int tsid;

    proc_ctx = trs_kzalloc(sizeof(struct trs_proc_ctx), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (proc_ctx == NULL) {
        trs_err("Mem alloc failed. (size=%lx)\n", sizeof(struct trs_proc_ctx));
        return NULL;
    }

    for (tsid = 0; tsid < ts_num; tsid++) {
        ka_task_mutex_init(&proc_ctx->ts_ctx[tsid].mutex);
        ka_task_mutex_init(&proc_ctx->ts_ctx[tsid].shm_ctx.mutex);
        proc_ctx->ts_ctx[tsid].shm_ctx.chan_id = -1;
        proc_ctx->ts_ctx[tsid].ts_inst = all_ts_inst[tsid]; /* only ts0 is valid */
    }

    (void)task_get_start_time_by_tgid(ka_task_get_current_tgid(), &proc_ctx->start_time);
    proc_ctx->force_recycle = false;
    proc_ctx->devid = ts_inst->inst.devid;
    proc_ctx->task_id = ka_base_atomic64_inc_return(&ts_inst->cur_task_id);
    proc_ctx->pid = ka_task_get_current_tgid();
    proc_ctx->release_type = TRS_PROC_RELEASE_LOCAL_REMOTE;
    if (strncpy_s(proc_ctx->name, TASK_COMM_LEN, ka_task_get_current_comm(), strlen(ka_task_get_current_comm())) != 0) {
#ifndef EMU_ST
        trs_warn("Strcpy warn. (pid=%d)\n", proc_ctx->pid);
#endif
    }
    proc_ctx->name[TASK_COMM_LEN - 1] = '\0';
    proc_ctx->mm = NULL;

    kref_safe_init(&proc_ctx->ref);
    ka_task_rwlock_init(&proc_ctx->ctx_rwlock);

    proc_fs_add_pid(proc_ctx);

    return proc_ctx;
}

void trs_proc_ctx_destroy(struct kref_safe *kref)
{
    struct trs_proc_ctx *proc_ctx = NULL;
    u32 tsid;

    proc_ctx = ka_container_of(kref, struct trs_proc_ctx, ref);

    proc_fs_del_pid(proc_ctx);
    for (tsid = 0; tsid < TRS_TS_MAX_NUM; tsid++) {
        ka_task_mutex_destroy(&proc_ctx->ts_ctx[tsid].mutex);
        ka_task_mutex_destroy(&proc_ctx->ts_ctx[tsid].shm_ctx.mutex);
        proc_ctx->ts_ctx[tsid].ts_inst = NULL;
    }
    trs_kfree(proc_ctx);
}

static int trs_vma_flag_check(ka_vm_area_struct_t *vma, int type)
{
    if ((vma->vm_flags & VM_HUGETLB) != 0) {
        trs_err("Invalid vm_flags. (vm_flags=0x%lx)\n", vma->vm_flags);
        return -EINVAL;
    }

    if ((type == TRS_MAP_TYPE_RO_DEV_MEM) || (type == TRS_MAP_TYPE_RO_REG)) {
        if ((vma->vm_flags & VM_WRITE) != 0) {
            trs_err("Invalid vm_flags. (vm_flags=0x%lx)\n", vma->vm_flags);
            return -EINVAL;
        }
    }

    return 0;
}

static int trs_check_va_range(ka_vm_area_struct_t *vma, unsigned long addr, unsigned long size)
{
    unsigned long end = addr + KA_MM_PAGE_ALIGN(size);
    if (((addr & (KA_MM_PAGE_SIZE - 1)) != 0) || (addr < vma->vm_start) || (addr > vma->vm_end) ||
        (end > vma->vm_end) || (addr >= end)) {
#ifndef EMU_ST
        trs_err("Invalid para. (addr=%pK; size=0x%lx; end=%pK; vm_start=%pK; vm_end=%pK)\n", (void *)(uintptr_t)addr,
            size, (void *)(uintptr_t)end, (void *)(uintptr_t)vma->vm_start, (void *)(uintptr_t)vma->vm_end);
#endif
        return -EINVAL;
    }

    return 0;
}

static int trs_check_va_map(ka_vm_area_struct_t *vma, unsigned long addr, unsigned long size)
{
    unsigned long end = addr + KA_MM_PAGE_ALIGN(size);
    unsigned long va_check, pfn;

    for (va_check = addr; va_check < end; va_check += KA_MM_PAGE_SIZE) {
        if (ka_mm_follow_pfn(vma, va_check, &pfn) == 0) {
            trs_err("Check va is map. (addr=0x%pK; size=0x%lx; va_check=0x%pK)\n",
                (void *)(uintptr_t)addr, size, (void *)(uintptr_t)va_check);
            return -EINVAL;
        }
    }

    return 0;
}

static int trs_check_va_unmap(ka_vm_area_struct_t *vma, unsigned long addr, unsigned long size)
{
    unsigned long end = addr + KA_MM_PAGE_ALIGN(size);
    unsigned long va_check, pfn;

    for (va_check = addr; va_check < end; va_check += KA_MM_PAGE_SIZE) {
        if (ka_mm_follow_pfn(vma, va_check, &pfn) != 0) {
            trs_err("Check va is unmap. (addr=0x%pK; size=0x%lx; va_check=0x%pK)\n",
                (void *)(uintptr_t)addr, size, (void *)(uintptr_t)va_check);
            return -EINVAL;
        }
    }

    return 0;
}

static int trs_get_remap_prot(int type, pgprot_t vm_page_prot, pgprot_t *prot)
{
    if ((type == TRS_MAP_TYPE_REG) || (type == TRS_MAP_TYPE_RO_REG)) {
        *prot = pgprot_device(vm_page_prot);
    } else if (type == TRS_MAP_TYPE_MEM) {
        *prot = vm_page_prot;
    } else { /* TRS_MAP_TYPE_DEV_MEM TRS_MAP_TYPE_DEV_RD_MEM */
#ifdef CONFIG_ARM64
        *prot = pgprot_writecombine(vm_page_prot);
#else
        *prot = vm_page_prot;
#endif
    }

    return 0;
}

static void trs_zap_vma_ptes(ka_vm_area_struct_t *vma, unsigned long addr, size_t len)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0)
    int ret;
    ret = ka_mm_zap_vma_ptes(vma, addr, KA_MM_PAGE_ALIGN(len));
    if (ret != 0) {
#ifndef EMU_ST
        trs_err("Unmap va failed. (vma_start=%pK; end=%pK; addr=%pK; len=0x%lx)\n",
            (void *)(uintptr_t)vma->vm_start, (void *)(uintptr_t)vma->vm_end, (void *)(uintptr_t)addr, len);
#endif
    }
#else
    ka_mm_zap_vma_ptes(vma, addr, KA_MM_PAGE_ALIGN(len));
#endif
}

static int trs_remap_pfn(struct trs_proc_ctx *proc_ctx, struct trs_mem_map_para *para,
    ka_vm_area_struct_t *vma)
{
    pgprot_t prot;
    int ret;

    ret = trs_get_remap_prot(para->type, vma->vm_page_prot, &prot);
    if (ret != 0) {
        return ret;
    }

    ret = ka_mm_remap_pfn_range(vma, para->va, __phys_to_pfn(para->pa), para->len, prot);
    if (ret != 0) {
#ifndef EMU_ST
        trs_err("Remap va failed. (vma_start=%pK; end=%pK; addr=%pK; len=0x%lx)\n",
            (void *)(uintptr_t)vma->vm_start, (void *)(uintptr_t)vma->vm_end, (void *)(uintptr_t)para->va, para->len);
#endif
    }

    return ret;
}

int trs_remap_sq_mem(struct trs_proc_ctx *proc_ctx, ka_vm_area_struct_t *vma,
    struct trs_mem_map_para *para)
{
    int ret;

    if (vma->vm_private_data != proc_ctx) {
        trs_err("Invalid vm private data. (pid=%d)\n", proc_ctx->pid);
        return -EINVAL;
    }

    ret = trs_vma_flag_check(vma, para->type);
    if (ret != 0) {
        return ret;
    }

    ret = trs_check_va_range(vma, para->va, para->len);
    if (ret != 0) {
        return ret;
    }

    ret = trs_check_va_map(vma, para->va, para->len);
    if (ret != 0) {
        return ret;
    }

    return trs_remap_pfn(proc_ctx, para, vma);
}

int trs_remap_sq(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, struct trs_mem_map_para *para)
{
    ka_vm_area_struct_t *vma = NULL;
    int ret;

    ka_task_down_write(get_mmap_sem(ka_task_get_current_mm()));

    vma = ka_mm_find_vma(ka_task_get_current_mm(), para->va);
    if (vma == NULL) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        trs_err("Find vma failed. (va=%pK)\n", (void *)(uintptr_t)para->va);
        return -EINVAL;
    }

    ret = trs_remap_sq_mem(proc_ctx, vma, para);
    ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));

    return ret;
}

int trs_unmap_sq_mem(struct trs_proc_ctx *proc_ctx, ka_vm_area_struct_t *vma,
    struct trs_mem_unmap_para *para)
{
    int ret;

    if (vma->vm_private_data != proc_ctx) {
        trs_err("Invalid vm private data. (pid=%d)\n", proc_ctx->pid);
        return -EINVAL;
    }

    ret = trs_check_va_range(vma, para->va, para->len);
    if (ret != 0) {
        return ret;
    }

    ret = trs_check_va_unmap(vma, para->va, para->len);
    if (ret != 0) {
        return ret;
    }

    trs_zap_vma_ptes(vma, para->va, para->len);

    return 0;
}

int trs_unmap_sq(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, struct trs_mem_unmap_para *para)
{
    ka_vm_area_struct_t *vma = NULL;
    int ret;

    ka_task_down_write(get_mmap_sem(ka_task_get_current_mm()));

    vma = ka_mm_find_vma(ka_task_get_current_mm(), para->va);
    if (vma == NULL) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        trs_err("Find vma failed. (va=0x%pK)\n", (void *)(uintptr_t)para->va);
        return -EINVAL;
    }

    ret = trs_unmap_sq_mem(proc_ctx, vma, para);
    ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));

    return ret;
}

static int trs_proc_release_msg_send(struct trs_core_ts_inst *ts_inst, int pid, u16 cmd_type)
{
    struct recycle_proc_msg msg;

    trs_mbox_init_header(&msg.header, cmd_type);
    msg.proc_info.app_cnt = 1;
    msg.proc_info.pid[0] = pid;

    /* adapt fill: plat_type, fid */
    return trs_core_notice_ts(ts_inst, (u8 *)&msg, sizeof(msg));
}

#define TRS_TS_LONG_RETRY_TIMES     160
#ifdef CFG_SOC_PLATFORM_FPGA
#define TRS_TS_RETRY_TIMES          1800
#else
#define TRS_TS_RETRY_TIMES          60
#endif

static int trs_proc_get_check_ts_times(u32 devid)
{
    int retry_times = TRS_TS_RETRY_TIMES;
#ifndef CFG_FEATURE_NOTSUPPORT_BOARD_CONFIG
    int ret;
    u32 soc_type;

    ret = hal_kernel_get_soc_type(devid, &soc_type);
    if (ret != 0) {
        return retry_times;
    }

    if ((soc_type == CHIP_CLOUD_V2) || (soc_type == CHIP_CLOUD_V3)) {
        retry_times = TRS_TS_LONG_RETRY_TIMES;
    }
#endif

    return retry_times;
}

int trs_proc_release_check_ts(struct trs_proc_ctx *proc_ctx, u32 tsid)
{
    int i, ret;
    int retry_times = trs_proc_get_check_ts_times(proc_ctx->devid);

    for (i = 0; i < retry_times; i++) {
        struct trs_core_ts_inst *ts_inst = trs_core_inst_get(proc_ctx->devid, tsid);
        if (ts_inst == NULL) {
            return 0;
        }

        trs_debug("Send check. (devid=%u; tsid=%u; name=%s; pid=%d; task_id=%lld)\n",
            ts_inst->inst.devid, tsid, proc_ctx->name, proc_ctx->pid, proc_ctx->task_id);
        ret = trs_proc_release_msg_send(ts_inst, proc_ctx->pid, TRS_MBOX_RECYCLE_CHECK);
        if (ret == 0) {
            trs_core_inst_put(ts_inst);
            return 0;
        }

        trs_core_inst_put(ts_inst);

        if ((ret == -EIO) || (TRS_IS_REBOOT_ACTIVE == true)) {
#ifndef EMU_ST
            return ret;
#endif
        }

        (i < 10) ? msleep(100) : msleep(1000); /* fisrt 10 times, sleep 100 ms, then sleep 1000 ms */
    }

    return -ETIMEDOUT;
}

void trs_proc_release_ras_report(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst)
{
    if (ts_inst->ops.ras_report != NULL) {
        ts_inst->ops.ras_report(&ts_inst->inst);
        return;
    }
    trs_debug("No need to send ras. (devid=%u; tsid=%u; pid=%d; task_id=%lld)\n",
        ts_inst->inst.devid, ts_inst->inst.tsid, proc_ctx->pid, proc_ctx->task_id);
    return;
}

bool trs_proc_is_res_leak(struct trs_proc_ctx *proc_ctx, u32 tsid)
{
    int i;

    if (proc_ctx->ts_ctx[tsid].shm_ctx.chan_id >= 0) {
        return true;
    }

    for (i = 0; i < TRS_CORE_MAX_ID_TYPE; i++) {
        if (trs_get_proc_res_num(proc_ctx, tsid, i) > 0) {
            return true;
        }
    }

    return false;
}

#define TSFW_QUEUE_FULL_RETRY_TIMES         200
#define TSFW_QUEUE_FULL_WAIT_INTERVAL_MIN   400
#define TSFW_QUEUE_FULL_WAIT_INTERVAL_MAX   500
void trs_proc_release_notice_ts(struct trs_core_ts_inst *ts_inst, struct trs_proc_ctx *proc_ctx)
{
    struct trs_id_inst *inst = &ts_inst->inst;
    bool is_open_need_limit = false;
    int queue_full_retry_cnt = 0;
    int ret;

retry:
    queue_full_retry_cnt++;
    ret = trs_proc_release_msg_send(ts_inst, proc_ctx->pid, TRS_MBOX_RECYCLE_PID);
    if (ret != 0) {
        trs_warn("Notice ts warn. (devid=%u; tsid=%u; retry_cnt=%d; ret=%d)\n",
            inst->devid, inst->tsid, queue_full_retry_cnt, ret);
    }

    if ((ret == -EBUSY) && (queue_full_retry_cnt < TSFW_QUEUE_FULL_RETRY_TIMES)) {
        if (is_open_need_limit == false) {
            ka_task_down_write(&ts_inst->ctrl_sem); /* limit trs open when notice ts busy */
            is_open_need_limit = true;
        }

        ka_system_usleep_range(TSFW_QUEUE_FULL_WAIT_INTERVAL_MIN, TSFW_QUEUE_FULL_WAIT_INTERVAL_MAX);
        goto retry;
    }

    if (is_open_need_limit == true) {
        ka_task_up_write(&ts_inst->ctrl_sem);
    }
}

static void trs_for_each_proc_res_id(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type,
    void (*recycle_func)(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id))
{
    struct trs_res_mng *res_mng = &ts_inst->res_mng[res_type];
    struct trs_id_inst *inst = &ts_inst->inst;
    u32 recycle_num = 0;
    u32 i;

    if (trs_get_proc_res_num(proc_ctx, inst->tsid, res_type) == 0) {
        return;
    }

    for (i = 0; i < res_mng->max_id; i++) {
        if (trs_is_id_pid_match(&res_mng->ids[i], proc_ctx->pid, proc_ctx->task_id)) {
            recycle_num++;
            recycle_func(proc_ctx, ts_inst, res_type, i);
            if (trs_get_proc_res_num(proc_ctx, ts_inst->inst.tsid, res_type) == 0) {
                break;
            }
        }
    }

    trs_debug("Recycle res. (devid=%u; tsid=%u; res_type=%d; recycle_num=%u; proc_res_num=%u)\n",
        inst->devid, inst->tsid, res_type, recycle_num, trs_get_proc_res_num(proc_ctx, inst->tsid, res_type));
}

void trs_proc_leak_res_show(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst)
{
    struct trs_id_inst *inst = &ts_inst->inst;
    struct trs_res_mng *res_mng = NULL;
    int i, j, num;

    for (i = 0; i < TRS_CORE_MAX_ID_TYPE; i++) {
        res_mng = &ts_inst->res_mng[i];
        num = 0;
        for (j = 0; (u32)j < res_mng->max_id; j++) {
            if (num == trs_get_proc_res_num(proc_ctx, inst->tsid, i)) {
                break;
            }
            if (trs_is_id_pid_match(&res_mng->ids[i], proc_ctx->pid, proc_ctx->task_id)) {
                num++;
                trs_warn("Leak res. (devid=%u; tsid=%u; res_type=%d; res_id=%d; proc_res_num=%d)\n",
                    inst->devid, inst->tsid, i, j, trs_get_proc_res_num(proc_ctx, inst->tsid, i));
            }
        }
    }
}

static int trs_proc_release_agent_master_check_ts(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst)
{
    int master_pid;
    int ret;

    ret = trs_core_get_host_pid(proc_ctx->pid, &master_pid);
    if (ret != 0) {
        return 0;
    }

    ret = trs_proc_release_msg_send(ts_inst, master_pid, TRS_MBOX_RECYCLE_CHECK);
    if (ret != 0) {
#ifndef EMU_ST
        trs_warn("Recycle warn. (devid=%u; tsid=%u; name=%s; pid=%d; task_id=%lld; master_pid=%d; ret=%d)\n",
            ts_inst->inst.devid, ts_inst->inst.tsid, proc_ctx->name, proc_ctx->pid, proc_ctx->task_id, master_pid, ret);
        trs_proc_leak_res_show(proc_ctx, ts_inst);
#endif
        return -EBUSY;
    }

    return 0;
}

static int trs_proc_ts_res_recycle(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst)
{
    struct trs_id_inst *inst = &ts_inst->inst;
    int i, ret;

    if (!proc_ctx->force_recycle) {
        ret = trs_proc_release_check_ts(proc_ctx, inst->tsid);
        if (ret != 0) {
            trs_warn("Recycle warn. (devid=%u; tsid=%u; name=%s; pid=%d; task_id=%lld; ret=%d)\n",
                inst->devid, inst->tsid, proc_ctx->name, proc_ctx->pid, proc_ctx->task_id, ret);
            trs_proc_leak_res_show(proc_ctx, ts_inst);
            return -EBUSY;
        }

        /* check host app recycle success if proc has host-app-agent resource */
        if ((ts_inst->location == UDA_LOCAL) && (trs_proc_has_agent_res(proc_ctx, inst->tsid))) {
            ret = trs_proc_release_agent_master_check_ts(proc_ctx, ts_inst);
            if (ret != 0) {
                return ret;
            }
        }
    }

    if (proc_ctx->ts_ctx[inst->tsid].shm_ctx.chan_id >= 0) {
        trs_shm_sqcq_recycle(proc_ctx, ts_inst);
    }

    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_HW_SQ, trs_hw_sqcq_recycle);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_HW_CQ, trs_hw_sqcq_recycle);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_SW_SQ, trs_sw_sqcq_recycle);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_SW_CQ, trs_sw_sqcq_recycle);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_MAINT_SQ, trs_maint_sqcq_recycle);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_MAINT_CQ, trs_maint_sqcq_recycle);

    if (!proc_ctx->force_recycle) {
        for (i = 0; i < TRS_HW_SQ; i++) {
            trs_for_each_proc_res_id(proc_ctx, ts_inst, i, trs_res_id_recycle);
        }
    }

    trs_debug("Recycle success. (devid=%u; tsid=%u)\n", inst->devid, inst->tsid);

    return 0;
}

void trs_clear_exit_proc_list(struct trs_core_ts_inst *ts_inst)
{
    struct trs_proc_ctx *proc_ctx = NULL;
    struct trs_proc_ctx *n = NULL;

    ka_task_down_write(&ts_inst->sem);
    ka_list_for_each_entry_safe(proc_ctx, n, &ts_inst->exit_proc_list_head, node) {
#ifndef EMU_ST
        proc_ctx->force_recycle = true;
        (void)trs_proc_ts_res_recycle(proc_ctx, ts_inst);
        ka_list_del(&proc_ctx->node);
        trs_proc_ctx_put(proc_ctx);
#endif
    }
    ka_task_up_write(&ts_inst->sem);
}

void trs_proc_release(struct trs_core_ts_inst *ts_inst, struct trs_proc_ctx *proc_ctx)
{
    proc_ctx->status = TRS_PROC_STATUS_EXIT;

    if (ts_inst->featur_mode == TRS_INST_PART_FEATUR_MODE) {
        return;
    }

    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_HW_SQ, trs_proc_diable_sq_status);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_LOGIC_CQ, trs_logic_cq_recycle);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_CB_CQ, trs_cb_sqcq_recycle);
    trs_for_each_proc_res_id(proc_ctx, ts_inst, TRS_CB_SQ, trs_cb_sqcq_recycle);
}

int trs_proc_recycle(struct trs_core_ts_inst *ts_inst, struct trs_proc_ctx *proc_ctx)
{
    int ret = 0;

    trs_debug("Begin recycle. (pid=%d)\n", proc_ctx->pid);

    if (trs_proc_is_res_leak(proc_ctx, ts_inst->inst.tsid)) {
        ret = trs_proc_ts_res_recycle(proc_ctx, ts_inst);
    }

    return ret;
}

