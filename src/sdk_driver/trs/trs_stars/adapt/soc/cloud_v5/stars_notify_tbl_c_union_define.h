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
#ifndef __STARS_NOTIFY_TBL_C_UNION_DEFINE_H__
#define __STARS_NOTIFY_TBL_C_UNION_DEFINE_H__

#define STARS_TABLE_NOTIFY_NUM          4864U
#define STARS_TABLE_NOTIFY_UNIT_NUM     304U
#define STARS_TABLE_NOTIFY_NUM_PER_UNIT 16U

#define STARS_TABLE_NOTIFY_GRP_NUM      16U
/*
 * DEFINE REGISTER UNION
 */
/* Define the union StarsNotifyTableSlice */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int notifyTableFlagSlice;
        unsigned int notifyTablePendingClrSlice : 1;    /* rsv, not used */
        unsigned int reserved                   : 31;   /* rsv, not used */
    } bits;

    unsigned long long data[16];
} StarsNotifyTableSlice;

typedef struct {
    StarsNotifyTableSlice starsNotifyTableSlice[STARS_TABLE_NOTIFY_NUM_PER_UNIT];
    unsigned int reserved[512];
} StarsNotifyTableUnit; /* 4KB */
/*
 * DEFINE EVENT TABLE INFO STRUCT
 */
typedef struct {
    StarsNotifyTableUnit starsNotifyTableUnit[STARS_TABLE_NOTIFY_UNIT_NUM];
} StarsNotifyTableSliceInfo; /* 1216KB */

/*
 * DEFINE GLOBAL STRUCT
 */
typedef struct {
    StarsNotifyTableSliceInfo StarsNotifyGroupTable[STARS_TABLE_NOTIFY_GRP_NUM];
} StarsNotifyTblRegsType;

static inline StarsNotifyTableSlice *trs_get_stars_notify_tab_slice(StarsNotifyTblRegsType *tbl_info, u32 id)
{
    u32 group_id = id / STARS_TABLE_NOTIFY_NUM;
    u32 unit = (id % STARS_TABLE_NOTIFY_NUM) / STARS_TABLE_NOTIFY_NUM_PER_UNIT;
    u32 offset = id % STARS_TABLE_NOTIFY_NUM % STARS_TABLE_NOTIFY_NUM_PER_UNIT;
    StarsNotifyTableSliceInfo *group_info = &(tbl_info->StarsNotifyGroupTable[group_id]);
    StarsNotifyTableUnit *unit_info = &(group_info->starsNotifyTableUnit[unit]);
    return &(unit_info->starsNotifyTableSlice[offset]);
}
#endif /* __STARS_NOTIFY_TBL_C_UNION_DEFINE_H__ */
