# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
MODULE_NAME = asdrv_dms

ifeq ($(DAVINCI_HIAI_DKMS),y)
	DRIVER_SOURCE_DIR := $(HIAI_DKMS_DIR)
else
	DRIVER_SOURCE_DIR := $(DRIVER_KERNEL_DIR)
endif

DRIVER_MODULE_DEVMNG_DIR := $(DRIVER_SOURCE_DIR)/dms/devmng

include $(FEATURE_MK_PATH)
EXTRA_CFLAGS += $(CONFIG_DEFINES)

EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_MODULE_TYPE=3
EXTRA_CFLAGS += -DCFG_FEATURE_MEMALLOC_SUBMODULE_TYPE=0

EXTRA_CFLAGS += -Wno-type-limits

ifeq ($(DAVINCI_HIAI_DKMS),y)
	obj-m += asdrv_dms.o
	asdrv_dms-y += dms_module.o

	include $(DRIVER_SOURCE_DIR)/dms/devmng/devmng.mk
else
	obj-m += asdrv_dms.o
	asdrv_dms-y += dms_module.o

	include $(DRIVER_SOURCE_DIR)/dms/devmng/devmng.mk
endif
