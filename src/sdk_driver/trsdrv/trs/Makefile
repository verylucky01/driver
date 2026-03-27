# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

ifeq ($(Driver_Install_Mode), vnpu_guest)
    MODULE_NAME = asdrv_vtrs
else 
    MODULE_NAME = asdrv_trs
endif

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := trs_init.o

ifneq ($(filter $(PRODUCT), ascend950),)
    EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_APM
    EXTRA_CFLAGS += -DCFG_FEATURE_NOTSUPPORT_BOARD_CONFIG
endif

ifeq ($(Driver_Install_Mode), vnpu_guest) #asdrv_vtrs
    include $(DRIVER_KERNEL_DIR)/trsdrv/trs/lba/near/sriov_sec_enhanced/adapt/lba_sec_eh.mk
    include $(DRIVER_KERNEL_DIR)/trsdrv/trs/trs_core/trs_core.mk
else  #asdrv_trs
    include $(DRIVER_KERNEL_DIR)/trsdrv/trs/lba/near/sia/adapt/lba_sia.mk
    include $(DRIVER_KERNEL_DIR)/trsdrv/trs/trs_core/trs_core.mk
    include $(DRIVER_KERNEL_DIR)/trsdrv/trs/trs_stars/adapt/near/trs_stars.mk
    include $(DRIVER_KERNEL_DIR)/trsdrv/trs/shr_id/shr_id.mk
    ifneq ($(filter $(PRODUCT), ascend910B),)
        include $(DRIVER_KERNEL_DIR)/trsdrv/trs/lba/near/sriov_sec_enhanced/agent/lba_sec_eh_agent.mk
    endif
endif
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)

ccflags-y += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Wdate-time -Werror -Wfloat-equal -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla
