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
set -e

dotted_line="----------------------------------------------------------------"
BASE_PATH=$(cd "$(dirname $0)"; pwd)
export BUILD_PATH="${BASE_PATH}/build"
export BUILD_OUT_PATH="${BASE_PATH}/build_out"

usage()
{
    echo "Usage:"
    echo "bash run_test.sh [-h] [-t/--test]"
    echo ""
    echo "Options:"
    echo "    -h/--help                     Print usage"
    echo "    -t/--test                     Build ut"
}

build_ut()
{
    [ ! -d "${BUILD_PATH}" ] || rm -rf ${BUILD_PATH}
    [ ! -d "${BUILD_OUT_PATH}" ] || rm -rf ${BUILD_OUT_PATH}

    mkdir -p ${BUILD_PATH}
    mkdir -p ${BUILD_OUT_PATH}
    cd ${BUILD_PATH}
    cmake -DCMAKE_INSTALL_PREFIX=${BUILD_OUT_PATH} ..
    if [ $? -ne 0 ]; then
        echo "execute command: cmake -DCMAKE_INSTALL_PREFIX=${BUILD_OUT_PATH} .. failed"
        return 1
    fi
    cmake --build . --verbose
    make install
    ${BUILD_OUT_PATH}/bin/npu_driver_utest
}

TEST=""

[ ! $# -eq 0 ] || usage

while [[ $# -gt 0 ]]; do
    case $1 in
    -h|--help)
        usage
        exit 0
        ;;

    -t|--test)
        TEST="true"
        shift
        ;;

    *)
        echo "[ERROR] Undefined option: ${1}"
        usage
        exit 1
        ;;
    esac
done

if [ "${TEST}" == "true" ];then
    build_ut
fi