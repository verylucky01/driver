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
    asdrv_dpa-objs += udis/udis_management.o udis/udis_timer.o udis/udis_cmd.o
    asdrv_dpa-objs += udis/host/udis_msg.o udis/host/udis_data.o udis/host/udis_init.o udis/host/udis_addr.o

    EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dpa/udis
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/common
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/pbl/dev_urd
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dpa/inc

    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=4
    EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=2
    EXTRA_CFLAGS += -DDRV_HOST
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    ifneq ($(ASCEND910_93_EX),true)
        EXTRA_CFLAGS += -DCFG_FEATURE_UDIS_SUPPORT_VF
    endif

    include $(FEATURE_MK_PATH)
    EXTRA_CFLAGS += $(CONFIG_DEFINES)
else
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    ifeq ($(DAVINCI_HIAI_DKMS), y)
        #for dkms
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc_open/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/ascend_platform
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dpa/udis
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/common
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/dev_urd
        EXTRA_CFLAGS += -DDRV_HOST
        EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=4
        EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=2
        ifneq ($(filter $(TARGET_CHIP_ID), hi1980b),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
            EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
            EXTRA_CFLAGS += -DCONFIG_PLATFORM_910B
            ifneq ($(ASCEND910_93_EX),true)
                EXTRA_CFLAGS += -DCFG_FEATURE_UDIS_SUPPORT_VF
            endif
        endif

        asdrv_dpa-objs += udis/udis_management.o udis/udis_timer.o udis/udis_cmd.o udis/host/udis_msg.o udis/host/udis_data.o udis/host/udis_init.o udis/host/udis_addr.o
    else
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dpa/udis
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/common
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/pbl/dev_urd
        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
        EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=4
        EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=2
        EXTRA_CFLAGS += -DDRV_HOST
        EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

        ifneq ($(filter $(PRODUCT), ascend910B),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
            ifneq ($(ASCEND910_93_EX),true)
                EXTRA_CFLAGS += -DCFG_FEATURE_UDIS_SUPPORT_VF
            endif
        endif

        include $(FEATURE_MK_PATH)
        EXTRA_CFLAGS += $(CONFIG_DEFINES)

        asdrv_dpa-objs += udis/udis_management.o udis/udis_timer.o udis/udis_cmd.o

        asdrv_dpa-objs += udis/host/udis_msg.o udis/host/udis_data.o udis/host/udis_init.o udis/host/udis_addr.o
    endif

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dpa/inc
    EXTRA_CFLAGS += -Wfloat-equal
    EXTRA_CFLAGS += -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-format-nonliteral -Wno-empty-body
    EXTRA_CFLAGS += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Werror \
                    -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla
    ifeq ($(use_hitest),kernel_true)
        export HITESTCC := $(CC)
        export HITESTLD := $(LD)
        CC = hitest_gcc
        LD = hitest_ld
    endif

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif