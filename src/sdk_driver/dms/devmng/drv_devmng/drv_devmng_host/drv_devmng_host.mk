# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifeq ($(TARGET_BUILD_TYPE),debug)
	EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
endif

ifneq ($(NOT_SUPPORT_SP), y)
	EXTRA_CFLAGS += -fstack-protector-all
endif

ifeq ($(DAVINCI_HIAI_DKMS),y)
# for DKMS compile
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc_open/inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/npu_inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dp_proc_mng/common
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/fms/soft_fault
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/command/ioctl
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/include
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/time
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/power
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/urd_forward
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_platform_host/ascend910
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_drv_host
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/ts_drv_common
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/ascend910
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_common

	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
	EXTRA_CFLAGS += -DCFG_FEATURE_VFIO
	EXTRA_CFLAGS += -DCFG_FEATURE_NOTIFY_REBOOT
	EXTRA_CFLAGS += -DCFG_FEATURE_VASCEND
	EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V2
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_DIE
	EXTRA_CFLAGS += -DCFG_FEATURE_PCIE_BBOX_ADDR
	EXTRA_CFLAGS += -DCFG_FEATURE_VF_USE_DEVID
	EXTRA_CFLAGS += -DCFG_FEATURE_VDEV_MEMORY_FROM_DEVMNG_H2D
	EXTRA_CFLAGS += -DCFG_FEATURE_BIND_TGID
	EXTRA_CFLAGS += -DCFG_FEATURE_GET_CURRENT_EVENTINFO
	EXTRA_CFLAGS += -DCFG_FEATURE_BBOX_KDUMP
	EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
	EXTRA_CFLAGS += -DCFG_FEATURE_PCIE_HOST_DEVICE_COMM
	EXTRA_CFLAGS += -DCFG_EDGE_HOST

	BUILD_PREFIX_DIR := devmng/drv_devmng/drv_devmng_host/ascend910

	asdrv_dms-y += devmng/drv_devmng/drv_devmng_common/devdrv_pcie.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/hvdevmng_init.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/hvdevmng_cmd_proc.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_pid_map.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_pid_map_adapt.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_chip_dev_map.o
	asdrv_dms-y += devmng/drv_devmng/drv_devmng_common/dev_mnt_vdevice.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_msg.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_pm.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_driver_pm.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_rand.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_black_box.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_container.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_device_online.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/drv_log.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/tsdrv_status.o

	ifeq ($(TARGET_PRODUCT),cloud)
		ccflags-y += -Wall -Werror
	endif

else
# for CMake compile
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/tsdrv/ts_drv/ts_drv_host
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/tsdrv/ts_drv/ts_drv_common

	EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/libc_sec
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/uda
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/pbl/uda/command/ioctl
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/dms/command/ioctl

	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_inc
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/event
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/time
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/power
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/include
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/urd_forward

	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
	EXTRA_CFLAGS += -DCFG_FEATURE_VASCEND
	EXTRA_CFLAGS += -DCFG_FEATURE_BBOX_KDUMP
	EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V2
	EXTRA_CFLAGS += -DCFG_FEATURE_CHIP_DIE
	EXTRA_CFLAGS += -DCFG_FEATURE_PCIE_BBOX_ADDR
	EXTRA_CFLAGS += -DCFG_FEATURE_VF_USE_DEVID
	EXTRA_CFLAGS += -DCFG_FEATURE_VDEV_MEMORY_FROM_DEVMNG_H2D
	EXTRA_CFLAGS += -DCFG_FEATURE_BIND_TGID
	EXTRA_CFLAGS += -DCFG_FEATURE_GET_CURRENT_EVENTINFO

	EXTRA_CFLAGS += -DCFG_FEATURE_VFIO
	EXTRA_CFLAGS += -I$(DRIVER_SOURCE_DIR)/platform/dc/drv_platform/ts_platform_host/ascend910
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_host/ascend910
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/drv_devmng/drv_devmng_common

	EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
	EXTRA_CFLAGS += -DCFG_FEATURE_PCIE_HOST_DEVICE_COMM
	EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
	EXTRA_CFLAGS += -DCFG_EDGE_HOST
	EXTRA_CFLAGS += -DCFG_FEATURE_INUSE_NUM_DYNAMIC
	BUILD_PREFIX_DIR := devmng/drv_devmng/drv_devmng_host/ascend910

	asdrv_dms-y += $(BUILD_PREFIX_DIR)/hvdevmng_init.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/hvdevmng_cmd_proc.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_pid_map.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_pid_map_adapt.o
	asdrv_dms-y += devmng/drv_devmng/drv_devmng_common/devdrv_pcie.o
	asdrv_dms-y += devmng/drv_devmng/drv_devmng_common/dev_mnt_vdevice.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_chip_dev_map.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_msg.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_pm.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_driver_pm.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_rand.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_black_box.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_manager_container.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/devdrv_device_online.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/drv_log.o
	asdrv_dms-y += $(BUILD_PREFIX_DIR)/tsdrv_status.o

	ifeq ($(ENABLE_BUILD_PRODUCT),TRUE)
		EXTRA_CFLAGS += -DENABLE_BUILD_PRODUCT
	endif

endif

EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/dc/drv_devmng
EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
