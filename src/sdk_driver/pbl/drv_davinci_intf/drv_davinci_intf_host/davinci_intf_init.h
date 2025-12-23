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

#ifndef __DAVINCI_INTF_INIT_H__
#define __DAVINCI_INTF_INIT_H__
#include <linux/cpumask.h>
#include <linux/rwsem.h>
#include <linux/version.h>

#include "ascend_dev_num.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
typedef u64 TASK_TIME_TYPE;
#else
typedef struct timespec TASK_TIME_TYPE;
#endif

#define DAVINIC_NONE_ROOT_ACCESS    (0640)

#ifndef ASSERT_RET
#define ASSERT_RET(a, ret) {                      \
        if (!(a)) {        \
            return (ret);  \
        }                  \
    }
#endif

#define DAVINIC_NOT_INIT_BY_OPENCMD    (-1)
#define DAVINIC_FREE_LIST_NOT_INIT    (-1)

#define DAVINIC_INTF_INVAILD_DEVICE_ID    (0xffffffffU)

#define DAVINIC_NOT_CONFIRM_USER_ID    (0xffffffffU)
#define DAVINIC_ROOT_USER_ID           (0x0)
/* max time 60s */
#define DAVINIC_CONFIRM_MAX_TIME    (6000)

#define DAVINIC_CONFIRM_EACH_TIME    (10)

#define DAVINIC_CONFIRM_WARN_MASK    (1000)

#define DAVINIC_FREE_WAIT_MAX_TIME    (3600000)
#define DAVINIC_FREE_WAIT_EACH_TIME    (1)

#define DAVINIC_CONFIRM_USER_NUM    (3)

#define DAVINIC_UNINIT_FILE    "uninit"

#define DAVINIC_FREE_IN_PARALLEL            (1)
#define DAVINIC_FREE_IN_ORDER               (2)
#define DAVINCI_INTF_FULL_DEV_NAME          "/dev/davinci_manager"
#define DAVINCI_INTF_NPU_DEV_CUST_NAME      "npu_device_cust"

#define MODULE_NAME_MAX_LEN 384

struct davinci_intf_file_stru {
    pid_t owner_pid;
    char module_name[DAVINIC_MODULE_NAME_MAX];
    struct inode *inode;
    struct file *file_op;
    struct list_head list;
    int valid;
    unsigned int seq;
    unsigned int open_time;
};

struct davinci_intf_process_stru;

struct davinci_intf_free_list_stru {
    pid_t owner_pid;
    struct davinci_intf_process_stru *owner_proc;
    struct list_head list;
    struct work_struct release_work;
    atomic_t current_count;
    unsigned int current_free_index;
    int all_flag;
};

struct davinci_intf_process_stru {
    void *owner_cb;
    pid_t owner_pid;
    struct list_head list;
    struct list_head file_list;
    struct mutex res_lock;
    atomic_t work_count;
    unsigned int  status;
    TASK_TIME_TYPE start_time;
    struct davinci_intf_free_list_stru *free_list;
};

struct davinci_intf_process_module_stru {
    char module_name[DAVINIC_MODULE_NAME_MAX];
    struct list_head list;
};

struct davinci_intf_sub_module_stru {
    int valid;
    int module_type;
    char module_name[DAVINIC_MODULE_NAME_MAX];
    struct file_operations ops;
    struct notifier_operations notifier;
    struct list_head list;
    int free_type;
    unsigned int open_module_max; /* one process can open max count module, 0 means no limit */
};

struct davinci_intf_stru {
    atomic_t count;
    struct rw_semaphore cb_sem;
    struct cdev cdev;
    struct device *device;
    struct list_head process_list;
    struct list_head module_list;
    unsigned int device_status[ASCEND_DEV_MAX_NUM];
    cpumask_var_t cpumask;
};

struct davinci_intf_free_file_stru {
    pid_t owner_pid;
    struct davinci_intf_free_list_stru *owner_list;
    char module_name[DAVINIC_MODULE_NAME_MAX];
    struct davinci_intf_private_stru *file_private;
    struct list_head list;
    unsigned int free_type;
    unsigned int free_index;
};

struct davinci_intf_private_stru {
    char module_name[DAVINIC_MODULE_NAME_MAX];
    unsigned int device_id;
    pid_t owner_pid;
    int close_flag;
    atomic_t work_count;
    int release_status;
    struct mutex fmutex;
    struct file_operations fops;
    struct notifier_operations notifier;
    struct davinci_intf_stru *device_cb;
    struct file priv_filep;
    unsigned int free_type;
    struct davinci_intf_file_stru *file_stru_node;
    struct davinci_intf_free_list_stru *free_list;
    TASK_TIME_TYPE start_time;
};

void release_file_free_list(struct davinci_intf_free_list_stru *file_free_list);
void drv_ascend_release_work(struct davinci_intf_free_list_stru *free_list);
void drv_intf_trans_free_list_nodes(struct davinci_intf_process_stru *proc,
    struct file *file, unsigned int free_index);
void free_uninit_file_pos(struct davinci_intf_process_stru *proc, struct file *file);
void drv_ascend_free_file_node(struct file *file);
int drv_ascend_add_release_list_all(struct davinci_intf_process_stru *proc, struct file *file);

#endif
