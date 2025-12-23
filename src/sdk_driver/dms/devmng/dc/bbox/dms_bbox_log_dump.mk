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
    asdrv_dms-y += devmng/dc/bbox/dms_bbox_log_dump.o
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/include
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/tsdrv/ts_drv/ts_drv_common
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/ascend910
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
else #for CMake
    asdrv_dms-y += devmng/dc/bbox/dms_bbox_log_dump.o
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/include
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/tsdrv/ts_drv/ts_drv_common
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/ascend910
    EXTRA_CFLAGS += -DCFG_FEATURE_LOG_DUMP_FROM_PCIE
endif