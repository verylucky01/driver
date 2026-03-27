# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

obj-m += asdrv_dpa.o
asdrv_dpa-objs := dpa_init.o

# apm/dp_proc_mng
ifneq ($(filter $(PRODUCT), ascend950),)
	include ${DRIVER_KERNEL_DIR}/dpa/apm/apm.mk
else ifneq ($(filter $(PRODUCT), ascend910B),)
	include ${DRIVER_KERNEL_DIR}/dpa/dp_proc_mng/master/pmaster/dp_proc_mng_host.mk
endif

#udis
include ${DRIVER_KERNEL_DIR}/dpa/udis/udis.mk

# rmo
include ${DRIVER_KERNEL_DIR}/dpa/rmo/rmo.mk

EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dpa/inc
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include

ccflags-y += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Wdate-time -Werror -Wfloat-equal -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla
