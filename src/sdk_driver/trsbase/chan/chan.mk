# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifeq ($(ENABLE_OPEN_SRC), y)
    asdrv_trsbase-objs += chan/chan_init.o chan/chan_rxtx.o chan/chan_ts_inst.o chan/chan_proc_fs.o chan/chan_abnormal.o

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsbase/chan
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include

    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE

    CFG_FEATURE_TRACE_EVENT_FUNC_SUPPORT=y
    ifeq ($(CFG_FEATURE_TRACE_EVENT_FUNC_SUPPORT),y)
        EXTRA_CFLAGS += -DCFG_FEATURE_TRACE_EVENT_FUNC
        asdrv_trsbase-objs += chan/chan_trace.o
    endif
else
    ccflags-y += -Wall -Werror
    ccflags-y += -Wno-missing-prototypes -Wno-missing-declarations
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG

    ifeq ($(DAVINCI_HIAI_DKMS),y)
    #for dkms
        DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)
        EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
        EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl

        ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980b),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif
    else
        DRIVER_SRC_BASE_DIR := $(DRIVER_KERNEL_DIR)
        EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
        EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV

        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
        ifneq ($(filter $(PRODUCT), ascend310p ascend910 ascend910B ascend310B ascend310Brc),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif
    endif

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trsbase/chan

    EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
    EXTRA_CFLAGS += -Iinclude/linux

    asdrv_trsbase-objs += chan/chan_init.o chan/chan_rxtx.o chan/chan_ts_inst.o chan/chan_proc_fs.o chan/chan_abnormal.o

    CFG_FEATURE_TRACE_EVENT_FUNC_SUPPORT=y
    ifeq ($(CFG_FEATURE_TRACE_EVENT_FUNC_SUPPORT),y)
        EXTRA_CFLAGS += -DCFG_FEATURE_TRACE_EVENT_FUNC
        asdrv_trsbase-objs += chan/chan_trace.o
    endif
endif