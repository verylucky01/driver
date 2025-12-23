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
add_library(rmo_obj OBJECT)

target_sources(rmo_obj PRIVATE
    rmo_user_res_map.c
    rmo_ioctl.c
    $<$<STREQUAL:${PRODUCT_SIDE},host>:${BASE_DIR}/rmo_user_mem_sharing.c>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:${BASE_DIR}/host/rmo_user_res_map_host.c>
)

target_include_directories(rmo_obj PRIVATE
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/dpa/rmo
    ${DRIVER_USER_DIR}/dpa/rmo/command/msg
    ${DRIVER_USER_DIR}/msnpureport/inc/slog/
    ${C_SEC_INCLUDE}/
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_KERNEL_DIR}/dpa/rmo/command/ioctl
)

target_link_libraries(rmo_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>
    -ldl
    -Wl,--Bsymbolic
)

target_compile_definitions(rmo_obj PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DRV_HOST>
)
