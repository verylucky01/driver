# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
KERNEL_DIR := $(srctree)

include $(FEATURE_MK_PATH)
EXTRA_CFLAGS += $(CONFIG_DEFINES)

ifeq ($(DAVINCI_HIAI_DKMS),y)
	DRIVER_MODULE_TOP_DIR := $(HIAI_DKMS_DIR)/fms
else
	DRIVER_MODULE_TOP_DIR := $(DRIVER_KERNEL_DIR)/fms
endif

DRIVER_MODULE_SMF_DIR := $(DRIVER_MODULE_TOP_DIR)/smf
EXTRA_CFLAGS += -I$(DRIVER_MODULE_TOP_DIR)/soft_fault

EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/config
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/include
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/core
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/drv_devmng/drv_devmng_inc
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/dev_urd

ifeq ($(DAVINCI_HIAI_DKMS),y) # for DKMS: TOP_DIR is NULL
    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif
    ccflags-y += -Wall -Werror -Wno-missing-prototypes -Wno-missing-declarations
    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_DRV_KERNEL_SOFT_EVENT
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=5
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=0
    EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_platform_host/ascend910
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/devmng/drv_devmng/drv_devmng_host/ascend910

    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/abl/libc_sec/include
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/abl/bbox/inc/bbox
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/abl/bbox/inc/bbox/device
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/abl/ascend_hal/user_space/src/dms/
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/ascend_platform
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor/config
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/even
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_drv_common
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/dev_urd/dc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/fms/command/ioctl
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/command/ioctl

    asdrv_fms-objs += soft_fault/soft_fault.o
    asdrv_fms-objs += soft_fault/dms_sensor_interface.o
    asdrv_fms-objs += soft_fault/drv_kernel_soft.o

else # for cmake and control cpu open
    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif
    ccflags-y += -Wall -Werror -funsigned-char -Wextra -Wformat=2 -Wfloat-equal -Wcast-align -Wvla -Wundef -Wstack-usage=2048 $(WDATE_TIME)
    ccflags-y += -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-missing-prototypes -Wno-missing-declarations

    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(TOP_DIR)/abl/bbox/inc/bbox
    EXTRA_CFLAGS += -I$(TOP_DIR)/abl/bbox/inc/bbox/device
    EXTRA_CFLAGS += -I$(TOP_DIR)/inc/toolchain/bbox/device # for ascend310 source release
    EXTRA_CFLAGS += -I$(TOP_DIR)/abl/ascend_hal/user_space/src/dms
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor/config
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/event
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/tsdrv/ts_drv/ts_drv_common
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/drv_devmng/drv_devmng_host/ascend910
    EXTRA_CFLAGS += -I${DRIVER_SOURCE_DIR}/platform/dc/drv_platform/ts_platform_host/ascend910
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/fms/command/ioctl
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/command/ioctl

    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=5
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=0
    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_OS_INIT_EVENT
    EXTRA_CFLAGS += -DCFG_FEATURE_DRV_KERNEL_SOFT_EVENT

    asdrv_fms-objs += soft_fault/soft_fault.o
	asdrv_fms-objs += soft_fault/dms_sensor_interface.o
    asdrv_fms-objs += soft_fault/drv_kernel_soft.o
    EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
endif
