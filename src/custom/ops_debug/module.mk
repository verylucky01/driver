# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ts_debug
LOCAL_KO_SRC_FOLDER := $(LOCAL_PATH)
LOCAL_INSTALLED_KO_FILES := ts_debug.ko

include $(CLEAR_VARS)
LOCAL_MODULE := debug_switch
LOCAL_KO_SRC_FOLDER := $(LOCAL_PATH)
LOCAL_INSTALLED_KO_FILES := debug_switch.ko

include $(BUILD_HOST_KO)
LOCAL_DEPEND_KO := drv_seclib_host asdrv_fms asdrv_trs asdrv_pbl asdrv_trsbase ascend_kernel_open_adapt

