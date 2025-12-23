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

#ifndef LOG_UT
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <linux/ktime.h>
#include <linux/pci.h>
#include <linux/mod_devicetable.h>

#include "securec.h"
#include "log_drv_agent.h"

STATIC log_ring_buf_t g_log_ring_buf = { 0 };
static DEFINE_RATELIMIT_STATE(drv_log_err_ratelimit, 1 * HZ, 5);

STATIC int log_get_date(char *date, u32 len)
{
    struct timespec64 sys_time = { 0 };
    struct rtc_time tm = { 0 };
    int ret;

    ktime_get_real_ts64(&sys_time);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    tm = rtc_ktime_to_tm(timespec64_to_ktime(sys_time));
#else
    rtc_time_to_tm(sys_time.tv_sec, &tm);
#endif
    ret = snprintf_s(date, len, len - 1, "%04ld-%02d-%02d-%02d:%02d:%02d.%06llu",
                     tm.tm_year + TWENTY_CENTURY, tm.tm_mon + JANUARY, tm.tm_mday, tm.tm_hour,
                     tm.tm_min, tm.tm_sec, sys_time.tv_nsec / KILO);
    if (ret < 0) {
        slog_drv_err("Log snprintf_s failed. (ret=%d)\n", ret);
        return ret;
    }
    return 0;
}

STATIC int log_write_ringbuffer(void)
{
    u32 buf_left, str_len;
    int ret;

    buf_left = LOG_RINGBUF_SIZE - g_log_ring_buf.point;
    str_len = strnlen(g_log_ring_buf.printk_buf, LOG_PRINT_LEN);
    if ((str_len == 0) || (str_len >= LOG_PRINT_LEN)) {
        return -EINVAL;
    }

    if (str_len > buf_left) {
        ret = memcpy_s(g_log_ring_buf.log_buf + g_log_ring_buf.point, buf_left, g_log_ring_buf.printk_buf, buf_left);
        if (ret != 0) {
            return ret;
        }
        ret = memcpy_s(g_log_ring_buf.log_buf, LOG_RINGBUF_SIZE, (char*)g_log_ring_buf.printk_buf + buf_left,
            str_len - buf_left);
        if (ret != 0) {
            return ret;
        }
    } else {
        ret = memcpy_s(g_log_ring_buf.log_buf + g_log_ring_buf.point, buf_left, g_log_ring_buf.printk_buf, str_len);
        if (ret != 0) {
            return ret;
        }
    }
    g_log_ring_buf.point = (g_log_ring_buf.point + str_len) % LOG_RINGBUF_SIZE;
    g_log_ring_buf.size = ((g_log_ring_buf.size + str_len) >= LOG_RINGBUF_SIZE) ?
        LOG_RINGBUF_SIZE : (g_log_ring_buf.size + str_len);

    return 0;
}

static int log_save_to_ringbuf(const char *fmt, va_list args)
{
    char date[DATATIME_MAXLEN] = { 0 };
    static char new_fmt[LOG_PRINT_LEN];
    char *module = NULL;
    unsigned long flags;
    va_list args_backup;
    int ret;

    if (g_log_ring_buf.log_buf == NULL) {
        return -EINVAL;
    }

    ret = log_get_date(date, DATATIME_MAXLEN);
    if (ret != 0) {
        return ret;
    }

    spin_lock_irqsave(&g_log_ring_buf.logbuf_lock, flags);
    ret = snprintf_s(new_fmt, LOG_PRINT_LEN, LOG_PRINT_LEN - 1, "[%s] %s", date, fmt + LOG_LEVEL_OFFSET);
    if (ret < 0) {
        spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
        slog_drv_err("Log snprintf_s failed. (ret=%d)\n", ret);
        return -EINVAL;
    }

    va_copy(args_backup, args);
    ret = vsnprintf_s(g_log_ring_buf.printk_buf, LOG_PRINT_LEN, LOG_PRINT_LEN - 1, new_fmt, args);
    if (ret <= 0) {
        spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
        module = va_arg(args_backup, char*);
        va_end(args_backup);
        if ((module != NULL) && (strnlen(module, LOG_PRINT_LEN) < LOG_PRINT_LEN)) {
            slog_drv_err("Log format is incorrect. (module=%s, ret=%d)\n", module, ret);
        }
        return -EINVAL;
    }
    va_end(args_backup);

    ret = log_write_ringbuffer();
    if (ret != 0) {
        spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
        slog_drv_err("Log write ringbuffer failed. (ret=%d)\n", ret);
        return ret;
    }

    spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
    return 0;
}

void log_save_to_ring_buf(const char *fmt, ...)
{
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = log_save_to_ringbuf(fmt, args);
    va_end(args);
    if (ret != 0) {
        slog_drv_err("Log save to ring_buf failed. (ret=%d)\n", ret);
        return;
    }

    return;
}
EXPORT_SYMBOL_GPL(log_save_to_ring_buf);

