#!/bin/sh
# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

## module log

if [ "$(id -u)" != "0" ]; then
    COMM_LOG_DIR="${HOME}/var/log/ascend_seclog"
else
    COMM_LOG_DIR="/var/log/ascend_seclog"
fi

COMM_OPERATION_LOGFILE="${COMM_LOG_DIR}/operation.log"
COMM_LOGFILE="${COMM_LOG_DIR}/ascend_install.log"
COMM_USERNAME="$(id -un)"
COMM_USERGROUP="$(id -gn)"

LOG_PKG_NAME="Common"
LOG_FILE="/dev/null"
LOG_STYLE="normal"

comm_init_log() {
    if [ ! -d "$COMM_LOG_DIR" ]; then
        mkdir -p "$COMM_LOG_DIR"
    fi
    if [ $(id -u) -ne 0 ]; then
        chmod 740 "$COMM_LOG_DIR"
    else
        chmod 750 "$COMM_LOG_DIR"
    fi
    if [ ! -f "$COMM_LOGFILE" ]; then
        touch "$COMM_LOGFILE"
    fi
    chmod 640 "$COMM_LOGFILE"
}

_comm_compose_log_msg() {
    local _outvar="$1"
    local _log_type="$2"
    local _msg="$3"
    local _cur_date="$(date +'%Y-%m-%d %H:%M:%S')"
    local _result

    if [ "${LOG_STYLE}" = "no-colon" ]; then
        _result="[${LOG_PKG_NAME}] [${_cur_date}] [${_log_type}]${_msg}"
    else
        _result="[${LOG_PKG_NAME}] [${_cur_date}] [${_log_type}]: ${_msg}"
    fi
    eval "${_outvar}=\"${_result}\""
}

_comm_echo_log_msg() {
    local log_type_="$1"
    local log_format_="$2"

    if [ $log_type_ = "INFO" ]; then
        echo "${log_format_}"
    elif [ $log_type_ = "WARNING" ]; then
        echo "${log_format_}"
    elif [ $log_type_ = "ERROR" ]; then
        echo "${log_format_}"
    elif [ $log_type_ = "DEBUG" ]; then
        :
    fi
}

comm_echo() {
    local log_type="$1"
    local msg="$2"
    local log_msg

    _comm_compose_log_msg "log_msg" "$log_type" "$msg"
    _comm_echo_log_msg "$log_type" "$log_msg"
}

comm_log() {
    local log_type="$1"
    local msg="$2"
    local log_msg

    _comm_compose_log_msg "log_msg" "$log_type" "$msg"
    _comm_echo_log_msg "$log_type" "$log_msg"
    echo "$log_msg" >> "${LOG_FILE}"
}

set_comm_log() {
    local pkg_name="$1"
    local log_file="$2"

    LOG_PKG_NAME="${pkg_name}"
    if [ "$log_file" != "" ]; then
        LOG_FILE="${log_file}"
    fi
}

comm_log_operation() {
    local cur_date="$(date +'%Y-%m-%d %H:%M:%S')"
    local operation="$1"
    local runfilename="$2"
    local result="$3"
    local installmode="$4"
    local all_parma="$5"
    local level=""
    if [ "${operation}"  = "Install" ]; then
        level="SUGGESTION"
    elif [ "${operation}" = "Upgrade" ]; then
        level="MINOR"
    elif  [ "${operation}" = "Uninstall" ]; then
        level="MAJOR"
    else
        level="UNKNOWN"
    fi

    if [ ! -f "${COMM_OPERATION_LOGFILE}" ]; then
        touch "${COMM_OPERATION_LOGFILE}"
        chmod 640 "${COMM_OPERATION_LOGFILE}"
    fi

    echo "${operation} ${level} ${COMM_USERNAME} ${cur_date} 127.0.0.1 ${runfilename} ${result} installmode=${installmode}; cmdlist=${all_parma}" >> "${COMM_OPERATION_LOGFILE}"
}

## end module

