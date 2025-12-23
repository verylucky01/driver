# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
KERNEL_DIR :=$(srctree)

ifeq ($(DAVINCI_HIAI_DKMS),y) # for DKMS cmake
	ifneq ($(NOT_SUPPORT_SP), y)
		EXTRA_CFLAGS += -fstack-protector-all
	endif

	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/devmng/include
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
	EXTRA_CFLAGS += -DCFG_HOST_ENV
else # for cmake

	EXTRA_CFLAGS += -DCFG_HOST_ENV
	ifeq ($(ENABLE_UBE), true)
		EXTRA_CFLAGS += -DCFG_FEATURE_UB
	endif
	ifneq ($(NOT_SUPPORT_SP), y)
		EXTRA_CFLAGS += -fstack-protector-all
	endif
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/libc_sec
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/dms/devmng/include
	EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
	EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
	EXTRA_CFLAGS += -Iinclude/linux
endif
ccflags-y += -Wall -Werror

ifeq ($(TARGET_BUILD_TYPE),debug)
	EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
endif

obj-m += ascend_adapter.o
ascend_adapter-objs := adapter_api.o adapter_func_init.o adapter_init.o
