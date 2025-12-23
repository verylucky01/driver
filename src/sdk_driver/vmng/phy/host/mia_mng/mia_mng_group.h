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
#ifndef _MIA_MNG_GROUP_H
#define _MIA_MNG_GROUP_H

#include "urd_define.h"
#include "urd_feature.h"
#include "urd_acc_ctrl.h"
#include "virtmng_public_def.h"

#define MIA_MODULE_NAME "MIA_HOST"

extern struct mutex g_mia_mng_mutex;

int init_module_MIA_MODULE_NAME(void);
int exit_module_MIA_MODULE_NAME(void);

int mia_register_urd(void);
void mia_unregister_urd(void);
int mia_mng_create_group(u32 udevid, struct vmng_ctrl_msg *mia_msg);
int mia_mng_delete_group(u32 udevid, struct vmng_ctrl_msg *mia_msg);

#endif // _MIA_MNG_GROUP_H