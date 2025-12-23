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

#include "pbl/pbl_feature_loader.h"
#include "comm_kernel_interface.h"
#include "mia_mng_ctrl.h"

struct mia_mng_inst *mia_table[ASCEND_DEV_MAX_NUM];
struct mutex g_mia_mng_mutex;
static u32 mia_inst_num;

STATIC int mia_mng_common_msg_send(u32 devid, struct vmng_ctrl_msg *msg)
{
    u32 msg_size = sizeof(*msg);
    u32 len = 0;
    int ret;

    ret = devdrv_common_msg_send(devid, (void *)msg, msg_size, msg_size, &len, DEVDRV_COMMON_MSG_VMNG);
    if ((ret != VMNG_OK) || (len != msg_size) || (msg->error_code != VMNG_OK)) {
        vmng_err("Send message failed. (devid=%u;ret=%d; len=%u; error_code=%d)\n", devid, ret, len, msg->error_code);
        return ret != 0 ? ret : msg->error_code;
    }
    return 0;
}

int mia_mng_msg_send(struct vmng_ctrl_msg *msg, int msg_type)
{
    struct mia_ctrl_info *mia_msg;
    int ret;

    if (msg == NULL) {
        vmng_err("Input mia_msg is null\n");
        return -EINVAL;
    }
    mia_msg = &msg->mia_msg;
    msg->type = msg_type;

    ret = mia_mng_common_msg_send(mia_msg->dev_id, msg);
    if (ret != 0) {
        vmng_err("Send mia msg failed. (devid=%u; ret=%d; type=%d)\n", mia_msg->dev_id, ret, msg_type);
        return ret;
    }

    return VMNG_OK;
}

struct mia_mng_inst *mia_mng_get_inst(u32 dev_id)
{
    if (dev_id >= ASCEND_DEV_MAX_NUM) {
        vmng_err("Invalid dev_id. (dev_id=%u)\n", dev_id);
        return NULL;
    }
    return mia_table[dev_id];
}

void mia_mng_set_group_status(u32 dev_id, u32 group_id, int status)
{
    struct mia_mng_inst *mia_inst = mia_mng_get_inst(dev_id);
    if (mia_inst == NULL) {
        return;
    }
    if (group_id >= MAX_MIA_GROUP_NUM) {
        vmng_err("Invalid group id. (id=%u)\n", group_id);
        return;
    }
    mia_inst->group_status[group_id] = (u32)status;
}

bool mia_mng_group_exist(u32 dev_id, u32 group_id)
{
    struct mia_mng_inst *mia_inst = mia_mng_get_inst(dev_id);
    if (mia_inst == NULL) {
        return false;
    }
    if (group_id >= MAX_MIA_GROUP_NUM) {
        vmng_err("Invalid group id. (id=%u)\n", group_id);
        return false;
    }
    return (mia_inst->group_status[group_id] == MIA_GROUP_CREATE);
}

STATIC int mia_mng_dev_init(u32 dev_id)
{
    struct mia_mng_inst *inst = NULL;

    if (dev_id >= ASCEND_DEV_MAX_NUM) {
        vmng_err("Invalid dev_id. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    mutex_lock(&g_mia_mng_mutex);
    if (mia_table[dev_id] != NULL) {
        mutex_unlock(&g_mia_mng_mutex);
        vmng_err("Mia instance has ready exist. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    inst = kzalloc(sizeof(struct mia_mng_inst), GFP_KERNEL);
    if (inst == NULL) {
        mutex_unlock(&g_mia_mng_mutex);
        vmng_err("Kzalloc mia instance failed. (dev_id=%u)\n", dev_id);
        return -ENOMEM;
    }
    inst->udevid = dev_id;
    mia_table[dev_id] = inst;
    mia_inst_num++;
    mutex_unlock(&g_mia_mng_mutex);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT_DEV(mia_mng_dev_init, FEATURE_LOADER_STAGE_5);

STATIC void mia_mng_dev_uninit(u32 dev_id)
{
    if (dev_id >= ASCEND_DEV_MAX_NUM) {
        vmng_err("Invalid dev_id. (dev_id=%u)\n", dev_id);
        return;
    }

    mutex_lock(&g_mia_mng_mutex);
    if (mia_table[dev_id] == NULL) {
        mutex_unlock(&g_mia_mng_mutex);
        vmng_err("Mia instance is null. (dev_id=%u)\n", dev_id);
        return;
    }

    mia_inst_num--;
    kfree(mia_table[dev_id]);
    mia_table[dev_id] = NULL;
    mutex_unlock(&g_mia_mng_mutex);
}
DECLAER_FEATURE_AUTO_UNINIT_DEV(mia_mng_dev_uninit, FEATURE_LOADER_STAGE_5);

STATIC int mia_mng_init(void)
{
    (void)memset_s(mia_table, sizeof(mia_table), 0, sizeof(mia_table));
    mia_inst_num = 0;
    mutex_init(&g_mia_mng_mutex);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(mia_mng_init, FEATURE_LOADER_STAGE_5);

STATIC void mia_mng_uninit(void)
{
    int i;
    for (i = 0; i < ASCEND_DEV_MAX_NUM; ++i) {
        if (mia_table[i] != NULL) {
            kfree(mia_table[i]);
            mia_table[i] = NULL;
        }
    }
}
DECLAER_FEATURE_AUTO_UNINIT(mia_mng_uninit, FEATURE_LOADER_STAGE_5);
