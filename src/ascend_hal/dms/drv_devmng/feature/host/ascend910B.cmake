# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

target_compile_definitions(devdrv_obj PRIVATE
    # ==== drv_devmng ====
    CFG_FEATURE_DRV_DEVMNG_INIT
    CFG_FEATURE_GET_PCIE_ID_INFO

    # common
    CFG_FEATURE_SHARE_LOG

    # ascend910
    CFG_SOC_PLATFORM_CLOUD
    CFG_SOC_PLATFORM_CLOUD_V2
    CFG_FEATURE_CHIP_DIE
    CFG_FEATURE_TS_RESOURCE_FROM_US
    CFG_FEATURE_SUPPORT_AUTH_ENABLE_SIGN
    CFG_FEATURE_HISS
    CFG_FEATURE_FFTS
    CFG_FEATURE_SRIOV
    CFG_FEATURE_VFG
    CFG_FEATURE_PSS_SIGN
    CFG_FEATURE_VF_USE_DEVID
    CFG_FEATURE_HOST_AICPU
    CFG_FEATURE_SOC_INFO
    CFG_FEATURE_TRS_REFACTOR
    CFG_FEATURE_SOC_VERSION
    CFG_FEATURE_LOG_DUMP_FROM_PCIE
    CFG_FEATURE_PHY_DEVICES_TOPO
    CFG_FEATURE_ERRORCODE_ON_NEW_CHIPS
    CFG_FEATURE_OSC_FREQ
    CFG_FEATURE_LLDP_TOOL
    CFG_FEATURE_NETWORK_ROCE
    CFG_FEATURE_BOND_PORT_CONFIG
    CFG_FEATURE_TRS_HB_REFACTOR
    CFG_FEATURE_QUERY_FREQ_INFO
    CFG_FEATURE_QUERY_AICORE_BITMAP
    CFG_FEATURE_QUERY_QOS_CFG_INFO
    CFG_FEATURE_QUERY_VA_INFO
    CFG_FEATURE_HW_INFO_FROM_BIOS
    CFG_FEATURE_BBOX_KDUMP
    CFG_FEATURE_GET_CURRENT_EVENTINFO
    CFG_FEATURE_SCAN_XSFP
    CFG_FEATURE_AIC_AIV_UTIL_FROM_TS
    CFG_FEATURE_QUERY_LOG_INFO
    CFG_FEATURE_TRANS_ETH_ID
    CFG_FEATURE_HD_COMNNECT_TYPE
    CFG_FEATURE_SUPPORT_DEVMNG_BBOX
    CFG_FEATURE_DMS_ARCH_V1
    CFG_FEATURE_GET_QOS_MASTER_CFG
    CFG_FEATURE_HOST_TS
)