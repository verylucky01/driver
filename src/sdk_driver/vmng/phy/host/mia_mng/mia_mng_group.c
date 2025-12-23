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

#include "linux/uaccess.h"
#include "pbl/pbl_feature_loader.h"
#include "pbl/pbl_uda.h"
#include "pbl/pbl_urd.h"
#include "pbl/pbl_urd_main_cmd_def.h"
#include "pbl/pbl_urd_sub_cmd_def.h"
#include "devdrv_user_common.h"

#include "mia_mng_ctrl.h"
#include "mia_mng_group.h"

static void mia_copy_soc_mia_res_info_ex_by_die(struct soc_mia_res_info_ex *self, struct soc_mia_res_info_ex *from)
{
    u32 die_idx;
    for (die_idx = 0; die_idx < MAX_DIE_NUM_PER_DEV; die_idx++) {
        self[die_idx] = from[die_idx];
    }
}

static int mia_mng_soc_add_group(u32 udevid, struct vmng_group_info *info)
{
    struct soc_mia_grp_info grp_info = {0};
    u32 group_id = info->group_id;
    int ret;

    grp_info.valid = MIA_GROUP_VALID;
    grp_info.vfid = info->vf_id - 1;
    grp_info.pool_id = info->vfg_id;
    grp_info.poolid_max = info->vf_id - 1;
    mia_copy_soc_mia_res_info_ex_by_die(grp_info.aic_info, (struct soc_mia_res_info_ex *)info->soc_aic);
    mia_copy_soc_mia_res_info_ex_by_die(grp_info.aiv_info, (struct soc_mia_res_info_ex *)info->soc_aiv);
    memcpy_s(&grp_info.rtsq_info, sizeof(struct soc_mia_res_info_ex), &info->soc_rtsq,
            sizeof(struct soc_mia_res_info_ex));
    memcpy_s(&grp_info.notify_info, sizeof(struct soc_mia_res_info_ex), &info->soc_notifyid,
            sizeof(struct soc_mia_res_info_ex));

    ret = soc_resmng_dev_set_mia_grp_info(udevid, group_id, &grp_info);
    if (ret != 0) {
        vmng_err("Add group to soc res failed. (udevid=%u;group_id=%u;ret=%d)\n", udevid, group_id, ret);
        return ret;
    }
    return 0;
}

static int mia_mng_soc_del_group(u32 udevid, u32 group_id)
{
    struct soc_mia_grp_info grp_info = {0};
    int ret;

    grp_info.valid = MIA_GROUP_INVALID;
    ret = soc_resmng_dev_set_mia_grp_info(udevid, group_id, &grp_info);
    if (ret != 0) {
        vmng_err("Del group to soc res failed. (udevid=%u;group_id=%u;ret=%d)\n", udevid, group_id, ret);
        return ret;
    }
    return 0;
}

static int mia_mng_create_group_pre_proc(char *in, u32 in_len, struct vmng_group_info *info)
{
    common_status_info_t *status_info = NULL;
    struct ts_group_info ts_group = {0};
    int ret;

    if ((in == NULL) || (in_len != sizeof(common_status_info_t))) {
        vmng_err("input arg is NULL, or in_len is wrong. (in_len=%u)\n", in_len);
        return -EINVAL;
    }
    status_info = (common_status_info_t *)in;
    if (status_info->commoninfo_len < sizeof(ts_group)) {
        vmng_err("Input len is wrong. (group_size=%lu, info_size=%u)\n",
            sizeof(ts_group), status_info->commoninfo_len);
        return -EINVAL;
    }
    ret = (int)copy_from_user(&ts_group, status_info->commoninfo, sizeof(ts_group));
    if (ret != 0) {
        vmng_err("Copy from user failed. (ret=%d)\n", ret);
        return ret;
    }
    ret = uda_devid_to_udevid(status_info->dev_id, &info->dev_id);
    if (ret != 0) {
        vmng_err("Get udevid failed. (devid=%u;ret=%d)\n", status_info->dev_id, ret);
        return ret;
    }
    info->group_id = (u32)(ts_group.group_id);
    info->aic = (u32)(ts_group.aicore_number);
    if (info->group_id >= MAX_MIA_GROUP_NUM) {
        vmng_err("Invalid group id. (devid=%u;group_id=%u)\n", info->dev_id, info->group_id);
        return -EINVAL;
    }
    return 0;
}

