# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

#### CPACK to package run #####

#### built-in package ####
message(STATUS "System processor: ${CMAKE_SYSTEM_PROCESSOR}")
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    message(STATUS "Detected architecture: x86_64")
    set(ARCH x86_64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|arm")
    message(STATUS "Detected architecture: ARM64")
    set(ARCH aarch64)
else ()
    message(WARNING "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

set(script_prefix ${CMAKE_SOURCE_DIR}/scripts/package)
set(INSTALL_SCRIPTS_FILES
    ${script_prefix}/driver/common/scripts/ver_check.sh
    ${CMAKE_BINARY_DIR}/lib/script/hccn_weak_dict.conf
    ${script_prefix}/common/sh/common_func.inc
    ${script_prefix}/common/sh/version_compatiable.inc
    ${CMAKE_BINARY_DIR}/hdcBasic.cfg
    ${script_prefix}/driver/ascend910B/conf/dms_events_conf.lst
)

# 以下文件是跟产品重复的部分，构建产品包时以产品文件为准
if(NOT ENABLE_BUILD_PRODUCT)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/common.sh)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/device_crl_check.sh)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/device_hot_reset.sh)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/ascend910B/feature.conf)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/ascend910B/scripts/help.info)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/install.sh)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/livepatch_sys_init.sh)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/run_driver_install.sh)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/run_driver_uninstall.sh)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/setenv.bash)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/uninstall.sh)
endif()

if(ASCEND910_93_EX)
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/ascend910_93/scripts/specific_func.inc)
else()
    list(APPEND INSTALL_SCRIPTS_FILES ${script_prefix}/driver/${PRODUCT}/scripts/specific_func.inc)
endif()

