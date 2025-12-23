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
hotreset_status_file="/var/log/hotreset_status.log"

LOG_LEVEL_MAJOR="MAJOR"
OPERATION_LOGPATH="${ASCEND_SECLOG}/operation.log"
LOG_OPERATION_UNINSTALL="Uninstall"
LOG_RESULT_SUCCESS="success"
LOG_RESULT_FAILED="failed"
SHELL_DIR=$(cd "$(dirname "$0")" || exit;pwd)
COMMON_SHELL="${SHELL_DIR}/common.sh"
source "${COMMON_SHELL}"

Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1" >> $logFile
    return 0
}
drvEcho() {
     cur_date=`date +"%Y-%m-%d %H:%M:%S"`
     echo  "[Driver] [$cur_date] $1"
}
drvColorEcho() {
     cur_date=`date +"%Y-%m-%d %H:%M:%S"`
     echo -e  "[Driver] [$cur_date] $1"
}

error() {
    local msg="$1"
    drvEcho "[ERROR]${msg}"
    log "[ERROR]${msg}"
    logOperation "${start_time}" "${LOG_RESULT_FAILED}"
    exitLog 1
}

startLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    drvEcho "[INFO]Start time: $cur_date" >> $logFile
}

exitLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    drvEcho "[INFO]End time: ${cur_date}" >> $logFile
    exit $1
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

logBaseVersion() {
    if [ -f "$uninstallPath"/driver/version.info ];then
        installed_version=`cat "$uninstallPath"/driver/version.info | awk -F '=' '$1=="Version" {print $2}'`
        if [ ! "${installed_version}"x = ""x ]; then
            drvEcho "[INFO]base version is ${installed_version}."
            log "[INFO]base version is ${installed_version}."
            return 0
        fi
    fi
    log "[INFO]base version was destroyed or not exist."
}

uninstallationCompletionMessage() {
        if [ -e "$hotreset_status_file" ]; then
            hotreset_status=`cat "$hotreset_status_file"`
            hotreset_status=${hotreset_status%%.*}
            rm -f "$hotreset_status_file"
        else
            hotreset_status="unknown"
        fi
        if [ "$hotreset_status" = "scan_success" ] || [ $Driver_Install_Type = "docker" ] ||  [ $Driver_Install_Type = "devel" ]; then
            drvColorEcho "[INFO]\033[32mDriver package uninstalled successfully! Uninstallation takes effect immediately. \033[0m"
        else
            drvColorEcho "[INFO]\033[32mDriver package uninstalled successfully! \033[0m\033[31mReboot needed for uninstallation to take effect! \033[0m"
        fi
}

logOperation() {
    local operation=${LOG_OPERATION_UNINSTALL}
    local start_time="$1"
    local runfilename="Ascend-driver-"$installed_version
    local result="$2"
    local cmdlist="none"
    local level="${LOG_LEVEL_MAJOR}"

    if [ ! -f "${OPERATION_LOGPATH}" ]; then
        touch ${OPERATION_LOGPATH}
        chmod 640 ${OPERATION_LOGPATH}
    fi

    if [ $quiet = y ]; then
        cmdlist="--quiet"
    fi

    echo "${operation} ${level} root ${start_time} 127.0.0.1 ${runfilename} ${result}"\
    "cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}

}

uninstallRun(){
    # remove version.info
    if [ -f "$uninstallPath"/driver/version.info ];then
        if [ "$Driver_Install_Type" != "docker" ];then
            chattr -i "$uninstallPath"/driver/version.info > /dev/null 2>&1
        fi
        rm -rf "$uninstallPath"/driver/version.info
        log "[INFO]rm -rf version.info success"
    fi
    uninstall_result=n
    if [ -f "$uninstallPath"/host_sys_stop.sh ]; then
        "$uninstallPath"/host_sys_stop.sh
    fi

    if [ -f "$uninstallPath"/host_servers_remove.sh ]; then
        cd "$uninstallPath"
        "$uninstallPath"/host_servers_remove.sh
        cd - > /dev/null 2>&1
    fi

    if [ -f "$uninstallPath"/driver/script/run_driver_uninstall.sh ]; then
        "$uninstallPath"/driver/script/run_driver_uninstall.sh --uninstall "$uninstallPath" "Uninstall"
        if [ $? -eq 0 ];then
            uninstall_result=y
        fi
    else
        error "ERR_NO:0x0080;ERR_DES:run_driver_uninstall.sh does not existed, uninstall driver failed"
    fi

    if [ $uninstall_result = n ]; then
        drvEcho "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed, details in : ${ASCEND_SECLOG}/ascend_install.log"
        log "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed"
        logOperation "${start_time}" "${LOG_RESULT_FAILED}"
        exitLog 1
    fi

    if [ $uninstall_result = y ]; then
        log "[INFO]uninstall driver success"
        if [ -f $installInfo ]; then
            # when the installation path is empty, delete it, make sure the directory exists before using ls
            if [ -d "${uninstallPath}" ] && [ `ls "${uninstallPath}" | wc -l` -eq 0 ];then
               rm -rf "${uninstallPath}"
            fi
            sed -i '/Driver_Install_Path_Param=/d' $installInfo
            sed -i '/Driver_Install_Type=/d' $installInfo
            sed -i '/Driver_Install_Mode=/d' $installInfo
            sed -i '/Driver_Install_For_All=/d' ${installInfo}
            if [ ` grep -c -i "Install_Path_Param" $installInfo ` -eq 0 ]; then
                rm -f $installInfo
            fi
        fi
        uninstallationCompletionMessage
        logOperation "${start_time}" "${LOG_RESULT_SUCCESS}"
        exitLog 0
    fi
    return 0
}

start_time=$(date +"%Y-%m-%d %H:%M:%S")
shellPath=$(cd "$(dirname "$0")";pwd)
uninstallPath=$(cd "$shellPath"/../../;pwd)
installed_version="none"
quiet=n
isRoot
createFolder
changeLogMode
logBaseVersion
startLog

while true
do
    case "$1" in
    --quiet)
        quiet=y
        shift
        ;;
    *)
        if [ ! "x$1" = "x" ]; then
            error "ERR_NO:0x0004;ERR_DES: Unrecognized parameters: $1. Only support '--quiet'."
        fi
        break
        ;;
    esac
done
trap "logOperation \"${start_time}\" \"${LOG_RESULT_FAILED}\";exit 1" INT QUIT TERM 
uninstallRun
exit 0
