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
add_library(queryfeature_obj OBJECT)

target_sources(queryfeature_obj PRIVATE
    ${BASE_DIR}/query_feature.c
)

target_include_directories(queryfeature_obj PRIVATE
    ${BASE_DIR}
    ${BASE_DIR}/inc
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/msnpureport/inc/slog/
    ${C_SEC_INCLUDE}/
    ${DRIVER_HAL_INC_DIR}
)

target_link_libraries(queryfeature_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>
    -ldl
    -Wl,--Bsymbolic
)

target_compile_definitions(queryfeature_obj PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DRV_HOST>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD_V2>
    $<$<AND:$<NOT:$<STREQUAL:${PRODUCT},ascend310p>>,$<NOT:$<STREQUAL:${PRODUCT},ascend910>>>:CFG_FEATURE_PROF_AICPU_CHAN_DEFUALT>
)
