# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

target_compile_definitions(dms_obj PRIVATE
    # ==== dms ====
    CFG_FEATURE_SRIOV
    CFG_FEATURE_SET_PAGE_RATIO
    CFG_FEATURE_KERNEL_6_6
    CFG_SOC_PLATFORM_CLOUD
    CFG_FEATURE_MEMORY
    CORE_NUM_PER_CHIP=8
    CFG_FEATURE_SOC_VERSION
    CFG_FEATURE_RECONSITUTION_TEMPORARY
    CFG_FEATURE_CC_INFO
    CFG_FEATURE_HBM_FROM_DRVMEM
    CFG_FEATURE_BIST
    $<$<AND:$<STREQUAL:${CMAKE_HOST_SYSTEM_PROCESSOR},aarch64>,$<STREQUAL:${ENABLE_UBE},true>>:CFG_FEATURE_SUPPORT_UB>
    CFG_FEATURE_VF_USE_DEVID
    CFG_FEATURE_TRS_MODE
    CFG_FEATURE_AIC_AIV_UTIL_FROM_TS
    CFG_FEATURE_GET_DEV_INDEX_IN_GROUP

    # bbox

    # board

    # chip

    # chip->can

    # chip->dvpp

    # chip->flash

    # chip->hccs

    # chip->host_aicpu

    # chip->imu

    # chip->isp

    # chip->memory
    CFG_DDR_REG_ADDR_950

    # chip->sensorhub

    # chip->sio

    # chip->soc

    # chip->ts

    # common

    # communication

    # dc

    # emmc

    # fault
    CFG_FEATURE_GET_CURRENT_EVENTINFO
    CFG_FEATURE_DEVICE_REPLACE

    # fault_inject

    # flash

    # hbm

    # log

    # lpm

    # p2p

    # pcie
    CFG_FEATURE_PCIE_LINK_ERROR_INFO

    # power

    # qos

    # sils

    # time

    # ub

    # udis

    # vdev
)