# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifneq ($(NOT_SUPPORT_SP), y)
    EXTRA_CFLAGS += -fstack-protector-all
endif

ifeq ($(DAVINCI_HIAI_DKMS),y)
    #for dkms
    DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/uda
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/uda/dc
    EXTRA_CFLAGS += -DDRV_HOST
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_PROCESS_GROUP
    EXTRA_CFLAGS += -DCFG_FEATURE_DEVID_TRANS
    $(MODULE_NAME)-objs += uda/spod_info.o
    $(MODULE_NAME)-objs += uda/dc/uda_proc_fs_adapt.o

    EXTRA_CFLAGS += -DCFG_FEATURE_KA
    EXTRA_CFLAGS += -I./uda/dc
else
    DRIVER_SRC_BASE_DIR := $(DRIVER_KERNEL_DIR)/src
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/libc_sec
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -DDRV_HOST
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_SURPORT_UDEV_MNG
    EXTRA_CFLAGS += -DCFG_FEATURE_DEVID_TRANS
    EXTRA_CFLAGS += -DCFG_FEATURE_PROCESS_GROUP
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_KA

    $(MODULE_NAME)-objs += uda/spod_info.o
endif

EXTRA_CFLAGS += -Wfloat-equal
EXTRA_CFLAGS += -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-format-nonliteral -Wno-empty-body
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/uda
EXTRA_CFLAGS += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Werror \
                -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla
$(MODULE_NAME)-objs += uda/uda_access.o uda/uda_dev.o uda/uda_fops.o uda/uda_notifier.o uda/uda_proc_fs.o

EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/uda/dc
$(MODULE_NAME)-objs += uda/dc/uda_proc_fs_adapt.o
