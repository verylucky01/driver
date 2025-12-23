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
	asdrv_dms-y += devmng/event/host/dms_event_host.o
	asdrv_dms-y += devmng/event/dms_adapt.o

	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_platform_host/ascend910
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/include
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/ascend910
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/event
else #for CMake & ctrl cpu open
	asdrv_dms-y += devmng/event/host/dms_event_host.o
	asdrv_dms-y += devmng/event/dms_adapt.o
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/ascend910
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/dms/dc/drv_platform/ts_platform_host/ascend910

	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/event
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/vmng
endif