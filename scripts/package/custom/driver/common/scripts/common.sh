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

# update "Driver_Install_Type" "UserName" "UserGroup" "Driver_Install_Path_Param" "Firmware_Install_Path_Param" "Firmware_Install_Type"
#        "Driver_Install_For_All" "Driver_Install_Mode"
install_info_key_array=("Driver_Install_Type" "UserName" "UserGroup" "Driver_Install_Path_Param" "Firmware_Install_Path_Param" "Firmware_Install_Type"
                        "Driver_Install_For_All" "Driver_Install_Mode")
updateInstallParam() {
    local _key="$1"
    local _val="$2"
    local _file="$3"

    if [ ! -f "${_file}" ]; then
        exit 1
    fi

    for key_param in "${install_info_key_array[@]}"; do
        if [ ${key_param} == ${_key} ]; then
            sed -i "/${_key}=/d" "${_file}"
            echo "${_key}=${_val}" >> "${_file}"
            break
        fi
    done
}

getInstallParam() {
    local _key="$1"
    local _file="$2"
    local _param

    if [ ! -f "${_file}" ];then
        exit 1
    fi

    # deal with abnormal install-cfg file.
    # when install-cfg file is abnormal, command 'grep' can not be used but 'sed' does.
    local normal_args
    grep -r "${_key}" "${_file}" | grep -q '='
    if [ $? -ne 0 ]; then
        normal_args=$(sed -n '/=/p' ${_file})
        echo "${normal_args}" > ${_file}
    fi

    for key_param in "${install_info_key_array[@]}"; do
        if [ ${key_param} == ${_key} ]; then
            _param=`grep -r "${_key}=" "${_file}" | cut -d"=" -f2-`
            break;
        fi
    done
    echo "${_param}"
}

setFileChmod() {
    local option="$1"
    local mod="$2"
    local file_path="$3"

    if [ ${input_install_for_all} == y ]; then
        local mod=${mod:0:-1}${mod: -2:1}
    fi

    chmod ${option} "${mod}" "${file_path}" >& /dev/null
    if [ $? -ne 0 ]; then
        echo "[ERROR]chmod ${mod} $file_path failed!"
        exit 1
    fi
}

createFile() {
    local _file=$1
    if [ -e "${_file}" ]; then
        rm -f "${_file}"
    fi
    touch "${_file}"
    chown -h "$2" "${_file}"
    setFileChmod -f "$3" "${_file}"
    if [ $? -ne 0 ]; then
        return 1
    fi
    return 0
}

megerConfig() {
    local _old_file=$1
    local _new_file=$2
    local _tmp

    if [ "${_old_file}" == "${_new_file}" ]; then
        return 1
    fi
    if [ ! -f "${_old_file}" ] || [ ! -f "${_new_file}" ]; then
        return 1
    fi

    diff "${_old_file}" "${_new_file}" > /dev/null
    if [ $? -eq 0 ]; then
        return 0 # old file content equal new file content
    fi

    # check file permission
    if [ ! -r "${_old_file}" ] && [ ! -w "${_new_file}" ]; then
        return 1
    fi

    cp -f "${_old_file}" "${_old_file}.old"
    cp -f "${_new_file}" "${_old_file}"
    _new_file="${_old_file}"
    _old_file="${_old_file}.old"

    while read _line; do
        _tmp=`echo "${_line}" | sed "s/ //g"`
        if [ x"${_tmp}" == "x" ]; then
            continue # null line
        fi
        _tmp=`echo "${_tmp}" | cut -d"#" -f1`
        if [ x"${_tmp}" == "x" ]; then
            continue # the line is comment
        fi

        _tmp=`echo "${_line}" | grep "="`
        if [ x"${_tmp}" == "x" ]; then
            continue
        fi

        local _key=`echo "${_line}" | cut -d"=" -f1`
        local _value=`echo "${_line}" | cut -d"=" -f2-`
        if [ x"${_key}" == "x" ]; then
            echo "the config format is unrecognized, line=${_line}"
            continue
        fi
        if [ x"${_value}" == "x" ]; then
            continue
        fi
        # replace config value to new file
        sed -i "/^${_key}=/c ${_key}=${_value}" "${_new_file}"
    done < "${_old_file}"

    rm -f "${_old_file}"
}

