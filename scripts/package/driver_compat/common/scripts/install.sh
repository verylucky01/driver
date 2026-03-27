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

# error number and description
OPERATE_FAILED="0x0001"
PARAM_INVALID="0x0002"
FILE_NOT_EXIST="0x0080"
FILE_NOT_EXIST_DES="File not found."
PERM_DENIED="0x0093"
PERM_DENIED_DES="Permission denied."

CURR_OPERATE_USER="$(id -nu 2>/dev/null)"
CURR_OPERATE_GROUP="$(id -ng 2>/dev/null)"
# defaults for general user
if [ "$(id -u)" != "0" ]; then
  DEFAULT_INSTALL_PATH="${HOME}/Ascend"
else
  DEFAULT_INSTALL_PATH="/usr/local/Ascend"
fi

if [ "$(id -u)" != "0" ]; then
  _LOG_PATH=$(echo "${HOME}")"/var/log/ascend_seclog"
  _INSTALL_LOG_FILE="${_LOG_PATH}/ascend_install.log"
else
  _LOG_PATH="/var/log/ascend_seclog"
  _INSTALL_LOG_FILE="${_LOG_PATH}/ascend_install.log"
fi

# run package's files info, CURR_PATH means current temp path
RUN_FILE_NAME=$(expr substr "$1" 5 $(expr ${#1} - 4))

CURR_PATH=$(dirname $(readlink -f $0))
. "${CURR_PATH}/common_func.inc"

RUN_PKG_INFO_FILE="${CURR_PATH}/../scene.info"
VERSION_INFO_FILE="${CURR_PATH}/../version.info"

ARCH_INFO=$(grep -e "arch" "$RUN_PKG_INFO_FILE" | cut --only-delimited -d"=" -f2-)

if [ "$(id -u)" != "0" ]; then
  INSTALL_INFO_PERM="600"
else
  INSTALL_INFO_PERM="644"
fi

DRIVER_COMPAT_DIR=driver-compat
ASCEND_INSTALL_INFO="ascend_install.info"
TARGET_INSTALL_PATH="${DEFAULT_INSTALL_PATH}" #--input-path
TARGET_USERNAME="${CURR_OPERATE_USER}"
TARGET_USERGROUP="${CURR_OPERATE_GROUP}"
TARGET_VERSION_DIR="" # TARGET_INSTALL_PATH + PKG_VERSION_DIR
TARGET_SHARED_INFO_DIR=""

# keys of infos in ascend_install.info
KEY_INSTALLED_UNAME="Driver_Compat_UserName"
KEY_INSTALLED_UGROUP="Driver_Compat_UserGroup"
KEY_INSTALLED_TYPE="Driver_Compat_Install_Type"
KEY_INSTALLED_PATH="Driver_Compat_Install_Path"
KEY_INSTALLED_VERSION="Driver_Compat_Version"

# init install cmd status, set default as n
CMD_LIST="$*"
IS_UNINSTALL=n
IS_INSTALL=n
IS_UPGRADE=n
IS_QUIET=n
IS_INPUT_PATH=n
IS_CHECK=n
IS_PRE_CHECK=n
IN_INSTALL_TYPE=""
IN_INSTALL_PATH=""
IS_DOCKER_INSTALL=n
IS_FOR_ALL=n
IS_SETENV=n
IS_JIT=n
DOCKER_ROOT=""
CONFLICT_CMD_NUMS=0
IN_FEATURE="All"

# log functions
getdate() {
  _cur_date=$(date +"%Y-%m-%d %H:%M:%S")
  echo "${_cur_date}"
}

logandprint() {
  is_error_level=$(echo $1 | grep -E 'ERROR|WARN|INFO')
  if [ "${is_quiet}" != "y" ] || [ "${is_error_level}" != "" ]; then
    echo "[Driver-compat] [$(getdate)] ""$1"
  fi
  echo "[Driver-compat] [$(getdate)] ""$1" >>"${_INSTALL_LOG_FILE}"
}

# log functions
# start info before shell executing
startlog() {
  echo "[Driver-compat] [$(getdate)] [INFO]: Start Time: $(getdate)"
}

exitlog() {
  echo "[Driver-compat] [$(getdate)] [INFO]: End Time: $(getdate)"
}

#check ascend_install.info for the change in code warning
get_installed_info() {
  local key="$1"
  local res=""
  if [ -f "${INSTALL_INFO_FILE}" ]; then
    chmod 644 "${INSTALL_INFO_FILE}" >/dev/null 2>&1
    res=$(cat ${INSTALL_INFO_FILE} | grep "${key}" | awk -F = '{print $2}')
  fi
  echo "${res}"
}

clean_before_reinstall() {
  local installed_path=$(get_installed_info "${KEY_INSTALLED_PATH}")
  local existed_files=$(find ${TARGET_VERSION_DIR}/${DRIVER_COMPAT_DIR} -type f -print 2>/dev/null)
  if [ -z "${existed_files}" ]; then
    logandprint "[INFO]: Directory is empty, directly install Driver-compat."
    return 0
  fi

  if [ "${IS_QUIET}" = "y" ]; then
    logandprint "[WARNING]: Directory has file existed or installed driver\
 compat, are you sure to keep installing Driver-compat in it? y"
  else
    if [ ! -f "${INSTALL_INFO_FILE}" ]; then
      logandprint "[INFO]: Directory has file existed, do you want to continue? [y/n]"
    else
      logandprint "[INFO]: Driver-compat package has been installed on the path ${TARGET_VERSION_DIR},\
 the version is $(get_installed_info "${KEY_INSTALLED_VERSION}"),\
 and the version of this package is ${RUN_PKG_VERSION}, do you want to continue? [y/n]"
    fi
    while true; do
      read yn
      if [ "$yn" = "n" ]; then
        logandprint "[INFO]: Exit to install Driver-compat."
        exitlog
        exit 0
      elif [ "$yn" = "y" ]; then
        break
      else
        echo "[WARNING]: Input error, please input y or n to choose!"
      fi
    done
  fi

  if [ "${installed_path}" = "${TARGET_VERSION_DIR}" ]; then
    logandprint "[INFO]: Clean the installed Driver-compat before install."
    uninstall_shell_file=${TARGET_VERSION_DIR}/${DRIVER_COMPAT_DIR}/script/uninstall.sh
    if [ ! -f "${uninstall_shell_file}" ]; then
      logandprint "[ERROR]: ERR_NO:${FILE_NOT_EXIST};ERR_DES:${FILE_NOT_EXIST_DES}.The file\
 (${uninstall_shell_file}) not exists. Please set the correct install \
 path or clean the previous version Driver-compat install info (${INSTALL_INFO_FILE}) and then reinstall it."
      return 1
    fi
    bash "${uninstall_shell_file}"
    if [ "$?" != 0 ]; then
      logandprint "[ERROR]: ERR_NO:${OPERATE_FAILED};ERR_DES:Clean the installed directory failed."
      return 1
    fi
  fi
  return 0
}

check_before_unInstall() {
  local existed_files=""
  existed_files=$(find ${TARGET_VERSION_DIR}/${DRIVER_COMPAT_DIR} -type f -print 2>/dev/null)
  if [ -z "${existed_files}" ]; then
    logandprint "[ERROR]: ERR_NO:${FILE_NOT_EXIST}:Runfile is not installed in ${TARGET_VERSION_DIR}, uninstall failed."
    comm_log_operation "Uninstall" "${RUN_FILE_NAME}" "failed" "${IN_INSTALL_TYPE}" "${CMD_LIST}"
    exitlog
    exit 1
  fi
}

check_docker_path() {
  docker_path="$1"
  if [ "${docker_path}" != "/"* ]; then
    echo "[Driver-compat] [ERROR]: ERR_NO:${PARAM_INVALID};ERR_DES:Parameter --docker-root\
 must with absolute path that which is start with root directory /. Such as --docker-root=/${docker_path}"
    exitlog
    exit 1
  fi

  judgment_path "${docker_path}"

  if [ ! -d "${docker_path}" ]; then
    echo "[Driver-compat] [ERROR]: ERR_NO:${FILE_NOT_EXIST}; The directory:${docker_path} not exist, please create this directory."
    exitlog
    exit 1
  fi
}

judgment_path() {
  check_install_path_valid "${1}"
  if [ $? -ne 0 ]; then
    echo "[Driver-compat][ERROR]: The Driver-compat install path ${1} is invalid, only characters in [a-z,A-Z,0-9,-,_] are supported!"
    exitlog
    exit 1
  fi
}

check_install_path() {
  TARGET_INSTALL_PATH="$1"
  # empty patch check
  if [ "x${TARGET_INSTALL_PATH}" = "x" ]; then
    echo "[Driver-compat] [ERROR]: ERR_NO:${PARAM_INVALID};ERR_DES:Parameter --install-path\
 not support that the install path is empty."
    exitlog
    exit 1
  fi
  # space check
  if echo "x${TARGET_INSTALL_PATH}" | grep -q " "; then
    echo "[Driver-compat] [ERROR]: ERR_NO:${PARAM_INVALID};ERR_DES:Parameter --install-path\
 not support that the install path contains space character."
    exitlog
    exit 1
  fi
  # delete last "/"
  local temp_path="${TARGET_INSTALL_PATH}"
  temp_path=$(echo "${temp_path%/}")
  if [ x"${temp_path}" = "x" ]; then
    temp_path="/"
  fi
  # covert relative path to absolute path
  local prefix=$(echo "${temp_path}" | cut -d"/" -f1 | cut -d"~" -f1)
  if [ "x${prefix}" = "x" ]; then
    TARGET_INSTALL_PATH="${temp_path}"
  else
    prefix=$(echo "${RUN_PATH}" | cut -d"/" -f1 | cut -d"~" -f1)
    if [ x"${prefix}" = "x" ]; then
      TARGET_INSTALL_PATH="${RUN_PATH}/${temp_path}"
    else
      echo "[Driver-compat] [ERROR]: ERR_NO:${PARAM_INVALID};ERR_DES: Run package path is invalid: $RUN_PATH"
      exitlog
      exit 1
    fi
  fi
  # covert '~' to home path
  local home=$(echo "${TARGET_INSTALL_PATH}" | cut -d"~" -f1)
  if [ "x${home}" = "x" ]; then
    local temp_path_value=$(echo "${TARGET_INSTALL_PATH}" | cut -d"~" -f2)
    if [ "$(id -u)" -eq 0 ]; then
      TARGET_INSTALL_PATH="/root$temp_path_value"
    else
      local home_path=$(eval echo "${USER}")
      home_path=$(echo "${home_path}%/")
      TARGET_INSTALL_PATH="$home_path$temp_path_value"
    fi
  fi
}

# execute prereq_check file
exec_pre_check() {
  bash "${PRE_CHECK_FILE}"
}

# execute prereq_check file and interact with user
interact_pre_check() {
  exec_pre_check
  if [ "$?" != 0 ]; then
    if [ "${IS_QUIET}" = y ]; then
      logandprint "[WARNING]: Precheck of Driver-compat execute failed! do you want to continue install? y"
    else
      logandprint "[WARNING]: Precheck of Driver-compat execute failed! do you want to continue install?  [y/n] "
      while true; do
        read yn
        if [ "$yn" = "n" ]; then
          echo "stop install Driver-compat!"
          exit 1
        elif [ "$yn" = y ]; then
          break
        else
          echo "[WARNING]: Input error, please input y or n to choose!"
        fi
      done
    fi
  fi
}

#get the dir of xxx.run
#install_path_curr=`echo "$2" | cut -d"/" -f2- `
# cut first two params from *.run
get_run_path() {
  RUN_PATH=$(echo "$2" | cut -d"-" -f3-)
  if [ x"${RUN_PATH}" = x"" ]; then
    RUN_PATH=$(pwd)
  else
    # delete last "/"
    RUN_PATH=$(echo "${RUN_PATH%/}")
    if [ "x${RUN_PATH}" = "x" ]; then
      # root path
      RUN_PATH=$(pwd)
    fi
  fi
}

check_arch() {
  local architecture=$(uname -m)
  # check platform
  if [ "${architecture}" != "${ARCH_INFO}" ]; then
    logandprint "[ERROR]: ERR_NO:${OPERATE_FAILED};ERR_DES:the architecture of the run package arch:${ARCH_INFO}\
  is inconsistent with that of the current environment ${architecture}. "
    exitlog
    exit 1
  fi
}

check_chmod_length() {
  local mod_num="$1"
  local new_mod_num=""
  local mod_num_length=$(expr length "$mod_num")
  if [ "$mod_num_length" -eq 3 ]; then
      new_mod_num="$mod_num"
      echo "$new_mod_num"
  elif [ "$mod_num_length" -eq 4 ]; then
      new_mod_num="$(expr substr $mod_num 2 3)"
      echo "$new_mod_num" 
  fi    
}

check_install_for_all() {
    local mod_num=""
    local other_mod_num=""
    if [ "$IS_FOR_ALL" = "y" ] && [ -d "${TARGET_INSTALL_PATH}" ]; then
        mod_num="$(stat -c %a ${TARGET_INSTALL_PATH})"
        mod_num="$(check_chmod_length $mod_num)"
        other_mod_num="$(expr substr $mod_num 3 1)"
        if [ "${other_mod_num}" -ne 5 ] && [ "${other_mod_num}" -ne 7 ]; then
            logandprint "[ERROR]: ERR_NO:${OPERATE_FAILED};ERR_DES: ${TARGET_INSTALL_PATH} permission is ${mod_num}, this permission does not support install_for_all param."
            exitlog
            exit 1
        fi
    fi
}

get_opts() {
  i=0
  while true
  do
    if [ "x$1" = "x" ]; then
      break
    fi
    if [ "$(expr substr "$1" 1 2)" = "--" ]; then
      i=$(expr $i + 1)
    fi
    if [ $i -gt 2 ]; then
      break
    fi
    shift 1
  done

  if [ "$*" = "" ]; then
    echo "[ERROR]: ERR_NO:${PARAM_INVALID}; ERR_DES:Unrecognized parameters.Try './xxx.run --help for more information.'"
    exitlog
    exit 1
  fi

  CMD_LIST="$*"

  while true; do
    # skip 2 parameters avoid run pkg and directory as input parameter
    case "$1" in
      --run)
        IN_INSTALL_TYPE=$(echo ${1} | awk -F"--" '{print $2}')
        IS_INSTALL="y"
        CONFLICT_CMD_NUMS=$(expr $CONFLICT_CMD_NUMS + 1)
        shift
        ;;
      --uninstall)
        IN_INSTALL_TYPE=$(echo ${1} | awk -F"--" '{print $2}')
        IS_UNINSTALL="y"
        CONFLICT_CMD_NUMS=$(expr $CONFLICT_CMD_NUMS + 1)
        shift
        ;;
      --docker-root=*)
        IS_DOCKER_INSTALL=y
        DOCKER_ROOT=$(echo ${1} | cut -d"=" -f2-)
        check_docker_path "${DOCKER_ROOT}"
        shift
        ;;
      --install-path=*)
        IS_INPUT_PATH="y"
        IN_INSTALL_PATH=$(echo ${1} | cut -d"=" -f2-)
        # check path
        judgment_path "${IN_INSTALL_PATH}"
        check_install_path "${IN_INSTALL_PATH}"
        shift
        ;;
      --quiet)
        IS_QUIET="y"
        shift
        ;;
      --install-for-all)
        IS_FOR_ALL="y"
        shift
        ;;
      --extract=*)
        CONFLICT_CMD_NUMS=$(expr $CONFLICT_CMD_NUMS + 1)
        shift;
        ;;
      --keep)
        shift;
        ;;
      --check)
        CONFLICT_CMD_NUMS=$(expr $CONFLICT_CMD_NUMS + 1)
        check=y
        shift
        ;;
      -*)
        echo "[Driver-compat] [ERROR]: ERR_NO:${PARAM_INVALID};ERR_DES:Unsupported parameters [$1],\
 operation execute failed. Please use [--help] to see the usage."
        exitlog
        exit 1
        ;;
      *)
        break
        ;;
    esac
  done
}

