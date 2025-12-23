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
# log
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
# if host, read from /boot/config-$(uname -r).
g_config_livepatch="y"
# execute side: host/device
g_sh_exec_side=""
# operate: install/uninstall
g_sh_opt_action=""
# current patch version path
g_livepatch_absolute_path=""
# current script name.
g_current_sh_name="$(basename $0)"
# local driver install path.
g_local_drv_root_path=""
# all livepatch kernel modules.
g_livepatch_kernel_modules=""
# upatch dir include all upatch sub dirs.
g_upatch_extract_dir="/var/live_patch/upatches"
g_upatch_active_dir="/usr/lib/syscare/patches"
# patch version file
g_extract_version_path="/var/live_patch/version.info"
g_active_version_path="/var/live_patch/active_version.info"
# syscared log
g_syscared_log_dir="/var/log/syscare/"
g_syscared_cur_log_file_name="syscared_rCURRENT.log"

## function area
log() {
    cur_date=$(date +"%Y-%m-%d %H:%M:%S")
    echo "[Driver_sph] [${cur_date}] [${g_current_sh_name}] $1" >> ${logFile}
}

spc_color_echo() {
    local cur_date="$(date +"%Y-%m-%d %H:%M:%S")"
    echo -e "[Driver_sph] [${cur_date}] $1"
}

