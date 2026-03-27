/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026. All rights reserved.
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
#ifndef RMO_SCHED_H
#define RMO_SCHED_H
#include "ka_base_pub.h"
#include "ka_system_pub.h"
#include "ka_task_pub.h"

#define RMO_MEM_SHARING_WAKEUP_TIMEINTERVAL 500 /* 500 ms */

static inline void rmo_try_cond_resched_by_time(unsigned long *pre_stamp, unsigned long time)
{
    unsigned long time_interval;

    time_interval = ka_system_jiffies_to_msecs(ka_jiffies - *pre_stamp);
    if ((time_interval > time) && !ka_base_in_atomic()) {
        ka_task_cond_resched();
        *pre_stamp = ka_jiffies;
    }
}

static inline void rmo_try_cond_resched(unsigned long *pre_stamp)
{
    rmo_try_cond_resched_by_time(pre_stamp, RMO_MEM_SHARING_WAKEUP_TIMEINTERVAL);
}
#endif
