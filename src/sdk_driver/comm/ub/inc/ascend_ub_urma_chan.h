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
#ifndef _ASCEND_UB_URMA_CHAN_H_
#define _ASCEND_UB_URMA_CHAN_H_
#include "ubcore_types.h"
#include "ascend_ub_dev.h"
#include "ascend_ub_msg.h"
#include "ascend_ub_msg_def.h"
#include "ascend_ub_jetty.h"
#include "comm_kernel_interface.h"

void ubdrv_urma_chan_init(struct ascend_ub_msg_dev *msg_dev);
void ubdrv_urma_chan_uninit(struct ascend_ub_msg_dev *msg_dev);
int ubdrv_device_alloc_urma_chan(struct ascend_ub_msg_dev *msg_dev, void *data);
int ubdrv_device_free_urma_chan(struct ascend_ub_msg_dev *msg_dev, void *data);
int ubdrv_alloc_urma_chan(u32 dev_id);
void ubdrv_free_urma_chan(u32 dev_id);
int ubdrv_urma_copy(u32 dev_id, enum devdrv_urma_chan_type type, enum devdrv_urma_copy_dir dir,
    struct devdrv_urma_copy *local, struct devdrv_urma_copy *peer);
int ubdrv_register_seg(u32 dev_id, struct devdrv_seg_info *info, void **tseg, size_t *out_len);
int ubdrv_unregister_seg(u32 dev_id, void *tseg, size_t in_len);
void *ubdrv_import_seg(u32 dev_id, u32 peer_token, void *peer_seg, size_t in_len, size_t *out_len);
int ubdrv_unimport_seg(u32 dev_id, void *peer_tseg, size_t in_len);
#endif