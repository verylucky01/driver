# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

ifeq ($(DAVINCI_HIAI_DKMS),y)
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svmdrv/common
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/kernel_adapt
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/kernel_adapt/include

endif

EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
EXTRA_CFLAGS += -I${C_SEC_INCLUDE}
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include
EXTRA_CFLAGS += -I$(DRIVER_CUST_INC_DIR)
EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
EXTRA_CFLAGS += -fstack-protector-all

# 模块名
obj-m += npu_peermem.o