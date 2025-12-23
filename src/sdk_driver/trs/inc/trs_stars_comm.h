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

#ifndef TRS_STARS_COMM_H
#define TRS_STARS_COMM_H
#include "ascend_kernel_hal.h"
/* TRS STARS res id ctrl cmd */
enum {
    TRS_STARS_RES_OP_RESET = 0,
    TRS_STARS_RES_OP_RECORD,
    TRS_STARS_RES_OP_ENABLE,
    TRS_STARS_RES_OP_DISABLE,
    TRS_STARS_RES_OP_CHECK_AND_RESET,
    TRS_STARS_RES_OP_MAX,
};

int trs_stars_soc_res_ctrl(struct trs_id_inst *inst, u32 type, u32 id, u32 cmd);

typedef struct tsmng_stars_info {
    void *data;
    u32 data_len;
} tsmng_stars_info_t;

typedef int (*stars_msg_send)(u32 devid, u32 cmd, struct tsmng_stars_info *in, struct tsmng_stars_info *out);
void tsmng_stars_msg_send_register(stars_msg_send handle);
void tsmng_stars_msg_send_unregister(void);
stars_msg_send tsmng_get_stars_msg_send_handle(void);
int tsmng_stars_msg_proc(u32 devid, u32 cmd, struct tsmng_stars_info *in, struct tsmng_stars_info *out);

int trs_ts_cq_process(u32 udevid, u32 tsid, int cq_type, u32 cqid, u32 pos);

int init_trs_stars(void);
void exit_trs_stars(void);
#endif
