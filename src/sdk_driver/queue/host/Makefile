# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
obj-m   += asdrv_queue.o    # host
asdrv_queue-objs := common/queue_module.o common/queue_context.o common/queue_dma.o common/queue_proc_fs.o common/queue_status_record.o queue_fops.o queue_host_msg.o queue_ctx_private.o common/queue_channel.o

EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc \
    -I$(DRIVER_KERNEL_DIR)/inc/pbl \
    -I$(DRIVER_KERNEL_DIR)/queue/host/common \
    -I$(DRIVER_KERNEL_DIR)/queue/host \
    -I$(DRIVER_KERNEL_DIR)/queue/command/ioctl/ \
    -I$(DRIVER_KERNEL_DIR)/queue/command/msg/ \
    -I$(DRIVER_KERNEL_DIR)/esched/command/ioctl/ \
    -I$(DRIVER_KERNEL_DIR)/esched/command/msg/ \
    -I$(DRIVER_HAL_INC_DIR) \
    -I${C_SEC_INCLUDE}

EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
EXTRA_CFLAGS += -DDRV_HOST
ccflags-y += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Wdate-time -Wfloat-equal -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla
