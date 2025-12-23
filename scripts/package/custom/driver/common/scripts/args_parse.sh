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

# get the version number under the installation path (version)
getVersionInstalled() {
    local version="none"
    if [ -f "$1"/driver/version.info ]; then
        version=$(cat "$1"/driver/version.info | awk -F '=' '$1=="Version" {print $2}')
    fi
    echo $version
}

# check if root permission
isRoot() {
    if [ $(id -u) -ne 0 ]; then
        drvEcho "[ERROR]ERR_NO:0x0093;ERR_DES:do not have root permission, operation failed, please use root permission!"
        exit 1
    fi
}

changeLogMode() {
    if [ ! -f $logFile ]; then
        touch $logFile
    fi
    setFileChmod -f 640 $logFile
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

uniqueMode() {
    if [ ! -z $installType ]; then
        errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:only support one type: full/debug/run/docker/devel, operation failed!"
    fi
}

# check if EP permission
isEP() {
    if [ $full_install = y ] || [ $debug = y ] || [ $upgrade = y ] || [ $run = y ] || [ $devel = y ]; then
        isRC_true=$(lspci | grep d107)
        if [ ! -z "$isRC_true" ]; then
            drvEcho "[WARNING]The current installation package is 310B EP, please check whether the current environment is EP"
            exit 1
        fi
    fi
}

# check if package and environment is match
checkDriverProduct() {
    if [ $full_install = y ] || [ $debug = y ] || [ $upgrade = y ] || [ $run = y ] || [ $devel = y ]; then
        local bdfs=($(lspci -nn | grep -o '19e5:d[0-9]0[0-9]'))
        first_bdf=$(echo "$device_bdf" | cut -d'|' -f1)
        log "[INFO] Driver  expected bdfs (${first_bdf//0x/}), device actual bdfs is (${bdfs[*]})"
        if [ -n "$device_bdf" ] && [ ${#bdfs[@]} -ne 0 ] && [[ ! ${bdfs[*]} =~ ${first_bdf//0x/} ]]; then
            # does not match the device_bdf configured in the driver package.
            drvEcho "[ERROR]The current installation package and environment is not match, please check."
            exit 1
        fi
    fi
}

# check if support permission
checkDebugMode() {
    if [ $debug = y ]; then
        systemd-detect-virt -v | grep -E "kvm|vmware|qemu|xen" >> /dev/null 2>&1
        if [ $? = 0 ]; then
            drvEcho "[INFO]Installation in VM env!"
        fi
        isMilanDriver=$(echo $device_bdf | grep -E "d802|d803|d500")
        if [ -z "$isMilanDriver" ]; then
            drvEcho "[ERROR]This product does not support debug install type!"
            exit 1
        fi
        # debug mode add warning information
        if [ $quiet = n ]; then
            drvColorEcho "[WARNING]\033[33mRisks exist when the install type is debug, do you want to continue? [y/n]\033[0m"
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
        else
            drvEcho "[ERROR]Risks exist when the install type is debug, quiet parameter is not supported!"
            exit 1
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

get_feature_config() {
    local config

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_HOT_RESET | awk -F'=' '{print $2}')
    log "[INFO]FEATURE_HOT_RESET is : $config"
    if [ "$config"x = "y"x ];then
        feature_hot_reset=y
        log "[INFO]set feature_hot_reset=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_CRL_CHECK | awk -F'=' '{print $2}')
    log "[INFO]FEATURE_CRL_CHECK is : $config"
    if [ "$config"x = "y"x ];then
        feature_crl_check=y
        log "[INFO]set feature_crl_check=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_NVCNT_CHECK | awk -F'=' '{print $2}')
    log "[INFO]FEATURE_NVCNT_CHECK is : $config"
    if [ "$config"x = "y"x ];then
        feature_nvcnt_check=y
        log "[INFO]set feature_nvcnt_check=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_DKMS_COMPILE | awk -F'=' '{print $2}')
    log "[INFO]FEATURE_DKMS_COMPILE is : $config"
    if [ "$config"x = "y"x ];then
        feature_dkms_compile=y
        log "[INFO]set feature_dkms_compile=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_VIRT_SCENE | awk -F'=' '{print $2}')
    log "[INFO]FEATURE_VIRT_SCENE is : $config"
    if [ "$config"x = "y"x ];then
        feature_virt_scene=y
        log "[INFO]set feature_virt_scene=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_NO_DEVICE_KERNEL | awk -F'=' '{print $2}')
    log "[INFO]FEATURE_NO_DEVICE_KERNEL is : $config"
    if [ "$config"x = "y"x ];then
        feature_no_device_kernel=y
        log "[INFO]set feature_no_device_kernel=y"
    fi

    config=$(cat "$sourcedir"/script/feature.conf | grep FEATURE_VER_COMPATIBLE_CHECK | awk -F'=' '{print $2}')
    log "[INFO]FEATURE_VER_COMPATIBLE_CHECK is : $config"
    if [ "$config"x = "y"x ];then
        feature_ver_compatible_check=y
        log "[INFO]set feature_ver_compatible_check=y"
    fi

    return 0
}

get_kmsagent_config() {
    if [ $kmsagent_is_enable = y ];then
        return 0
    fi

    if [ -f $ASCEND_DRIVER_CFG_FILE ];then
        local kms_config_txt=`grep -r "${KMSAGENT_CONFIG_ITEM}=" "${ASCEND_DRIVER_CFG_FILE}" | cut -d"=" -f2-`
        if [ "${kms_config_txt}X" = "enableX" ];then
            kmsagent_is_enable=y
        fi
    fi
}

unsupportParam() {
    if [ $uninstall = y ];then
        if [ $i -eq 1 ];then
            return 0
        fi
        if [ $force = y ] && [ $i -eq 2 ];then
            log "[INFO]Uninstall and take effect after reboot"
            return 0
        else
            errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:uninstall can't use with other parameters except force."
        fi
    fi
    # version/check can only be used independently.
    if [ "${version}" = "y" ] || [ "${check}" = "y" ]; then
        if [ $i -gt 1 ]; then
            errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:version/check can't use with other parameters."
        fi
    else
        # if not input full/debug/run/docker/upgrade/devel parameter, installation will stop.
        if [ $j -ne 1 ]; then
            if [ "${force}" = "y" ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:The parameter needs to be used with full/debug/run/upgrade/uninstall."
            else
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:The parameter needs to be used with full/debug/run/docker/upgrade/devel."
            fi
        else
            # if install for all, docker-install-type will not be allowed.
            if [ "${input_install_for_all}" = "y" ] && [[ "${docker}" = "y" || "${upgrade}" = "y" ]]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:install-for-all param need used with full/debug/run/devel."
            fi
            # if input install-path, upgrade-install-type will not be allowed.
            if [ "${input_path_flag}" = "y" ] && [ "${upgrade}" = "y" ]; then
                errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:install-path param need used with full/debug/run/docker/devel."
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
                        errorUsage 1 "[ERROR]ERR_NO:0x0004;ERR_DES:install-username and install-usergroup params need used with full/debug/run/docker/devel."
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
# Script entry
crossVersion

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
runPackagePath=""
full_install=n
uninstall=n
upgrade=n
debug=n
norebuild=n
input_install_path=""
input_path_flag=n
input_install_username=n
input_install_usergroup=n
Install_Path_Param="/usr/local/Ascend"
operation="Install"
quiet=n
force=n
run=n
version=n
devel=n
installType=""
installMode="normal"
install_for_all="no"
docker=n
check=n
kmsagent_is_enable=n
vnpu_host=n
vnpu_guest=n
vnpu_docker_host=n
runPackagePath=""
driver_install_status=0

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

isRoot
changeLogMode

# i is used for: version/uninstall/check can only be used independently.
i=0
# j is used for: common install-type is must.
j=0
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
    --debug)
    i=$(( $i + 1 ))
    j=$(( $j + 1 ))
    operation=${LOG_OPERATION_INSTALL}
    uniqueMode
    installType="debug"
    installMode="debug"
    debug=y
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
    --force)
    i=$(( $i + 1 ))
    force=y
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
    --vnpu_guest)
    i=$(( $i + 1 ))
    installMode="vnpu_guest"
    vnpu_guest=y
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
    --check)
    i=$(( $i + 1 ))
    check=y
    shift
    ;;
    --norebuild)
    norebuild=y
    shift
    ;;
    --repack)
    repack=y
    packagedir=""
    shift
    packagename=$1
    if [ $# -gt 0 ]; then shift; fi
    ;;
    --repack-path=*)
    repack=y
    packagedir=`echo $1 | cut -d"=" -f2 `
    if test x"${packagedir}" = x; then
        errorUsage $1
    fi
    shift
    packagename=$1
    if [ $# -gt 0 ]; then shift; fi
    ;;
    -*)
    errorUsage $1
    ;;
    *)
    break
    ;;
    esac
done

checkArch
isEP
checkDriverProduct
checkDebugMode

if test x"$repack" = xy; then
    source ./driver/script/repack_driver.sh && exitLog $? || exit 1
fi

checkProcess
logBaseVersion

if [ $upgrade = y ] || [ $full_install = y ] || [ $debug = y ] || [ $run = y ] || [ $devel = y ] || [ $docker = y ]; then
    drvColorEcho  "[WARNING]\033[33mDo not power off or restart the system during the installation/upgrade\033[0m"
fi

get_feature_config
get_kmsagent_config
unsupportParam