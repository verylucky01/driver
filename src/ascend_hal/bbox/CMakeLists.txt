# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
set(product_list "ascend910B" "ascend310B" "ascend310Besl" "ascend310Bemu" "ascend310Brc" "ascend310Brcesl" "ascend310Brcemu" "ascend910_95" "ascend910_95esl" "ascend910_55" "ascend910_55esl" "ascend910_96" "ascend910_96esl")
if (${PRODUCT} IN_LIST product_list)

    get_filename_component(PROJ_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

    include(cmake/bbox_intf_pub.cmake)
    include(cmake/bbox_func_pub.cmake)

    get_product_name(PROD_NAME)

    ##### target name #####
    set(LIB_BBOX_PROC bbox_proc)
    set(LIB_BBOX_DUMP bbox_dump)
    set(EXE_DUMP_TOOL bbox_dump_tool)
    set(OBJ_DATA_PARSE bbox_data_parse)
    set(ETC_BBOX_CONFIG bbox_conf)

    ##### target dir #####
    set(BBOX_HOST_DIR "bbox_excep_mgr")
    set(BBOX_TOOL_DIR "bbox_tools")

    ##### install dir #####
    set(INSTALL_LIBRARY_DIR "lib")
    set(INSTALL_RUNTIME_DIR "bin")

    add_subdirectory(${BBOX_HOST_DIR})
    add_subdirectory(${BBOX_TOOL_DIR})
endif ()