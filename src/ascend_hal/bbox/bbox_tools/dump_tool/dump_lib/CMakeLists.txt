# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
add_library(${LIB_BBOX_DUMP} STATIC)

get_filename_component(BASE_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

target_sources(${LIB_BBOX_DUMP}
    PRIVATE
        bbox_dump_exception.c
        bbox_dump_lib.c
)

target_include_directories(${LIB_BBOX_DUMP}
    PUBLIC
        ${BASE_DIR}
        ${PROJ_DIR}/config/${PROD_NAME}/
)

target_link_libraries(${LIB_BBOX_DUMP} PUBLIC ${OBJ_TOOL_COMMON})
target_link_libraries(${LIB_BBOX_DUMP} PUBLIC ${OBJ_DUMP_COMMON})