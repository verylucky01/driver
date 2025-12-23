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
installInfo="/etc/ascend_install.info"
username=HwHiAiUser
usergroup=HwHiAiUser
hotreset_status_file="/var/log/hotreset_status.log"
driverCrlStatusFile="/root/ascend_check/driver_crl_status_tmp"

# load specific_func.inc
source "${PWD}"/driver/script/specific_func.inc

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

export feature_hot_reset=n
export feature_crl_check=n
export feature_nvcnt_compile=n
export feature_dkms_compile=n
export input_install_for_all=n
export feature_virt_scene=n
export feature_no_device_kernel=n
export feature_ver_compatible_check=n
export feature_device_exist_check=n
export feature_flash_version_check=n

export native_pkcs_conf

log() {
   cur_date=`date +"%Y-%m-%d %H:%M:%S"`
   echo "[Driver] [$cur_date] $1" >> $logFile
}
drvEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}
drvColorEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo -e  "[Driver] [$cur_date] $1"

}
get_feature_config() {
    local config

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_HOT_RESET)
    log "[INFO]FEATURE_HOT_RESET is : $config"
    if [ "$config"x = "FEATURE_HOT_RESET=y"x ];then
        feature_hot_reset=y
        log "[INFO]set feature_hot_reset=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_CRL_CHECK)
    log "[INFO]FEATURE_CRL_CHECK is : $config"
    if [ "$config"x = "FEATURE_CRL_CHECK=y"x ];then
        feature_crl_check=y
        log "[INFO]set feature_crl_check=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_NVCNT_CHECK)
    log "[INFO]FEATURE_NVCNT_CHECK is : $config"
    if [ "$config"x = "FEATURE_NVCNT_CHECK=y"x ];then
        feature_nvcnt_check=y
        log "[INFO]set feature_nvcnt_check=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_DKMS_COMPILE)
    log "[INFO]FEATURE_DKMS_COMPILE is : $config"
    if [ "$config"x = "FEATURE_DKMS_COMPILE=y"x ];then
        feature_dkms_compile=y
        log "[INFO]set feature_dkms_compile=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_VIRT_SCENE)
    log "[INFO]FEATURE_VIRT_SCENE is : $config"
    if [ "$config"x = "FEATURE_VIRT_SCENE=y"x ];then
        feature_virt_scene=y
        log "[INFO]set feature_virt_scene=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_NO_DEVICE_KERNEL)
    log "[INFO]FEATURE_NO_DEVICE_KERNEL is : $config"
    if [ "$config"x = "FEATURE_NO_DEVICE_KERNEL=y"x ];then
        feature_no_device_kernel=y
        log "[INFO]set feature_no_device_kernel=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_VER_COMPATIBLE_CHECK)
    log "[INFO]FEATURE_VER_COMPATIBLE_CHECK is : $config"
    if [ "$config"x = "FEATURE_VER_COMPATIBLE_CHECK=y"x ];then
        feature_ver_compatible_check=y
        log "[INFO]set feature_ver_compatible_check=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_DEVICE_EXIST_CHECK)
    log "[INFO]FEATURE_DEVICE_EXIST_CHECK is : $config"
    if [ "$config"x = "FEATURE_DEVICE_EXIST_CHECK=y"x ];then
        feature_device_exist_check=y
        log "[INFO]set feature_device_exist_check=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_DEVICE_FLASH_VERSION_CHECK)
    log "[INFO]FEATURE_DEVICE_FLASH_VERSION_CHECK is : $config"
    if [ "$config"x = "FEATURE_DEVICE_FLASH_VERSION_CHECK=y"x ];then
        feature_flash_version_check=y
        log "[INFO]set FEATURE_DEVICE_FLASH_VERSION_CHECK=y"
    fi
    return 0
}

exitLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    drvEcho "[INFO]End time: ${cur_date}"
    drvEcho "[INFO]End time: ${cur_date}" >> $logFile
    exit $1
}

errorUsage() {
    if [ $# -eq 1 ]; then
        drvEcho "[ERROR]ERR_NO:0x0004;ERR_DES: Unrecognized parameters: ${1}. Try './xxx.run --help' for more information."
        exitLog 1
    elif [ $# -eq 2 ]; then
        log "$2"
        drvEcho "$2"
        exitLog "$1"
    else
        drvEcho "[ERROR]ERR_NO:0x0004;ERR_DES: Unrecognized parameters. Try './xxx.run --help' for more information."
        exit 1
    fi
}
# LogFile polling and backup
rotateLog() {
    echo "${logFile} {
        su root root
        daily
        size=5M
        rotate 3
        missingok
        create 440 root root
    }" > /etc/logrotate.d/ascend-install
}

startLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    drvEcho "[INFO]Start time: $cur_date"
    drvEcho "[INFO]Start time: $cur_date" >> $logFile
}

installationCompletionMessage() {
    local run_install_type="$1"
    local new_path

    new_path="${Install_Path_Param%/}"
    if [ -e "$hotreset_status_file" ]; then
        hotreset_status=`cat "$hotreset_status_file"`
        rm -f "$hotreset_status_file"
    else
        hotreset_status="unknown"
    fi

    # Convert it to lowercase.
    run_install_type="${run_install_type,,}"
    # delete 'e' from install type.
    run_install_type="${run_install_type/e/}ed"

    if [ "$hotreset_status"x = "success"x ] || [ "$installType"x = "docker"x ] || [ "$installType"x = "devel"x ] ; then
        drvColorEcho  "[INFO]\033[32mDriver package ${run_install_type} successfully! The new version takes effect immediately. \033[0m"
        if [ "$installType"x = "docker"x ];then
            # lib-put-path is different from the others.
            if [ "${lib_put_path}"x = "specific"x ]; then
                echo -e "Please make sure that\n    - LD_LIBRARY_PATH includes $new_path/driver/lib64\n"\
                        "   - Please refer to the instruction manual for specific details"
            else
                echo -e "Please make sure that\n    - LD_LIBRARY_PATH includes $new_path/driver/lib64/common:$new_path/driver/lib64/driver\n"\
                        "   - Please refer to the instruction manual for specific details"
            fi
        fi
    else
        drvColorEcho  "[INFO]\033[32mDriver package ${run_install_type} successfully! \033[0m\033[31mReboot needed for installation/upgrade to take effect! \033[0m"
    fi
}

