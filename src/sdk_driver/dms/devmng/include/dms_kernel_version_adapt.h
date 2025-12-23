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

#ifndef __DMS_KERNEL_VERSION_ADAPT_H__
#define __DMS_KERNEL_VERSION_ADAPT_H__

#include <linux/version.h>

#include "kernel_version_adapt.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
#include <linux/time.h>
#include <linux/time64.h>
#include <linux/timekeeping.h>

static inline void do_gettimeofday(struct timeval *tv)
{
    struct timespec64 ts;
    ktime_get_real_ts64(&ts);
    tv->tv_sec = ts.tv_sec;
    tv->tv_usec = ts.tv_nsec / NSEC_PER_USEC;
}

#endif

#endif
