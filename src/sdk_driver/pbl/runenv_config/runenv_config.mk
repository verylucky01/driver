# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifeq ($(DAVINCI_HIAI_DKMS), y)
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/runenv_config
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
else
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/pbl/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/pbl/runenv_config
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV
    EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
endif

ifneq ($(NOT_SUPPORT_SP), y)
    EXTRA_CFLAGS += -fstack-protector-all
endif

ccflags-y += -Wall -Werror -Wtrampolines $(WDATE_TIME) -Wfloat-equal -Wvla -Wundef
$(MODULE_NAME)-objs += runenv_config/runenv_config_module.o runenv_config/docker_query.o runenv_config/vm_query.o runenv_config/drv_version_query.o

include $(FEATURE_MK_PATH)
EXTRA_CFLAGS += $(CONFIG_DEFINES)
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
