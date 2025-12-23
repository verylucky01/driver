/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_BUFF_ADP_H
#define DRV_BUFF_ADP_H

#include <stdlib.h>

#include "ascend_hal_error.h"
#include "drv_buff_common.h"

#ifndef QUEUE_UT
#if defined(__arm__) || defined(__aarch64__)
#define dsb(opt)    { asm volatile("dsb " #opt : : : "memory"); }
#else
#define dsb(opt)
#endif
#else
#define dsb(opt)
#endif
#define rmb()       dsb(ld) /* read fence */
#define wmb()       dsb(st) /* write fence */
#define mb()        dsb(sy) /* rw fence */
#define isb() asm volatile("isb" : : : "memory")

struct buff_config_handle_arg {
    void *data;
};

struct buff_get_info_handle_arg {
    uint32 in_size;
    uint32 out_size;
    void *in;
    void *out;
};

typedef drvError_t (*buff_config_adp_handle)(struct buff_config_handle_arg *arg);
typedef drvError_t (*buff_get_info_adp_handle)(struct buff_get_info_handle_arg *arg);

struct buff_config_para_adp {
    uint32 cmd_len;
    buff_config_adp_handle config_handle;
};

struct buff_get_info_adp {
    uint32 cmd_len;
    buff_get_info_adp_handle get_info_handle;
};
void buff_set_pid(pid_t pid);
int buff_api_getpid(void);
void buff_set_process_uni_id(uint64 node_id);
uint64 buff_get_process_uni_id(void);
int buff_get_current_devid(void);
unsigned long buff_make_devid_to_flags(int devid, unsigned long flags);
int buff_get_devid_from_flags(unsigned long flags);
unsigned int buff_get_all_devid_flag(unsigned long flags);
drvError_t buff_get_timeout_mbuf_info(struct buff_get_info_handle_arg *para_in);
drvError_t buff_timeout_mbuf_check(struct buff_config_handle_arg *para_in);
drvError_t memzone_cfg(memZoneCfg *mz_cfg);

#ifdef EMU_ST
#define THREAD __thread
int buff_get_current_pid(void);
#else
#include <sys/types.h>
#include <unistd.h>
#define THREAD
static inline int buff_get_current_pid(void)
{
    return getpid();
}
#endif

/* atomic set bit by offset, if set success return 0, else return 1 */
static inline int bitmap_set_rtn_atomic(unsigned long *bitmap, unsigned long offset)
{
    unsigned long next, val, old;
    unsigned long mask = (1UL << offset);

    val = *bitmap;
    do {
        old = val;
        next = (val | mask);
        val = __sync_val_compare_and_swap(bitmap, old, next);
    } while (val != old);

    return (int)((val & mask) >> offset);
}

/* atomic clear bit by offset, recycle until success */
static inline void bitmap_clear_bit_atomic(unsigned long *bitmap, unsigned long offset)
{
    unsigned long next, val, old;
    unsigned long mask = ~(1UL << offset);
    val = *bitmap;
    do {
        old = val;
        next = (val & mask);
        val = __sync_val_compare_and_swap(bitmap, old, next);
    } while (val != old);
}

static inline void buff_api_atomic_sub(uint32 *val, uint32 sub_val)
{
    __sync_fetch_and_sub(val, sub_val);
}

static inline void buff_api_atomic_dec(uint32 *val)
{
    buff_api_atomic_sub(val, 1);
}

static inline unsigned int buff_api_atomic_dec_and_return(uint32 *val)
{
    unsigned int ret;

    ret = __sync_sub_and_fetch(val, 1);

    return ret;
}

static inline void buff_api_atomic_add(uint32 *val, uint32 add_val)
{
    __sync_fetch_and_add(val, add_val);
}

static inline void buff_api_atomic_inc(uint32 *val)
{
    buff_api_atomic_add(val, 1);
}

static inline unsigned int buff_api_atomic_inc_and_return(uint32 *val)
{
    return __sync_fetch_and_add(val, 1);
}

static inline unsigned int buff_api_atomic_read(uint32 *val)
{
    unsigned int cur, next;

    cur = *val;
    do {
        next = cur;
        cur = __sync_val_compare_and_swap(val, next, next);
    } while (cur != next);

    return cur;
}

static inline uint64 buff_api_atomic_read_u64(uint64 *val)
{
    uint64 cur, next;

    cur = *val;
    do {
        next = cur;
        cur = __sync_val_compare_and_swap(val, next, next);
    } while (cur != next);

    return cur;
}

enum buff_delay_action {
    BUFF_MZ_ALLOC,
    BUFF_MZ_LARGE,
    BUFF_MP_ALLOC,
    BUFF_MBUF_FREE,
    BUFF_DELAY_ACTION_MAX
};

struct buff_delay_trace {
    unsigned int time_threshold;
    unsigned int cost_time;
    unsigned int start_time;
    unsigned int pid;
    unsigned int rsv[4];
};

#endif
