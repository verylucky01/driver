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
pci_dev_path="/sys/bus/pci/devices/"
logFile="${ASCEND_SECLOG}/ascend_install.log"
hotreset_status_file="/var/log/hotreset_status.log"
installInfo="/etc/ascend_install.info"
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
COMMON_SHELL="$SHELL_DIR/common.sh"
# load common.sh, get install.info
source "${COMMON_SHELL}"
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
Driver_Install_Mode="$(getInstallParam "Driver_Install_Mode" "${installInfo}")"

# set default value for hot-reset.
reset_dev_nums="8"
# hot-reset support all chips.
HOT_RESET_SUPPORT_ALL_CHIPS="-1"

if [ -f "${Driver_Install_Path_Param}"/driver/script/specific_func.inc ];then
    . "${Driver_Install_Path_Param}"/driver/script/specific_func.inc
elif [ -f  "${PWD}"/driver/script/specific_func.inc ];then
    . ./driver/script/specific_func.inc
fi

# Get pf numbers and all bdf, which must be after specific_func.inc is loaded.
get_pci_info

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1" >> $logFile
}
drvEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}

printf_process_info()
{
    local app_list="$1"

    for app in $app_list
    do
        ps -f -p "$app" | grep "$app" >> $logFile
    done
}
# 1.check whether any process is still calling the kernel-mode ko based on the character-device of the driver,
#   if it is, the function will return 1, otherwise, it will return 0.
# 2.check whether the character-device of the driver is used in Docker based on the Docker image information,
#   if it is, the function will return 1, otherwise, it will return 0.
check_user_process()
{
    local dev_files="$(ls /dev/davinci* 2>/dev/null | grep -oE "davinci[0-9]{1,2}$") ${character_device}"
    local f_file
    local apps
    local docker_dev
    local docker_list

    if which fuser >& /dev/null; then
        for f_file in $dev_files
        do
            apps=`fuser -uv /dev/"$f_file"`
            if [ -n "${apps}" ]; then
                log $f_file" has user process:""$apps"
                printf_process_info "${apps}"
                return 1
            fi
        done
    else
        for f_file in $dev_files
        do
            apps=$(ls -l /proc/*/fd |egrep "fd:|/dev/"$f_file""|grep /dev/"$f_file" -B1|grep "fd:"|sed 's#/fd:##;s#/proc/##'|head)
            if [ -n "${apps}" ]; then
                log $f_file" has user process:""$apps"
                printf_process_info "${apps}"
                return 1
            fi
        done
    fi

    for f_file in $dev_files
    do
        docker >> /dev/null 2>&1
        if [ $? -eq 0 ];then
            docker_list=`docker ps -q`
            if [ -n "${docker_list}" ]; then
                docker_dev=`docker inspect $(docker ps -q)| grep "$f_file"`
                if [ -n "${docker_dev}" ]; then
                    log $f_file" has used by docker"
                    return 1
                fi
            fi
        fi
    done
    return 0
}


# get modules from specific_func.inc
remove_all_ko()
{
    local module modules

    if [ "${Driver_Install_Mode}" = "normal" ]; then
        modules="$(echo ${mode_normal} | awk -F [\|] '{for(i=1;i<NF;i=i+1) print $i}')"
    elif [ "${Driver_Install_Mode}" = "vnpu_host" ]; then
        modules="$(echo ${mode_vnpu_host} | awk -F [\|] '{for(i=1;i<NF;i=i+1) print $i}')"
    elif [ "${Driver_Install_Mode}" = "vnpu_docker_host" ]; then
        modules="$(echo ${mode_vnpu_docker_host} | awk -F [\|] '{for(i=1;i<NF;i=i+1) print $i}')"
    fi

    for module in $modules
    do
        log "[INFO]rmmod $module"
        rmmod $module >& /dev/null
        if [ $? -ne 0 ] && [ "$module" != "host_notify" ] && [ "$module" != "drv_vascend" ] && [ "$module" != "ascend_kernel_open_adapt" ];then
            log "[ERROR]rmmod failed"
            return 1
        fi
    done
    return 0
}
master_set_hotreset_flag()
{
        local bdfs=$1
        local bdf
        for bdf in $bdfs
        do
            log "[INFO]set hot reset flag: "$bdf
            echo 1 > $pci_dev_path$bdf"/hotreset_flag"
        done
}

get_bdfs()
{
    local find_type=$1
    local bdf="" bdfs=() chip_id="" len=0

    if [ "${find_type}" = "all" ]; then
        bdfs=(${g_bdfs[*]})
    elif [ "${find_type}" = "master" ] && echo "${device_bdf}" | grep -Ewq "0xd801|0xd803"; then
        for bdf in ${g_bdfs[*]}
        do
            if [ -e "${pci_dev_path}${bdf}/chip_id" ]; then
                chip_id=$(cat ${pci_dev_path}${bdf}/chip_id)
                if [ "${chip_id}" = "0" ]; then
                    len=${#bdfs[*]}
                    bdfs[${len}]="${bdf}"
                fi
            fi
        done
    # If master and not 0xd801, it will do reset for all.
    elif [ "${find_type}" = "master" ] && echo "${device_bdf}" | grep -vEwq "0xd801|0xd803"; then
        bdfs=(${g_bdfs[*]})
    # If slave and not 0xd801, it will do nothing.
    elif [ "${find_type}" = "slave" ] && echo "${device_bdf}" | grep -Ewq "0xd801|0xd803"; then
        for bdf in ${g_bdfs[*]}
        do
            if [ -e "${pci_dev_path}${bdf}/chip_id" ]; then
                chip_id=$(cat ${pci_dev_path}${bdf}/chip_id)
                if [ "${chip_id}" != "0" ]; then
                    len=${#bdfs[*]}
                    bdfs[${len}]="${bdf}"
                fi
            fi
        done
    fi

    echo "${bdfs[*]}"
    return 0;
}

get_bridge_bus_bdfs()
{
    local bdf bdfs bus_bdfs;
    bdfs="$1"
    bus_bdfs=""
        for bdf in $bdfs
        do
        if [ -e $pci_dev_path$bdf"/bus_name" ];then
            bus_name=`cat $pci_dev_path$bdf"/bus_name"`
            bus_bdfs=$bus_bdfs$bus_name" "
        fi
    done
    echo "$bus_bdfs"
}

remove_devs()
{
    local bdfs=$1
    local bdf=""

    for bdf in ${bdfs}
    do
        if echo ${bdf} | grep -Eq "^0{4}:\w{2}:\w{2}.2$"; then
            log "[INFO]remove dev: ${bdf}"
            echo 1 > ${pci_dev_path}${bdf}"/remove"

            bdf="${bdf/.2/.1}"
            log "[INFO]remove dev: ${bdf}"
            echo 1 > ${pci_dev_path}${bdf}"/remove"

            bdf=${bdf/.1/.0}
            log "[INFO]remove dev: ${bdf}"
            echo 1 > ${pci_dev_path}${bdf}"/remove"
        else
            log "[INFO]remove dev: ${bdf}"
            echo 1 > ${pci_dev_path}${bdf}"/remove"
        fi
    done

    return 0
}
master_bridge_do_hotreset()
{
    local bdf bus_bdfs
    bus_bdfs="$1"

    for bdf in $bus_bdfs
        do
            log "[INFO]set hot_reset bus(43): "$bdf
            setpci -s "$bdf" 3e.b=43
        done
    sleep 0.002
    for bdf in $bus_bdfs
        do
            log "[INFO]set hot_reset bus(03): "$bdf
            setpci -s "$bdf" 3e.b=03
        done
}
master_do_hotreset()
{
    local bdfs=$1
    local bdf=""

    for bdf in ${bdfs}
    do
        if echo ${bdf} | grep -Eq "^0{4}:\w{2}:\w{2}.2$"; then
            master_bridge_do_hotreset "$masterDevsBus"
            sleep 1
            log "[INFO].remove master device to redistribution bar address"
            remove_devs "$masterDevs"
            break
        else
            log "[INFO]hot reset dev: "${bdf}
            echo 1 > ${pci_dev_path}${bdf}"/reset"
            if [ $? -ne 0 ]; then
                log "[ERROR] hot reset failed"
                return 1
            fi
        fi
    done
}
rescan_bus()
{
        local bus_bdfs=$1
        local bdf
        for bdf in $bus_bdfs
        do
                log "[INFO]rescan bus: "$bdf
                echo 1 > $pci_dev_path$bdf"/rescan"
        done
}

get_devs_num()
{
    local bdf bdfs
    bdfs=$1
    i=0
        for bdf in $bdfs
        do
        ((i++))
        done
    echo $i
}

check_and_disable_sriov()
{
    local bus_bdfs=$1
    local bdf
    for bdf in $bus_bdfs
    do
        log "[INFO]check sriov support status: $bdf"
        lspci -vvvs $bdf | grep SR-IOV
        if [ $? -eq 0 ]; then
            vf_num=`cat $pci_dev_path$bdf/sriov_numvfs`
            if [ $vf_num != "0" ];then
                log "[INFO]disable sriov for "$bdf
                echo 0 > $pci_dev_path$bdf"/sriov_numvfs"
                if [ $? -ne 0 ]; then
                    log "[ERROR] disable sriov failed"
                    return 1
               fi
            else
                log "[INFO] sriov is disable："$bdf
            fi
        else
            log "[INFO]sriov is not support for "$bdf
        fi
    done
}

# Identify whether there is a davinci divided device that can be used in virtual machines and containers.
# Confirm the existence of divided devices by checking if the vdavinci device or uuid virtual machine files exist.
is_splited()
{
    local d
    ls /dev/vdavinci* >/dev/null 2>&1 && return 0
    for d in $(find /sys/devices/pci* -name mdev_supported_types);do
        ls $d/../davinci* >/dev/null 2>&1 || continue
        ls $d/*/devices/* >/dev/null 2>&1 && return 0
    done
    return 1
}

fn_unbind() {
    local allDevs="$1"
    # unbind
    for bdf in ${allDevs}
    do
        # set default state flag
        echo ${bdf} >  /sys/bus/pci/drivers/devdrv_device_driver/unbind && log "[INFO]bdf ${bdf} unbind success" || { log "[ERROR]bdf ${bdf} unbind failed" && return 1; }
    done
    log "[INFO]sleep 5s for unbind" && sleep 5
}

# 0: support hot reset.
# 1: not support hot reset.
is_support_hot_reset() {
    # support hot reset for all device number for 910_73
    lspci | grep -wq d803
    if [ $? -eq 0 ]; then
        log "[INFO]reset support for all device number."
        return 0
    fi

    log "[INFO]reset_dev_nums=[${reset_dev_nums}]."
    if [ "${reset_dev_nums}" = "${HOT_RESET_SUPPORT_ALL_CHIPS}" ]; then
        return 0
    elif echo "${devnum}" | grep -Ewq "${reset_dev_nums}"; then
        return 0
    else
        return 1
    fi
}

hot_reset()
{
    if is_splited;then
        log "[INFO]hot reset skip by splited"
        return
    fi
    if [ -e "$hotreset_status_file" ];then
        hotreset_status=`cat "$hotreset_status_file"`
        hotreset_status=${hotreset_status%.*}
    else
        hotreset_status="unknown"
    fi
     log "[INFO]hotreset_status:"$hotreset_status
    if [ "$hotreset_status" = "scan_success" ];then
        log "[INFO]hot reset has already executed"
        return
    fi

    log "[INFO]try hot reset device"
    echo "start" > "$hotreset_status_file"

    check_user_process >& /dev/null
        if [ $? -ne 0 ]; then
                log "[INFO]user process exist, no hot reset"
                echo "abort" > "$hotreset_status_file"
                return
        fi

    local allDevs=`get_bdfs "all"`
    log "[INFO]allDevs: $allDevs"
    local masterDevs=`get_bdfs "master"`
    log "[INFO]masterDevs: $masterDevs"
    local slaveDevs=`get_bdfs "slave"`
    log "[INFO]slaveDevs: $slaveDevs"

    local devnum=`get_devs_num "$allDevs"`
    if ! is_support_hot_reset; then
        log "[INFO]total dev num: "$devnum" not surport hotreset"
        echo "abort" > "$hotreset_status_file"
        return
    fi
    log "[INFO]all devnum: $devnum"

    local master_dev_num=`get_devs_num "$masterDevs"`
    local slave_dev_num=`get_devs_num "$slaveDevs"`
    log "[INFO]master devnum: $master_dev_num"
    log "[INFO]slave devnum: $slave_dev_num"
    if [ `expr $master_dev_num + $slave_dev_num` -ne $devnum ]; then
        log "[WARNING]The current number of devices does not meet the hot reset requirements"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    local masterDevsBus=`get_bridge_bus_bdfs "$masterDevs"`
    log "[INFO]masterDevsBus: $masterDevsBus"
    local slaveDevsBus=`get_bridge_bus_bdfs "$slaveDevs"`
    log "[INFO]slaveDevsBus: $slaveDevsBus"

    local busnum=`get_devs_num "$masterDevsBus"`
    if [ $busnum == 0 ]; then
        log "[INFO]master bus num: "$busnum" not surport hotreset"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    drvEcho "[INFO]device hot reset start"

    log "[INFO]1.set hotreset flag for bios"
    master_set_hotreset_flag "$masterDevs"

    log "[INFO]1.1 check sriov and disable sriov when enable"
    check_and_disable_sriov "$allDevs" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        log "[ERROR] check and disable sriov failed"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    log "[INFO]2.unbind all modules"
    fn_unbind "$allDevs"

    log "[INFO]3.remove old version modules ko"
    remove_all_ko
    if [ $? -ne 0 ]; then
        log "[WARNING]ko can not be removed"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    log "[INFO]4.remove slave devices avoid host reset"
    remove_devs "$slaveDevs"
    sleep 1

    log "[INFO]5.devices do hot reset"
    master_do_hotreset "$masterDevs"
    if [ $? -ne 0 ]; then
        log "[ERROR] master hot reset failed"
        return
    fi

    log "[INFO]6.remove master devices prepare for rescan"
    lspci | grep -wq d803
    if [ $? -eq 0 ]; then
        log "[INFO]6.1 sleep 5s before remove operation "
        sleep 5
    fi

    remove_devs "$masterDevs"
    sleep 1

    log "[INFO]7.rescan and wait all removed devices"
    local scan_devnum try_cnt
    scan_devnum=0
    try_cnt=0
    while [ $devnum != $scan_devnum ]
    do
        if (($try_cnt > 10)); then
            log "[WARNING]hot reset failed, all device num:"$devnum", rescan device num:"$scan_devnum
            echo "fail" > "$hotreset_status_file"
            return
        fi
        ((try_cnt++))

        sleep 1
        get_pci_info
        rescan_bus "$masterDevsBus"
        rescan_bus "$slaveDevsBus"
        allDevs=`get_bdfs "all"`
        scan_devnum=`get_devs_num "$allDevs"`

    done

    drvEcho "[INFO]device hot reset finish"
    log "[INFO]device hot reset finish"
    echo "scan_success."$(date +%s) > "$hotreset_status_file"
}

hot_reset

