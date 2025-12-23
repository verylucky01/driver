# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
target_sources(dms_obj PRIVATE
    dms_pcie_info.c
)
target_include_directories(dms_obj PRIVATE
    ${TOP_DIR}/inc/driver
    ${DRIVER_USER_DIR}/inc
    ${BASE_DIR}
)
set(dms_architecture_v1 "ascend310p" "helper310p" "ascend310B" "ascend310Brc" "ascend910" "ascend910B")

target_compile_definitions(dms_obj PRIVATE
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_PCIE_LINK_ERROR_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910_95>:CFG_FEATURE_PCIE_LINK_ERROR_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910_95esl>:CFG_FEATURE_PCIE_LINK_ERROR_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910_55>:CFG_FEATURE_PCIE_LINK_ERROR_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910_55esl>:CFG_FEATURE_PCIE_LINK_ERROR_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910_96>:CFG_FEATURE_PCIE_LINK_ERROR_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910_96esl>:CFG_FEATURE_PCIE_LINK_ERROR_INFO>
    $<$<STREQUAL:${PRODUCT},ascend310B>:CFG_FEATURE_ERRORCODE_ON_NEW_CHIPS>
    $<$<STREQUAL:${PRODUCT},ascend310Brc>:CFG_FEATURE_ERRORCODE_ON_NEW_CHIPS>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_ERRORCODE_ON_NEW_CHIPS>
    $<$<IN_LIST:${PRODUCT},${dms_architecture_v1}>:CFG_FEATURE_DMS_ARCH_V1>
)