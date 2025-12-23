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
#ifndef TRS_UB_INFO_H
#define TRS_UB_INFO_H

#include <linux/types.h>

struct trs_ub_info {
    u32 die_id;
    u32 func_id;
};

int trs_ub_info_query(struct trs_id_inst *inst, u32 *die_id, u32 *func_id);
void trs_ub_info_set(u32 devid, struct trs_ub_info *ub_info);

#endif /* TRS_UB_INFO_H */