uninstallationCompletionMessage() {
        if [ -e "$hotreset_status_file" ]; then
            hotreset_status=`cat "$hotreset_status_file"`
            hotreset_status=${hotreset_status%%.*}
            rm -f "$hotreset_status_file"
        else
            hotreset_status="unknown"
        fi
        if [ "$hotreset_status"x = "scan_success"x ] || [ "$Driver_Install_Type"x = "docker"x ] || [ "$Driver_Install_Type"x = "devel"x ]; then
            drvColorEcho  "[INFO]\033[32mDriver package uninstalled successfully! Uninstallation takes effect immediately. \033[0m"
        else
            drvColorEcho  "[INFO]\033[32mDriver package uninstalled successfully! \033[0m\033[31mReboot needed for uninstallation to take effect! \033[0m"
        fi
}
# check whether hotreset fails
isHotresetFailed(){
    if [ "${hotreset_status}" = "scan_success"* ] || [ "${hotreset_status}" = "success" ]; then
        echo "false"
    else
        echo "true"
    fi
}
# support absolute path and relative path
getAbsolutePath(){

    local current_path="${PWD}" parent_dir=""
    flag1=$(echo "${Install_Path_Param:0:3}" |grep "\./")''$(echo "$Install_Path_Param" |grep "~/")''$(echo "$Install_Path_Param" |grep "/\.")
    flag2=$(echo "${Install_Path_Param:0:1}" |grep "/")

    if [ -z $flag1  ] && [  -z $flag2  ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:install-path input error, install failed"
    fi
    if [ ! -z $flag1 ];then
        cd "$runPackagePath" >& /dev/null
        log "[INFO]enter runPackagePath:$runPackagePath"

        eval "cd ${Install_Path_Param}" >& /dev/null
        if [ $? -ne 0 ]; then
            # get parent directory
            parent_dir=`dirname "${Install_Path_Param}"`
            log "[INFO]parent_dir value is [${parent_dir}]"

            eval "cd ${parent_dir}" >& /dev/null
            if [ $? -eq 0 ]; then
                cd - >& /dev/null
                eval "mkdir ${Install_Path_Param}" >& /dev/null
                if [ $? -ne 0 ]; then
                    log "[ERROR][${parent_dir}] is invalid."
                    drvEcho "[ERROR]Relative path $Install_Path_Param doesn't exist, please input a valid path"
                    exitLog 1
                else
                    log "[INFO]create dir [${Install_Path_Param}] success."
                    setFileChmod -f 755 ${Install_Path_Param}
                    eval "cd ${Install_Path_Param}" >& /dev/null
                fi
            else
                log "[ERROR][${parent_dir}] is invalid."
                drvEcho "[ERROR]Relative path $Install_Path_Param doesn't exist, please input a valid path"
                exitLog 1
            fi
        fi

        log "[INFO]enter Install_Path_Param=$Install_Path_Param, pwd: $PWD "
        Install_Path_Param="${PWD}"
        cd "${current_path}" >& /dev/null

        log "[INFO]current path: $PWD"
    fi
    log "[INFO]Install_Path_Param=$Install_Path_Param"
}

# check if the installation path is valid
isValidPath() {
    [ $uninstall = y ] && return 0
    makeDefaultPath=n
    if [ "$input_install_path" = "" ]; then
        if [ -f $installInfo ]; then
            Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
            if [ ! -z "$Driver_Install_Path_Param" ]; then
                Install_Path_Param="$Driver_Install_Path_Param"
            fi
        fi
    else
        Install_Path_Param="${input_install_path}"
    fi

    getAbsolutePath
    if [ ! -d "$Install_Path_Param" ]; then
        if [ "$Install_Path_Param" = "/usr/local/Ascend" ]; then
            mkdir -p "$Install_Path_Param"
            setFileChmod -f 755 "$Install_Path_Param"
            makeDefaultPath=y
        else
            parentPath=`dirname "$Install_Path_Param"`
            if [ ! -d "$parentPath" ];then
                errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:install path $parentPath not exists,runpackage only support create one level of directory,need create $parentPath."
            fi
            su - "$username" -c "cd $parentPath" >> /dev/null 2>&1
            if [ $? -ne 0 ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:The $username do not have the permission to access $parentPath, please reset the directory to a right permission."
            fi
            mkdir -p "$Install_Path_Param"
            setFileChmod -f 755 "$Install_Path_Param"
            makeDefaultPath=y
        fi
    fi

    su - "$username" -c "cd $Install_Path_Param" >> /dev/null 2>&1
    if [ $? -ne 0 ]; then
        if [ $makeDefaultPath = y ]; then
            rm -rf "$Install_Path_Param"
        fi
        errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:The $username do not have the permission to access $Install_Path_Param, please reset the directory to a right permission."
    fi
}

check_boot_server_path() {
    Install_Path_Param="${input_install_path}"

    #1. if input_install_path is relative path: will use or creat the path at runPackage dir(getAbsolutePath)
    getAbsolutePath

    #2. if input_install_path is absolute path: will use or creat the absolute path
    if [ ! -d "$Install_Path_Param" ]; then
        parentPath=`dirname "$Install_Path_Param"`
        if [ ! -d "$parentPath" ];then
            errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:install path $parentPath not exists,runpackage only support create one level of directory,need create $parentPath."
        fi

        mkdir -p "$Install_Path_Param"
        setFileChmod -f 755 "$Install_Path_Param"
    fi
}

check_device_image_crl() {
    if [ $feature_crl_check = y ]; then
        local ret=0
        . ./driver/script/device_crl_check.sh
        device_images_crl_check && ret=$? || ret=$?
        if [ ${ret} -ne 0 ];then
            if [ ${ret} -eq 2 ]; then
                drvColorEcho "[ERROR]\033[31mThe software signature verification failed because the signature mode used by the software is inconsistent with the current configuration. Currently configured is [${native_pkcs_conf}], details in : $logFile \033[0m"
            else
                drvColorEcho "[ERROR]\033[31mDevice_images_crl_check failed, details in : $logFile \033[0m"
            fi
            log "[ERROR]new device crl check failed, stop upgrade"
            rm -f $driverCrlStatusFile
            exitLog 1
        fi
    fi
}

# check if there are multiple run processes in execution
checkProcess(){
    name=`ps -ef | awk '$2=='$$'{print $10}'|rev|cut -d "/" -f1|rev`
    checkname=`echo "$name" | awk -F "-" '{print $1"-"$2}'`
    shellname=`echo $0 |rev |cut -d "/" -f1 |rev`
    process=`ps -ef | grep -v "grep" | grep -w "$shellname" |grep -w "${checkname}-.*.run"`
    pid=`echo "$process" | awk -F ' ' '{print $2}'`
    ret=`echo "$process" | awk -F ' ' '{print $3}' | grep -v "$pid" | wc -l`
    if [ $ret -gt 1 ]; then
        log "[INFO]$name;$shellname;$ret;$process"
        errorUsage 1 "[ERROR]ERR_NO:0x0094;ERR_DES:There is already a process being executed,please do not execute multiple tasks at the same time"
    fi
}

# check the disk space of the installation directory before installation or upgrade，driver need 300M，firmware need 32M.
checkFreeSpace(){
    [ $uninstall = y ] && return 0
    freeSpace=`df -BM "$Install_Path_Param" |awk '/[0-9]%/{print $(NF-2)}'`
    log "[INFO]freeSpace:${freeSpace}"
    if [ ${freeSpace%?} -le $occupy_space ];then
        errorUsage 1 "[ERROR]free space only ${freeSpace}, cannot install driver package (greater than ${occupy_space}M), please make sure enough space under the installation path"
    fi
}
# check if the user exists
checkUser(){
    ret=`cat /etc/passwd | cut -f1 -d':' | grep -w "$1" -c`
    if [ $ret -le 0 ]; then
        return 1
    else
        return 0
    fi
}

isExistsGroup(){
    local ret=`cat /etc/group | cut -f1 -d':' | grep -w "$1" -c`
    if [ $ret -le 0 ]; then
        return 1
    else
        return 0
    fi
}

# check the association between users and user groups
checkGroup(){
    result=$(groups "$2" | grep ":")
    if [ "${result}X" != "X" ]; then
        group_user_related=`groups "$2"|awk -F":" '{print $2}'|grep -w "$1"`
    else
        group_user_related=`groups "$2"|grep -w "$1"`
    fi
    if [ "${group_user_related}x" != "x" ];then
        return 0
    else
        return 1
    fi
}

# check installation conditions
checkUserAndGroup() {
    [ $uninstall = y ] && return 0
    checkUser "${username}"
    if [ $? -ne 0 ];then
        errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:${username} not exists! Please add ${username}"
    fi
    isExistsGroup "${usergroup}"
    if [ $? -ne 0 ];then
        errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:${usergroup} group not exists! Please add ${usergroup} group"
    fi

    checkGroup "${usergroup}" "${username}"
    if [ $? -ne 0 ];then
        errorUsage 1 "[ERROR]ERR_NO:0x0096;ERR_DES:${usergroup} not right! Please check the relatianship of ${username} and ${usergroup}"
    fi
    drvEcho "[INFO]set username and usergroup, ${username}:${usergroup}"

}

getUserInfo() {
    local old_install_for_all=""

    if [ -f ${installInfo} ]; then
        UserName=$(getInstallParam "UserName" "${installInfo}")
        UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
        if [ ${upgrade} = "y" ] && [ "$input_install_for_all" = "n" ]; then
            old_install_for_all=$(getInstallParam "Driver_Install_For_All" "${installInfo}")
            if [ ! -z "${old_install_for_all}" ]; then
                install_for_all="${old_install_for_all}"
            fi
        fi
        if [ ${input_install_username} = y ] || [ ${input_install_usergroup} = y ]; then
            if [ "x${UserName}" = "x" ]; then
                sub_file=`echo $Install_Path_Param |awk -F"/" '{print $NF}'`
                username_cur=`ls -l "${Install_Path_Param}/../" |grep -w "${sub_file}" |awk '{print $3}'`
                if [ "x${username}" != "x${username_cur}" ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                fi
            else
                if [ "x${username}" != "x${UserName}" ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                else
                    username="${UserName}"
                fi
            fi

            if [ "x${UserGroup}" = "x" ]; then
                sub_file=`echo $Install_Path_Param |awk -F"/" '{print $NF}'`
                usergroup_cur=`ls -l "${Install_Path_Param}/../" |grep -w "${sub_file}" |awk '{print $4}'`
                if [ "x${usergroup}" != "x${usergroup_cur}" ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                fi
            else
                if [ "x${usergroup}" != "x${UserGroup}" ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                else
                    usergroup="${UserGroup}"
                fi
            fi
        fi
        if [ ${input_install_username} = n ] || [ ${input_install_usergroup} = n ]; then
            if [ "x${UserName}" != "x" ] && [ $uninstall = n ]; then
                if [ "${UserName}" = "root" ] && [ "${install_for_all}" = "no" ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:username not permission for root (except install-for-all scene), check /etc/ascend_install.info"
                else
                    username="${UserName}"
                fi
            fi
            if [ "x${UserGroup}" != "x" ] && [ $uninstall = n ]; then
                if [ "${UserGroup}" = "root" ] && [ "${install_for_all}" = "no" ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:usergroup not permission for root (except install-for-all scene), check /etc/ascend_install.info"
                else
                    usergroup="${UserGroup}"
                fi
            fi
        fi
    fi
}
# check if root permission
isRoot() {
    if [ $(id -u) -ne 0 ]; then
        drvEcho "[ERROR]ERR_NO:0x0093;ERR_DES:do not have root permission, operation failed, please use root permission!"
        exit 1
    fi
}

# get the version number under the installation path (version2)
getVersionInstalled() {
    version2="none"
    if [ -f "$1"/driver/version.info ]; then
        version2=`cat "$1"/driver/version.info | awk -F '=' '$1=="Version" {print $2}'`
    fi
    echo $version2
}
# get the version number in the run package (version1)
getVersionInRunFile() {
    version1="none"
    if [ -f ./version.info ]; then
        version1=`cat version.info | awk -F '=' '$1=="Version" {print $2}'`
    fi
    echo $version1
}

logBaseVersion() {
    if [ -f ${installInfo} ];then
        Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
        if [ ! -z "$Driver_Install_Path_Param" ]; then
            installed_version=$(getVersionInstalled "$Driver_Install_Path_Param")
            if [ ! "${installed_version}"x = ""x ]; then
                drvEcho "[INFO]base version is ${installed_version}."
                log "[INFO]base version is ${installed_version}."
                return 0
            fi
        fi
    fi
    log "[INFO]base version was destroyed or not exist."
}

# determine whether the command returns a failure, and report an error if it fails.
# $1:Operation type, $2:ret code,   $3:(Optional)print messages
error() {
    local operation="$1"
    local retcode="$2"
    local msg="$3"
    if [ ${retcode} -ne 0 ]; then
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
    fi

    if [ $# -eq 3 ]; then
        if [ ${retcode} -ne 0 ]; then
            drvEcho "[ERROR]${msg}"
            log "[ERROR]${msg}"
            exitLog 1
        fi
    elif [ $# -eq 2 ]; then
        if [ ${retcode} -ne 0 ]; then
            exitLog 1
        fi
    fi
}

# update /etc/ascend_install.info
updateInstallInfo() {
    local installMode_before=""

    if [ -f "$installInfo" ]; then
        UserName=$(getInstallParam "UserName" "${installInfo}")
        UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
        Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
        Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
        Driver_Install_For_All=$(getInstallParam "Driver_Install_For_All" "${installInfo}")
        if [ -z "$UserName" ]; then
            updateInstallParam "UserName" "$username" "$installInfo"
        fi
        if [ -z "$UserGroup" ]; then
            updateInstallParam "UserGroup" "$usergroup" "$installInfo"
        fi
        updateInstallParam "Driver_Install_Type" "$installType" "$installInfo"
        updateInstallParam "Driver_Install_Path_Param" "$Install_Path_Param" "$installInfo"
        updateInstallParam "Driver_Install_For_All" "$install_for_all" "$installInfo"
    else
        createFile "$installInfo" "root":"root" 644
        updateInstallParam "UserName" "$username" "$installInfo"
        updateInstallParam "UserGroup" "$usergroup" "$installInfo"
        updateInstallParam "Driver_Install_Type" "$installType" "$installInfo"
        updateInstallParam "Driver_Install_Path_Param" "$Install_Path_Param" "$installInfo"
        updateInstallParam "Driver_Install_For_All" "$install_for_all" "$installInfo"
    fi
    if [ "${upgrade}" = "y" ]; then
        installMode_before="$(getInstallParam "Driver_Install_Mode" "${installInfo}")"
        if [ -z "${installMode_before}" ]; then
            updateInstallParam "Driver_Install_Mode" "${installMode}" "${installInfo}"
        else
            updateInstallParam "Driver_Install_Mode" "${installMode_before}" "${installInfo}"
        fi
    else
        updateInstallParam "Driver_Install_Mode" "${installMode}" "${installInfo}"
    fi
    setFileChmod -f 644 $installInfo
}

parent_dirs_permision_check(){
    current_dir="$1" parent_dir="" short_install_dir=""
    local owner="" mod_num=""

    parent_dir=$(dirname "${current_dir}")
    short_install_dir=$(basename "${current_dir}")
    log "[INFO]parent_dir value is [${parent_dir}] and children_dir value is [${short_install_dir}]"

    if [ "${current_dir}"x = "/"x ]; then
        log "[INFO]parent_dirs_permision_check success"
        return 0
    else
        owner=$(stat -c %U "${parent_dir}"/"${short_install_dir}")
        if [ "${owner}" != "root" ]; then
            log "[WARNING][${short_install_dir}] permision isn't right, it should belong to root."
            return 1
        fi
        log "[INFO][${short_install_dir}] belongs to root."

        mod_num=$(stat -c %a "${parent_dir}"/"${short_install_dir}")
        if [ ${mod_num} -lt 755 ] && [ ${input_install_for_all} == n ]; then
            log "[WARNING][${short_install_dir}] permission is too small, it is recommended that the permission be 755 for the root user."
            return 2
        elif [ ${mod_num} -eq 755 ] && [ ${input_install_for_all} == n ]; then
            log "[INFO][${short_install_dir}] permission is ok."
        else
            log "[WARNING][${short_install_dir}] permission is too high, it is recommended that the permission be 755 for the root user."
        fi
        if [ ${mod_num} -lt 750 ] && [ ${input_install_for_all} == y ]; then
             log "[WARNING][${parent_dir}/${short_install_dir}] permission is too small."
             return 2
        fi

        parent_dirs_permision_check "${parent_dir}"
    fi
}

install_path_should_belong_to_root() {
    local ret=0

    # install drv first time
    if [ ${first_time_install_drv_flag} = "Y" ]; then
        # default dir doesn't exist or exists, but permission not correct
        if [ "${Install_Path_Param}" = "/usr/local/Ascend" ]; then
            mkdir -p "${Install_Path_Param}"
            setFileChmod -f 755 "${Install_Path_Param}"
            chown -f root:root "${Install_Path_Param}" >& /dev/null
        fi

        parent_dirs_permision_check "${Install_Path_Param}" && ret=$? || ret=$?
        if [ ${ret} -eq 1 ]; then
            log "[ERROR][${short_install_dir}] permision not right, it should belong to root."
            drvColorEcho  "[ERROR]\033[31mThe given directory, including its parents, should belong to root, details in : $logFile \033[0m"
            return 1
        elif [ ${ret} -eq 2 ]; then
            log "[ERROR][${short_install_dir}] permission is too small."
            drvColorEcho  "[ERROR]\033[31mThe given directory, or its parents, permission is too small, details in : $logFile \033[0m"
            return 1
        fi
    # it has already installed before.
    else
        # if upgrade and doesn't give install-path
        if [ "${old_drv_install_path}" = "${Install_Path_Param}" ]; then
            # if upgrade, default path
            if [ "${Install_Path_Param}" = "/usr/local/Ascend" ] || [ "${Install_Path_Param}" = "/usr/local/HiAI" ]; then
                setFileChmod -f 755 "${Install_Path_Param}"
                chown -f root:root "${Install_Path_Param}" >& /dev/null
            fi
            # dir permission check
            parent_dirs_permision_check "${Install_Path_Param}" && ret=$? || ret=$?
            # --quiet
            [ ${quiet} = y ] && [ ${ret} -ne 0 ] && return 0

            if [ ${ret} -ne 0 ]; then
                drvEcho "[WARNING]You are going to put run-files on a unsecure install-path, do you want to continue? [y/n]"
                readYesOrNoFromTerminal
            fi
        # upgrade and give install-path
        else
            parent_dirs_permision_check "${Install_Path_Param}" && ret=$? || ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR]the given dir, or its parents, permission is invalid."
                drvColorEcho " [ERROR]\033[31mThe given dir, or its parents, permission is invalid, details in : $logFile \033[0m"
                return 1
            fi
        fi
    fi

    return 0
}

# check whether any script size is zero or not.
check_local_file_size() {
    local filelist="${Driver_Install_Path_Param}"/driver/script/filelist.csv
    local install_type=""

    [ -e "${filelist}" ] && [ $(stat -c %s "${filelist}") -ne 0 ] || { log "[INFO]filelist.csv doesn't exist or its size is zero." && return 1; }

    # check whether any file size is zero, if yes, it will return 1 directly.
    cd "${Driver_Install_Path_Param}" >& /dev/null
    # the full install-type and the run install-type contain the same scripts.
    [ "${Driver_Install_Type}" = "full" ] && install_type="run" || install_type="${Driver_Install_Type}"
    if stat -c %s $(grep "\.sh" "${filelist}" | grep -w "${install_type}" | awk -F [,] '{print $4}') | grep -wq "0"; then
        log "[INFO]there is a local script with zero size."
        cd - >& /dev/null
        return 1
    fi
    cd - >& /dev/null

    log "[INFO]check_local_file_size success."
    return 0
}

installRun() {
    updateInstallInfo
    ./driver/script/run_driver_install.sh "$Install_Path_Param" $installType 
    driver_install_status=$?
    if [ $driver_install_status -eq 0 ];then
        installationCompletionMessage $1
    else
        drvColorEcho "[INFO]Failed to ${1,,} driver package, please retry after uninstall and reboot!"
    fi

}

uninstallRun() {
    local uninstall_para=$1

    operation="${LOG_OPERATION_UNINSTALL}"
    Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
    Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
    [ -f /etc/pss.cfg ] && native_pkcs_conf=$(cat /etc/pss.cfg)
    # not check CRL when in docker or devel mode
    if [ $uninstall = n ] && [ ! $Driver_Install_Type = "docker" ] && [ ! $Driver_Install_Type = "devel" ] && [ $feature_crl_check = y ]; then
        [ ${input_path_flag} = "y" ] && export NEW_Driver_Install_Path="${Install_Path_Param}"
        . ./driver/script/device_crl_check.sh
        #check CRL of images
        device_images_crl_check && ret=$? || ret=$?
        if [ ${ret} -ne 0 ];then
            if [ ${ret} -eq 2 ]; then
                drvColorEcho "[ERROR]\033[31mThe software signature verification failed because the signature mode used by the software is inconsistent with the current configuration. Currently configured is [${native_pkcs_conf}], details in : $logFile \033[0m"
            else
                drvColorEcho "[ERROR]\033[31mDevice_images_crl_check failed, details in : $logFile \033[0m"
            fi
            log "[ERROR]new device crl check failed, stop upgrade"
            rm -f $driverCrlStatusFile
            exitLog 1
        fi

        rm -f /etc/pss.cfg >& /dev/null
        log "[INFO]remove /etc/pss.cfg success"
    fi
    # remove version.info
    if [ -f "$Driver_Install_Path_Param"/driver/version.info ];then
        if [ ! "$Driver_Install_Type" = "docker" ]; then
            chattr -i "$Driver_Install_Path_Param"/driver/version.info > /dev/null 2>&1
        fi
        rm -rf "$Driver_Install_Path_Param"/driver/version.info > /dev/null 2>&1
        log "[INFO]rm -rf version.info success"
    fi
    if [ ! "$Driver_Install_Type" = "devel" ] && [ ! "$Driver_Install_Type" = "docker" ]; then
        # This is for compatibility with earlier versions.
        if [ -f "$Driver_Install_Path_Param"/host_sys_stop.sh ]; then
            "$Driver_Install_Path_Param"/host_sys_stop.sh
        fi
        if [ -f "$Driver_Install_Path_Param"/host_servers_remove.sh ]; then
            bash "$Driver_Install_Path_Param"/host_servers_remove.sh
        else
            ./host_servers_remove.sh
        fi
    fi

    # if new-install-path is the same as the old one, the original directory will not be deleted.
    [ "${uninstall}" = "n" ] && export NEW_Driver_Install_Path="${Install_Path_Param}"
    uninstall_result=n
    if check_local_file_size; then
        if [ -f "$Driver_Install_Path_Param"/driver/script/run_driver_uninstall.sh ]; then
            "$Driver_Install_Path_Param"/driver/script/run_driver_uninstall.sh --uninstall "$Driver_Install_Path_Param" "$uninstall_para" && uninstall_result=y
        fi
    fi
    if [ $uninstall_result = n ]; then
        ./driver/script/run_driver_uninstall.sh --uninstall "$Driver_Install_Path_Param" "$uninstall_para"
        if [ $? -eq 0 ]; then
            uninstall_result=y
        else
            log "[WARNING]uninstall driver failed"
        fi
    fi

    if [ $uninstall = y ] && [ $uninstall_result = n ]; then
        drvEcho "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed, details in : ${ASCEND_SECLOG}/ascend_install.log"
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
        log "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed"
        exitLog 1
    fi

    if [ $uninstall_result = y ]; then
        log "[INFO]uninstall driver success"
        if [ $uninstall = y ]; then
            # when the installation path is empty, delete it, make sure the directory exists before using ls
            if [ -d "${Driver_Install_Path_Param}" ] && [ `ls "${Driver_Install_Path_Param}" | wc -l` -eq 0 ];then
               rm -rf "${Driver_Install_Path_Param}"
            fi
            sed -i '/Driver_Install_Path_Param=/d' $installInfo
            sed -i '/Driver_Install_Type=/d' $installInfo
            sed -i '/Driver_Install_Mode=/d' ${installInfo}
            sed -i '/Driver_Install_For_All=/d' ${installInfo}
            if [ ` grep -c -i "Install_Path_Param" $installInfo ` -eq 0 ]; then
                rm -f $installInfo
            fi

            logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_SUCCESS}" "${installType}" "${all_parma}"
            uninstallationCompletionMessage
            exitLog 0
        fi
    fi
    unset "Driver_Install_Path_Param"

}

changeLogMode() {
    if [ ! -f $logFile ]; then
        touch $logFile
    fi
    setFileChmod -f 640 $logFile
}

logOperation() {
    local operation="$1"
    local start_time="$2"
    local runfilename="$3"
    local result="$4"
    local install_type="$5"
    local cmdlist="$6"
    local level

    if [ "${operation}" = "${LOG_OPERATION_INSTALL}" ]; then
        level="${LOG_LEVEL_SUGGESTION}"
    elif [ "${operation}" = "${LOG_OPERATION_UPGRADE}" ]; then
        level="${LOG_LEVEL_MINOR}"
    elif [ "${operation}" = "${LOG_OPERATION_UNINSTALL}" ]; then
        level="${LOG_LEVEL_MAJOR}"
    else
        level="${LOG_LEVEL_UNKNOWN}"
    fi

    if [ ! -d "${OPERATION_LOGDIR}" ]; then
        mkdir -p ${OPERATION_LOGDIR}
        setFileChmod -f 750 ${OPERATION_LOGDIR}
    fi

    if [ ! -f "${OPERATION_LOGPATH}" ]; then
        touch ${OPERATION_LOGPATH}
        setFileChmod -f 640 ${OPERATION_LOGPATH}
    fi

    if [ $upgrade = y ] || [ $uninstall = y ]; then
        echo "${operation} ${level} root ${start_time} 127.0.0.1 ${runfilename} ${result}"\
            "cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}
    else
        echo "${operation} ${level} root ${start_time} 127.0.0.1 ${runfilename} ${result}"\
            "install_type=${install_type}; cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}
    fi
}

uniqueMode() {
    if [ ! -z $installType ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:only support one type: full/run/docker/devel/boot-server, operation failed!"
    fi
}

unsupportParam() {
    # version/uninstall/check/boot-server-path can only be used independently.
    if [ "${version}" = "y" ] || [ "${uninstall}" = "y" ] || [ "${check}" = "y" ] || [ "${boot_server}" = "y" ]; then
        if [ $i -gt 1 ]; then
            errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:version/uninstall/check/boot-server can't use with other parameters."
        fi
        if [ "${input_install_for_all}" = "y" ] && [ "${boot_server}" = "y" ]; then
            errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:install-for-all param need used with full/run/upgrade/devel."
        fi
    else
        # if not input full/run/docker/upgrade/devel parameter, installation will stop.
        if [ $j -ne 1 ]; then
            errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:param need used with full/run/docker/upgrade/devel."
        elif [ $k -gt 1 ]; then
            errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:the number of vnpu parameters can be only one."
        else
            # if install for all, docker-install-type will not be allowed.
            if [ "${input_install_for_all}" = "y" ] && [ "${docker}" = "y" ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:install-for-all param need used with full/run/upgrade/devel."
            fi
            # if input install-path, upgrade-install-type will not be allowed.
            if [ "${input_path_flag}" = "y" ] && [ "${upgrade}" = "y" ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:install-path param need used with full/run/docker/devel."
            fi
            # chip features determine whether vnpu_host/vnpu_guest is supported or not.
            if [ "${feature_virt_scene}" = "n" ] && [ "${vnpu_host}" = "y" -o "${vnpu_guest}" = "y" -o "${vnpu_docker_host}" = "y" ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:vnpu_host/vnpu_guest/vnpu_docker_host is not supported here."
            fi
            # if does support, mode vnpu_host/vnpu_guest only full/run is allowed.
            if [ "${feature_virt_scene}" = "y" ] && [ "${vnpu_host}" = "y" -o "${vnpu_guest}" = "y" -o "${vnpu_docker_host}" = "y" ] && [ "${full_install}" = "n" -a "${run}" = "n" ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:vnpu_host/vnpu_guest/vnpu_docker_host param need used with full/run."
            fi
            # if the installation user is specified, upgrade-install-type will not be allowed.
            if [ "${input_install_username}" = "y" ] || [ "${input_install_usergroup}" = "y" ]; then
                if [ "${input_install_username}" = "y" ] && [ "${input_install_usergroup}" = "y" ]; then
                    if [ "$upgrade" = "y" ]; then
                        errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:install-username and install-usergroup params need used with full/run/docker/devel."
                    fi
                else
                    # need to specify user name and user group at the same time
                    drvEcho "[ERROR]ERR_NO:0x0004;ERR_DES:Username and usergroup are not specified at the same time."
                    drvEcho "[ERROR]Please input the right params and try again."
                    log "[ERROR]ERR_NO:0x0004;ERR_DES:Username and usergroup are not specified at the same time."
                    exitLog 1
                fi
            fi
        fi
    fi
}

crossVersion() {
    if [ -f /etc/HiAI_install.info ]; then
        . /etc/HiAI_install.info
        if [ -z $Install_Path_Param ]; then
            drvEcho "[ERROR]ERR_NO:0x0094;ERR_DES:Operation failed, An all-in-one RUN package is found, which needs to be uninstalled before proceeding."
        else
            drvEcho "[ERROR]ERR_NO:0x0094;ERR_DES:Operation failed, An all-in-one RUN package is found in $Install_Path_Param, which needs to be uninstalled before proceeding."
        fi
        exit
    fi
}

# read yes or no from terminal.
readYesOrNoFromTerminal() {
    while true
    do
        read yn
        if [ "$yn" = n ]; then
            drvEcho "[INFO]Stop installation!"
            exitLog 0;
        elif [ "$yn" = y ]; then
            break;
        else
            drvEcho "[ERROR]ERR_NO:0x0002;ERR_DES:input error, please input again!"
        fi
    done

    return 0
}

# Check whether the RUN package matches the environment or not.
checkRunPkgIsSuitableByPCI() {
    # If the feature is closed, it will skip.
    [ ${feature_device_exist_check} == "n" ] && return 0

    # To get the number of chips which are suitable for run-pkg.
    get_pci_info
    # If over 0, which means that it matches, it will skip here.
    [ ${g_dev_nums} -gt 0 ] && return 0

    log "[WARNING]There is no chip matching the driver package."
    drvColorEcho "[WARNING]\033[33mThere is no chip matching the driver package.\033[0m"

    # If --quiet, it will skip.
    [ ${quiet} = y ] && return 0

    drvEcho "[INFO]You are going to install a driver package which does not have a matching chip in the environment, do you want to continue? [y/n]"
    readYesOrNoFromTerminal

    return 0
}

is_flash_version_check() {
    local drv_root_path=$1
    local drv_upgrade_tool="${drv_root_path}/driver/tools/upgrade-tool"
    local image_patch="${drv_root_path}/driver/device"

    local first_time=y
    local package_version=-1
    local max_driver_version=-1
    local ret=0

    [ ${feature_flash_version_check} == "n" -o "$installType"x = "docker"x ] && return 0

    lsmod | grep -E "drv_pcie_host|drv_vpcie" >& /dev/null && first_time=n
    if [ "$first_time" = "y" ]; then
        log "[WARNING]The device hardware flash version cannot be obtained during the first installation."
        return 0
    fi

    if ! [ -d "${image_patch}" ]; then
        log "[WARNING]The driver image file directory does not exist."
        return 0
    fi

    which od >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        log "[WARNING]od is required to obtain data, please install."
        return 0
    fi

    package_version=$(od -x ${image_patch}/*_hbm.bin -j 0x2084 -N 4 | awk '{print $3}')
    package_version=$(printf "%d\n" 0x$package_version)

    max_driver_version=$(timeout 10s ${drv_upgrade_tool} --device_index -1 --hw_base_version 2>/dev/null \
        | grep -E '{"device_id"' | awk -F'"' '{print $5}' | awk -F'}|,|:' '{print $2}' | sort -nr | head -n1) \
        2>&1 && ret=$? || ret=$?

    if [ ${ret} -eq 124 ]; then
        log "[WARNING]'${drv_upgrade_tool} --device_index -1 --hw_base_version' command time(10 seconds) out"
        return 0
    fi

    if [ ${ret} -ne 0 ]; then
        log "[WARNING]The upgrade-tool cannot query the device hardware flash version."
        return 0
    fi

    if [ "$max_driver_version" == "crc_error" ]; then
        log "[WARNING]The crc check of the flash version is abnormal."
        return 0
    fi

    if [ "$max_driver_version" == "" ] || [ "$max_driver_version" == "-1" ]; then
        log "[WARNING]The device flash version could not be obtained."
        return 0
    fi

    if [ "${max_driver_version}" -gt "${package_version}" ]; then
        errorUsage 1 "[ERROR]Installation failed, the driver package version=${package_version} is earlier than the device flash hardware version=${max_driver_version}."
    fi

    log "[INFO]Flash version check success. package_flash_version=${package_version}, max_device_flash_version=${max_driver_version}"
    return 0
}

boot_server_file_copy() {
    local ret=0
    # 1. check the path is relative or absolute, not exist will be created
    # 2. check the exist path is root
    # 3. check the space is enough
    # 4. check CRL of images
    check_boot_server_path

    parent_dirs_permision_check "${Install_Path_Param}" && ret=$? || ret=$?
    if [ $ret -ne 0 ]; then
        drvColorEcho "[ERROR]\033[31mThe given dir, or its parents, permission is invalid, details in : $logFile \033[0m"
        exitLog 1
    fi

    checkFreeSpace

    check_device_image_crl

    for image in $image_bin
    do
        if [ ! -f "$sourcedir"/device/$image ]; then
            drvColorEcho "[ERROR]\033[31mload file($image) is missing, failed \033[0m"
            exitLog 1
        fi
    done

    for image in $image_bin
    do
        cp -f "$sourcedir"/device/$image "${Install_Path_Param}"
        setFileChmod -f 440 "${Install_Path_Param}"/$image
    done
}

crossVersion
isRoot

if [ ! -d "${ASCEND_SECLOG}" ]; then
    mkdir -p ${ASCEND_SECLOG}
    setFileChmod -f 755 /var/log
    setFileChmod -f 750 ${ASCEND_SECLOG}
fi

if [ -f $installInfo ]; then
    setFileChmod -f 644 $installInfo
    UserName=$(getInstallParam "UserName" "${installInfo}")
    UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
    if [ ! -z "$UserName" ]; then
        username="$UserName"
    fi
    if [ ! -z "$UserGroup" ]; then
        usergroup="$UserGroup"
    fi
fi

runfilename="${1##*/}"
full_install=n
uninstall=n
upgrade=n
input_install_path=""
input_path_flag=n
input_install_username=n
input_install_usergroup=n
Install_Path_Param="/usr/local/Ascend"
operation="Install"
quiet=n
run=n
version=n
devel=n
installType=""
installMode="normal"
install_for_all="no"
docker=n
check=n
vnpu_host=n
vnpu_guest=n
vnpu_docker_host=n
runPackagePath=""
driver_install_status=0
boot_server=n

# cut first two params from *.run
# $1: run package name，$2: run package path，$3: parameter list
runPackagePath="$2"
runPackagePath="${runPackagePath:2}"
log "[INFO]runPackagePath =$runPackagePath"
shift 2

start_time=$(date +"%Y-%m-%d %H:%M:%S")
all_parma="$@"
if [ -z "$all_parma" ]; then
    errorUsage
fi

startLog
drvEcho "[INFO]LogFile: ${ASCEND_SECLOG}/ascend_install.log"
drvEcho "[INFO]OperationLogFile: ${ASCEND_SECLOG}/operation.log"
log "[INFO]UserCommand: $runfilename $all_parma"

changeLogMode
checkProcess
logBaseVersion

# i is used for: version/uninstall/check can only be used independently.
i=0
# j is used for: common install-type full/devel/docker/upgrade is must.
j=0
# k is used for: the number of vnpu parameters can be only one.
k=0
while true
do
    case "$1" in
    --uninstall)
    i=$(( $i + 1 ))
    operation=${LOG_OPERATION_UNINSTALL}
    uninstall=y
    shift
    ;;
    --upgrade)
    i=$(( $i + 1 ))
    j=$(( $j + 1 ))
    operation=${LOG_OPERATION_UPGRADE}
    upgrade=y
    shift
    ;;
    --full)
    i=$(( $i + 1 ))
    j=$(( $j + 1 ))
    operation=${LOG_OPERATION_INSTALL}
    uniqueMode
    installType="full"
    full_install=y
    shift
    ;;
    --devel)
    i=$(( $i + 1 ))
    j=$(( $j + 1 ))
    uniqueMode
    installType="devel"
    devel=y
    shift
    ;;
    --docker)
    i=$(( $i + 1 ))
    j=$(( $j + 1 ))
    uniqueMode
    installType="docker"
    docker=y
    shift
    ;;
    --version)
    i=$(( $i + 1 ))
    version=y
    shift
    ;;
    --quiet)
    i=$(( $i + 1 ))
    quiet=y
    shift
    ;;
    --run)
    i=$(( $i + 1 ))
    j=$(( $j + 1 ))
    uniqueMode
    installType="run"
    run=y
    shift
    ;;
    --vnpu_host)
    i=$(( $i + 1 ))
    k=$(( $k + 1 ))
    installMode="vnpu_host"
    vnpu_host=y
    shift
    ;;
    --vnpu_guest)
    i=$(( $i + 1 ))
    k=$(( $k + 1 ))
    installMode="vnpu_guest"
    vnpu_guest=y
    shift
    ;;
    --vnpu_docker_host)
    i=$(( $i + 1 ))
    k=$(( $k + 1 ))
    installMode="vnpu_docker_host"
    vnpu_docker_host=y
    shift
    ;;
    --install-for-all)
    input_install_for_all=y
    install_for_all="yes"
    shift
    ;;
    --install-username=*)
    i=$(( $i + 1 ))
    input_install_username=y
    username=`echo "$1" | cut -d"=" -f2- `
    shift
    ;;
    --install-usergroup=*)
    i=$(( $i + 1 ))
    input_install_usergroup=y
    usergroup=`echo "$1" | cut -d"=" -f2- `
    shift
    ;;
    --install-path=*)
    i=$(( $i + 1 ))
    input_path_flag=y
    temp_path=`echo "$1" | cut -d"=" -f2- `
    input_install_path=`echo "$temp_path"`
    if [ "$input_install_path""x" = "x" ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:install path is null, install failed"
    else
        shift
    fi
    ;;
    --boot-server-path=*)
    i=$(( $i + 1 ))
    uniqueMode
    installType="boot_server"
    boot_server=y
    input_path_flag=y
    temp_path=`echo "$1" | cut -d"=" -f2- `
    input_install_path=`echo "$temp_path"`
    if [ "$input_install_path""x" = "x" ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:boot server path is null, install failed"
    else
        shift
    fi
    ;;
    --check)
    i=$(( $i + 1 ))
    check=y
    shift
    ;;
    -*)
    errorUsage $1
    ;;
    *)
    break
    ;;
    esac
