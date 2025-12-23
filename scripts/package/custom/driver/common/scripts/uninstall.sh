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
runLogFile="${ASCEND_SECLOG}/ascend_run_servers.log"
ASCEND_DRIVER_SETUP_SCRIPT="ascend_driver_config.sh"
KMSAGENT_CONFIG_ITEM="kmsagent"
hotreset_status_file="/var/log/hotreset_status.log"
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
PACKAGE_TYPE="run"
if [ "${PACKAGE_TYPE}" = "run" ];then
    sourcedir="$PWD"/driver
else 
    sourcedir="$SHELL_DIR"/..
fi
LOG_LEVEL_MAJOR="MAJOR"
OPERATION_LOGPATH="${ASCEND_SECLOG}/operation.log"
LOG_OPERATION_UNINSTALL="Uninstall"
LOG_RESULT_SUCCESS="success"
LOG_RESULT_FAILED="failed"

COMMON_SHELL="${SHELL_DIR}/common.sh"
source "${COMMON_SHELL}"

Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    user_id=`id | awk '{printf $1}'`
    echo "[Driver] [$cur_date] [$user_id] "$1 >> $logFile
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

if [ -f /usr/local/Ascend/Atlas_rpm_driver_tag.flag ]; then
    drvColorEcho "[INFO]\033[32mRPM driver package installed successfully!\033[0m"
    chattr -i /usr/local/Ascend/Atlas_rpm_driver_tag.flag
    rm -f /usr/local/Ascend/Atlas_rpm_driver_tag.flag
    exit 0
fi
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
        if [ "${PACKAGE_TYPE}" != "deb" ];then
            rm -f "$hotreset_status_file"
        fi
    else
        hotreset_status="unknown"
    fi
    if [ "$hotreset_status"x = "scan_success"x ] || [ "$Driver_Install_Type"x = "docker"x ] || [ "$Driver_Install_Type"x = "devel"x ]; then
        drvColorEcho  "[INFO]\033[32mDriver package uninstalled successfully! Uninstallation takes effect immediately. \033[0m"
    else
        if [ "$hotreset_status"x = "ko_abort"x ]; then
            drvColorEcho "[INFO]\033[32mDriver package uninstalled finished! \033[0m"
            drvColorEcho "[WARNING]\033[33mKernel modules can not be removed, reboot needed for uninstallation to take effect! \033[0m"
        else
            drvColorEcho "[INFO]\033[32mDriver package uninstalled successfully! \033[0m\033[31mReboot needed for uninstallation to take effect! \033[0m"
        fi
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

exitInstallInfo() {
    drvColorEcho "[ERROR]\033[31mThe davinci nodes are occupied by some processes, please stop processes and install or uninstall again, details in : $logFile \033[0m"
    log "[ERROR]The davinci nodes are occupied by some processes, please stop processes and install or uninstall again."
    drvColorEcho "[INFO]If you want to install or uninstall the driver forcibly, add the force parameter. For details, see [--help]."
    logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
    exitLog 1
}

livepatch_audit_log_record() {
    local newest_livepatch_version="$1" return_code=$2
    local log_opt="Uninstall" log_level="MAJOR" result="" livepatch_name="Ascend-driver_sph-${newest_livepatch_version}"
    local user_name="root" cur_ip="127.0.0.1" opt_file="${ASCEND_SECLOG}/operation.log" cmdlist="none"

    if [ ! -e "${opt_file}" ]; then
        touch "${opt_file}"
        chmod -f 400 "${opt_file}"
    fi

    if [ ${return_code} -eq 0 ]; then
        result="success"
    else
        result="failed"
    fi

    echo "${log_opt} ${log_level} ${user_name} ${g_start_time} ${cur_ip} ${livepatch_name} ${result} cmdlist=${cmdlist}" >> ${opt_file}
    return 0
}

uninstall_livepatch() {
    local uninstall_type=$1
    local drv_root_dir="${Driver_Install_Path_Param}"
    local livepatch_install_conf="${drv_root_dir}/driver/livepatch/livepatch_install.info"
    local newest_patch_version="" newest_patch_path="" uninstall_sh=""

    newest_patch_version="$(grep -v "^$" "${livepatch_install_conf}" 2>/dev/null | sed -n '$p')"
    if [ -z "${newest_patch_version}" ]; then
        log "[INFO]newest livepatch is NULL and skip here."
        return 0
    fi

    if [ ! -d "${drv_root_dir}/driver/livepatch/${newest_patch_version}" ]; then
        log "[INFO]The livepatch dir doesn't exist and skip here."
        log "[INFO]To remove [${drv_root_dir}/driver/livepatch]."
        rm -rf "${drv_root_dir}/driver/livepatch"
        return 0
    fi

    newest_patch_path="${drv_root_dir}/driver/livepatch/${newest_patch_version}"
    uninstall_sh="${drv_root_dir}/driver/livepatch/${newest_patch_version}/script/run_livepatch_uninstall.sh"

    "${uninstall_sh}" "${uninstall_type}" "y" "${newest_patch_path}" 999 && ret=$? || ret=$?
    if (( ret != 0 )); then
        log "[ERROR]The livepatch's ${uninstall_type}-uninstall failed."
    fi
    livepatch_audit_log_record "${newest_patch_version}" ${ret}

    return 1
}

