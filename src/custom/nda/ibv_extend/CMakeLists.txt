# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
#
# File Name     : CMakeLists.txt
# Description   : Build configuration for ibv_extend library

cmake_minimum_required(VERSION 3.13)
project(ibv_extend VERSION 3.2.0 LANGUAGES C)

# 设置构建目录路径
set(BUILD_INCLUDE ${PROJECT_BINARY_DIR}/include)
set(BUILD_ETC /etc)

# 根据系统架构自动选择库目录
# 检查系统是否使用 lib64 目录
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64位系统
    # 优先级: /usr/local/lib64 > /usr/lib64 > /usr/local/lib > /usr/lib
    if(EXISTS "/usr/local/lib64")
        set(BUILD_LIB /usr/local/lib64)
    elseif(EXISTS "/usr/lib64")
        set(BUILD_LIB /usr/lib64)
    elseif(EXISTS "/usr/local/lib")
        set(BUILD_LIB /usr/local/lib)
    else()
        set(BUILD_LIB /usr/lib)
    endif()
else()
    # 32位系统
    if(EXISTS "/usr/local/lib")
        set(BUILD_LIB /usr/local/lib)
    else()
        set(BUILD_LIB /usr/lib)
    endif()
endif()

message(STATUS "System architecture: ${CMAKE_SYSTEM_PROCESSOR}")

# 设置安装路径
set(EXT_CONFIG_DIR "${BUILD_ETC}/libibverbs_extend.d")
set(EXTEND_VERBS_PROVIDER_DIR "${BUILD_LIB}")

message(STATUS "Using extend config dir: ${EXT_CONFIG_DIR}")
message(STATUS "Using extend verbs provider dir: ${EXTEND_VERBS_PROVIDER_DIR}")

# 配置头文件
configure_file("config.h.in" "${BUILD_INCLUDE}/config.h" ESCAPE_QUOTES @ONLY)

