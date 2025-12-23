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

install_info="/etc/ascend_install.info"
install_info_key_array=("Driver_Install_Type" "UserName" "UserGroup" "Driver_Install_Path_Param" "Driver_Install_Mode" "Driver_Install_For_All")
log_file="/var/log/device_boot_init.log"

dev_nums=0
arg1_devid=$1
g_lock_file=""

log() {
    local pkg_type="$1" content="$2"
    local cur_date=$(date +"%Y-%m-%d %H:%M:%S")

    echo "[${pkg_type}] [${cur_date}] ${content}" >> $log_file
}

getInstallParam() {
    local _key="$1"
    local _file="$2"
    local _param

    if [ ! -f "${_file}" ];then
        exit 1
    fi

    for key_param in "${install_info_key_array[@]}"; do
        if [ ${key_param} == ${_key} ]; then
            _param=`grep -r "${_key}=" "${_file}" | cut -d"=" -f2-`
            break;
        fi
    done
    echo "${_param}"
}

network_cfg_recovery() {
    if [ "${Driver_Install_Mode}" = "vnpu_guest" ]; then
        log "aicpu" "[INFO][${arg1_devid}]The Driver_Install_Mode is vnpu_guest and skip."
        return 0
    fi

    log "aicpu" "slot_id is ${1}, hccn_tool recovery"
    nohup "${install_path}"/driver/tools/hccn_tool -i ${1} -cfg recovery > /dev/null 2>&1 &
}

get_pci_info() {
    local bdf="" bdfs=() i=0

    dev_nums=0
    for bdf in $(ls /sys/bus/pci/devices/)
    do
        cat /sys/bus/pci/devices/${bdf}/vendor 2>/dev/null | grep -Ewq "${vendor_bdf}" || continue
        cat /sys/bus/pci/devices/${bdf}/device 2>/dev/null | grep -Ewq "${device_bdf}" || continue
        [ -e /sys/bus/pci/devices/${bdf}/physfn ] && continue
        # 0x0110: it means that the chip is recognized as 1 on the OS PCI bus but 2 on the device management driver.
        cat /sys/bus/pci/devices/${bdf}/subsystem_device 2>/dev/null | grep -wq "0x0110" && dev_nums=$(expr ${dev_nums} + 1)
        dev_nums=$(expr ${dev_nums} + 1)
        # to get all bdfs
        bdfs[${i}]="${bdf}"
        i=$(expr ${i} + 1)
    done
    log "driver_sph" "[INFO][${arg1_devid}]dev_nums=[${dev_nums}] and bdfs=[${bdfs[*]}]"

    return 0
}

modules_load_check() {
    local args_val=$@
    local module_name=""

    for module_name in ${args_val}
    do
        # drv_vascend may not load.
        [ "${module_name}" = "drv_vascend" ] && continue

        if ! lsmod | grep -wq "${module_name}" >& /dev/null; then
            log "driver_sph" "[INFO][${arg1_devid}]The [${module_name}] is unloaded."
            return 1
        fi
    done

    log "driver_sph" "[INFO][${arg1_devid}]The kernel modules load success."
    return 0
}

wait_for_modules_loading() {
    local kernel_modules="" check_times=60

    if [ "${Driver_Install_Mode}" = "normal" ]; then
        kernel_modules="$(echo ${mode_normal} | awk -F [\|] '{for(i=1;i<NF;i=i+1) print $i}')"
    elif [ "${Driver_Install_Mode}" = "debug" ]; then
        kernel_modules="$(echo ${mode_debug} | awk -F [\|] '{for(i=1;i<NF;i=i+1) print $i}')"
    elif [ "${Driver_Install_Mode}" = "vnpu_host" ]; then
        kernel_modules="$(echo ${mode_vnpu_host} | awk -F [\|] '{for(i=1;i<NF;i=i+1) print $i}')"
    else
        kernel_modules="$(echo ${mode_vnpu_docker_host} | awk -F [\|] '{for(i=1;i<NF;i=i+1) print $i}')"
    fi

    # it will wait 60s for kerne-modules' loading.
    for i in $(seq 1 ${check_times})
    do
        log "driver_sph" "[INFO][${arg1_devid}]It is [$i] times for wait_for_modules_loading."
        if modules_load_check ${kernel_modules}; then
            break
        fi
        sleep 1
    done

    if [ $i -eq 60 ]; then
        log "driver_sph" "[INFO][${arg1_devid}]modules_load_check timed out."
        return 1
    fi

    log "driver_sph" "[INFO][${arg1_devid}]wait_for_modules_loading success."
    return 0
}

is_device_startup()#@ phy_devid
{
    local drv_upgrade_tool="${Driver_Install_Path_Param}/driver/tools/upgrade-tool"
    local davinci_ok_count

    [ -e /dev/davinci${arg1_devid} ] || { log "driver_sph" "[INFO][${arg1_devid}] /dev/davinci${arg1_devid} not exist.";return 1;}

    get_pci_info
    davinci_ok_count=$({ timeout 8s "${drv_upgrade_tool}" --device_index -1 --boot_status 2>/dev/null;}|grep "boot status:16"|wc -l)
    if [ ${dev_nums} -ne ${davinci_ok_count} ]; then
        log "driver_sph" "[INFO][${arg1_devid}]wait device start: ${dev_nums} not equal ${davinci_ok_count}."
        return 1
    fi

    log "driver_sph" "[INFO][${arg1_devid}]wait device start done, device count: ${dev_nums}."
    return 0
}

