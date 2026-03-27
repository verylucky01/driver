# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
include $(FEATURE_MK_PATH)
EXTRA_CFLAGS += $(CONFIG_DEFINES)

$(info $(MODULE_NAME))

DRIVER_MODULE_DAVINCI_INTF_DIR := $(DRIVER_SOURCE_DIR)/pbl/drv_davinci_intf/drv_davinci_intf_host

EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/libc_sec
EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc/pbl
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc/ascend_platform
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/uda/command/ioctl
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/kernel_adapt/include
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/tsdrv/ts_drv/ts_drv_host/ts_drv_common
EXTRA_CFLAGS += -I$(DRIVER_MODULE_DAVINCI_INTF_DIR)

EXTRA_CFLAGS += -DCFG_HOST_ENV
ifneq ($(filter $(PRODUCT), ascend910B),)
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
endif

ifneq ($(filter $(PRODUCT), ascend950),)
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
endif

ifneq ($(NOT_SUPPORT_SP), y)
    EXTRA_CFLAGS += -fstack-protector-all
endif

ccflags-y += -Wall -Werror
ccflags-y += -funsigned-char -Wextra -Wformat=2 -Wfloat-equal -Wcast-align -Wvla -Wundef -Wstack-usage=2048 $(WDATE_TIME)
ccflags-y += -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers

# include C standard library for lib_sec
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
EXTRA_CFLAGS += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Werror \
                -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla

$(MODULE_NAME)-objs += drv_davinci_intf/drv_davinci_intf_host/davinci_intf_init.o drv_davinci_intf/drv_davinci_intf_host/davinci_intf_process.o
