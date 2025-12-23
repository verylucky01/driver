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
DEP_INFO_FILE="/etc/ascend_install.info"

if [[ -f "${DEP_INFO_FILE}" ]]; then
    . ${DEP_INFO_FILE}
    DRV_LIB64_COMMON_LDPATH=""${Driver_Install_Path_Param%*/}"/driver/lib64/common"
    DRV_LIB64_DRV_LDPATH=""${Driver_Install_Path_Param%*/}"/driver/lib64/driver"
    DRV_LIB64_LDPATH=""${Driver_Install_Path_Param%*/}"/driver/lib64"

    export LD_LIBRARY_PATH="${DRV_LIB64_COMMON_LDPATH}":"${DRV_LIB64_DRV_LDPATH}":"${DRV_LIB64_LDPATH}":"${LD_LIBRARY_PATH}"
else
    echo "[WARNING] Could not set env, please check the driver."
fi