int mia_mng_create_group(u32 udevid, struct vmng_ctrl_msg *msg)
{
    struct mia_ctrl_info *mia_msg = &msg->mia_msg;
    int ret;

    ret = mia_mng_msg_send(msg, MIA_MNG_MSG_CREATE_GROUP);
    if (ret != 0) {
        vmng_err("Mia send msg failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = mia_mng_soc_add_group(udevid, &mia_msg->group_info);
    if (ret != 0) {
        (void)mia_mng_msg_send(msg, MIA_MNG_MSG_DELETE_GROUP);
        return ret;
    }

    ret = uda_dev_ctrl_ex(udevid, UDA_CTRL_ADD_GROUP, mia_msg->group_info.group_id);
    if (ret != 0) {
        (void)mia_mng_soc_del_group(udevid, mia_msg->group_info.group_id);
        (void)mia_mng_msg_send(msg, MIA_MNG_MSG_DELETE_GROUP);
        return ret;
    }
    return 0;
}


STATIC int mia_mng_urd_create_group(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    struct mia_mng_inst *mia_inst;
    struct vmng_ctrl_msg msg = {0};
    u32 udevid = 0;
    u32 group_id;
    int ret;

    ret = mia_mng_create_group_pre_proc(in, in_len, &msg.mia_msg.group_info);
    if (ret != 0) {
        vmng_err("Pre proc failed. (ret=%d)\n", ret);
        return -EINVAL;
    }

    udevid = msg.mia_msg.group_info.dev_id;
    group_id = msg.mia_msg.group_info.group_id;
    msg.mia_msg.dev_id = udevid;
    mia_inst = mia_mng_get_inst(udevid);
    if (mia_inst == NULL) {
        vmng_err("Get mia inst failed. (udevid=%u,group_id=%u)\n", udevid, group_id);
        return -EINVAL;
    }

    mutex_lock(&g_mia_mng_mutex);
    if (mia_mng_group_exist(udevid, group_id)) {
        vmng_err("Mia group has created. (devid=%u;group_id=%u)\n", udevid, group_id);
        mutex_unlock(&g_mia_mng_mutex);
        return -EINVAL;
    }

    ret = mia_mng_create_group(udevid, &msg);
    if (ret != 0) {
        vmng_err("Mia create group failed. (udevid=%u;ret=%d)\n", udevid, ret);
        mutex_unlock(&g_mia_mng_mutex);
        return -EINVAL;
    }
    mia_mng_set_group_status(udevid, group_id, MIA_GROUP_CREATE);
    mutex_unlock(&g_mia_mng_mutex);
    return 0;
}

static int mia_mng_delete_group_pre_proc(char *in, u32 in_len, struct vmng_group_info *info)
{
    common_status_info_t *status_info = NULL;
    struct delete_ts_group_info del_info = {0};
    int ret;

    if ((in == NULL) || (in_len != sizeof(common_status_info_t))) {
        vmng_err("input arg is NULL, or in_len is wrong. (in_len=%u)\n", in_len);
        return -EINVAL;
    }

    status_info = (common_status_info_t *)in;
    if (status_info->commoninfo_len < sizeof(del_info)) {
        vmng_err("Input len is wrong. (group_size=%lu, info_size=%u)\n",
            sizeof(del_info), status_info->commoninfo_len);
        return -EINVAL;
    }

    ret = (int)copy_from_user(&del_info, status_info->commoninfo, sizeof(del_info));
    if (ret != 0) {
        vmng_err("Copy from user failed. (ret=%d)\n", ret);
        return ret;
    }
    ret = uda_devid_to_udevid(status_info->dev_id, &info->dev_id);
    if (ret != 0) {
        vmng_err("Get udevid failed. (devid=%u;ret=%d)\n", status_info->dev_id, ret);
        return ret;
    }
    info->group_id = (u32)(del_info.group_id);
    if (info->group_id >= MAX_MIA_GROUP_NUM) {
        vmng_err("Invalid group id. (devid=%u;group_id=%u)\n", info->dev_id, info->group_id);
        return -EINVAL;
    }
    return 0;
}

int mia_mng_delete_group(u32 udevid, struct vmng_ctrl_msg *msg)
{
    struct mia_ctrl_info *mia_msg = &msg->mia_msg;
    int ret;

    ret = uda_dev_ctrl_ex(udevid, UDA_CTRL_DEL_GROUP, mia_msg->group_info.group_id);
    if (ret != 0) {
        vmng_err("Del mia group failed. (devid=%u;ret=%d)\n", udevid, ret);
        return ret;
    }

    ret = mia_mng_soc_del_group(udevid, mia_msg->group_info.group_id);
    if (ret != 0) {
        vmng_err("Del soc mia group failed. (devid=%u;ret=%d)\n", udevid, ret);
        (void)uda_dev_ctrl_ex(udevid, UDA_CTRL_ADD_GROUP, mia_msg->group_info.group_id);
        return ret;
    }

    ret = mia_mng_msg_send(msg, MIA_MNG_MSG_DELETE_GROUP);
    if (ret != 0) {
        /* if device delete group failed, no need to rollback */
        vmng_err("Mia send msg failed. (devid=%u;ret=%d)\n", udevid, ret);
    }

    return 0;
}

STATIC int mia_mng_urd_delete_group(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    struct mia_mng_inst *mia_inst;
    struct vmng_ctrl_msg msg = {0};
    u32 udevid = 0;
    u32 group_id;
    int ret;

    ret = mia_mng_delete_group_pre_proc(in, in_len, &msg.mia_msg.group_info);
    if (ret != 0) {
        vmng_err("Pre proc failed. (ret=%d)\n", ret);
        return -EINVAL;
    }
    udevid = msg.mia_msg.group_info.dev_id;
    group_id = msg.mia_msg.group_info.group_id;
    msg.mia_msg.dev_id = udevid;
    mia_inst = mia_mng_get_inst(udevid);
    if (mia_inst == NULL) {
        vmng_err("Get mia inst failed. (udevid=%u,group_id=%u)\n", udevid, group_id);
        return -EINVAL;
    }

    mutex_lock(&g_mia_mng_mutex);
    if (!mia_mng_group_exist(udevid, group_id)) {
        vmng_err("Mia group has deleted. (devid=%u;group_id=%u)\n", udevid, group_id);
        mutex_unlock(&g_mia_mng_mutex);
        return -EINVAL;
    }

    ret = mia_mng_delete_group(udevid, &msg);
    if (ret != 0) {
        vmng_err("Mia delete group failed. (udevid=%u;ret=%d)\n", udevid, ret);
        mutex_unlock(&g_mia_mng_mutex);
        return -EINVAL;
    }
    mia_mng_set_group_status(udevid, group_id, MIA_GROUP_DELETE);
    mutex_unlock(&g_mia_mng_mutex);
    return 0;
}

BEGIN_DMS_MODULE_DECLARATION(MIA_MODULE_NAME)
BEGIN_FEATURE_COMMAND()
ADD_FEATURE_COMMAND(MIA_MODULE_NAME,
    DMS_MAIN_CMD_CAPA_GROUP,
    DMS_SUBCMD_CREATE_CAPA_GROUP,
    NULL,
    NULL,
#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA)
    DMS_SUPPORT_ALL,
#else
    DMS_ACC_ROOT_ONLY | DMS_VDEV_NOT_PHYSICAL | DMS_ENV_VIRTUAL | DMS_ENV_DOCKER,
#endif

    mia_mng_urd_create_group)
ADD_FEATURE_COMMAND(MIA_MODULE_NAME,
    DMS_MAIN_CMD_CAPA_GROUP,
    DMS_SUBCMD_DEL_CAPA_GROUP,
    NULL,
    NULL,
#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA)
    DMS_SUPPORT_ALL,
#else
    DMS_ACC_ROOT_ONLY | DMS_VDEV_NOT_PHYSICAL | DMS_ENV_VIRTUAL | DMS_ENV_DOCKER,
#endif
    mia_mng_urd_delete_group)
END_FEATURE_COMMAND()
END_MODULE_DECLARATION()

int mia_register_urd(void)
{
    CALL_INIT_MODULE(MIA_MODULE_NAME);
    return 0;
}

void mia_unregister_urd(void)
{
    CALL_EXIT_MODULE(MIA_MODULE_NAME);
}

DECLAER_FEATURE_AUTO_INIT(mia_register_urd, FEATURE_LOADER_STAGE_5);
DECLAER_FEATURE_AUTO_UNINIT(mia_unregister_urd, FEATURE_LOADER_STAGE_5);