comm_set_install_for_all_mod() {
    local _outvar="$1"
    local _mod="$2"
    local _new_mod

    local _new_mod="${_mod%?}"

    local _new_mod="${_new_mod}${_new_mod#${_new_mod%?}}"

    eval "${_outvar}=\"${_new_mod}\""
}

comm_create_file() {
    local path="$1"
    local mod="$2"
    local own="$3"
    local install_for_all="$4"

    if [ -d "$path" ]; then
        comm_log "WARNING" "remove existed dir $path before create file."
        rm -rf "$path"
    fi
    touch "$path"
    if [ $? -ne 0 ]; then
        comm_log "WARNING" "create file $path failed."
        return 1
    fi

    if [ "$install_for_all" = "true" ] || [ "$install_for_all" = "y" ]; then
        comm_set_install_for_all_mod "mod" "$mod"
    fi
    chmod "$mod" "$path"
    if [ $? -ne 0 ]; then
        comm_log "WARNING" "chmod path $path $mod failed."
        return 1
    fi

    chown "$own" "$path"
    if [ $? -ne 0 ]; then
        comm_log "WARNING" "chown path $path $own failed."
        return 1
    fi
    return 0
}

comm_create_dir() {
    local path="$1"
    local mod="$2"
    local own="$3"
    local install_for_all="$4"

    if [ "$path" = "" ]; then
        comm_log "WARNING" "dir path is empty"
        return 1
    fi

    if [ ! -d "$path" ]; then
        mkdir -p "$path"
        if [ $? -ne 0 ]; then
            comm_log "WARNING" "create dir $path failed."
            return 1
        fi
    fi

    if [ "$install_for_all" = "true" ] || [ "$install_for_all" = "y" ]; then
        comm_set_install_for_all_mod "mod" "$mod"
    fi
    chmod "$mod" "$path"
    if [ $? -ne 0 ]; then
        comm_log "WARNING" "chmod path $path $mod failed."
        return 1
    fi

    chown -f "$own" "$path"
    if [ $? -ne 0 ]; then
        comm_log "WARNING" "chown path $path $own failed."
        return 1
    fi
}

comm_create_package_dir() {
    local package_dir="$1"
    local own="$2"
    local install_for_all="$3"
    if [ ! -d "$package_dir" ]; then
        comm_create_dir "$package_dir" "755" "$own" "$install_for_all"
    fi
}

comm_create_install_info_by_path() {
    local install_info="$1"
    local own="$2"
    if [ ! -f "$install_info" ]; then
        comm_create_file "$install_info" "640" "$own" "false"
    fi
}

comm_update_install_param() {
    local key="$1"
    local val="$2"
    local file="$3"
    local param=""
    if [ ! -f "${file}" ]; then
        return 1
    fi
    param="$(grep -i "^${key}=" "${file}")"
    if [ "${param}" = "" ]; then
        echo "${key}=${val}" >> "${file}"
    else
        sed -i "/^${key}=/Ic ${key}=${val}" "${file}"
    fi
}

comm_get_install_param() {
    local _outvar="$1"
    local _file="$2"
    local _key="$3"
    local _result

    if [ ! -f "${_file}" ]; then
        comm_log "WARNING" "file ${_file} doesn't exist in get install param."
        return 1
    fi
    _result="$(grep -i "^${_key}=" "${_file}" | cut -d"=" -f2-)"
    eval "${_outvar}=\"${_result}\""
}

