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

sourcedir="$PWD"/driver
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
COMMON_SHELL="$SHELL_DIR/driver/script/common.sh"
# load common.sh, get install.info
source "${COMMON_SHELL}"
# read Driver_Install_Path_Param from installInfo
UserName=$(getInstallParam "UserName" "${installInfo}")
UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
username="$UserName"
usergroup="$UserGroup"
if [ "$username" = "" ]; then
    username=HwHiAiUser
    usergroup=HwHiAiUser
fi

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Server] [$cur_date] $1" >> $logFile
    return 0
}

remove_autostart_ubuntu() {
    setupfile=/etc/init.d/host_sys_init.sh

    if [ -f $setupfile ];then
        update-rc.d -f host_sys_init.sh remove > /dev/null 2>&1
        rm -f $setupfile
    fi

    log "[INFO]servers unsetup success"
    return 0
}

remove_autostart_centos() {
    target_path=/etc/init.d
    if [ ! -d $target_path ];then
        target_path=/etc/rc.d/init.d
    fi

    if [ -f $target_path/host_sys_init.sh ]; then
        chkconfig host_sys_init.sh off
        chkconfig --del host_sys_init.sh
        rm -f $target_path/host_sys_init.sh
    fi

    # remove_autostart_centos_port
    log "[INFO]server remove success"
}

remove_so_conf() {
    file_conf=/etc/ld.so.conf.d/ascend_driver_so.conf

    if [ -f $file_conf ];then
        rm -f $file_conf
        ldconfig > /dev/null 2>&1
    fi

    log "[INFO]run environment unset success"
}

override_cfg() {
    local _src_file=$1
    local _dest_file=$2

    if [ -f "${_src_file}" ]; then
        chattr -i "${_dest_file}" > /dev/null 2>&1
        cp -f "${_src_file}" "${_dest_file}"
        rm -f "${_src_file}"
    fi
}

remove_bin_conf()
{
        hccn_bin=/usr/bin/hccn_tool
        if [ -f $hccn_bin ];then
                rm -f $hccn_bin
        fi

    hcp_bin=/usr/bin/hcp
        if [ -f $hcp_bin ];then
        rm -f $hcp_bin
        fi

        omg_bin=/usr/bin/atc
        if [ -f $omg_bin ];then
        rm -f $omg_bin
        fi

        nnnode_bin=/usr/bin/nnnodeExp
        if [ -f $nnnode_bin ];then
        rm -f $nnnode_bin
        fi

    msnpureport_bin=/usr/bin/msnpureport
    if [ -f $msnpureport_bin ];then
        rm -f $msnpureport_bin
    fi

    log "[INFO]run bin unset success"
    return 0
}

log "[INFO]server unload start..."
if [ -f "$Driver_Install_Path_Param"/driver/script/common_func.inc ];then
    . "$Driver_Install_Path_Param"/driver/script/common_func.inc
else
    . ./driver/script/common_func.inc
fi
get_system_info
release_os=$HostOsName
case "$release_os" in
    Ubuntu | Debian)
    remove_autostart_ubuntu
    ;;

    CentOS | EulerOS)
    remove_autostart_centos
    ;;

    *)
    if [ "$release_os" = "openEuler" ]; then
        remove_autostart_centos
    elif [ -f "${Driver_Install_Path_Param}"/host_services_exit.sh ]; then
        "${Driver_Install_Path_Param}"/host_services_exit.sh "${release_os}"
        if [ $? -ne 0 ]; then
            log "[ERROR]host services exit failed."
        fi
    else
        log "[ERROR]os($release_os) not support"
    fi
    ;;
esac

log "[INFO]auto start scripts setuped on $release_os $release_version"

remove_so_conf
remove_bin_conf

log "[INFO]server unload end..."

exit 0
