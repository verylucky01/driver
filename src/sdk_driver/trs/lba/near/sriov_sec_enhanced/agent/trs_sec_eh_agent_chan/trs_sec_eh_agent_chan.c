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

#include "trs_sqe_update.h"
#include "trs_sec_eh_agent_chan.h"

int trs_agent_chan_config(struct trs_id_inst *inst)
{
    int ret;

#ifndef EMU_ST
    ret = trs_sqe_update_init(inst->devid);
    if (ret != 0) {
        return ret;
    }
#endif

    return 0;
}

void trs_agent_chan_deconfig(struct trs_id_inst *inst)
{
#ifndef EMU_ST
    trs_sqe_update_uninit(inst->devid);
#endif
}
