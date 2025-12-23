# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
set(INC_PATH_LIST
    ${RDMA_CORE_INCLUDE}
    ../inc_extend/private/network
    ../inc_extend/hns_roce_user
    ${DRIVER_USER_DIR}/libc_sec
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
    ${C_SEC_INCLUDE}
    ${DRIVER_HAL_INC_DIR}
)

add_library(ascend_rdma_lite SHARED
    hns_roce_lite.c
    hns_roce_lite_stdio.c
    rdma_lite.c)

target_include_directories(ascend_rdma_lite PRIVATE
    ${INC_PATH_LIST}
)

target_compile_options(ascend_rdma_lite PRIVATE
    -fPIC
    -pipe
    -Wall
    -Wextra
    -Wfloat-equal
    -fno-common
    -O2
    -Werror
    -fno-strict-aliasing
    -fstack-protector-strong
    -fsigned-char
    -fno-common
    -std=gnu11
    -Wno-unused-parameter
)

target_link_options(ascend_rdma_lite PRIVATE
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,-z,noexecstack
    -Wl,--build-id=none
    -s
    -shared
    -Wl,-Bsymbolic
)

target_compile_definitions(ascend_rdma_lite PRIVATE
    OPEN_BUILD_PROJECT
    HNS_ROCE_DEVICE
    LOG_CPP
    $<$<STREQUAL:${PRODUCT_SIDE},host>:LOG_HOST>)

target_link_libraries(ascend_rdma_lite PRIVATE
    -Wl,--no-as-needed
    ascend_hal
    -Wl,--as-needed
    -lrt
    -ldl
    -lpthread
)

install(TARGETS ascend_rdma_lite
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL
)

add_dependencies(ascend_rdma_lite ${RDMA_CORE_NAME})
