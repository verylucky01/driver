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
#include "ascend_hal_define.h"
#include "pbl/pbl_uda.h"
#include "dms_template.h"
#include "dms/dms_cmd_def.h"
#include "urd_acc_ctrl.h"
#include "urd_feature.h"
#ifdef CFG_FEATURE_VM_ADAPT
#include "trs_sec_eh_auto_init.h"
#else
#include "trs_sia_adapt_auto_init.h"
#endif
#include "trs_core.h"
#include "trs_mailbox_def.h"
#include "trs_host_rpc.h"

static int trs_urd_rpc_msg_ctrl(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    struct dms_get_device_info_in *in_cfg = (struct dms_get_device_info_in *)in;
    struct dms_get_device_info_out *cfg_out = (struct dms_get_device_info_out *)out;
    struct trs_rpc_call_msg rpc_call_msg = {0};
    struct trs_id_inst inst;
    u32 udevid, vf_id;
    u8 usr_data[TRS_RPC_MAX_DATA_LEN];
    int ret;

    if ((feature == NULL) || (in == NULL) || (in_len != sizeof(struct dms_get_device_info_in))) {
        trs_err("Param invalid. (feature=%d; in=%d; in_len=%u)\n",
            feature != NULL, in != NULL, in_len);
        return -EINVAL;
    }

    if (in_cfg->buff_size > TRS_RPC_MAX_DATA_LEN) {
        trs_err("Invalid size. (buff_size=%u)\n", in_cfg->buff_size);
        return -EINVAL;
    }

    ret = (int)ka_base_copy_from_user(usr_data, in_cfg->buff, in_cfg->buff_size);
    if (ret != 0) {
        trs_err("Copy_from_user failed. (ret=%d; buff_size=%u)\n", ret, in_cfg->buff_size);
        return ret;
    }

    ret = uda_devid_to_phy_devid(in_cfg->dev_id, &udevid, &vf_id);
    if (ret != 0) {
        trs_err("Failed to transfer logical ID to physical ID. (logic_id=%u; ret=%d)\n", in_cfg->dev_id, ret);
        return ret;
    }
    trs_debug("Convert udevid. (logic_id=%u; udevid=%u)\n", in_cfg->dev_id, udevid);

    trs_id_inst_pack(&inst, udevid, 0);
    rpc_call_msg.header.cmd_type = TRS_MBOX_DSMI_RPC_CALL;
    ret = trs_rpc_msg_ctrl(&inst, ka_task_get_current_tgid(), usr_data, in_cfg->buff_size, &rpc_call_msg);
    if (ret != 0) {
        trs_err("Failed to send rpc msg. (ret=%d)\n", ret);
        return ret;
    }

    if (cfg_out != NULL) {
        cfg_out->out_size = rpc_call_msg.rpc_call_header.len;
        if (cfg_out->out_size > in_cfg->buff_size) {
            trs_err("Invalid out_size. (out_size=%u; buff_size=%u)\n", cfg_out->out_size, in_cfg->buff_size);
            return -EINVAL;
        }
        ret = ka_base_copy_to_user(in_cfg->buff, rpc_call_msg.data, cfg_out->out_size);
        if (ret != 0) {
            trs_err("Failed to memcpy. (out_len=%u; msg_len=%u)\n", out_len, cfg_out->out_size);
            return -EFAULT;
        }
        trs_debug("Output. (udevid=%u; msg_len=%u)\n", udevid, cfg_out->out_size);
    }

    trs_debug("Urd ctrl msg success. (udevid=%u; msg_len=%u)\n", udevid, in_cfg->buff_size);
    return 0;
}

int trs_urd_rpc_msg_set(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    int ret;

    ret = trs_urd_rpc_msg_ctrl(feature, in, in_len, NULL, out_len);
    if (ret != 0) {
        trs_err("Trs urd rpc msg set failed. (ret=%d)\n", ret);
        return ret;
    }
    trs_debug("Trs urd rpc msg set success. (in_len=%u)\n", in_len);
    return 0;
}

int trs_urd_rpc_msg_get(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    int ret;

    if ((out == NULL) || (out_len != sizeof(struct dms_get_device_info_out))) {
        trs_err("Param invalid. (out=%d; in_len=%u)\n", out != NULL, out_len);
        return -EINVAL;
    }

    ret = trs_urd_rpc_msg_ctrl(feature, in, in_len, out, out_len);
    if (ret != 0) {
        trs_err("Trs urd rpc msg get failed. (ret=%d)\n", ret);
        return ret;
    }
    trs_debug("Trs urd rpc msg get success.\n");
    return 0;
}

#define DMS_MODULE_TRS_RPC_CTRL "trs_rpc_ctrl"
BEGIN_DMS_MODULE_DECLARATION(DMS_MODULE_TRS_RPC_CTRL)
BEGIN_FEATURE_COMMAND()
ADD_FEATURE_COMMAND(DMS_MODULE_TRS_RPC_CTRL,
    DMS_GET_SET_DEVICE_INFO_CMD,
    ZERO_CMD,
    "main_cmd=0xb,sub_cmd=0xb",
    NULL,
    DMS_ACC_ROOT_ONLY | DMS_ENV_NOT_NORMAL_DOCKER | DMS_VDEV_NOTSUPPORT,
    trs_urd_rpc_msg_set)
ADD_FEATURE_COMMAND(DMS_MODULE_TRS_RPC_CTRL,
    DMS_GET_GET_DEVICE_INFO_CMD,
    ZERO_CMD,
    "main_cmd=0xb,sub_cmd=0xb",
    NULL,
    DMS_ACC_ALL | DMS_ENV_ALL | DMS_VDEV_NOTSUPPORT,
    trs_urd_rpc_msg_get)
END_FEATURE_COMMAND()
END_MODULE_DECLARATION()

int trs_urd_rpc_msg_ctrl_init(void)
{
    CALL_INIT_MODULE(DMS_MODULE_TRS_RPC_CTRL);
    trs_info("Init urd rpc msg ctrl.\n");
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(trs_urd_rpc_msg_ctrl_init, FEATURE_LOADER_STAGE_6);

void trs_urd_rpc_msg_ctrl_uninit(void)
{
    CALL_EXIT_MODULE(DMS_MODULE_TRS_RPC_CTRL);
    trs_info("Uninit urd rpc msg ctrl.\n");
}
DECLAER_FEATURE_AUTO_UNINIT(trs_urd_rpc_msg_ctrl_uninit, FEATURE_LOADER_STAGE_6);
