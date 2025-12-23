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

#ifndef __VIRTMNGHOST_PCI_H__
#define __VIRTMNGHOST_PCI_H__

#include "vmng_kernel_interface.h"
#include "ascend_dev_num.h"
#include "virtmng_public_def.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define VMNG_FID_BEGIN 0x1
#define VMNG_START_TIMER_OUT 100
#define VMNG_START_TIMER_CYCLE (1 * HZ)
#define VMNG_AICORE_NUM 32
#define VMNGH_INIT_INSTANCE_TIMEOUT (4 * HZ)   /* 4s */

#define VMNG_NON_TRANS_MSG_S_DESC_SIZE 0x10000 /* 64k, pcie support */
#define VMNG_NON_TRANS_MSG_C_DESC_SIZE 0x10000 /* 64k, pcie support */

#define VMNG_WAIT_INIT_TIME 100 /* 10s */

int vmngh_vdev_msg_send(struct vmng_ctrl_msg_info *info, int msg_type);
void vmngh_free_vdev_host_stop(u32 dev_id, u32 fid);

int vmngh_add_mia_dev(u32 dev_id, u32 vfid, u32 agent_flag);
int vmngh_remove_mia_dev(u32 dev_id, u32 vfid, u32 agent_flag);

int vmngh_init_module(void);
void vmngh_exit_module(void);
#endif
