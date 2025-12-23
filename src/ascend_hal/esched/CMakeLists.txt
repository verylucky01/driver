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
add_library(eventsched_obj OBJECT)

target_sources(eventsched_obj PRIVATE
    comm/event_sched_app.c
    comm/event_sched.c
    esched_adapt.c
)

target_include_directories(eventsched_obj PRIVATE
    ${BASE_DIR}
    ${BASE_DIR}/comm
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_KERNEL_DIR}/dpa/apm/command/ioctl
    ${DRIVER_KERNEL_DIR}/esched/command/ioctl
    ${DRIVER_KERNEL_DIR}/esched/command/msg
    ${DRIVER_USER_DIR}/ascend_base_hal
    ${DRIVER_USER_DIR}/esched/command
    ${C_SEC_INCLUDE}
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
    ${DRIVER_KERNEL_DIR}/dms/command/ioctl
)

target_compile_definitions(eventsched_obj PRIVATE
    CFG_FEATURE_EXTERNAL_CDEV
    CFG_ENV_HOST
    DRV_HOST
)

target_link_libraries(eventsched_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>
)