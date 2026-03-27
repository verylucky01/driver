# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
set(MAKE_ARGS
    BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

list(APPEND MAKE_ARGS
        RUN_MODE=host
        KBUILD_MODPOST_WARN=1
        DRIVER_KERNEL_DIR=${DRIVER_KERNEL_DIR}
        DRIVER_HAL_INC_DIR=${DRIVER_HAL_INC_DIR}
        ENABLE_OPEN_SRC=${ENABLE_OPEN_SRC}
        C_SEC_INCLUDE=${C_SEC_INCLUDE}
)

set(drv_dvpp_cmdlist_depends
    ts_agent asdrv_svm asdrv_pbl
    drv_pcie_host asdrv_vpc drv_seclib_host asdrv_vascend_adapt asdrv_vmng ascend_kernel_open_adapt)
add_host_ko(LOCAL_MODULE drv_dvpp_cmdlist
    KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
    USE_LOCAL_MODULE_BIN_FOLDER
    MAKE_ARGS ${MAKE_ARGS}
    TARGETE_DPENDS "${drv_dvpp_cmdlist_depends}")
if ((${PRODUCT} STREQUAL ascend910B))
    add_subdirectory(vdrv)
endif()
