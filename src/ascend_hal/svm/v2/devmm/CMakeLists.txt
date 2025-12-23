# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
target_sources(devmm_obj PRIVATE
    devmm_svm.c
    devmm_virt_interface.c
    devmm_virt_base_heap.c
    devmm_virt_com_heap.c
    devmm_virt_dvpp_heap.c
    devmm_virt_read_only_heap.c
    devmm_cache_coherence.c
    devmm_map_dev_reserve.c
    svm_mem_statistics.c
    svm_adapt.c
    devmm_host_mem_pool.c
    devmm_proc_event.c
    devmm_dynamic_addr.c
    devmm_record.c
)

add_subdirectory(devmm_rbtree)
