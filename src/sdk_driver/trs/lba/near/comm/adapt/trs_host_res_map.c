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
#include "ka_kernel_def_pub.h"

#include "ascend_hal_define.h"
#include "dpa_kernel_interface.h"
#include "trs_core_near_ops.h"
#ifdef CFG_FEATURE_TRS_SIA_ADAPT
#include "trs_sia_adapt_auto_init.h"
#elif defined(CFG_FEATURE_TRS_SEC_EH_ADAPT)
#include "trs_sec_eh_auto_init.h"
#endif

struct apm_res_map_ops g_res_map_ops = {
    .owner = KA_THIS_MODULE,
    .res_is_belong_to_proc = trs_host_res_is_belong_to_proc,
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
