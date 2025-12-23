# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
get_filename_component(MODULE_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

add_library(dms_obj OBJECT)
target_sources(dms_obj PRIVATE
)

target_include_directories(dms_obj PRIVATE
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
    ${DRIVER_USER_DIR}/dmc/device_monitor/include
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/inc/mmpa_sdk
    ${DRIVER_KERNEL_DIR}/drv_memory/hbm_isolation
    ${DRIVER_KERNEL_DIR}/tsdrv/ts_drv/ts_drv_common
    ${DRIVER_KERNEL_DIR}/pbl/dev_urd/command/ioctl
    ${DRIVER_KERNEL_DIR}/dms/command/ioctl
    ${DRIVER_KERNEL_DIR}/pbl/uda/command/ioctl
    ${MODULE_SRC_DIR}/common
    ${MODULE_SRC_DIR}/include
    ${DRIVER_USER_DIR}/ascend_base_hal
    ${DRIVER_USER_DIR}/inc/dms
    ${DRIVER_USER_DIR}/dms/drv_devmng
    ${DRIVER_USER_DIR}/dms/drv_devmng/ascend910
)

target_link_libraries(dms_obj PRIVATE
    $<BUILD_INTERFACE:c_sec_headers>
    $<BUILD_INTERFACE:intf_pub>
    # $<BUILD_INTERFACE:slog_headers>
)

set(utilization_ts_product_list "ascend910B")
set(boot_status_no_device_error_list "ascend910B")
set(err_code_not_optimization_list "ascend910B")

include(${FEATURE_CMAKE_PATH})
target_compile_definitions(dms_obj PRIVATE
    ${CONFIG_DEFINES}
    $<$<STREQUAL:${PRODUCT_SIDE},host>:CFG_EDGE_HOST>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DMS_HOST>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DRV_HOST>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:CFG_HOST_ENV>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SRIOV>

    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SOC_VERSION>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_VF_USE_DEVID>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_FREQ_ADJUSTABLE>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_PASS_THROUGH_MCU_BY_IMU>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_HBM_FLASH>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SET_PAGE_RATIO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SET_HPAGE_CLEAR>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_MEMORY>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CORE_NUM_PER_CHIP=8>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_RECONSITUTION_TEMPORARY>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SUPPORT_GET_SPOD_PING_INFO>
    $<$<IN_LIST:${PRODUCT},${utilization_ts_product_list}>:CFG_FEATURE_AIC_AIV_UTIL_FROM_TS>
    $<$<IN_LIST:${PRODUCT},${boot_status_no_device_error_list}>:CFG_FEATURE_NO_DEVICE_NO_ERR>
    $<$<IN_LIST:${PRODUCT},${err_code_not_optimization_list}>:CFG_FEATURE_ERR_CODE_NOT_OPTIMIZATION>
)

target_compile_options(dms_obj PRIVATE
    -fno-common
)

add_subdirectory(bbox)
add_subdirectory(board)
add_subdirectory(chip)
add_subdirectory(common)
add_subdirectory(fault)
add_subdirectory(fault_inject)
add_subdirectory(time)
add_subdirectory(vdev)
add_subdirectory(power)
add_subdirectory(qos)
add_subdirectory(hbm)
add_subdirectory(lpm)
add_subdirectory(emmc)
add_subdirectory(pcie)
add_subdirectory(log)
add_subdirectory(sils)
add_subdirectory(ub)
add_subdirectory(p2p)
add_subdirectory(flash)
add_subdirectory(communication)
add_subdirectory(udis)
add_subdirectory(drv_devmng)
add_subdirectory(dc)
