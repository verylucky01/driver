# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
add_library(${OBJ_DATA_PARSE} INTERFACE)

set(PASER_PROD_NAME ${PROD_NAME})

target_sources(${OBJ_DATA_PARSE}
    INTERFACE
        common_parser/bbox_commlog_parser.c
        common_parser/bbox_common_parser.c
        common_parser/bbox_data_model.c
        ../../data_model_product/${PASER_PROD_NAME}/bbox_data_model_${PASER_PROD_NAME}.c
        ../../bbox_adapt.c
        kernel_log_parser/bbox_kernel_log_parser.c
        kernel_log_parser/bbox_kernel_klog_parser.c
        kernel_log_parser/bbox_kernel_rblog_parser.c
        os_data_parser/bbox_ddr_ap_adapter.c
        rdr_data_parser/bbox_rdr_data_parser.c
)

target_include_directories(${OBJ_DATA_PARSE}
    INTERFACE
        ${BASE_DIR}/data_parser/common_parser
        ${BASE_DIR}/data_parser
        ${BASE_DIR}/file_manage
        ${BASE_DIR}/include
        ${BASE_DIR}/utils
        ${PROJ_DIR}/config/${PROD_NAME}/
        ${PROJ_DIR}/
)

target_link_libraries(${OBJ_DATA_PARSE} INTERFACE
    $<BUILD_INTERFACE:bbox_intf_pub>
)