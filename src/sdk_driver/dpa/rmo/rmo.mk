# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

asdrv_dpa-objs += rmo/rmo_init.o rmo/rmo_fops.o rmo/rmo_proc_fs.o rmo/rmo_mem_sharing.o rmo/rmo_mem_sharing_ctx.o

ifeq ($(ENABLE_UBE), true)
    asdrv_dpa-objs += rmo/host/adapt/rmo_ub_adapt.o
else
    asdrv_dpa-objs += rmo/host/adapt/rmo_pcie_adapt.o
endif

DRIVER_SRC_DPA_BASE_DIR := $(DRIVER_KERNEL_DIR)/dpa
EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include
EXTRA_CFLAGS += -I$(DRIVER_SRC_DPA_BASE_DIR)/rmo
EXTRA_CFLAGS += -I$(DRIVER_SRC_DPA_BASE_DIR)/rmo/command/ioctl

ifneq ($(filter $(PRODUCT), ascend950),)
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernelspace/urma/code/kmod/ubcore/include/
endif
