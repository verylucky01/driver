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

add_subdirectory(soc_adapt)
target_sources(drvdsmi_host PRIVATE
    dsmi_product_ext.c
    dsmi_device_info.c
    dsmi_device_pcie_domain.c
    $<$<STREQUAL:${PRODUCT},ascend310>:devdrv_alarm.c>
    $<$<STREQUAL:${PRODUCT},ascend310>:dsmi_device_share.c>
    $<$<STREQUAL:${PRODUCT},ascend310p>:devdrv_alarm.c>
    $<$<STREQUAL:${PRODUCT},ascend310p>:dsmi_device_share.c>
    $<$<STREQUAL:${PRODUCT},ascend310B>:devdrv_alarm.c>
    $<$<STREQUAL:${PRODUCT},ascend310B>:dsmi_device_share.c>
)

target_include_directories(drvdsmi_host PRIVATE
    ${DRIVER_HAL_INC_DIR}
    ${DRIVER_HAL_INC_DIR}/dms
    ${DRIVER_USER_DIR}/dmc/dsmi/dsmi_product_ext
    ${DRIVER_USER_DIR}/dmc/dsmi/include/
    ${DRIVER_USER_DIR}/dmc/dms/common/
    ${DRIVER_USER_DIR}/inc/
    ${DRIVER_USER_DIR}/inc/dmc
    ${DRIVER_USER_DIR}/inc/dms/
    ${DRIVER_USER_DIR}/dms/common
    ${DRIVER_USER_DIR}/ascend_base_hal
    ${DRIVER_USER_DIR}/dms/drv_devmng/inc
    ${DRIVER_KERNEL_DIR}/pbl/dev_urd
    ${DRIVER_CUST_INC_DIR}
    $<$<STREQUAL:${PRODUCT},ascend310>:${DRIVER_USER_DIR}/dms/drv_devmng/ascend310>
    $<$<STREQUAL:${PRODUCT},ascend310B>:${DRIVER_USER_DIR}/dms/drv_devmng/ascend910>
    $<$<STREQUAL:${PRODUCT},ascend310p>:${DRIVER_USER_DIR}/dms/drv_devmng/ascend910>
    $<$<STREQUAL:${PRODUCT},ascend910>:${DRIVER_USER_DIR}/dms/drv_devmng/ascend910>
    $<$<STREQUAL:${PRODUCT},ascend910B>:${DRIVER_USER_DIR}/dms/drv_devmng/ascend910>
)

target_compile_definitions(drvdsmi_host PRIVATE
    DSMI_PRODUCT_USER_INTERFACE
    $<$<STREQUAL:${PRODUCT},ascend310>:CFG_SOC_PLATFORM_MINI>
    $<$<STREQUAL:${PRODUCT},ascend310>:CFG_PCIE_BDF>
    $<$<STREQUAL:${PRODUCT},ascend310>:CFG_DEVICE_ALARMINFO>
    $<$<STREQUAL:${PRODUCT},ascend310>:CFG_PCIE_ERROR_RATE>
    $<$<STREQUAL:${PRODUCT},ascend310>:CFG_PCIE_HOT_RESET>
    $<$<STREQUAL:${PRODUCT},ascend310B>:CFG_SOC_PLATFORM_MINIV3>
    $<$<STREQUAL:${PRODUCT},ascend310B>:CFG_DEVICE_ALARMINFO>
    $<$<STREQUAL:${PRODUCT},ascend310p>:CFG_SOC_PLATFORM_MINIV2>
    $<$<STREQUAL:${PRODUCT},ascend310p>:CFG_DEVICE_ALARMINFO>
    $<$<STREQUAL:${PRODUCT},ascend310p>:CFG_PCIE_ERROR_RATE>
    $<$<STREQUAL:${PRODUCT},ascend310p>:CFG_FEATURE_ECC_DDR>
    $<$<STREQUAL:${PRODUCT},ascend310p>:CFG_FEATURE_ROOTKEY_STATUS>
    $<$<STREQUAL:${PRODUCT},ascend910>:CFG_SOC_PLATFORM_CLOUD>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_SOC_PLATFORM_CLOUD_V2>
    $<$<STREQUAL:${PRODUCT},ascend310p>:CFG_FEATURE_DEVICE_SHARE>
    $<$<STREQUAL:${PRODUCT},ascend310B>:CFG_FEATURE_DEVICE_SHARE>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_PCIE_HCCS_BANDWIDTH>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_HBM_MANUFACTURER_ID>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_ROOTKEY_STATUS>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_SERDES_INFO>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_INIT_MCU_BOARD_ID>
    $<$<STREQUAL:${PRODUCT},ascend910B>:CFG_FEATURE_HCCS_BANDWIDTH>
)
