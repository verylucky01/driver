# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
# DP_PROC_MNG_DEPEND
if(${PRODUCT} STREQUAL ascend910B)
    set(drv_dp_proc_mng_host_depends drv_seclib_host drv_pcie_host asdrv_vmng asdrv_pbl ascend_kernel_open_adapt)
else()
    set(drv_dp_proc_mng_host_depends drv_seclib_host drv_pcie_host asdrv_pbl ascend_kernel_open_adapt)
endif ()

# APM_DEPEND
if(${PRODUCT} STREQUAL ascend950)
    set(ascend_apm_host_depends asdrv_pbl drv_seclib_host)
endif ()

if(${PRODUCT} STREQUAL ascend910B OR ${PRODUCT} STREQUAL ascend950)
    set(ascend_rmo_host_depends drv_seclib_host drv_pcie_host asdrv_pbl ascend_kernel_open_adapt)
endif ()

# UDIS_DEPEND
set(ascend_udis_depends drv_seclib_host asdrv_pbl drv_pcie_host ascend_kernel_open_adapt)

add_host_ko(LOCAL_MODULE asdrv_dpa
    KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
    MAKE_ARGS ${MAKE_ARGS}
    TARGETE_DPENDS "${ascend_apm_host_depends}" "${drv_dp_proc_mng_host_depends}" "${ascend_udis_depends}" "${ascend_rmo_host_depends}")