/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <errno.h>

#include "ascend_hal_define.h"

#include "drv_buff_common.h"
#include "grp_mng.h"
#include "buff_user_interface.h"
#include "drv_buff_adp.h"
#include "buff_manage_base.h"
#include "buff_mng.h"

void *buff_base_addr = NULL;
void *buff_end_addr = NULL;
#define BUFF_POOL_ID_MAX 0x7FFFFFFF
THREAD int buffpool_id = BUFF_POOL_ID_MAX;

bool buff_pool_is_ready(void)
{
    return (buff_base_addr != NULL);
}

int buff_get_default_pool_id(void)
{
    return buffpool_id;
}

unsigned long long buff_get_base_addr(void)
{
    return (unsigned long long)(uintptr_t)buff_base_addr;
}

bool is_buff_addr(unsigned long va)
{
    return (va >= (unsigned long)(uintptr_t)buff_base_addr) &&
        (va < (unsigned long)(uintptr_t)buff_end_addr);
}

void *buff_blk_alloc(int pool_id, unsigned long size, unsigned long flag, uint32_t *blk_id)
{
    unsigned long offset;
    drvError_t ret;

    if (!buff_pool_is_ready()) {
        buff_err("buff mng not init\n");
        return NULL;
    }

    ret = buff_pool_blk_alloc(pool_id, size, flag, &offset, blk_id);
    if (ret != DRV_ERROR_NONE) {
        buff_err("pool_id %d alloc failed, size %lx\n", pool_id, size);
        return NULL;
    }

    return (char *)buff_base_addr + offset;
}

void buff_blk_free(int pool_id, void *addr)
{
    unsigned long offset;
    drvError_t ret;

    if (!buff_pool_is_ready()) {
        buff_err("buff mng not init\n");
        return;
    }

    offset = (unsigned long)((uintptr_t)addr - (uintptr_t)buff_base_addr);

    ret = buff_pool_blk_free(pool_id, offset);
    if (ret != DRV_ERROR_NONE) {
        buff_err("pool_id %d free failed, offset %lx\n", pool_id, offset);
    }
}

drvError_t buff_blk_get(void *addr, int *pool_id, void **alloc_addr,
    unsigned long *alloc_size, uint32_t *blk_id)
{
    unsigned long offset, alloc_offset;
    drvError_t ret;

    offset = (unsigned long)((uintptr_t)addr - (uintptr_t)buff_base_addr);

    ret = buff_pool_blk_get(offset, pool_id, &alloc_offset, alloc_size, blk_id);
    if (ret != DRV_ERROR_NONE) {
        buff_warn("get not success, offset %lx\n", offset);
        return ret;
    }

    *alloc_addr = (char *)buff_base_addr + alloc_offset;

    return DRV_ERROR_NONE;
}

void buff_blk_put(int pool_id, void *addr)
{
    unsigned long offset;
    drvError_t ret;

    offset = (unsigned long)((uintptr_t)addr - (uintptr_t)buff_base_addr);

    ret = buff_pool_blk_put(pool_id, offset);
    if (ret != DRV_ERROR_NONE) {
        buff_err("pool_id %d put failed, offset %lx\n", pool_id, offset);
    }
}

drvError_t buff_set_prop(const char *prop_name, unsigned long value)
{
    if (!buff_pool_is_ready()) {
        buff_err("buff mng not init\n");
        return DRV_ERROR_PARA_ERROR;
    }

    return buff_pool_set_prop(buffpool_id, prop_name, value);
}

drvError_t buff_get_prop(const char *prop_name, unsigned long *value)
{
    if (!buff_pool_is_ready()) {
        buff_err("buff mng not init\n");
        return DRV_ERROR_PARA_ERROR;
    }

    return buff_pool_get_prop(buffpool_id, prop_name, value);
}

drvError_t buff_pool_init(int pool_id, int mem_fd, unsigned long long max_mem_size, GroupShareAttr attr)
{
    void *addr;
    unsigned int prot = 0;

    if (is_authed_read(attr)) {
        prot |= PROT_READ;
    }

    if (is_authed_write(attr)) {
        prot |= PROT_WRITE;
    }

    if (prot == 0) {
        buff_err("pool_id %u mem_fd %d attr is error\n", pool_id, mem_fd);
        return DRV_ERROR_PARA_ERROR;
    }

    /* parent proc has been attached */
    if (buff_base_addr != NULL) {
#ifdef EMU_ST
        buffpool_id = pool_id;
        return DRV_ERROR_NONE;
#else
        (void)munmap(buff_base_addr, max_mem_size);
#endif
    }

    addr = mmap((void *)BUFF_MEM_BASE, max_mem_size, (int)prot, MAP_SHARED, mem_fd, 0);
    if (addr == MAP_FAILED) {
        buff_err("pool_id %d mem_fd %d mmap failed errno %d\n", pool_id, mem_fd, errno);
        return DRV_ERROR_INNER_ERR;
    }

    if (addr != (void *)BUFF_MEM_BASE) {
        buff_err("pool_id %d mem_fd %d mmap addr %p error\n", pool_id, mem_fd, addr);
        (void)munmap(addr, max_mem_size);
        return DRV_ERROR_INNER_ERR;
    }
    (void)madvise(addr, max_mem_size, MADV_DONTDUMP);
    buff_base_addr = addr;
    buff_end_addr = (char *)buff_base_addr + max_mem_size;
    buffpool_id = pool_id;

    return DRV_ERROR_NONE;
}

