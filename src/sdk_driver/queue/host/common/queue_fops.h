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

#ifndef QUEUE_FOPS_H
#define QUEUE_FOPS_H
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/semaphore.h>

#include "queue_ioctl.h"
#include "queue_dma.h"
#include "queue_context.h"

#ifdef CFG_PLATFORM_FPGA
#define QUEUE_HOST_WAIT_MAX_TIME 10000000 /* 10000 s */
#else
#define QUEUE_HOST_WAIT_MAX_TIME 10000 /* 10 s */
#endif

#define QUEUE_DEVICE_WAIT_MAX_TIME (QUEUE_HOST_WAIT_MAX_TIME / 2)
#define QUEUE_MAX_DMA_BLK_CNT 26000

extern long hdcdrv_kernel_connect(int dev_id, int service_type, int *session, const char *session_id);
extern long hdcdrv_kernel_close(int session, const char *session_id);
extern long hdcdrv_kernel_send(int session, const char *session_id, void *buf, int len);
extern long hdcdrv_kernel_send_timeout(int session, const char *session_id, void *buf, int len, int timeout);

extern long (*const drv_queue_ioctl_handlers[QUEUE_CMD_MAX])
    (struct file *filep, unsigned int cmd, unsigned long arg);

int queue_drv_open(struct inode *inode, struct file *file);
int queue_drv_release(struct inode *inode, struct file *file);
int queue_drv_module_init(const struct file_operations *ops);
void queue_drv_module_exit(void);
int queue_wakeup_enqueue(struct queue_context *context, u64 que_chan_addr);

#endif
