# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

add_library(intf_pub INTERFACE)

set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> -D__FILE__='\"$(notdir $(abspath <SOURCE>))\"' -Wno-builtin-macro-redefined <INCLUDES> <FLAGS> -o <OBJECT>   -c <SOURCE>")
target_compile_options(intf_pub INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    -fPIC
    -pipe
    -Wall
    -Wextra
    -Wfloat-equal
    -fno-common
    -fstack-protector-strong
    $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address -fsanitize=leak -fsanitize-recover=address,all -fno-stack-protector -fno-omit-frame-pointer -g>
    $<$<BOOL:${ENABLE_TSAN}>:-fsanitize=thread -fsanitize-recover=thread,all -g>
    $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined -fno-sanitize=alignment -g>
    $<$<BOOL:${ENABLE_GCOV}>:-fprofile-arcs -ftest-coverage>
)

target_compile_definitions(intf_pub INTERFACE
    # host中配置，device中不要配置
    $<$<COMPILE_LANGUAGE:CXX>:_GLIBCXX_USE_CXX11_ABI=0>
)

target_link_options(intf_pub INTERFACE
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,-z,noexecstack
    -Wl,--build-id=none
    $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:-pie>
    -s
    $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address -fsanitize=leak -fsanitize-recover=address>
    $<$<BOOL:${ENABLE_TSAN}>:-fsanitize=thread>
    $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>
    $<$<BOOL:${ENABLE_GCOV}>:-fprofile-arcs -ftest-coverage>
)

target_link_libraries(intf_pub INTERFACE
    $<$<BOOL:${ENABLE_GCOV}>:-lgcov>
    -pthread
)

# 如下增强编译选项组件代码整改完成后开启
target_compile_options(intf_pub INTERFACE
    # 可选安全编译选项，会影响性能，组件评估是否开启
    $<$<CONFIG:Release>:-O2 -D_FORTIFY_SOURCE=2>
    # 增强告警选项
    -Werror
    -Wconversion
    -Wundef
    -Wunused
    -Wcast-qual
    -Wpointer-arith
    -Wdate-time
    -Wunused-macros
    -Wformat=2
    -Wshadow
    -Wsign-compare
    -Wunused-macros
    -Wvla
    -Wdisabled-optimization
    -Wempty-body
    -Wignored-qualifiers
    -Wtype-limits
    -Wshift-negative-value
    -Wswitch-default
    -Wframe-larger-than=$<IF:$<OR:$<BOOL:${ENABLE_ASAN}>,$<BOOL:${ENABLE_UBSAN}>>,131072,32768>
    -Wshift-count-overflow
    -Wwrite-strings
    -Wmissing-format-attribute
    -Wformat-nonliteral
    $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
    $<$<COMPILE_LANGUAGE:C>:-Wnested-externs>
    $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>
    $<$<COMPILE_LANGUAGE:CXX>:-Wdelete-non-virtual-dtor>
    $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
    $<$<CXX_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:CXX>:-Wsized-deallocation>>
    $<$<CXX_COMPILER_ID:GNU>:-Wimplicit-fallthrough=3>
    $<$<CXX_COMPILER_ID:GNU>:-Wshift-overflow=2>
    $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-cond>
    $<$<CXX_COMPILER_ID:GNU>:-Wtrampolines>
    $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op>
    $<$<CXX_COMPILER_ID:GNU>:-Wsuggest-attribute=format>
    $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-branches>
    # -Wmissing-include-dirs
    $<$<CXX_COMPILER_ID:GNU>:-Wformat-signedness>
    $<$<CXX_COMPILER_ID:GNU>:-Wreturn-local-addr>
    -Wredundant-decls
    -Wfloat-conversion
)