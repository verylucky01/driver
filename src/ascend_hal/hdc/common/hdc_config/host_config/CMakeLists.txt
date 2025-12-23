# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/hdcBasic.cfg
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/hdcBasic.cfg ${CMAKE_BINARY_DIR}/hdcBasic.cfg
)
add_custom_target(hdcBasic_cfg DEPENDS ${CMAKE_BINARY_DIR}/hdcBasic.cfg)

if(DEFINED INSTALL_RUNTIME_DIR)
    install(
        FILES ${CMAKE_BINARY_DIR}/hdcBasic.cfg
        DESTINATION ${INSTALL_RUNTIME_DIR} OPTIONAL
    )
endif()