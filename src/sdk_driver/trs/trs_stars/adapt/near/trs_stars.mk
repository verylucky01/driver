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
    $(MODULE_NAME)-objs += trs_stars/comm/trs_stars_soc.o trs_stars/adapt/comm/trs_stars_func_com.o trs_stars/adapt/comm/src/stars_event_tbl_ns.o trs_stars/adapt/comm/src/stars_notify_tbl.o trs_stars/adapt/near/trs_stars_near_func.o
    $(MODULE_NAME)-objs += trs_stars/adapt/near/stars_v1/trs_stars_v1_func_adapt.o

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_stars/comm
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_stars/adapt/comm
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_stars/adapt/comm/src
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_stars/adapt/near
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_stars/adapt/soc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_stars/adapt/soc/cloud_v2
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/drv_devmng/drv_devmng_inc/

    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_STARS
else
    ccflags-y += -Wno-missing-prototypes -Wno-missing-declarations
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_STARS

    ifeq ($(DAVINCI_HIAI_DKMS), y)
        #for dkms
        DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc_open/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
        ifneq ($(filter $(TARGET_CHIP_ID), hi1910b),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc/mini_v3
        else ifneq ($(filter $(TARGET_CHIP_ID), hi1980b),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc/cloud_v2
        else ifneq ($(filter $(TARGET_CHIP_ID), hi1980d),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc/cloud_v4
        endif
    else
        DRIVER_SRC_BASE_DIR := $(DRIVER_KERNEL_DIR)

        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
        ifneq ($(filter $(PRODUCT), ascend310B ascend310Brc ascend310Brcesl ascend310Besl ascend310Brcemu ascend310Bemu),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc/mini_v3
        else ifneq ($(filter $(PRODUCT), ascend910B ascend910Besl ascend910Bemu),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc/cloud_v2
        else ifneq ($(filter $(PRODUCT), ascend910_95 ascend910_95esl),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc/cloud_v4
        else ifneq ($(filter $(PRODUCT), ascend910_55esl ascend910_55fpga ascend910_55 ascend910_96esl ascend910_96),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc/cloud_v5
        endif

    endif


    EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
    EXTRA_CFLAGS += -Iinclude/linux

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I${C_SEC_INCLUDE}
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/comm
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/comm
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/comm/src
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/near
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_stars/adapt/soc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/drv_devmng/drv_devmng_inc/

    $(MODULE_NAME)-objs += trs_stars/comm/trs_stars_soc.o trs_stars/adapt/comm/trs_stars_func_com.o trs_stars/adapt/comm/src/stars_event_tbl_ns.o trs_stars/adapt/comm/src/stars_notify_tbl.o trs_stars/adapt/near/trs_stars_near_func.o
    ifeq ($(DAVINCI_HIAI_DKMS),y)
        #for dkms
        ifneq ($(filter $(TARGET_CHIP_ID), hi1980d),)
            $(MODULE_NAME)-objs += trs_stars/adapt/near/stars_v2/trs_stars_v2_func_adapt.o trs_stars/adapt/comm/src/stars_cnt_notify_tbl.o
        else
            $(MODULE_NAME)-objs += trs_stars/adapt/near/stars_v1/trs_stars_v1_func_adapt.o
        endif
    else
        ifeq ($(filter $(PRODUCT), ascend910_95 ascend910_95esl ascend910_55esl ascend910_55fpga ascend910_55),)
            $(MODULE_NAME)-objs += trs_stars/adapt/near/stars_v1/trs_stars_v1_func_adapt.o
        else
            $(MODULE_NAME)-objs += trs_stars/adapt/near/stars_v2/trs_stars_v2_func_adapt.o trs_stars/adapt/comm/src/stars_cnt_notify_tbl.o
        endif
    endif
endif