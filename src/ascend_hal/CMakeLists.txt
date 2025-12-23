# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
# build libascend_hal.so
add_subdirectory(build)

add_subdirectory(bbox)
add_subdirectory(esched)
add_subdirectory(hdc)
add_subdirectory(queue)
add_subdirectory(buff)
add_subdirectory(dmc)
add_subdirectory(pbl)
add_subdirectory(dms)
add_subdirectory(svm)
add_subdirectory(dpa)
add_subdirectory(trs)
add_subdirectory(queryfeature)
add_subdirectory(commlib)
add_subdirectory(roce)
add_subdirectory(mmpa)
add_subdirectory(msnpureport)