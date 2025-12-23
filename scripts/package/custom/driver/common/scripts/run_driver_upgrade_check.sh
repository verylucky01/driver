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

DPKG_STATUS_FILE=/var/lib/dpkg/tmp.ci/control

DRIVER_DIR_INSTALLED="$(dirname $(dirname $0))"
DRIVER_DIR_INSTALLING="${PWD}/driver"
UPGRADE_TOOL="${DRIVER_DIR_INSTALLED}/tools/upgrade-tool"

FLASH_VERSION_CACHE=/run/ascend/hardware/flash_version.txt
CDR_MANUFACTURERS_CACHE=/run/ascend/hardware/cdrs.txt
HBM_INFORMATION_CACHE=/run/ascend/hardware/hbm.txt

LOG_FILE_OPERATION="/var/log/ascend_seclog/operation.log"
LOG_FILE="/var/log/ascend_seclog/ascend_install.log"
LOG_MOD_NAME=Driver
LOG_CMD_INDEX=0


logger_debug()   { LEVEL=DEBUG   logger_log "$@"; }
logger_info()    { LEVEL=INFO    logger_log "$@"; }
logger_warning() { LEVEL=WARNING logger_log "$@"; }
logger_error()   { LEVEL=ERROR   logger_log "$@"; }
logger_log() {
    local red=31
    local green=32
    local yellow=33
    local ERROR=red WARNING=yellow INFO=green
    local color code

    LEVEL=${LEVEL:-NOTSET}
    if [[ $1 == -q ]]; then
        shift
    elif [[ ${LEVEL} =~ ^(INFO|WARNING|ERROR)$ ]]; then
        if [[ $1 == -c ]]; then
            shift
            if [[ ${LEVEL} != INFO ]]; then
                color=${!LEVEL}
                code=${!color}
            fi
        fi
        if [[ -n ${code} ]]; then
            printf "[${LOG_MOD_NAME}] [%(%F %T)T] [%s] \e[${code}m%s\e[0m\n" -1 ${LEVEL} "$*"
        else
            printf "[${LOG_MOD_NAME}] [%(%F %T)T] [%s] %s\n" -1 ${LEVEL} "$*"
        fi
    fi
    local user=$(id | awk '{printf $1}')
    printf "[${LOG_MOD_NAME}] [%(%F %T)T] [%s] [%s] %s\n" -1 "${user}" ${LEVEL} "$*" >> ${LOG_FILE}
}

teefd3() { tee >(cat >&3); }

# Execute the given command and duplicate its stdout & stderr to log file
logger_eval() {
    local ident error status
    (( ++LOG_CMD_INDEX ))
    ident=$(printf "cmd%03d\n" ${LOG_CMD_INDEX})
    logger_debug "[${ident}] $@"
    eval "$@" 3>> ${LOG_FILE} 1> >(teefd3) 2> >(teefd3 >&2)
    error=$?
    if (( error == 0 )); then
        status=ok
    elif (( error == 124 )) && [[ $1 =~ ^timeout ]]; then
        status="timed out"
    else
        status="failed (error=${error})"
    fi
    logger_debug "[${ident}] ${status}"
    return ${error}
}

get_ppid() { ps -o ppid= $1 | tr -d ' '; }

get_process_cmdline() { cat /proc/$1/cmdline | tr '\0' ' ' && echo; }

get_process_startup_time() {
    date "+%Y-%m-%d %T" -r /proc/$1/cmdline 2>/dev/null
}

