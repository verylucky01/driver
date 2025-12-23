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
    ifeq ($(Driver_Install_Mode), vnpu_guest)
        MODULE_NAME = asdrv_vtrs
    else 
        MODULE_NAME = asdrv_trs
    endif

    obj-m += $(MODULE_NAME).o
    $(MODULE_NAME)-objs := trs_init.o

    ifeq ($(Driver_Install_Mode), vnpu_guest) #asdrv_vtrs
        include $(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/lba_sec_eh.mk
        include $(DRIVER_KERNEL_DIR)/trs/trs_core/trs_core.mk
    else  #asdrv_trs
        include $(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt/lba_sia.mk
        include $(DRIVER_KERNEL_DIR)/trs/trs_core/trs_core.mk
        include $(DRIVER_KERNEL_DIR)/trs/trs_stars/adapt/near/trs_stars.mk
        include $(DRIVER_KERNEL_DIR)/trs/shr_id/shr_id.mk
        include $(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/agent/lba_sec_eh_agent.mk
    endif

    ccflags-y += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Wdate-time -Werror -Wfloat-equal -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla

else
    ifeq ($(Driver_Install_Mode), vnpu_guest)
        MODULE_NAME = asdrv_vtrs
    else 
        MODULE_NAME = asdrv_trs
    endif

    obj-m += $(MODULE_NAME).o
    $(MODULE_NAME)-objs := trs_init.o

    ifeq ($(Driver_Install_Mode), vnpu_guest) #asdrv_vtrs
        ifeq ($(DAVINCI_HIAI_DKMS), y)
            ifneq ($(filter $(TARGET_CHIP_ID), hi1980b hi1980d),)
                include $(HIAI_DKMS_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/lba_sec_eh.mk
                include $(HIAI_DKMS_DIR)/trs/trs_core/trs_core.mk
            endif
        else
            ifneq ($(filter $(PRODUCT), ascend910B ascend910_95 ascend910_95esl),)
                include $(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/adapt/lba_sec_eh.mk
                include $(DRIVER_KERNEL_DIR)/trs/trs_core/trs_core.mk
            endif
        endif
    else  #asdrv_trs
        ifeq ($(DAVINCI_HIAI_DKMS), y)
            ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980b hi1980d),)
                include $(HIAI_DKMS_DIR)/trs/lba/near/sia/adapt/lba_sia.mk
                include $(HIAI_DKMS_DIR)/trs/trs_core/trs_core.mk
            endif

            ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980b hi1980d),)
                include $(HIAI_DKMS_DIR)/trs/trs_stars/adapt/near/trs_stars.mk
                include $(HIAI_DKMS_DIR)/trs/shr_id/shr_id.mk
            endif

            ifneq ($(filter $(TARGET_CHIP_ID), hi1980b),)
                include $(HIAI_DKMS_DIR)/trs/lba/near/sriov_sec_enhanced/agent/lba_sec_eh_agent.mk
            endif
        else
            ifneq ($(filter $(PRODUCT), ascend310B ascend310Brc ascend910B ascend910_95 ascend910_95esl ascend910_96 ascend910_96esl),)
                include $(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt/lba_sia.mk
                include $(DRIVER_KERNEL_DIR)/trs/trs_core/trs_core.mk
            endif

            ifneq ($(filter $(PRODUCT), ascend310B ascend910B ascend910_95 ascend910_95esl ascend910_96 ascend910_96esl),)
                include $(DRIVER_KERNEL_DIR)/trs/trs_stars/adapt/near/trs_stars.mk
                include $(DRIVER_KERNEL_DIR)/trs/shr_id/shr_id.mk
            endif

            ifneq ($(filter $(PRODUCT), ascend910B),)
                include $(DRIVER_KERNEL_DIR)/trs/lba/near/sriov_sec_enhanced/agent/lba_sec_eh_agent.mk
            endif
        endif
    endif

    ifeq ($(use_hitest),kernel_true)
        export HITESTCC := $(CC)
        export HITESTLD := $(LD)
        CC = hitest_gcc
        LD = hitest_ld
    endif
endif