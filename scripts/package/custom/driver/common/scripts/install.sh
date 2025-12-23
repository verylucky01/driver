#!/bin/bash

# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
runLogFile="${ASCEND_SECLOG}/ascend_run_servers.log"
installInfo="/etc/ascend_install.info"
ASCEND_DRIVER_CFG_FILE="/etc/ascend_driver.conf"
KMSAGENT_CONFIG_ITEM="kmsagent"
KMSAGENT_WORK_DIR="/var/kmsagentd"
KMSAGENT_CONF_MAIN_KSF="/var/kmsagentd/kmsconf.ksf"
KMSAGENT_SERVICE_DIR="/lib/systemd/system"
KMSAGENT_USER_SERVICE_DIR="/usr/lib/systemd/system"
KMSAGENT_SERVICE_NAME="ascend-kmsagent.service"
ASCEND_DRIVER_SETUP_SCRIPT="ascend_driver_config.sh"
username=HwHiAiUser
usergroup=HwHiAiUser
hotreset_status_file="/var/log/hotreset_status.log"
driverCrlStatusFile="/root/ascend_check/driver_crl_status_tmp"
davinci_num_file="/var/log/davinci_num.log"

# load specific_func.inc
source "${PWD}"/driver/script/specific_func.inc

# load log_func
source ./driver/script/log_common.sh

# load common.sh, get install.info
sourcedir="$PWD"/driver
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
COMMON_SHELL="$SHELL_DIR/common.sh"
source "${COMMON_SHELL}"

OPERATION_LOGDIR="${ASCEND_SECLOG}"
OPERATION_LOGPATH="${OPERATION_LOGDIR}/operation.log"
LOG_OPERATION_INSTALL="Install"
LOG_OPERATION_UPGRADE="Upgrade"
LOG_OPERATION_UNINSTALL="Uninstall"
LOG_LEVEL_SUGGESTION="SUGGESTION"
LOG_LEVEL_MINOR="MINOR"
LOG_LEVEL_MAJOR="MAJOR"
LOG_LEVEL_UNKNOWN="UNKNOWN"
LOG_RESULT_SUCCESS="success"
LOG_RESULT_FAILED="failed"
# the two are both for secure-install-path
first_time_install_drv_flag="NA"
old_drv_install_path="NA"
install_for_all="no"
native_pkcs_conf=""

# pkg arch
PKG_ARCH="UNKNOWN"

# pcie buf number
PCIE_BDF_910B="19e5:d802"

# check if it is the first time installation flag
first_time=y

export feature_hot_reset=n
export feature_crl_check=n
export feature_nvcnt_compile=n
export feature_dkms_compile=n
export input_install_for_all=n
export feature_virt_scene=n
export feature_no_device_kernel=n
export feature_ver_compatible_check=n

export native_pkcs_conf


statusUpdate() {
    echo "Driver_Install_Status=complete" >> $installInfo
    log "Installation status updated successfully"
}

main() {
    # parameter parsing
    source ./driver/script/args_parse.sh "$@"|| exit 1

    # pre-installation system check
    source ./driver/script/env_check.sh || exit 1

    # install | upgrade | uninstall
    source ./driver/script/driver_install.sh || exit 1

    if [ $driver_install_status -eq 0 ];then
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_SUCCESS}" "${installType}" "${all_parma}"
    else
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
        exitLog 1
    fi

    # return value: 0-installation and hotreset successful, 1-installation failed, 2-installation successful but hotreset failed
    if [ $docker = y ] || [ $devel = y ]; then
        exitLog 0
    else
        statusUpdate
        sync
        log "[INFO]The sync is executed."
        sleep 1
        if [ "$(isHotresetFailed)" = "true" ]; then
            exitLog 2
        else
            exitLog 0
        fi
    fi
}

main "$@"