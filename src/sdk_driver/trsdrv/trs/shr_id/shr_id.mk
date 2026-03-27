# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

$(MODULE_NAME)-objs += shr_id/trs_shr_id.o shr_id/trs_shr_id_fops.o shr_id/trs_shr_id_node.o shr_id/trs_shr_id_event_update.o
ifneq ($(filter $(PRODUCT), ascend910B),)
    $(MODULE_NAME)-objs += shr_id/trs_shr_id_spod.o shr_id/trs_shr_id_spod_msg_recv.o shr_id/trs_shr_id_spod_event_update.o
endif

EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/trs
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trsbase/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/shr_id
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/shr_id/command/ioctl
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/platform/dc/drv_platform/ts_platform_host/ascend910
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/tsdrv/ts_drv/ts_drv_common

EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
EXTRA_CFLAGS += -DCFG_FEATURE_TRS_SHR_ID
ifneq ($(filter $(PRODUCT), ascend910B ascend950),)
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
endif
