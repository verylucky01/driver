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

#include "ka_base_pub.h"
#include "ka_system_pub.h"
#include "ka_task_pub.h"
#include "ka_sched_pub.h"

void ka_try_cond_resched_by_time(unsigned long *pre_stamp, unsigned long time)
{
    unsigned long timeinterval;

    if (ka_base_in_atomic()) {
        return;
    }

    timeinterval = ka_system_jiffies_to_msecs(ka_jiffies - *pre_stamp);
    if (timeinterval > time) {
        ka_task_cond_resched();
        *pre_stamp = ka_jiffies;
    }
}
EXPORT_SYMBOL_GPL(ka_try_cond_resched_by_time);

void ka_try_cond_resched(unsigned long *pre_stamp)
{
    ka_try_cond_resched_by_time(pre_stamp, 500);   /* 500ms */
}
EXPORT_SYMBOL_GPL(ka_try_cond_resched);