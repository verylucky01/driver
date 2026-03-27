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

#include "ka_fs_pub.h"
#include "ubcore_uapi.h"
#include "udma_ctl.h"
#include "ascend_ub_main.h"
#include "ascend_ub_mem_decoder.h"
#include "pbl/pbl_soc_res.h"
#include "pbl/pbl_uda.h"
#include "addr_trans.h"
#include "comm_kernel_interface.h"

int ubdrv_set_mem_decoder_info(struct ubdrv_ubmem_set_decoder_info *info)
{
    struct soc_reg_base_info io_base = {0};
    int ret;

    io_base.io_base = (phys_addr_t)info->uba;
    io_base.io_base_size = (size_t)info->max_share_mem_size;
    ret = soc_resmng_dev_set_reg_base(info->dev_id, UB_MEM_KEY_UB_BASE, &io_base);
    if (ret != 0) {
        ubdrv_err("Failed to set uba base to soc_res. (dev_id=%u;ret=%d)\n", info->dev_id, ret);
        return ret;
    }

    ret = soc_resmng_dev_set_key_value(info->dev_id, UB_MEM_KEY_UB_TID, info->tid);
    if (ret != 0) {
        ubdrv_err("Failed to set uba to soc_res. (dev_id=%u;ret=%d)\n", info->dev_id, ret);
        return ret;
    }

    ret = soc_resmng_dev_set_key_value(info->dev_id, UB_MEM_KEY_UB_MEM_ID, info->mem_id);
    if (ret != 0) {
        ubdrv_err("Failed to set uba to soc_res. (dev_id=%u;ret=%d)\n", info->dev_id, ret);
        return ret;
    }

    return 0;
}
