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
    $(MODULE_NAME)-objs += shr_id/trs_shr_id.o shr_id/trs_shr_id_fops.o shr_id/trs_shr_id_node.o shr_id/trs_shr_id_event_update.o
    $(MODULE_NAME)-objs += shr_id/trs_shr_id_spod.o shr_id/trs_shr_id_spod_msg_recv.o shr_id/trs_shr_id_spod_event_update.o

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/shr_id
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/shr_id/command/ioctl
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/platform/dc/drv_platform/ts_platform_host/ascend910
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/tsdrv/ts_drv/ts_drv_common

    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_SHR_ID
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
else
    ccflags-y += -Wno-missing-prototypes -Wno-missing-declarations
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

    EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
    ccflags-y += -Wall -Werror

    $(MODULE_NAME)-objs += shr_id/trs_shr_id.o shr_id/trs_shr_id_fops.o shr_id/trs_shr_id_node.o shr_id/trs_shr_id_event_update.o

    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG

    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_SHR_ID

    ifeq ($(DAVINCI_HIAI_DKMS), y)
        #for dkms
        DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)
        ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980b),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif

        ifeq ($(TARGET_CHIP_ID), hi1980b)
            #for spod
            EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc_open/inc
            $(MODULE_NAME)-objs += shr_id/trs_shr_id_spod.o shr_id/trs_shr_id_spod_msg_recv.o shr_id/trs_shr_id_spod_event_update.o
        endif

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver

        #for trs_shr_id_event_update.o
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/devmng/drv_devmng/drv_devmng_inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_drv_common
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_platform_host/ascend910
        #for trs_shr_id_event_update.o end
    else
        DRIVER_SRC_BASE_DIR := $(DRIVER_KERNEL_DIR)
        ifneq ($(filter $(PRODUCT), ascend310p ascend910 ascend910B ascend310B),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif

        ifneq ($(filter $(PRODUCT), ascend910B),)
            #for spod
            $(MODULE_NAME)-objs += shr_id/trs_shr_id_spod.o shr_id/trs_shr_id_spod_msg_recv.o shr_id/trs_shr_id_spod_event_update.o
        endif

        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    endif

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/shr_id
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/shr_id/command/ioctl
    EXTRA_CFLAGS += -I$(TOP_DIR)/drivers/ai_sdk/arc/linux/kernel_open/src/kernel_adapt/include

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/platform/dc/drv_platform/ts_platform_host/ascend910
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/tsdrv/ts_drv/ts_drv_common
endif
