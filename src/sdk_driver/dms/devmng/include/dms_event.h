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

#ifndef __DMS_EVENT_H__
#define __DMS_EVENT_H__
#include <linux/hashtable.h>
#include "fms/fms_dtm.h"
#include "fms/fms_smf.h"

#define DMS_EVENT_CMD_NAME "DMS_EVENT"

#define DMS_EVENT_ERROR_ARRAY_NUM (128UL)

#define EVENT_ID_HASH_TABLE_BIT 8
#define EVENT_ID_HASH_TABLE_SIZE (0x1 << EVENT_ID_HASH_TABLE_BIT)
#define EVENT_ID_HASH_TABLE_MASK (EVENT_ID_HASH_TABLE_SIZE - 1)

struct dms_converge_htable {
    DECLARE_HASHTABLE(htable, EVENT_ID_HASH_TABLE_BIT);
    struct mutex lock;

    bool converge_switch;
    bool converge_validity;
    bool need_free;
};

int dms_event_init(void);
void dms_event_exit(void);

#endif
