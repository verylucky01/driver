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

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] "$1 >> $logFile
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
        log "[ERROR]chmod ${mod} $file_path failed!"
        exit 1
    fi
}

createFile() {
    local _file=$1
    local _user=$2
    if [ -e "${_file}" ]; then
        rm -rf "${_file}"
    fi
    su - -s /bin/bash "${_user%:*}" -c "touch ${_file}"
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
            log "[INFO]the config format is unrecognized, line=${_line}"
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
        [ -e /sys/bus/pci/devices/${bdf}/physfn ] && continue
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
