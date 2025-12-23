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

#ifndef __TRS_SHR_PROC_H__
#define __TRS_SHR_PROC_H__

bool trs_proc_cp2_type_check(struct trs_core_ts_inst *ts_inst);
int trs_shr_proc_get_share_pid(struct trs_core_ts_inst *ts_inst, u32 *share_pid);
int trs_shr_proc_open(ka_file_t *file, struct trs_core_ts_inst *ts_inst, struct trs_task_info_struct *task_info);
int trs_shr_proc_close(struct trs_task_info_struct *task_info);
bool trs_shr_proc_check(struct trs_task_info_struct *task_info);
bool trs_shr_proc_support_cmd_check(unsigned int cmd);
bool trs_proc_support_cmd_check(struct trs_task_info_struct *task_info, unsigned int cmd);

#endif
