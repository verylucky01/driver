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
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
installInfo="/etc/ascend_install.info"
crldir="/root/ascend_check"
crltool="unknow"
driverCrlStatusFile="$crldir/driver_crl_status_tmp"
nvcntFile="$crldir/nvcnt_tmp"
image_nvcnt_get_flag=0
image_nvcnt=0
sourcedir="$PWD"/driver
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
COMMON_SHELL="$SHELL_DIR/common.sh"
# load common.sh, get install.info
source "${COMMON_SHELL}"
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
first_flag=y
upgrade_crl_log="/var/log/upgradetool/upgrade_crl.log"

# user's crl varibles
user_crl_dir="${Driver_Install_Path_Param}"/CMS
user_root_cer="${user_crl_dir}"/user.xer
user_root_crl="${user_crl_dir}"/user.crl
cp_from_original_path="n"

CRL_OPERATION_LOGDIR="/var/log/upgradetool"
CRL_OPERATION_LOGPATH="${CRL_OPERATION_LOGDIR}/upgrade_crl.log"
LOG_LEVEL_SUGGESTION="SUGGESTION"
LOG_LEVEL_MINOR="MINOR"
LOG_LEVEL_MAJOR="MAJOR"
MODULE="DEVICE_CRL_CHECK"

start_time=$(date +"%Y-%m-%d %H:%M:%S")
log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1" >> $logFile
}

logOperationCrl() {
    local operation="$1"
    local level="$2"
    local start_time="$3"
    local module="$4"
    local result="$5"

    if [ ! -d "${CRL_OPERATION_LOGDIR}" ]; then
        mkdir -p ${CRL_OPERATION_LOGDIR}
        chmod 700 ${CRL_OPERATION_LOGDIR}
    fi

    if [ ! -f "${CRL_OPERATION_LOGPATH}" ]; then
        touch ${CRL_OPERATION_LOGPATH}
        chmod 600 ${CRL_OPERATION_LOGPATH}
    fi

    echo "${operation} ${level} root ${start_time} 127.0.0.1 ${module} ${result}" >> ${CRL_OPERATION_LOGPATH}
}

