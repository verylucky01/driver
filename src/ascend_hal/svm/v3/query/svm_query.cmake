# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

get_filename_component(SVM_QUERY_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
file(GLOB SVM_QUERY_HOST_SRCS
    ${SVM_QUERY_DIR}/*.c
    ${SVM_QUERY_DIR}/get_mem_size/client/*.c
    ${SVM_QUERY_DIR}/get_mem_token_info/*.c
    ${SVM_QUERY_DIR}/pci_adapt/*.c
)

list(APPEND SVM_SRC_FILES ${SVM_QUERY_HOST_SRCS})
list(APPEND SVM_SRC_INC_FILES
    ${SVM_QUERY_DIR}
    ${SVM_QUERY_DIR}/get_mem_size/client/
    ${SVM_QUERY_DIR}/get_mem_token_info/
    ${SVM_QUERY_DIR}/pci_adapt/
)
