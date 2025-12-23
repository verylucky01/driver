# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
target_sources(hdc_obj PRIVATE
    hdc_server.c
    hdc_core.c
    hdc_client.c
    hdc_cfg_parse.c
    hdc_pcie_drv.c
    hdc_ppc.c
    hdc_epoll.c
    hdc_file_trans.c
)

add_subdirectory(pcie)
add_subdirectory(sock)
add_subdirectory(hdc_config)
