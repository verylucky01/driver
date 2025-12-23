# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

target_include_directories(drvdsmi_host PRIVATE
    ${DRIVER_USER_DIR}/dmc/dsmi/dsmi_product_ext/soc_adapt
    $<$<STREQUAL:${PRODUCT},ascend910>:${DRIVER_USER_DIR}/dmc/dsmi/dsmi_product_ext/soc_adapt/cloud>
    $<$<STREQUAL:${PRODUCT},ascend910B>:${DRIVER_USER_DIR}/dmc/dsmi/dsmi_product_ext/soc_adapt/cloud>
)
