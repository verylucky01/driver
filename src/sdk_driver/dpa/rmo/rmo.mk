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
    asdrv_dpa-objs += rmo/rmo_init.o rmo/rmo_fops.o rmo/rmo_proc_fs.o rmo/rmo_mem_sharing.o

    DRIVER_SRC_DPA_BASE_DIR := $(DRIVER_KERNEL_DIR)/dpa
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_SRC_DPA_BASE_DIR)/rmo
    EXTRA_CFLAGS += -I$(DRIVER_SRC_DPA_BASE_DIR)/rmo/command/ioctl
else
    ccflags-y += -Wno-missing-prototypes -Wno-missing-declarations
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    ifeq ($(TOP_DIR),)
        #for dkms
        DRIVER_SRC_DPA_BASE_DIR := $(HIAI_DKMS_DIR)/dpa

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
    else
        DRIVER_SRC_DPA_BASE_DIR := $(DRIVER_KERNEL_DIR)/src/dpa

        EXTRA_CFLAGS += -I$(TOP_DIR)/abl/libc_sec/include
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I$(TOP_DIR)/inc/driver
    endif

    EXTRA_CFLAGS += -I$(DRIVER_SRC_DPA_BASE_DIR)/rmo
    EXTRA_CFLAGS += -I$(DRIVER_SRC_DPA_BASE_DIR)/rmo/command/ioctl
    EXTRA_CFLAGS += -I$(DRIVER_SRC_DPA_BASE_DIR)/rmo/res_map_ops

    EXTRA_CFLAGS += -Wfloat-equal
    EXTRA_CFLAGS += -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-format-nonliteral -Wno-empty-body

    EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
    EXTRA_CFLAGS += -Iinclude/linux

    ifeq ($(PRODUCT_SIDE), host)
        asdrv_dpa-objs += rmo/rmo_init.o rmo/rmo_fops.o rmo/rmo_proc_fs.o rmo/rmo_mem_sharing.o
    else
        asdrv_dpa-objs += rmo/rmo_init.o rmo/rmo_fops.o rmo/rmo_proc_fs.o
        asdrv_dpa-objs += rmo/res_map_ops/rmo_res_map.o rmo/res_map_ops/rmo_res_map_ctx.o rmo/res_map_ops/rmo_res_cfg_parse.o
    endif

    ifneq ($(filter $(PRODUCT), ascend910_95esl ascend910_55esl ascend910_96esl),)
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_ESL
    endif

    ifneq ($(filter $(PRODUCT), ascend910_96esl ascend910_96),)
        EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_PBHA
    endif
endif
