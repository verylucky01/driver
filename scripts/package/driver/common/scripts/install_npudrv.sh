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

# 默认参数赋值
set_dir() {
    if [ -e /HOST_ROOT_TO_CONTAINER/etc/run_dir.info ];then
        . /HOST_ROOT_TO_CONTAINER/etc/run_dir.info
        run_dir=/HOST_ROOT_TO_CONTAINER/$Dir_Path_Param
        echo "$Dir_Path_Param run package is be use success"
    else
        run_dir=/HOST_ROOT_TO_CONTAINER/home/
        echo "/home/ run package is be use success"
    fi
}

installInfo="/HOST_ROOT_TO_CONTAINER/etc/ascend_install.info"
run=HiAI_1.1.T60.B112_Linux.run
unpack_name=HiAI_1.1.T60.B112_Linux.run
first_time_flag=y

# root用户检测
root_check() {
    [ $UID -ne 0 ] && echo "please execute the script by root user." && return 1 || return 0
}

# 从默认路径取run包
set_run_package() {
    run=`ls ${run_dir}/*.run 2>/dev/null | grep -E "Ascend.*-driver-" | awk 'END{print $NF}'`
    [ "$run" = "" ] && echo "Driver run-package doesn't exist." && return 1
    echo "set_run_package success."

    return 0
}

# 设置解压目录名称
set_unpack_name() {
    unpack_name=`date +"%Y%m%d%H%M%S%N"`
    echo "set_unpack_name success."

    return 0
}

# 解压run包
unpack_run_package() {
    local ret
    echo "it is unpacking now, which will cost a little time, please wait for a moment."
    $run --noexec --extract=/var/log/$unpack_name >> /dev/null 2>&1 && ret=$? || ret=$?
    echo "unpack_run_package success."

    return $ret
}
# 判断是否首次安装
set_install_flag() {
    if [ -f $installInfo ] && cat "${installInfo}" | grep -q "Driver_Install_Path_Param" && `chroot /HOST_ROOT_TO_CONTAINER/ lsmod | grep -q "drv_pcie_hdc_host"`; then
        first_time_flag=n
    fi
}

# 首次安装
child_driver_install() {
    local ret
    echo "------------install info start------------"
    chroot /HOST_ROOT_TO_CONTAINER/ ${run:23} --full --quiet && ret=$? || ret=$?
    echo "------------install info end  ------------"

    return $ret
}

# 版本是否一致
is_same_version() {
    local old_version new_version
    if [ "$first_time_flag" = "n" ]; then
        . $installInfo
        Driver_Install_Path_Param=${Driver_Install_Path_Param#*/}

        if [ ! -f /HOST_ROOT_TO_CONTAINER/${Driver_Install_Path_Param}/driver/version.info ]; then
            echo "/HOST_ROOT_TO_CONTAINER/${Driver_Install_Path_Param}/driver/version.info doesn't exits."
            return 1
        fi

        old_version=$(cat /HOST_ROOT_TO_CONTAINER/${Driver_Install_Path_Param}/driver/version.info 2>/dev/null | grep -m 1 -i "version" | awk -F [=] '{print $NF}')
        echo "old driver version is [${old_version}]."

        new_version=$(cat /var/log/${unpack_name}/version.info 2>/dev/null | grep -m 1 -i "version" | awk -F [=] '{print $NF}')
        echo "new driver version is [${new_version}]."

        [ "${old_version}"x = "${new_version}"x ] && return 0 || return 1
    fi

    return 1
}

# 安装驱动
run_driver_install() {
    local ret
    if [ "$first_time_flag" = "y" ]; then
        child_driver_install && ret=$? || ret=$?
    else
        if is_same_version; then
            echo "to install version is already existed in current environment." && ret=2
        else
            child_driver_install && ret=$? || ret=$?
        fi
    fi

    return $ret
}

clear_cache() {
    rm -rf /var/log/${unpack_name}
    echo "clear cache success."
}

####################
#
#  main process
#
####################

root_check
if [ $? -ne 0 ]; then
    echo "root_check failed."
    exit 1
fi

set_dir
if [ $? -ne 0 ]; then
    echo "set_dir failed."
    exit 1
fi

set_run_package
if [ $? -ne 0 ]; then
    echo "set_run_package failed."
    exit 1
fi

set_unpack_name
if [ $? -ne 0 ]; then
    echo "set_unpack_name failed."
    exit 1
fi

unpack_run_package
if [ $? -ne 0 ]; then
    echo "unpack_run_package failed."
    clear_cache
    exit 1
fi
set_install_flag
if [ $? -ne 0 ]; then
    echo "set_install_flag failed."
    clear_cache
    exit 1
fi

run_driver_install && rets=$? || rets=$?
if [ $rets -ne 0 ]; then
    if [ $rets -eq 2 ]; then
        echo "install success"
        clear_cache
        exit 0
    else
        clear_cache
        exit 1
    fi
fi

clear_cache

exit 0
