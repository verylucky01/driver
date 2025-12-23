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

#include "ka_task_pub.h"
#include "pbl/pbl_uda.h"
#include "pbl_urd.h"
#include "pbl_urd_main_cmd_def.h"
#include "pbl_urd_sub_cmd_def.h"
#include "comm_kernel_interface.h"

#include "trs_pub_def.h"
#include "trs_core.h"
#include "soc_adapt.h"
#include "trs_chip_def_comm.h"
#ifdef CFG_FEATURE_TRS_SIA_ADAPT
#include "trs_sia_adapt_auto_init.h"
#elif defined(CFG_FEATURE_TRS_SEC_EH_ADAPT)
#include "trs_sec_eh_auto_init.h"
#endif
#include "trs_host_mode_config.h"

#define TRS_URD_CMD_NAME        "TRS_URD_CMD_NAME"
#define TRS_HOST_PHY_DEV_MAX    100U

static DECLARE_RWSEM(trs_mode_lock);
static int g_trs_sq_send_mode[TRS_HOST_PHY_DEV_MAX] = {TRS_MODE_TYPE_SQ_SEND_HIGH_PERFORMANCE, };
static bool g_trs_sq_send_mode_user_set_flag[TRS_HOST_PHY_DEV_MAX] = {false, };
static bool g_trs_mode_is_sia_flag[TRS_HOST_PHY_DEV_MAX] = {true, };

static void trs_sq_send_mode_to_xia(u32 chip_id, int mode)
{
    if ((devdrv_get_connect_protocol(chip_id) != CONNECT_PROTOCOL_PCIE) ||
        (trs_soc_get_chip_type(chip_id) != TRS_CHIP_TYPE_CLOUD_V4)) {
        return;
    }

    ka_task_down_write(&trs_mode_lock);
    if (g_trs_sq_send_mode_user_set_flag[chip_id] == true) {
        ka_task_up_write(&trs_mode_lock);
        return;
    }

    g_trs_sq_send_mode[chip_id] = mode;
    ka_task_up_write(&trs_mode_lock);
    trs_info("Tsdrv sq send mode to xia, set sq send mode success. (chip_id=%u; mode=%d)\n", chip_id, mode);
}

int trs_mode_config_to_sia(u32 chip_id)
{
    trs_sq_send_mode_to_xia(chip_id, TRS_MODE_TYPE_SQ_SEND_HIGH_PERFORMANCE);
    g_trs_mode_is_sia_flag[chip_id] = true;
    return 0;
}

int trs_mode_config_to_mia(u32 chip_id)
{
    trs_sq_send_mode_to_xia(chip_id, TRS_MODE_TYPE_SQ_SEND_HIGH_SECURITY);
    g_trs_mode_is_sia_flag[chip_id] = false;
    return 0;
}

static int trs_mode_config_check_scene(u32 chip_id)
{
    struct trs_id_inst pm_inst = {0};

    if (g_trs_mode_is_sia_flag[chip_id] == false) {
        trs_err("Vnpus scene, can not set mode.\n");
        return -EPERM;
    }

    trs_id_inst_pack(&pm_inst, chip_id, 0);
    if (trs_check_ts_inst_has_proc(&pm_inst)) {
        trs_err("Pf device still has proc, can not set mode. (chip_id=%u)\n", chip_id);
        return -EBUSY;
    }

    return 0;
}

static int trs_sq_send_mode_config_by_urd(struct trsModeInfo *info)
{
    int ret = 0;

    if ((info->mode < 0) || (info->mode >= TRS_MODE_TYPE_SQ_SEND_MAX) || (info->devId >= TRS_HOST_PHY_DEV_MAX)) {
        trs_err("The sq send mode or devid is invalid. (mode=%d; max=%d; devid=%u; max=%u)\n",
            info->mode, TRS_MODE_TYPE_SQ_SEND_MAX, info->devId, TRS_HOST_PHY_DEV_MAX);
        return -EINVAL;
    }

    if ((uda_is_phy_dev(info->devId) == false) || (uda_is_udevid_exist(info->devId) == false)) {
        trs_err("Not a valid phy dev. (devid=%u)\n", info->devId);
        return -ENODEV;
    }

    if ((devdrv_get_connect_protocol(info->devId) != CONNECT_PROTOCOL_PCIE) ||
        (trs_soc_get_chip_type(info->devId) != TRS_CHIP_TYPE_CLOUD_V4)) {
        return -EOPNOTSUPP;
    }

    ret = trs_mode_config_check_scene(info->devId);
    if (ret != 0) {
        return ret;
    }

    ka_task_down_write(&trs_mode_lock);
    g_trs_sq_send_mode[info->devId] = info->mode;
    g_trs_sq_send_mode_user_set_flag[info->devId] = true;
    ka_task_up_write(&trs_mode_lock);
    trs_info("Set sq send mode success. (chip_id=%u; mode=%d)\n", info->devId, info->mode);
    return 0;
}