get_pci_info() {
    local bdf="" i=0

    g_dev_nums=0
    g_bdfs=()

    for bdf in $(ls /sys/bus/pci/devices/)
    do
        cat /sys/bus/pci/devices/${bdf}/vendor 2>/dev/null | grep -Ewq "${vendor_bdf}" || continue
        cat /sys/bus/pci/devices/${bdf}/device 2>/dev/null | grep -Ewq "${device_bdf}" || continue
        # 0x0110: it means that the chip is recognized as 1 on the OS PCI bus but 2 on the device management driver.
        cat /sys/bus/pci/devices/${bdf}/subsystem_device 2>/dev/null | grep -wq "0x0110" && g_dev_nums=$(expr ${g_dev_nums} + 1)
        g_dev_nums=$(expr ${g_dev_nums} + 1)
        # to get all bdfs
        g_bdfs[${i}]="${bdf}"
        i=$(expr ${i} + 1)
    done
    log "[INFO]g_dev_nums=[${g_dev_nums}] and g_bdfs=[${g_bdfs[*]}]"

    return 0
}

checkUserDocker() {
    local dev_files="/dev/davinci_manager /dev/devmm_svm /dev/hisi_hdc"
    local dev_files2=`find /dev -name "davinci[0-9]*"`
    local davinci_files=(${dev_files[*]} ${dev_files2[*]})
    local f_file
    local docker_dev
    local docker_list
    for f_file in $davinci_files
    do
        docker >> /dev/null 2>&1
        if [ $? -eq 0 ];then
            docker_list=`docker ps -q 2> /dev/null`
            if [ -n "${docker_list}" ]; then
                docker_dev=`docker inspect $(docker ps -q) | grep "$f_file"`
                if [ -n "${docker_dev}" ]; then
                    log "[WARNING]"$f_file" has used by docker"
                    return 1
                fi
            fi
        fi
    done
    return 0
}