# pre-check
check_opts() {
  if [ "${CONFLICT_CMD_NUMS}" != 1 ]; then
    echo "[Driver-compat] [ERROR]: ERR_NO:${PARAM_INVALID};ERR_DES:\
 only support one type: run/uninstall, operation execute failed!\
 Please use [--help] to see the usage."
    exitlog
    exit 1
  fi
}

# init target_dir and log for install
init_env() {
  TARGET_VERSION_DIR="$TARGET_INSTALL_PATH"
  # Splicing docker-root and install-path
  if [ "${IS_DOCKER_INSTALL}" = "y" ]; then
    # delete last "/"
    local temp_path_param="${DOCKER_ROOT}"
    local temp_path_val=$(echo "${temp_path_param%/}")
    if [ "x${temp_path_val}" = "x" ]; then
      temp_path_val="/"
    fi
    TARGET_VERSION_DIR=${temp_path_val}${TARGET_VERSION_DIR}
  fi

  INSTALL_INFO_FILE="${TARGET_VERSION_DIR}/driver-compat/${ASCEND_INSTALL_INFO}"

  logandprint "[INFO]: Execute the Driver-compat run package."
  logandprint "[INFO]: OperationLogFile path: ${COMM_LOGFILE}."
  logandprint "[INFO]: Input params: $CMD_LIST"

  get_package_version "RUN_PKG_VERSION" "$VERSION_INFO_FILE"
  local installed_version=$(get_installed_info "${KEY_INSTALLED_VERSION}")
  if [ "${installed_version}" = "" ]; then
    logandprint "[INFO]: Version of installing Driver-compat is ${RUN_PKG_VERSION}."
  else
    if [ "${RUN_PKG_VERSION}" != "" ]; then
      logandprint "[INFO]: Existed Driver-compat version is ${installed_version},\
 the new Driver-compat version is ${RUN_PKG_VERSION}."
    fi
  fi
}

