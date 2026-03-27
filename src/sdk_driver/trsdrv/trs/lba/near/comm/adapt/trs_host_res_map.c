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
#include "ka_kernel_def_pub.h"

#include "ascend_hal_define.h"
#include "dpa_kernel_interface.h"
#include "pbl/pbl_uda.h"
#include "trs_core_near_ops.h"
#ifdef CFG_FEATURE_TRS_SIA_ADAPT
#include "trs_sia_adapt_auto_init.h"
#elif defined(CFG_FEATURE_TRS_SEC_EH_ADAPT)
#include "trs_sec_eh_auto_init.h"
#endif

int trs_res_map_ops_init(void);
void trs_res_map_ops_uninit(void);

static int trs_host_update_res_info(u32 udevid, struct res_map_info_in *res_info)
{
    if (res_info->priv != NULL) {
        struct trs_res_map_priv *priv = (struct trs_res_map_priv *)res_info->priv;
        u32 local_add_id, remote_add_id;
        int ret;

        if ((priv->flag & TSDRV_FLAG_REMOTE_ID) != 0) {
            ret = uda_udevid_to_add_id(udevid, &local_add_id);
            if (ret != 0) {
                trs_err("Fail to get local add id. (ret=%d; udevid=%u)\n", ret, udevid);
                return ret;
            }

            ret = uda_udevid_to_add_id(priv->remote_devid, &remote_add_id);
            if (ret != 0) {
                trs_err("Fail to get remote add id. (ret=%d; remote_udevid=%u)\n", ret, priv->remote_devid);
                return ret;
            }
            trs_debug("Update priv. (udevid=%u; local_add_id=%u; remote_udevid=%u; remote_add_id=%u)\n",
                udevid, local_add_id, priv->remote_devid, remote_add_id);
            priv->local_devid = local_add_id;
            priv->remote_devid = remote_add_id;
            res_info->res_id = (remote_add_id << 20) | res_info->res_id; /* first 20 bits for res id */
        }
    }
    return 0;
}

struct apm_res_map_ops g_res_map_ops = {
    .owner = KA_THIS_MODULE,
    .res_is_belong_to_proc = trs_host_res_is_belong_to_proc,
    .update_res_info = trs_host_update_res_info,
};

int trs_res_map_ops_init(void)
{
    int ret;

    ret = hal_kernel_apm_res_map_ops_register(RES_ADDR_TYPE_STARS_NOTIFY_RECORD,
        &g_res_map_ops);
    if (ret != 0) {
        trs_err("Failed to register notify res map ops.\n");
        return ret;
    }

    ret = hal_kernel_apm_res_map_ops_register(RES_ADDR_TYPE_STARS_CNT_NOTIFY_RECORD,
        &g_res_map_ops);
    if (ret != 0) {
        hal_kernel_apm_res_map_ops_unregister(RES_ADDR_TYPE_STARS_NOTIFY_RECORD);
        trs_err("Failed to register cnt notify res map ops.\n");
        return ret;
    }

    trs_info("Init res map ops success.\n");
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(trs_res_map_ops_init, FEATURE_LOADER_STAGE_0);

void trs_res_map_ops_uninit(void)
{
    hal_kernel_apm_res_map_ops_unregister(RES_ADDR_TYPE_STARS_CNT_NOTIFY_RECORD);
    hal_kernel_apm_res_map_ops_unregister(RES_ADDR_TYPE_STARS_NOTIFY_RECORD);
}
DECLAER_FEATURE_AUTO_UNINIT(trs_res_map_ops_uninit, FEATURE_LOADER_STAGE_0);
