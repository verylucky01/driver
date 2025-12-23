#!/bin/sh
# install_common_parser.sh函数库
# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

set_global_vars() {
    local arch
    local scene_filepath="${curpath}/../scene.info"

    if [ -f "${scene_filepath}" ]; then
        get_scene_arch "arch" "${scene_filepath}"
        if [ "${arch}" != "" ]; then
            PKG_ARCH="${arch}"
        fi
    fi
}

get_thread_num() {
    local _outvar="$1"
    local _thread_num="$(cat /proc/cpuinfo | grep "^processor" | wc -l)"

    if [ "$_thread_num" = "" ] || [ "$_thread_num" = "0" ]; then
        _thread_num="1"
    fi

    eval "${_outvar}=\"$((_thread_num*2))\""
}

init_fifo() {
    local _outvar="$1"
    local _thread_num="$2"
    local _tmppath_if tmp
    local _skip_msg="skip init fifo"

    if [ "$_thread_num" = "" ]; then
        _thread_num="1"
        comm_log "WARNING" "thread number is empty, use ${_thread_num}."
    fi

    if [ $_thread_num -le 0 ]; then
        comm_log "WARNING" "thread number is ${_thread_num}, use 1."
        _thread_num="1"
    fi

    get_tmp_file _tmppath_if "fifo"
    if [ -e "$_tmppath_if" ]; then
        rm -rf "$_tmppath_if"
        check_ret_warning "$?" "remove old file $_tmppath_if failed, ${_skip_msg}."
        ret="$?" && [ $ret -ne 0 ] && return $ret
    fi

    mkfifo "$_tmppath_if"
    check_ret_warning "$?" "make fifo $_tmppath_if failed, ${_skip_msg}."
    ret="$?" && [ $ret -ne 0 ] && return $ret

    exec 8<> "$_tmppath_if"
    check_ret_warning "$?" "open fifo $_tmppath_if file descriptor failed, ${_skip_msg}."
    ret="$?" && [ $ret -ne 0 ] && return $ret

    rm -f "$_tmppath_if"
    check_ret_warning "$?" "remove fifo $_tmppath_if failed, ${_skip_msg}."
    ret="$?" && [ $ret -ne 0 ] && return $ret

    for tmp in $(seq $_thread_num); do
        echo >&8
        check_ret_warning "$?" "echo fifo $_tmppath_if failed, ${_skip_msg}."
        ret="$?" && [ $ret -ne 0 ] && return $ret
    done

    eval "${_outvar}=\"${_tmppath_if}\""
}

exec_with_param() {
    local param="$1"
    shift 1
    local exec_mode fifo_path="$PARALLEL_FIFO" tmp
    extract_1st "exec_mode" "$param"
    if [ "$exec_mode" = "concurrency" ]; then
        if [ "$fifo_path" = "none" ] || [ "$fifo_path" = "" ]; then
            "$@" &
        else
            read tmp <&8
            {
                "$@"
                ret="$?"
                echo >&8
                exit $ret
            } &
        fi
    else
        "$@"
    fi
}
