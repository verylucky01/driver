/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <errno.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/mempolicy.h>
#include "user_log.h"
#include "securec.h"
#include "ascend_hal.h"
#include "ascend_hal_define.h"

static inline unsigned long long align(unsigned long long val, unsigned int align)
{
    return (val + align - 1) & ~(align - 1);
}

int hns_roce_hal_alloc_buf(void **buf, unsigned int *length, unsigned int size, unsigned int page_size, unsigned int dev_id)
{
#define NODE_MASK_LIST_NUM 2
#define MAX_NODE_BIT_NUM 128
#define NODE_LONG_BIT_NUM 64
#define PAGE_ALIGN_2MB (2 * 1024 * 1024)
    unsigned long long node_mask[NODE_MASK_LIST_NUM] = {0};
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    unsigned long long node_index = 0;
    unsigned long long node_cnt, i;
    struct MemInfo info = {0};
    unsigned int max_node = 0;
    int ret;

    if (size == 0 || page_size == 0) {
        roce_err("hns_roce_hal_alloc_buf size %u or page_size %u is zero!", size, page_size);
        return -EINVAL;
    }

    *length = align(size, page_size);

    ret = (int)halMemGetInfo(dev_id, MEM_INFO_TYPE_BAR_NUMA_INFO, &info);
    if (ret) {
        roce_err("hns_roce_hal_alloc_buf: halMemGetInfo failed, ret [%d]", ret);
        return ret;
    }

    node_cnt = (unsigned long long)info.numa_info.node_cnt;
    if (node_cnt > sizeof(info.numa_info.node_id) / sizeof(int)) {
        roce_err("hns_roce_hal_alloc_buf: halMemGetInfom node_cnt 0x%llx", node_cnt);
        return -EINVAL;
    }

    for (i = 0; i < node_cnt; i++) {
        node_index = (unsigned long long)info.numa_info.node_id[i];
        if (node_index >= MAX_NODE_BIT_NUM) {
            roce_err("hns_roce_hal_alloc_buf: halMemGetInfom node_index 0x%llx > %d, failed!", node_index,
                     MAX_NODE_BIT_NUM);
            return -EINVAL;
        }

        max_node = (node_index > max_node) ? node_index : max_node;

        if (node_index >= NODE_LONG_BIT_NUM) {
            node_index -= NODE_LONG_BIT_NUM;
            node_mask[1] |= ((unsigned long long)1 << node_index);
        } else {
            node_mask[0] |= ((unsigned long long)1 << node_index);
        }
    }

    // plus 2 to avoid kernel get_nodes maxnode param high-order truncation bug
    max_node += 2;

    if (page_size == PAGE_ALIGN_2MB) {
        flags |= MAP_HUGETLB;
    }
    *buf = mmap(NULL, *length, PROT_READ | PROT_WRITE, flags, -1, 0);
    if (*buf == MAP_FAILED) {
        roce_err("hns_roce_hal_alloc_buf mmap failed. start addr 0x%llx length 0x%x",
                 (unsigned long long)*buf, *length);
        return -ENOMEM;
    }

    ret = syscall(__NR_mbind, *buf, *length, MPOL_BIND, node_mask, max_node, MPOL_MF_MOVE);
    if (ret != 0) {
        roce_err("hns_roce_hal_alloc_buf mbind failed. start addr 0x%llx length 0x%x, ret = %d\n",
                 (unsigned long long)*buf, *length, ret);
        goto err_unmamp;
    }

    ret = memset_s(*buf, *length, 0, *length);
    if (ret != 0) {
        roce_err("hns_roce_hal_alloc_buf memset_s failed. start addr 0x%llx length 0x%x, ret = %d\n",
                 (unsigned long long)*buf, *length, ret);
        goto err_unmamp;
    }

    return 0;

err_unmamp:
    munmap(*buf, *length);
    *buf = NULL;

    return -ENOMEM;
}

int hns_roce_hal_get_dev_id(unsigned int chip_id, unsigned int die_id, unsigned int *dev_id)
{
    unsigned int *device_list;
    unsigned int dev_num = 0;
    long int chip_id_val = 0;
    long int die_id_val = 0;
    unsigned int i;
    int ret;

    ret = drvGetDevNum(&dev_num);
    if (ret) {
        roce_err("hns_roce_hal_get_dev_id failed!, ret is %d", ret);
        return -EINVAL;
    }

    if (dev_num == 0) {
        roce_err("hns_roce_hal_get_dev_id dev_num is 0, failed!");
        return -EINVAL;
    }

    device_list = (unsigned int *)calloc(dev_num, sizeof(unsigned int));
    if (device_list == NULL) {
        roce_err("hns_roce_hal_get_dev_id failed, calloc device list failed");
        return -ENOMEM;
    }

    ret = drvGetDeviceLocalIDs(device_list, dev_num);
    if (ret) {
        roce_err("hns_roce_hal_get_dev_id failed, drvGetDeviceLocalIDs failed, ret is %d", ret);
        goto out;
    }

    for (i = 0; i < dev_num; i++) {
        ret = halGetDeviceInfo(device_list[i], MODULE_TYPE_SYSTEM, INFO_TYPE_PHY_CHIP_ID, &chip_id_val);
        if (ret) {
            roce_err("hns_roce_hal_get_dev_id get chip_id failed, halGetDeviceInfo failed, ret is %d", ret);
            goto out;
        }

        ret = halGetDeviceInfo(device_list[i], MODULE_TYPE_SYSTEM, INFO_TYPE_PHY_DIE_ID, &die_id_val);
        if (ret) {
            roce_err("hns_roce_hal_get_dev_id get die_id failed, halGetDeviceInfo failed, ret is %d", ret);
            goto out;
        }

        if (chip_id_val == chip_id && die_id_val == die_id) {
            *dev_id = device_list[i];
            goto out;
        }
    }

    *dev_id = 0;
    ret = EINVAL;
    roce_err("hns_roce_hal_get_dev_id failed, dev_id set to 0!");

out:
    free(device_list);
    device_list = NULL;
    return ret;
}

int hns_roce_hal_alloc_ai_buf(void **buf, unsigned int *length, unsigned int size, unsigned int page_size,
    unsigned int dev_id, unsigned int grp_id)
{
    unsigned long flag = 0;
    int ret;

    if (buf == NULL || length == NULL) {
        roce_err("buf is NULL or length is NULL");
        return -EINVAL;
    }

    if (size == 0 || page_size == 0) {
        roce_err("size %u or page_size %u is zero", size, page_size);
        return -EINVAL;
    }

    *length = align(size, page_size);
    flag = ((unsigned long)dev_id << BUFF_FLAGS_DEVID_OFFSET) | BUFF_SP_SVM;

    ret = halBuffAllocAlignEx(*length, page_size, flag, grp_id, buf);
    if (ret != 0) {
        roce_err("halBuffAllocAlignEx failed, length:0x%x, page_size:0x%x, dev_id:0x%x, flag:0x%lx, grp_id:%u, ret:%d",
            *length, page_size, dev_id, flag, grp_id, ret);
    }
    return ret;
}

int hns_roce_hal_free_ai_buf(void *buf)
{
    return halBuffFree(buf);
}