done

get_feature_config
if [ $upgrade = y ] || [ $full_install = y ] || [ $run = y ]; then
    checkRunPkgIsSuitableByPCI
fi

if [ $upgrade = y ] || [ $full_install = y ] || [ $run = y ] || [ $devel = y ] || [ $docker = y ] || [ $boot_server = y ]; then
    drvColorEcho  "[WARNING]\033[33mDo not power off or restart the system during the installation/upgrade\033[0m"
fi

unsupportParam
if [ $input_install_username = y ] || [ $input_install_usergroup = y ]; then
    if [ -z $username ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:input_install_username is empty"
    elif [ $username = "root" ] && [ ${input_install_for_all} = n ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:username not permission for root"
    fi

    if [ -z $usergroup ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:input_install_group is empty"
    elif [ $usergroup = "root" ] && [ ${input_install_for_all} = n ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:usergroup not permission for root"
    fi
fi

if [ $check = y ]; then
    ./driver/script/ver_check.sh $check $quiet
    exitLog $?
fi
if [ $version = y ]; then
    drvEcho "[INFO]driver version :"$(getVersionInRunFile)
    exitLog 0
fi

trap "logOperation \"${operation}\" \"${start_time}\" \"${runfilename}\" \"${LOG_RESULT_FAILED}\" \"${installType}\" \"${all_parma}\";exit 1" \
INT QUIT TERM HUP
if [ $upgrade = y ]; then
    uniqueMode
    if [ -f ${installInfo} ];then
        Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
        if [ -z "$Driver_Install_Type" ]; then
            installType="full"
        else
            installType="$Driver_Install_Type"
        fi
    else
        installType="full"
    fi
fi

# check install-path if it's valid according to regular-expression.
if [ "${input_path_flag}" = "y" ]; then
    source "${SHELL_DIR}/common_func.inc"
    check_install_path_valid "${input_install_path}"
    if [ $? -ne 0 ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:The install-path [${input_install_path}] is invalid according to regular-expression, only characters in [a-z,A-Z,0-9,-,_] are supported."
    else
        log "[INFO]check_install_path_valid success."
    fi
fi

if [ $boot_server = y ]; then
    boot_server_file_copy
    logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_SUCCESS}" "${installType}" "${all_parma}"
    drvEcho "[INFO]install boot_server_file success."
    exitLog 0
fi

getUserInfo
rotateLog
checkUserAndGroup
isValidPath
checkFreeSpace

# if hotreset-status file exist, delete it.
[ -e "${hotreset_status_file}" ] && rm -f "${hotreset_status_file}" >& /dev/null

if [ -f ${installInfo} ]; then
    Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
    # first installation scenario
    if [ -z "$Driver_Install_Path_Param" ]; then
        # uninstall scenario
        if [ $uninstall = y ]; then
            error "${LOG_OPERATION_UNINSTALL}" 1 "ERR_NO:0x0080;ERR_DES:Driver package is not installed on this device, uninstall failed"
        # installation scenario
        elif [ $full_install = y ] || [ $upgrade = y ] || [ $run = y ] || [ $docker = y ] || [ $devel = y ]; then
            first_time_install_drv_flag="Y"
            install_path_should_belong_to_root
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            installRun "Install"
            operation="${LOG_OPERATION_INSTALL}"
        fi
    else
        first_time_install_drv_flag="N"
        old_drv_install_path="${Driver_Install_Path_Param}"

        version1=$(getVersionInRunFile)
        version2=$(getVersionInstalled "$Driver_Install_Path_Param")
        # uninstall scenario
        if [ $uninstall = y ]; then
            uninstallRun "Uninstall"
        # overwrite installation scenarios
        elif [ $full_install = y ] || [ $run = y ] || [ $docker = y ] || [ $devel = y ]; then
            install_path_should_belong_to_root
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            is_flash_version_check "${old_drv_install_path}"
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            # run-pkg version compatible check.
            ./driver/script/ver_check.sh $check $quiet || exitLog 1
            if [ ! $version2"x" = "x" ] && [ ! "$version2" = "none" ] && [ $quiet = n ]; then
                # determine whether to overwrite the installation
                drvEcho "[INFO]Driver package has been installed on the path $Driver_Install_Path_Param, the version is ${version2}, and the version of this package is ${version1},do you want to continue?  [y/n] "
                readYesOrNoFromTerminal
            fi
            uninstallRun "Install"
            installRun "Install"
            operation="${LOG_OPERATION_INSTALL}"
        # upgrade scenario
        elif [ $upgrade = y ]; then
            install_path_should_belong_to_root
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            is_flash_version_check "${old_drv_install_path}"
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            ./driver/script/ver_check.sh $check $quiet || exitLog 1
            uninstallRun "Upgrade"
            installRun "Upgrade"
            operation="${LOG_OPERATION_UPGRADE}"
        fi
    fi
else
    # uninstall scenario
    if [ $uninstall = y ]; then
        error "${LOG_OPERATION_UNINSTALL}" 1 "ERR_NO:0x0080;ERR_DES:Driver package is not installed on this device, uninstall failed"
    # installation scenario
    elif [ $full_install = y ] || [ $upgrade = y ] || [ $run = y ] || [ $docker = y ] || [ $devel = y ]; then
        first_time_install_drv_flag="Y"
        install_path_should_belong_to_root || exitLog 1
        ./driver/script/ver_check.sh $check $quiet || exitLog 1
        installRun "Install"
        operation="${LOG_OPERATION_INSTALL}"
    fi
fi

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
    if [ "$(isHotresetFailed)" = "true" ]; then
        exitLog 2
    else
        exitLog 0
    fi
fi