check_pre_install() {
  local installed_user=$(get_installed_info "${KEY_INSTALLED_UNAME}")
  local installed_group=$(get_installed_info "${KEY_INSTALLED_UGROUP}")
  if [ "${installed_user}" != "" ] || [ "${installed_group}" != "" ]; then
    if [ "${installed_user}" != "${TARGET_USERNAME}" ] || [ "${installed_group}" != "${TARGET_USERGROUP}" ]; then
      logandprint "[ERROR]: The user and group are not same with last installation,\
do not support overwriting installation!"
      exitlog
      exit 1
    fi
  fi

}

#Support the installation script when the specified path (relative path and absolute path) does not exist
mkdir_install_path() {
  if [ "${IS_INSTALL}" = "n" ]; then
    return
  fi

  local base_dir=$(dirname ${TARGET_VERSION_DIR})
  if [ ! -d ${base_dir} ]; then
    logandprint "[ERROR]: ERR_NO:${FILE_NOT_EXIST}; The directory:${base_dir} not exist, please create this directory."
    exitlog
    exit 1
  fi

  if [ -d "${TARGET_VERSION_DIR}" ]; then
    test -w ${TARGET_VERSION_DIR} >>/dev/null 2>&1
    if [ "$?" -ne 0 ]; then
      #All paths exist with write permission
      logandprint "[ERROR]: ERR_NO:${PERM_DENIED};ERR_DES:${PERM_DENIED_DES}. The ${TARGET_USERNAME} do\
 access ${TARGET_VERSION_DIR} failed, please reset the directory to a right permission."
      exit 1
    fi
  else
    test -w ${base_dir} >>/dev/null 2>&1
    if [ "$?" -ne 0 ]; then
      #All paths exist with write permission
      logandprint "[ERROR]: ERR_NO:${PERM_DENIED};ERR_DES:${PERM_DENIED_DES}. The ${TARGET_USERNAME} do\
 access ${base_dir} failed, please reset the directory to a right permission."
      exit 1
    else
      comm_create_dir "${TARGET_VERSION_DIR}" "750" "${TARGET_USERNAME}:${TARGET_USERGROUP}" "${IS_FOR_ALL}"
    fi
  fi
}

