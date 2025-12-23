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

#ifndef __STARS_CNT_NOTIFY_TBL_C_UNION_DEFINE_H__
#define __STARS_CNT_NOTIFY_TBL_C_UNION_DEFINE_H__

#define STARS_TABLE_CNT_NOTIFY_NUM          4864U
#define STARS_TABLE_CNT_NOTIFY_UNIT_NUM     304U
#define STARS_TABLE_CNT_NOTIFY_NUM_PER_UNIT 16U

#define STARS_TABLE_CNT_NOTIFY_GRP_NUM      16U

typedef union {
    struct {
        unsigned int notify_cnt_status_slice;
        unsigned int rsv0[7];
        unsigned int notify_cnt_add_slice : 1;
        unsigned int rsv1 : 31;
        unsigned int rsv2[7];
        unsigned int notify_cnt_bit_wr_slice;
        unsigned int rsv3[7];
        unsigned int notify_cnt_bit_clr_slice;
    } regs;

    unsigned long long data[16];
} stars_cnt_notify_table_slice;

typedef struct {
    stars_cnt_notify_table_slice stars_cnt_notify_table_slice[STARS_TABLE_CNT_NOTIFY_NUM_PER_UNIT];
    unsigned int reserved[512];
} stars_cnt_notify_table_unit; /* 4KB */

typedef struct {
    stars_cnt_notify_table_unit stars_cnt_notify_table_unit[STARS_TABLE_CNT_NOTIFY_UNIT_NUM];
} stars_cnt_notify_table_slice_info; /* 1216KB */

typedef struct {
    stars_cnt_notify_table_slice_info stars_cnt_notify_group_table[STARS_TABLE_CNT_NOTIFY_GRP_NUM];
} stars_cnt_notify_tbl_regs_type;

static inline stars_cnt_notify_table_slice *trs_get_stars_cnt_notify_tab_slice(stars_cnt_notify_tbl_regs_type *tbl_info,
    u32 id)
{
    u32 group_id = id / STARS_TABLE_CNT_NOTIFY_NUM;
    u32 unit = (id % STARS_TABLE_CNT_NOTIFY_NUM) / STARS_TABLE_CNT_NOTIFY_NUM_PER_UNIT;
    u32 offset = id % STARS_TABLE_CNT_NOTIFY_NUM % STARS_TABLE_CNT_NOTIFY_NUM_PER_UNIT;

    stars_cnt_notify_table_slice_info *group_info = &(tbl_info->stars_cnt_notify_group_table[group_id]);
    stars_cnt_notify_table_unit *unit_info = &(group_info->stars_cnt_notify_table_unit[unit]);
    return &(unit_info->stars_cnt_notify_table_slice[offset]);
}

#endif /* __STARS_CNT_NOTIFY_TBL_C_UNION_DEFINE_H__ */
