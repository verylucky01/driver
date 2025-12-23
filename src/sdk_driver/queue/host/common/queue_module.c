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
#ifndef QUEUE_UT

#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/printk.h>
#include <linux/mutex.h>
#include <asm/atomic.h>
#include <linux/hashtable.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/mod_devicetable.h>
#include <linux/ioctl.h>

#include <linux/fs.h>
#include <linux/module.h>

#include "queue_module.h"
#include "queue_fops.h"


STATIC int queue_open(struct inode *inode, struct file *file)
{
    return queue_drv_open(inode, file);
}

STATIC int queue_release(struct inode *inode, struct file *file)
{
    return queue_drv_release(inode, file);
}

STATIC long queue_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    if (filep->private_data == NULL) {
        queue_err("The filep private_data is NULL.\n");
        return -EINVAL;
    }

    if (_IOC_TYPE(cmd) != QUEUE_IOC_MAGIC || (unsigned int)_IOC_NR(cmd) >= (unsigned int)QUEUE_CMD_MAX) {
        queue_err("Cmd is invalid. (_IOC_TYPE=%u, _IOC_NR=%u)\n", _IOC_TYPE(cmd), _IOC_NR(cmd));
        return -EINVAL;
    }

    if (arg == 0) {
        queue_err("Arg is null. (_IOC_NR=%u)\n", _IOC_NR(cmd));
        return -EINVAL;
    }

    if (drv_queue_ioctl_handlers[_IOC_NR(cmd)] == NULL) {
        queue_err("invalid cmd, cmd = %u\n", _IOC_NR(cmd));
        return -EINVAL;
    }

    return drv_queue_ioctl_handlers[_IOC_NR(cmd)](filep, cmd, arg);
}

static const struct file_operations g_queue_fops = {
    .owner =    THIS_MODULE,
    .unlocked_ioctl = queue_ioctl,
    .open = queue_open,
    .release = queue_release,
};

STATIC int __init queue_module_init(void)
{
    int ret;

    ret = queue_drv_module_init(&g_queue_fops);
    if (ret != 0) {
        queue_err("module init failed, ret=%d.\n", ret);
        return ret;
    }

    return ret;
}

STATIC void __exit queue_module_exit(void)
{
    queue_drv_module_exit();
}

module_init(queue_module_init);
module_exit(queue_module_exit);

MODULE_LICENSE("GPL");
#else
void queue_module_ut(void)
{
    return;
}
#endif
