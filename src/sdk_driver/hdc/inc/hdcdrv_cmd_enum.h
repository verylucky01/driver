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

#ifndef _HDCDRV_CMD_ENUM_H_
#define _HDCDRV_CMD_ENUM_H_

#include "hdcdrv_cmd_ioctl.h"
#include "hdcdrv_cmd_msg.h"

#define HDCDRV_MANAGE_MIN_SERVICE_TYPE 64
#define HDCDRV_MANAGE_MAX_SERVICE_TYPE 95
#define HDCDRV_DATA_MIN_SERVICE_TYPE 96
#define HDCDRV_DATA_MAX_SERVICE_TYPE 127

#define HDCDRV_DEV_ID_DEFAULT ((int)(-1))

#define HDCDRV_SUPPORT_MAX_DEV_MSG_CHAN 64
#define HDCDRV_PFSTATE_NON_TRANS_CHAN_ID HDCDRV_SUPPORT_MAX_DEV_MSG_CHAN
#define HDCDRV_PFSTATE_MAX_CHAN (HDCDRV_SUPPORT_MAX_DEV_MSG_CHAN + 1)

#define HDCDRV_SRIOV_VF_SUPPORT_MAX_NORMAL_MSG_CHAN 2
#ifdef CFG_FEATURE_VFIO
#define HDCDRV_SUPPORT_MAX_DEV_NORMAL_MSG_CHAN (HDCDRV_DEV_MAX_VDEV_PER_DEVICE * 2)
#else
#define HDCDRV_SUPPORT_MAX_DEV_NORMAL_MSG_CHAN HDCDRV_SUPPORT_MAX_SERVICE
#endif

#define HDCDRV_SID_LEN 32

#define HDCDRV_NOWAIT_RETRY_TIMES 3000
#define HDCDRV_NOWAIT_USLEEP_MIN  1000
#define HDCDRV_NOWAIT_USLEEP_MAX  1010

#define HDCDRV_CLOSE_RMT_SESSION_RETRY_CNT 10

/* if add a new error code need to add the same str in g_errno_str */
#define HDCDRV_WAIT_ALWAYS 0
#define HDCDRV_NOWAIT 1
#define HDCDRV_WAIT_TIMEOUT 2

#define HDCDRV_MODE_DEFAULT 0
#define HDCDRV_MODE_CONTAINER 1


#define HDCDRV_SESSION_RUN_ENV_UNKNOW 0
#define HDCDRV_SESSION_RUN_ENV_PHYSICAL 1
#define HDCDRV_SESSION_RUN_ENV_PHYSICAL_CONTAINER 2
#define HDCDRV_SESSION_RUN_ENV_VIRTUAL 3
#define HDCDRV_SESSION_RUN_ENV_VIRTUAL_CONTAINER 4

#define HDCDRV_MEM_ORDER_1MB 8      /* 1M */


#define HDCDRV_MEM_MIN_PAGE_LEN_BIT PAGE_SHIFT
#define HDCDRV_MEM_MIN_HUGE_PAGE_LEN_BIT HPAGE_SHIFT

#define HDCDRV_MEM_ORDER_NUM 11 /* 4M order is 10 */
#define HDCDRV_MEM_SCORE_SCALE 100

#define HDCDRV_FAST_MEM_TYPE_TX_DATA 0
#define HDCDRV_FAST_MEM_TYPE_TX_CTRL 1
#define HDCDRV_FAST_MEM_TYPE_RX_DATA 2
#define HDCDRV_FAST_MEM_TYPE_RX_CTRL 3
#define HDCDRV_FAST_MEM_TYPE_DVPP 4
#define HDCDRV_FAST_MEM_TYPE_ANY 5
#define HDCDRV_FAST_MEM_TYPE_MAX  6
#define HDCDRV_PAGE_TYPE_NORMAL 0
#define HDCDRV_PAGE_TYPE_HUGE 1
#define HDCDRV_PAGE_TYPE_NONE 2
#define HDCDRV_PAGE_TYPE_REGISTER 3

#define HDCDRV_SESSION_OWNER_PM 0
#define HDCDRV_SESSION_OWNER_VM 1
#define HDCDRV_SESSION_OWNER_CT 2

#define HDCDRV_DEFAULT_VM_ID 0 /* physical */

#define HDCDRV_RETRY_SLEEP_TIME 5 /* ms */

#define HDCDRV_KERNEL_WITHOUT_CTX (struct hdcdrv_ctx *)-4

/* same as HDC_EPOLL_OP_* */
#define HDCDRV_EPOLL_OP_ADD 0
#define HDCDRV_EPOLL_OP_DEL 1

/* same as HDC_EPOLL_* */
#define HDCDRV_EPOLL_CONN_IN (0x1 << 0)
#define HDCDRV_EPOLL_DATA_IN (0x1 << 1)
#define HDCDRV_EPOLL_FAST_DATA_IN (0x1 << 2)
#define HDCDRV_EPOLL_SESSION_CLOSE (0x1 << 3)

#define HDCDRV_EPOLL_CTL_PARA_NUM    4

#define HDCDRV_RUNNING_NORMAL 0
#define HDCDRV_RUNNING_SUSPEND 1
#define HDCDRV_RUNNING_RESUME 2 // rcv resume callback
#define HDCDRV_RUNNING_SUSPEND_ENTERING 3

#endif
