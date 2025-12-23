# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
KERNEL_DIR :=$(srctree)

ifeq ($(DAVINCI_HIAI_DKMS),y) # for DKMS: TOP_DIR is NULL
    EXTRA_CFLAGS += -DCFG_FEATURE_HEART_BEAT
    EXTRA_CFLAGS += -DCFG_EDGE_HOST
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/devmng/dc/heart_beat
    asdrv_dms-y += devmng/dc/heart_beat/hb_host.o
    asdrv_dms-y += devmng/dc/heart_beat/hb_read.o
    asdrv_dms-y += devmng/dc/heart_beat/hb_write.o
    asdrv_dms-y += devmng/dc/heart_beat/heartbeat_init.o
else # for cmake and control cpu open
    asdrv_dms-y += devmng/dc/heart_beat/hb_host.o
    asdrv_dms-y += devmng/dc/heart_beat/hb_read.o
    asdrv_dms-y += devmng/dc/heart_beat/hb_write.o
    asdrv_dms-y += devmng/dc/heart_beat/heartbeat_init.o
    EXTRA_CFLAGS += -DCFG_EDGE_HOST
    EXTRA_CFLAGS += -DCFG_FEATURE_HEART_BEAT
    EXTRA_CFLAGS += -DCFG_FEATURE_HEARTBEAT_CNT_PCIE
    EXTRA_CFLAGS += -I${DRIVER_MODULE_DEVMNG_DIR}/dc/heart_beat
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
endif
