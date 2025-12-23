# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

#### CPACK to package run #####

# download makeself package
if (${BUILD_COMPONENT} STREQUAL "DRIVER")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party/makeself-fetch.cmake)
endif()

set(CPACK_PACKAGE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build_out)

function(pack_built_in)
    if(${PRODUCT} STREQUAL ascend910B AND ASCEND910_93_EX)
        set(CPACK_PRODUCT ascend910_93)
    else()
        set(CPACK_PRODUCT ${PRODUCT})
    endif()

    if (${BUILD_COMPONENT} STREQUAL "DRIVER_COMPAT")
        include(cmake/config/package_config/package_driver_compat.cmake)
    endif()

    if (${BUILD_COMPONENT} STREQUAL "DRIVER")
        include(cmake/third_party/driver_device_fetch.cmake)
        get_driver_device()
        include(cmake/config/package_config/package_${PRODUCT}.cmake)
    endif()
endfunction()