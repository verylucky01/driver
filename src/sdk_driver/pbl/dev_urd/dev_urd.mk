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

ifeq ($(DAVINCI_HIAI_DKMS),y)
    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/common

    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE

    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/dev_urd/dc
    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_VFG
    EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_STATUS
else     # for cmake
    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/libc_sec
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/src/common

    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/dev_urd/dc
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_STATUS
    EXTRA_CFLAGS += -DCFG_FEATURE_VFG
    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
endif

ifneq ($(NOT_SUPPORT_SP), y)
    EXTRA_CFLAGS += -fstack-protector-all
endif

ifeq ($(TARGET_BUILD_TYPE),debug)
    EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
endif

ccflags-y += -Wall -Werror -Wtrampolines $(WDATE_TIME) -Wfloat-equal -Wvla -Wundef
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
EXTRA_CFLAGS += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Werror \
                -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wunused
$(MODULE_NAME)-objs += dev_urd/urd_feature.o dev_urd/urd_init.o dev_urd/urd_notifier.o
$(MODULE_NAME)-objs += dev_urd/urd_container.o dev_urd/urd_acc_ctrl.o
$(MODULE_NAME)-objs += dev_urd/urd_kv.o
