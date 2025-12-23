# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
occupy_space=300
vendor_bdf="0x19e5"
device_bdf="0xd803"
mode_normal="asdrv_queue|asdrv_buff|ts_agent|asdrv_trs|asdrv_esched|asdrv_trsbase|asdrv_svm|asdrv_vnic|drv_pcie_hdc_host|asdrv_dms|asdrv_fms|asdrv_vmng|ascend_soc_platform|asdrv_dpa|drv_pcie_host|asdrv_vpc|drv_vascend|drv_vascend_stub|asdrv_pbl|ascend_kernel_open_adapt|drv_seclib_host"
mode_vnpu_host="${mode_normal}"
mode_vnpu_docker_host="${mode_normal}"
mode_vnpu_guest="drv_vascend_stub|asdrv_vvpc|drv_pcie_host|asdrv_vnic|drv_pcie_hdc_host|ascend_soc_platform|asdrv_trsbase|asdrv_fms|drv_virtmng_host_stub|asdrv_dms|asdrv_esched|asdrv_vtrs|asdrv_svm|asdrv_queue|ts_agent_vm|asdrv_dpa|asdrv_pbl|ascend_kernel_open_adapt|drv_seclib_host"
src_names=(device_sw.bin device_sw.img itrustee.img hisserika.bin device_boot.img sysBaseConfig.bin hbm_img.bin network_fw_offline_2PF_asic.bin IMU_task.bin device_config.bin device_config_16g.bin)
dst_names=(ascend_910_93_device_sw.bin ascend_910_93_device_sw.img ascend_910_93_tee.bin ascend_910_93_hsm.bin ascend_910_93_device_boot.img ascend_910_93_syscfg.bin ascend_910_93_hbm.bin ascend_910_93_imp.bin ascend_910_93_imu.bin ascend_910_93_device_config.bin ascend_910_93_device_config_16g.bin)
crltool_bin="ascend_check.bin"
crlFile="ascend_cloud_v2.crl"
curcrlFile="cur_ascend_cloud_v2.crl"
source_tee="device_sw.img"
pcie_tee="ascend_910_93_device_sw.img"
image_bin="device_sw.bin device_sw.img itrustee.img hisserika.bin device_boot.img sysBaseConfig.bin hbm_img.bin network_fw_offline_2PF_asic.bin IMU_task.bin device_config.bin device_config_16g.bin"
lib_put_path="common"
basic_ko="drv_pcie_hdc_host.ko"
vnpu_guest_ko="drv_pcie_hdc_host.ko"
reset_dev_nums="8|16"
character_device="davinci_manager devmm_svm devmm_svm_agent hisi_hdc"
