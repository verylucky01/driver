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
	ifeq ($(DAVINCI_HIAI_DKMS),y) #for DKMS
		asdrv_dms-y += devmng/status/dms_osc_freq.o
		asdrv_dms-y += devmng/dc/status/dms_vdev.o
		asdrv_dms-y += devmng/dc/status/dms_dev_topology.o
		asdrv_dms-y += devmng/dc/status/dms_host_aicpu_info.o
		asdrv_dms-y += devmng/dc/status/dms_host_notify_ready.o
		asdrv_dms-y += devmng/status/dms_feature_pub.o
		asdrv_dms-y += devmng/status/dms_basic_info.o

		asdrv_dms-y += devmng/status/dms_chip_hal.o
		asdrv_dms-y += devmng/status/dms_chip_info.o
		asdrv_dms-y += devmng/dc/status/dms_chip_info_adapt.o
		EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/status
		EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/dc/status
	endif
else #for CMake & ctrl cpu open
	asdrv_dms-y += devmng/status/dms_osc_freq.o
	asdrv_dms-y += devmng/dc/status/dms_vdev.o
	asdrv_dms-y += devmng/dc/status/dms_dev_topology.o
	asdrv_dms-y += devmng/dc/status/dms_host_aicpu_info.o
	asdrv_dms-y += devmng/dc/status/dms_host_notify_ready.o
	asdrv_dms-y += devmng/status/dms_feature_pub.o
	asdrv_dms-y += devmng/status/dms_basic_info.o
	asdrv_dms-y += devmng/status/dms_chip_hal.o
	asdrv_dms-y += devmng/status/dms_chip_info.o
	asdrv_dms-y += devmng/dc/status/dms_chip_info_adapt.o

	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/status
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/dc/status
endif