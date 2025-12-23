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

#include "trs_core.h"
#include "trs_stars_comm.h"
#include "shr_id.h"
#include "id_pool.h"
#include "trs_tsmng.h"
#include "trs_pm_adapt.h"
#include "trs_mia_adapt.h"
#include "trs_sia_agent.h"
#include "trs_mia_agent.h"
#include "trs_sec_eh_adapt.h"
#include "trs_sec_eh_agent.h"

struct submodule_ops {
    int (*init) (void);
    void (*uninit)(void);
};

static struct submodule_ops g_sub_table[] = {
#ifdef CFG_FEATURE_TRS_CORE
    {trs_core_init_module, trs_core_exit_module},
#endif
#ifdef CFG_FEATURE_TRS_STARS
    {init_trs_stars, exit_trs_stars},
#endif
#ifdef CFG_FEATURE_TRS_ID_POOL
    {id_pool_setup_init, id_pool_setup_uninit},
#endif
#ifdef CFG_FEATURE_TRS_TSMNG
    {tsmng_init, tsmng_exit},
#endif
#ifdef CFG_FEATURE_TRS_SHR_ID
    {shr_id_init_module, shr_id_exit_module},
#endif
#ifdef CFG_FEATURE_TRS_SIA_ADAPT
    {init_trs_adapt, exit_trs_adapt},
#endif
#ifdef CFG_FEATURE_TRS_MIA_ADAPT
    {trs_mia_device_adapt_init, trs_mia_device_adapt_exit},
#endif
#ifdef CFG_FEATURE_TRS_SIA_AGENT
    {init_trs_agent, exit_trs_agent},
#endif
#ifdef CFG_FEATURE_TRS_MIA_AGENT
    {trs_mia_agent_init, trs_mia_agent_exit},
#endif
#ifdef CFG_FEATURE_TRS_SEC_EH_ADAPT
    {init_trs_sec_eh, exit_trs_sec_eh},
#endif
#ifdef CFG_FEATURE_TRS_SEC_EH_AGENT
    {init_sec_eh_trs_agent, exit_sec_eh_trs_agent},
#endif
};

static int __init init_trs(void)
{
    int index, ret;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);
 
    for (index = 0; index < table_size; index++) {
        ret = g_sub_table[index].init();
        if  (ret != 0) {
            goto out;
        }
    }
    return 0;
 out:
    for (; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
    return ret;
}

static void __exit exit_trs(void)
{
    int index;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);
 
    for (index = table_size; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
}

ka_module_init(init_trs);
ka_module_exit(exit_trs);

KA_MODULE_LICENSE("GPL");
KA_MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
KA_MODULE_DESCRIPTION("TRS");
