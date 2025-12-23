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

#ifndef VIRTMNGHOST_MSG_COMMON_H
#define VIRTMNGHOST_MSG_COMMON_H

#include "virtmng_msg_pub.h"

int vmngh_alloc_common_msg_cluster(struct vmng_msg_dev *msg_dev);
void vmngh_free_common_msg_cluster(struct vmng_msg_dev *msg_dev);

void vmngh_register_extended_common_msg_client(struct vmng_msg_dev *msg_dev);
void vmngh_unregister_extended_common_msg_client(struct vmng_msg_dev *msg_dev);

#endif
