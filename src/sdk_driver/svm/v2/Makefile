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
        asdrv_svm-objs += master/pmaster/svm_shmem_node_pod.o
        asdrv_svm-objs += master/comm/svm_ref_server_occupier.o  master/comm/svm_recycle_thread.o
        asdrv_svm-objs += master/pmaster/msg/svm_msg_client.o master/pmaster/msg/svm_host_msg_client.o master/pmaster/msg/svm_device_msg_client.o
        asdrv_svm-objs += master/comm/svm_master_mem_mng.o master/comm/svm_master_proc_fs.o master/comm/svm_master_mem_stats.o master/comm/svm_master_feature_proc_fs.o
        asdrv_svm-objs += master/comm/devmm_proc_info.o master/comm/devmm_channel.o master/comm/svm_master_addr_map.o master/comm/svm_master_memset.o master/comm/svm_master_vma_ops.o master/comm/svm_master_mem_repair.o master/comm/svm_master_dev_capability.o master/comm/svm_master_phy_allocator.o
        asdrv_svm-objs += master/comm/svm_master_convert.o master/comm/svm_master_advise.o master/comm/svm_master_get_host_info.o master/comm/svm_master_cgroup.o master/comm/svm_master_proc_mng.o master/comm/svm_master_memcpy.o
        asdrv_svm-objs += master/comm/svm_dev_res_mng.o master/comm/svm_task_dev_res_mng.o master/comm/svm_master_dma_desc_mng.o master/comm/svm_master_mem_create.o master/comm/svm_master_mem_map.o master/comm/svm_master_addr_ref_ops.o master/comm/svm_master_mem_share.o
        asdrv_svm-objs += master/pmaster/devmm_proc_mem_copy.o master/pmaster/svm_master_remote_map.o master/pmaster/svm_shmem_interprocess.o master/pmaster/svm_master_pm_proc_mng.o master/pmaster/svm_dma_prepare_pool.o
        asdrv_svm-objs += common/svm_dma.o common/svm_mem_query.o common/devmm_page_cache.o common/svm_proc_mng.o common/devmm_dev.o common/devmm_common.o common/svm_heap_mng.o common/svm_mmu_notifier.o common/svm_dynamic_addr.o
        asdrv_svm-objs += common/svm_pci_dev_tbl.o common/svm_module_ops.o common/svm_mem_mng.o common/svm_res_idr.o common/svm_version_adapt.o common/svm_proc_fs.o common/devmm_register_dma.o
        asdrv_svm-objs += common/svm_rbtree.o common/svm_srcu_work.o common/svm_page_cnt_stats.o common/svm_gfp.o  common/svm_dma_map.o common/svm_mem_split.o common/svm_log.o
        asdrv_svm-objs += common/svm_proc_gfp.o common/svm_phy_addr_blk_mng.o common/svm_mem_create.o common/svm_mem_share.o common/svm_mem_map.o common/svm_vmma_mng.o
        asdrv_svm-objs += master/pmaster/svm_master_query.o
        asdrv_svm-objs += master/pmaster/svm_master_register_ops.o master/pmaster/svm_hot_reset.o master/pmaster/svm_master_process_status_mng.o master/pmaster/svm_pmaster_mem_map.o master/pmaster/svm_shmem_node.o master/pmaster/svm_shmem_procfs.o
        obj-m := asdrv_svm.o

        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I${DRIVER_KERNEL_DIR}/svm/v2/command/msg
        EXTRA_CFLAGS += -I${DRIVER_KERNEL_DIR}/svm/v2/command/ioctl
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/common
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/common/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/comm
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/comm/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster/msg
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster/pm_adapt
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/devmng/include
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/command/ioctl

        EXTRA_CFLAGS += -DDRV_HOST
        EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
        EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
        EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
        EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V2               #temporary

        ccflags-y += -fno-common -fstack-protector-all -funsigned-char -pipe -s -Wall -Wcast-align -Werror -Wdate-time -Wfloat-equal -Wformat -Wstack-usage=2048 -Wstrict-prototypes -Wtrampolines -Wundef -Wunused -Wvla