comm_update_install_info() {
    local install_path_param=""
    local install_type=""
    local username=""
    local usergroup=""
    local feature_type=""
    local chip_type=""
    local package install_info

    while true
    do
        case "$1" in
        --install-path-param=*)
            install_path_param="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --install-type=*)
            install_type="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --username=*)
            username="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --usergroup=*)
            usergroup="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --feature-type=*)
            feature_type="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --chip-type=*)
            chip_type="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        *)
            break
            ;;
        esac
    done

    if [ $# -lt 2 ]; then
        return 1
    fi

    package="$1"
    install_info="$2"

    if [ "$install_path_param" != "" ]; then
        comm_update_install_param "${package}_Install_Path_Param" "$install_path_param" "$install_info"
    fi
    if [ "$install_type" != "" ]; then
        comm_update_install_param "${package}_Install_Type" "$install_type" "$install_info"
    fi
    if [ "$username" != "" ]; then
        comm_update_install_param "${package}_UserName" "$username" "$install_info"
    fi
    if [ "$usergroup" != "" ]; then
        comm_update_install_param "${package}_UserGroup" "$usergroup" "$install_info"
    fi
    if [ "$feature_type" != "" ]; then
        comm_update_install_param "${package}_Feature_Type" "$feature_type" "$install_info"
    fi
    if [ "$chip_type" != "" ]; then
        comm_update_install_param "${package}_Chip_Type" "$chip_type" "$install_info"
    fi
}

comm_start_log() {
    local all_parma="$@"
    local cur_date="$(date +'%Y-%m-%d %H:%M:%S')"
    comm_log "INFO" "Start time:$cur_date"
    comm_log "INFO" "LogFile:${COMM_LOGFILE}"
    comm_log "INFO" "InputParams:$all_parma"
    comm_log "INFO" "OperationLogFile:${COMM_OPERATION_LOGFILE}"
}

comm_exit_log() {
    local cur_date="$(date +'%Y-%m-%d %H:%M:%S')"
    comm_log "INFO" "End time:${cur_date}"
    exit "$1"
}

comm_print_usage() {
    local runfilename="$1"
    comm_log "INFO" "Please input this command for help: ./${runfilename} --help"
}

comm_judgmentpath() {
    local install_path="$1"
    local pkg_name="$2"
    check_install_path_valid "$install_path"
    if [ $? -ne 0 ]; then
        comm_log "ERROR" "The $pkg_name install_path $install_path is invalid, only characters in [a-z,A-Z,0-9,-,_] are supported!"
        exit 1
    fi
}

comm_normalize_install_path() {
    local _outvar="$1"
    local _install_path="$2"
    local _slashes_num _result
    _slashes_num=$(echo "$_install_path" | grep -o '/' | wc -l)
    if [ "$_slashes_num" -gt 1 ]; then
        _result=$(echo "$_install_path" | sed "s/\/*$//g")
    else
        _result="$_install_path"
    fi
    eval "${_outvar}=\"${_result}\""
}

comm_parse_install_path() {
    local _outvar="$1"
    local _install_path="$2"
    local _pkg_name="$3"

    comm_judgmentpath "$_install_path" "$_pkg_name"
    comm_normalize_install_path "$_outvar" "$_install_path"
}

comm_err_file_or_directory_not_exist() {
    comm_log "ERROR" "The file or directory doesn't exist, $1"
    comm_exit_log 1
}

os_name() {
    if [ ! -f "$1" ];then
        HostOsName=unknown
        HostOsFullName=unknown
        HostOsVersion=unknown
        return
    fi

    HostOsName=$(cat "$1" | grep ^NAME= | awk -F "[\" ]" '{print $2}')
    HostOsFullName=$(cat "$1" | grep ^NAME= | awk -F "\"" '{print $2}')

    if [ x"$HostOsName" = "x" ];then
        HostOsName=$(cat "$1" | grep ^NAME= | awk -F "[=]" '{print $2}')
        HostOsFullName=$(cat "$1" | grep ^NAME= | awk -F "\"" '{print $2}')
    fi
    if [ x"$HostOsName" = "x" ];then
        HostOsName=unknown
        HostOsFullName=unknown
    fi
    HostOsVersion=$(cat "$1" | grep ^VERSION_ID= | awk -F "\"" '{print $2}')
    if [ x"$HostOsVersion" = "x" ];then
        HostOsVersion=unknown
    fi
    return
}

