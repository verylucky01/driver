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

#ifndef TRS_ABNORMAL_INFO_H
#define TRS_ABNORMAL_INFO_H

#include "trs_h2d_msg.h"

typedef int (*tsmng_abnormal_proc_func)(u32 devid, u32 tsid, void *data, void *out);

int tsmng_register_abnormal_proc_func(tsmng_abnormal_proc_func func);
void tsmng_unregister_abnormal_proc_func(tsmng_abnormal_proc_func func);

#endif
