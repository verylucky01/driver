# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

if(NOT CANN_3RD_LIB_PATH)
    set(CANN_3RD_LIB_PATH ${PROJECT_SOURCE_DIR}/../..)
endif()
if(NOT CANN_3RD_PKG_PATH)
    set(CANN_3RD_PKG_PATH ${PROJECT_SOURCE_DIR}/../..)
endif()

set(EXTERN_DEPEND_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party)
set(EXTERN_DEPEND_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/download)

file(MAKE_DIRECTORY "${EXTERN_DEPEND_SOURCE_DIR}")
file(MAKE_DIRECTORY "${EXTERN_DEPEND_DOWNLOAD_DIR}")

include(${PROJECT_SOURCE_DIR}/cmake/third_party/secure_c.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/third_party/rdma-core.cmake)

