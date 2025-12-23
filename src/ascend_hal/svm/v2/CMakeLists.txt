# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
get_filename_component(BASE_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
add_library(devmm_obj OBJECT)

add_subdirectory(common)
add_subdirectory(devmm)

target_include_directories(devmm_obj PRIVATE
    ${BASE_DIR}
    ${BASE_DIR}/devmm/
    ${BASE_DIR}/devmm/devmm_rbtree
    ${BASE_DIR}/common
    ${DRIVER_USER_DIR}/msnpureport/inc/slog/
    ${C_SEC_INCLUDE}/
    ${DRIVER_HAL_INC_DIR}
    ${TOP_DIR}/inc
    ${DRIVER_USER_DIR}/msnpureport/inc/slog/
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/inc/common
    ${DRIVER_USER_DIR}/dmc/device_monitor/lib/include
    ${DRIVER_USER_DIR}/svm/v2/devmm
    ${DRIVER_USER_DIR}/svm/v2/command/msg
    ${DRIVER_USER_DIR}/commlib
    ${DRIVER_KERNEL_DIR}/dpa/apm/command/ioctl
    ${DRIVER_KERNEL_DIR}/svm/v2/command/ioctl
    ${DRIVER_KERNEL_DIR}/dms/command/ioctl
)

target_compile_definitions(devmm_obj PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DRV_HOST>
    __USE_GNU
    _GNU_SOURCE
)

set(INSTALL_RUNTIME_DIR bin)

target_link_libraries(devmm_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>)
    