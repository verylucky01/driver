# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

$(MODULE_NAME)-objs += trs_core/trs_fops.o trs_core/trs_ts_inst.o trs_core/trs_proc.o trs_core/trs_res_mng.o trs_core/trs_sqcq_map.o trs_core/trs_hw_sqcq.o trs_core/trs_sw_sqcq.o trs_core/trs_logic_cq.o trs_core/trs_cb_sqcq.o trs_core/trs_shm_sqcq.o trs_core/trs_proc_fs.o trs_core/trs_gdb_sqcq.o trs_core/trs_shr_proc.o trs_core/trs_shr_sqcq.o
CFG_FEATURE_TRACE_EVENT_FUNC_SUPPORT=y
ifeq ($(CFG_FEATURE_TRACE_EVENT_FUNC_SUPPORT),y)
    EXTRA_CFLAGS += -DCFG_FEATURE_TRACE_EVENT_FUNC
    $(MODULE_NAME)-objs += trs_core/trs_core_trace.o
endif

ifneq ($(filter $(PRODUCT), ascend950),)
    EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_APM
    EXTRA_CFLAGS += -DCFG_FEATURE_NOTSUPPORT_BOARD_CONFIG
endif

EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/trs
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/dc/trs_core
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trsbase/inc
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/trs_core
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/trs_core/command/ioctl
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsdrv/trs/trs_core/command/msg
EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include

EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
EXTRA_CFLAGS += -DCFG_FEATURE_TRS_CORE
EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV
EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_STREAM_TASK
