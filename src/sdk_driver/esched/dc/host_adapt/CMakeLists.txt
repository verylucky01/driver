# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
if (${PRODUCT} STREQUAL ascend910B)
    set(asdrv_esched_depends asdrv_dms drv_seclib_host drv_pcie_host asdrv_pbl asdrv_trsbase asdrv_dpa ascend_kernel_open_adapt)
elseif (${PRODUCT} STREQUAL ascend950)
    set(asdrv_esched_depends asdrv_dms drv_seclib_host asdrv_pbl asdrv_vmng asdrv_trsbase ascend_kernel_open_adapt drv_pcie_host asdrv_dpa)
endif()
add_host_ko(LOCAL_MODULE asdrv_esched
            KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
            MAKE_ARGS ${MAKE_ARGS}
            TARGETE_DPENDS "${asdrv_esched_depends}")
