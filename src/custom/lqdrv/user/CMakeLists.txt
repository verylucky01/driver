# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-z,relro,-z,now -s -Wall -Wextra -fPIC -fPIE -fstack-protector-all -D_FORTIFY_SOURCE=2 -O2 -ftrapv")
set(CMAKE_C_STANDARD 11)
get_filename_component(BASE_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
add_library(lingqu-dcmi SHARED
    src/lingqu-dcmi.c
    src/lingqu-dcmi-log.c
)

target_include_directories(lingqu-dcmi PRIVATE
    ${TOP_DIR}/abl/libc_sec/include
    ${C_SEC_INCLUDE}
    ${DRIVER_CUST_DIR}/lqdrv/user/inc
    ${DRIVER_CUST_DIR}/lqdrv/kernel
)

add_dependencies(lingqu-dcmi c_sec)
target_link_libraries(lingqu-dcmi PRIVATE libc_sec.a)

install(TARGETS  lingqu-dcmi
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL)