int trs_mode_config_by_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    struct trsModeInfo *info = NULL;
    int ret = 0;

    if ((feature == NULL) || (in == NULL) || (in_len != sizeof(struct trsModeInfo))) {
        trs_err("Param invalid. (feature=%d; in=%d; in_len=%u)\n", feature != NULL, in != NULL, in_len);
        return -EINVAL;
    }

    info = (struct trsModeInfo *)in;
    if (info->mode_type >= TRS_MODE_TYPE_MAX) {
        trs_err("The mode_type is invalid. (mode_type=%d; msx=%d)\n", info->mode_type, TRS_MODE_TYPE_MAX);
        return -EINVAL;
    }

    if (info->mode_type == TRS_MODE_TYPE_SQ_SEND) {
        ret = trs_sq_send_mode_config_by_urd(info);
    }

    return ret;
}

/* default high perform mode, scene:
    * a) phy dev is default high perform mode
    * b) if user set by dsmi in phy, then docker or virtmachine(need extra set op) will inherit.
    * c) if user dsmi not setted, then set to high security mode when vnpu online.
    */
int trs_get_sq_send_mode(u32 udevid)
{
    struct uda_mia_dev_para mia_para = {0};

    if (uda_is_phy_dev(udevid) == false) {
        (void)uda_udevid_to_mia_devid(udevid, &mia_para);
    } else {
        mia_para.phy_devid = udevid;
    }

    return g_trs_sq_send_mode[mia_para.phy_devid];
}

static int trs_sq_send_mode_query_by_urd(u32 devid, int *mode)
{
    u32 udevid = 0;
    int ret = 0;

    ret = uda_devid_to_udevid(devid, &udevid);
    if (ret != 0) {
        return -ENODEV;
    }

    *mode = trs_get_sq_send_mode(udevid);
    return 0;
}

int trs_mode_query_by_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    struct trsModeInfo *in_info = NULL;
    int ret = 0;

    if ((feature == NULL) || (in == NULL) || (in_len != sizeof(struct trsModeInfo)) ||
        (out == NULL) || (out_len != sizeof(int))) {
        trs_err("Param invalid. (feature=%d; in=%d; in_len=%u; out=%d; out_len=%u)\n", feature != NULL,
            in != NULL, in_len, out != NULL, out_len);
        return -EINVAL;
    }

    in_info = (struct trsModeInfo *)in;
    if (in_info->mode_type >= TRS_MODE_TYPE_MAX) {
        trs_err("The mode_type is invalid. (mode_type=%d; msx=%d)\n", in_info->mode_type, TRS_MODE_TYPE_MAX);
        return -EINVAL;
    }

    if (in_info->mode_type == TRS_MODE_TYPE_SQ_SEND) {
        ret = trs_sq_send_mode_query_by_urd(in_info->devId, (int *)out);
    }

    return ret;
}

BEGIN_DMS_MODULE_DECLARATION(TRS_URD_CMD_NAME)
BEGIN_FEATURE_COMMAND()

ADD_FEATURE_COMMAND(TRS_URD_CMD_NAME, URD_TRS_MODE_CONFIG_CMD, ZERO_CMD, NULL, NULL,
                    DMS_ACC_ROOT | DMS_ENV_PHYSICAL | DMS_ENV_ADMIN_DOCKER | DMS_VDEV_NOTSUPPORT,
                    trs_mode_config_by_urd)

ADD_FEATURE_COMMAND(TRS_URD_CMD_NAME, URD_TRS_MODE_QUERY_CMD, ZERO_CMD, NULL, NULL,
                    DMS_SUPPORT_ALL,
                    trs_mode_query_by_urd)

END_FEATURE_COMMAND()
END_MODULE_DECLARATION()

int trs_host_mode_config_init(void)
{
    u32 i = 0;

    for (i = 0; i < TRS_HOST_PHY_DEV_MAX; i++) {
        g_trs_sq_send_mode[i] = TRS_MODE_TYPE_SQ_SEND_HIGH_PERFORMANCE;
        g_trs_sq_send_mode_user_set_flag[i] = false;
        g_trs_mode_is_sia_flag[i] = true;
    }

    CALL_INIT_MODULE(TRS_URD_CMD_NAME);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(trs_host_mode_config_init, FEATURE_LOADER_STAGE_2);

void trs_host_mode_config_uninit(void)
{
    CALL_EXIT_MODULE(TRS_URD_CMD_NAME);
}
DECLAER_FEATURE_AUTO_UNINIT(trs_host_mode_config_uninit, FEATURE_LOADER_STAGE_2);
