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
#ifndef __DMC_LOG_EVENT_WRITE_H__
#define __DMC_LOG_EVENT_WRITE_H__
#include "ka_base_pub.h"
#include "ka_task_pub.h"

#define LOG_PRINT_LEN         0x400     // 1024 Bytes

typedef struct log_drv_fault_mng {
    unsigned int buf_read;
    unsigned int buf_write;
    unsigned int buf_size;
    ka_task_spinlock_t spinlock;
    ka_wait_queue_head_t wq;
    ka_atomic_t status;
    char printk_buf[LOG_PRINT_LEN];
    unsigned char *vir_addr; // Log Ring Buffer
    unsigned char *temp_buf;
} log_drv_fault_mng_t;

void log_set_fault_mng_info(struct log_drv_fault_mng *fault_mng);
#endif