else
    ifeq ($(TARGET_BUILD_TYPE),debug)
        EXTRA_CFLAGS += -DCFG_BUILD_DEBUG
    endif

    ifeq ($(ENABLE_ASAN),true)
        EXTRA_CFLAGS += -DCFG_FEATURE_ENABLE_ASAN
    endif

    ifneq ($(NOT_SUPPORT_SP), y)
        EXTRA_CFLAGS += -fstack-protector-all
    endif

    ccflags-y += -Wall -Werror -Wno-missing-prototypes -Wno-missing-declarations

    ifeq ($(DAVINCI_HIAI_DKMS), y)
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc_open/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dev_inc/inc/pbl
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/inc/driver
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/common
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/master/
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/pmaster/
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/pmaster/comm
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/pmaster/comm/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/pmaster/common
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/pmaster/common/inc
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/pmaster/msg
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/pmaster/pm_adapt

        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/devmng/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/kernel_adapt/include
        EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/dms/command/ioctl

        EXTRA_CFLAGS += -DDRV_HOST
        EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
        ifneq ($(filter $(TARGET_CHIP_ID), hi1910b hi1980 hi1980b),)
            EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif
        # ascend910
        ifeq ($(TARGET_PRODUCT),cloud)
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
            ifeq ($(TARGET_CHIP_ID), hi1980)
                EXTRA_CFLAGS += -DCFG_FEATURE_VFIO
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/vmaster
                asdrv_svm-objs += pm_adapt/devmm_pm_adapt.o pm_adapt/devmm_pm_vpc.o pm_adapt/svm_pm_version_adapt.o
            endif
            ifeq ($(TARGET_CHIP_ID), hi1980b)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V2
                asdrv_svm-objs += svm_shmem_node_pod.o
            endif
        # ascend310 + ascend310p + ascend310b
        else ifeq ($(TARGET_PRODUCT),mini)
            # ascend310p
            ifeq ($(TARGET_CHIP_ID), $(ASCEND_FLORENCE_DC_V10))
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
                EXTRA_CFLAGS += -DCFG_FEATURE_VFIO
                EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
                EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
                EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/svm/vmaster
                asdrv_svm-objs += pm_adapt/devmm_pm_adapt.o pm_adapt/devmm_pm_vpc.o pm_adapt/svm_pm_version_adapt.o
            endif
        endif

        asdrv_svm-objs += msg/svm_msg_client.o msg/svm_host_msg_client.o msg/svm_device_msg_client.o
        asdrv_svm-objs += comm/devmm_proc_info.o comm/devmm_channel.o comm/svm_master_memset.o comm/svm_master_addr_map.o comm/svm_master_vma_ops.o comm/svm_master_convert.o comm/svm_master_memcpy.o comm/svm_master_mem_repair.o
        asdrv_svm-objs += comm/svm_master_advise.o comm/svm_master_get_host_info.o comm/svm_master_mem_mng.o comm/svm_master_cgroup.o comm/svm_master_proc_mng.o comm/svm_master_proc_fs.o comm/svm_master_mem_stats.o comm/svm_master_dev_capability.o
        asdrv_svm-objs += comm/svm_dev_res_mng.o comm/svm_task_dev_res_mng.o comm/svm_master_dma_desc_mng.o comm/svm_master_mem_create.o comm/svm_master_mem_map.o comm/svm_master_addr_ref_ops.o comm/svm_master_mem_share.o comm/svm_master_feature_proc_fs.o
        asdrv_svm-objs += svm_master_query.o svm_shmem_interprocess.o devmm_proc_mem_copy.o svm_master_remote_map.o svm_master_pm_proc_mng.o svm_dma_prepare_pool.o
        asdrv_svm-objs += common/svm_dma.o common/devmm_dev.o common/devmm_common.o common/devmm_page_cache.o common/svm_proc_mng.o common/svm_mem_query.o common/svm_heap_mng.o common/svm_mmu_notifier.o common/svm_dynamic_addr.o
        asdrv_svm-objs += common/svm_page_cnt_stats.o common/svm_pci_dev_tbl.o common/svm_module_ops.o common/svm_mem_mng.o common/svm_res_idr.o common/svm_version_adapt.o common/svm_proc_fs.o common/svm_log.o
        asdrv_svm-objs += common/svm_rbtree.o common/svm_srcu_work.o common/svm_gfp.o common/svm_dma_map.o common/svm_mem_split.o common/devmm_register_dma.o
        asdrv_svm-objs += common/svm_proc_gfp.o common/svm_phy_addr_blk_mng.o common/svm_mem_create.o common/svm_mem_share.o common/svm_mem_map.o common/svm_vmma_mng.o
        asdrv_svm-objs += svm_master_register_ops.o svm_hot_reset.o svm_master_process_status_mng.o svm_pmaster_mem_map.o svm_shmem_node.o svm_shmem_procfs.o

        obj-m := asdrv_svm.o
    else
        ccflags-y += -Wtrampolines $(WDATE_TIME) -Wfloat-equal -Wvla -Wundef -funsigned-char -Wformat=2 -Wstack-usage=2048 -Wcast-align -Wextra
        ccflags-y += -Wno-unused-parameter -Wno-missing-field-initializers -Wno-sign-compare -Wno-format-nonliteral
        EXTRA_CFLAGS += -I$(DRIVER_HAL_INC_DIR)
        EXTRA_CFLAGS += -I$(C_SEC_INCLUDE)
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/inc/pbl
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/common
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/common/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/comm
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/comm/inc
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster/svm_common
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster/master_comm
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster/msg
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/pmaster/pm_adapt
        EXTRA_CFLAGS += -I${DRIVER_KERNEL_DIR}/svm/v2/command/msg
        EXTRA_CFLAGS += -I${DRIVER_KERNEL_DIR}/svm/v2/command/ioctl

        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/devmng/include
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/kernel_adapt/include
        EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/dms/command/ioctl

        EXTRA_CFLAGS += -DDRV_HOST
        EXTRA_CFLAGS += -DCFG_FEATURE_SHARE_LOG
        ifneq ($(filter $(PRODUCT), ascend310B ascend310p ascend910 ascend910B),)
            EXTRA_CFLAGS += -DCFG_FEATURE_HOST_LOG
            EXTRA_CFLAGS += -DCFG_FEATURE_KA_ALLOC_INTERFACE
        endif
        ifneq ($(filter $(PRODUCT),  ascend910 ascend910B),)
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_HOST
            EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD
            ifeq ($(PRODUCT),ascend910)
                EXTRA_CFLAGS += -DCFG_FEATURE_VFIO
                EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/vmaster

                asdrv_svm-objs += pm_adapt/devmm_pm_adapt.o pm_adapt/devmm_pm_vpc.o pm_adapt/svm_pm_version_adapt.o
            endif
            ifeq ($(PRODUCT),ascend910B)
                ifeq ($(ADAPT_KP_OS_FOR_EMU_TEST),ON)
                    EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_ESL_FPGA
                    EXTRA_CFLAGS += -DADAPT_KP_OS_FOR_EMU_TEST
                endif
                asdrv_svm-objs += svm_shmem_node_pod.o
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_CLOUD_V2               #temporary
            endif
        else ifneq ($(filter $(PRODUCT),  ascend310p ascend310B ascend310Besl ascend310Bemu),)
            ifeq  ($(PRODUCT),ascend310p)
                EXTRA_CFLAGS += -DCFG_SOC_PLATFORM_MINIV2
                EXTRA_CFLAGS += -DCFG_FEATURE_VFIO
                EXTRA_CFLAGS += -I$(DRIVER_KERNEL_DIR)/svm/v2/master/vmaster

                asdrv_svm-objs += pm_adapt/devmm_pm_adapt.o pm_adapt/devmm_pm_vpc.o pm_adapt/svm_pm_version_adapt.o
            endif
        endif
        asdrv_svm-objs += msg/svm_msg_client.o msg/svm_host_msg_client.o msg/svm_device_msg_client.o
        asdrv_svm-objs += master/comm/svm_master_mem_mng.o master/comm/svm_master_proc_fs.o master/comm/svm_master_mem_stats.o master/comm/svm_master_feature_proc_fs.o
        asdrv_svm-objs += master/comm/devmm_proc_info.o master/comm/devmm_channel.o master/comm/svm_master_addr_map.o master/comm/svm_master_memset.o master/comm/svm_master_vma_ops.o master/comm/svm_master_mem_repair.o master/comm/svm_master_dev_capability.o master/comm/svm_master_phy_allocator.o
        asdrv_svm-objs += master/comm/svm_master_convert.o master/comm/svm_master_advise.o master/comm/svm_master_get_host_info.o master/comm/svm_master_cgroup.o master/comm/svm_master_proc_mng.o master/comm/svm_master_memcpy.o
        asdrv_svm-objs += master/comm/svm_dev_res_mng.o master/comm/svm_task_dev_res_mng.o master/comm/svm_master_dma_desc_mng.o master/comm/svm_master_mem_create.o master/comm/svm_master_mem_map.o master/comm/svm_master_addr_ref_ops.o master/comm/svm_master_mem_share.o
        asdrv_svm-objs += devmm_proc_mem_copy.o svm_master_remote_map.o svm_shmem_interprocess.o svm_master_pm_proc_mng.o svm_dma_prepare_pool.o
        asdrv_svm-objs += common/svm_dma.o common/svm_mem_query.o common/devmm_page_cache.o common/svm_proc_mng.o common/devmm_dev.o common/devmm_common.o common/svm_heap_mng.o common/svm_mmu_notifier.o common/svm_dynamic_addr.o
        asdrv_svm-objs += common/svm_pci_dev_tbl.o common/svm_module_ops.o common/svm_mem_mng.o common/svm_res_idr.o common/svm_version_adapt.o common/svm_proc_fs.o common/devmm_register_dma.o
        asdrv_svm-objs += common/svm_rbtree.o common/svm_srcu_work.o common/svm_page_cnt_stats.o common/svm_gfp.o  common/svm_dma_map.o common/svm_mem_split.o common/svm_log.o
        asdrv_svm-objs += common/svm_proc_gfp.o common/svm_phy_addr_blk_mng.o common/svm_mem_create.o common/svm_mem_share.o common/svm_mem_map.o common/svm_vmma_mng.o
        asdrv_svm-objs += svm_master_query.o
        asdrv_svm-objs += svm_master_register_ops.o svm_hot_reset.o svm_master_process_status_mng.o svm_pmaster_mem_map.o svm_shmem_node.o svm_shmem_procfs.o

        obj-m := asdrv_svm.o
    endif
endif
