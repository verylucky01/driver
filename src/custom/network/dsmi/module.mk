# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
LOCAL_PATH 		:= 	$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE 	:= 	libdsmi_network

LOCAL_LDFLAGS	+= 	-lrt

LOCAL_LDFLAGS	+=	-ldl

PATH_BRIDGE		:=

LOCAL_SRC_FILES :=

LOCAL_SRC_FILES += $(PATH_BRIDGE)ds_net.c
LOCAL_SRC_FILES += $(PATH_BRIDGE)ds_net_ext.c
LOCAL_SRC_FILES += $(PATH_BRIDGE)/../common/hccn_comm.c

LOCAL_C_INCLUDES:= $(TOPDIR)inc/network

IO_ROOT_DIR := $(TOPDIR)third_party

LOCAL_C_INCLUDES+= 	$(IO_ROOT_DIR)/../libc_sec/include
LOCAL_C_INCLUDES+=	$(TOPDIR)inc/toolchain
LOCAL_C_INCLUDES+=	$(TOPDIR)inc/driver
LOCAL_C_INCLUDES+=	$(TOPDIR)drivers/network/include
LOCAL_C_INCLUDES+=	$(TOPDIR)drivers/network/hccn/common

LOCAL_LD_DIRS :=

LOCAL_CFLAGS += -Werror -DDRV_HOST

## add more LOCAL_SRC_FILES and LOCAL_C_INCLUDES

LOCAL_SHARED_LIBRARIES := libc_sec libdrvdsmi_host libascend_hal

include $(BUILD_HOST_SHARED_LIBRARY)
