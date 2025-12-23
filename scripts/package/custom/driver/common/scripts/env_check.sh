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

getUserInfo() {
    if [ -f ${installInfo} ]; then
        UserName=$(getInstallParam "UserName" "${installInfo}")
        UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
        Driver_Install_For_All=$(getInstallParam "Driver_Install_For_All" "${installInfo}")
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
                if [ "${UserName}" = "root" ] && [ $upgrade != y ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:username not permission for root, check /etc/ascend_install.info"
                else
                    username="${UserName}"
                    if [ "x$Driver_Install_For_All" = "xyes" ] && [ $upgrade = y ]; then
                        install_for_all="$Driver_Install_For_All"
                        input_install_for_all=y
                    fi
                fi
            fi
            if [ "x${UserGroup}" != "x" ]; then
                if [ "${UserGroup}" = "root" ] && [ $uninstall = n ] && [ $upgrade != y ]; then
                    errorUsage 1 "[ERROR]ERR_NO:0x0091;ERR_DES:usergroup not permission for root, check /etc/ascend_install.info"
                else
                    usergroup="${UserGroup}"
                    if [ "x$Driver_Install_For_All" = "xyes" ] && [ $upgrade = y ]; then
                        install_for_all="$Driver_Install_For_All"
                        input_install_for_all=y
                    fi
                fi
            fi
        fi
    fi
}

# check if the user exists
checkUser(){
    ret=$(cat /etc/passwd | cut -f1 -d':' | grep -w "$1" -c)
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

# support absolute path and relative path
getAbsolutePath(){

    local current_path="${PWD}" parent_dir=""
    flag1=""
    case "$Install_Path_Param" in
        *./* | *~/* | */. | */.?*) flag1="matched" ;;
    esac
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
            su - "$username" -s /bin/bash -c "test -x $parentPath" >>/dev/null 2>&1
            if [ $? -ne 0 ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:The $username do not have the permission to access $parentPath, please reset the directory to a right permission."
            fi
            mkdir -p "$Install_Path_Param"
            setFileChmod -f 755 "$Install_Path_Param"
            makeDefaultPath=y
        fi
    fi

    su - "$username" -s /bin/bash -c "test -x $Install_Path_Param" >>/dev/null 2>&1
    if [ $? -ne 0 ]; then
        if [ $makeDefaultPath = y ]; then
            rm -rf "$Install_Path_Param"
        fi
        errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:The $username do not have the permission to access $Install_Path_Param, please reset the directory to a right permission."
    fi
    # format the installation path after checking its validity
    which realpath >/dev/null 2>&1 && Install_Path_Param=`realpath $Install_Path_Param 2>/dev/null`
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

check_firmware_version_byfile() {
    firmware_version_byfile=$(cat ${Driver_Install_Path_Param}/firmware/version.info | grep ^Version= | awk -F. '{print $2}')
    if [ "${firmware_version_byfile}"x = "11"x ];then
        please_change_RC1driver_package
    fi
}

please_change_RC1driver_package() {
    drvEcho "[WARNING]Download and install the 23.0.RC1 driver package. Then upgrade to the target version. "
    log "[WARNING]Download and install the 23.0.RC1 driver package. Then upgrade to the target version."
    exitLog 1
}

check_firmware_version_bybar() {
    for DEVICE_BDF_910B in ${DEVICE_BDFS_910B[@]}
    do
        BARSPACE_FIRMWARE_91OB=$(lspci -s $DEVICE_BDF_910B -xxxx | grep 4e0)
        firmware_version_bybar=$(echo $BARSPACE_FIRMWARE_91OB | awk -F '[: ]+' '{print $9}')
        if [ "${firmware_version_bybar}"x = "11"x ];then
            please_change_RC1driver_package
        fi
    done
}

check_pkg_firmware_version() {
    [ $uninstall = y ] && return 0

    # drv_pcie_host: Physical machine
    # drv_vpcie: Compute-slicing virtual-machine
    lsmod | grep -E "drv_pcie_host|drv_vpcie" >& /dev/null && first_time=n
    log "check if it is the first time installation flag, first_time:$first_time"

    DEVICE_BDFS_910B=`lspci -D -d $PCIE_BDF_910B | awk '{print $1}'`
    if [ "${DEVICE_BDFS_910B}" != "" ];then
        if [ "$first_time" = "y" ];then
            check_firmware_version_bybar
        else
            if [ -e ${Driver_Install_Path_Param}/firmware/version.info ];then
                check_firmware_version_byfile
            else
                #This sentence does not need to be printed in the virtual machine scenario
                systemd-detect-virt -v | grep -E "kvm|vmware|qemu|xen" >> /dev/null 2>&1
                if [ $? -ne 0 ]; then
                    log "[WARNING]There is no firmware version.info."
                fi
                check_firmware_version_bybar
            fi
        fi
    fi
}

checkArch() {
    local curpath="$(dirname $(readlink -f "${BASH_SOURCE:-$0}"))"
    local scene_filepath="${curpath}/../scene.info"
    local arch

    if [ -f "${scene_filepath}" ]; then
        arch="$(grep "^arch=" "${scene_filepath}" | cut -d = -f 2)"
        if [ "${arch}" != "" ]; then
            PKG_ARCH="${arch}"
        fi
    fi

    if [ "$(arch)" != "${PKG_ARCH/-/_}" ]; then
        drvColorEcho "[ERROR]\033[31mArch mismatch, src package is ${PKG_ARCH}, cur os is $(arch) \033[0m"
        exitLog 1
    fi
}

# LogFile polling and backup

rotateLog() {
    config=$'        su root root\n        daily\n        size=5M\n        rotate 3\n        missingok\n        create 440 root root'
    printf '%s {\n%s\n    }\n' "$logFile" "$config" > /etc/logrotate.d/tmp
}

# check env tool before installation
checkEnvTool() {
    checkToolFile="${SHELL_DIR}/check_tools.conf"
    [ -e "${checkToolFile}" ] || return

    local strLeakTool=""
    local retCheckEnvTool="y"

    while read toolFileLine || [[ -n ${toolFileLine} ]]
    do
        # Some object only need to install one of toolFileLine
        toolA=`echo ${toolFileLine} | cut -d \, -f 1`
        toolB=`echo ${toolFileLine} | cut -d \, -f 2`
        which ${toolA} > /dev/null 2>&1 || which ${toolB} > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            strLeakTool="${strLeakTool}${toolFileLine},"
            retCheckEnvTool="n"
            continue;
        fi
    done < ${checkToolFile}

    if [ "${retCheckEnvTool}"x = "n"x ]; then
        errorUsage 1 "[ERROR]The list of missing tools: ${strLeakTool}"
    fi

    return 0
}

checkEnvFile() {
    # check /etc/pam.d/su Indicates whether to forcibly enter the password when su user
    if [ -e /etc/pam.d/su ]; then
        grep "auth" /etc/pam.d/su | grep "sufficient" |grep "pam_rootok.so" | grep -v "#" >/dev/null 2>&1
        if [ $? -ne 0 ];then
            drvEcho "[WARNING]The Root use cmd(su user) must enter the user passwd forcibly(set by /etc/pam.d/su)"
            log "[WARNING]The Root user cmd(su user) must enter the user passwd forcibly(set by /etc/pam.d/su)"
        fi
    fi
    return 0
}

checkItems() {
    # set status of PCIe driver for first installation
    # drv_pcie_host: Physical machine
    # drv_vpcie: Compute-slicing virtual-machine
    PCIE_INSMOD_STATUS_BEFORE_INSTALL="n"
    lsmod | grep -E "drv_pcie_host|drv_vpcie" >& /dev/null && PCIE_INSMOD_STATUS_BEFORE_INSTALL="y"
    log "[INFO]PCIE_INSMOD_STATUS_BEFORE_INSTALL=${PCIE_INSMOD_STATUS_BEFORE_INSTALL}"

    checkEnvTool
    checkEnvFile
}

# get the version number in the run package (version1)
getVersionInRunFile() {
    version1="none"
    if [ -f ./version.info ]; then
        version1=`cat version.info | awk -F '=' '$1=="Version" {print $2}'`
    fi
    echo $version1
}

uninstall_deb_sph() {
    sph_name=$(dpkg -l | grep -E 'atlas.*driver-sph|ascend.*driver-sph|alan.*driver-sph' | awk '{print $2}')
    driver_install_path_param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
    if [ -n "$sph_name" ]; then
        rm -rf /var/lib/dpkg/info/$sph_name*
        dpkg --remove --force-remove-reinstreq $sph_name > /dev/null 2>&1
        chattr -iR "$driver_install_path_param"/driver/livepatch > /dev/null 2>&1
        chattr -i "$driver_install_path_param"/version_sph.info > /dev/null 2>&1
        rm -rf "$driver_install_path_param"/driver/livepatch
        rm -rf "$driver_install_path_param"/version_sph.info
    fi
}

uninstall_rpm_sph() {
    sph_name=$(rpm -qa | grep -E 'Atlas.*driver-sph|Ascend.*driver-sph|Alan.*driver-sph')
    driver_install_path_param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
    if [ -n "$sph_name" ]; then
        rpm -e --noscripts $sph_name > /dev/null 2>&1
        chattr -i -R "$driver_install_path_param"/driver/livepatch > /dev/null 2>&1
        chattr -i "$driver_install_path_param"/version_sph.info > /dev/null 2>&1
        rm -rf "$driver_install_path_param"/driver/livepatch/
        rm -rf "$driver_install_path_param"/version_sph.info
    fi
}

# mixed package handling
check_package_mix() {
    local package_name ret
    if command -v dpkg &> /dev/null; then
        uninstall_deb_sph
        package_name=$(dpkg -l | grep -E 'atlas.*driver|ascend.*driver|alan.*driver' | grep -v 'sph'| awk '{print $2}')
        install_status=$(dpkg -l | grep -E 'atlas.*driver|ascend.*driver|alan.*driver' | grep -v 'sph' | awk '{print $1}')
        if [ -n "$package_name" ] && [ ${install_status} = "ii" ]; then
            drvEcho "[INFO]This environment has installed deb package $package_name, and it will now be uninstalled."
            dpkg -r $package_name
            ret=$?
            if [ ${ret} -ne 0 ]; then
                drvEcho "[WARNING]Driver uninstall failed, details in ${logFile}."
                return
            fi
        fi
    fi
    if command -v rpm &> /dev/null; then
        uninstall_rpm_sph
        package_name=$(rpm -qa | grep -E 'Atlas.*driver|Ascend.*driver|Alan.*driver' | grep -v 'sph')
        if [ -n "$package_name" ]; then
            drvEcho "[INFO]This environment has installed rpm package $package_name, and it will now be uninstalled."
            # ignore the logic of deleting user and usergroup when uninstalling RPM package
            if ([ x"${username}" == x"HwHiAiUser" ] || [ x"${usergroup}" == x"HwHiAiUser" ]) && [ -f ${Driver_Install_Path_Param}/creatuserHwHiAiUser ]; then
                rm -f ${Driver_Install_Path_Param}/creatuserHwHiAiUser
            fi
            rpm -e $package_name
            ret=$?
            if [ ${ret} -ne 0 ]; then
                drvEcho "[WARNING]Driver uninstall failed, details in ${logFile}."
                return
            fi
        fi
    fi
}

# Script entry

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

checkArch
[ $docker = y ] || check_pkg_firmware_version

if [ $check = y ]; then
    ./driver/script/ver_check.sh $check $quiet
    exitLog $?
fi

if [ $version = y ]; then
    drvEcho "[INFO]driver version :"$(getVersionInRunFile)
    exit 0
fi
trap 'logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"; exit 1' \
INT QUIT TERM HUP

if [ $upgrade = y ]; then
    uniqueMode
    if [ -f ${installInfo} ];then
        Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
        if [ -z "$Driver_Install_Type" -o "$Driver_Install_Type" = "debug" ]; then
            installType="full"
        else
            installType="$Driver_Install_Type"
        fi
    else
        installType="full"
    fi
fi

# --docker can only used in the container without no driver mapping
if [ $docker = y ]; then
    if [ -f ${installInfo} ]; then
        Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
        if [ ! -z "$Driver_Install_Type" ]; then
            if [ "${installType}"x != "${Driver_Install_Type}"x ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0003;ERR_DES:The parameter docker can only be used in the container without no driver mapping."
            fi
        fi
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

getUserInfo
rotateLog
checkUserAndGroup
isValidPath
checkFreeSpace
check_package_mix

if [ $full_install = y ] || [ $debug = y ] || [ $upgrade = y ] || [ $run = y ]; then
    [ "$Driver_Install_Type" = "docker" ] || checkItems
fi