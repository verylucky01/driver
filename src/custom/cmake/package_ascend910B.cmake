# ----------------------------------------------------------------------------
# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------
#### CPACK to package run #####
#### built-in package ####

set(script_prefix ${CMAKE_SOURCE_DIR}/scripts/package)
set(CUSTOM_INSTALL_SCRIPTS_FILES
    ${script_prefix}/custom/driver/common/scripts/args_parse.sh
    ${script_prefix}/custom/driver/common/scripts/ascend_driver_config.sh
    ${script_prefix}/custom/driver/common/scripts/check_tools.conf
    ${script_prefix}/custom/driver/common/scripts/common.sh
    ${script_prefix}/custom/driver/common/scripts/custom_op_config_recover.sh
    ${script_prefix}/custom/driver/common/scripts/task_op_timeout_config_recover.sh
    ${script_prefix}/custom/driver/common/scripts/device_share_config_recover.sh
    ${script_prefix}/custom/driver/common/scripts/deal_ko.sh
    ${script_prefix}/custom/driver/common/scripts/device_crl_check.sh
    ${script_prefix}/custom/driver/common/scripts/device_hot_reset.sh
    ${script_prefix}/custom/driver/common/scripts/driver_install.sh
    ${script_prefix}/custom/driver/common/scripts/env_check.sh
    ${script_prefix}/custom/driver/ascend910B/scripts/feature.conf
    ${script_prefix}/custom/driver/common/scripts/file_copy.sh
    ${script_prefix}/custom/driver/ascend910B/scripts/help.info
    ${script_prefix}/custom/driver/common/scripts/install.sh
    ${script_prefix}/custom/driver/common/scripts/livepatch_sys_init.sh
    ${script_prefix}/custom/driver/common/scripts/log_common.sh
    ${script_prefix}/custom/driver/common/scripts/msn_config_recover.sh
    ${script_prefix}/custom/driver/common/scripts/repack_driver.sh
    ${script_prefix}/custom/driver/common/scripts/run_driver_install.sh
    ${script_prefix}/custom/driver/common/scripts/run_driver_uninstall.sh
    ${script_prefix}/custom/driver/common/scripts/run_driver_upgrade_check.sh
    ${script_prefix}/custom/driver/common/scripts/setenv.bash
    ${script_prefix}/custom/driver/common/scripts/uninstall.sh
    ${script_prefix}/custom/driver/common/scripts/vnpu_config_recover.sh
    ${CMAKE_BINARY_DIR}/lib/script/kmsagent_backup_conf.sh
    ${CMAKE_BINARY_DIR}/lib/script/kmsagent_install.sh
    ${CMAKE_BINARY_DIR}/lib/script/kmsagent_restore_conf.sh
    ${CMAKE_BINARY_DIR}/lib/script/kmsagent_uninstall.sh
    ${CMAKE_BINARY_DIR}/lib/script/kmsagent.service
    ${CMAKE_SOURCE_DIR}/build/makeself/makeself-header.sh
    ${CMAKE_SOURCE_DIR}/build/makeself/makeself.sh
)
if(ASCEND910_93_EX)
    list(APPEND CUSTOM_INSTALL_SCRIPTS_FILES ${script_prefix}/custom/driver/ascend910_93/scripts/version_cap.map)
else()
    list(APPEND CUSTOM_INSTALL_SCRIPTS_FILES ${script_prefix}/custom/driver/${PRODUCT}/scripts/version_cap.map)
