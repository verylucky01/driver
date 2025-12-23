# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

if (${PRODUCT_SIDE} STREQUAL host)
    set(ts_debug_depends drv_seclib_host asdrv_svm asdrv_fms asdrv_trs asdrv_pbl asdrv_trsbase ascend_kernel_open_adapt drv_pcie_host)
    add_host_ko(LOCAL_MODULE ts_debug
        KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
        MAKE_ARGS ${MAKE_ARGS}
        TARGETE_DPENDS "${ts_debug_depends}")
    set(debug_switch_depends drv_seclib_host asdrv_svm asdrv_fms asdrv_trs asdrv_pbl asdrv_trsbase ascend_kernel_open_adapt drv_pcie_host)
    add_host_ko(LOCAL_MODULE debug_switch
        KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
        MAKE_ARGS ${MAKE_ARGS}
        TARGETE_DPENDS "${debug_switch_depends}")
endif()
