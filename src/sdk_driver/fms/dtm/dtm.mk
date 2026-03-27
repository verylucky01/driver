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
include $(FEATURE_MK_PATH)
EXTRA_CFLAGS += $(CONFIG_DEFINES)

DRIVER_MODULE_TOP_DIR := $(DRIVER_KERNEL_DIR)/fms
DRIVER_MODULE_SMF_DIR := $(DRIVER_MODULE_TOP_DIR)/smf
DRIVER_MODULE_DTM_DIR := $(DRIVER_MODULE_TOP_DIR)/dtm

EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/config
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/include
EXTRA_CFLAGS += -I$(DRIVER_MODULE_TOP_DIR)
EXTRA_CFLAGS += -I$(DRIVER_MODULE_DTM_DIR)
EXTRA_CFLAGS += -I${C_SEC_INCLUDE}
EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/pbl/uda/command/ioctl
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include
EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor
EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/event

ifneq ($(filter $(PRODUCT), ascend910B),)
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_VFG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_STATUS
    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
endif

ifneq ($(filter $(PRODUCT), ascend950),)
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
endif

ccflags-y += -Wall -Werror
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)

ifneq ($(NOT_SUPPORT_SP), y)
    EXTRA_CFLAGS += -fstack-protector-all
endif

asdrv_fms-objs += dtm/dms_dev_node.o
asdrv_fms-objs += dtm/dms_node_type.o
asdrv_fms-objs += dtm/dms_dtm_init.o