# Function to check boot status using upgrade-tool
check_boot_status() {
    local docker_type=$1
    local patch_use_status=$2
    # set hot reset flag
    local phyflag=""
    if [ "$force" = n ]; then
        if [ -f "$Driver_Install_Path_Param"/driver/tools/upgrade-tool ]; then
            systemd-detect-virt -v | grep -E "kvm|vmware|qemu|xen" >> /dev/null 2>&1
            phyflag=$?
            timeout 20s "$Driver_Install_Path_Param"/driver/tools/upgrade-tool --device_index -1 --boot_status 2>> /dev/null | grep -v fail | grep "boot status" | grep -v "boot status:0" >> /dev/null 2>&1
            if [ $? -eq 0 ]; then
                if [ "$phyflag"x = "0"x ]; then
                    log "[INFO]VM scene, continue to uninstall the old software package."
                else
                    if [ "$docker_type"x = "docker"x ]; then
                        if checkUserDocker; then
                           log "[INFO]Check docker process  is over, continue to uninstall the old software package."
                        else
                           exitInstallInfo
                        fi
                    else
                        if checkUserDocker && setHotResetFlag; then
                            log "[INFO]Check docker process and set hot reset flag is over, continue to uninstall the old software package."
                        else
                            if (( patch_use_status != 0 )); then
                                 error "ERR_NO:0x0080;ERR_DES: Uninstallation and recovery failed, so reboot is recommended."
                            else
                                 exitInstallInfo
                            fi
                        fi
                    fi
                fi
            else
                log "[WARNING]The chip can not set the hot reset flag."
            fi
        fi
    fi
}


uninstallRun(){
    if [ -f "$Driver_Install_Path_Param"/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} ]; then
        "$Driver_Install_Path_Param"/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} $KMSAGENT_CONFIG_ITEM stop >> $runLogFile
    fi
    if [ "${PACKAGE_TYPE}" != "rpm" ];then
        # uninstall livepatch
        g_start_time="$(date +"%Y-%m-%d %H:%M:%S")"

        check_boot_status "docker" 0

        local patch_use_status=0
        uninstall_livepatch "pre"
        patch_use_status=$?
        # set hot reset flag
        check_boot_status "all" ${patch_use_status}

        # remove livepatch directory
        uninstall_livepatch "post"
    else
        local phyflag=""
        if [ "$force" = n ]; then
            if [ -f "$Driver_Install_Path_Param"/driver/tools/upgrade-tool ]; then
                systemd-detect-virt -v | grep -E "kvm|vmware|qemu|xen" >> /dev/null 2>&1
                phyflag=$?
                timeout 20s "$Driver_Install_Path_Param"/driver/tools/upgrade-tool --device_index -1 --boot_status 2>> /dev/null | grep -v fail | grep "boot status" | grep -v "boot status:0" >> /dev/null 2>&1
                if [ $? -eq 0 ]; then
                    if [ "$phyflag"x = "0"x ]; then
                        log "[INFO]VM scene, continue to uninstall the old software package."
                    else
                        if checkUserDocker && setHotResetFlag; then
                            log "[INFO]Check docker process and set hot reset flag is over, continue to uninstall the old software package."
                        else
                            exitInstallInfo
                        fi
                    fi
                else
                    log "[WARNING]The chip can not set the hot reset flag."
                fi
            fi
        fi
    fi
    # remove version.info
    if [ -f "$uninstallPath"/driver/version.info ];then
        chattr -i "$uninstallPath"/driver/version.info > /dev/null 2>&1
        if [ "${PACKAGE_TYPE}" != "rpm" ];then
            rm -rf "$uninstallPath"/driver/version.info
        fi
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
        "$uninstallPath"/driver/script/run_driver_uninstall.sh --uninstall "$uninstallPath" $force
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
force=n
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
    --force)
        force=y
        shift
        ;;
    *)
        if [ ! "x$1" = "x" ]; then
            error "ERR_NO:0x0004;ERR_DES: Unrecognized parameters: $1. Only support '--quiet' and '--force'."
        fi
        break
        ;;
    esac
done
trap "logOperation \"${start_time}\" \"${LOG_RESULT_FAILED}\";exit 1" INT QUIT TERM
uninstallRun
exit 0