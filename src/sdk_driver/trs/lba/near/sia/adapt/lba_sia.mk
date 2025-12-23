# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifeq ($(ENABLE_OPEN_SRC), y)
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_comm.o lba/near/comm/adapt/trs_host_ts_cq.o lba/near/comm/adapt/trs_host_id.o lba/near/comm/adapt/trs_host_msg.o lba/near/comm/adapt/trs_host_rpc.o lba/near/comm/adapt/trs_near_adapt_init.o lba/near/comm/adapt/trs_host_group.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/soc_adapt.o lba/near/sia/adapt/trs_host_init/trs_host.o lba/near/sia/adapt/trs_host_chan/trs_host_chan.o lba/near/sia/adapt/trs_host_chan/trs_sqe_update.o lba/near/sia/adapt/trs_host_core/trs_host_core.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/stars_v1/trs_chan_stars_v1_ops.o lba/near/comm/adapt/trs_host_chan/stars_v1/trs_chan_stars_v1_ops_stars.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/stars_v2/trs_chan_stars_v2_ops.o lba/near/comm/adapt/trs_host_chan/stars_v2/trs_chan_stars_v2_ops_stars.o lba/near/comm/adapt/trs_host_chan/stars_v2/trs_chan_stars_v2_maint_sqcq.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_mbox.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_mem.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_rsv_mem.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_db.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_id.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_event_update.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/tscpu/trs_tscpu_chan_near_ops.o lba/near/comm/adapt/trs_host_chan/tscpu/trs_tscpu_chan_near_ops_db.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_core/trs_core_near_ops_mbox.o lba/near/comm/adapt/trs_host_core/stars_v1/trs_core_stars_v1_ops.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_core/stars_v2/trs_core_stars_v2_ops.o lba/near/comm/adapt/trs_host_core/stars_v2/trs_ub_info.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_core/tscpu/trs_tscpu_core_near_ops.o lba/near/comm/adapt/trs_host_core/trs_core_near_ops.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_mode_config.o
    $(MODULE_NAME)-objs += lba/comm/adapt/trs_chan_mem_pool.o lba/comm/adapt/trs_chan_mem.o lba/comm/adapt/trs_chan_mbox.o lba/comm/adapt/trs_mbox.o lba/comm/adapt/trs_hard_mbox.o lba/comm/adapt/trs_rsv_mem.o lba/comm/adapt/trs_ts_db.o lba/comm/adapt/trs_chan_update.o lba/comm/adapt/trs_chan_irq.o lba/comm/adapt/trs_ts_status.o lba/comm/adapt/trs_stars.o lba/comm/adapt/trs_id_range.o
    $(MODULE_NAME)-objs += lba/comm/adapt/stars_v1/trs_stars_cq.o lba/comm/adapt/stars_v1/trs_chan_sqcq.o lba/comm/adapt/stars_v1/trs_stars_sq.o lba/comm/adapt/trs_chan_maint_sqcq.o
    $(MODULE_NAME)-objs += lba/comm/adapt/stars_v2/trs_stars_v2_cq.o lba/comm/adapt/stars_v2/trs_stars_v2_chan_sqcq.o lba/comm/adapt/stars_v2/trs_stars_v2_sq.o
    $(MODULE_NAME)-objs += lba/comm/adapt/tscpu/trs_tscpu_sq.o lba/comm/adapt/tscpu/trs_tscpu_cq.o lba/comm/adapt/tscpu/trs_tscpu_chan_sqcq.o
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/soc_adapt/mini_v3
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/mini_v3/soc_adapt_res_mini_v3.o
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/soc_adapt/cloud_v2
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/cloud_v2/soc_adapt_res_cloud_v2.o
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/soc_adapt/mini_v2
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/mini_v2/soc_adapt_res_mini_v2.o
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/soc_adapt/cloud_v4
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/cloud_v4/soc_adapt_res_cloud_v4.o
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/soc_adapt/cloud_v5
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/cloud_v5/soc_adapt_res_cloud_v5.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_db.o lba/near/comm/adapt/mbox/hard/trs_host_hard_mbox.o

    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
    EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
    EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_core
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_core/command/ioctl
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/trs_core/command/msg
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/mbox/hard
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/mbox/soft
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/soc_adapt
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_chan
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_chan/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_chan/stars_v2
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_chan/tscpu
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_core
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_core/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_core/stars_v2
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/comm/adapt/trs_host_core/tscpu
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt/trs_host_chan
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt/trs_host_core
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/near/sia/adapt/trs_host_init
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/comm/adapt
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/comm/adapt/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/comm/adapt/stars_v2
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/trs/lba/comm/adapt/tscpu
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/devmng/include/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/devmng/config/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/pbl/dev_urd/
    EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/command/ioctl

    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV
    EXTRA_CFLAGS += -DTRS_HOST
    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_SIA_ADAPT
    EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
    ifneq ($(filter $(PRODUCT), ascend910B),)
        EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_RMO
    endif
