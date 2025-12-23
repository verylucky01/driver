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

ifeq ($(DAVINCI_HIAI_DKMS),y)
	DRIVER_MODULE_TOP_DIR := $(HIAI_DKMS_DIR)/fms
else
	DRIVER_MODULE_TOP_DIR := $(DRIVER_KERNEL_DIR)/fms
endif

DRIVER_MODULE_SMF_DIR := $(DRIVER_MODULE_TOP_DIR)/smf
DRIVER_MODULE_DTM_DIR := $(DRIVER_MODULE_TOP_DIR)/dtm
DRIVER_MODULE_FPDC_DIR := $(DRIVER_MODULE_TOP_DIR)/fpdc

EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/config
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/include
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/dev_urd

ifeq ($(DAVINCI_HIAI_DKMS),y)         # for DKMS cmake
    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/ascend_platform
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/dev_urd/dc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/uda/command/ioctl
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/common
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/tsdrv/ts_drv/ts_drv_common

    EXTRA_CFLAGS += -I$(DRIVER_MODULE_DTM_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/event
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/core

    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_LOG_STANDARD_FAULT_EVENT
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=5
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=0

    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE


    EXTRA_CFLAGS += -DCFG_FEATURE_PG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_VFG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_AICPU
    EXTRA_CFLAGS += -DCFG_FEATURE_NEW_EVENT_CODE
    EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_LINK_STATUS
    EXTRA_CFLAGS += -DCFG_FEATURE_GET_CURRENT_EVENTINFO
    ccflags-y += -Wall -Werror

    asdrv_fms-objs += smf/dms_smf_init.o
    asdrv_fms-objs += smf/logdrv/log_drv_write_ring_buffer.o

    include $(DRIVER_MODULE_SMF_DIR)/core/dms_sensor_core.mk
    include $(DRIVER_MODULE_SMF_DIR)/event/dms_event.mk
    include $(DRIVER_MODULE_SMF_DIR)/sensor/dms_sensor.mk
else     # for cmake
    EXTRA_CFLAGS += -DCFG_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    EXTRA_CFLAGS += -DCFG_FEATURE_EP_MODE
    EXTRA_CFLAGS += -DCFG_FEATURE_NEW_EVENT_CODE
    EXTRA_CFLAGS += -DCFG_FEATURE_PG
    EXTRA_CFLAGS += -DCFG_FEATURE_VFG
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
    EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_LINK_STATUS
    EXTRA_CFLAGS += -DCFG_FEATURE_GET_CURRENT_EVENTINFO
    EXTRA_CFLAGS += -I${C_SEC_INCLUDE}
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/common
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/tsdrv/ts_drv/ts_drv_common
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/pbl/uda/command/ioctl
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_DTM_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/sensor
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/event
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_SMF_DIR)/core
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=5
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=0
	ifeq ($(ENABLE_BUILD_PRODUCT),TRUE)
		EXTRA_CFLAGS += -DENABLE_BUILD_PRODUCT
	endif
    ccflags-y += -Wall -Werror
    asdrv_fms-objs += smf/dms_smf_init.o
    asdrv_fms-objs += smf/logdrv/log_drv_write_ring_buffer.o
    include ${DRIVER_MODULE_SMF_DIR}/core/dms_sensor_core.mk
    include ${DRIVER_MODULE_SMF_DIR}/event/dms_event.mk
    include ${DRIVER_MODULE_SMF_DIR}/sensor/dms_sensor.mk
endif

EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
