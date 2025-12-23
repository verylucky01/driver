# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

set(DRIVER_TARGETS
    # kernel
    drv_seclib_host
    asdrv_pbl
    drv_vascend_stub
    asdrv_vpc
    drv_pcie_host
    asdrv_vnic
    asdrv_vmng
    drv_pcie_hdc_host
    asdrv_vvpc
    drv_virtmng_host_stub
    asdrv_fms
    asdrv_dms
    asdrv_trsbase
    asdrv_buff
    asdrv_dpa
    ascend_soc_platform
    ascend_kernel_open_adapt
    asdrv_esched
    asdrv_trs
    asdrv_vtrs
    asdrv_svm
    asdrv_queue
    drv_vascend
    ts_agent
    ts_agent_vm

    # user
    ascend_hal
    drvdsmi_host
    hdcBasic_cfg
)

set(DRIVER_CUSTOM_TARGETS
    # kernel
    npu_peermem
    tc_pcidev
    ts_debug
    debug_switch

    # user
    dcmi
    lingqu-dcmi
    dsmi_network
)

set(DRIVER_COMPAT_TARGETS
    c_sec
    ascend_hal
    ascend_rdma_lite
)