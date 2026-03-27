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

DRIVER_MODULE_TOP_DIR := $(DRIVER_KERNEL_DIR)/fms
DRIVER_MODULE_SMF_DIR := $(DRIVER_MODULE_TOP_DIR)/smf

EXTRA_CFLAGS += -I$(DRIVER_MODULE_TOP_DIR)/soft_fault
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/config
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/include
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/core
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/drv_devmng/drv_devmng_inc
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/dev_urd
EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/trs
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor
EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor/config
EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/event
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/tsdrv/ts_drv/ts_drv_common
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/drv_devmng/drv_devmng_host/ascend910
EXTRA_CFLAGS += -I${DRIVER_SOURCE_DIR}/platform/dc/drv_platform/ts_platform_host/ascend910
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/fms/command/ioctl
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/command/ioctl

ifneq ($(NOT_SUPPORT_SP), y)
    EXTRA_CFLAGS += -fstack-protector-all
endif
ccflags-y += -Wall -Werror -funsigned-char -Wextra -Wformat=2 -Wfloat-equal -Wcast-align -Wvla -Wundef -Wstack-usage=2048 $(WDATE_TIME)
ccflags-y += -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-missing-prototypes -Wno-missing-declarations

ifneq ($(filter $(PRODUCT), ascend910B),)
    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_OS_INIT_EVENT
    EXTRA_CFLAGS += -DCFG_FEATURE_DRV_KERNEL_SOFT_EVENT
endif

ifneq ($(filter $(PRODUCT), ascend950),)
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_ASCEND950_STUB
    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_OS_INIT_EVENT
    EXTRA_CFLAGS += -DCFG_FEATURE_DRV_KERNEL_SOFT_EVENT
endif

asdrv_fms-objs += soft_fault/soft_fault.o
asdrv_fms-objs += soft_fault/dms_sensor_interface.o
asdrv_fms-objs += soft_fault/drv_kernel_soft.o
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
