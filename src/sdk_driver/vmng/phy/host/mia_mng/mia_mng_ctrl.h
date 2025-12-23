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
#ifndef _MIA_MNG_CTRL_H
#define _MIA_MNG_CTRL_H
#include <linux/mutex.h>
#include "ascend_dev_num.h"
#include "virtmng_public_def.h"

#define MAX_MIA_GROUP_NUM 2

#define MIA_GROUP_DELETE 0
#define MIA_GROUP_CREATE 1

struct mia_mng_inst {
    u32 udevid;                 /* udevid */
    int mia_context;            /* vm or pm container or others */
    int mia_status;
    struct mutex mutex_lock;
    u32 group_status[MAX_MIA_GROUP_NUM];    /* 1:create 0:delete */
};

int mia_mng_msg_send(struct vmng_ctrl_msg *msg, int msg_type);
struct mia_mng_inst *mia_mng_get_inst(u32 dev_id);
void mia_mng_set_group_status(u32 dev_id, u32 group_id, int status);
bool mia_mng_group_exist(u32 dev_id, u32 group_id);

#endif // _MIA_MNG_CTRL_H