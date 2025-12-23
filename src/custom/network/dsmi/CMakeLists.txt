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
add_library(dsmi_network  SHARED
    ds_net.c
    ds_net_ext.c
    ${BASE_DIR}/../common/hccn_comm.c)

target_include_directories(dsmi_network PRIVATE
    ${BASE_DIR}
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
    ${C_SEC_INCLUDE}
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/inc/dmc
    ${DRIVER_USER_DIR}/inc/dms
    ${DRIVER_CUST_INC_DIR}
    ${DRIVER_CUST_DIR}/network/inc
    ${DRIVER_CUST_DIR}/network/common
    ${DRIVER_CUST_DIR}/dev_prod/user/dsmi_product_ext
    ${DRIVER_KERNEL_DIR}/dms/command/ioctl
)

target_compile_options(dsmi_network PRIVATE
    -Werror
)

target_link_libraries(dsmi_network PRIVATE
    -Wl,--no-as-needed
    c_sec
    drvdsmi_host
    ascend_hal
    -Wl,--as-needed
    -lrt
    -ldl
)

target_compile_definitions(dsmi_network PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:DRV_HOST>
    $<$<STREQUAL:${PRODUCT},ascend910>:CFG_SOC_PLATFORM_CLOUD>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD>
    $<$<STREQUAL:${PRODUCT},ascend910B>:HCCN_TOOL_SHOW_EXT_STAT>
)

install(TARGETS  dsmi_network
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL)