update_install_info() {
  local key_val="$1"
  local val="$2"
  local old_val=$(get_installed_info "${key_val}")
  if [ -f "${INSTALL_INFO_FILE}" ]; then
    if [ "x${old_val}" = "x" ]; then
      echo "${key_val}=${val}" >>"${INSTALL_INFO_FILE}"
    else
      sed -i "/${key_val}/c ${key_val}=${val}" "${INSTALL_INFO_FILE}"
    fi
  else
    echo "${key_val}=${val}" >"${INSTALL_INFO_FILE}"
  fi
}

update_install_infos() {
  local uname="$1"
  local ugroup="$2"
  local type="$3"
  local path="$4"
  local version
  get_package_version "version" "$VERSION_INFO_FILE"
  comm_create_file "${INSTALL_INFO_FILE}" "${INSTALL_INFO_PERM}" "${TARGET_USERNAME}:${TARGET_USERGROUP}" "${IS_FOR_ALL}"

  update_install_info "${KEY_INSTALLED_UNAME}" "${uname}"
  update_install_info "${KEY_INSTALLED_UGROUP}" "${ugroup}"
  update_install_info "${KEY_INSTALLED_TYPE}" "${type}"
  update_install_info "${KEY_INSTALLED_PATH}" "${path}"
  update_install_info "${KEY_INSTALLED_VERSION}" "${version}"
}

