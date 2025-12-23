# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifeq ($(TARGET_BUILD_TYPE),debug)
	EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
endif

ifneq ($(NOT_SUPPORT_SP), y)
	EXTRA_CFLAGS += -fstack-protector-all
endif

$(MODULE_NAME)-objs += soc_resmng/soc_resmng.o soc_resmng/soc_subsys_ts.o soc_resmng/soc_res_sync.o soc_resmng/soc_cgroup_parse.o soc_resmng/soc_interface.o

ifeq ($(TARGET_BUILD_TYPE),debug)
	$(MODULE_NAME)-objs += soc_resmng/soc_proc_fs.o
endif

include $(FEATURE_MK_PATH)
EXTRA_CFLAGS += $(CONFIG_DEFINES)

ifeq ($(DAVINCI_HIAI_DKMS),y)
	DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)
	EXTRA_CFLAGS += -DCFG_HOST_ENV
	EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG

	EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_LINK_STATUS
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_DIE

	$(MODULE_NAME)-objs += soc_resmng/soc_topo_query.o
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/ascend_platform
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
else
	DRIVER_SRC_BASE_DIR := $(DRIVER_KERNEL_DIR)/src
	$(MODULE_NAME)-objs += soc_resmng/soc_topo_query.o

	EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
	EXTRA_CFLAGS += -DCFG_HOST_ENV
	EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_LINK_STATUS
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_DIE

	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
	EXTRA_CFLAGS += -I${C_SEC_INCLUDE}
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/pbl/soc_resmng/command/ioctl
	EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)

endif

EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trsbase/inc
EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/inc
