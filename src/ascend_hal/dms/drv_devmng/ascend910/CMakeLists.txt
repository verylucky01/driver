# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
target_sources(devdrv_obj PRIVATE
    devdrv_hccl.c
    devdrv_pcie.c
    devdrv_manager.c
    devdrv_container.c
    devdrv_common.c
    devdrv_vdev.c
    devdrv_host_flush_cache.c
)

target_include_directories(devdrv_obj PRIVATE
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
    ${DRIVER_HAL_INC_DIR}/depends/hiss
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/inc/mmpa_sdk
    ${DRIVER_USER_DIR}/tsdrv/
    $<$<STREQUAL:${PRODUCT},ascend910B>:${DRIVER_USER_DIR}/inc/config/user_config/cloudv2>
    $<$<STREQUAL:${PRODUCT},ascend910B>:${DRIVER_USER_DIR}/tsdrv/ascend910>
    ${DRIVER_USER_DIR}/dms/include
    ${DRIVER_USER_DIR}/dms/common
    ${DRIVER_USER_DIR}/dms/hbm
    ${MODULE_SRC_DIR}/inc
    ${MODULE_SRC_DIR}/ascend910
    $<$<STREQUAL:${PRODUCT},ascend910B>:${MODULE_SRC_DIR}/ascend910/ascd910>
    ${MODULE_SRC_DIR}/dc
    ${DRIVER_KERNEL_DIR}/inc
    ${DRIVER_KERNEL_DIR}/pbl/dev_urd/command/ioctl
    ${DRIVER_KERNEL_DIR}/pbl/uda/command/ioctl
    ${DRIVER_KERNEL_DIR}/inc/ascend_platform
    ${DRIVER_KERNEL_DIR}/dpa/apm/command/ioctl
    ${DRIVER_KERNEL_DIR}/dms/command/ioctl
)

target_link_libraries(devdrv_obj PRIVATE
    $<BUILD_INTERFACE:c_sec_headers>
)

set(utilization_ts_product_list "ascend910B")
set(support_host_klog_query "ascend910B")
set(support_trans_eth_id "ascend910B")
set(support_ep_mode "ascend910B")
set(support_bbox_product_list "ascend910B")
set(dms_architecture_v1 "ascend910B")

include(${FEATURE_CMAKE_PATH})
target_compile_definitions(devdrv_obj PRIVATE
    ${CONFIG_DEFINES}
    CFG_EDGE_HOST
    DRV_HOST
    DEVDRV_HOST
    PCIE_HOST
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD_V2>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_CHIP_DIE>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_TS_RESOURCE_FROM_US>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SUPPORT_AUTH_ENABLE_SIGN>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_HISS>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_FFTS>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SRIOV>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_VFG>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_PSS_SIGN>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_VF_USE_DEVID>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_HOST_AICPU>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SOC_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_TRS_REFACTOR>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SOC_VERSION>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_LOG_DUMP_FROM_PCIE>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_PHY_DEVICES_TOPO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_ERRORCODE_ON_NEW_CHIPS>

    $<$<IN_LIST:${PRODUCT},${utilization_ts_product_list}>:CFG_FEATURE_AIC_AIV_UTIL_FROM_TS>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_OSC_FREQ>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_LLDP_TOOL>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_NETWORK_ROCE>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_BOND_PORT_CONFIG>
    $<$<IN_LIST:${PRODUCT},${support_trans_eth_id}>:CFG_FEATURE_TRANS_ETH_ID>

    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_TRS_HB_REFACTOR>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_QUERY_FREQ_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_QUERY_AICORE_BITMAP>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_QUERY_QOS_CFG_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_QUERY_VA_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_HW_INFO_FROM_BIOS>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_BBOX_KDUMP>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_GET_CURRENT_EVENTINFO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SCAN_XSFP>

    $<$<IN_LIST:${PRODUCT},${support_ep_mode}>:CFG_FEATURE_HD_COMNNECT_TYPE>

    $<$<IN_LIST:${PRODUCT},${support_bbox_product_list}>:CFG_FEATURE_SUPPORT_DEVMNG_BBOX>
    $<$<AND:$<STREQUAL:${PRODUCT_SIDE},host>,$<IN_LIST:${PRODUCT},${support_host_klog_query}>>:CFG_FEATURE_QUERY_LOG_INFO>
    $<$<IN_LIST:${PRODUCT},${dms_architecture_v1}>:CFG_FEATURE_DMS_ARCH_V1>
)

target_compile_options(devdrv_obj PRIVATE
    -fno-common
)

add_subdirectory(devdrv_dsmi)

if(${PRODUCT} STREQUAL ascend910B)
   add_subdirectory(ascd910)
else()
   message(FATAL_ERROR "Cannot find product: ${PRODUCT}.")
endif()
