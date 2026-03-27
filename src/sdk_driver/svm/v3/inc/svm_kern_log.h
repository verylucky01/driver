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

#ifndef SVM_KERN_LOG_H
#define SVM_KERN_LOG_H

#include "ka_task_pub.h"
#include "ka_system_pub.h"

#ifndef EMU_ST /* Simulation ST is required and cannot be deleted. */
#include "dmc_kernel_interface.h"
#include "svm_share_log.h"
#else
#include "ut_log.h"
#endif

#define MODULE_SVM "svm"

#define svm_err(fmt, ...) do { \
    drv_err(MODULE_SVM, "<%s:%d:%d:%d> " fmt, \
        ka_task_get_current_comm(), ka_task_get_current_tgid(), ka_task_get_current_pid(), ka_system_raw_smp_processor_id(), ##__VA_ARGS__); \
        svm_share_log_err(fmt, ##__VA_ARGS__); \
} while (0)

#define svm_warn(fmt, ...) do { \
    drv_warn(MODULE_SVM, "<%s:%d:%d:%d> " fmt, \
        ka_task_get_current_comm(), ka_task_get_current_tgid(), ka_task_get_current_pid(), ka_system_raw_smp_processor_id(), ##__VA_ARGS__); \
} while (0)

#define svm_info(fmt, ...) do { \
    drv_info(MODULE_SVM, "<%s:%d:%d:%d> " fmt, \
        ka_task_get_current_comm(), ka_task_get_current_tgid(), ka_task_get_current_pid(), ka_system_raw_smp_processor_id(), ##__VA_ARGS__); \
        svm_share_log_run_info(fmt, ##__VA_ARGS__); \
} while (0)

#define svm_debug(fmt, ...) do { \
    drv_pr_debug(MODULE_SVM, "<%s:%d:%d:%d> " fmt, \
        ka_task_get_current_comm(), ka_task_get_current_tgid(), ka_task_get_current_pid(), ka_system_raw_smp_processor_id(), ##__VA_ARGS__); \
} while (0)

/* instance trace. used in dev/task/others instance create and destroy.
   For example: dev init/uninit, task init/uninit
   The product development phase is initially defined as info, and is later changed to debug after stabilization. */
#define svm_inst_trace svm_debug

#define svm_err_if(cond, fmt, ...)  \
    if (cond)                       \
        svm_err(fmt, ##__VA_ARGS__)

#define svm_no_err_if(cond, fmt, ...) \
    if (cond) {                       \
        svm_info(fmt, ##__VA_ARGS__); \
    } else {                          \
        svm_err(fmt, ##__VA_ARGS__);  \
    }

#endif

