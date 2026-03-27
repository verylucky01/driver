/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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
#ifndef TRS_HOST_SOFT_MBOX__
#define TRS_HOST_SOFT_MBOX__

int trs_soft_mbox_send(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);
int trs_soft_mbox_init(u32 ts_inst_id);
void trs_soft_mbox_uninit(u32 ts_inst_id);

#endif /* TRS_HOST_SOFT_MBOX__ */
