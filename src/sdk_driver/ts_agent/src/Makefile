# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
# ################################################################################################################
# AR20241204266067 ts_agent不能直接包含除<linux/types.h>外的内核头文件
# ################################################################################################################
TMP_TOP_DIR :=
TMP_CURRENT_DIR :=
TMP_DEV_INC_DIR :=
TMP_OPEN_INC_DIR :=
TMP_LIBC_SEC_BASE_DIR :=
TMP_OPEN_KA_INC_DIR :=
STARS_FLAG = n



ifeq ($(ENABLE_OPEN_SRC), y)
    TMP_TOP_DIR := $(DRIVER_KERNEL_DIR)
    TMP_CURRENT_DIR = $(DRIVER_KERNEL_DIR)/ts_agent
    TMP_DEV_INC_DIR = $(TMP_TOP_DIR)/inc
    TMP_OPEN_INC_DIR = $(DRIVER_HAL_INC_DIR)
    TMP_OPEN_KA_INC_DIR = $(DRIVER_KERNEL_DIR)/kernel_adapt/include


    EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V2
    EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_STARS
    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_HOST_VIRTUAL_MACHINES
    STARS_FLAG = y

	EXTRA_CFLAGS += -I$(TMP_CURRENT_DIR)/inc/ \
					-I$(TMP_TOP_DIR) \
					-I$(TMP_TOP_DIR)/inc \
					-I$(TMP_DEV_INC_DIR) \
					-I$(TMP_OPEN_INC_DIR) \
					-I$(TMP_TOP_DIR)/inc/driver \
					-I$(TMP_TOP_DIR)/ace/csruntime/inc \
					-I$(TMP_OPEN_KA_INC_DIR) \
                    -I$(C_SEC_INCLUDE)
    

	ccflags-y += -Wall -Werror
	obj-m := ts_agent_vm.o
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_core/command/ioctl/

    ts_agent_vm-objs := ts_agent_module.o \
                    ts_agent_update_sqe.o

else
    ifneq ($(TOP_DIR),)
        TMP_TOP_DIR := $(DRIVER_KERNEL_DIR)
        TMP_CURRENT_DIR = $(DRIVER_KERNEL_DIR)/ts_agent
        TMP_DEV_INC_DIR = $(TMP_TOP_DIR)/inc
        TMP_OPEN_INC_DIR = $(DRIVER_HAL_INC_DIR)
        TMP_OPEN_KA_INC_DIR = $(DRIVER_KERNEL_DIR)/kernel_adapt/include
        TMP_LIBC_SEC_BASE_DIR = $(TMP_OPEN_INC_DIR)/depends/libc_sec
    else ifeq ($(DAVINCI_HIAI_DKMS), y)
        TMP_TOP_DIR = $(HIAI_DKMS_DIR)
        TMP_CURRENT_DIR = $(TMP_TOP_DIR)/ts_agent
        TMP_DEV_INC_DIR = $(TMP_TOP_DIR)/dev_inc/inc
        TMP_LIBC_SEC_BASE_DIR = $(TMP_TOP_DIR)/libc_sec/include/
        TMP_OPEN_KA_INC_DIR = $(TMP_TOP_DIR)/kernel_adapt/include
    else
        TMP_CURRENT_DIR = $(strip \
            $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
            $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
        )
        TMP_TOP_DIR = $(TMP_CURRENT_DIR)/../../
        TMP_DEV_INC_DIR = $(TMP_TOP_DIR)/drivers/ai_sdk/arc/linux/kernel_space/inc
        TMP_LIBC_SEC_BASE_DIR = $(TMP_TOP_DIR)/abl/libc_sec/include/
        TMP_OPEN_KA_INC_DIR = $(TMP_TOP_DIR)/drivers/ai_sdk/arc/linux/kernel_open/src/kernel_adapt/include
    endif


    EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V2
    EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_STARS
    STARS_FLAG = y
    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_HOST_VIRTUAL_MACHINES

	EXTRA_CFLAGS += -I$(TMP_CURRENT_DIR)/inc/ \
                    -I$(TMP_DEV_INC_DIR)/ \
					-I$(TMP_TOP_DIR) \
					-I$(TMP_TOP_DIR)/inc \
					-I$(TMP_OPEN_INC_DIR) \
					-I$(TMP_TOP_DIR)/inc/driver \
					-I$(TMP_TOP_DIR)/ace/csruntime/inc \
					-I$(TMP_OPEN_KA_INC_DIR) \
                    -I$(TMP_OPEN_KA_INC_DIR)/ka_arch/$(ARCH) \
					-I$(TMP_LIBC_SEC_BASE_DIR)

    ccflags-y += -Wall -Werror
    obj-m := ts_agent_vm.o


    ifeq ($(DAVINCI_HIAI_DKMS), y)
        EXTRA_CFLAGS += -I$(TMP_TOP_DIR)/rms/trs_drv/inc/
    else
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/inc
    endif

    ts_agent_vm-objs := ts_agent_module.o \
                        ts_agent_update_sqe.o
endif