log_interception() {
    local pkg_name_regexp='\b((Ascend|Atlas).*-driver.*\.(deb|rpm|run))\b'
    local pkg_name
    local op_type='Upgrade'
    local start_time
    local status='failed'
    local install_type='full'
    local cmdline
    local cmdargs
    local entry_pid pid=$$

    # Search the process tree from bottom to top,
    # to obtain the name of the installation package
    while (( pid != 1 )); do
        pid=$(get_ppid ${pid})
        cmdargs=($(get_process_cmdline ${pid}))
        if [[ ${cmdargs[@]} =~ ${pkg_name_regexp} ||
              ${cmdargs[1]} =~ (.*/install\.sh)$ ]]; then
            pkg_name="${BASH_REMATCH[1]}"
            entry_pid=${pid}
            cmdline="${cmdargs[@]}"
        fi
        [[ ${cmdargs[0]} =~ /(bash|sh)$ ]] || break
    done
    [[ -n ${pkg_name} ]] || return 1

    start_time=$(get_process_startup_time ${entry_pid})

    _log_operation
}

_log_operation() {
    local level=UNKNOWN
    local addr=127.0.0.1
    local user=$(id -un)

    case "${op_type}" in
        Install )   level=SUGGESTION ;;
        Upgrade )   level=MINOR ;;
        Uninstall ) level=MAJOR ;;
    esac

    local msg="${op_type} ${level} ${user} ${start_time} ${addr} ${pkg_name} ${status}"
    if [[ ${op_type} == Install ]]; then
        msg+=" install_type=${install_type};"
    fi

    [[ -f ${LOG_FILE_OPERATION} ]] || {
        [[ -d ${LOG_FILE_OPERATION%/*} ]] || {
            mkdir -p ${LOG_FILE_OPERATION%/*}
            chmod 750 ${LOG_FILE_OPERATION%/*}
        }
        touch ${LOG_FILE_OPERATION}
        chmod 640 ${LOG_FILE_OPERATION}
    }

    echo "${msg} cmdlist=(${cmdline})." >> ${LOG_FILE_OPERATION}
}

version_lt() {
    local ver1=(${1//./ })
    local ver2=(${2//./ })
    local i v1 v2
    for i in {0..3}; do
        v1=${ver1[${i}]}
        v2=${ver2[${i}]}
        [[ ${v1} == ${v2} ]] && continue
        [[ -z ${v1} ]] && return 0
        [[ -z ${v2} ]] && return 1
        if [[ ${v1}${v2} =~ ^[0-9]+$ ]]; then
            (( ${v1} < ${v2} )) && return 0 || return 1
        else
            [[ ${v1} < ${v2} ]] && return 0 || return 1
        fi
    done
    return 1
}

_try_save_cache() {
    # Cannot use chattr here in the run partition
    local content=$(cat)
    [[ -n  ${content} ]] || return 1
    chmod +w "${cache}" &>/dev/null || mkdir -p "${cache%/*}"
    echo "${content}" > ${cache} 2>/dev/null
    chmod -w "${cache}"
}

_try_load_cache() {
    if [[ -f ${cache} ]]; then
        cat "${cache}"
    fi
}

get_card_phy_numbers() {
    ls /dev/davinci[0-9]* 2> /dev/null | grep -o '[0-9][0-9]*'
}

# Call the given getter function on all cards in parallel to get their
# information, the information obtained will be sorted by card number.
# Arguments:
#   $1 -- The getter function
get_all_card_info_by() {
    local getter="$1"
    local cards=($(get_card_phy_numbers))
    (( ${#cards[@]} > 0 )) || {
        logger_warning -q 'No npu device is in place'
        return 1
    }
    (
        for card in "${cards[@]}"; do
            echo "${card}:$(${getter} ${card})" &
        done
        wait
    ) | sort -t: -nk1 | awk -vrc=1 '/:./{rc=0}1; END{exit rc}'
}


save_flash_version_cache() {
    local cache="${FLASH_VERSION_CACHE}"
    logger_eval timeout 10s "${UPGRADE_TOOL}" --device_index -1 --hw_base_version | _try_save_cache
}

parse_flash_versions() {
    sed -rn 's/.*"'${package}'"\s*:\s*(\w+)\s*[,}].*/\1/p' | grep '.'
}

get_flash_versions() {
    local cache="${FLASH_VERSION_CACHE}"
    local output

    output=$(logger_eval timeout 10s "${UPGRADE_TOOL}" --device_index -1 --hw_base_version 2>/dev/null)
    case $? in
        127 ) return 127 ;;  # File does not exist
    esac

    if echo "${output}" | grep -q 'Unknown argument'; then
        logger_debug "An inappropriate version of upgrade-tool is being used"
        return 0
    fi
    echo "${output}" | parse_flash_versions && return 0
    # Try to parse flash versions from the cache file
    _try_load_cache | parse_flash_versions
}

# Get the maximum required driver versions of all NPU stored in flash
get_max_flash_version() {
    local package=driver
    local max_version=0
    local ver versions
    versions=($(get_flash_versions))
    if (( ${#versions[@]} == 0 )); then
        logger_warning -q "Failed to get required ${package} versions"
        return 0
    fi
    if [[ ${versions[@]} =~ crc ]]; then
        logger_error -c "Required ${package} version crc error"
        return 2
    fi
    for ver in "${versions[@]}"; do
        if (( ver > max_version )); then
            max_version=${ver}
        fi
    done
    logger_debug "Flash required ${package} versions: max=${max_version}, all=(${versions[@]})"
    max_flash_version=${max_version}
}

: <<__commented_code__
parse_image_version() {
    local magic version
    read magic version < <(
        hexdump -s 0x2084 -n 8 "$1" \
            | head -n 1 \
            | awk '{print $2, $3}' 2>/dev/null
    )
    [[ ${magic} == 00a5 ]] || return 1
    echo $((0x${version}))  # Convert hexadecimal to decimal
}
__commented_code__

# Choice of byte reading utils:
#   hexdump <- util-linux
#   od      <- coreutils
parse_image_version() {
    local magic version
    read magic version < <(
        od -d -j 0x2084 -N 8 "$1" \
            | head -n 1 \
            | awk '{print $2, $3}' 2>/dev/null
    )
    [[ ${magic} == 165 ]] && echo ${version}  # 165 == 0x00a5
}

# Get the minimum capability version of image files in the package being installed
get_min_image_version() {
    local min_version=65535
    local img version
    local image_versions=()

    if [[ ${PACKAGE_TYPE} == deb ]] && [[ -f ${DPKG_STATUS_FILE} ]]; then
        get_deb_package_capability
        return $?
    fi

    for img in "${DRIVER_DIR_INSTALLING}"/device/*; do
        # Skip itrustee.img, ts_agent.ko & ascend_cloud_v2.crl
        [[ ${img} =~ (itrustee\.img|\.ko|\.crl)$ ]] && {
            logger_debug "Skip reading capability version of file ${img}"
            continue
        }
        [[ -e ${img} ]] || break
        version=$(parse_image_version "${img}") || {
            logger_error -q "Cannot get capability version of the image ${img}"
            return 1
        }
        if (( version < min_version )); then
            min_version=${version}
        fi
        image_versions+=("${version} ${img#${PWD}/}")
    done
    local indented_version_list=$(
        IFS=$'\n'
        echo
        echo "${image_versions[*]}" | sed -r 's/^/    /'
    )
    logger_debug "The image capability versions:${indented_version_list}"
    logger_debug "The minimum capability version: ${min_version}"

    min_image_version=${min_version}
}

get_deb_package_field() {
    awk "/^$1:/"'{print $2}' ${DPKG_STATUS_FILE} | sed 's/\r//' | grep '.'
}

get_deb_package_capability() {
    local cap=$(get_deb_package_field 'Version-Capability')
    if [[ -n ${cap} ]]; then
        min_image_version=${cap}
        logger_debug "The deb package capability version: ${cap}"
        return 0
    else
        logger_error -q "Cannot get capability version of the deb package"
        return 1
    fi
}

# Get the minimum version whose capability version number >= max_flash_version
get_min_required_version() {
    local cap min_cap=0XFFFF
    local ver min_ver
    # The format of each line in version_cap.map:
    #   <capability>,<version>
    while read cap ver; do
        if (( cap >= max_flash_version && cap < min_cap )); then
            min_cap=${cap}
            min_ver=${ver}
        fi
    done < <(sed 's/\r//;s/#.*$//;/^$/d' "$1" | awk -F, '$1=$1')
    if [[ ${min_ver} ]]; then
        echo ${min_ver}
        return 0
    fi
    return 1
}

# Get the target package version being installed
get_installing_pkg_version() {
    if [[ -f ./version.info ]]; then
        awk -F= '/^[Vv]ersion=/{print $2}' ./version.info
        return 0
    elif [[ ${PACKAGE_TYPE} == deb ]] && [[ -f ${DPKG_STATUS_FILE} ]]; then
        get_deb_package_field 'Version' && return 0
    fi
    echo 'none'
}

# Print rollback interception prompt information
show_version_suggestion() {
    local config="${DRIVER_DIR_INSTALLED}/script/version_cap.map"
    local required_version min_required_version
    if [[ -f ${config} ]]; then
        min_required_version=$(get_min_required_version "${config}") || {
            logger_warning -q "Invalid capability version config file ${config#${PWD}/}"
            logger_warning -q "Cannot get minimum required driver version"
        }
    else
        logger_warning -q "${config#${PWD}/} does not exist"
    fi
    if [[ ${min_required_version} ]]; then
        required_version="version ${min_required_version} or later"
    else
        required_version="a newer version"
    fi
    logger_error -q "The driver version ${PACKAGE_VERSION} is not allowed in this device."
    logger_error -c "Version check does not match, please use ${required_version}."
}

fail_exit() {
    logger_debug -q "Upgrade check failed (error=$1)"
    logger_error -q "Installation is stopped by interception rules"
    logger_error -c "Driver upgrade failed, details in ${LOG_FILE}"
    log_interception
    exit 1
}

is_910b() { lspci -nn | grep -q '19e5:d802'; }

is_910_93() { lspci -nn | grep -q '19e5:d803'; }

is_910b_or_910_93() {
    lspci -nn | grep -q '19e5:d80[23]'
}


# Returns:
#  - 0 means no interception is performed.
#  - otherwise it means the installation should be stopped.
check_rollback_interception_by_flash() {
    is_910b_or_910_93 || return 0

    local lost_card=no
    local max_flash_version min_image_version

    get_max_flash_version || return 2
    (( max_flash_version == 0 )) && return 0
    get_min_image_version || {
        show_version_suggestion
        return 1
    }
    if (( min_image_version < max_flash_version )); then
        show_version_suggestion
        return 1
    fi
    return 0
}


save_cdr_manufacturers_cache() {
    local cache="${CDR_MANUFACTURERS_CACHE}"
    is_910b || return 0
    get_all_card_info_by get_cdr_manufacturer | _try_save_cache
}

get_cdr_manufacturer() {
    timeout 10 hccn_tool -i ${card} -scdr -g 2>/dev/null\
        | sed -rn "s/.*Manufacturer:\s*(\w+)/\1/p;
                   s/.*is not supported on.*/NA/p"
}

get_cdr_manufacturers() {
    local cache="${CDR_MANUFACTURERS_CACHE}"
    get_all_card_info_by get_cdr_manufacturer && return 0
    logger_warning -q "Cannot get cdr manufacturers, try to use cache"
    _try_load_cache
}

check_rollback_interception_by_cdr() {
    local base_version
    local base_version_tr6=23.0.7
    local base_version_trunk=24.1.rc2.2
    local manufacturers

    is_910b || return 0

    manufacturers=($(get_cdr_manufacturers))
    logger_debug "CDR manufacturers: ${manufacturers[@]}"
    [[ ${manufacturers[@]} =~ HUYANG024 ]] || return 0
    # There are new cdrs
    if [[ ${PACKAGE_VERSION} =~ ^[0-9]+\.[0-9]+\.[0-9].*$ ]]; then
        base_version=${base_version_tr6}
    else
        base_version=${base_version_trunk}
    fi
    compare_and_tell_base_version
}

compare_and_tell_base_version() {
    version_lt ${PACKAGE_VERSION} ${base_version} || return 0
    logger_error -q "The driver version ${PACKAGE_VERSION} is not allowed in this device."
    logger_error -c "Version check does not match, please use ${base_version} or later."
    return 1
}


get_board_id() {
    timeout 20s npu-smi info -t board -i $1 -c 0 2>/dev/null \
        | awk '/Board ID/{print $NF}'
}

get_hbm_info() {
    timeout 20s npu-smi info -t memory -i $1 -c 0 2>/dev/null \
        | awk '/HBM (Clock Speed|Manufacturer ID)/{r=$NF (r ? ","r : "")}
                END{print r}'
}

get_board_and_hbm_info() {
    local board_id hbm_info
    exec 3< <(get_hbm_info $1)
    read board_id < <(get_board_id $1)
    read hbm_info <&3
    [[ -n ${board_id}${hbm_info} ]] && echo "${board_id},${hbm_info}"
}

save_hbm_anti_rollback_info_cache() {
    local cache="${HBM_INFORMATION_CACHE}"
    is_910b || return 0
    get_all_card_info_by get_board_and_hbm_info | _try_save_cache
}

get_hbm_anti_rollback_info() {
    local cache="${HBM_INFORMATION_CACHE}"
    get_all_card_info_by get_board_and_hbm_info && return 0
    logger_warning -q "Cannot get hbm information, try to use cache"
    _try_load_cache
}

check_rollback_interception_by_hbm() {
    is_910b || return 0
    local base_version
    local base_version_tr6=23.0.3
    local base_version_trunk=24.0.rc1
    local hbm_info_arr=($(get_hbm_anti_rollback_info))

    logger_debug "HBM information: ${hbm_info_arr[@]}"
    [[ ${hbm_info_arr[@]} =~ :0x31,0x56,1600 ]] || return 0

    if [[ ${PACKAGE_VERSION} =~ ^[0-9]+\.[0-9]+\.[0-9].*$ ]]; then
        base_version=${base_version_tr6}
    elif [[ ${PACKAGE_VERSION} =~ ^[0-9]+\.[0-9]+\.rc[0-9].*$ ]]; then
        base_version=${base_version_trunk}
    else
        return 0
    fi
    compare_and_tell_base_version
}


cache_hardware_version_info() {
    local cache="${FLASH_VERSION_CACHE%/*}/timestamp"
    local curr_ts=$(date '+%s')
    local cache_ts=$(_try_load_cache)
    (( curr_ts - cache_ts >= 60 )) || return 0
    date '+%s' | _try_save_cache
    logger_debug 'Saving hardware version information'
    save_flash_version_cache
    save_cdr_manufacturers_cache
    save_hbm_anti_rollback_info_cache
    date '+%s' | _try_save_cache
}


execute_upgrade_checks() {
    logger_debug 'Start executing upgrade check'

    PACKAGE_TYPE=$1
    PACKAGE_VERSION=$(get_installing_pkg_version | tr '[:upper:]' '[:lower:]')
    logger_debug "The package version being installed is ${PACKAGE_VERSION}"

    # Execute upgrade checks in sequence, exit if any one fails
    check_rollback_interception_by_flash || fail_exit $?
    check_rollback_interception_by_cdr || fail_exit $?
    check_rollback_interception_by_hbm || fail_exit $?

    logger_debug 'Upgrade check successful'
}

source /usr/local/Ascend/driver/bin/setenv.bash

case $1 in
    -c ) execute_upgrade_checks $2 ;;
    -s ) cache_hardware_version_info ;;
    * )
        echo "Bad usage, valid options are -c/-s"
        exit 1;
        ;;
esac