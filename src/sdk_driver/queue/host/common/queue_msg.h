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

#ifndef QUEUE_MSG_H
#define QUEUE_MSG_H

#include <linux/device.h>
#include <linux/types.h>
#include <linux/rwsem.h>
#include <linux/workqueue.h>

#include "queue_channel.h"
#include "queue_status_record.h"

#include "comm_kernel_interface.h"
#include "queue_h2d_kernel_msg.h"

int queue_drv_msg_chan_init(void);
void queue_drv_msg_chan_uninit(void);
void queue_register_hdc_cb_func(void);
void queue_unregister_hdc_cb_func(void);

struct device *queue_get_device(u32 dev_id);
void queue_put_device(u32 dev_id);
void queue_recv_work(struct work_struct *work);

#endif