driver_compat_file_copy() {
    if [ "${IS_FOR_ALL}" = "n" ]; then
        bash ${CURR_PATH}/install_common_parser.sh --makedir --username="$CURR_OPERATE_USER" --usergroup="$CURR_OPERATE_GROUP" "$IN_INSTALL_TYPE" "$TARGET_VERSION_DIR" ${CURR_PATH}/filelist.csv
    else
        bash ${CURR_PATH}/install_common_parser.sh --makedir --install_for_all --username="$CURR_OPERATE_USER" --usergroup="$CURR_OPERATE_GROUP" "$IN_INSTALL_TYPE" "$TARGET_VERSION_DIR" ${CURR_PATH}/filelist.csv
    fi
    if [ $? -ne 0 ];then
        logandprint "[ERROR]Driver-compat package mkdir failed"
        return 1
    fi
    if [ "${IS_FOR_ALL}" = "n" ]; then
        bash ${CURR_PATH}/install_common_parser.sh --copy --username="$CURR_OPERATE_USER" --usergroup="$CURR_OPERATE_GROUP" "$IN_INSTALL_TYPE" "$TARGET_VERSION_DIR" ${CURR_PATH}/filelist.csv
    else
        bash ${CURR_PATH}/install_common_parser.sh --copy --install_for_all --username="$CURR_OPERATE_USER" --usergroup="$CURR_OPERATE_GROUP" "$IN_INSTALL_TYPE" "$TARGET_VERSION_DIR" ${CURR_PATH}/filelist.csv
    fi
    if [ $? -ne 0 ];then
        logandprint "[ERROR]Driver-compat package copy failed"
        return 1
    fi

    if [ "${IS_FOR_ALL}" = "n" ]; then
        bash ${CURR_PATH}/install_common_parser.sh --chmoddir --username="$CURR_OPERATE_USER" --usergroup="$CURR_OPERATE_GROUP" "$IN_INSTALL_TYPE" "$TARGET_VERSION_DIR" ${CURR_PATH}/filelist.csv
    else
        bash ${CURR_PATH}/install_common_parser.sh --chmoddir --install_for_all --username="$CURR_OPERATE_USER" --usergroup="$CURR_OPERATE_GROUP" "$IN_INSTALL_TYPE" "$TARGET_VERSION_DIR" ${CURR_PATH}/filelist.csv
    fi
    if [ $? -ne 0 ];then
        logandprint "[ERROR]Driver-compat package chmoddir failed"
        return 1
    fi

    return 0
}

