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

## global variable area.
install_info="/etc/ascend_install.info"
install_info_key_array=("Driver_Install_Type" "UserName" "UserGroup" "Driver_Install_Path_Param" "Driver_Install_Mode" "Driver_Install_For_All")
log_file="/var/log/upgradetool/load_ube_mgmt_pack.log"

cmd="$1"
src_pack_name_path="$2"

log() {
    local content="$1"
    local cur_date=$(date +"%Y-%m-%d %H:%M:%S")

    echo "[${cur_date}] ${content}" >> $log_file
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

Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${install_info}")
ube_mgmt_pack_path="${Driver_Install_Path_Param}/driver/ube_mgmt"
ube_mgmt_pack_name_path="${ube_mgmt_pack_path}/UBEngine-mgmt-driver.tar.gz"

load_ube_mgmt_func()
{
    local file_size=0
    local max_file_size=20971520

    # judge src pack 
    if [ ! -f "${src_pack_name_path}" ]; then
        log "[ERROR]${src_pack_name_path} not exit."
        return 1;
    fi

    file_size=$(stat -c %s "${src_pack_name_path}")
    if [ "${file_size}" -gt "${max_file_size}" ]; then
        log "[ERROR]${src_pack_name_path} pack size over max pack zie. (file_size=${file_size}, max_file_size=${max_file_size})."
        return 1;
    fi

    if [ "${file_size}" -eq 0 ]; then
        log "[ERROR]${src_pack_name_path} pack size is 0. (file_size=${file_size})"
        return 1;
    fi

    if [ ! -d "${ube_mgmt_pack_path}" ]; then
        log "[ERROR]load ube mgmt pack dir not exit"
        return 1;
    fi

    #copy src pack to dst pack
    chattr -i ${ube_mgmt_pack_path} >& /dev/null
    chattr -i ${ube_mgmt_pack_name_path} >& /dev/null
    #tar -zxf after file name is: "${ube_mgmt_pack_path}/UBEngine-mgmt-driver.tar.gz"
    tar -zxf ${src_pack_name_path} --directory="${ube_mgmt_pack_path}/";ret=$?
    chattr +i ${ube_mgmt_pack_name_path} >& /dev/null
    chattr +i ${ube_mgmt_pack_path} >& /dev/null
    if [ ${ret} -ne 0 ]; then
        log "[ERROR]cp fail"
        return ${ret}
    fi

    log "[INFO]The ube_mgmt packet load success."
    return 0
}

unload_ube_mgmt_func()
{
    local ret=0

    if [ ! -f "${ube_mgmt_pack_name_path}" ]; then
        log "[INFO]${ube_mgmt_pack_name_path} not exit."
        return 0;
    fi

    chattr -i ${ube_mgmt_pack_path} >& /dev/null
    chattr -i ${ube_mgmt_pack_name_path} >& /dev/null
    rm -rf "${ube_mgmt_pack_name_path}";ret=$?
    chattr +i ${ube_mgmt_pack_path} >& /dev/null
    if [ ${ret} -ne 0 ]; then
        log "[ERROR]rm fail."
        return ${ret}
    fi

    log "[INFO]The ube_mgmt packet unload success."
    return 0
}

upgrade_ubde_mgmt_func() {
    local drv_upgrade_tool="${Driver_Install_Path_Param}/driver/tools/upgrade-tool"
    local pack_find="NULL"
    local ret=0

    # judge fixed file is ext
    if [ ! -f "${drv_upgrade_tool}" ]; then
        log "[ERROR]upgrade tool not exit"
        return 1
    fi

    if [ -f "${ube_mgmt_pack_name_path}" ]; then
        pack_find=${ube_mgmt_pack_name_path}
        log "[INFO]ube mgmt pck path name: ${pack_find}"
    fi

    # upgrade packet
    ${drv_upgrade_tool} --device_index -1 --upgrade_ube_mgmt_pack ${pack_find};ret=$?
    if [ ${ret} -ne 0 ]; then
        log "[ERROR]The ube_mgmt packet upgrade fail."
        return 1
    fi

    log "[INFO]The ube_mgmt packet upgrade success."
    return 0;
}

if [ ! -f "$log_file" ]; then
    touch ${log_file}
fi
chmod 600 ${log_file}

main_process() {
    local ret=0
    log "[INFO]load ube cmd: ${cmd}."

    if [ ${cmd} == "load" ]; then
        log "[INFO]load ube mgmt pack"
        load_ube_mgmt_func;ret=$?
    elif [ "$cmd" == "unload" ]; then
        log "[INFO]unload ube mgmt pack"
        unload_ube_mgmt_func;ret=$?
    elif [ ${cmd} == "upgrade" ]; then
        log "[INFO]upgrade ube mgmt pack"
        upgrade_ubde_mgmt_func;ret=$?
    else
        log "[INFO]load ube mgmt pack cmd invalid: ${cmd}"
        ret=1
    fi

    return ${ret}
}

########################################################
#
# main process
#
########################################################

main_process;ret=$?
exit ${ret}
