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

	ifneq ($(NOT_SUPPORT_SP), y)
		EXTRA_CFLAGS += -fstack-protector-all
	endif

	include $(DRIVER_MODULE_DEVMNG_DIR)/dc/heart_beat/heartbeat.mk

	ccflags-y += -Wall -Wno-missing-prototypes -Wno-missing-declarations
	ifeq ($(Driver_Install_Mode),vnpu_guest)
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/time/dms_time.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/urd_forward/dms_urd_forward.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/drv_devmng_host.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/core/dms_core.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/custom/dms_custom.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/event/dms_event_adapt.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/hotreset/dms_hotreset.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/status/dms_status.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/config/dms_config.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/product/dms_product.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/chip_dev/dms_chip_dev.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/bbox/dms_bbox_log_dump.mk
	else
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/time/dms_time.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/urd_forward/dms_urd_forward.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/drv_devmng_host.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/core/dms_core.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/custom/dms_custom.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/event/dms_event_adapt.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/hotreset/dms_hotreset.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/status/dms_status.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/config/dms_config.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/product/dms_product.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/chip_dev/dms_chip_dev.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/bbox/dms_bbox_log_dump.mk
		include $(DRIVER_MODULE_DEVMNG_DIR)/dc/hccs/dms_hccs.mk
		asdrv_dms-y += devmng/drv_devmng/drv_devmng_common/devdrv_spod_info.o
		asdrv_dms-y += devmng/dc/status/dms_spod_info.o
		EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_GET_SPOD_PING_INFO
	endif
	EXTRA_CFLAGS += -DCFG_FEATURE_PG
	EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
	EXTRA_CFLAGS += -DCFG_FEATURE_VFG
	EXTRA_CFLAGS += -DCFG_FEATURE_NEW_EVENT_CODE
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_INFO_FROM_DEVINFO
	EXTRA_CFLAGS += -DCFG_FEATURE_ERRORCODE_ON_NEW_CHIPS
	EXTRA_CFLAGS += -DCFG_FEATURE_TRS_HB_REFACTOR
	EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_LINK_STATUS
	EXTRA_CFLAGS += -DCFG_FEATURE_GET_CURRENT_EVENTINFO
	EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
	EXTRA_CFLAGS += -DCFG_FEATURE_KA
	EXTRA_CFLAGS += -DCFG_HOST_ENV
	EXTRA_CFLAGS += -DCFG_FEATURE_HEART_BEAT
	EXTRA_CFLAGS += -DCFG_FEATURE_HEALTH_ERR_CODE
	EXTRA_CFLAGS += -DCFG_FEATURE_DEV_TOPOLOGY
	EXTRA_CFLAGS += -DCFG_FEATURE_PCIE_HOST_DEVICE_COMM
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/ascend_platform
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/dev_urd
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/dev_urd/dc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/common
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/fms/smf/sensor
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/fms/smf
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/fms/smf/event
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/fms/dtm
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/include

	include $(DRIVER_MODULE_DEVMNG_DIR)/dc/log/dms_log_info.mk
else #for CMake & ctrl cpu open
	ifneq ($(NOT_SUPPORT_SP), y)
		EXTRA_CFLAGS += -fstack-protector-all
	endif

	ccflags-y += -Wall -funsigned-char -Wextra -Werror -Wformat=2 -Wfloat-equal -Wcast-align -Wundef $(WDATE_TIME)
	ccflags-y += -Wno-unused-parameter -Wno-sign-compare  -Wno-missing-field-initializers -Wno-format-nonliteral -Wno-empty-body -Wno-missing-prototypes -Wno-missing-declarations
	ccflags-y += -fno-common -fstack-protector-all -pipe -s -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wunused -Wvla

	EXTRA_CFLAGS += -DCFG_FEATURE_KA
	EXTRA_CFLAGS += -DCFG_HOST_ENV
	EXTRA_CFLAGS += -DCFG_FEATURE_DEV_TOPOLOGY
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_INFO_FROM_DEVINFO
	EXTRA_CFLAGS += -DCFG_FEATURE_GET_CURRENT_EVENTINFO
	EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
	EXTRA_CFLAGS += -DCFG_FEATURE_HEART_BEAT
	EXTRA_CFLAGS += -DCFG_FEATURE_HEALTH_ERR_CODE
	EXTRA_CFLAGS += -DCFG_FEATURE_NEW_EVENT_CODE
	EXTRA_CFLAGS += -DCFG_FEATURE_ERRORCODE_ON_NEW_CHIPS
	EXTRA_CFLAGS += -DCFG_FEATURE_TRS_HB_REFACTOR
	EXTRA_CFLAGS += -DCFG_FEATURE_UDIS
	EXTRA_CFLAGS += -DCFG_FEATURE_PG
	EXTRA_CFLAGS += -DCFG_FEATURE_VFG
	EXTRA_CFLAGS += -DCFG_FEATURE_SRIOV
	EXTRA_CFLAGS += -DCFG_FEATURE_TOPOLOGY_BY_HCCS_LINK_STATUS
	EXTRA_CFLAGS += -DCFG_FEATURE_GET_CURRENT_EVENTINFO
	EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_GET_SPOD_PING_INFO

	include $(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/drv_devmng_host.mk
	include $(DRIVER_MODULE_DEVMNG_DIR)/dc/dms.mk
	include $(DRIVER_MODULE_DEVMNG_DIR)/config/dms_config.mk
	include $(DRIVER_MODULE_DEVMNG_DIR)/product/dms_product.mk

	EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/ascend_platform
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/common
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/dev_urd
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/fms/smf/event
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/fms/smf/sensor
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/fms/smf/sensor/config
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/fms/soft_fault
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/include
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_inc
	EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
	EXTRA_CFLAGS += -I$(TOP_DIR)/drivers/firmware/hiss/inc
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/dev_urd/dc

	asdrv_dms-y += devmng/drv_devmng/drv_devmng_common/devdrv_spod_info.o
	asdrv_dms-y += devmng/dc/status/dms_spod_info.o
endif #end for CMake