install_package() {
  if [ "${IS_INSTALL}" = "n" ]; then
    return
  fi

  # use uninstall to clean the install folder
  clean_before_reinstall
  if [ "$?" != 0 ]; then
    comm_log_operation "Install" "${RUN_FILE_NAME}" "failed" "${IN_INSTALL_TYPE}" "${CMD_LIST}"
    exitlog
    exit 1;
  fi

  logandprint "[INFO]: Install to target dir ${TARGET_VERSION_DIR}"

  driver_compat_file_copy

  if [ $? -eq 0 ]; then
    update_install_infos "${TARGET_USERNAME}" "${TARGET_USERGROUP}" "${IN_INSTALL_TYPE}" "${TARGET_INSTALL_PATH}"
    logandprint "[INFO]: Driver-compat package installed successfully! The new version takes effect immediately."
    echo "Please make sure that
        - LD_LIBRARY_PATH includes ${TARGET_VERSION_DIR}/${DRIVER_COMPAT_DIR}/lib64"
    comm_log_operation "Install" "${RUN_FILE_NAME}" "succeeded" "${IN_INSTALL_TYPE}" "${CMD_LIST}"
  else
    logandprint "[ERROR]: Driver-compat package install failed, please retry after uninstall!"
    comm_log_operation "Install" "${RUN_FILE_NAME}" "failed" "${IN_INSTALL_TYPE}" "${CMD_LIST}"
  fi
  exitlog
}

