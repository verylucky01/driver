# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

macro(get_host_linux_distributor)
    # Distributor ID:	Ubuntu
    find_program(LSB_RELEASE_PROGRAM lsb_release)
    if(LSB_RELEASE_PROGRAM)
        execute_process(COMMAND bash -c "lsb_release -i | sed 's@Distributor ID:\t@@g'"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE HOST_LINUX_DISTRIBUTOR_ID
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (result)
            set(HOST_LINUX_DISTRIBUTOR_ID unknown)
        endif()
    elseif(EXISTS "/etc/os-release")
        execute_process(COMMAND bash -c "cat /etc/os-release | grep '^NAME' | awk -F '\"' '{print \$2}'"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE HOST_LINUX_DISTRIBUTOR_ID
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (result)
            set(HOST_LINUX_DISTRIBUTOR_ID unknown)
        endif()
    endif()
    string(TOLOWER ${HOST_LINUX_DISTRIBUTOR_ID} HOST_LINUX_DISTRIBUTOR_ID)

    if ((HOST_LINUX_DISTRIBUTOR_ID STREQUAL "euleros") OR (HOST_LINUX_DISTRIBUTOR_ID STREQUAL "debian"))
        # EulerOS特殊处理，拼接Release和Codename，下例中版本号为2.8
        # Distributor ID:	EulerOS
        # Description:	EulerOS release 2.0 (SP8)
        # Release:	2.0
        # Codename:	SP8
        execute_process(COMMAND bash -c "lsb_release -r | sed 's@[a-zA-Z:\t ]@@g' | awk -F. '{print $1}'"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE major_version
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (result)
            set(major_version 0)
        endif()

        execute_process(COMMAND bash -c "lsb_release -c | sed 's@[a-zA-Z:\t ]@@g' | awk -F '' '{print $0}' | cut -b 1-2"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE minor_version
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if ((result) OR (minor_version STREQUAL ""))
            set(minor_version 0)
        endif()

        set(HOST_LINUX_DISTRIBUTOR_RELEASE "${major_version}.${minor_version}")
    else()
        # 保留主版本号和次版本号，删除修订版本号
        # Release:	18.04
        # Release:	7.6.1810
        if(LSB_RELEASE_PROGRAM)
            execute_process(COMMAND bash -c "lsb_release -r | sed 's@[a-zA-Z:\t ]@@g' | awk -F. '{print $1 \".\" $2}' "
            RESULT_VARIABLE result
            OUTPUT_VARIABLE HOST_LINUX_DISTRIBUTOR_RELEASE
            OUTPUT_STRIP_TRAILING_WHITESPACE)
            if (result)
                set(HOST_LINUX_DISTRIBUTOR_RELEASE 0.0)
            endif()
        elseif(EXISTS "/etc/os-release")
            execute_process(COMMAND bash -c "cat /etc/os-release | grep '^VERSION_ID' | awk -F '\"' '{print \$2}'"
                RESULT_VARIABLE result
                OUTPUT_VARIABLE HOST_LINUX_DISTRIBUTOR_RELEASE
                OUTPUT_STRIP_TRAILING_WHITESPACE)
            if (result)
                set(HOST_LINUX_DISTRIBUTOR_RELEASE unknown)
            endif()
        endif()
    endif()

endmacro(get_host_linux_distributor)

macro(get_host_kernel_path)
    if (DEFINED CUSTOM_KERNEL_PATH)
        set(HOST_KERNEL_PATH ${CUSTOM_KERNEL_PATH} CACHE PATH "host kernel path")
    endif()
endmacro(get_host_kernel_path)