# 使用 project() 命令设置的版本号
set(IBV_EXTEND_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(IBV_EXTEND_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(IBV_EXTEND_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(IBV_EXTEND_VERSION "${PROJECT_VERSION}")

# 定义源文件
set(IBV_EXTEND_SOURCES
    ibv_extend.c
)

# 定义头文件
set(IBV_EXTEND_HEADERS
    ibv_extend.h
)

# 查找或构建rdma-core依赖（必须在add_library之前）
if(DEFINED LIBIBVERBS_BUILD_DIR)
    set(IBVERBS_INCLUDE_DIRS ${LIBIBVERBS_BUILD_DIR}/include)
    set(IBVERBS_LIBRARY_DIRS ${LIBIBVERBS_BUILD_DIR}/lib)
    set(IBVERBS_LIBRARIES ibverbs)
    message(STATUS "Using manually specified libibverbs paths:")
    message(STATUS "  Include: ${IBVERBS_INCLUDE_DIRS}")
    message(STATUS "  Library: ${IBVERBS_LIBRARY_DIRS}")
else()
    if(NOT DEFINED LIBIBVERBS_SOURCE_DIR)
        # 确保3rd目录存在
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rd")
            file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd")
        endif()
        set(LIBIBVERBS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/rdma-core)
    endif()

    # 设置rdma-core路径
    set(RDMA_CORE_BUILD_DIR "${LIBIBVERBS_SOURCE_DIR}/build")

    # 检查是否需要下载rdma-core源码
    if(NOT EXISTS "${LIBIBVERBS_SOURCE_DIR}/CMakeLists.txt")
        set(RDMA_CORE_VERSION "58.1")
        set(RDMA_CORE_URL "https://github.com/linux-rdma/rdma-core/releases/download/v${RDMA_CORE_VERSION}/rdma-core-${RDMA_CORE_VERSION}.tar.gz")
        set(RDMA_CORE_ARCHIVE "${CMAKE_BINARY_DIR}/rdma-core-${RDMA_CORE_VERSION}.tar.gz")

        message(STATUS "==================================================")
        message(STATUS "Downloading rdma-core ${RDMA_CORE_VERSION}")
        message(STATUS "URL: ${RDMA_CORE_URL}")
        message(STATUS "This may take several minutes depending on network...")
        message(STATUS "==================================================")

        # 使用 file(DOWNLOAD) 以支持超时和进度显示
        file(DOWNLOAD
            ${RDMA_CORE_URL}
            ${RDMA_CORE_ARCHIVE}
            TIMEOUT 600            # 10分钟超时
            INACTIVITY_TIMEOUT 60 # 60秒无数据传输则超时
            SHOW_PROGRESS          # 显示下载进度
            STATUS DOWNLOAD_STATUS
        )

        # 检查下载结果
        list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
        list(GET DOWNLOAD_STATUS 1 DOWNLOAD_MESSAGE)
        if(NOT DOWNLOAD_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to download rdma-core: ${DOWNLOAD_MESSAGE}\n"
                "Please check your network connection or download manually:\n"
                "  wget ${RDMA_CORE_URL}\n"
                "  Extract to: ${LIBIBVERBS_SOURCE_DIR}")
        endif()

        message(STATUS "Download completed, extracting...")

        # 解压文件到临时目录
        set(EXTRACT_DIR "${CMAKE_BINARY_DIR}/rdma-core-extract")
        file(MAKE_DIRECTORY ${EXTRACT_DIR})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${RDMA_CORE_ARCHIVE}
            WORKING_DIRECTORY ${EXTRACT_DIR}
            RESULT_VARIABLE EXTRACT_RESULT
        )

        if(NOT EXTRACT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to extract rdma-core archive")
        endif()

        file(REMOVE_RECURSE ${LIBIBVERBS_SOURCE_DIR})
        file(RENAME "${EXTRACT_DIR}/rdma-core-${RDMA_CORE_VERSION}" ${LIBIBVERBS_SOURCE_DIR})
        file(REMOVE_RECURSE ${EXTRACT_DIR})

        # 清理压缩包
        file(REMOVE ${RDMA_CORE_ARCHIVE})

        message(STATUS "rdma-core source ready at: ${LIBIBVERBS_SOURCE_DIR}")
    endif()

    # 检查是否需要编译rdma-core
    if(NOT EXISTS "${RDMA_CORE_BUILD_DIR}/lib/libibverbs.so")
        message(STATUS "Building rdma-core ${RDMA_CORE_VERSION}...")

        # 安装编译依赖 - 支持 yum (openEuler/CentOS/RHEL) 和 apt-get (Ubuntu/Debian)
        find_program(YUM yum)
        find_program(APT_GET apt-get)

        if(YUM)
            # openEuler/CentOS/RHEL 使用 yum
            message(STATUS "Detected yum package manager (openEuler/CentOS/RHEL)")
            message(STATUS "Installing gcc gcc-c++ make cmake libnl3-devel libudev-devel pkgconfig python3-devel")
            execute_process(
                COMMAND sudo yum install -y gcc gcc-c++ make cmake libnl3-devel libudev-devel pkgconfig python3-devel
                WORKING_DIRECTORY ${LIBIBVERBS_SOURCE_DIR}
                RESULT_VARIABLE DEPS_INSTALL_RESULT
            )
            if(NOT DEPS_INSTALL_RESULT EQUAL 0)
                message(WARNING "Failed to install some dependencies, please install manually")
            endif()
        elseif(APT_GET)
            # Ubuntu/Debian 使用 apt-get
            message(STATUS "Detected apt-get package manager (Ubuntu/Debian)")
            message(STATUS "Installing gcc g++ make cmake libnl-3-dev libudev-dev pkg-config python3-dev")
            execute_process(
                COMMAND sudo apt-get install -y gcc g++ make cmake libnl-3-dev libudev-dev pkg-config python3-dev
                WORKING_DIRECTORY ${LIBIBVERBS_SOURCE_DIR}
                RESULT_VARIABLE DEPS_INSTALL_RESULT
            )
            if(NOT DEPS_INSTALL_RESULT EQUAL 0)
                message(WARNING "Failed to install some dependencies, please install manually")
            endif()
        else()
            message(WARNING "Neither yum nor apt-get found, please install dependencies manually")
            message(STATUS "Required dependencies:")
            message(STATUS "  For openEuler/CentOS/RHEL: gcc gcc-c++ make cmake libnl3-devel libudev-devel pkgconfig python3-devel")
            message(STATUS "  For Ubuntu/Debian: gcc g++ make cmake libnl-3-dev libudev-dev pkg-config python3-dev")
        endif()

        # 执行build.sh脚本
        if(EXISTS "${LIBIBVERBS_SOURCE_DIR}/build.sh")
            execute_process(
                COMMAND bash build.sh
                WORKING_DIRECTORY ${LIBIBVERBS_SOURCE_DIR}
                RESULT_VARIABLE BUILD_RESULT
            )
            if(NOT BUILD_RESULT EQUAL 0)
                message(WARNING "Failed to build rdma-core, please build manually")
            endif()
        endif()
    endif()
    # 编译完成
    if(EXISTS "${RDMA_CORE_BUILD_DIR}/lib/libibverbs.so")
        message(STATUS "rdma-core built successfully")
        # 设置libibverbs路径
        set(IBVERBS_INCLUDE_DIRS "${RDMA_CORE_BUILD_DIR}/include")
        set(IBVERBS_LIBRARY_DIRS "${RDMA_CORE_BUILD_DIR}/lib")
        set(IBVERBS_LIBRARIES ibverbs)
    else()
        message(FATAL_ERROR "Failed to build rdma-core")
    endif()
endif()

message(STATUS "Using rdma-core from source build:")
message(STATUS "  Source:  ${LIBIBVERBS_SOURCE_DIR}")
message(STATUS "  Include: ${IBVERBS_INCLUDE_DIRS}")
message(STATUS "  Library: ${IBVERBS_LIBRARY_DIRS}")

# 查找或构建libboundscheck安全函数库依赖
if(DEFINED LIBBOUNDSCHECK_BUILD_DIR)
    # 使用预编译的libboundscheck
    set(BOUNDSCHECK_INCLUDE_DIRS ${LIBBOUNDSCHECK_BUILD_DIR}/include)
    set(BOUNDSCHECK_LIBRARY_DIRS ${LIBBOUNDSCHECK_BUILD_DIR}/lib)
    set(BOUNDSCHECK_LIBRARIES c_sec)
    message(STATUS "Using pre-built libboundscheck:")
    message(STATUS "  Include: ${BOUNDSCHECK_INCLUDE_DIRS}")
    message(STATUS "  Library: ${BOUNDSCHECK_LIBRARY_DIRS}")
else()
    if(NOT DEFINED LIBBOUNDSCHECK_SOURCE_DIR)
        # 确保3rd目录存在
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rd")
            file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd")
        endif()
        set(LIBBOUNDSCHECK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/libboundscheck)
    endif()

    # 设置libboundscheck路径
    set(BOUNDSCHECK_BUILD_DIR "${LIBBOUNDSCHECK_SOURCE_DIR}")

    # 检查是否需要下载libboundscheck源码
    if(NOT EXISTS "${LIBBOUNDSCHECK_SOURCE_DIR}/Makefile")
        set(BOUNDSCHECK_VERSION "1.1.16")
        set(BOUNDSCHECK_URL "https://gitcode.com/cann-src-third-party/libboundscheck/releases/download/v${BOUNDSCHECK_VERSION}/libboundscheck-v${BOUNDSCHECK_VERSION}.tar.gz")
        set(BOUNDSCHECK_ARCHIVE "${CMAKE_BINARY_DIR}/libboundscheck-v${BOUNDSCHECK_VERSION}.tar.gz")

        message(STATUS "==================================================")
        message(STATUS "Downloading libboundscheck ${BOUNDSCHECK_VERSION}")
        message(STATUS "URL: ${BOUNDSCHECK_URL}")
        message(STATUS "This may take several minutes depending on network...")
        message(STATUS "==================================================")

        # 使用 file(DOWNLOAD) 以支持超时和进度显示
        file(DOWNLOAD
            ${BOUNDSCHECK_URL}
            ${BOUNDSCHECK_ARCHIVE}
            TIMEOUT 600            # 10分钟超时
            INACTIVITY_TIMEOUT 60 # 60秒无数据传输则超时
            SHOW_PROGRESS          # 显示下载进度
            STATUS DOWNLOAD_STATUS
        )

        # 检查下载结果
        list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
        list(GET DOWNLOAD_STATUS 1 DOWNLOAD_MESSAGE)
        if(NOT DOWNLOAD_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to download libboundscheck: ${DOWNLOAD_MESSAGE}\n"
                "Please check your network connection or download manually:\n"
                "  wget ${BOUNDSCHECK_URL}\n"
                "  Extract to: ${LIBBOUNDSCHECK_SOURCE_DIR}")
        endif()

        message(STATUS "Download completed, extracting...")

        # 解压文件到临时目录
        set(EXTRACT_DIR "${CMAKE_BINARY_DIR}/libboundscheck-extract")
        file(MAKE_DIRECTORY ${EXTRACT_DIR})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${BOUNDSCHECK_ARCHIVE}
            WORKING_DIRECTORY ${EXTRACT_DIR}
            RESULT_VARIABLE EXTRACT_RESULT
        )

        if(NOT EXTRACT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to extract libboundscheck archive")
        endif()

        file(REMOVE_RECURSE ${LIBBOUNDSCHECK_SOURCE_DIR})
        file(RENAME "${EXTRACT_DIR}/libboundscheck-v${BOUNDSCHECK_VERSION}" ${LIBBOUNDSCHECK_SOURCE_DIR})
        file(REMOVE_RECURSE ${EXTRACT_DIR})

        # 清理压缩包
        file(REMOVE ${BOUNDSCHECK_ARCHIVE})

        message(STATUS "libboundscheck source ready at: ${LIBBOUNDSCHECK_SOURCE_DIR}")
    endif()

    # 检查是否需要编译libboundscheck
    if(NOT EXISTS "${BOUNDSCHECK_BUILD_DIR}/lib/libc_sec.so")
        message(STATUS "Building libboundscheck ${BOUNDSCHECK_VERSION}...")

        # 直接使用make编译
        execute_process(
            COMMAND make -j
            WORKING_DIRECTORY ${LIBBOUNDSCHECK_SOURCE_DIR}
            RESULT_VARIABLE BUILD_RESULT
        )

        if(NOT BUILD_RESULT EQUAL 0)
            message(WARNING "Failed to build libboundscheck, please build manually")
        else()
            message(STATUS "libboundscheck compiled successfully")
            
            # 重命名libboundscheck.so为libc_sec.so
            if(EXISTS "${BOUNDSCHECK_BUILD_DIR}/lib/libboundscheck.so")
                execute_process(
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${BOUNDSCHECK_BUILD_DIR}/lib/libboundscheck.so"
                    "${BOUNDSCHECK_BUILD_DIR}/lib/libc_sec.so"
                    RESULT_VARIABLE RENAME_RESULT
                )
                if(NOT RENAME_RESULT EQUAL 0)
                    message(WARNING "Failed to rename libboundscheck.so to libc_sec.so")
                else()
                    message(STATUS "Renamed libboundscheck.so to libc_sec.so")
                endif()
            endif()
        endif()
    endif()

    # 编译完成
    if(EXISTS "${BOUNDSCHECK_BUILD_DIR}/lib/libc_sec.so")
        message(STATUS "libboundscheck built successfully")
        # 设置libboundscheck路径
        set(BOUNDSCHECK_INCLUDE_DIRS "${BOUNDSCHECK_BUILD_DIR}/include")
        set(BOUNDSCHECK_LIBRARY_DIRS "${BOUNDSCHECK_BUILD_DIR}/lib")
        set(BOUNDSCHECK_LIBRARIES c_sec)
    else()
        message(FATAL_ERROR "Failed to build libboundscheck")
    endif()

    if(NOT "${BOUNDSCHECK_LIBRARIES}" STREQUAL "")
        message(STATUS "Using libboundscheck from source build:")
        message(STATUS "  Source:  ${LIBBOUNDSCHECK_SOURCE_DIR}")
        message(STATUS "  Include: ${BOUNDSCHECK_INCLUDE_DIRS}")
        message(STATUS "  Library: ${BOUNDSCHECK_LIBRARY_DIRS}")
    endif()
endif()

# 添加库搜索路径（必须在add_library之前），兼容旧版CMake
if(EXISTS "${IBVERBS_LIBRARY_DIRS}")
    link_directories(${IBVERBS_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "IBVERBS_LIBRARY_DIRS does not exist: ${IBVERBS_LIBRARY_DIRS}")
endif()

# 添加libboundscheck库搜索路径
if(NOT "${BOUNDSCHECK_LIBRARIES}" STREQUAL "" AND EXISTS "${BOUNDSCHECK_LIBRARY_DIRS}")
    link_directories(${BOUNDSCHECK_LIBRARY_DIRS})
endif()

# 创建共享库
add_library(ibv_extend SHARED
    ${IBV_EXTEND_SOURCES}
    ${IBV_EXTEND_HEADERS}
)

# 设置库版本号
set_target_properties(ibv_extend PROPERTIES
    VERSION ${IBV_EXTEND_VERSION}
    SOVERSION ${IBV_EXTEND_VERSION_MAJOR}
    OUTPUT_NAME "ibv_extend"
)

# 设置包含目录
target_include_directories(ibv_extend PRIVATE
    ${BUILD_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 检查包含目录是否存在
if(EXISTS "${IBVERBS_INCLUDE_DIRS}")
    target_include_directories(ibv_extend SYSTEM PRIVATE ${IBVERBS_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "IBVERBS_INCLUDE_DIRS does not exist: ${IBVERBS_INCLUDE_DIRS}")
endif()

# 添加libboundscheck包含目录
if(NOT "${BOUNDSCHECK_LIBRARIES}" STREQUAL "" AND EXISTS "${BOUNDSCHECK_INCLUDE_DIRS}")
    target_include_directories(ibv_extend SYSTEM PRIVATE ${BOUNDSCHECK_INCLUDE_DIRS})
endif()

# 链接库
if(NOT "${BOUNDSCHECK_LIBRARIES}" STREQUAL "")
    target_link_libraries(ibv_extend PRIVATE ${IBVERBS_LIBRARIES} ${BOUNDSCHECK_LIBRARIES})
else()
    target_link_libraries(ibv_extend PRIVATE ${IBVERBS_LIBRARIES})
endif()

if(DEFINED IBVERBS_LDFLAGS AND NOT "${IBVERBS_LDFLAGS}" STREQUAL "")
    target_link_options(ibv_extend PRIVATE ${IBVERBS_LDFLAGS})
endif()

# 添加编译选项
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # 通用编译选项
    target_compile_options(ibv_extend PRIVATE
        -Wall
        -Wextra
        -fPIC
        -fvisibility=hidden
    )

    # 根据构建类型设置不同选项
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Debug 模式: 添加调试信息，关闭优化，启用调试宏
        target_compile_options(ibv_extend PRIVATE
            -g
            -O0
            -DDEBUG
        )
        target_compile_definitions(ibv_extend PRIVATE DEBUG)
    else()
        # Release 模式: 启用优化，将警告视为错误
        target_compile_options(ibv_extend PRIVATE
            -O2
            -Werror
            -DNDEBUG
        )
    endif()
endif()

# 编译测试程序
option(BUILD_TESTS "Build test programs" ON)
option(BUILD_UNIT_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    # 包含st目录下的CMakeLists.txt
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/st/CMakeLists.txt")
        add_subdirectory(st)
    endif()

    # 包含ut目录下的CMakeLists.txt
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ut/CMakeLists.txt")
        add_subdirectory(ut)
    endif()
endif()

# 安装规则
install(TARGETS ibv_extend
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

install(FILES ${IBV_EXTEND_HEADERS}
    DESTINATION include
)