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

#ifndef _HDCDRV_ADAPT_H_
#define _HDCDRV_ADAPT_H_
#include <linux/types.h>
#include <linux/version.h>
#include "securec.h"
#include "hdcdrv_cmd_msg.h"
#include "hdcdrv_cmd_ioctl.h"
#include "dms/dms_devdrv_manager_comm.h"

extern unsigned int hdcdrv_dev_num;

#define HDCDRV_REAL_MAX_SESSION          ((int)(HDCDRV_SINGLE_DEV_MAX_SESSION * (int)hdcdrv_dev_num))
#define HDCDRV_REAL_MAX_SHORT_SESSION    (HDCDRV_SINGLE_DEV_MAX_SHORT_SESSION * (int)hdcdrv_dev_num)
#define HDCDRV_REAL_MAX_LONG_SESSION     (HDCDRV_REAL_MAX_SESSION - HDCDRV_REAL_MAX_SHORT_SESSION)

#if defined(CFG_FEATURE_VFIO)
#define HDCDRV_VM_NUM (HDCDRV_DEV_MAX_VDEV_PER_DEVICE * hdcdrv_dev_num)
#define HDCDRV_EPOLL_FD_NUM (HDCDRV_VM_NUM * 128)
#else
#if defined(CFG_FEATURE_SRIOV)
#define HDCDRV_VM_NUM (HDCDRV_DEV_MAX_VDEV_PER_DEVICE * hdcdrv_dev_num)
#define HDCDRV_EPOLL_FD_NUM (HDCDRV_VM_NUM * 128)
#else
#define HDCDRV_VM_NUM hdcdrv_dev_num
#define HDCDRV_EPOLL_FD_NUM (128U * hdcdrv_dev_num)
#endif
#endif
#define HDCDRV_MAX_VM_NUM (HDCDRV_VM_NUM + 1U)

#define HDCDRV_DOCKER_MAX_NUM (MAX_DOCKER_NUM + 1U) /* 1(host)+69(MAX_DOCKER_NUM) */
#define HDCDRV_PHY_HOST_ID (MAX_DOCKER_NUM) /* host id */

#define module_hdcdrv "hdcdrv"

#ifdef DRV_UT
#define drv_err_log(fmt, ...) do { \
    drv_err(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0)
#define drv_info_log(fmt, ...) do { \
    drv_info(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0)
#else
#define drv_err_log(fmt, ...) do { \
    drv_err(module_hdcdrv, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
    share_log_err(HDC_SHARE_LOG_START, fmt, ##__VA_ARGS__); \
} while (0)
#define drv_info_log(fmt, ...) do { \
    share_log_run_info(HDC_SHARE_LOG_RUNINFO_START, fmt, ##__VA_ARGS__); \
} while (0)
#endif

static inline int hdcdrv_get_max_support_dev(void)
{
  return (int)HDCDRV_SUPPORT_MAX_DEV;
}

int hdcdrv_get_msgchan_refcnt(u32 dev_id);
int hdcdrv_put_msgchan_refcnt(u32 dev_id);

#ifndef CFG_FEATURE_VHDC_ADAPT
struct hdcdrv_ctrl_msg;
union hdcdrv_cmd;
u32 hdcdrv_gen_unique_value(void);
bool hdcdrv_ctrl_msg_connect_get_permission(const struct hdcdrv_ctrl_msg *msg, u32 devid);
int hdcdrv_get_connect_fid(int service_type, u32 fid);
long hdcdrv_convert_id_from_vir_to_phy(u32 drv_cmd, union hdcdrv_cmd *cmd_data, u32 *vfid);
void hdcdrv_init_register(void);
void hdcdrv_uninit_unregister(void);
void hdcdrv_set_session_run_env(u32 dev_id, u32 fid, int *run_env);
#endif

#define HDC_NID_ID_MAX_NUM 32
struct page *hdcdrv_alloc_pages_node_inner(u32 dev_id, gfp_t gfp_mask, u32 order);
void *hdcdrv_kzalloc_mem_node_inner(u32 dev_id, gfp_t gfp_mask, u32 size, u32 level);
gfp_t hdcdrv_init_mem_pool_get_gfp(void);
u64 hdcdrv_get_hash_fid(u32 fid);

#endif