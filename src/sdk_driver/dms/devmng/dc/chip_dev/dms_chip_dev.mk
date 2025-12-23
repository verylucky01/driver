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
	asdrv_dms-y += devmng/dc/chip_dev/dms_chip_dev.o
	asdrv_dms-y += devmng/dc/chip_dev/dms_chip_dev_map.o
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/dc/chip_dev
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_DIE
	EXTRA_CFLAGS += -DCFG_EDGE_HOST
else #for CMake
	asdrv_dms-y += devmng/dc/chip_dev/dms_chip_dev.o
	asdrv_dms-y += devmng/dc/chip_dev/dms_chip_dev_map.o
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/dc/chip_dev
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_DIE
	EXTRA_CFLAGS += -DCFG_EDGE_HOST
endif

EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
EXTRA_CFLAGS += -Iinclude/linux