# 1.check whether any process is still calling the kernel-mode ko based on the character-device of the driver,
#   if it is, the function will return 1, otherwise, it will return 0.
# 2.check whether the character-device of the driver is used in Docker based on the Docker image information,
#   if it is, the function will return 1, otherwise, it will return 0.
checkUserProcess() {
    local dev_files="/dev/davinci_manager /dev/devmm_svm /dev/hisi_hdc"
    local dev_files2=`find /dev -name "davinci[0-9]*"`
    local davinci_files=(${dev_files[*]} ${dev_files2[*]})
    local f_file
    local apps
    local app
    local docker_dev
    local docker_list
    if which fuser >& /dev/null; then
        for f_file in $davinci_files
        do
            apps=`fuser -uv "$f_file" 2> /dev/null`
            if [ -n "${apps}" ]; then
                log "[WARNING]"$f_file" has user process: ""$apps"
                for app in $apps
                do
                    ps -ef| grep -w "$app"| grep -v "grep" >> $logFile
                done
                return 1
            fi
        done
    else
        for f_file in $davinci_files
        do
            apps=$(ls -l /proc/*/fd | egrep "fd:|"$f_file"" | grep "$f_file" -B1 | grep "fd:" | sed 's#/fd:##;s#/proc/##' | head)
            if [ -n "${apps}" ]; then
                log "[WARNING]"$f_file" has user process: ""$apps"
                for app in $apps
                do
                    ps -ef| grep -w "$app"| grep -v "grep" >> $logFile
                done
                return 1
            fi
        done
    fi
    checkUserDocker || return 1 
    return 0
}

checkPidNameMatch() {
    local pid
    local name
    local proc_name

    pid=$1;
    name=$2;

    if [[ ! -d "/proc/$pid" ]]; then
        log "[INFO] this pid ${pid} is dead"
        return 1
    fi

    proc_name=$(cat /proc/$pid/status | grep '^Name:' | sed 's/^Name:[[:space:]]*//')
    log "[INFO] proc_name ${proc_name} and name ${name}"
    if [[ "$proc_name" != "$name" ]]; then
        log "[INFO] proc_name ${proc_name} and name ${name} are not matched"
        return 1
    fi

    log "[INFO] pid ${pid} and name ${name} are matched"
    return 0
}

killCurrentProcess(){
    local device
    local result
    local pid
    local name
    local max_retries=3

    pid=$1;
    name=$2;
    device="/dev/davinci_manager"

    result=$(lsof "$device" 2>/dev/null | grep "$pid")
    if [[ -n "${result}" ]]; then
        log "[INFO]: this pid ${pid} is occupy npu"
    else
        log "[INFO]: this pid ${pid} isn't occupy npu"
        return 0
    fi

    for ((i = 1; i <= max_retries; i++)); do
        kill -0 "${pid}" 2>/dev/null
        if [[ $? -ne 0 ]]; then
            log "[INFO] process already dead: ${name} (PID: ${pid})"
            return 0
        fi

        kill -9 "${pid}" 2>/dev/null
        if [[ $? == 0 ]]; then
            log "[INFO] successfully to send SIGKILL: ${name} (PID: ${pid})"
            sleep 2
            break
        else
            log "[WARNING]killed process ${name} (PID: ${pid}) fail"
            # 重试
            if [[ $i == $max_retries ]]; then
                log "[WARNING] failed to kill after ${max_retries} attempts: ${name} (PID: ${pid})"
            else
                log "[INFO] retrying... attempt $i"
                sleep 2
            fi
        fi
    done

    kill -0 "${pid}" 2>/dev/null
    if [[ $? == 0 ]]; then
        kill -9 "${pid}" 2>/dev/null
        if [[ $? == 0 ]]; then
            log "[INFO] successfully to send SIGKILL again: ${name} (PID: ${pid})"
            sleep 2
        fi
    fi

    kill -0 "${pid}" 2>/dev/null
    if [[ $? == 0 ]]; then
        log "[WARNING]the process ${name} (PID: ${pid}) is still alive"
        return 1
    fi
    return 0
}

killWhiteProcess(){
    local cfg_file
    local i
    local line
    local name
    local pid
    local isA2_driver
    local isA3_driver

    isA2_driver=$(echo $device_bdf | grep d802)
    isA3_driver=$(echo $device_bdf | grep d803)
    if [[ -z "$isA2_driver" && -z "$isA3_driver" ]]; then
        log "[INFO] this type driver need not kill white process"
        return 0
    fi

    cfg_file="/etc/custom_process.cfg"
    if [[ ! -f "${cfg_file}" ]]; then
        log "[INFO]config file not exist: ${cfg_file}"
        return 0
    fi

    if [[ ! -r "${cfg_file}" ]]; then
        log "[WARNING] can't read config: ${cfg_file}"
        return 1
    fi

    while IFS= read -r line || [[ -n "${line}" ]]; do
        line="${line//[[:space:]]/}"
        [[ -z "${line}" || "${line}" =~ ^# ]] && continue
        if [[ "${line}" =~ ^AllowKilledWhenResetNPU:[^:]+:[0-9]+$ ]]; then
            name=$(echo "${line}" | cut -d':' -f2)
            pid=$(echo "${line}" | cut -d':' -f3)
            if ! [[ "${pid}" =~ ^[0-9]+$ ]]; then
                log "[INFO]: pid is invalid ${line}"
                continue
            fi

            checkPidNameMatch ${pid} ${name}
            if [[ $? != 0 ]]; then
                continue
            fi

            killCurrentProcess ${pid} ${name}
            if [[ $? != 0 ]]; then
                log "[WARNING] failed to kill this process ${name} (PID ${pid})"
                continue
            fi
        else
            log "[INFO]invalid string: ${line}"
        fi
    done < <(head -n 20 "${cfg_file}")

    return 0
}

setHotResetFlag() {
    local new_path="${Driver_Install_Path_Param%/}"
    local tool_path="$new_path/driver/tools/upgrade-tool"
    local cnt=0

    killWhiteProcess
    if [[ $? -ne 0 ]]; then
        log "[WARNING]kill White Process fail"
    fi

    [ -e "${tool_path}" ] || return 0
    #Anti-jitter 3 times
    log "[INFO]Start to set the hot reset flag for all devices."
    while [[ ${cnt} -lt 3 ]]
    do
        cnt=$(($cnt+1))
        #Only check keyword fail
        #The old tool does not support this feature, and the setting behavior will be skipped.
        timeout 20s ${tool_path} --device_index -1 --hot_reset_flag | grep fail >> /dev/null 2>&1
        if [ $? -ne 0 ]; then
            log "[INFO]The hot reset flag has been set."
            return 0
        fi
        log "[WARNING]The set hot reset flag failed to take effect."
        checkUserProcess >& /dev/null
        sleep 1
    done
    return 1
}

checknoChroot() {
    if [ -e /proc/1/root/ ]; then
        # refer to systemctl
        device_proc=$(stat /proc/1/root/ |grep Device | grep Inode|awk '{print $2}')
        inode_proc=$(stat /proc/1/root/ |grep Device | grep Inode|awk '{print $4}')
        device_root=$(stat / |grep Device | grep Inode|awk '{print $2}')
        inode_root=$(stat / |grep Device | grep Inode|awk '{print $4}')
        if [ "$device_proc"x = "$device_root"x -a  "$inode_proc"x = "$inode_root"x ]; then
            return 0

        else
            return 1
        fi
    else
        if [ "$(ls -di /)" = "2 /" ]; then
            return 0
        else
            return 1
        fi
    fi
}

getKernelVer() {
    if checknoChroot; then
        uname -r
    else
        # chroot env
        which rpm >/dev/null 2>&1
        if [ $? -eq 0 ]; then
            rpm -qa | grep ^kernel-[0-9] | tail -1 | sed 's/kernel-//g'
            return 0
        fi
        which dpkg >/dev/null 2>&1
        if [ $? -eq 0 ]; then
            dpkg --list|grep linux-image|head -1|awk '{print $2}'| sed 's/linux-image-//g'
            return 0
        fi
    fi
}