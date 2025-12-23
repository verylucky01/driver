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
username=$UserName
usergroup=$UserGroup
if [ "$username" = "" ]; then
    username=HwHiAiUser
    usergroup=HwHiAiUser
fi

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    user_id=`id | awk '{printf $1}'`
    echo "[Server] [$cur_date] [$user_id] "$1 >> $logFile
}

servers_autostart_ubuntu() {
    setupfile="$Driver_Install_Path_Param"/host_sys_init.sh

    if [ -f $setupfile ]; then
        target_path=/etc/init.d
        if [ -f $target_path/host_sys_init.sh ]; then
            update-rc.d -f host_sys_init.sh remove > /dev/null 2>&1
            rm -f $target_path/host_sys_init.sh

            log "[INFO]server remove success"
        fi

        cp -fp $setupfile $target_path/
        update-rc.d host_sys_init.sh defaults 90 > /dev/null 2>&1

        log "[INFO]server($target_path/host_sys_init.sh) setup success"
    else
        log "[ERROR]no $setupfile file, server setup failed"
    fi
}

servers_autostart_centos() {
    local selinux_flag=0
    setupfile="$Driver_Install_Path_Param"/host_sys_init.sh

    if [ -f "$setupfile" ]; then
        target_path=/etc/init.d
        if [ ! -d $target_path ];then
            target_path=/etc/rc.d/init.d
        fi

        if [ -f $target_path/host_sys_init.sh ]; then
            chkconfig host_sys_init.sh off
            chkconfig --del host_sys_init.sh >/dev/null 2>&1
            rm -f $target_path/host_sys_init.sh

            log "[INFO]server remove success"
        fi

        cp -fp "$setupfile" $target_path/

        which md5sum >/dev/null 2>&1
        if [ $? -eq 0 ];then
            if [ ! -f "$target_path"/host_sys_init.sh ] || [ $(md5sum "$setupfile" | awk '{print $1}') != $(md5sum "$target_path"/host_sys_init.sh | awk '{print $1}') ]; then
                log "[ERROR]$target_path/host_sys_init.sh does not exist or md5sums do not match"
                exit 1
            fi
        fi

        chkconfig --add host_sys_init.sh >/dev/null 2>&1
        chkconfig host_sys_init.sh on

        log "[INFO]server($target_path/host_sys_init.sh) setup success"
                getenforce 2>/dev/null | grep -i "enforcing" >/dev/null 2>&1 && selinux_flag=1
                if [ $selinux_flag -eq 1 ];then
                    chcon -u system_u -r object_r -t initrc_exec_t  $target_path/host_sys_init.sh
                    log "[INFO]set host_sys_init selinux type success"
                fi

    else
        log "[ERROR]no $setupfile file, server setup failed"
    fi
    return 0
}

so_ldconfig() {
    file_conf=/etc/ld.so.conf.d/ascend_driver_so.conf

    if [ -f $file_conf ];then
        rm -f $file_conf
        ldconfig > /dev/null 2>&1
    fi
    echo "${Driver_Install_Path_Param%*/}"/driver/lib64/common >> $file_conf
    echo "${Driver_Install_Path_Param%*/}"/driver/lib64/driver >> $file_conf
    echo "${Driver_Install_Path_Param%*/}"/driver/lib64 >> $file_conf
    chmod -f 440 ${file_conf}
    ldconfig > /dev/null 2>&1

    log "[INFO]run environment set success"
}

bin_config() {
    hcp_bin="${Driver_Install_Path_Param%*/}"/driver/tools/hcp
    if [ -f "$hcp_bin" ];then
        rm -rf /usr/bin/hcp >>/dev/null 2>&1
        ln -sf "$hcp_bin" /usr/bin/hcp
    fi

    omg_bin="${Driver_Install_Path_Param%*/}"/runtime/bin/atc
    if [ -f "$omg_bin" ];then
        rm -rf /usr/bin/atc >>/dev/null 2>&1
        ln -sf "$omg_bin" /usr/bin/atc
    fi

    nnnode_bin="${Driver_Install_Path_Param%*/}"/runtime/bin/nnnodeExp
    if [ -f "$nnnode_bin" ];then
        rm -rf /usr/bin/nnnode_bin >>/dev/null 2>&1
        ln -sf "$nnnode_bin" /usr/bin/nnnodeExp
    fi

    hccn_bin="${Driver_Install_Path_Param%*/}"/driver/tools/hccn_tool
    if [ -f "$hccn_bin" ];then
        rm -rf /usr/bin/hccn_tool >>/dev/null 2>&1
        ln -sf "$hccn_bin" /usr/bin/hccn_tool
    fi

    msnpureport_bin="${Driver_Install_Path_Param%*/}"/driver/tools/msnpureport
    if [ -f "$msnpureport_bin" ];then
        rm -rf /usr/bin/msnpureport >>/dev/null 2>&1
        ln -sf "$msnpureport_bin" /usr/bin/msnpureport
    fi
    log "[INFO]run bin set success"
    return 0
}

HostOsName=unknown
HostOsVersion=unknown
. "$Driver_Install_Path_Param"/driver/script/common_func.inc
get_system_info
release_os=$HostOsName
release_version=$HostOsVersion

version_lt() { test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" != "$1"; }

# start
log "[INFO]server load start..."
case "$release_os" in
    Ubuntu)
    if version_lt $release_version "16.04";then
        log "[WARNING]update-rc.d may not work on $release_os $release_version"
    fi
    servers_autostart_ubuntu
    ;;

    CentOS | EulerOS | BigCloud)
    if [ "$release_os" = "CentOS" ];then
        if version_lt $release_version "7";then
            log "[WARNING]chkconfig may not work on $release_os $release_version"
        fi
    fi
    if [ "$release_os" = "EulerOS" ];then
        if version_lt $release_version "2.0";then
            log "[WARNING]chkconfig may not work on $release_os $release_version"
        fi
    fi
    if [ "$release_os" = "BigCloud" ];then
        if version_lt $release_version "7";then
            log "[WARNING] chkconfig may not work on $release_os $release_version"
        fi
    fi
    servers_autostart_centos
    ;;

    # this is to be compatible with other OSs.
    *)
    which chkconfig >/dev/null 2>&1
    if [ $? -eq 0 ];then
        servers_autostart_centos
    else
        which update-rc.d >/dev/null 2>&1
        if [ $? -eq 0 ];then
            servers_autostart_ubuntu
        else
            log "[ERROR]chkconfig or update-rc.d may not work on $release_os"
            exit 1
        fi
    fi
    ;;
esac

log "[INFO]auto start scripts setuped on $release_os $release_version"

so_ldconfig
bin_config

log "[INFO]server load end..."

exit 0
