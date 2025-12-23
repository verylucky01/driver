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

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/types.h>

#include "pbl/pbl_runenv_config.h"
#include "docker_query.h"
#include "vm_query.h"
#include "runenv_config_module.h"

int recfg_init(void);
void recfg_exit(void);

/* The name cannot be changed because it is exposed to the user. */
static u32 deployment_mode = DBL_DEVICE_DEPLOYMENT;
module_param(deployment_mode, uint, S_IRUSR);

u32 dbl_get_deployment_mode(void)
{
    return deployment_mode;
}
EXPORT_SYMBOL(dbl_get_deployment_mode);

int dbl_get_run_env(void)
{
    int mach_flag = DBL_IN_PHYSICAL_MACH;

    if (run_in_normal_docker() == true) {
        mach_flag = DBL_IN_NORMAL_DOCKER;
    } else if (run_in_admin_docker() == true) {
        mach_flag = DBL_IN_ADMIN_DOCKER;
    } else if (run_in_virtual_mach() == true) {
        mach_flag = DBL_IN_VIRTUAL_MACH;
    }

    return mach_flag;
}
EXPORT_SYMBOL(dbl_get_run_env);

int recfg_init(void)
{
#ifdef CFG_FEATURE_HOST_ENV
    deployment_mode = DBL_HOST_DEPLOYMENT;
#endif
    recfg_info("Module init success.\n");
    return 0;
}

void recfg_exit(void)
{
    recfg_info("Module exit success.\n");
}
