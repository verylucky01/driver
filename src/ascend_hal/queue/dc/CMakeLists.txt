# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
add_library(queue_obj OBJECT)

target_include_directories(queue_obj PRIVATE
    ${DRIVER_USER_DIR}/ascend_base_hal
    ${DRIVER_USER_DIR}/buff/dc/comm
    ${DRIVER_USER_DIR}/buff/dc/comm/lib
    ${DRIVER_USER_DIR}/buff/dc/comm/common
    ${DRIVER_USER_DIR}/commlib
    ${DRIVER_USER_DIR}/queue/command
    ${DRIVER_USER_DIR}/queue/dc/core
    ${DRIVER_USER_DIR}/queue/dc/interface
    ${C_SEC_INCLUDE}
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_KERNEL_DIR}/queue/command/ioctl
    ${DRIVER_KERNEL_DIR}/esched/command/ioctl
    ${DRIVER_KERNEL_DIR}/queue/command/msg
    ${DRIVER_KERNEL_DIR}/esched/command/msg
    ${DRIVER_USER_DIR}/buff/dc/share_fd_adp
    ${DRIVER_USER_DIR}/esched/comm
    ${DRIVER_USER_DIR}/esched/dc
    ${DRIVER_USER_DIR}/esched/command
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
    ${DRIVER_KERNEL_DIR}/dms/command/ioctl
)

target_compile_definitions(queue_obj PRIVATE
    DRV_HOST
    CFG_FEATURE_REQ_DMA_SRC_DST_ADDR_ALIGN
    CFG_FEATURE_HOST_SUPPORT_LOCAL_QUEUE
)

target_link_libraries(queue_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>
)

add_subdirectory(core)
add_subdirectory(agent)
add_subdirectory(interface)