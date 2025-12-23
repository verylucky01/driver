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

sourcedir_path=""
driver_file_copy() {

    # Prevent empty directories from occupying the disk space.
    if [ "${PACKAGE_TYPE}" = "run" ];then
        rm -f /etc/hdcBasic.cfg
        if [ "$installType"x != "docker"x ]; then
            rm -rf /usr/local/bin/npu-smi
            rm -rf /usr/local/sbin/npu-smi
        fi
    elif [ "${PACKAGE_TYPE}" = "deb" ]; then
        if [ -d /usr/local/bin/npu-smi ]; then
            rm -rf /usr/local/bin/npu-smi
        fi
        if [ -d /usr/local/sbin/npu-smi ]; then
            rm -rf /usr/local/sbin/npu-smi
        fi
    fi

    if [ "${PACKAGE_TYPE}" = "run" ];then
        sourcedir_path=$sourcedir/..
    else
        sourcedir_path=$SHELL_DIR
    fi

    if [ "${PACKAGE_TYPE}" = "run" ];then
        if [[ $input_install_for_all =~ n|no ]]; then
            "$sourcedir_path"/install_common_parser.sh --makedir --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" "$sourcedir_path"/filelist.csv
        else
            "$sourcedir_path"/install_common_parser.sh --makedir --install_for_all --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" "$sourcedir_path"/filelist.csv
        fi
        if [ $? -ne 0 ];then
            log "[ERROR]driver mkdir failed"
            return 1
        fi
        if [[ $input_install_for_all =~ n|no ]]; then
            "$sourcedir_path"/install_common_parser.sh --copy --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" "$sourcedir_path"/filelist.csv
        else
            "$sourcedir_path"/install_common_parser.sh --copy --install_for_all --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" "$sourcedir_path"/filelist.csv
        fi
        if [ $? -ne 0 ];then
            log "[ERROR]driver copy failed"
            return 1
        fi
    fi
    # set bash env
    BASH_ENV_PATH="${Driver_Install_Path_Param}/driver/bin/setenv.bash"
    if [ -f "install_common_parser.sh" ] && [ -f "${BASH_ENV_PATH}" ]; then
        "$sourcedir_path"/install_common_parser.sh --add-env-rc --package="driver" --username="${username}" --usergroup="${usergroup}" --setenv "$installPathParam" "${BASH_ENV_PATH}" "bash"
        if [ $? -ne 0 ]; then
            log "[ERROR]ERR_NO:0x0089;ERR_DES:failed to set bash env."
            return 1
        fi
    fi
    if [[ $input_install_for_all =~ n|no ]]; then
        "$sourcedir_path"/install_common_parser.sh --chmoddir --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" "$sourcedir_path"/filelist.csv
    else
        "$sourcedir_path"/install_common_parser.sh --chmoddir --install_for_all --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" "$sourcedir_path"/filelist.csv
    fi
    if [ $? -ne 0 ];then
        log "[ERROR]driver chmoddir failed"
        return 1
    fi

    # driver/kernel
    if [ -d "${Driver_Install_Path_Param}"/driver/kernel ]; then
        chmod -Rf 400 "${Driver_Install_Path_Param}"/driver/kernel
        find "${Driver_Install_Path_Param}"/driver/kernel -type d -name "*" | xargs chmod 500
    fi

    return 0
}