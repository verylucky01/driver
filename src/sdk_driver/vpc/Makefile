# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
MODULE_NAME := asdrv_${SERVER_TYPE}
obj-m += ${MODULE_NAME}.o
${MODULE_NAME}-objs := common/virtmng_msg_pub.o common/virtmng_msg_common.o common/virtmng_msg_admin.o
${MODULE_NAME}-objs += common/virtmng_extension.o common/virtmng_stack.o common/virtmng_resource.o
ifeq ($(SERVER_TYPE), vpc)
    ${MODULE_NAME}-objs += phy/virtmnghost_msg.o phy/virtmnghost_msg_common.o phy/virtmnghost_msg_admin.o
    ${MODULE_NAME}-objs += phy/virtmnghost_vpc.o phy/virtmnghost_vpc_unit.o
    ${MODULE_NAME}-objs += phy/adapt/virtmnghost_resource_default.o phy/adapt/virtmnghost_resource_sriov.o
    ${MODULE_NAME}-objs += phy/adapt/vpc_soc_adapt.o
    ${MODULE_NAME}-objs += phy/vmngh_vpc_interface.o
else
    ${MODULE_NAME}-objs += virt/virtmngagent_msg.o virt/virtmngagent_msg_common.o virt/virtmngagent_msg_admin.o
    ${MODULE_NAME}-objs += virt/virtmngagent_vpc.o virt/virtmngagent_vpc_unit.o
    ${MODULE_NAME}-objs += virt/vmnga_vpc_interface.o
endif

EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vmng/command
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vmng/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vpc/common
EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
ifeq ($(SERVER_TYPE), vpc)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vmng/phy/host
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vpc/phy
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vpc/phy/adapt
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vmng/phy/vascend_drv_stub
else
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vmng/virt/vpcie
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vpc/virt
endif

EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

ccflags-y += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Werror -Wfloat-equal
ccflags-y += -Wformat=2 -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla -Wno-format-nonliteral