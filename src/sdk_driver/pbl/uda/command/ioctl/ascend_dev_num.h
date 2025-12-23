/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef __ASCEND_DEV_NUM_H__
#define __ASCEND_DEV_NUM_H__

#ifdef CONFIG_PLATFORM_310P
#include "ascend_platform/ascend_dev_num_310P.h"
#endif
#ifdef CONFIG_PLATFORM_910
#include "ascend_platform/ascend_dev_num_910.h"
#endif
#ifdef CONFIG_PLATFORM_310B
#include "ascend_platform/ascend_dev_num_310B.h"
#endif
#ifdef CONFIG_PLATFORM_310M
#include "ascend_platform/ascend_dev_num_310B.h"
#endif
#ifdef CONFIG_PLATFORM_910B
#include "ascend_platform/ascend_dev_num_910B.h"
#endif
#ifdef CONFIG_PLATFORM_910_95
#include "ascend_platform/ascend_dev_num_910_95.h"
#endif
#ifdef CONFIG_PLATFORM_910_96
#include "ascend_platform/ascend_dev_num_910_95.h"
#endif

#endif /* __ASCEND_DEV_NUM_H__ */