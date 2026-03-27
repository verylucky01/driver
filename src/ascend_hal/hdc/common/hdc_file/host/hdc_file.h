/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef __ASCEND_HAL_HDC_ADAPT_H__
#define __ASCEND_HAL_HDC_ADAPT_H__

#ifndef HDC_UT_TEST
#include "mmpa_api.h"
#endif

#include <stdbool.h>
#include "ascend_hal.h"
#include "ascend_hal_base.h"
#if !defined(EMU_ST) || !defined(DRV_UT)
#include "dmc_user_interface.h"
#else
#include "linux/fs.h"
#include "ascend_inpackage_hal.h"
#include "ut_log.h"
#endif

/* Maximum number of devices */
#if defined(CFG_FEATURE_SRIOV)
#define HDC_MAX_DEVICE_NUM 1124
#define MAX_VF_DEVID_START 100
#else
#define HDC_MAX_DEVICE_NUM 64
#define MAX_VF_DEVID_START 32
#endif

#define HDC_LOG_RUN_INFO_LIMIT(format, ...) do { \
    DRV_RUN_INFO(HAL_MODULE_TYPE_HDC, format, ##__VA_ARGS__); \
} while (0)

#define HDC_MAX_UB_DEV_CNT 356

#ifdef HDC_UT_TEST
typedef int mmSockHandle;
typedef signed int mmProcess;
typedef unsigned int UINT32;
typedef signed int INT32;
typedef unsigned char UINT8;
typedef unsigned long long UINT64;
typedef signed long long INT64;
typedef pthread_mutex_t mmMutex_t;
#endif

struct filesock;
void hdc_create_dir_info_output(void);
void *drv_hdc_mmap(mmProcess fd, void *addr, unsigned int alloc_len, unsigned int flag);
signed int drv_hdc_alloc_len_check(enum drvHdcMemType mem_type, unsigned int len, unsigned int flag);
void drv_hdc_set_malloc_flag(unsigned int *flag);
bool drv_hdc_is_support_session_close(void);
signed int get_local_trusted_base_path(signed int user_mode, char *path, signed int dev_id);
signed int get_peer_trusted_base_path(signed int user_mode, char *path, signed int peer_devid);
#ifndef CFG_SOC_PLATFORM_RC
hdcError_t drv_hdc_dst_path_right_check(const char *dst_path, const char *base_path, int root_privilege);
hdcError_t hdc_get_session_attr_check(HDC_SESSION session, int *devid);
int hdc_directory_path_compose(int devid, char *base_path, char *dm_base_path, char *cann_base_path);
#endif
hdcError_t validate_resource(struct filesock *fs);
hdcError_t drv_hdc_dst_path_depth_check(const char *path);

#endif