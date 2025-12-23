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

#ifndef TRS_TIMESTAMP_H
#define TRS_TIMESTAMP_H
#include "ka_base_pub.h"
#include "ka_system_pub.h"

static inline u64 trs_get_us_timestamp(void)
{
#ifndef EMU_ST
    return (u64)ka_system_ktime_to_us(ka_system_ktime_get_boottime());
#else
    return 0;
#endif
}

static inline u64 trs_get_s_timestamp(void)
{
    return (ka_jiffies / KA_HZ);
}
#endif
