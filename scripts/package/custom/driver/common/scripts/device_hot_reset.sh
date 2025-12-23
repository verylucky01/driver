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
NPU_LOG_CFG="/etc/npu_dev_syslog.cfg"
installInfo="/etc/ascend_install.info"
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
COMMON_SHELL="$SHELL_DIR/common.sh"
ASCEND_910A_BDF="19e5:d801"
ASCEND_910B_NUM=`lspci | grep -c d802`
ASCEND_910_93_NUM=`lspci | grep -c d803`
# load common.sh, get install.info
source "${COMMON_SHELL}"
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
Driver_Install_Mode="$(getInstallParam "Driver_Install_Mode" "${installInfo}")"

if [ -f "${Driver_Install_Path_Param}"/driver/script/specific_func.inc ];then
    . "${Driver_Install_Path_Param}"/driver/script/specific_func.inc
elif [ -f  "${PWD}"/driver/script/specific_func.inc ];then
    . ./driver/script/specific_func.inc
fi
log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    user_id=`id | awk '{printf $1}'`
    echo "[Driver] [$cur_date] [$user_id] "$1 >> $logFile
}
drvEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}

# get modules from specific_func.inc
remove_all_ko()
{
    local module modules msg
    has_tc_pcidev=$(lsmod | grep tc_pcidev)
    has_npu_peermem=$(lsmod | grep npu_peermem)

    case "${Driver_Install_Mode}" in
        normal)             modules="${mode_normal//|/ }";;
        debug)              modules="${mode_debug//|/ }";;
        vnpu_host)          modules="${mode_vnpu_host//|/ }";;
        vnpu_docker_host)   modules="${mode_vnpu_docker_host//|/ }";;
        *)
            log "[ERROR] Unsupported installation mode"
            return 1
            ;;
    esac
    for module in ${modules// drv_seclib_host/}
    do
        if [ "$module" = "tc_pcidev" ] && [ -z "$has_tc_pcidev" ]; then
            log "[WARNING]Module tc_pcidev is not currently loaded"
            continue
        fi

        if [ "$module" = "npu_peermem" ] && [ -z "$has_npu_peermem" ]; then
            continue
        fi
        msg=$(rmmod $module 2>&1)
        if [ $? -ne 0 ];then
            if [ "$module" != "host_notify" ] && [ "$module" != "drv_vascend" ] && [ "$module" != "tc_pcidev" ] && [ "$module" != "ascend_kernel_open_adapt" ];then
                log "[ERROR]$msg"
                log "[ERROR]rmmod $module failed"
                return 1
            fi
            if [ "$module" = "tc_pcidev" ];then
                log "[ERROR]$msg"
                log "[ERROR]rmmod $module failed"
                log "[WARNING]ko can not be removed"
                echo "ko_abort" > "$hotreset_status_file"
            fi
        fi
        if [ -e "$hotreset_status_file" ];then
            hotreset_status=`cat "$hotreset_status_file"`
            hotreset_status=${hotreset_status%.*}
        fi
        if [ "$hotreset_status" != "ko_abort" ];then
            log "[INFO]rmmod $module success"
        fi
    done

    lsmod | grep "drv_seclib_host" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        log "[INFO]rmmod drv_seclib_host"
        rmmod drv_seclib_host >& /dev/null || { log "[ERROR]rmmod failed" && return 1; }
    fi

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
    local bdf bdfs

    bdfs=""
    get_pci_info
    for bdf in ${g_bdfs[*]}
    do
        if [ "$find_type" == "all" ];then
            bdfs=$bdfs$bdf" "
        fi
        if [ "$find_type" == "master" ];then
            if [ -e $pci_dev_path$bdf"/chip_id" ];then
                chip_id=`cat $pci_dev_path$bdf"/chip_id"`
                if [ "$chip_id" == "0" ];then
                    bdfs=$bdfs$bdf" "
                fi
            fi
                fi
        if [ "$find_type" == "slave" ];then
                        if [ -e $pci_dev_path$bdf"/chip_id" ];then
                                chip_id=`cat $pci_dev_path$bdf"/chip_id"`
                                if [ "$chip_id" != "0" ];then
                                        bdfs=$bdfs$bdf" "
                                fi
                        fi
                fi
    done

    echo "$bdfs"

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

check_exist_reset_file()
{
    local bdf bus_bdfs
    bus_bdfs="$1"

    for bdf in $bus_bdfs
    do
        if [ ! -e ${pci_dev_path}${bdf}"/reset" ]; then
            log "[WARNING]${pci_dev_path}${bdf}/reset is not exist"
            return 1
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
        fi
    done
}

find_devctl_cap() {
    local device=$1
    local last_off=0x40
    local id_and_next cap_id next_ptr
 
    while true; do
        id_and_next=$(setpci -s "${device}" "${last_off}.w")
        cap_id=$(printf "0x%02x\n" $((0x${id_and_next} & 0xFF)))
        next_ptr=$(printf "0x%02x\n" $(((0x${id_and_next} >> 8) & 0xff)))
        if [[ ${cap_id} == 0x10 ]]; then  # Devctl Capability ID
            echo ${last_off}
            return 0
        fi
        last_off=${next_ptr}
        (( 0x40 <= last_off && last_off < 0x100 )) || break
    done

    return 1
}

set_slave_bus_link_disable() {
    local init_val
    local set_val
    local bdf
    local pcie_offset
    local link_ctrl_offset
    local link_ctrl=0x10   # Link Control
 
    for bdf in $@; do
        pcie_offset=$(find_devctl_cap ${bdf}) || return 1
        link_ctrl_offset=$(printf "0x%x\n" $(( pcie_offset + ${link_ctrl} )))
        init_val=$(setpci -s "${bdf}" "${link_ctrl_offset}.w")
        set_val=$(printf "0x%02x\n" $((0x${init_val} | 0x10))) # lnkctl bit 4 置1
        setpci -s "${bdf}" "${link_ctrl_offset}.w=${set_val}" # 禁用端口，断开下游设备
        log "[INFO]set ${bdf} register ${link_ctrl_offset}=${set_val}"
    done
}

set_slave_bus_link_enable() {
    local init_val
    local set_val
    local bdf
    local pcie_offset
    local link_ctrl_offset
    local link_ctrl=0x10   # Link Control
 
    for bdf in $@; do
        pcie_offset=$(find_devctl_cap ${bdf}) || return 1
        link_ctrl_offset=$(printf "0x%x\n" $(( pcie_offset + ${link_ctrl} )))
        init_val=$(setpci -s "${bdf}" "${link_ctrl_offset}.w")
        set_val=$(printf "0x%02x\n" $((0x${init_val} & 0xef))) # lnkctl bit 4 清零
        setpci -s "${bdf}" "${link_ctrl_offset}.w=${set_val}" #启用端口，下游设备链路恢复
        log "[INFO]set ${bdf} register ${link_ctrl_offset}=${set_val}"
    done
}

rescan_bus()
{
    local bus_bdfs=$1
    local bdf
    for bdf in $bus_bdfs
    do
        log "[INFO]rescan bus: "$bdf
        if [ -e $pci_dev_path$bdf"/rescan" ]; then
            echo 1 > $pci_dev_path$bdf"/rescan"
        else
            echo 1 > $pci_dev_path$bdf"/dev_rescan"
        fi
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

check_stop_devlog_daemon() {
    if ! grep -wq 'syslog_.*_mode:enable' ${NPU_LOG_CFG} &>/dev/null; then
        return
    fi

    local pids=$(ps -ef | awk '/msnpureport report --permanent/&&!/awk/{print $2}')
    if (( ${#pids[@]} > 0 )); then
        log "[INFO] There are device log daemons running, kill them"
        kill -9 ${pids[@]} &> /dev/null
    else
        log "[INFO] There is no device log daemon running"
    fi
}

hot_reset()
{
    if is_splited; then
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

    check_stop_devlog_daemon

    checkUserProcess >& /dev/null
    if [ $? = "1" ]; then
        log "[INFO]user process exist, no hot reset"
        echo "abort" > "$hotreset_status_file"
        return
    fi
    if [ -d "/sys/bus/mdev/devices/" ] && [ "$(ls -A /sys/bus/mdev/devices/)" ]; then
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
    if [ "$pcie_bdf"x = "$ASCEND_910A_BDF"x ] || [ $ASCEND_910B_NUM -ne 0 ] || [ $ASCEND_910_93_NUM -ne 0 ]; then
        log "[INFO]devnum: $devnum"
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
    fi

    check_exist_reset_file "$allDevs"
    if [ $? = "1" ]; then
        echo "fail" > "$hotreset_status_file"
        return
    fi

    drvEcho "[INFO]device hot reset start"

    log "[INFO]1.set hotreset flag for bios"
    master_set_hotreset_flag "$masterDevs"

    log "[INFO]1.1 check sriov and disable sriov when enable"
    check_and_disable_sriov "$masterDevs" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        log "[ERROR] check and disable sriov failed"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    local allDevs=`get_bdfs "all"`
    local masterDevs=`get_bdfs "master"`
    local slaveDevs=`get_bdfs "slave"`
    local devnum=`get_devs_num "$allDevs"`
    local machine=$(uname -m)

    log "[INFO]2.unbind all modules"
    fn_unbind "$allDevs"

    log "[INFO]3.remove old version modules ko"
    remove_all_ko
    if [ $? = "1" ]; then
        log "[WARNING]ko can not be removed"
        echo "ko_abort" > "$hotreset_status_file"
        return
    fi

    if [ "$pcie_bdf"x = "$ASCEND_910A_BDF"x ] || [ $ASCEND_910B_NUM -ne 0 ] || [ $ASCEND_910_93_NUM -ne 0 ]; then
        log "[INFO]4.remove slave devices avoid host reset"
        remove_devs "$slaveDevs"
        sleep 1

        if [ "$machine"x == "x86_64"x ] && [ $ASCEND_910_93_NUM -ne 0 ]; then
            log "[INFO]set slave bus link disable:$slaveDevsBus"
            set_slave_bus_link_disable "$slaveDevsBus"
	        sleep 1
        fi
        log "[INFO]5.devices do hot reset"
        master_do_hotreset "$masterDevs"
        if [ "$machine"x == "x86_64"x ] && [ $ASCEND_910_93_NUM -ne 0 ]; then
            log "[INFO]set slave bus link enable:$slaveDevsBus"
            set_slave_bus_link_enable "$slaveDevsBus"
            sleep 1
        fi
        sleep 2
    else
        log "[INFO]5.devices do hot reset"
        master_do_hotreset "$allDevs"
        sleep 2
    fi

    if [ $ASCEND_910B_NUM -ne 0 ] || [ $ASCEND_910_93_NUM -ne 0 ]; then
        log "[INFO]6.remove master devices prepare for rescan"
        remove_devs "$masterDevs"
        sleep 1
    fi

    lspci | grep -wq d803
    if [ $? -eq 0 ]; then
        log "[INFO]6.1 sleep 5s before remove operation "
        sleep 5
    fi

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
        if [ "$pcie_bdf"x = "$ASCEND_910A_BDF"x ] || [ $ASCEND_910B_NUM -ne 0 ] || [ $ASCEND_910_93_NUM -ne 0 ]; then
            rescan_bus "$masterDevsBus"
            rescan_bus "$slaveDevsBus"
        fi
        allDevs=`get_bdfs "all"`
        scan_devnum=`get_devs_num "$allDevs"`

    done

    if [ -e "$hotreset_status_file" ];then
        hotreset_status=`cat "$hotreset_status_file"`
        hotreset_status=${hotreset_status%.*}
    fi
    if [ "$hotreset_status" = "ko_abort" ];then
        return
    fi

    drvEcho "[INFO]device hot reset finish"
    log "[INFO]device hot reset finish"
    echo "scan_success."$(date +%s) > "$hotreset_status_file"
}

hot_reset