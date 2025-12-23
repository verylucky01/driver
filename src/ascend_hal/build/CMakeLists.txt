# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
add_library(ascend_hal SHARED)
add_dependencies(ascend_hal secure_c)

target_link_libraries(ascend_hal PRIVATE
    $<BUILD_INTERFACE:intf_pub>
    urd_obj
    devdrv_obj
    dms_obj
    prof_obj
    eventsched_obj
    hdc_obj
    queue_obj
    commlib_obj
    uda_obj
    uda_base_obj
    trs_drv_obj
    rbtree_obj
    devmm_obj
    drvaibuff_obj
    drvaibuff_base_obj
    rmo_obj
    drv_log_obj
    drv_log_base_obj
    queryfeature_obj
    dp_proc_mng_obj
    -ldl
    c_sec
    rt
)

install(TARGETS ascend_hal
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR} OPTIONAL
)