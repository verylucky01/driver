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

#ifndef TOPIC_SCHED_V1_H
#define TOPIC_SCHED_V1_H

#include <asm/io.h>

#include "ascend_hal_define.h"
#include "esched.h"

void topic_sched_host_aicpu_intr_mask_set_v1(void __iomem *io_base, u32 mask_index, u32 vf_id, u32 val);
void topic_sched_host_ctrlcpu_intr_mask_set_v1(void __iomem *io_base, u32 vf_id, u32 val);
bool topic_sched_host_ccpu_is_mb_valid_v1(const void __iomem *io_base, u32 mb_id, u32 vf_id);
bool topic_sched_host_aicpu_is_mb_valid_v1(const void __iomem *io_base, u32 mb_id, u32 vf_id);
void topic_sched_host_aicpu_intr_clr_v1(void __iomem *io_base, u32 intr_index, u32 vf_id, u32 val);
void topic_sched_host_ccpu_intr_clr_v1(void __iomem *io_base, u32 vf_id, u32 val);
void topic_sched_host_aicpu_intr_enable_v1(void __iomem *io_base, u32 cpu_index, u32 vf_id);
void topic_sched_host_ctrlcpu_intr_enable_v1(void __iomem *io_base, u32 cpu_index, u32 vf_id);
void topic_sched_host_aicpu_int_all_status_v1(const void __iomem *io_base, u32 *val, u32 vf_id);
void topic_sched_host_aicpu_intr_all_clr_v1(void __iomem *io_base, u32 val, u32 vf_id);
void topic_sched_host_aicpu_int_status_v1(const void __iomem *io_base, u32 intr_index, u32 *val, u32 vf_id);
void topic_sched_host_ccpu_int_status_v1(const void __iomem *io_base, u32 *val, u32 vf_id);
void esched_drv_cpu_report_v1(struct topic_data_chan *topic_chan, u32 vf_id, u32 error_code, u32 status);
int esched_drv_host_map_addr_v1(u32 dev_id, struct sched_hard_res *res);
void esched_host_iounmap_v1(struct sched_hard_res *res);

#endif
