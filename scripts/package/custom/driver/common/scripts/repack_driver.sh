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

scriptRepackFileCheck() {
    HEAD_SH="$tmpdir/driver/script/makeself-header.sh"
    MAKESELF_PATH="$tmpdir/driver/script/makeself.sh"
    HELP_INFO="$tmpdir/driver/script/help.info"
    INSTALL_SH="driver/script/install.sh"
    REBUILD_SH="$tmpdir/driver/script/run_driver_ko_rebuild.sh"
    REBUILD_LOG_DIR="/var/log/ascend_seclog"
    REBUILD_LOG="${REBUILD_LOG_DIR}/ascend_rebuild.log"

    if [ ! -e "${HEAD_SH}" ];then
        echo >&2
        echo "[ERROR]header script: ${HEAD_SH} no exist, please check." >&2
        return 1
    fi
    chmod +x ${HEAD_SH}
    if [ ! -e "${MAKESELF_PATH}" ];then
        echo >&2
        echo "[ERROR]makeself script: ${MAKESELF_PATH} no exist, please check." >&2
        return 1
    fi
    chmod +x ${MAKESELF_PATH}

    which pigz >/dev/null
    if [ $? -ne 0 ];then
        echo >&2
        echo "[ERROR]can't find pigz, please check whether the pigz is installed." >&2
        return 1
    fi

    if [ ! -e ${REBUILD_SH} ];then
        echo >&2
        echo "[ERROR]can't find rebuild scripts, please check package is support rebuild." >&2
        return 1
    fi
    if [ ! -d ${REBUILD_LOG_DIR} ];then
        mkdir -p ${REBUILD_LOG_DIR}
    fi
    return 0
}

doRepack() {
    echo "Target repack package will be ${packagename_base}"

    cd "${pwd_of_file}"
    scriptRepackFileCheck
    if [ $? -ne 0 ];then
        echo "[ERROR]repackage file check failed." >&2
        return 1
    fi

    if [ "${norebuild}" == "y" ]; then
        bash ${REBUILD_SH} --norebuild
    else
        bash ${REBUILD_SH} --force
    fi
    if [ $? -ne 0 ];then
        echo "[ERROR]rebuild failed." >&2
        return 2
    fi

    if test x"$quiet" = xy; then
        QUIET_ARGS="--quiet"
    fi
    bash ${MAKESELF_PATH} ${QUIET_ARGS} --header ${HEAD_SH} --help-header ${HELP_INFO} --pigz --complevel 4 --nomd5 --sha256 \
        "$tmpdir" "${packagename}" "ASCEND DRIVER RUN PACKAGE" ${INSTALL_SH} >> ${REBUILD_LOG}
    if [ $? -ne 0 ];then
        echo "[ERROR]repack failed." >&2
        return 3
    fi

    echo "Repack success."
    return 0
}


doRepackSp() {
    echo "Target repack package will be ${packagename_base}"

    cd "${pwd_of_file}"
    scriptRepackFileCheck
    if [ $? -ne 0 ];then
        echo "[ERROR]repackage file check failed." >&2
        return 1
    fi

    if test x"$quiet" = xy; then
        QUIET_ARGS="--quiet"
    fi
    bash ${MAKESELF_PATH} ${QUIET_ARGS} --header ${HEAD_SH} --help-header ${HELP_INFO} --pigz --complevel 4 --nomd5 --sha256 \
        "$tmpdir" "${packagename}" "ASCEND DRIVER RUN PACKAGE" ${INSTALL_SH} >> ${REBUILD_LOG}
    if [ $? -ne 0 ];then
        echo "[ERROR]repack failed." >&2
        return 3
    fi

    echo "Repack success."
    return 0
}

# Script entry

if test x"$repack" = xy; then
    tmpdir="${PWD}"
    if test x"${packagedir}" != x; then
        if [[ ! "$packagedir" =~ ^/.* ]]; then
        packagedir="${runPackagePath}/${packagedir}"
        fi
        if [ ! -d "${packagedir}" ]; then
            echo "[ERROR]Target directory ${packagedir} not exists, aborting." >&2
            exit 1
        fi
        tmpdir=${packagedir}
    fi

    packagename_base="${packagename}"
    if test x"$packagename" = x; then
        packagename=`basename ${runfilename}`
        packagename=${packagename%.*}
        packagename_base="${packagename}-custom.run"
        packagename="${runPackagePath}/${packagename_base}"
    else
        if [[ ! "$packagename" =~ ^/.* ]]; then
            packagename="${runPackagePath}/${packagename}"
        fi
        packagename_path=`dirname ${packagename}`
        if [ ! -d "${packagename_path}" ]; then
            echo "[ERROR]Repack target directory ${packagename_path} not exists, aborting." >&2
            exit 1
        fi
    fi

    if [[ ${packagename} =~ ".run" ]];then
        if test -e "$packagename"; then
            echo "[ERROR]Target package $packagename already exists, aborting." >&2
            exit 1
        fi
    else
        echo "[ERROR]Package name: ${packagename}, The package must be in the .run format." >&2
        exit 1
    fi

    if [[ ${packagename} =~ "euleros2.13_sp-aarch64.run" ]] && [ "${ci_envir}"x == "true"x ]; then
        doRepackSp
    else
        doRepack
    fi

    exitLog $?
fi