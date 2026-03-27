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

#### built-in package ####
message(STATUS "System processor: ${CMAKE_SYSTEM_PROCESSOR}")
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    message(STATUS "Detected architecture: x86_64")
    set(ARCH x86_64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|arm")
    message(STATUS "Detected architecture: ARM64")
    set(ARCH aarch64)
else ()
    message(WARNING "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# ============= CPack =============
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")

set(CPACK_INSTALL_PREFIX "/")
set(CPACK_CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
set(CPACK_CMAKE_BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CPACK_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# ============= Custom CPack parameter =============
set(CPACK_SOC "${PRODUCT}")
set(CPACK_OS_VERSION "${HOST_LINUX_DISTRIBUTOR_ID}${HOST_LINUX_DISTRIBUTOR_RELEASE}")
set(CPACK_ARCH "${ARCH}")
set(CPACK_FEATURE_LIST "")
if (${CPACK_PKG_NAME} STREQUAL driver)
    if(${PRODUCT} STREQUAL ascend910B AND ASCEND910_93_EX)
        set(CPACK_SOC_EX ascend910_93)
        set(CPACK_FEATURE_LIST feature_910_93.list)
    elseif(${PRODUCT} STREQUAL ascend910B)
        set(CPACK_FEATURE_LIST feature_910b.list)
    elseif(${PRODUCT} STREQUAL ascend950)
        set(CPACK_FEATURE_LIST feature_pcie.list)
    endif()

    set(CPACK_PKG_SCENE_INFO_INSTALL_PATH "driver/scene.info")
    set(CPACK_PKG_HELP_INFO_INSTALL_PATH "driver/script/help.info")
    set(CPACK_PKG_INSTALL_SH_INSTALL_PATH "driver/script/install.sh")
endif()

if (${CPACK_PKG_NAME} STREQUAL driver_compat)
    if(${PRODUCT} STREQUAL ascend910B AND ASCEND910_93_EX)
        set(CPACK_SOC_EX ascend910_93)
    endif()

    set(CPACK_PKG_SCENE_INFO_INSTALL_PATH "scene.info")
    set(CPACK_PKG_HELP_INFO_INSTALL_PATH "script/help.info")
    set(CPACK_PKG_INSTALL_SH_INSTALL_PATH "script/install.sh")
endif()

message(STATUS "set CPack custom parameter pkg_name=${CPACK_PKG_NAME}, soc=${CPACK_SOC}, soc_ex=${CPACK_SOC_EX}")

# ============= CPack package setting =============
set(CPACK_SET_DESTDIR ON)
set(CPACK_GENERATOR External)
set(CPACK_EXTERNAL_PACKAGE_SCRIPT "${CMAKE_SOURCE_DIR}/cmake/makeself_built_in.cmake")
set(CPACK_EXTERNAL_ENABLE_STAGING true)
set(CPACK_PACKAGE_DIRECTORY "${CPACK_PACKAGE_OUTPUT_DIRECTORY}")

message(STATUS "Package output path = ${CPACK_PACKAGE_OUTPUT_DIRECTORY}")
include(CPack)
