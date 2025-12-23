# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
add_library(dp_proc_mng_obj OBJECT)

target_sources(dp_proc_mng_obj PRIVATE
    dp_proc_mng.c
    dp_proc_mng_mem_stats.c
    dc/dp_adapt.c
)

target_include_directories(dp_proc_mng_obj PRIVATE
    ${DRIVER_USER_DIR}/commlib
    ${DRIVER_USER_DIR}/dpa/dp_proc_mng
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_KERNEL_DIR}/inc
    ${DRIVER_KERNEL_DIR}/dpa/dp_proc_mng/command/ioctl
    ${DRIVER_USER_DIR}/msnpureport/inc/slog/
    ${C_SEC_INCLUDE}/
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_KERNEL_DIR}/dms/command/ioctl
)

target_compile_definitions(dp_proc_mng_obj PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DRV_HOST>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SUPPORT_SRIOV>
)

target_link_libraries(dp_proc_mng_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>)