get_os_info() {
    if [ -f /etc/os-release ];then
        os_name /etc/os-release
    elif [ -f /etc/centos-release ];then
        HostOsName=CentOS
        HostOsFullName="CentOS Linux"
        HostOsVersion=$(cat /etc/centos-release | awk '{print $4}')
    else
        which lsb_release >/dev/null 2>&1
        if [ $? -eq 0 ];then
            HostOsName=$(lsb_release -si)
            HostOsFullName="${HostOsName}"
            HostOsVersion=$(lsb_release -sr)
        else
            os_name /etc/issue
        fi
    fi
    return
}

get_system_info() {
    get_os_info
    HostArch=$(uname -m)
    KernelVersion=$(uname -r)
}

version_gt() {
    if [ "$2"x = "x" ];then
        return 0
    else
        test "$(echo "$@" | tr " " "\n" | sort -V | head -n 1)" != "$1"
    fi
}
version_le() {
    if [ "$2"x = "x" ];then
        return 0
    else
        test "$(echo "$@" | tr " " "\n" | sort -V | head -n 1)" = "$1"
    fi
}
version_lt() {
    if [ "$2"x = "x" ];then
        return 0
    else
        test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" != "$1"
    fi
}
version_ge() {
    if [ "$2"x = "x" ];then
        return 0
    else
        test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" = "$1"
    fi
}

version_vaild() {
    local ver_range="$1"        #version range
    local ver="$2"              #version provider
    local clean_ver_range=$(echo "$ver_range" | sed 's/"//g')
    local new_ver_range=$(echo "$clean_ver_range" | sed 's/\[//' | sed 's/\]//' | sed 's/(//' | sed 's/)//')

    local start=$(echo $new_ver_range | awk -F ',' '{print $1}')
    local end=$(echo $new_ver_range | awk -F ',' '{print $2}')

    if  echo $clean_ver_range | grep -Eq "^\[.*\]$" ; then

        if  version_ge $ver $start  && version_le $ver $end ; then

            return 0
        fi
    elif echo $clean_ver_range | grep -Eq "^\[.*\)$"; then

        if  version_ge $ver $start  && version_lt $ver $end ; then
            return 0
        fi
    elif echo $clean_ver_range | grep -Eq "^\(.*\]$"; then

        if  version_gt $ver $start  && version_le $ver $end ; then
            return 0
        fi
    elif echo $clean_ver_range | grep -Eq "^\(.*\)$"; then

        if  version_gt $ver $start  && version_lt $ver $end ; then
            return 0
        fi
    else

        if version_ge $ver $clean_ver_range ;then
            return 0
        fi
    fi

    return 1
}

check_pkg_ver_deps() {
    ver_path="$1"
    req_pkg="$2"
    req_ver_path="$3"

    ver_info_list=$(awk -F '[_=]' '$1=="required" && $2=="'$req_pkg'" {print $1":"$2":"$3":"$4":"$5}' "$ver_path")

    for line in ${ver_info_list}
    do
        itf=`echo $line | cut -d":" -f 3`
        ver=`echo $line | cut -d":" -f 5`
        req_ver=`awk -F '=' '$1=="'$itf'_version" {print $2}' "$req_ver_path"`
        if ! version_vaild $ver $req_ver; then
            VerCheckStatus=FAIL
            ver_check_status=FAIL
            return
        fi
    done
    VerCheckStatus=SUCC
    ver_check_status=SUCC
}

check_install_path_valid() {
    local install_path="$1"

    if echo "${install_path}" | grep -Eq '/{2,}|\.{3,}'; then
        return 1
    fi

    if echo "${install_path}" | grep -Eq '^~?[a-zA-Z0-9./_-]*$'; then
        return 0
    else
        return 1
    fi
}

