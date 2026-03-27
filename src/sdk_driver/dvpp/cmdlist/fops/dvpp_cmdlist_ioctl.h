/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026. All rights reserved.
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
#ifndef BASE_DVPP_CMDLIST_IOCTL_H
#define BASE_DVPP_CMDLIST_IOCTL_H

#include <linux/types.h>
#include <asm/ioctl.h>
#include "ka_ioctl_pub.h"
#include "ka_list_pub.h"
#include "ka_task_pub.h"
#include "ka_common_pub.h"
#include "dvpp_cmdlist_sqe.h"
#include "dvpp_cmdlist_interface.h"

typedef struct {
    ka_rw_semaphore_t rw_sem;
    ka_hlist_head_t head;
    pid_t pid;
} dvpp_cmdlist_private_data;

typedef struct {
    int32_t pid; // tgid
    dvpp_cmdlist_private_data* data;
} dvpp_cmdlist_ioctl_info;

// 用来传入内核态执行的函数指针的参数结构体
typedef struct {
    dvpp_cmdlist_ioctl_info info;
    void *user_data;    // 用户态传入的数据，如gen_cmdlist中的dvpp_gen_cmdlist_user_data
} dvpp_cmdlist_ioctl_args;

// 统一函数指针格式方便扩展
typedef int32_t (*CMDLIST_HANDLER)(dvpp_cmdlist_ioctl_args*);

typedef struct {
    uint32_t cmd;
    CMDLIST_HANDLER handler;
} cmdlist_case;

int32_t dvpp_cmdlist_dev_init(void);
void dvpp_cmdlist_dev_exit(void);

int32_t dvpp_set_gen_cmdlist_func(cmdlist_case *cmd_case);
int32_t dvpp_get_gen_cmdlist_info_from_ioctl(dvpp_cmdlist_ioctl_args *arg,
    dvpp_gen_cmdlist_user_data *user_data, int32_t *pid, uint32_t *devid, uint32_t *phyid, struct dvpp_sqe *sqe);
int32_t dvpp_set_gen_cmdlist_sqe_to_user_data(struct dvpp_sqe *sqe, dvpp_gen_cmdlist_user_data *user_data);
void dvpp_get_version_init(void);
void dvpp_del_chn_hlist_init(void);

#endif // #ifndef BASE_DVPP_CMDLIST_IOCTL_H