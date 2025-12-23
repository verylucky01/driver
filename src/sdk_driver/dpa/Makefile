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
    obj-m += asdrv_dpa.o
    asdrv_dpa-objs := dpa_init.o

    # dp_proc_mng
    include ${DRIVER_KERNEL_DIR}/dpa/dp_proc_mng/master/pmaster/dp_proc_mng_host.mk

    #udis
    include ${DRIVER_KERNEL_DIR}/dpa/udis/udis.mk

    # rmo
    include ${DRIVER_KERNEL_DIR}/dpa/rmo/rmo.mk

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dpa/inc

    ccflags-y += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Wdate-time -Werror -Wfloat-equal -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla

else
    obj-m += asdrv_dpa.o
    asdrv_dpa-objs := dpa_init.o
    ifeq ($(DAVINCI_HIAI_DKMS), y)
        # dp_proc_mng/apm
        ifneq ($(filter $(TARGET_CHIP_ID), hi1980d),)
            include $(HIAI_DKMS_DIR)/dpa/apm/apm.mk
        else ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980b hi1980),)
            include $(HIAI_DKMS_DIR)/dpa/dp_proc_mng/pmaster/dp_proc_mng_host.mk
        else ifeq ($(TARGET_CHIP_ID), $(ASCEND_FLORENCE_DC_V10))
            include $(HIAI_DKMS_DIR)/dpa/dp_proc_mng/pmaster/dp_proc_mng_host.mk
        endif

        # rmo
        ifneq ($(filter $(TARGET_CHIP_ID), hi1980b),)
            include $(HIAI_DKMS_DIR)/dpa/rmo/rmo.mk
        endif

        #udis
        ifneq ($(filter $(TARGET_CHIP_ID), hi1980b),)
            include $(HIAI_DKMS_DIR)/dpa/udis/udis.mk
        endif

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dpa/inc
    else
        # dp_proc_mng/apm
        ifneq ($(filter $(PRODUCT), ascend910_95 ascend910_95esl ascend910_55 ascend910_55fpga ascend910_55esl ascend910_96 ascend910_96esl),)
		    include ${DRIVER_KERNEL_DIR}/dpa/apm/apm.mk
        else ifneq ($(filter $(PRODUCT), ascend310B ascend310p ascend910B ascend910),)
            include ${DRIVER_KERNEL_DIR}/dpa/dp_proc_mng/master/pmaster/dp_proc_mng_host.mk
        endif

        # rmo
        ifneq ($(filter $(PRODUCT), ascend910B),)
            include ${DRIVER_KERNEL_DIR}/dpa/rmo/rmo.mk
        endif

        #udis
        ifneq ($(filter $(PRODUCT), ascend910B),)
            include ${DRIVER_KERNEL_DIR}/dpa/udis/udis.mk
        endif

        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dpa/inc
    endif

    ifeq ($(use_hitest),kernel_true)
        export HITESTCC := $(CC)
        export HITESTLD := $(LD)
        CC = hitest_gcc
        LD = hitest_ld
    endif
endif