# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
add_library(hns-rdma-hal SHARED
    hns_roce_hal.c
)

target_include_directories(hns-rdma-hal PRIVATE
    ../inc_extend/private/network
    ${DRIVER_USER_DIR}/libc_sec
    ${DRIVER_USER_DIR}/inc
    ${DRIVER_USER_DIR}/msnpureport/inc/slog
    ${DRIVER_HAL_INC_DIR}
)

target_compile_options(hns-rdma-hal PRIVATE
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

target_link_options(hns-rdma-hal PRIVATE
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,-z,noexecstack
    -Wl,--build-id=none
    -s
    -shared
    -Wl,-Bsymbolic
)

target_compile_definitions(hns-rdma-hal PRIVATE
    OPEN_BUILD_PROJECT
    HNS_ROCE_DEVICE
     $<$<STREQUAL:${PRODUCT_SIDE},host>:LOG_HOST>)

target_link_libraries(hns-rdma-hal PRIVATE
    -Wl,--no-as-needed
    c_sec
    slog
    ascend_hal
    -Wl,--as-needed
)

install(TARGETS  hns-rdma-hal
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL
)
