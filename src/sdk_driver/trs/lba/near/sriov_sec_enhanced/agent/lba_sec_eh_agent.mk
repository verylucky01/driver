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
    $(MODULE_NAME)-objs += lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_init.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_dev_init.o
    $(MODULE_NAME)-objs += lba/near/sriov_sec_enhanced/agent/trs_sec_eh_cfg.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_id.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_sq.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_msg.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_mbox.o
    $(MODULE_NAME)-objs += lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_chan/trs_sec_eh_agent_chan.o

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vmng/phy/vascend_drv_stub
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt/trs_host_core
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/mbox/hard
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/trs_host_core/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/trs_host_chan/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/trs_host_chan
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/comm/adapt
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/mbox/hard/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_chan
    
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_SEC_EH_AGENT
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
else
    ccflags-y += -Wno-missing-prototypes -Wno-missing-declarations
    MAKE_PRODUCT := $(PRODUCT)
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_SEC_EH_AGENT

    ifeq ($(DAVINCI_HIAI_DKMS), y)
        #for dkms
        DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc_open/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/vascend/

        ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980b),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif
    else
        DRIVER_SRC_BASE_DIR := $(DRIVER_KERNEL_DIR)

        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/vmng/phy/vascend_drv_stub

        ifneq ($(filter $(PRODUCT), ascend310p ascend910 ascend910B ascend310B),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif

    endif

    ccflags-y += -Wall -Werror

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt/trs_host_core
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/mbox/hard
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/trs_host_core/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/trs_host_chan/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/near_comm/trs_host_chan
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/comm/adapt
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/mbox/hard/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sriov_sec_enhanced/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_chan

    $(MODULE_NAME)-objs += lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_init.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_dev_init.o
    $(MODULE_NAME)-objs += lba/near/sriov_sec_enhanced/agent/trs_sec_eh_cfg.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_id.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_sq.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_msg.o lba/near/sriov_sec_enhanced/agent/trs_sec_eh_mbox.o
    $(MODULE_NAME)-objs += lba/near/sriov_sec_enhanced/agent/trs_sec_eh_agent_chan/trs_sec_eh_agent_chan.o
endif
