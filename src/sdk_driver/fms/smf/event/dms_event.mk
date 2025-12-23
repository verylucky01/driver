# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/fms/smf/event
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/vmng
EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/devmng/drv_devmng/drv_devmng_inc

ifeq ($(DAVINCI_HIAI_DKMS),y)
	asdrv_fms-y += smf/event/dms_event.o
	asdrv_fms-y += smf/event/dms_event_converge.o
	asdrv_fms-y += smf/event/dms_event_distribute.o
	asdrv_fms-y += smf/event/dms_event_distribute_proc.o
	asdrv_fms-y += smf/event/dms_event_dfx.o
	asdrv_fms-y += smf/event/smf_event_adapt.o

	EXTRA_CFLAGS += -I${HIAI_DKMS_DIR}/ts_drv_common
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_platform_host/ascend910
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/command/ioctl
else #for CMake & ctrl cpu open
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/dms/dc/drv_platform/ts_platform_host/ascend910
	EXTRA_CFLAGS += -I${DRIVER_KERNEL_DIR}/src/tsdrv/ts_drv/ts_drv_common
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/command/ioctl

	asdrv_fms-y += smf/event/dms_event.o
	asdrv_fms-y += smf/event/dms_event_converge.o
	asdrv_fms-y += smf/event/dms_event_distribute.o
	asdrv_fms-y += smf/event/dms_event_distribute_proc.o
	asdrv_fms-y += smf/event/dms_event_dfx.o
	asdrv_fms-y += smf/event/smf_event_adapt.o
endif
