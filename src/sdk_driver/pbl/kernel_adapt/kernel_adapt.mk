# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/memory
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/task
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/fs
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/system
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/base
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/driver
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/pci
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/net
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/dfx
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/kernel_adapt/timer

ifeq ($(DAVINCI_HIAI_DKMS),y)
    KERNEL_DIR :=$(srctree)
    # for dkms
    DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
    EXTRA_CFLAGS += -I$(srctree)/include/linux
else
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/libc_sec
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(srctree)/include/linux
endif

ifeq ($(TARGET_BUILD_TYPE),debug)
    EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
endif

ifneq ($(NOT_SUPPORT_SP), y)
    EXTRA_CFLAGS += -fstack-protector-all
endif

EXTRA_CFLAGS += -Wall -Werror

ccflags-y += -Wall -Werror -Wtrampolines $(WDATE_TIME) -Wfloat-equal -Wvla -Wundef -funsigned-char -Wformat=2 -Wstack-usage=2048 -Wcast-align -Wextra
ccflags-y += -Wno-unused-parameter -O2 -Wno-missing-field-initializers -Wno-sign-compare

$(MODULE_NAME)-objs += kernel_adapt/ka_module_init.o kernel_adapt/event_notify_proc.o kernel_adapt/timer/ka_timer.o

EXTRA_CFLAGS += -DCFG_FEATURE_KA_MEM_MNG
EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
$(MODULE_NAME)-objs += kernel_adapt/memory/ka_memory_mng.o kernel_adapt/memory/ka_rbtree.o kernel_adapt/memory/ka_memory.o kernel_adapt/memory/ka_memory_query.o
$(MODULE_NAME)-objs += kernel_adapt/ka_proc_fs.o kernel_adapt/task/ka_task.o kernel_adapt/fs/ka_fs.o kernel_adapt/system/ka_system.o kernel_adapt/base/ka_base.o
$(MODULE_NAME)-objs += kernel_adapt/driver/ka_driver.o kernel_adapt/pci/ka_pci.o kernel_adapt/net/ka_net.o kernel_adapt/dfx/ka_dfx.o
