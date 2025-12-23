/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef _DCMI_FOR_OM_H_
#define _DCMI_FOR_OM_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef int (*FN_GET_CARD_NUM_LIST)(int *, int *, int);
typedef int (*FN_GET_DEVICE_NUM_IN_CARD)(int, int *);
typedef int (*FN_GET_DEVICE_HEALTH)(int, int, unsigned int *);
typedef int (*FN_GET_DEVICE_TEMPERATURE)(int, int, int *);
typedef int (*FN_GET_DEVICE_POWER_INFO)(int, int, int *);
typedef int (*FN_GET_MEMORY_INFO)(int, int, unsigned long long *);
typedef int (*FN_GET_DEVICE_UTILIZATION_RATE)(int, int, int, unsigned int *);
typedef int (*FN_GET_OM_ELABEL_INFO)(int, unsigned char *, unsigned short, unsigned short *);
typedef int (*FN_SET_OM_ELABEL_INFO)(int, unsigned char *, unsigned short, unsigned short);
typedef int (*GET_ELABEL_FUNC_HOOKEE)(FN_GET_OM_ELABEL_INFO, FN_GET_OM_ELABEL_INFO,
                                      FN_GET_OM_ELABEL_INFO, FN_SET_OM_ELABEL_INFO,
                                      FN_GET_OM_ELABEL_INFO);

typedef int (*HAL_NPU_FUNC_HOOKEE)(FN_GET_CARD_NUM_LIST, FN_GET_DEVICE_NUM_IN_CARD, FN_GET_DEVICE_HEALTH,
                                   FN_GET_DEVICE_TEMPERATURE, FN_GET_DEVICE_POWER_INFO, FN_GET_MEMORY_INFO,
                                   FN_GET_DEVICE_UTILIZATION_RATE);

int dcmi_for_om_get_device_memory_size(int card_id, int device_id, unsigned long long *memory_size);
int get_elabel_func_hooker(GET_ELABEL_FUNC_HOOKEE hal_hook_func);
int hal_npu_func_hooker(HAL_NPU_FUNC_HOOKEE hal_hook_func);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* _DCMI_FOR_OM_H_ */