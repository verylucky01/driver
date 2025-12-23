# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
set(asdrv_esched_depends drv_seclib_host drv_pcie_host asdrv_pbl asdrv_trsbase asdrv_dpa)

add_host_ko(LOCAL_MODULE asdrv_esched
            KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
            MAKE_ARGS ${MAKE_ARGS}
            TARGETE_DPENDS "${asdrv_esched_depends}")
