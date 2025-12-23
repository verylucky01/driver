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
installInfo="/etc/ascend_install.info"
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
check=n
quiet=y
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
COMMON_SHELL="$SHELL_DIR/common.sh"
# load common.sh, get install.info.
source "${COMMON_SHELL}"
# load common_func.inc and version_compatiable.inc, for run-pkg compatible check.
source ${SHELL_DIR}/common_func.inc
if [ "${feature_ver_compatible_check}" = "y" ]; then
    source ${SHELL_DIR}/version_compatiable.inc
fi

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1" >> $logFile
    return 0
}
drvEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}
# check if root permission 
isRoot() {
    if [ $(id -u) -ne 0 ]; then
        drvEcho "[ERROR]ERR_NO:0x0093;ERR_DES:do not have root permission, operation failed,\
         please use root permission!"
        exit 1
    fi
}

createFolder() {
    if [ ! -d /var/log ]; then
        mkdir -p /var/log
        chmod 755 /var/log
    fi

    if [ ! -d "${ASCEND_SECLOG}" ]; then
        mkdir -p ${ASCEND_SECLOG}
        chmod 750 ${ASCEND_SECLOG}
    fi
}

changeLogMode() {
    if [ ! -f $logFile ]; then
        touch $logFile
    fi
    chmod 640 $logFile
}

versionCheck() {
    if [ -f $installInfo ];then
        # The install-cfg-file check is must, or current script will exit with 1.
        Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
        Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
        Firmware_Install_Path_Param=$(getInstallParam "Firmware_Install_Path_Param" "${installInfo}")

        # if develop or docker install-mode, return 0 directly.
        if [ "${Driver_Install_Type}" = "devel" ] || [ "${Driver_Install_Type}" = "docker" ];then
            log "[INFO]devel or docker installmode, stop version relationship check"
            return 0
        fi

        # run-pkg version compatible check, which is in the file common_func.inc.
        if [ "${feature_ver_compatible_check}" = "y" ]; then
            [ -z "${Driver_Install_Path_Param}" ] && Driver_Install_Path_Param="/usr/local/Ascend"
            preinstall_check --install-path="${Driver_Install_Path_Param}" --script-dir="${SHELL_DIR}" --package="driver" --logfile="${logFile}" --logstyle="no-colon" || exit $?
            log "[INFO]preinstall_check success"
        fi

        if [ ! -z "$Firmware_Install_Path_Param" ] && [ -f "$Firmware_Install_Path_Param"/firmware/version.info ]; then
            req_ver_info="$Firmware_Install_Path_Param"/firmware/version.info
        else
            log "[WARNING]old firmware not installed, stop version relationship check"
            return 0
        fi
    else
        if [ "${feature_ver_compatible_check}" = "y" ]; then
            Driver_Install_Path_Param="/usr/local/Ascend"
            preinstall_check --install-path="${Driver_Install_Path_Param}" --script-dir="${SHELL_DIR}" --package="driver" --logfile="${logFile}" --logstyle="no-colon" || exit $?
            log "[INFO]preinstall_check success"
        fi

        log "[WARNING]not exist $installInfo, stop version relationship check"
        return 0
    fi

    if [ -f ./version.info ];then
        ver_info="$PWD"/version.info
    else
        ver_info="$Driver_Install_Path_Param"/driver/version.info
    fi
    req_pkg_name=firmware
    check_pkg_ver_deps "$ver_info" $req_pkg_name $req_ver_info
    ret=$VerCheckStatus
    if [ "$ret"x = "SUCC"x ];then
        log "[INFO]driver and firmware version relationship check success"
        drvEcho "[INFO]driver and firmware version relationship check success"
        return 0
    else
        log "[WARNING]driver and firmware version relationship check fail"
        drvEcho "[WARNING]driver and firmware version relationship check fail"
        if [ "$check"x = "y"x ];then
            log "[INFO]commandline: --check, not install"
            return 0
        fi
        if [ "$quiet"x = "n"x ]; then
                echo "[WARNING]driver required firmware version not match ,do you want to continue?  [y/n] "
                while true
                do
                    read yn
                    if [ "$yn" = n ]; then
                        echo "Stop installation!"
                        exit 1
                    elif [ "$yn" = y ]; then
                        break;
                    else
                        echo "ERR_NO:0x0002;ERR_DES:input error, please input again!"
                    fi
                done
        fi
    fi
    return 0
}

check=$1
quiet=$2
isRoot
createFolder
changeLogMode

versionCheck
exit 0
