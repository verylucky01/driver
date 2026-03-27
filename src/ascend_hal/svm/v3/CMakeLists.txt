# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

get_filename_component(SVM_USER_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(SVM_SRC_FILES)
set(SVM_SRC_INC_FILES)
set(SVM_COMPILE_DEFINITIONS)

add_library(devmm_obj OBJECT)

include(${SVM_USER_DIR}/api/svm_api.cmake)
include(${SVM_USER_DIR}/assign/svm_assign.cmake)
include(${SVM_USER_DIR}/share/svm_share.cmake)
include(${SVM_USER_DIR}/op/svm_op.cmake)
include(${SVM_USER_DIR}/query/svm_query.cmake)
include(${SVM_USER_DIR}/sys_cmd/svm_sys_cmd.cmake)
include(${SVM_USER_DIR}/umc/svm_umc.cmake)
include(${SVM_USER_DIR}/dbi/svm_dbi.cmake)
include(${SVM_USER_DIR}/mem_show_cfg/svm_mem_show_cfg.cmake)
include(${SVM_USER_DIR}/urma_adapt/svm_urma_adapt.cmake)
include(${SVM_USER_DIR}/apbi/svm_apbi.cmake)

target_sources(devmm_obj PRIVATE
    ${SVM_SRC_FILES}
)

list(APPEND SVM_SRC_INC_DIRS
    ${SVM_USER_DIR}/inc/
    ${SVM_USER_DIR}/inc/assign
    ${SVM_USER_DIR}/inc/def
    ${SVM_USER_DIR}/inc/op
    ${SVM_USER_DIR}/inc/share
    ${SVM_USER_DIR}/inc/query
    ${SVM_USER_DIR}/inc/umc
    ${SVM_USER_DIR}/command/msg
    ${SVM_USER_DIR}/inc/urma_adapt
    ${DRIVER_KERNEL_DIR}/svm/v3/command/ioctl/
)

target_include_directories(devmm_obj PRIVATE
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_USER_DIR}/msnpureport/inc/slog/
    ${C_SEC_INCLUDE}/
    ${DRIVER_USER_DIR}/dmc/device_monitor/lib/include
    ${DRIVER_USER_DIR}/pbl/commlib
    ${DRIVER_USER_DIR}/buff/dc/comm/lib

    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/inc/dpa
    ${DRIVER_USER_DIR}/inc/pbl
    ${DRIVER_USER_DIR}/inc/common

    ${DRIVER_KERNEL_DIR}/inc
    ${DRIVER_KERNEL_DIR}/dpa/apm/command/ioctl

    ${SVM_SRC_INC_DIRS}
)

if(${ENABLE_UBE})
    list(APPEND SVM_COMPILE_DEFINITIONS
        CFG_FEATURE_SUPPORT_UB
    )
endif()

list(APPEND SVM_COMPILE_DEFINITIONS
    CFG_FEATURE_NOT_SUPPORT_URMA)

target_compile_definitions(devmm_obj PRIVATE
    DRV_HOST
    ${SVM_COMPILE_DEFINITIONS}
)

target_link_libraries(devmm_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>
)

target_compile_options(devmm_obj PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wno-error=duplicated-branches>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-error=duplicated-branches>
)

set(INSTALL_RUNTIME_DIR bin)