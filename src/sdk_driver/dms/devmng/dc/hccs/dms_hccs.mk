# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
ifeq ($(DAVINCI_HIAI_DKMS),y)
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/dc/hccs
	asdrv_dms-y += devmng/dc/hccs/dms_hccs_init.o
	asdrv_dms-y += devmng/dc/hccs/dms_hccs_credit.o
else #for CMake
	EXTRA_CFLAGS += -I$(DRIVER_MODULE_DEVMNG_DIR)/dc/hccs
	asdrv_dms-y += devmng/dc/hccs/dms_hccs_init.o
	asdrv_dms-y += devmng/dc/hccs/dms_hccs_credit.o
endif