void log_to_printk_and_ringbuf(const char *fmt, ...)
{
    va_list args;
    int ret;

    if (__ratelimit(&drv_log_err_ratelimit)) {
        va_start(args, fmt);
        (void)vprintk(fmt, args);
        va_end(args);
    }

    va_start(args, fmt);
    ret = log_save_to_ringbuf(fmt, args);
    va_end(args);
    if (ret != 0) {
        slog_drv_err("Log save to ring_buf failed. (ret=%d)\n", ret);
        return;
    }

    return;
}
EXPORT_SYMBOL_GPL(log_to_printk_and_ringbuf);

int log_get_ringbuffer(char *buff, u32 buf_len, u32 *out_len)
{
    unsigned long flags;
    u32 pos, actual_len;
    char *tmp_buf;
    int ret;

    if ((buff == NULL) || (buf_len != LOG_RINGBUF_SIZE) || (out_len == NULL)) {
        slog_drv_err("Invalid paras. (buff=%d, buf_len=%u, out_len=%d)\n", (int)(buff == NULL), buf_len,
            (int)(out_len == NULL));
        return -EINVAL;
    }

    tmp_buf = log_drv_vzalloc(LOG_RINGBUF_SIZE);
    if (tmp_buf == NULL) {
        slog_drv_err("Vzalloc tmp_buf failed.\n");
        return -ENOMEM;
    }

    spin_lock_irqsave(&g_log_ring_buf.logbuf_lock, flags);
    pos = g_log_ring_buf.point;

    if ((g_log_ring_buf.size > LOG_RINGBUF_SIZE) || (pos >= LOG_RINGBUF_SIZE)) {
        spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
        log_drv_vfree(tmp_buf);
        tmp_buf = NULL;
        slog_drv_err("Invalid paras. (max_len=%u, size=%u, pos=%u)\n", LOG_RINGBUF_SIZE, g_log_ring_buf.size, pos);
        return -EINVAL;
    }

    if (g_log_ring_buf.size < LOG_RINGBUF_SIZE) {
        ret = memcpy_s((void *)(uintptr_t)tmp_buf, LOG_RINGBUF_SIZE, g_log_ring_buf.log_buf, pos);
        if (ret != 0) {
            spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
            goto copy_fail;
        }
    } else {
        ret = memcpy_s((void *)(uintptr_t)tmp_buf, LOG_RINGBUF_SIZE, g_log_ring_buf.log_buf + pos, LOG_RINGBUF_SIZE - pos);
        if (ret != 0) {
            spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
            goto copy_fail;
        }
        ret = memcpy_s((void *)(uintptr_t)tmp_buf + (LOG_RINGBUF_SIZE - pos), pos, g_log_ring_buf.log_buf, pos);
        if (ret != 0) {
            spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);
            goto copy_fail;
        }
    }

    actual_len = g_log_ring_buf.size;
    spin_unlock_irqrestore(&g_log_ring_buf.logbuf_lock, flags);

    ret = copy_to_user((void *)(uintptr_t)buff, tmp_buf, actual_len);
    if (ret != 0) {
        goto copy_fail;
    }

    *out_len = actual_len;
    log_drv_vfree(tmp_buf);
    tmp_buf = NULL;

    return 0;

copy_fail:
    log_drv_vfree(tmp_buf);
    tmp_buf = NULL;
    slog_drv_err("Log copy failed. (ret=%d)\n", ret);
    return ret;
}
EXPORT_SYMBOL_GPL(log_get_ringbuffer);

STATIC int log_ringbuffer_init(void)
{
    g_log_ring_buf.log_buf = log_drv_vzalloc(LOG_RINGBUF_SIZE);
    if (g_log_ring_buf.log_buf == NULL) {
        slog_drv_err("Vzalloc g_log_ring_buf failed.\n");
        return -ENOMEM;
    }

    g_log_ring_buf.point = 0;
    g_log_ring_buf.size = 0;
    spin_lock_init(&g_log_ring_buf.logbuf_lock);

    return 0;
}

STATIC void log_ringbuffer_uninit(void)
{
    if (g_log_ring_buf.log_buf != NULL) {
        log_drv_vfree(g_log_ring_buf.log_buf);
        g_log_ring_buf.log_buf = NULL;
    }
}

int log_drv_module_init(void)
{
    int ret;

    ret = log_ringbuffer_init();
    if (ret != 0) {
        slog_drv_err("Log ringbuffer init failed. (ret=%d)\n", ret);
        return ret;
    }

    slog_drv_info("Host logdrv module init successfully.\n");
    return 0;
}

void log_drv_module_exit(void)
{
    log_ringbuffer_uninit();

    return;
}
#else
int drv_log_ut(void)
{
    return 0;
}
#endif