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
#ifndef __DMC_LOG_EVENT_WRITE_H__
#define __DMC_LOG_EVENT_WRITE_H__
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/atomic.h>

#define LOG_PRINT_LEN         0x400     // 1024 Bytes

typedef struct log_drv_fault_mng {
    unsigned int buf_read;
    unsigned int buf_write;
    unsigned int buf_size;
    spinlock_t spinlock;
    wait_queue_head_t wq;
    atomic_t status;
    char printk_buf[LOG_PRINT_LEN];
    unsigned char *vir_addr; // Log Ring Buffer
    unsigned char *temp_buf;
} log_drv_fault_mng_t;

void log_set_fault_mng_info(struct log_drv_fault_mng *fault_mng);
#endif