else
    ccflags-y += -Wno-missing-prototypes -Wno-missing-declarations
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
    EXTRA_CFLAGS += -DCFG_FEATURE_HOST_ENV
    EXTRA_CFLAGS += -DTRS_HOST

    EXTRA_CFLAGS += -DCFG_FEATURE_TRS_SIA_ADAPT

    ifeq ($(DAVINCI_HIAI_DKMS), y)
        #for dkms
        DRIVER_SRC_BASE_DIR := $(HIAI_DKMS_DIR)

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc_open/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver

        ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980b),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif
    else
        DRIVER_SRC_BASE_DIR := $(DRIVER_KERNEL_DIR)

        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)

        ifneq ($(filter $(PRODUCT), ascend310p ascend910 ascend910B ascend310B),)
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif
    endif

    ccflags-y += -Wall -Werror

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trsbase/inc
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/trs_core
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/mbox/hard
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/mbox/soft
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/soc_adapt
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_chan
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_chan/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_chan/stars_v2
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_chan/tscpu
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_core
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_core/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_core/stars_v2
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/trs_host_core/tscpu

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt/trs_host_chan
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt/trs_host_core
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt/trs_host_init
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/comm/adapt
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/comm/adapt/stars_v1
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/comm/adapt/stars_v2
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/comm/adapt/tscpu
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/dms/devmng/include/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/dms/devmng/config/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/pbl/dev_urd/
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/dms/command/ioctl

    EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
    EXTRA_CFLAGS += -Iinclude/linux

    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_comm.o lba/near/comm/adapt/trs_host_ts_cq.o lba/near/comm/adapt/trs_host_id.o lba/near/comm/adapt/trs_host_msg.o lba/near/comm/adapt/trs_host_rpc.o lba/near/comm/adapt/trs_near_adapt_init.o lba/near/comm/adapt/trs_host_group.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/soc_adapt.o lba/near/sia/adapt/trs_host_init/trs_host.o lba/near/sia/adapt/trs_host_chan/trs_host_chan.o lba/near/sia/adapt/trs_host_chan/trs_sqe_update.o lba/near/sia/adapt/trs_host_core/trs_host_core.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/stars_v1/trs_chan_stars_v1_ops.o lba/near/comm/adapt/trs_host_chan/stars_v1/trs_chan_stars_v1_ops_stars.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/stars_v2/trs_chan_stars_v2_ops.o lba/near/comm/adapt/trs_host_chan/stars_v2/trs_chan_stars_v2_ops_stars.o lba/near/comm/adapt/trs_host_chan/stars_v2/trs_chan_stars_v2_maint_sqcq.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_mbox.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_mem.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_rsv_mem.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_db.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_ops_id.o lba/near/comm/adapt/trs_host_chan/trs_chan_near_event_update.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_chan/tscpu/trs_tscpu_chan_near_ops.o lba/near/comm/adapt/trs_host_chan/tscpu/trs_tscpu_chan_near_ops_db.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_core/trs_core_near_ops_mbox.o lba/near/comm/adapt/trs_host_core/stars_v1/trs_core_stars_v1_ops.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_core/stars_v2/trs_core_stars_v2_ops.o lba/near/comm/adapt/trs_host_core/stars_v2/trs_ub_info.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_core/tscpu/trs_tscpu_core_near_ops.o lba/near/comm/adapt/trs_host_core/trs_core_near_ops.o
    $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_mode_config.o
    $(MODULE_NAME)-objs += lba/comm/adapt/trs_chan_mem_pool.o lba/comm/adapt/trs_chan_mem.o lba/comm/adapt/trs_chan_mbox.o lba/comm/adapt/trs_mbox.o lba/comm/adapt/trs_hard_mbox.o lba/comm/adapt/trs_rsv_mem.o lba/comm/adapt/trs_ts_db.o lba/comm/adapt/trs_chan_update.o lba/comm/adapt/trs_chan_irq.o lba/comm/adapt/trs_ts_status.o lba/comm/adapt/trs_stars.o lba/comm/adapt/trs_id_range.o

    $(MODULE_NAME)-objs += lba/comm/adapt/stars_v1/trs_stars_cq.o lba/comm/adapt/stars_v1/trs_chan_sqcq.o lba/comm/adapt/stars_v1/trs_stars_sq.o lba/comm/adapt/trs_chan_maint_sqcq.o
    $(MODULE_NAME)-objs += lba/comm/adapt/stars_v2/trs_stars_v2_cq.o lba/comm/adapt/stars_v2/trs_stars_v2_chan_sqcq.o lba/comm/adapt/stars_v2/trs_stars_v2_sq.o

    $(MODULE_NAME)-objs += lba/comm/adapt/tscpu/trs_tscpu_sq.o lba/comm/adapt/tscpu/trs_tscpu_cq.o lba/comm/adapt/tscpu/trs_tscpu_chan_sqcq.o

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/soc_adapt/mini_v3
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/mini_v3/soc_adapt_res_mini_v3.o

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/soc_adapt/cloud_v2
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/cloud_v2/soc_adapt_res_cloud_v2.o

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/soc_adapt/mini_v2
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/mini_v2/soc_adapt_res_mini_v2.o

    ifeq ($(DAVINCI_HIAI_DKMS), y)
        ifneq ($(filter $(TARGET_CHIP_ID), hi1980d),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt/trs_host_ub
            EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernelspace/urma/code/kmod/ubcore/include/
            EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_APM
            EXTRA_CFLAGS += -DCFG_FEATURE_SQ_SUPPORT_SVM_MEM

            ifneq ($(filter $(PRODUCT), ascend910_95 ascend910_95esl),)
                ifeq ($(ENABLE_UBE), true)
                    EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_UB_CONNECTION
                    $(MODULE_NAME)-objs += lba/near/sia/adapt/trs_host_ub/trs_ub_host_init.o lba/near/comm/trs_ub_init_common.o
                endif
            else
                ARCHS := $(shell (uname -m))
                ifneq ($(ARCHS), x86_64)
                    EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_UB_CONNECTION
                    $(MODULE_NAME)-objs += lba/near/sia/adapt/trs_host_ub/trs_ub_host_init.o lba/near/comm/trs_ub_init_common.o
                endif
            endif

            $(MODULE_NAME)-objs += lba/near/comm/adapt/mbox/soft/trs_host_soft_mbox.o lba/near/comm/adapt/trs_host_res_map.o
        else
            $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_db.o lba/near/comm/adapt/mbox/hard/trs_host_hard_mbox.o
        endif
    else
        ifneq ($(filter $(PRODUCT), ascend910_95 ascend910_95esl ascend910_55esl ascend910_55fpga ascend910_55 ascend910_96esl ascend910_96),)
            EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/sia/adapt/trs_host_ub
            EXTRA_CFLAGS += -I$(TOP_DIR)/ubengine/ssapi/kernelspace/urma/code/kmod/ubcore/include/
            EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_APM
            EXTRA_CFLAGS += -DCFG_FEATURE_SQ_SUPPORT_SVM_MEM

            ifneq ($(filter $(PRODUCT), ascend910_55fpga ascend910_96),)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_FPGA
            endif

            ifneq ($(filter $(PRODUCT), ascend910_95 ascend910_95esl),)
                ifeq ($(ENABLE_UBE), true)
                    EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_UB_CONNECTION
                    $(MODULE_NAME)-objs += lba/near/sia/adapt/trs_host_ub/trs_ub_host_init.o lba/near/comm/trs_ub_init_common.o
                endif
            else
                ARCHS := $(shell (uname -m))
                ifneq ($(ARCHS), x86_64)
                    EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_UB_CONNECTION
                    $(MODULE_NAME)-objs += lba/near/sia/adapt/trs_host_ub/trs_ub_host_init.o lba/near/comm/trs_ub_init_common.o
                endif
            endif

            $(MODULE_NAME)-objs += lba/near/comm/adapt/mbox/soft/trs_host_soft_mbox.o lba/near/comm/adapt/trs_host_res_map.o
        else
            $(MODULE_NAME)-objs += lba/near/comm/adapt/trs_host_db.o lba/near/comm/adapt/mbox/hard/trs_host_hard_mbox.o
        endif
    endif
    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/soc_adapt/cloud_v4
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/cloud_v4/soc_adapt_res_cloud_v4.o

    EXTRA_CFLAGS += -I$(DRIVER_SRC_BASE_DIR)/trs/lba/near/comm/adapt/soc_adapt/cloud_v5
    $(MODULE_NAME)-objs += lba/near/comm/adapt/soc_adapt/cloud_v5/soc_adapt_res_cloud_v5.o
    ifneq ($(filter $(PRODUCT), ascend910B),)
        EXTRA_CFLAGS += -DCFG_FEATURE_SUPPORT_RMO
    endif
endif