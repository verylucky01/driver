# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifeq ($(DAVINCI_HIAI_DKMS), y) # for dkms compile
	EXTRA_CFLAGS += -DDRV_HOST
	EXTRA_CFLAGS += -DCFG_ENV_HOST
	EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
	EXTRA_CFLAGS += -DSSAPI_USE_MAMI
	EXTRA_CFLAGS += -DCFG_FEATURE_CDMA_LOAD_IMAGE
	EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_RUN_INSTALL
	EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/command/msg/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/inc/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/unrelease/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/host/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/host/dev_online/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/host/load_image/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/host/msg/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/comm/ub/host/unit/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/pbl/msg_chan/command/msg/
	EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/kernel_adapt/include/
else # for non-dkms compile
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/command/msg/
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/pbl/msg_chan/command/msg/
	ifeq ($(PRODUCT_SIDE), host)
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/host/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/host/dev_online/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/host/load_image/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/host/msg/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/host/unit/
		EXTRA_CFLAGS += -DDRV_HOST
		EXTRA_CFLAGS += -DCFG_ENV_HOST
		ifneq ($(filter $(PRODUCT), ascend950),)
			EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
		endif
	else
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/device/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/device/dev_online/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/device/load_image/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/device/msg/
		EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/device/unit/
		EXTRA_CFLAGS += -DCFG_ENV_DEV
	endif

	EXTRA_CFLAGS += -I$(TOP_DIR)/abl/libc_sec/include
	EXTRA_CFLAGS += -I${TOP_DIR}/inc/driver
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/pbl/dev_urd
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/pbl/dev_urd/dc
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/inc/
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/comm/ub/unrelease/
	EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/src/kernel_adapt/include/
	EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernel/include
	EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernel/include/ub/ubus
	EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernel/drivers/ub/ubus
	EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernel/include/ub/urma/udma/
	EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernel/include/uapi/ub/urma/udma/
	EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernelspace/urma/code/kmod/ubcore/include
	EXTRA_CFLAGS += -Iinclude/linux
	EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
	EXTRA_CFLAGS += -DCFG_FEATURE_DRV_LOG
	EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
	ifneq ($(filter $(PRODUCT), ascend950esl ascend910_96 ascend910_96esl),)
		EXTRA_CFLAGS += -DCFG_FEATURE_UB_PCIE_SEL_BY_STUB
		ifneq ($(filter $(PRODUCT), ascend910_96 ascend910_96esl),)
			EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V5
			EXTRA_CFLAGS += -DSSAPI_USE_MAMI
		endif
		ifneq ($(filter $(PRODUCT), ascend950esl ascend910_96esl),)
			EXTRA_CFLAGS += -DCFG_PLATFORM_ESL
		endif
		ifneq ($(filter $(PRODUCT), ascend910_96),)
			EXTRA_CFLAGS += -DCFG_PLATFORM_FPGA
		endif
	endif
	ifneq ($(filter $(PRODUCT), ascend950),)
		EXTRA_CFLAGS += -DSSAPI_USE_MAMI
		EXTRA_CFLAGS += -DCFG_FEATURE_CDMA_LOAD_IMAGE
		EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_RUN_INSTALL
	endif

endif

MODULE_NAME := asdrv_ub
obj-m :=$(MODULE_NAME).o
$(MODULE_NAME)-objs := common/ascend_ub_main.o
$(MODULE_NAME)-objs += common/ub_res/ascend_ub_jetty.o
$(MODULE_NAME)-objs += unrelease/ascend_ub_socket.o
$(MODULE_NAME)-objs += common/msg/ascend_ub_admin_msg.o
$(MODULE_NAME)-objs += common/msg/ascend_ub_urma_chan.o
$(MODULE_NAME)-objs += common/load_image/ascend_ub_load.o
$(MODULE_NAME)-objs += common/load_image/ascend_ub_path.o
$(MODULE_NAME)-objs += common/msg/ascend_ub_non_trans_chan.o
$(MODULE_NAME)-objs += common/msg/ascend_ub_common_msg.o
$(MODULE_NAME)-objs += common/msg/ascend_ub_msg.o
$(MODULE_NAME)-objs += common/unit/ascend_ub_proc_fs.o
$(MODULE_NAME)-objs += common/unit/ascend_ub_urd.o
$(MODULE_NAME)-objs += common/dev_online/ascend_ub_link_chan.o
$(MODULE_NAME)-objs += common/dev_online/ascend_ub_pair_dev_info.o
$(MODULE_NAME)-objs += common/unit/ascend_ub_rao.o
$(MODULE_NAME)-objs += common/unit/ascend_ub_hotreset.o
$(MODULE_NAME)-objs += common/unit/ascend_ub_mem_decoder.o

ifeq ($(PRODUCT_SIDE), host)
	$(MODULE_NAME)-objs += host/dev_online/ascend_ub_dev_online_adapt.o
	$(MODULE_NAME)-objs += host/load_image/ascend_ub_load_image_adapt.o
	$(MODULE_NAME)-objs += host/msg/ascend_ub_msg_adapt.o
	$(MODULE_NAME)-objs += host/msg/ascend_ub_non_trans_chan_adapt.o
	$(MODULE_NAME)-objs += host/unit/ascend_ub_hotreset_adapt.o
	$(MODULE_NAME)-objs += host/unit/ascend_ub_mem_decoder_adapt.o
	$(MODULE_NAME)-objs += host/unit/ascend_ub_proc_fs_adapt.o
	$(MODULE_NAME)-objs += host/unit/ascend_ub_rao_adapt.o
	$(MODULE_NAME)-objs += host/unit/ascend_ub_urd_adapt.o
	$(MODULE_NAME)-objs += host/ascend_ub_main_adapt.o
endif

ifeq ($(PRODUCT_SIDE), device)
	$(MODULE_NAME)-objs += device/dev_online/ascend_ub_dev_online_adapt.o
	$(MODULE_NAME)-objs += device/msg/ascend_ub_msg_adapt.o
	$(MODULE_NAME)-objs += device/msg/ascend_ub_non_trans_chan_adapt.o
	$(MODULE_NAME)-objs += device/unit/ascend_ub_mem_decoder_adapt.o
	$(MODULE_NAME)-objs += device/unit/ascend_ub_proc_fs_adapt.o
	$(MODULE_NAME)-objs += device/unit/ascend_ub_rao_adapt.o
	$(MODULE_NAME)-objs += device/unit/ascend_ub_urd_adapt.o
	$(MODULE_NAME)-objs += device/ascend_ub_main_adapt.o
endif

# /* for chip macro */
include $(FEATURE_MK_PATH)
EXTRA_CFLAGS += $(CONFIG_DEFINES)

ifneq ($(NOT_SUPPORT_SP), y)
	EXTRA_CFLAGS += -fstack-protector-all
endif

ccflags-y += -Wall -Werror -Wtrampolines $(WDATE_TIME) -Wfloat-equal -Wvla -Wundef -funsigned-char -Wformat=2 -Wstack-usage=2048 -Wcast-align
ccflags-y += -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-empty-body