device_images_nvcnt_check() {
    local image_nvcnt_tmp=0
    local ret=0
    if [ -f $crldir/$crltool_bin ]; then
        crltool=$crldir/$crltool_bin
    else
        crltool="$sourcedir"/tools/$crltool_bin
    fi
    if [ ! -f $nvcntFile ];then
        touch $nvcntFile
        chmod 600 $nvcntFile
        log "[INFO]touch $nvcntFile success"
    fi

    "${crltool}" --help 2>/dev/null | grep -w "4" | grep -wq "get nvcnt from image"
    if [ $? -ne 0 ]; then
        log "[WARNING]ascend_check.bin not support nvcnt check, ignore this command."
        return 0
    fi

    # use crltool get nvcnt ,input param :$1:input type(0:get crl file, 1:compare crl newer, 2:check vaild,3:verification signature,4:get nvcnt), 
    # $2:image type(0:cms type,1:soc type),$3:image, $4:nvcnt file
    # return value: 0-success, 1-cannot get nvcnt, 2-get nvcnt failed
    "$crltool" 4 0 "$sourcedir"/device/$source_tee $nvcntFile > /dev/null 2>&1
    ret=$?
    if [ $ret -ne 0 ];then
        if [ $ret -eq 1 ];then
            image_nvcnt_get_flag=1
            log "[INFO]not get nvcnt ,set image_nvcnt_get_flag=1"
        else
            log "[ERROR]get nvcnt failed,ret =$ret ,stop upgrade"
            return 1
        fi
    else
        . $nvcntFile
        image_nvcnt=$IMAGE_NVCNT
        image_nvcnt_get_flag=0
        log "[INFO]image_nvcnt=$IMAGE_NVCNT ,set image_nvcnt_get_flag=0"
    fi
    for image in $image_bin
    do
        "$crltool"  4 0 "$sourcedir"/device/$image $nvcntFile > /dev/null 2>&1
        ret=$?
        if [ $ret -ne 0 ];then
            if [ $ret -eq 1 ];then
                if [ $image_nvcnt_get_flag -ne 1 ];then
                    log "[ERROR]$image_nvcnt_get_flag not equal 1 ,stop upgrade"
                    return 1
                fi 
            else
                log "[ERROR]get nvcnt failed,ret =$ret ,stop upgrade"
                return 1
            fi
        else
            if [ $image_nvcnt_get_flag -ne 0 ];then
                log "[ERROR]$image_nvcnt_get_flag not equal 0,stop upgrade"
                return 1
            fi
            . $nvcntFile
            image_nvcnt_tmp=$IMAGE_NVCNT
            if [ $image_nvcnt -ne $image_nvcnt_tmp ];then
                log "[ERROR]$source_tee nvcnt:$image_nvcnt,$image nvcnt:$image_nvcnt_tmp"
                return 1
            fi
        fi
    done
    return 0
}
# case 1: First installation scenario
gen_cur_image_crl_check_valid() {
    local ret=0
    log "[INFO]case one start: generate new images crl, check vaild and cms "

    if [ ! -f $crldir/$crlFile ] ;then
        touch $crldir/$crlFile
        chmod 400 $crldir/$crlFile
        log "[INFO]touch $crlFile success"
    fi
    if [ -f $crldir/$crltool_bin ]; then
         crltool=$crldir/$crltool_bin
    else
         crltool="$sourcedir"/tools/$crltool_bin
    fi
    # get images crl , input param:$1:input type(0:get crl file), $2:images, $3:crl file
    # return value: 0-success,others-fail
    "$crltool" 0 "$sourcedir"/device/$source_tee $crldir/$crlFile >& /dev/null && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        log "[INFO]1.cannot generate new images crl, direct install"
        rm -rf $crldir/$crlFile
        return 0
    fi
    log "[INFO]1.generate new images crl success , start check valid"
    # check crl vaild, input param: $1:input type, $2: crl file
    # return value: 0-success, others:check invalid
    "$crltool" 2  $crldir/$crlFile >& /dev/null && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        rm -rf ${crldir}/${crlFile}
        # if return 6, which means that the PSS signature verification failed.
        if [ ${ret} -eq 6 ]; then
            log "[ERROR]PSS signature verification failed, stop install"
            return 2
        else
            log "[ERROR]2.check new images crl failed, stop install"
            return 1
        fi
    fi
    log "[INFO]2.check new images crl success,start check cms"
    # Verification signature ,input param: $1:input type, $2:newer crl, $3:dest images
    # return value: 0- success,others- fail
    "$crltool" 3  $crldir/$crlFile  "$sourcedir"/device/$source_tee >& /dev/null && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        rm -rf ${crldir}/${crlFile}
        if [ ${ret} -eq 6 ]; then
            log "[ERROR]PSS signature verification failed, stop install"
            return 2
        else
            log "[ERROR]3.check new images crl cms failed, stop install"
            return 1
        fi
    fi
    log "[INFO]3.check new images crl cms success, update tool and crl"
    if [ -f $crldir/$crltool_bin ]; then
        rm -f $crldir/$crltool_bin
    fi
    cp -f "$sourcedir"/tools/$crltool_bin $crldir/$crltool_bin
    chmod 500 $crldir/$crltool_bin
    cp -f "$sourcedir"/tools/ascend_upgrade_crl.sh $crldir/ascend_upgrade_crl.sh
    chmod 500 $crldir/ascend_upgrade_crl.sh
    log "[INFO]4.update /root/ascend_check/$crltool_bin, ascend_upgrade_crl.sh and crl success"
    log "[INFO]case one end: generate new images crl, check vaild and cms "
    logOperationCrl "UPGRADE" "${LOG_LEVEL_MAJOR}" "${start_time}" "${MODULE}" "upgrade $crltool_bin,crl file and ascend_upgrade_crl.sh success"
    return 0

}
# case 2:not first installation scenario and crl exist in/root/ascend_check/
gen_cur_image_crl_compare_newer() {
    local ret=0
    local new_tool_flag=0
    log "[INFO]case two start: generate new images crl, compare newer and check cms "

    if [ ! -f $crldir/$crltool_bin ]; then
        cp -f $sourcedir/tools/$crltool_bin $crldir/$crltool_bin

        chmod 500 $crldir/$crltool_bin
        cp -f "$sourcedir"/tools/ascend_upgrade_crl.sh $crldir/ascend_upgrade_crl.sh
        chmod 500 $crldir/ascend_upgrade_crl.sh
        new_tool_flag=1
        log "[INFO]use new $crltool_bin and ascend_upgrade_crl.sh in runpackage"
        logOperationCrl "UPGRADE" "${LOG_LEVEL_MINOR}" "${start_time}" "${MODULE}" "upgrade $crltool_bin, ascend_upgrade_crl.sh success"
    fi
    touch $crldir/$curcrlFile
    chmod 400 $crldir/$curcrlFile
    # get images crl , input param:$1:input type, $2:images, $3:crl file
    # return value: 0-success,others-fail
    $crldir/$crltool_bin 0 "$sourcedir"/device/$source_tee $crldir/$curcrlFile >& /dev/null && ret=$? || ret=$? 
    if [ ${ret} -ne 0 ]; then
        rm -rf ${crldir}/${curcrlFile}
        if [ ${ret} -eq 6 ]; then
            log "[ERROR]PSS signature verification failed, stop install"
            return 2
        else
            log "[ERROR]1.generate new images crl fail, stop install"
            return 1
        fi
    fi
    log "[INFO]1.generate new images crl success, need check if newer"
    # compare the new and old crl , input param:$1:input type(1:compare crl newer), $2:new images, $3:old images
    # return value: 0-SCPS_SAME,1-SCPS_NEW,2-SCPS_OLD,3-SCPS_MIX
    $crldir/$crltool_bin 1 $crldir/$curcrlFile $crldir/$crlFile >& /dev/null && ret=$? || ret=$? 
    if [ $ret -eq 0 ] || [ $ret -eq 1 ] || [ $ret -eq 5 ] ; then
         log "[INFO]2.ret=$ret,$curcrlFile same or newer, start check cms"
         # Verification signature ,input param: $1:input type, $2:newer crl, $3:dest images
         # return value: 0- success,others- fail
         $crldir/$crltool_bin 3 $crldir/$curcrlFile "$sourcedir"/device/$source_tee >& /dev/null && ret=$? || ret=$?
         if [ ${ret} -ne 0 ]; then
            rm -rf ${crldir}/${curcrlFile}
            if [ ${ret} -eq 6 ]; then
                log "[ERROR]PSS signature verification failed, stop install"
                return 2
            else
                log "[ERROR]3.check $curcrlFile cms failed, stop install"
                return 1
            fi
         fi
         log "[INFO]3.check $curcrlFile cms  success, update tool and crl"
         # update crl tool and crl
         rm -rf $crldir/$crlFile
         mv $crldir/$curcrlFile $crldir/$crlFile
         if [ $new_tool_flag -eq 0 ]; then
            rm -f $crldir/$crltool_bin
            cp -f "$sourcedir"/tools/$crltool_bin $crldir/$crltool_bin
            chmod 500 $crldir/$crltool_bin
            cp -f "$sourcedir"/tools/ascend_upgrade_crl.sh $crldir/ascend_upgrade_crl.sh
            chmod 500 $crldir/ascend_upgrade_crl.sh
            log "[INFO]update $crltool_bin and ascend_upgrade_crl.sh in runpackage"
            logOperationCrl "UPGRADE" "${LOG_LEVEL_MINOR}" "${start_time}" "${MODULE}" "upgrade $crltool_bin, ascend_upgrade_crl.sh success"
         fi
         logOperationCrl "UPGRADE" "${LOG_LEVEL_MAJOR}" "${start_time}" "${MODULE}" "upgrade crl file success"
         log "[INFO]4.update $crltool_bin and crl success"
    elif [ $ret -eq 2 ] || [ $ret -eq 4 ]; then
         log "[INFO]2.ret=$ret,$curcrlFile older, start check cms"
         rm -rf $crldir/$curcrlFile
         $crldir/$crltool_bin 3 $crldir/$crlFile "$sourcedir"/device/$source_tee > /dev/null 2>&1
         if [ $? -ne 0 ]; then
             log "[ERROR]3.check $crlFile cms failed,stop install"
             return 1
         fi
         # if local crl is newer, it will upgrade tool and ascend_upgrade_crl.sh.
         cp -f "$sourcedir"/tools/$crltool_bin $crldir/$crltool_bin
         chmod 500 $crldir/$crltool_bin
         cp -f "$sourcedir"/tools/ascend_upgrade_crl.sh $crldir/ascend_upgrade_crl.sh
         chmod 500 $crldir/ascend_upgrade_crl.sh
         log "[INFO]3.check $crlFile cms  success, not upgrade crl"
    elif [ ${ret} -eq 6 ]; then
        log "[ERROR]PSS signature verification failed, stop install"
        rm -rf $crldir/$curcrlFile
        return 2
    else
        log "[ERROR]2.ret=$ret,$curcrlFile not newer or both crl completeness valid check fail, stop update images"
        rm -rf $crldir/$curcrlFile
        return 1
    fi
    log "[INFO]case two end: generate new images crl, compare newer and check cms "
    return 0
}
# case 3: not first installation scenario but crl not exist in/root/ascend_check/ 
gen_old_image_crl() {
    log "[INFO]case three start: generate old images crl, then choose to case one or case two"
    if [ ! -f "$Driver_Install_Path_Param"/driver/device/$pcie_tee ];then
        # not exist old images, convert to case 1
        gen_cur_image_crl_check_valid && ret=$? || ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_check_valid failed"
            return ${ret}
        fi
        log "[INFO]gen_cur_image_crl_check_valid success"
        return 0
    fi
    touch $crldir/$crlFile
    chmod 400 $crldir/$crlFile
    log "[INFO]touch $crlFile success"
    if [ -f $crldir/$crltool_bin ]; then
         crltool=$crldir/$crltool_bin
    else
         crltool="$sourcedir"/tools/$crltool_bin
    fi
    "$crltool" 0 "$Driver_Install_Path_Param"/driver/device/$pcie_tee $crldir/$crlFile >& /dev/null && ret=$? || ret=$? 
    if [ ${ret} -ne 0 ]; then
        log "[INFO]old images not exist crl, check new images, choose to case one "
        rm -rf $crldir/$crlFile
        gen_cur_image_crl_check_valid && ret=$? || ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_check_valid failed"
            return ${ret}
        fi
        log "[INFO]gen_cur_image_crl_check_valid success"
        return 0
    else
        log "[INFO]old images exist crl, compare if cur crl newer,choose to case two"
        gen_cur_image_crl_compare_newer && ret=$? || ret=$? 
        if [ ${ret} -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_compare_newer failed"
            return ${ret}
        fi
        log "[INFO]gen_cur_image_crl_compare_newer success"
        return 0
    fi
    log "[INFO]case three end: generate old images crl, then choose to case one or case two"
}

rename_tool_and_crl_file() {
    mv $crldir/ascend_7*_check.bin $crldir/$crltool_bin >& /dev/null
    mv $crldir/ascend7*.crl  $crldir/$crlFile >& /dev/null

    if [ -f "${crldir}/${crltool_bin}" ]; then
        cp -f ${sourcedir}/tools/${crltool_bin} ${crldir}/${crltool_bin}
        chmod -f 500 ${crldir}/${crltool_bin}
        cp -f ${sourcedir}/tools/ascend_upgrade_crl.sh ${crldir}/ascend_upgrade_crl.sh
        chmod -f 500 ${crldir}/ascend_upgrade_crl.sh
    fi
}

hw_crl_check() {
    crltool="$crldir/$crltool_bin"

    rename_tool_and_crl_file
    if [ "$feature_nvcnt_check"x = "y"x ]; then
        device_images_nvcnt_check
        if [ $? -ne 0 ];then
            if [  -f $nvcntFile ];then
                rm -f $nvcntFile
                log "[INFO]rm $nvcntFile success"
            fi
            log "[ERROR]device_images_nvcnt_check failed, stop upgrade"
            return 1
        else
            if [  -f $nvcntFile ];then
                rm -f $nvcntFile
                log "[INFO]rm $nvcntFile success"
            fi
            log "[INFO]device_images_nvcnt_check success"
        fi
    fi

    if [ -f $crldir/$crlFile ]; then
        local crl_size=$(ls -al $crldir/$crlFile | awk '{ print $5 }')
        if [ $crl_size = "0" ]; then
            log "[INFO]crl file size is 0, delete it"
            rm $crldir/$crlFile
        fi
        log "[INFO] crl file size check success"
    fi

    if [ "$first_flag" = "y" ] && [ ! -f $crldir/$crlFile ]; then
        gen_cur_image_crl_check_valid && ret=$? || ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_check_valid failed"
            logOperationCrl "UPGRADE" "${LOG_LEVEL_SUGGESTION}" "${start_time}" "${MODULE}" "crl check failed ,not upgrade $crltool_bin,crl file and ascend_upgrade_crl.sh"
            return ${ret}
        fi
        log "[INFO]first_flag=y,gen_cur_image_crl_check_valid success"
        return 0
    fi
    if [ -f $crldir/$crlFile ]; then
         gen_cur_image_crl_compare_newer && ret=$? || ret=$?
         if [ ${ret} -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_compare_newer failed"
            logOperationCrl "UPGRADE" "${LOG_LEVEL_SUGGESTION}" "${start_time}" "${MODULE}" "crl check failed ,not upgrade $crltool_bin,crl file and ascend_upgrade_crl.sh"
            return ${ret}
         fi
         log "[INFO]gen_cur_image_crl_compare_newer success"
         return 0
    else
        gen_old_image_crl && ret=$? || ret=$?
        if [ ${ret} -ne 0 ]; then
            log "[ERROR]gen_old_image_crl fail, install failed"
            logOperationCrl "UPGRADE" "${LOG_LEVEL_SUGGESTION}" "${start_time}" "${MODULE}" "crl check failed ,not upgrade $crltool_bin,crl file and ascend_upgrade_crl.sh"
            return ${ret}
        fi
    fi
    return 0
}

device_images_crl_check() {
    local ret=0

    # get $crltool_bin, $crlFile $curcrlFile from  specific_func.inc
    . ./driver/script/specific_func.inc
    if [ ! -d $crldir ]; then
        mkdir -p $crldir
        chmod 500 $crldir
        log "[INFO]mkdir $crldir success"
    fi

    # check so files.
    if [ ! -d "$Driver_Install_Path_Param"/driver/lib64 ];then
        log "[WARNING]crl check lack of dependent so files"
        return 0
    fi

    lsmod | grep "drv_pcie_host " > /dev/null 2>&1 && first_flag=n

    # if checked before, it will return directly.
    [ -f "${driverCrlStatusFile}" ] && log "[INFO]device images crl have check before, stop check again" && return 0 || touch ${driverCrlStatusFile}
    # load so
    export LD_LIBRARY_PATH="${sourcedir}"/lib64/:"${sourcedir}"/lib64/common:"${sourcedir}"/lib64/driver:${LD_LIBRARY_PATH}

    hw_crl_check || { ret=$? && log "[ERROR]hw_crl_check failed." && return ${ret}; }

    return 0
}