uninstall_package() {
  if [ "${IS_UNINSTALL}" = "n" ]; then
    return
  fi
  logandprint "[INFO]: Uninstall target dir ${TARGET_VERSION_DIR}"

  check_before_unInstall

  bash ${CURR_PATH}/install_common_parser.sh  --package="driver-compat" --remove-install-info --uninstall --username="${CURR_OPERATE_USER}" --usergroup="${CURR_OPERATE_GROUP}" --docker-root="$DOCKER_ROOT" "$IN_INSTALL_TYPE" "$TARGET_INSTALL_PATH" ${CURR_PATH}/filelist.csv
  if [ $? -eq 0 ]; then
    remove_dir_if_empty "${TARGET_VERSION_DIR}"
    logandprint "[INFO]: Driver-compat package uninstall successfully! Uninstallation takes effect immediately."
    comm_log_operation "Uninstall" "${RUN_FILE_NAME}" "succeeded" "${IN_INSTALL_TYPE}" "${CMD_LIST}"
  else
    logandprint "[ERROR]: Driver-compat uninstall failed, please retry after uninstall!"
    comm_log_operation "Uninstall" "${RUN_FILE_NAME}" "failed" "${IN_INSTALL_TYPE}" "${CMD_LIST}"
  fi
  exitlog
}

main() {
  comm_init_log

  check_arch

  get_run_path "$@"

  startlog

  get_opts "$@"

  check_opts

  init_env

  check_install_for_all

  check_pre_install

  mkdir_install_path

  install_package

  uninstall_package
}

main "$@"
exit 0

