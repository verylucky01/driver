# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

get_filename_component(BASE_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
aux_source_directory(./dcmi_inner/src SRC_LIST)
aux_source_directory(./dcmi_init/src SRC_LIST)
aux_source_directory(./dcmi_interface/src SRC_LIST)

#编译dcmi动态链接库
add_library(dcmi SHARED)
target_link_libraries(dcmi PRIVATE
    dcmi_obj
)
add_dependencies(dcmi c_sec drvdsmi_host)

#编译dsmi静态链接库
add_library(dcmi_static STATIC)
add_dependencies(dcmi_static c_sec drvdsmi_host dsmi_network)

target_link_libraries(dcmi_static PRIVATE
    dcmi_obj
)
set_target_properties(dcmi_static PROPERTIES OUTPUT_NAME "dcmi")

# 编译dcmi_obj
add_library(dcmi_obj OBJECT)
add_dependencies(dcmi_obj c_sec drvdsmi_host)
target_sources(dcmi_obj PRIVATE ${SRC_LIST})

target_link_libraries(dcmi_obj PRIVATE
    -Wl,--no-as-needed
    c_sec
    dsmi_network
    drvdsmi_host
    mmpa
    ascend_hal
    -Wl,--as-needed
    -lpthread
    -ldl
    -lm
)

target_compile_options(dcmi_obj PRIVATE
    -Wall
    -Werror
    -fPIC
)
target_include_directories(dcmi_obj PUBLIC ${C_SEC_INCLUDE}
                                PUBLIC ${DRIVER_HAL_INC_DIR}
                                PUBLIC ${BASE_DIR}/dcmi_inner/inc
                                PUBLIC ${BASE_DIR}/dcmi_init/inc
                                PUBLIC ${BASE_DIR}/dcmi_interface/inc
                                PUBLIC ${DRIVER_CUST_INC_DIR})

target_compile_definitions(dcmi_obj PRIVATE DCMI_VERSION=\"${PROJECT_VERSION}\")

install(TARGETS  dcmi
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL)
