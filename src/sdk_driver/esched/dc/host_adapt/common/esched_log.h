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

#ifndef EVENT_ESCHED_LOG_H
#define EVENT_ESCHED_LOG_H

#include "ka_system_pub.h"

#ifndef EMU_ST
#include "dmc_kernel_interface.h"
#else
#include "ut_log.h"
#endif

#define module_event_sched "event_sched"

static inline u32 sched_get_cur_processor_id(void)
{
    return ka_system_raw_smp_processor_id();
}

#ifdef CFG_FEATURE_LOG_OPTIMIZE
#define sched_err(fmt, ...) do { \
    drv_err(module_event_sched, "<%s:%d:%d:%d> " fmt, \
    ka_task_get_current()->comm, ka_task_get_current()->tgid, ka_task_get_current()->pid, sched_get_cur_processor_id(), ##__VA_ARGS__); \
} while (0)
#else
#define sched_err(fmt, ...) do { \
    drv_err(module_event_sched, "<%s:%d:%d:%d> " fmt, \
    ka_task_get_current()->comm, ka_task_get_current()->tgid, ka_task_get_current()->pid, sched_get_cur_processor_id(), ##__VA_ARGS__); \
    share_log_err(ESCHED_SHARE_LOG_START, fmt, ##__VA_ARGS__); \
} while (0)
#endif
#define sched_warn(fmt, ...) do { \
    drv_warn(module_event_sched, "<%s:%d:%d:%d> " fmt, \
    ka_task_get_current()->comm, ka_task_get_current()->tgid, ka_task_get_current()->pid, sched_get_cur_processor_id(), ##__VA_ARGS__); \
} while (0)
#define sched_info(fmt, ...) do { \
    drv_info(module_event_sched, "<%s:%d:%d:%d> " fmt, \
    ka_task_get_current()->comm, ka_task_get_current()->tgid, ka_task_get_current()->pid, sched_get_cur_processor_id(), ##__VA_ARGS__); \
} while (0)
#define sched_debug(fmt, ...) do { \
    drv_pr_debug(module_event_sched, "<%s:%d:%d:%d> " fmt, \
    ka_task_get_current()->comm, ka_task_get_current()->tgid, ka_task_get_current()->pid, sched_get_cur_processor_id(), ##__VA_ARGS__); \
} while (0)

#define sched_err_spinlock(fmt, ...)
#define sched_warn_spinlock(fmt, ...)
#define sched_info_spinlock(fmt, ...)
#define sched_debug_spinlock(fmt, ...)

#if (defined EMU_ST)
#define sched_info_printk_ratelimited(fmt, ...)					\
    sched_info(fmt, ##__VA_ARGS__)
#else
#define sched_info_printk_ratelimited(fmt, ...)					\
({									\
	static KA_BASE_DEFINE_RATELIMIT_STATE(sched_rate_limit,				\
				      KA_DEFAULT_RATELIMIT_INTERVAL,	\
				      KA_DEFAULT_RATELIMIT_BURST);		\
									\
    if (__ka_base_ratelimit(&sched_rate_limit))	{					\
        sched_info(fmt, ##__VA_ARGS__);				\
    } \
})
#endif

#if (defined EMU_ST)
#define sched_err_printk_ratelimited(fmt, ...)					\
    sched_err(fmt, ##__VA_ARGS__)
#else
#define sched_err_printk_ratelimited(fmt, ...)					\
({									\
	static KA_BASE_DEFINE_RATELIMIT_STATE(sched_rate_limit,				\
				      KA_DEFAULT_RATELIMIT_INTERVAL,	\
				      KA_DEFAULT_RATELIMIT_BURST);		\
									\
    if (__ka_base_ratelimit(&sched_rate_limit))	{					\
        sched_err(fmt, ##__VA_ARGS__);				\
    } \
})
#endif

#endif