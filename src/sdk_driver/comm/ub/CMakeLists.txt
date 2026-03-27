# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
if((${PRODUCT} STREQUAL ascend950) OR (${PRODUCT} STREQUAL ascend950esl) OR
	(${PRODUCT} STREQUAL ascend910_96) OR (${PRODUCT} STREQUAL ascend910_96esl))

    if (${PRODUCT_SIDE} STREQUAL host)
        set(asdrv_ub_depends asdrv_pbl ascend_kernel_open_adapt)
        add_host_ko(LOCAL_MODULE asdrv_ub
                    KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
                    MAKE_ARGS ${MAKE_ARGS} PRODUCT_SIDE=host
                    TARGETE_DPENDS "${asdrv_ub_depends}")
    endif ()

    if (${PRODUCT_SIDE} STREQUAL device)
        set(asdrv_ub_depends asdrv_pbl ascend_kernel_open_adapt)
        add_device_ko(LOCAL_MODULE asdrv_ub
                      KO_SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
                      MAKE_ARGS ${MAKE_ARGS} PRODUCT_SIDE=device
                      TARGETE_DPENDS "${asdrv_ub_depends}")
    endif ()

    set(UB_PDT_LIST "ascend950" "ascend950esl")
    if("${PRODUCT_SIDE}" STREQUAL "host" AND "${PRODUCT}" IN_LIST UB_PDT_LIST)
        install(FILES
                ${TOP_DIR}/build/bin/ccec_for_milan/uvb_driver/cis.ko
                ${TOP_DIR}/build/bin/ccec_for_milan/uvb_driver/odf.ko
                DESTINATION lib OPTIONAL)
    endif ()
endif ()