create_softlink_icp_relative() {
    local src_path="$1"
    local dst_path="$2"
    local source="top${src_path}"
    local target="top${dst_path}"

    local common="${target%/*}"

    local forward="${source#"$common"/}"

    local result=""

    while [ "${forward}" = "${source}" ]; do
        common="$(dirname "$common")"
        forward="${source#"$common"/}"
        result="../${result}"
    done

    result="${result}${forward}"

    ln -sfn "${result}" "${dst_path}"
    if [ $? -ne 0 ]; then
        log "ERROR" "create softlink relative from ${src_path} to ${dst_path} failed!"
        return 1
    fi
    return 0
}

create_softlink() {
    local _src_dir="$1"
    local _dst_dir="$2"
    local _sub_dir_src="$3"
    local _sub_dir_dst="$4"
    local _sub_dir_dst_new _dst_path ret total_ret=0

    for _src_path in "${_src_dir}"/${_sub_dir_src}
    do
        if [ -z "$_sub_dir_dst" ]; then
            _sub_dir_dst_new="$(basename "${_src_path}")"
        else
            _sub_dir_dst_new="${_sub_dir_dst}"
        fi
        _dst_path="${_dst_dir}/${_sub_dir_dst_new}"
        if [ -L "${_dst_path}" ]; then
            rm -f "${_dst_path}"
        fi

        _dst_path="$(readlink -f "${_dst_path}")"
        if [ -d "${_dst_path}" ]; then

            _dst_path="${_dst_path}/$(basename "${_src_path}")"
        fi

        _src_path="$(readlink -f "${_src_path}")"

        create_softlink_icp_relative "${_src_path}" "${_dst_path}"
        ret="$?" && [ $ret -ne 0 ] && total_ret=$ret
    done

    return $total_ret
}

create_softlink_by_relative_ln() {
    local _src_dir="$1"
    local _dst_dir="$2"
    local _sub_dir_src="$3"
    local _sub_dir_dst="$4"
    local _src_path="$_src_dir/$_sub_dir_src"

    local _dst_path
    if [ -z "$_sub_dir_dst" ]; then
        _sub_dir_dst=$(basename $_src_path)
    fi
    _dst_path="$_dst_dir/$_sub_dir_dst"
    if [ -L "${_dst_path}" ]; then
        rm -f "${_dst_path}"
    fi
    ln -sr $_src_path $_dst_path
}

create_softlink_if_exists() {
    local _src_dir="$1"
    local _sub_dir_src="$3"
    local _src_path="$_src_dir/$_sub_dir_src"

    if [ -e $_src_path ]; then
        create_softlink "$@"
    fi
}

is_dir_empty() {

    if [ ! -d "$1" ]; then
        return 1
    fi

    if [ "$(ls -A "$1" 2>&1)" != "" ]; then
        return 2
    fi
    return 0
}

remove_dir_if_empty() {
    local dirpath="$1"

    if is_dir_empty "${dirpath}"; then
        rm -rf "${dirpath}"
    fi
    return 0
}

remove_dir() {
    local dirpath="$1"

    if [ -e "${dirpath}" ]; then
        chmod u+w -R "${dirpath}"
    fi
    rm -rf "${dirpath}"

    return 0
}

remove_softlink() {
    local path="$1"
    [ -L "${path}" ] && rm -f "${path}"
}

process_pre_check() {
    local pkg_name="$1"
    local pre_check_func="$2"
    local standalone="$3"

    log "INFO" "${pkg_name} do pre_check started."
    ${pre_check_func}
    ret=$?
    if [ $ret -ne 0 ]; then
        log "WARNING" "${pkg_name} do pre check failed."
    fi
    log "INFO" "${pkg_name} do pre_check finished."

    if [ "${standalone}" = "y" ]; then
        if [ $ret -ne 0 ]; then
            exitInstallLog 1
        fi
        exitInstallLog 0
    fi
}

does_pkg_installed() {
    local install_path="$1"
    local pkgname="$2"
    local varname="$3"
    local result="true"
    local pkg_path="${install_path}/${pkgname}"
    local script_path="${pkg_path}/script"

    if [ -L "${pkg_path}" ]; then
        result="false"
    fi

    if [ ! -d "${pkg_path}" ]; then
        result="false"
    fi

    if [ -L "${script_path}" ]; then
        result="false"
    fi

    if [ ! -d "${script_path}" ]; then
        result="false"
    fi

    eval "${varname}=\"${result}\""
}

