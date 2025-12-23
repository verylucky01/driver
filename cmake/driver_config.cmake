# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

message(STATUS "HOST_SYSTEM_NAME                    = ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "HOST_SYSTEM_VERSION                 = ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "HOST_SYSTEM_PROCESSOR               = ${CMAKE_HOST_SYSTEM_PROCESSOR}")

get_host_linux_distributor()
message(STATUS "HOST_LINUX_DISTRIBUTOR_ID           = ${HOST_LINUX_DISTRIBUTOR_ID}")
message(STATUS "HOST_LINUX_DISTRIBUTOR_RELEASE      = ${HOST_LINUX_DISTRIBUTOR_RELEASE}")

set(TARGET_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
set(TARGET_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
set(TARGET_LINUX_DISTRIBUTOR_ID ${HOST_LINUX_DISTRIBUTOR_ID})
set(TARGET_LINUX_DISTRIBUTOR_RELEASE ${HOST_LINUX_DISTRIBUTOR_RELEASE})
message(STATUS "TARGET_SYSTEM_NAME                  = ${TARGET_SYSTEM_NAME}")
message(STATUS "TARGET_SYSTEM_PROCESSOR             = ${TARGET_SYSTEM_PROCESSOR}")
message(STATUS "TARGET_LINUX_DISTRIBUTOR_ID         = ${TARGET_LINUX_DISTRIBUTOR_ID}")
message(STATUS "TARGET_LINUX_DISTRIBUTOR_RELEASE    = ${TARGET_LINUX_DISTRIBUTOR_RELEASE}")
message(STATUS "CMAKE_GENERATOR                     = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_BUILD_TYPE                    = ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CONFIGURATION_TYPES           = ${CMAKE_CONFIGURATION_TYPES}")

# use native kernel
if (NOT DEFINED CUSTOM_KERNEL_PATH)
    set(CUSTOM_KERNEL_PATH /lib/modules/${CMAKE_HOST_SYSTEM_VERSION}/build)
endif()
message("KERNEL_SOURCE_PATH=" ${CUSTOM_KERNEL_PATH})
get_host_kernel_path()

include(cmake/config/driver_config/driver_config_${PRODUCT}.cmake)

if(${BUILD_COMPONENT} STREQUAL "DRIVER")
    add_custom_target(driver ALL
        COMMAND echo "Build driver targets"
        DEPENDS ${DRIVER_TARGETS}
    )
    if(ENABLE_BUILD_PRODUCT)
        add_dependencies(driver ${DRIVER_CUSTOM_TARGETS})
    endif()
endif()

if(${BUILD_COMPONENT} STREQUAL "DRIVER_COMPAT")
    add_custom_target(driver_compat ALL
        COMMAND echo "Build driver compat targets"
        DEPENDS ${DRIVER_COMPAT_TARGETS}
    )
endif()
