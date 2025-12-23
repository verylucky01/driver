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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/device.h>
#include <linux/uio_driver.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include "securec.h"

#include "common.h"
#include "pci-dev.h"
#include "ioctl_comm.h"
#include "ioctl_comm_def.h"
#include "ka_dfx_pub.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"

#ifndef STATIC_SKIP
    #define STATIC static
#else
    #define STATIC
#endif

typedef struct PCI_BAR_RD_IN_tag {
    uint32_t bus; /* 这条 PCI 总线的总线编号 */
    uint32_t device;
    uint32_t function;
    uint32_t bar;  /* bar id */
    uint32_t addr; /* bar中的地址 */
} PCI_BAR_RD_IN_S;
struct pci_dev_info {
    struct pci_dev *pdev;
    struct mutex lock;
    void __iomem *bar_mem_addr;
    unsigned long bar_mem_len; /* 长度为0 说明bar无效 */
    uint32_t bus;                           /* 这条 PCI 总线的总线编号 */
    uint32_t device;
    uint32_t function;
    uint32_t irq_num;
};

extern SramDescCtlHeader g_fault_event_head;

STATIC int lq_get_fault_event_head_info(IOCTL_CMD_S *ioctl_cmd)
{
    int ret;
    SramDescCtlHeader info_pipe;    /* 这个结构体是否和 sdk 新版本的结构体信息一致 ？ */

    info_pipe.version = g_fault_event_head.version;
    info_pipe.length = g_fault_event_head.length;
    info_pipe.nodeSize = g_fault_event_head.nodeSize;
    info_pipe.nodeNum = g_fault_event_head.nodeNum;
    info_pipe.nodeHead = g_fault_event_head.nodeHead;
    info_pipe.nodeTail = g_fault_event_head.nodeTail;

    ret = copy_to_user(ioctl_cmd->out_addr, &info_pipe, sizeof(SramDescCtlHeader));
    if (ret != 0) {
        printk(KA_KERN_ERR "[lqdcmi]copy_to_user fail ret=%d\n", ret);
        return ret;
    }

    return 0;
}

STATIC int get_all_fault_by_pci(IOCTL_CMD_S *ioctl_cmd)
{
    int ret;
    FaultEventNodeTable *temp_table = NULL;
    size_t table_size = sizeof(FaultEventNodeTable) * CAPACITY;

    if (ioctl_cmd->out_size < table_size) {
        printk(KA_KERN_ERR "Buffer too small: %zu < %zu.\n", ioctl_cmd->out_size, table_size);
        return -EFAULT;
    }

    temp_table = kzalloc(table_size, KA_GFP_KERNEL);
    if (!temp_table) {
        printk(KA_KERN_ERR "[lqdcmi]kzalloc fail.\n");
        return -ENOMEM;
    }
    ka_task_mutex_lock(&g_kernel_table_mutex);
    ret = memcpy_s(temp_table, table_size, g_kernel_event_table, table_size);
    if (ret != 0) {
        ka_task_mutex_unlock(&g_kernel_table_mutex);
        printk(KA_KERN_ERR "[lqdcmi]memcpy_s fail. ret=%d\n", ret);
        goto OUT_FREE;
    }
    ka_task_mutex_unlock(&g_kernel_table_mutex);

    ret = copy_to_user(ioctl_cmd->out_addr, temp_table, table_size);
    if (ret != 0) {
        printk(KA_KERN_ERR "[lqdcmi]copy_to_user fail, ret=%d.\n", ret);
        goto OUT_FREE;
    }
    printk(KA_KERN_INFO "Get all fault table success\r\n");

OUT_FREE:
    ka_mm_kfree(temp_table);
    temp_table = NULL;
    return ret;
}

IOCTL_CMD_INFO_S ioctl_cmd_fun[] = {
    { .cmd = IOCTL_GET_NODE_INFO,     .cmd_fun = get_all_fault_by_pci,      .cmd_fun_pre = NULL,      },
    { .cmd = IOCTL_GET_HEAD_INFO,     .cmd_fun = lq_get_fault_event_head_info,          .cmd_fun_pre = NULL,      },
};

uint32_t g_ioctl_cmd_num = sizeof(ioctl_cmd_fun) / sizeof(IOCTL_CMD_INFO_S);

int pcidev_ioctl(void* msg)
{
    IOCTL_CMD_S ioctl_cmd = {0};
    uint32_t i;
    int ret;

    if (msg == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]pointer paremeter msg is NULL!\n");
        return -EINVAL;
    }

    ret = ka_base_copy_from_user(&ioctl_cmd, msg, sizeof(ioctl_cmd));
    if (ret != 0) {
        printk(KA_KERN_ERR "[lqdcmi]ka_base_copy_from_user fail ret: %d\n", ret);
        return ret;
    }

    for (i = 0; i < g_ioctl_cmd_num; i++) {
        if (ioctl_cmd.cmd != ioctl_cmd_fun[i].cmd) {
            continue;
        }
        if (ioctl_cmd_fun[i].cmd_fun_pre != NULL) {
            ret = ioctl_cmd_fun[i].cmd_fun_pre();
            if (ret != 0) {
                break;
            }
        }

        ret = (*ioctl_cmd_fun[i].cmd_fun)(&ioctl_cmd);
        break;
    }

    if (i == g_ioctl_cmd_num) {
        printk(KA_KERN_ERR "[lqdcmi]ioctl cmd %d not exist\n", ioctl_cmd.cmd);
        return -EINVAL;
    }

    return ret;
}