get_driver_install_path() {
    local _outvar="$1"
    local _install_path="$2"

    local _install_info="/etc/ascend_install.info"
    local _ascend_hal_name="libascend_hal.so"
    local _old_ifs
    local _var
    local _driver_install_path
    local _ldconfig_result
    local _ldconfig_cnt

    if [ -d "${_install_path}/driver" ]; then
        eval "${_outvar}=\"${_install_path}\""
        return 0
    fi

    if [ -f ${_install_info} ]; then
        . ${_install_info}
        if [ ! -z ${Driver_Install_Path_Param} ]; then
            _driver_install_path="${Driver_Install_Path_Param}"
            eval "${_outvar}=\"${_driver_install_path}\""
            return 0
        fi
    fi

    eval "${_outvar}=\"\""
}

run_uninstall_script() {
    local install_path="$1"
    local pkgname="$2"
    local uninstall_path="${install_path}/${pkgname}/script/uninstall.sh"

    if [ -f "${uninstall_path}" ]; then
        "${uninstall_path}"
        return $?
    else
        return 1
    fi
}

get_package_version() {
    local _outvar="$1"
    local _version_info_path="$2"

    if [ ! -f "${_version_info_path}" ]; then
        eval "${_outvar}=\"\""
        return 1
    fi

    read $_outvar <<EOF
$(grep "^Version=" "${_version_info_path}" | cut -d= -f2-)
EOF
}

get_version() {
    get_package_version "$@"
}

get_version_dir() {
    local _outvar="$1"
    local _version_info_path="$2"
    local _result

    if [ ! -f "${_version_info_path}" ]; then
        eval "${_outvar}=\"\""
        return 1
    fi

    _result="$(grep "^version_dir=" "${_version_info_path}" | cut -d= -f2-)"
    eval "${_outvar}=\"${_result}\""
}

get_arch() {
    local _outvar="$1"
    local _scene_info_path="$2"
    local _result

    if [ ! -f "${_scene_info_path}" ]; then
        eval "${_outvar}=\"\""
        return 1
    fi

    _result="$(grep "^arch=" "${_scene_info_path}" | cut -d= -f2-)"
    eval "${_outvar}=\"${_result}\""
}

get_install_package_dir() {
    local _outvar="$1"
    local _version_info_path="$2"
    local _install_path="$3"
    local _pkg_name="$4"
    local _pkg_is_multi_version _pkg_version_dir _result

    is_multi_version_pkg "_pkg_is_multi_version" "$_version_info_path"
    get_version_dir "_pkg_version_dir" "$_version_info_path"

    if [ "$_pkg_is_multi_version" = "true" ]; then
        _result="$_install_path/$_pkg_version_dir/$_pkg_name"
    else
        _result="$_install_path/$_pkg_name"
    fi
    eval "${_outvar}=\"${_result}\""
}

titled_word() {
    local word="$1"
    awk -v word="${word}" '
        BEGIN {print toupper(substr(word, 1, 1)) substr(word, 2)}
    '
}

_package_to_log_pkg_name() {
    local _outvar="$1"
    local _pkg_name="$2"
    read $_outvar <<EOF
$(titled_word "$_pkg_name")
EOF
}

get_titled_package_name() {
    _package_to_log_pkg_name "$@"
}

