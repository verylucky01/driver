# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
set(ascend_trs_core_host_depends asdrv_trsbase asdrv_pbl asdrv_dms drv_seclib_host asdrv_esched asdrv_dpa ascend_kernel_open_adapt)

set(ascend_trs_pm_adapt_host_depends asdrv_trsbase asdrv_esched asdrv_pbl drv_pcie_host asdrv_dms drv_seclib_host asdrv_dpa ascend_kernel_open_adapt)

set(ascend_trs_sub_stars_depends asdrv_pbl drv_seclib_host ascend_kernel_open_adapt)

set(ascend_trs_shrid_depends asdrv_pbl asdrv_dms drv_seclib_host asdrv_esched drv_pcie_host ascend_kernel_open_adapt)

set(ascend_trs_sec_eh_adapt_depends drv_seclib_host asdrv_pbl asdrv_trsbase asdrv_vvpc drv_pcie_host asdrv_dpa ascend_kernel_open_adapt)

set(ascend_trs_sec_eh_agent_depends asdrv_pbl asdrv_vmng drv_pcie_host asdrv_vpc drv_vascend_stub ascend_kernel_open_adapt)

add_host_ko(LOCAL_MODULE asdrv_trs
    KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
    MAKE_ARGS ${MAKE_ARGS}
    TARGETE_DPENDS "${ascend_trs_core_host_depends}" "${ascend_trs_sub_stars_depends}" "${ascend_trs_shrid_depends}" "${ascend_trs_pm_adapt_host_depends}" "${ascend_trs_sec_eh_agent_depends}")
add_host_ko(LOCAL_MODULE asdrv_vtrs
    KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
    MAKE_ARGS ${MAKE_ARGS} Driver_Install_Mode=vnpu_guest
    TARGETE_DPENDS "${ascend_trs_core_host_depends}" "${ascend_trs_sec_eh_adapt_depends}")