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

#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/notifier.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
#include <linux/panic_notifier.h>
#endif

#include "devdrv_common.h"
#include "dms_define.h"
#include "dms_timer.h"
#include "soft_fault_define.h"
#include "heart_beat.h"

#ifdef CFG_ENV_FPGA
#define HEART_BEAT_TIMER_EXPIRE_SEC (1 * 50)
#define HB_FORGET_WHITE_JUDGE_TIME (12 * 50)
#else
#define HEART_BEAT_TIMER_EXPIRE_SEC 1
#define HB_FORGET_WHITE_JUDGE_TIME 12
#endif

#define SHM_NOT_INIT 1
struct hb_write_block {
    u64 count;
    u64 last_normal_time;
    u64 last_write_time;
    u64 miss_write_count;
    u32 device_status;
};

struct hb_write_timer {
    struct hrtimer timer;
    struct work_struct work;
    struct timespec64 last_write_time;
    struct timespec64 last_normal_time;
    unsigned long long forget_count; /* number of times heartbeat was not written for more than 12 seconds */
    int write_ret;
};

static struct hb_write_block g_hb_write_block[DEVICE_NUM_MAX] = {{0}};
#if (defined CFG_FEATURE_HEARTBEAT_CNT_PCIE) || (!defined CFG_HOST_ENV)
static struct hb_write_timer g_hb_write_timer = {{{{0}}}, {{0}}};
#endif

struct hb_write_block* hb_get_write_item(unsigned int dev_id)
{
    return &g_hb_write_block[dev_id];
}

#ifndef DMS_UT
#if (defined CFG_FEATURE_HEARTBEAT_CNT_PCIE) || (!defined CFG_HOST_ENV)
int hb_update_heartbeat_count(void)
{
    int ret;
    unsigned int dev_id;

    for (dev_id = 0; dev_id < DEVICE_NUM_MAX; dev_id++) {
        if (g_hb_write_block[dev_id].device_status != HEART_BEAT_READY) {
            continue;
        }
        g_hb_write_block[dev_id].count++;

        ret = hb_set_heart_beat_count(dev_id, g_hb_write_block[dev_id].count);
        if (ret != 0) {
            soft_drv_err("Device heartbeat set failed. (count=%llu; ret=%d; device=%u)\n",
                            g_hb_write_block[dev_id].count, ret, dev_id);
            return ret;
        }
    }

    return 0;
}

static bool hb_write_work_abnormal_check(struct hb_write_timer *timer_info, struct timespec64 current_time)
{
    unsigned long interval;

    interval = current_time.tv_sec - timer_info->last_write_time.tv_sec;
    if (timer_info->last_write_time.tv_sec != 0 && interval > HB_FORGET_WHITE_JUDGE_TIME) {
        timer_info->forget_count++;
        return true;
    }

    return false;
}

static enum hrtimer_restart heart_beat_write_count(struct hrtimer *htr)
{
    int ret;
    bool abnormal = false;
    struct hb_write_timer *info = NULL;
    struct timespec64 current_time;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 6, 0)
    struct timespec tmp_current_time;
#endif

    if (htr == NULL) {
        hrtimer_forward_now(htr, ktime_set(HEART_BEAT_TIMER_EXPIRE_SEC, 0));
        return HRTIMER_RESTART;
    }

    info = container_of(htr, struct hb_write_timer, timer);
    ret = hb_update_heartbeat_count();
    if (ret != 0) {
        if (ret == SHM_NOT_INIT) {
            hrtimer_forward_now(htr, ktime_set(HEART_BEAT_TIMER_EXPIRE_SEC, 0));
            return HRTIMER_RESTART;
        }
        info->write_ret = ret;
        abnormal = true;
    } else {
        info->write_ret = 0;
        abnormal = false;
    }

#ifndef DRV_SOFT_UT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    jiffies_to_timespec64(jiffies, &current_time);
#else
    jiffies_to_timespec(jiffies, &tmp_current_time);

    current_time.tv_nsec = tmp_current_time.tv_nsec;
    current_time.tv_sec = tmp_current_time.tv_sec;
#endif
#endif

    info->last_write_time = current_time;
    if (abnormal || hb_write_work_abnormal_check(info, current_time)) {
        schedule_work(&info->work);
    } else {
        info->last_normal_time = current_time;
    }
    hrtimer_forward_now(htr, ktime_set(HEART_BEAT_TIMER_EXPIRE_SEC, 0));
    return HRTIMER_RESTART;
}

STATIC void heart_beart_write_dfx_handler(struct work_struct *work)
{
    struct hb_write_timer *timer_info = NULL;

    timer_info = container_of(work, struct hb_write_timer, work);
    soft_drv_warn("Don't write heartbeat for a long time."
        "(write heartbeat ret=%d; tatol forget count=%llu; last nomal write time =%llds; last write time=%llds)\n",
        timer_info->write_ret, timer_info->forget_count, timer_info->last_normal_time.tv_sec, timer_info->last_write_time.tv_sec);
}
#endif

void heart_beat_write_status_init(u32 dev_id)
{
    struct hb_write_block* write_item = NULL;

    write_item = hb_get_write_item(dev_id);
    if (write_item == NULL) {
        return;
    }
    write_item->device_status = HEART_BEAT_READY;
    return;
}

void heart_beat_write_status_uninit(u32 dev_id)
{
    struct hb_write_block* write_item = NULL;

    write_item = hb_get_write_item(dev_id);
    if (write_item == NULL) {
        return;
    }
    if (write_item->device_status != HEART_BEAT_LOST) {
        write_item->device_status = HEART_BEAT_EXIT;
    }
    return;
}

int heart_beat_write_timer_init(void)
{
#if (defined CFG_FEATURE_HEARTBEAT_CNT_PCIE) || (!defined CFG_HOST_ENV)
    int i;

    for (i = 0; i < DEVICE_NUM_MAX; i++) {
        g_hb_write_block[i].count = 0;
        g_hb_write_block[i].device_status = HEART_BEAT_NOT_INIT;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 1)
    hrtimer_init(&g_hb_write_timer.timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_HARD);
#else
    hrtimer_init(&g_hb_write_timer.timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
#endif
    g_hb_write_timer.timer.function = heart_beat_write_count;
    INIT_WORK(&g_hb_write_timer.work, heart_beart_write_dfx_handler);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 1)
    hrtimer_start(&g_hb_write_timer.timer, ktime_set(HEART_BEAT_TIMER_EXPIRE_SEC, 0), HRTIMER_MODE_REL_HARD);
#else
    hrtimer_start(&g_hb_write_timer.timer, ktime_set(HEART_BEAT_TIMER_EXPIRE_SEC, 0), HRTIMER_MODE_REL);
#endif
    return DRV_ERROR_NONE;
#else
    return 0;
#endif
}

void heart_beat_write_timer_exit(void)
{
#if (defined CFG_FEATURE_HEARTBEAT_CNT_PCIE) || (!defined CFG_HOST_ENV)
    int i;
    int ret;

    for (i = 0; i < DEVICE_NUM_MAX; i++) {
        heart_beat_write_status_uninit(i);
    }

    ret = hrtimer_cancel(&g_hb_write_timer.timer);
    if (ret < 0) {
        soft_drv_warn("hrtimer cannot cancel. (ret=%d)\n", ret);
    }
    (void)cancel_work_sync(&g_hb_write_timer.work);
    soft_drv_info("Device heartbeat exit success.\n");

    return;
#endif
}
#endif