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

set(VERSION_INFO_OUTPUT ${CMAKE_BINARY_DIR}/version.info)
execute_process(
    COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/package/package.py --pkg_name driver_compat --chip_name ${CPACK_PRODUCT} --os_arch linux-${ARCH}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE result
    ERROR_VARIABLE error
    RESULT_VARIABLE code
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "package.py result: ${code}")
if (NOT code EQUAL 0)
    message(FATAL_ERROR "version.info generation failed: ${result}")
else ()
    message(STATUS "version.info generated successfully: ${result}")

    if (NOT EXISTS ${VERSION_INFO_OUTPUT})
        message(FATAL_ERROR "Output file not created: ${VERSION_INFO_OUTPUT}")
    endif ()
endif ()

file(STRINGS ${VERSION_INFO_OUTPUT} version_line REGEX "^Version=")
if(version_line)
    string(REGEX REPLACE "^Version=" "" version_value ${version_line})
    set(DRIVER_COMPAT_VERSION ${version_value})
    message(STATUS "driver compat version: ${DRIVER_COMPAT_VERSION}")
endif()

install(FILES ${CMAKE_BINARY_DIR}/version.info
    DESTINATION .
    COMPONENT driver_compat
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
)

set(lib64_driver_prefix ${CMAKE_BINARY_DIR}/lib)
set(LIB64_DRIVER_FILES
    ${lib64_driver_prefix}/libc_sec.so
    ${lib64_driver_prefix}/libascend_hal.so
    ${lib64_driver_prefix}/libascend_rdma_lite.so
    ${lib64_driver_prefix}/libhns-rdma-hal.so
    ${lib64_driver_prefix}/libhns-rdmav25.so
    ${lib64_driver_prefix}/libhns-rdmav34.so
    ${lib64_driver_prefix}/librdmacm.so.1
    ${lib64_driver_prefix}/libibumad.so.3
    ${lib64_driver_prefix}/libibverbs.so.1
)
install(FILES ${LIB64_DRIVER_FILES}
    DESTINATION .
    COMPONENT driver_compat
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
)

# ============= CPack =============
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

set(CPACK_INSTALL_PREFIX "/")
set(CPACK_CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
set(CPACK_CMAKE_BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CPACK_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CPACK_SOC "${CPACK_PRODUCT}")
set(CPACK_OS_VERSION "${HOST_LINUX_DISTRIBUTOR_ID}${HOST_LINUX_DISTRIBUTOR_RELEASE}")
set(CPACK_ARCH "${ARCH}")
set(CPACK_SET_DESTDIR ON)
set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_DIRECTORY "${CPACK_PACKAGE_OUTPUT_DIRECTORY}")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL driver_compat)

if(${CPACK_SOC} STREQUAL "ascend910B")
    set(PACKAGE_PREFIX "Ascend-hdk-910b-driver-compat")
elseif(${CPACK_SOC} STREQUAL "ascend910_93")
    set(PACKAGE_PREFIX "Ascend-hdk-A3-driver-compat")
else()
    message(FATAL_ERROR "Unknow: soc=${CPACK_SOC}")
endif()

set(CPACK_ARCHIVE_DRIVER_COMPAT_FILE_NAME "${PACKAGE_PREFIX}_${DRIVER_COMPAT_VERSION}_${CPACK_OS_VERSION}-${CPACK_ARCH}")

message(STATUS "Package output path = ${CPACK_PACKAGE_OUTPUT_DIRECTORY}")
include(CPack)