endif()
install(FILES ${CUSTOM_INSTALL_SCRIPTS_FILES}
    DESTINATION driver/script
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

set(CUSTOM_COMMOM_SCRIPTS_FILES
    ${script_prefix}/custom/driver/common/scripts/host_servers_remove.sh
    ${script_prefix}/custom/driver/common/scripts/host_servers_setup.sh
    ${script_prefix}/custom/driver/common/scripts/host_services_exit.sh
    ${script_prefix}/custom/driver/common/scripts/host_services_setup.sh
    ${script_prefix}/custom/driver/ascend910B/scripts/host_sys_init.sh
    ${script_prefix}/custom/driver/common/scripts/npu-healthcheck.sh
    ${script_prefix}/custom/driver/common/scripts/nputool-log
)
install(FILES ${CUSTOM_COMMOM_SCRIPTS_FILES}
    DESTINATION .
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

set(CUSTOM_INSTALL_HEADER_FILES
     ${CMAKE_SOURCE_DIR}/src/custom/include/dsmi_common_interface_custom.h
     ${CMAKE_SOURCE_DIR}/src/custom/include/dcmi_interface_api.h
)
install(FILES ${CUSTOM_INSTALL_HEADER_FILES}
    DESTINATION driver/include
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

set(host_delivery_prefix ${CMAKE_BINARY_DIR}/lib)
set(CUSTOM_HOST_DELIVERY_FILES
    ${host_delivery_prefix}/npu_peermem.ko
    ${host_delivery_prefix}/ts_debug.ko
    ${host_delivery_prefix}/debug_switch.ko
)
if(ASCEND910_93_EX)
    list(APPEND CUSTOM_HOST_DELIVERY_FILES ${host_delivery_prefix}/tc_pcidev.ko)
endif()
install(FILES ${CUSTOM_HOST_DELIVERY_FILES}
    DESTINATION driver/host
)

set(lib64_driver_prefix ${CMAKE_BINARY_DIR}/lib)
set(CUSTOM_LIB64_DRIVER_FILES
    ${lib64_driver_prefix}/libdsmi_network.so
    ${CMAKE_BINARY_DIR}/lib/lib64/driver/libaivault.so
    ${CMAKE_BINARY_DIR}/lib/lib64/driver/libaivaulttee.so
    ${CMAKE_BINARY_DIR}/lib/lib64/driver/libkmc.so.20
    ${CMAKE_BINARY_DIR}/lib/lib64/driver/libsdp.so.20
    ${CMAKE_BINARY_DIR}/lib/lib64/driver/libsecurec.so
    ${CMAKE_BINARY_DIR}/lib/lib64/driver/kmsagent.conf
)
if(ASCEND910_93_EX)
    list(APPEND CUSTOM_LIB64_DRIVER_FILES ${lib64_driver_prefix}/liblingqu-dcmi.so)
endif()
install(FILES ${CUSTOM_LIB64_DRIVER_FILES}
    DESTINATION driver/lib64/driver
)

set(CUSTOM_LIB64_STUB_FILES
    ${CMAKE_BINARY_DIR}/lib/lib64/stub/libcrypto.so
    ${CMAKE_BINARY_DIR}/lib/lib64/stub/libssl.so
)
install(FILES ${CUSTOM_LIB64_STUB_FILES}
    DESTINATION driver/lib64/stub
)

install(FILES ${lib64_driver_prefix}/libdcmi.so
    DESTINATION driver/lib64
)

set(tools_prefix ${CMAKE_BINARY_DIR}/lib/tools)
set(CUSTOM_TOOLS_FILES
    ${tools_prefix}/npu-smi
    ${tools_prefix}/build.info
    ${tools_prefix}/kmsagent
    ${CMAKE_SOURCE_DIR}/scripts/package/custom/version.info
    ${CMAKE_SOURCE_DIR}/scripts/package/custom/driver/common/scripts/ascend_bug_report.sh
    ${CMAKE_SOURCE_DIR}/scripts/package/custom/driver/common/scripts/npu_log_collect.sh
)
install(FILES ${CUSTOM_TOOLS_FILES}
    DESTINATION driver/tools
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

set(CUSTOM_CERT_FILES
    ${CMAKE_BINARY_DIR}/lib/cert/ca/Equipment_Root_CA_2041.pem
    ${CMAKE_BINARY_DIR}/lib/cert/ca/IT_Product_CA_2041.pem
    ${CMAKE_BINARY_DIR}/lib/cert/ca/Computing_RSA-PSS_Equipment_CA_2071.pem
    ${CMAKE_BINARY_DIR}/lib/cert/ca/Computing_RSA-PSS_Equipment_CA_2099.pem
    ${CMAKE_BINARY_DIR}/lib/cert/ca/RSA_Equipment_Root_CA_2071.pem
    ${CMAKE_BINARY_DIR}/lib/cert/ca/RSA-PSS_Equipment_Root_CA_2099.pem
)
install(FILES ${CUSTOM_CERT_FILES}
    DESTINATION driver/cert/ca
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)