########################################################
# Function: Eop_check
# Description: escalation of privileges check.
# Parameter:
#     input:
#     $1 -- $@
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
Eop_check() {
    local sh_args=("$@") args_num=$#
    local reg_exec_side='(host|device)' reg_opt_action='(install|uninstall)' reg_absolute_path='(/(.+))+'
    local exec_cmd=("${reg_exec_side}" "${reg_opt_action}" "${reg_absolute_path}")
    local i=0 max_index=$(expr ${#exec_cmd[*]} - 1)

    if [ ${args_num} -ne ${#exec_cmd[*]} ]; then
        log "[ERROR][LINENO=${LINENO}]Invalid parameter numbers."
        return 1
    fi

    for i in $(seq 0 ${max_index})
    do
        egrep -q "^${exec_cmd[${i}]}$" <<< "${sh_args[${i}]}" >& /dev/null
        if [ $? -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Invalid parameter for arg${i}."
            return 1
        fi
    done

    g_sh_exec_side="${sh_args[0]}"
    g_sh_opt_action="${sh_args[1]}"
    if [ "${g_sh_exec_side}" = "host" ]; then
        g_livepatch_absolute_path="${sh_args[2]}"
    else
        g_livepatch_absolute_path="/var/live_patch"
    fi

    log "[INFO][LINENO=${LINENO}]arg0=[${g_sh_exec_side}]; arg1=[${g_sh_opt_action}]; arg2=[${g_livepatch_absolute_path}]"
    log "[INFO][LINENO=${LINENO}]Eop_check success."
    return 0
}

########################################################
# Function: init_patch_modules
# Description: to set value for g_livepatch_kernel_modules.
# Parameter:
#     input: N/A
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
init_patch_modules() {
    # if host, drv_pcie_host must be insmoded.
    if [ "${g_sh_exec_side}" = "host" ] && ! lsmod 2>/dev/null | grep -wq "drv_pcie_host"; then
        log "[ERROR][LINENO=${LINENO}]The [drv_pcie_host] is not insmod."
        return 2
    fi

    # kernel modules; ko;
    g_livepatch_kernel_modules="$(echo $(ls ${g_livepatch_absolute_path}/modules/* 2>/dev/null | grep .tar.gz$))"
    # if they are both NULL, return directly.
    if [ -z "${g_livepatch_kernel_modules}" ]; then
        log "[INFO][LINENO=${LINENO}]There is no livepatch and skip."
        return 0
    fi
    log "[INFO][LINENO=${LINENO}]g_livepatch_kernel_modules=[${g_livepatch_kernel_modules}]."

    log "[INFO][LINENO=${LINENO}]init_patch_modules success."
    return 0
}

file_limit_backup() {
    local log_file=$1
    [ -f ${log_file} ] || return
    
    local log_file_size=$(stat -c %s ${log_file})
    [ $log_file_size -lt 102400 ] && return
    
    cp -a ${log_file} ${log_file}.bak
    >${log_file}
    log "[INFO][LINENO=${LINENO}]log file:$log_file backup by size:$log_file_size."
}
########################################################
# Function: device_log_truncate
# Description: to limit device log size.
# Parameter:
#     input: N/A
#
#     output: N/A
#
# Return: N/A
#
# Others:
########################################################
device_log_truncate() {
    [ "${g_sh_exec_side}" = "host" ] && return
    file_limit_backup ${logFile}
    local syscared_bak_log=$(ls $g_syscared_log_dir|grep -v $g_syscared_cur_log_file_name)
    [ "$syscared_bak_log" = "" ] && return
    log "[INFO][LINENO=${LINENO}]rm syscared bak log:$syscared_bak_log, $(ls -l $g_syscared_log_dir)."
    (cd $g_syscared_log_dir && rm -f $syscared_bak_log)
}

########################################################
# Function: load_livepatch_conf
# Description: load OS livepatch configuration.
# Parameter:
#     input: N/A
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
load_livepatch_conf() {
    # if in the device, the /boot/config-$(uname -r) doesn't exist.
    if [ "${g_sh_exec_side}" = "device" ]; then
        log "[INFO][LINENO=${LINENO}]If it is in device, skip OS cmd and conf check."
        return 0
    fi

    if ! which livepatch >& /dev/null; then
        log "[WARNING][LINENO=${LINENO}]The command [livepatch] not found."
        g_config_livepatch="n"

        # if host patches are not null and 'install' scenario, this will be printed on the terminal.
        if [ ! -z "${g_livepatch_kernel_modules}" ] && [ "${g_sh_opt_action}" = "install" ]; then
            spc_color_echo "\033[33m[WARNING]The command [livepatch] not found and host patches will not load.\033[0m"
        elif [ ! -z "${g_livepatch_kernel_modules}" ] && [ "${g_sh_opt_action}" = "uninstall" ]; then
            spc_color_echo "\033[33m[WARNING]The command [livepatch] not found and host patches may not unload.\033[0m"
        fi
        return 0
    fi

    if [ ! -f "/boot/config-$(uname -r)" ]; then
        log "[ERROR][LINENO=${LINENO}]The file [/boot/config-$(uname -r)] doesn't exist."
        return 2
    fi

    # to check the OS does support 'livepatch' or not. 
    if ! grep -wq "CONFIG_LIVEPATCH=y" /boot/config-$(uname -r) 2>/dev/null; then
        log "[INFO][LINENO=${LINENO}]The OS CONFIG_LIVEPATCH is [N]."
        g_config_livepatch="n"

        if [ ! -z "${g_livepatch_kernel_modules}" ] && [ "${g_sh_opt_action}" = "install" ]; then
            spc_color_echo "\033[33m[WARNING]The OS CONFIG_LIVEPATCH is not [y] and host patches will not load.\033[0m"
        fi
    fi

    return 0
}

########################################################
# Function: upgrade_tool_exist_check
# Description: to check if upgrade-tool exist or not.
# Parameter:
#     input: N/A
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
upgrade_tool_exist_check() {
    if [ "${g_sh_exec_side}" = "device" ]; then
        log "[INFO][LINENO=${LINENO}]If it is in device, skip upgrade-tool exist check."
        return 0
    fi

    g_local_drv_root_path="$(output_local_drv_root_path)"

    log "[INFO][LINENO=${LINENO}]g_local_drv_root_path=[${g_local_drv_root_path}]."
    if [ ! -f "${g_local_drv_root_path}"/driver/tools/upgrade-tool ]; then
        log "[ERROR][LINENO=${LINENO}]The upgrade-tool doesn't exist."
        return 2
    fi
    log "[INFO][LINENO=${LINENO}]upgrade_tool_exist_check success."
    return 0
}

########################################################
# Function: update_single_livepatch_status
# Description: to update a single module livepatch status.
# Parameter:
#     input:
#     $1 -- modules whose status need to be updated.
#     $2 -- patch status value.
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
update_single_livepatch_status() {
    # patch_name: klp_xxx.tar.gz
    local patch_name="$1" set_value="$2"
    local module_name="" i=0

    # klp_xxx
    module_name=${patch_name%%.*}
    log "[INFO][LINENO=${LINENO}]module_name=[${module_name}]; set_value=[${set_value}]"

    if ! lsmod | grep -wq "${module_name}"; then
        log "[INFO][LINENO=${LINENO}]The [${module_name}] is not in the kernel and skip here."
        return 0
    fi

    if [ ${set_value} -eq 1 ]; then
        # if failed to enable the livepatch, it will retry 5 times.
        for i in $(seq 1 6)
        do
            log "[INFO][LINENO=${LINENO}]It is [$i] times to enable the [${module_name}]."
            livepatch -a "${module_name}" >& /dev/null;ret=$?
            if [ ${ret} -ne 0 ]; then
                continue
            fi
            break
        done
    else
        # if failed to disable the livepatch, it will retry 5 times too.
        for i in $(seq 1 6)
        do
            log "[INFO][LINENO=${LINENO}]It is [$i] times to disable the [${module_name}]."
            livepatch -d "${module_name}" >& /dev/null;ret=$?
            if [ ${ret} -ne 0 ]; then
                continue
            fi
            break
        done
    fi
    if [ ${ret} -ne 0 ]; then
        log "[ERROR][LINENO=${LINENO}]Failed to update livepatch [${patch_name}] status."
        return 1
    fi

    log "[INFO][LINENO=${LINENO}]Calling update_single_livepatch_status for [${module_name}] succeeded."
    return 0
}

########################################################
# Function: update_all_livepatch_status
# Description: to update all modules livepatch status.
# Parameter:
#     input:
#     $1 -- modules whose status need to be updated.
#     $2 -- patch status value.
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- update and rollback both failed
#     2 -- update failed but rollback succeeded
#
# Others:
########################################################
update_all_livepatch_status() {
    local args_val=$@ args_num=$#
    local args_arr=()
    local patch_name=""
    local max_module_index=0 set_value_index=0
    local set_value=0

    if [ ${args_num} -eq 1 ]; then
        log "[INFO][LINENO=${LINENO}]The update-args is NULL and skip."
        return 0
    fi
    log "[INFO][LINENO=${LINENO}]args_val=[${args_val}]; args_num=[${args_num}]."

    # format str to array.
    args_arr=(${args_val})
    # to get set-value index.
    set_value_index=$(expr ${args_num} - 1)
    # to get set-value.
    set_value=${args_arr[${set_value_index}]}
    # to get the max module's index.
    max_module_index=$(expr ${args_num} - 2)

    for i in $(seq 0 ${max_module_index})
    do
        patch_name=${args_arr[${i}]}
        if ! update_single_livepatch_status ${patch_name##*/} ${set_value}; then
            return 1
        fi
    done

    log "[INFO][LINENO=${LINENO}]Calling update_all_livepatch_status succeeded."
    return 0
}

kill_upatch_manage() {
    local try_i ps_print upatch_manage_print patch_pid
    
    for try_i in {1..10};do
        ps_print=$(ps)
        upatch_manage_print=$(echo "$ps_print"|grep upatch-manage)
        [ "$upatch_manage_print" = "" ]&&{ log "[INFO][LINENO=${LINENO}] kill upatch-manage success, try_i:$try_i.";return 0;}
        log "[INFO][LINENO=${LINENO}] kill upatch-manage begin, try_i:$try_i, ps:$upatch_manage_print."
        if [ $try_i = 1 ];then
            for patch_pid in $(echo "$upatch_manage_print"|sed 's/.*--pid //'|awk '{print $1}');do
                log "[INFO][LINENO=${LINENO}] block pid:$patch_pid, $(echo "$ps_print"|grep "$patch_pid")."
            done
        fi
        killall upatch-manage
        sleep 0.2
    done
    log "[ERROR][LINENO=${LINENO}] kill upatch-manage fail, try_i:$try_i."
    return 1
}
########################################################
# Function: update_single_upatch_status
# Description: to update a single upatch status.
# Parameter:
#     input:
#     $1 -- action: apply/accept/remove
#     $2 -- upatch: upatch uuid
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
update_single_upatch_status() {
    local print ret try_i action=$1 upatch=$2
    
    for try_i in {1..1};do
        print=$(timeout 15 syscare $action $upatch 2>&1);ret=$?
        [ $ret -eq 0 ] && { log "[INFO][LINENO=${LINENO}] $action upatch:$upatch success, print:$print, try_i:$try_i.";return 0;}
        log "[WARNING][LINENO=${LINENO}] $action upatch:$upatch unsuccess, ret:$ret, print:$print, try_i:$try_i, upatch-manage:$(ps|grep upatch-manage)."
        kill_upatch_manage
        sleep 0.5
    done
    log "[ERROR][LINENO=${LINENO}] $action upatch:$upatch fail, try_i:$try_i."
    return 1
}

wait_all_upatch_load_once() {
    local file_cnt patch_info_cnt upatch_cnt load_cnt try_i print ret
    
    file_cnt=$(find $g_upatch_extract_dir -type f|wc -l)
    patch_info_cnt=$(find $g_upatch_extract_dir -type f -name patch_info|wc -l)
    upatch_cnt=$((file_cnt - patch_info_cnt))
    log "cp begin, cmd: cp -a $g_upatch_extract_dir/* $g_upatch_active_dir, src:$(ls -l $g_upatch_extract_dir/*), dst:$(ls -l $g_upatch_active_dir/)"
    print=$(cp -a $g_upatch_extract_dir/* $g_upatch_active_dir 2>&1);ret=$?
    [ $ret -eq 0 ] || { log "[WARNING][LINENO=${LINENO}] cp upatches file unsuccess: $ret, $print, src:$(ls -l $g_upatch_extract_dir/*), dst:$(ls -l $g_upatch_active_dir/*)";return 1;}
    log "cp success, cmd: cp -a $g_upatch_extract_dir/* $g_upatch_active_dir, src:$(ls -l $g_upatch_extract_dir/*), dst:$(ls -l $g_upatch_active_dir/*)"
    for try_i in {1..10}; do
        load_cnt=$(syscare list |sed 1d |wc -l)
        [ $upatch_cnt -eq $load_cnt ] && { log "[INFO][LINENO=${LINENO}] wait all upatch load success, upatch cnt:$load_cnt.";return 0;}
        log "[INFO][LINENO=${LINENO}] wait all upatch load, expect:$upatch_cnt, now:$load_cnt, try:$try_i."
        sleep 0.5
    done
    log "[WARNING][LINENO=${LINENO}] wait all upatch load once unsuccess, expect:$upatch_cnt, now:$load_cnt, try:$try_i, src:$(ls -l $g_upatch_extract_dir/*), dst:$(ls -l $g_upatch_active_dir/*)."
    return 1
}
########################################################
# Function: wait_all_upatch_load
# Description: wait all upatch load by syscared, then can manage by syscare.
# Parameter:
#     input: N/A
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
wait_all_upatch_load() {
    local try_i
    
    test -d $g_upatch_active_dir || { log "[ERROR][LINENO=${LINENO}] dest dir:$g_upatch_active_dir not exist.";return 1;}
    
    for try_i in {1..5}; do
        wait_all_upatch_load_once && return 0
        log "[INFO][LINENO=${LINENO}] wait all upatch load unsuccess, try:$try_i."
        rm -rf $g_upatch_active_dir/*
        sleep 0.5
    done
    log "[ERROR][LINENO=${LINENO}] wait all upatch load fail, try:$try_i."
    return 1
}

########################################################
# Function: update_all_upatch_status
# Description: to update all upatch livepatch status.
# Parameter:
#     input:
#     $1 -- patch status value. 0: active, 1: deactive
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
update_all_upatch_status() {
    local ret tool_fail_ret print upatch action1 action2

    if [ $# -ne 1 ]; then
        log "[INFO][LINENO=${LINENO}]The update-args is not 1 and skip."
        return 0
    fi

    if [ $1 -eq 0 ]; then
        action1=apply
        action2=accept
        tool_fail_ret=1
        [ "$(ls $g_upatch_extract_dir)" = "" ] && { log "[INFO][LINENO=${LINENO}] update all upatch status:$action1 skip by no upatch.";return 0;}
    else
        action1=remove
        action2=
        tool_fail_ret=0
        [ "$(ls $g_upatch_active_dir)" = "" ] && { log "[INFO][LINENO=${LINENO}] update all upatch status:$action1 skip by no upatch.";return 0;}
    fi

    print=$(syscare list 2>&1);ret=$?
    [ $ret -ne 0 ] && { log "[ERROR][LINENO=${LINENO}] update all upatch status:$1 $action1, syscare exec fail: ret:$ret, $print.";return $tool_fail_ret;}

    if [ $1 -eq 0 ]; then
        wait_all_upatch_load || return 1
    fi
    print=$(syscare list)
    log "[INFO][LINENO=${LINENO}] update all upatch status:$1 $action1 begin, $print."
    for upatch in $(echo "$print" |sed 1d|awk '{print $1}'); do
        update_single_upatch_status $action1 $upatch || return 1
        [ "$action2" = "" ] && continue
        update_single_upatch_status $action2 $upatch || return 1
    done
    log "[INFO][LINENO=${LINENO}] update all upatch status:$action1 success, $(syscare list)."
    return 0
}

########################################################
# Function: module_operate_check
# Description: to check if the livepatch loaded/unloaded success.
# Parameter:
#     input:
#     $1 -- operate: load or unload livepatch.
#     $2 -- module name
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
module_operate_check() {
    # patch_name: klp_xxx.tar.gz
    local module_opt="$1" patch_name="$2"
    local module_name=""

    # klp_xxx
    module_name=${patch_name%%.*}
    if [ "${module_opt}" = "insmod" ]; then
        if lsmod | grep -wq "${module_name}"; then
            log "[INFO][LINENO=${LINENO}]The [${module_name}] is already in the kernel."
            return 0
        fi

        # insmod the kernel module.
        livepatch -l "${patch_name}" >& /dev/null;ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to insmod [${module_name}]."
            return 1
        fi
    elif [ "${module_opt}" = "rmmod" ]; then
        if lsmod | grep -wq "${module_name}"; then
            # rmmod the kernel module.
            livepatch -r "${patch_name}" >& /dev/null;ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to rmmod [${module_name}]."
                return 1
            fi
        else
            log "[INFO][LINENO=${LINENO}]The [${module_name}] is not in the kernel."
        fi
    else
        log "[ERROR][LINENO=${LINENO}]The module_opt=[${module_opt}] is invalid."
        return 1
    fi

    log "[INFO][LINENO=${LINENO}]Successfully ${module_opt} the [${module_name}]."
    return 0
}

########################################################
# Function: rmmod_patch_modules_from_kernel
# Description: rmmod patch's modules from kernel.
# Parameter:
#     input:
#     $1 -- module list.
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
rmmod_patch_modules_from_kernel() {
    local args_val=$@ args_num=$#
    local patch_name=""

    if [ ${args_num} -eq 0 ] || [ -z "${args_val}" ]; then
        log "[INFO][LINENO=${LINENO}]The rmmod-args is NULL and skip."
        return 0
    fi


    for patch_name in ${args_val}
    do
        # klp_xxx.tar.gz
        log "[INFO][LINENO=${LINENO}]It is to unload [${patch_name}]."
        if ! module_operate_check "rmmod" "${patch_name##*/}"; then
            return 1
        fi
    done

    return 0
}

########################################################
# Function: insmod_patch_modules_to_kernel
# Description: insmod single patch's module to kernel.
# Parameter:
#     input:
#     $1 -- module list.
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
insmod_patch_modules_to_kernel() {
    local args_val=$@ args_num=$#
    local patch_name=""

    if [ ${args_num} -eq 0 ] || [ -z "${args_val}" ]; then
        log "[INFO][LINENO=${LINENO}]The insmod-args is NULL and skip."
        return 0
    fi

    for patch_name in ${args_val}
    do
        log "[INFO][LINENO=${LINENO}]It is to load [${patch_name}]."
        if ! module_operate_check "insmod" "${patch_name##*/}"; then
            return 1
        fi
    done

    log "[INFO][LINENO=${LINENO}]Calling insmod_patch_modules_to_kernel succeeded."
    return 0
}

########################################################
# Function: output_local_drv_root_path
# Description: to get local driver install path.
# Parameter:
#     input: N/A
#
#     output:
#     $1 -- local driver root path.
#
# Return:
#     0 -- success
#
# Others:
########################################################
output_local_drv_root_path() {
    local local_drv_root_path=""

    # /usr/local/Ascend/driver/livepatch/patch_version
    local_drv_root_path=${g_livepatch_absolute_path%/driver/livepatch/*}
    echo "${local_drv_root_path}"
    log "[INFO][LINENO=${LINENO}]local_drv_root_path=[${local_drv_root_path}]"

    cd - >& /dev/null

    return 0
}

########################################################
# Function: device_livepatch_operate_check
# Description: prevent ko from being packed into initrd.
# Parameter:
#     input:
#     $1 -- install/uninstall.
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- fail
#
# Others:
########################################################
device_livepatch_operate_check() {
    local livepatch_opt="$1"
    local root_home_path="$(cd ~>&/dev/null;echo ${PWD};cd ->&/dev/null)"
    local tool_file="${g_local_drv_root_path}/driver/tools/upgrade-tool" patch_file=""

    if [ ! -e "${g_livepatch_absolute_path}/script/specific_func.inc" ]; then
        log "[WARNING][LINENO=${LINENO}]The [${g_livepatch_absolute_path}/script/specific_func.inc] doesn't exist."
        return 0
    fi

    . "${g_livepatch_absolute_path}/script/specific_func.inc"
    if [ -z "${device_livepatch_name}" ]; then
        log "[INFO][LINENO=${LINENO}]The device livepatch is NULL and skip."
        return 0
    fi
    patch_file="$(ls ${g_livepatch_absolute_path}/device/* | grep -w ${device_livepatch_name})"
    [ -z "${patch_file}" ] && log "[INFO][LINENO=${LINENO}]The device livepatch is NULL and skip." && return 0

    # to set LD_LIBRARY_PATH.
    export LD_LIBRARY_PATH="${g_local_drv_root_path}/driver/lib64/common/:${g_local_drv_root_path}/driver/lib64/driver/:${LD_LIBRARY_PATH}"
    # install or uninstall patch.
    if [ "${livepatch_opt}" = "install" ]; then
        "${tool_file}" --device_index -1 --patch "${patch_file}" >& /dev/null;ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to load livepatch to device."
            return 1
        fi
    else
        "${tool_file}" --device_index -1 --unload_patch >& /dev/null;ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to unload device livepatch."
            return 1
        fi
    fi

    log "[INFO][LINENO=${LINENO}]Calling device_livepatch_operate_check to [${livepatch_opt}] succeeded."
    return 0
}

########################################################
# Function: load_livepatch
# Description: to load livepatch and make it take effect.
# Parameter:
#     input: N/A
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- update and rollback both failed
#     2 -- update failed but rollback succeeded
#
# Others:
########################################################
load_livepatch() {
    if [ "${g_sh_exec_side}" = "host" ]; then
        if [ "${g_config_livepatch}" = "y" ]; then
            insmod_patch_modules_to_kernel ${g_livepatch_kernel_modules};ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke insmod_patch_modules_to_kernel."
                return ${ret}
            fi

            update_all_livepatch_status ${g_livepatch_kernel_modules} 1;ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke update_all_livepatch_status."
                return ${ret}
            fi

            device_livepatch_operate_check "install";ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke device_livepatch_operate_check."
                return ${ret}
            fi
        else
            device_livepatch_operate_check "install";ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke device_livepatch_operate_check."
                return ${ret}
            fi
        fi
    else
        insmod_patch_modules_to_kernel ${g_livepatch_kernel_modules};ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to invoke insmod_patch_modules_to_kernel."
            return ${ret}
        fi

        update_all_livepatch_status ${g_livepatch_kernel_modules} 1;ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to invoke update_all_livepatch_status."
            return ${ret}
        fi

        update_all_upatch_status 0;ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to invoke update_all_upatch_status."
            return ${ret}
        fi

        cp -a $g_extract_version_path $g_active_version_path
    fi

    log "[INFO][LINENO=${LINENO}]Calling load_livepatch succeeded."
    return 0
}

########################################################
# Function: unload_livepatch
# Description: to unload livepatch and make it take effect.
# Parameter:
#     input: N/A
#
#     output: N/A
#
# Return:
#     0 -- success
#     1 -- update and rollback both failed
#     2 -- update failed but rollback succeeded
#
# Others:
########################################################
unload_livepatch() {
    if [ "${g_sh_exec_side}" = "host" ]; then
        if [ "${g_config_livepatch}" = "y" ]; then
            local device_check_ret=0
            device_livepatch_operate_check "uninstall";device_check_ret=$?
            if [ ${device_check_ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke device_livepatch_operate_check."
                update_all_livepatch_status ${g_livepatch_kernel_modules} 0
                rmmod_patch_modules_from_kernel ${g_livepatch_kernel_modules}
                return ${device_check_ret}
            fi

            update_all_livepatch_status ${g_livepatch_kernel_modules} 0;ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke update_all_livepatch_status."
                return ${ret}
            fi

            rmmod_patch_modules_from_kernel ${g_livepatch_kernel_modules};ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke rmmod_patch_modules_from_kernel."
                return ${ret}
            fi
        else
            device_livepatch_operate_check "uninstall";ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Failed to invoke device_livepatch_operate_check."
                return ${ret}
            fi
        fi
    else
        update_all_livepatch_status ${g_livepatch_kernel_modules} 0;ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to invoke update_all_livepatch_status."
            return ${ret}
        fi

        rmmod_patch_modules_from_kernel ${g_livepatch_kernel_modules};ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to invoke rmmod_patch_modules_from_kernel."
            return ${ret}
        fi

        update_all_upatch_status 1;ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR][LINENO=${LINENO}]Failed to invoke update_all_upatch_status."
            return ${ret}
        fi
    fi

    log "[INFO][LINENO=${LINENO}]Calling unload_livepatch succeeded."
    return 0
}

main_process() {
    cd "${g_livepatch_absolute_path}/modules" >& /dev/null

    if [ "${g_sh_opt_action}" = "install" ]; then
        load_livepatch;ret=$?
        if [ ${ret} -ne 0 ]; then
            unload_livepatch;ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Both livepatch loading and rollback failed."
                ret=1
            else
                log "[ERROR][LINENO=${LINENO}]Livepatch loading failed but rollback succeeded."
                ret=2
            fi
        fi
    else
        unload_livepatch;ret=$?
        if [ ${ret} -ne 0 ]; then
            load_livepatch;ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR][LINENO=${LINENO}]Both livepatch unloading and rollback failed."
                ret=1
            else
                log "[ERROR][LINENO=${LINENO}]Livepatch unloading failed but rollback succeeded."
                ret=2
            fi
        fi
    fi

    cd - >& /dev/null

    return ${ret}
}

########################################################
#
# main process
#
########################################################

# for device
if [ ! -e "${ASCEND_SECLOG}" ]; then
    mkdir -p "${ASCEND_SECLOG}" -m 750
    touch "${logFile}"
    chmod 400 "${logFile}"
fi

# escalation of privileges check.
Eop_check "$@"
if [ $? -ne 0 ]; then
    exit 2
fi

# to limit device log size
device_log_truncate

# to init user or kernel modules's value.
if ! init_patch_modules; then
    exit 2
fi

# load livepatch conf.
if ! load_livepatch_conf; then
    exit 2
fi

if ! upgrade_tool_exist_check; then
    exit 2
fi

main_process;ret=$?
exit ${ret}