wait_for_single_device_ready() {
    local drv_upgrade_tool="${Driver_Install_Path_Param}/driver/tools/upgrade-tool"
    local i=0 max=32

    if [ ! -f "${drv_upgrade_tool}" ]; then
        log "driver_sph" "[INFO][${arg1_devid}]The [${drv_upgrade_tool}] doesn't exist and skip here."
        return 0
    fi

    # it will wait 5*32=160s for all devices booting into OS.
    # the wait-time is same as firmware-upgrade.
    for i in $(seq 1 ${max})
    do
        # if first times sleep 5s here, it will get reliable dev_nums.
        sleep 5
        log "driver_sph" "[INFO][${arg1_devid}]It is [$i] times for wait_for_single_device_ready."
        if is_device_startup; then
            log "driver_sph" "[INFO][${arg1_devid}]wait_for_single_device_ready success."
            return 0
        fi
    done

    log "driver_sph" "[WARNING][${arg1_devid}]wait_for_single_device_ready timed out."
    return 1
}

load_livepatch() {
    local newest_livepatch_version="" load_livepatch_sh=""

    # if the vf goes online, it will skip here.
    if [ ${arg1_devid} -ge 100 ]; then
        log "driver_sph" "[INFO][${arg1_devid}]The livepatch doesn't support on VF."
        return 0
    fi

    if [ "${Driver_Install_Type}" = "docker" -o "${Driver_Install_Type}" = "devel" ]; then
        log "driver_sph" "[INFO][${arg1_devid}]The Driver_Install_Type is not full or run and skip livepatch loading."
        return 0
    fi

    if [ "${Driver_Install_Mode}" = "vnpu_guest" ]; then
        log "driver_sph" "[INFO][${arg1_devid}]The Driver_Install_Mode is vnpu_guest and skip livepatch loading."
        return 0
    fi

    newest_livepatch_version="$(grep -v "^$" "${Driver_Install_Path_Param}/driver/livepatch/livepatch_install.info" 2>/dev/null | sed -n "$"p)"
    if [ -z "${newest_livepatch_version}" ]; then
        log "driver_sph" "[INFO][${arg1_devid}]The livepatch_install.info is empty and skip livepatch loading."
        return 0
    fi

    load_livepatch_sh="${Driver_Install_Path_Param}/driver/script/livepatch_sys_init.sh"
    if [ ! -e "${load_livepatch_sh}" ]; then
        log "driver_sph" "[INFO][${arg1_devid}]The [${load_livepatch_sh}] doesn't exist."
        return 0
    fi

    # it loads for modules_load_check and device numbers.
    if [ ! -f "${Driver_Install_Path_Param}/driver/script/specific_func.inc" ]; then
        log "driver_sph" "[INFO][${arg1_devid}]The file specific_func.inc doesn't exist."
        return 0
    fi
    . "${Driver_Install_Path_Param}/driver/script/specific_func.inc"

    if ! wait_for_modules_loading; then
        return 1
    fi

    wait_for_single_device_ready

    g_lock_file="${Driver_Install_Path_Param}/driver/livepatch/${newest_livepatch_version}/script/rmmod_insmod_lock"
    # -E: conflict-exit-code; -n: nonblock;
    # if the device is first to start, it will sleep 18s here and other devices will succeed directly.
    flock -E 3 -n ${g_lock_file} -c "sleep 18;${load_livepatch_sh} host install ${Driver_Install_Path_Param}/driver/livepatch/${newest_livepatch_version}";ret=$?
    if [ ${ret} -eq 0 -o ${ret} -eq 3 ]; then
        log "driver_sph" "[INFO][${arg1_devid}]ret=[${ret}]The [${newest_livepatch_version}] livepatch load success."
        return 0
    fi

    log "driver_sph" "[ERROR][${arg1_devid}]ret=[${ret}]Failed to load livepatch [${newest_livepatch_version}]."
    return 1
}

########################################################
#
# main process
#
########################################################

if [ ! -f "$log_file" ]; then
    touch ${log_file}
fi
chmod 600 ${log_file}

Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${install_info}")
Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${install_info}")
Driver_Install_Mode=$(getInstallParam "Driver_Install_Mode" "${install_info}")
install_path="${Driver_Install_Path_Param}"

log "aicpu" "<<<<<<<< device_${1} boot init >>>>>>>>"
log "aicpu" "device boot init start, physical id: $1, ip_addr: 127.0.0.1, user: root"

# restore network cfg
if [ -f "${install_path}"/driver/tools/hccn_tool ];then
	log "aicpu" "<<<<<<<< device_${1} network cfg recovery start >>>>>>>>"
	network_cfg_recovery ${1}
	log "aicpu" "<<<<<<<< device_${1} network cfg recovery end >>>>>>>>"
fi

load_livepatch &

log "driver_sph" "[INFO][${arg1_devid}]The loading of livepatch will run in the background."

# there will be extrend contents for compute-group in follow.
