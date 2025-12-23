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
#ifndef TRS_CORE_STARS_V2_OPS_H
#define TRS_CORE_STARS_V2_OPS_H

#include <linux/types.h>

#include "trs_pub_def.h"
#include "trs_core.h"

#define TRS_SUPPORT_PROC_NUM 0U

struct trs_core_adapt_ops *trs_core_get_stars_v2_adapt_ops(void);
int trs_core_stars_v2_ops_get_sq_id_head_from_hw_cqe(struct trs_id_inst *inst, void *hw_cqe, u32 *sqid, u32 *sq_head);

int trs_core_stars_v2_ops_get_stream_from_cqe(struct trs_id_inst *inst, void *hw_cqe, u32 *stream_id);
int trs_core_stars_v2_ops_cqe_to_logic_cqe(struct trs_id_inst *inst, void *hw_cqe, struct trs_logic_cqe *logic_cqe);
#endif /* TRS_CORE_STARS_V2_OPS_H */

