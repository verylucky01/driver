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
add_library(prof_obj OBJECT)

target_sources(prof_obj PRIVATE
    prof_interface.c
    ./core/prof_core.c
    ./core/prof_poll.c
    ./core/prof_chan.c
    ./core/prof_chan_list.c
    ./adapt/prof_adapt.c
    ./adapt/prof_buff.c
    ./adapt/user/prof_adapt_user.c
    ./comm/prof_communication.c
    $<$<NOT:$<IN_LIST:${PRODUCT},${product_not_support_event}>>:${CMAKE_CURRENT_SOURCE_DIR}/comm/event/prof_event_master.c>
    $<$<NOT:$<IN_LIST:${PRODUCT},${product_not_support_event}>>:${CMAKE_CURRENT_SOURCE_DIR}/comm/event/prof_event_comm.c>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:${CMAKE_CURRENT_SOURCE_DIR}/adapt/kernel/prof_adapt_h2d.c>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:${CMAKE_CURRENT_SOURCE_DIR}/adapt/kernel/prof_adapt_hdc.c>
    $<$<AND:$<STREQUAL:${PRODUCT_SIDE},host>,$<IN_LIST:${PRODUCT},${product_support_urma}>>:${CMAKE_CURRENT_SOURCE_DIR}/adapt/kernel/prof_adapt_urma.c>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:${CMAKE_CURRENT_SOURCE_DIR}/comm/hdc/prof_hdc_comm.c>
    $<$<STREQUAL:${PRODUCT_SIDE},host>:${CMAKE_CURRENT_SOURCE_DIR}/comm/hdc/prof_hdc.c>
    $<$<AND:$<STREQUAL:${PRODUCT_SIDE},host>,$<IN_LIST:${PRODUCT},${product_support_urma}>>:${CMAKE_CURRENT_SOURCE_DIR}/comm/urma/prof_urma_comm.c>
    $<$<AND:$<STREQUAL:${PRODUCT_SIDE},host>,$<IN_LIST:${PRODUCT},${product_support_urma}>>:${CMAKE_CURRENT_SOURCE_DIR}/comm/urma/prof_urma.c>
    $<$<STREQUAL:${PRODUCT_SIDE},device>:${CMAKE_CURRENT_SOURCE_DIR}/adapt/user/prof_adapt_user_ioctl.c>
    $<$<STREQUAL:${PRODUCT_SIDE},device>:${CMAKE_CURRENT_SOURCE_DIR}/adapt/kernel/prof_adapt_ioctl.c>
    $<$<STREQUAL:${PRODUCT_SIDE},device>:${CMAKE_CURRENT_SOURCE_DIR}/comm/ioctl/prof_ioctl.c>
    $<$<AND:$<STREQUAL:${PRODUCT_SIDE},device>,$<NOT:$<IN_LIST:${PRODUCT},${product_not_support_event}>>>:${CMAKE_CURRENT_SOURCE_DIR}/comm/event/prof_event_agent.c>
)

target_include_directories(prof_obj PRIVATE
    ${BASE_DIR}
    ${BASE_DIR}/core/
    ${BASE_DIR}/adapt/
    ${BASE_DIR}/adapt/user/
    ${BASE_DIR}/adapt/kernel/
    ${BASE_DIR}/comm/
    ${BASE_DIR}/comm/event/
    ${BASE_DIR}/command/
    $<$<STREQUAL:${PRODUCT_SIDE},host>:${BASE_DIR}/comm/hdc/>
    $<$<AND:$<STREQUAL:${PRODUCT_SIDE},host>,$<IN_LIST:${PRODUCT},${product_support_urma}>>:${BASE_DIR}/comm/urma/>
    $<$<STREQUAL:${PRODUCT_SIDE},device>:${BASE_DIR}/comm/ioctl/>
    ${TOP_DIR}/inc
    ${DRIVER_HAL_INC_DIR}
    ${C_SEC_INCLUDE}
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_KERNEL_DIR}/dmc/prof/command/ioctl
    ${DRIVER_KERNEL_DIR}/dmc/prof/command/msg
    $<$<STREQUAL:${IN_BRANCH},trunk_ai>:${DRIVER_USER_DIR}/dms/drv_devmng/ascend910/>
    $<$<IN_LIST:${PRODUCT},${product_support_urma}>:${TOP_DIR}/ubengine/ssapi/kernelspace/urma/code/include>
    $<$<IN_LIST:${PRODUCT},${product_support_urma}>:${TOP_DIR}/ubengine/ssapi/kernelspace/urma/code/lib/urma/include>
    ${DRIVER_USER_DIR}/inc/dms
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
)

target_compile_definitions(prof_obj PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DRV_HOST>
    $<$<STREQUAL:${PRODUCT},ascend910_95>:CFG_SOC_PLATFORM_CLOUD_V4>
    $<$<STREQUAL:${PRODUCT},ascend910_95esl>:CFG_SOC_PLATFORM_CLOUD_V4>
    $<$<STREQUAL:${PRODUCT},ascend910_96>:CFG_SOC_PLATFORM_CLOUD_V4>
    $<$<STREQUAL:${PRODUCT},ascend910_96esl>:CFG_SOC_PLATFORM_CLOUD_V4>
    $<$<STREQUAL:${PRODUCT},ascend910_55esl >:CFG_SOC_PLATFORM_CLOUD_V4>
    $<$<STREQUAL:${PRODUCT},ascend910_55>:CFG_SOC_PLATFORM_CLOUD_V4>
    $<$<IN_LIST:${PRODUCT},${ascend910_96}>:SSAPI_USE_MAMI>
    $<$<IN_LIST:${PRODUCT},${ascend910_96esl}>:SSAPI_USE_MAMI>
)

target_link_libraries(prof_obj PRIVATE
    $<BUILD_INTERFACE:intf_pub>
)