preinstall_process() {
    local install_path=""
    local docker_root=""
    local script_dir=""
    local package=""
    local logfile=""
    local real_install_path
    local version_info_path
    local ret

    eval set -- "$@ --"

    while true
    do
        case "$1" in
        --install-path=*)
            install_path="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --docker-root=*)
            docker_root="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --script-dir=*)
            script_dir="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --package=*)
            package="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --logfile=*)
            logfile="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        --logstyle=*)
            LOG_STYLE="$(echo "$1" | cut -d"=" -f2-)"
            shift
            ;;
        *)
            break
            ;;
        esac
    done

    if [ "${install_path}" = "" ]; then
        echo "error: install_path is needed!"
        return 1
    fi
    if [ "${script_dir}" = "" ]; then
        echo "error: script_dir is needed!"
        return 1
    fi

    if [ "${package}" = "" ]; then
        echo "error: package is needed!"
        return 1
    fi
    if [ "${logfile}" = "" ]; then
        echo "error: logfile is needed!"
        return 1
    fi

    LOG_FILE="${logfile}"
    _package_to_log_pkg_name "LOG_PKG_NAME" "${package}"

    if [ "${docker_root}" = "" ]; then
        real_install_path="${install_path}"
    else
        real_install_path="${docker_root}/${install_path}"
    fi

    version_info_path="$(pwd)/version.info"

    _check_version_compatiable "${version_info_path}" "${real_install_path}" "${script_dir}"
    ret=$?
    if [ $ret -ne 0 ]; then
        return 1
    fi

    return 0
}

preinstall_check() {
    preinstall_process "$@"
}

is_multi_version_pkg() {
    local _outvar="$1"
    local _filepath="$2"
    local _ret="false"

    if [ -f "${_filepath}" ]; then
        grep "^version_dir=" "${_filepath}" > /dev/null
        [ $? -eq 0 ] && _ret="true"
    fi
    eval "${_outvar}=\"${_ret}\""
}

_filelist_column() {
    local _filepath="$1"
    local _idx="$2"
    tail -n +2 "$_filepath" | cut -d, -f${_idx} | sort | uniq
}

_remove_comm_feature() {
    local _outvar="$1"
    local _feature="$2"
    local _result

    _result="$(echo "$_feature" | sed 's/comm//g' | sed 's/,,/,/g' | sed 's/^,//g' | sed 's/,$//g')"
    eval "${_outvar}=\"${_result}\""
}

_feature_to_regex() {
    local _outvar="$1"
    local _feature="$2"
    local _result_ftr
    _result_ftr="\b(($(echo "$_feature" | sed 's/,/)|(/g')))\b"
    eval "${_outvar}=\"${_result_ftr}\""
}

contain_feature() {
    local _outvar="$1"
    local _feature="$2"
    local _filepath="$3"
    local _feature_regex _feature_cf

    _remove_comm_feature "_feature_cf" "$_feature"
    if [ "$_feature_cf" = "" ]; then
        eval "${_outvar}=\"false\""
        return 0
    fi

    _feature_to_regex "_feature_regex" "$_feature_cf"
    _filelist_column "$_filepath" 10 | grep -E "$_feature_regex" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        eval "${_outvar}=\"true\""
    else
        eval "${_outvar}=\"false\""
    fi
}

add_fileitem() {
    local filepath="$1"
    local operation="$2"
    local path_in_pkg="$3"
    local install_path="$4"
    local permission="$5"
    local owner_group="$6"
    local install_type="$7"
    local softlink="$8"
    local pkg_inner_softlink="$9"
    local block="${10}"
    local feature="${11}"
    local chip="${12}"
    local arch="${13}"
    local is_common_path="N"

    if [ "$owner_group" = "DEFAULT" ]; then
        owner_group="\\\\\$username:\\\\\$usergroup"
    fi

    if [ "$feature" = "DEFAULT" ]; then
        feature="all"
    fi

    if [ "$chip" = "DEFAULT" ]; then
        chip="all"
    fi

    if echo "$install_path" | grep "^${arch}-linux/" > /dev/null; then
        is_common_path="Y"
    fi

    echo "NA,${operation},${path_in_pkg},${install_path},FALSE,${permission},${owner_group},${install_type},${softlink},${feature},${is_common_path},FALSE,NA,${block},${pkg_inner_softlink},${chip}" >> ${filepath}
}

custom_process_after_parse_argv() { :; }