install(FILES ${INSTALL_SCRIPTS_FILES}
    DESTINATION driver/script
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

set(COMMOM_SCRIPTS_FILES
    ${script_prefix}/common/sh/common_func_v2.inc
    ${script_prefix}/common/sh/common_func_v3.inc
    ${script_prefix}/common/sh/common_installer.inc
    ${script_prefix}/common/sh/install_common_parser.sh
    ${script_prefix}/common/sh/script_operator.inc
)

# 以下文件是跟产品重复的部分，构建产品包时以产品文件为准
if(NOT ENABLE_BUILD_PRODUCT)
    list(APPEND COMMOM_SCRIPTS_FILES ${script_prefix}/driver/ascend910B/scripts/host_sys_init.sh)
    list(APPEND COMMOM_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/host_servers_setup.sh)
    list(APPEND COMMOM_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/host_servers_remove.sh)
    list(APPEND COMMOM_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/host_services_setup.sh)
    list(APPEND COMMOM_SCRIPTS_FILES ${script_prefix}/driver/common/scripts/host_services_exit.sh)
endif()

install(FILES ${COMMOM_SCRIPTS_FILES}
    DESTINATION .
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

set(source_prefix ${CMAKE_SOURCE_DIR})
set(HEADER_FILES
    ${source_prefix}/pkg_inc/dsmi_common_interface.h
    ${source_prefix}/pkg_inc/dsmi_common_interface_base.h
    ${source_prefix}/pkg_inc/dms_device_node_type.h
    ${source_prefix}/pkg_inc/ascend_hal_error.h
)
install(FILES ${HEADER_FILES}
    DESTINATION driver/include
)

set(device_delivery_prefix ${CMAKE_BINARY_DIR}/lib/host)
set(DEVICE_DELIVERY_FILES
    ${device_delivery_prefix}/device_sw.img
    ${device_delivery_prefix}/device_sw.bin
    ${device_delivery_prefix}/hisserika.bin
    ${device_delivery_prefix}/itrustee.img
    ${device_delivery_prefix}/device_boot.img
    ${device_delivery_prefix}/IMU_task.bin
    ${device_delivery_prefix}/hbm_img.bin
    ${device_delivery_prefix}/sysBaseConfig.bin
    ${device_delivery_prefix}/network_fw_offline_2PF_asic.bin
    ${device_delivery_prefix}/device_config.bin
    ${device_delivery_prefix}/device_config_16g.bin
)

install(FILES ${DEVICE_DELIVERY_FILES}
    DESTINATION driver/device
)

set(host_delivery_prefix ${CMAKE_BINARY_DIR}/lib)
set(HOST_DELIVERY_FILES
    ${host_delivery_prefix}/asdrv_pbl.ko
    ${host_delivery_prefix}/ascend_kernel_open_adapt.ko
    ${host_delivery_prefix}/asdrv_esched.ko
    ${host_delivery_prefix}/ascend_soc_platform.ko
    ${host_delivery_prefix}/asdrv_fms.ko
    ${host_delivery_prefix}/asdrv_trsbase.ko
    ${host_delivery_prefix}/asdrv_trs.ko
    ${host_delivery_prefix}/asdrv_svm.ko
    ${host_delivery_prefix}/asdrv_dms.ko
    ${host_delivery_prefix}/asdrv_dpa.ko
    ${host_delivery_prefix}/drv_pcie_hdc_host.ko
    ${host_delivery_prefix}/drv_pcie_host.ko
    ${host_delivery_prefix}/asdrv_vnic.ko
    ${host_delivery_prefix}/drv_seclib_host.ko
    ${host_delivery_prefix}/asdrv_queue.ko
    ${host_delivery_prefix}/asdrv_buff.ko
    ${host_delivery_prefix}/drv_vascend_stub.ko
    ${host_delivery_prefix}/asdrv_vmng.ko
    ${host_delivery_prefix}/drv_vascend.ko
    ${host_delivery_prefix}/asdrv_vpc.ko
    ${host_delivery_prefix}/ts_agent.ko
    ${host_delivery_prefix}/asdrv_vvpc.ko
    ${host_delivery_prefix}/drv_virtmng_host_stub.ko
    ${host_delivery_prefix}/asdrv_vtrs.ko
    ${host_delivery_prefix}/ts_agent_vm.ko
)
install(FILES ${HOST_DELIVERY_FILES}
    DESTINATION driver/host
)

set(lib64_driver_prefix ${CMAKE_BINARY_DIR}/lib)
set(LIB64_DRIVER_FILES
    ${lib64_driver_prefix}/libascend_hal.so
    ${lib64_driver_prefix}/libdrvdsmi_host.so
    ${lib64_driver_prefix}/libascend_rdma_lite.so
    ${CMAKE_BINARY_DIR}/lib/lib64/common/libtls_adp.so
    ${CMAKE_BINARY_DIR}/lib/lib64/common/libascend_kms.so
)
install(FILES ${LIB64_DRIVER_FILES}
    DESTINATION driver/lib64/driver
)

set(lib64_common_prefix ${CMAKE_BINARY_DIR}/lib)
set(LIB64_COMMON_FILES
    ${lib64_common_prefix}/libc_sec.so
    ${lib64_common_prefix}/libmmpa.so
    ${CMAKE_BINARY_DIR}/lib/lib64/common/dcache_lock_mix.o
)
install(FILES ${LIB64_COMMON_FILES}
    DESTINATION driver/lib64/common
)

set(tools_prefix ${CMAKE_BINARY_DIR}/lib/tools)
set(TOOLS_FILES
    ${CMAKE_SOURCE_DIR}/scripts/package/driver/common/scripts/device_boot_init.sh
    ${CMAKE_BINARY_DIR}/lib/msnpureport_auto_export.sh
    ${CMAKE_BINARY_DIR}/lib/msnpureport
    ${CMAKE_SOURCE_DIR}/scripts/package/driver/common/scripts/install_npudrv.sh
    ${CMAKE_BINARY_DIR}/lib/ascend_upgrade_crl.sh
    ${tools_prefix}/ascend_check.bin
    ${tools_prefix}/upgrade-tool
    ${tools_prefix}/hccn_tool
)

install(FILES ${TOOLS_FILES}
    DESTINATION driver/tools
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

if(ENABLE_BUILD_PRODUCT)
    include(${CMAKE_SOURCE_DIR}/src/custom/cmake/package_${PRODUCT}.cmake)
endif()

string(FIND "${ASCEND_COMPUTE_UNIT}" ";" SEMICOLON_INDEX)
if (SEMICOLON_INDEX GREATER -1)
    # 截取分号前的字串
    math(EXPR SUBSTRING_LENGTH "${SEMICOLON_INDEX}")
    string(SUBSTRING "${ASCEND_COMPUTE_UNIT}" 0 "${SUBSTRING_LENGTH}" compute_unit)
else()
    # 没有分号取全部内容
    set(compute_unit "${ASCEND_COMPUTE_UNIT}")
endif()

message(STATUS "current compute_unit is: ${compute_unit}")

# ============= CPack =============
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")

set(CPACK_INSTALL_PREFIX "/")

set(CPACK_CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
set(CPACK_CMAKE_BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CPACK_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CPACK_SOC "${CPACK_PRODUCT}")
set(CPACK_OS_VERSION "${HOST_LINUX_DISTRIBUTOR_ID}${HOST_LINUX_DISTRIBUTOR_RELEASE}")
set(CPACK_ARCH "${ARCH}")
set(CPACK_SET_DESTDIR ON)
set(CPACK_GENERATOR External)
set(CPACK_EXTERNAL_PACKAGE_SCRIPT "${CMAKE_SOURCE_DIR}/cmake/makeself_built_in.cmake")
set(CPACK_EXTERNAL_ENABLE_STAGING true)
set(CPACK_PACKAGE_DIRECTORY "${CPACK_PACKAGE_OUTPUT_DIRECTORY}")

message(STATUS "Package output path = ${CPACK_PACKAGE_OUTPUT_DIRECTORY}")
include(CPack)
