# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

obj-m += asdrv_fms.o
asdrv_fms-objs := fms_module.o

ifeq ($(DAVINCI_HIAI_DKMS),y)
	DRIVER_SOURCE_DIR := $(HIAI_DKMS_DIR)
else
	DRIVER_SOURCE_DIR := $(DRIVER_KERNEL_DIR)
endif

include ${DRIVER_SOURCE_DIR}/fms/dtm/dtm.mk
include ${DRIVER_SOURCE_DIR}/fms/smf/smf.mk

ifeq ($(DAVINCI_HIAI_DKMS),y)
    include ${DRIVER_SOURCE_DIR}/fms/soft_fault/soft_fault.mk
else
    include ${DRIVER_SOURCE_DIR}/fms/soft_fault/soft_fault.mk
endif

EXTRA_CFLAGS += -Wno-type-limits

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
