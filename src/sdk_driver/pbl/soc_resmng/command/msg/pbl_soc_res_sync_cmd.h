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

#ifndef PBL_SOC_RES_SYNC_CMD_H
#define PBL_SOC_RES_SYNC_CMD_H

enum soc_res_sync_dir {
    SOC_DIR_P2V,
    SOC_DIR_D2H,
    SOC_DIR_MAX
};

enum soc_res_sync_scope {
    SOC_DEV,
    SOC_TS_SUBSYS,
    SOC_DEV_DIE,
    SOC_SCOPE_MAX
};

enum soc_res_sync_type {
    SOC_MISC_RES,
    SOC_KEY_VALUE_RES,
    SOC_REG_ADDR,
    SOC_RSV_MEM_ADDR,
    SOC_IRQ_RES,
    SOC_MIA_RES,
    SOC_ATTR_RES,
    SOC_RES_TYPE_MAX
};

struct res_sync_target {
    enum soc_res_sync_dir dir;
    enum soc_res_sync_scope scope;
    enum soc_res_sync_type type;
    u32 id;
    u32 dst_udevid;
};

#endif
