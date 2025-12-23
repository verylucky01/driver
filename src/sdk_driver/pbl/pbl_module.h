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
#ifndef PBL_MODULE_H
#define PBL_MODULE_H

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

int ka_module_init(void);
void ka_module_exit(void);
int drv_ascend_intf_init(void);
void drv_davinci_intf_exit(void);
int uda_init_module(void);
void uda_exit_module(void);
int resmng_init_module(void);
void resmng_exit_module(void);
int recfg_init(void);
void recfg_exit(void);
int urd_init(void);
void urd_exit(void);
int prof_framework_init(void);
void prof_framework_exit(void);
int ascend_ctl_init(void);
void ascend_ctl_exit(void);

#ifdef CFG_ENV_HOST
int log_drv_module_init(void);
void log_drv_module_exit(void);
#ifndef CFG_FEATURE_KO_ALONE_COMPILE
int devdrv_base_comm_init(void);

void devdrv_base_comm_exit(void);
#endif
#else
int dev_user_cfg_module_init(void);
int dfm_init(void);
int bdcfg_init(void);
int ccfg_init(void);
int ipcdrv_pbl_init_module(void);

void dev_user_cfg_module_exit(void);
void dfm_exit(void);
void bdcfg_exit(void);
void ccfg_exit(void);
void ipcdrv_pbl_exit_module(void);

#ifndef CFG_FEATURE_KO_ALONE_COMPILE
int icmdrv_pbl_init_module(void);
int pkicms_dev_init(void);
int devdrv_base_comm_init(void);

void icmdrv_pbl_exit_module(void);
void pkicms_dev_exit(void);
void devdrv_base_comm_exit(void);
#endif
#endif /* CFG_ENV_HOST */

#endif