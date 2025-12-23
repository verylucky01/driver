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

log() {
   local cur_date=`date +"%Y-%m-%d %H:%M:%S"`
   local user_id=`id | awk '{printf $1}'`
   echo "[Driver] [$cur_date] [$user_id] "$1 >> $logFile
}

drvEcho() {
    local cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}

exitLog() {
    local cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    drvEcho "[INFO]End time: ${cur_date}"
    drvEcho "[INFO]End time: ${cur_date}" >> $logFile
    exit $1
}

errorUsage() {
    if [ $# -eq 1 ]; then
        drvEcho "[ERROR]ERR_NO:0x0004;ERR_DES: Unrecognized parameters: ${1}. Try './xxx.run --help' for more information."
        exitLog 1
    elif [ $# -eq 2 ]; then
        log "$2"
        drvEcho "$2"
        exitLog "$1"
    else
        drvEcho "[ERROR]ERR_NO:0x0004;ERR_DES: Unrecognized parameters. Try './xxx.run --help' for more information."
        exit 1
    fi
}

drvColorEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo -e  "[Driver] [$cur_date] $1"
}


startLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    drvEcho "[INFO]Start time: $cur_date"
    drvEcho "[INFO]Start time: $cur_date" >> $logFile
}

logOperation() {
    local operation="$1"
    local starttime="$2"
    local runfilename="$3"
    local op_result="$4"
    local install_type="$5"
    local cmdlist="$6"
    local level

    if [ "${operation}" = "${LOG_OPERATION_UPGRADE}" ]; then
        level="${LOG_LEVEL_MINOR}"
    elif [ "${operation}" = "${LOG_OPERATION_INSTALL}" ]; then
        level="${LOG_LEVEL_SUGGESTION}"
    elif [ "${operation}" = "${LOG_OPERATION_UNINSTALL}" ]; then
        level="${LOG_LEVEL_MAJOR}"
    else
        level="${LOG_LEVEL_UNKNOWN}"
    fi

    if [ ! -f "${OPERATION_LOGPATH}" ]; then
        touch ${OPERATION_LOGPATH}
        setFileChmod -f 640 ${OPERATION_LOGPATH}
    fi

    if [ ! -d "${OPERATION_LOGDIR}" ]; then
        mkdir -p ${OPERATION_LOGDIR}
        setFileChmod -f 750 ${OPERATION_LOGDIR}
    fi

    if [ $upgrade = y ] || [ $uninstall = y ]; then
        echo "${operation} ${level} root ${starttime} 127.0.0.1 ${runfilename} ${op_result}"\
            "cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}
    else
        echo "${operation} ${level} root ${starttime} 127.0.0.1 ${runfilename} ${op_result}"\
            "install_type=${install_type}; cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}
    fi
}