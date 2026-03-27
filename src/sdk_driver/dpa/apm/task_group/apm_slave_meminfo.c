/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "ka_common_pub.h"
#include "ka_base_pub.h"
#include "ka_errno_pub.h"
#include "ka_fs_pub.h"

#include <securec.h>

#include "apm_proc_mem_query_handle.h"
#include "ascend_hal_define.h"
#include "apm_kern_log.h"
#include "apm_slab.h"
#include "apm_kernel_ioctl.h"
#include "apm_slave_meminfo.h"

#define SHAREPOOL_INFO_PATH         "/proc/sharepool/proc_overview"
#define SHAREPOOL_INFO_MAX_LEN      4096
#define SHAREPOOL_INFO_MEMBER_NUM   3
#define BYTES_PER_KB 1024ul

#ifndef EMU_ST
static ssize_t apm_read_file(ka_file_t *fp, char *dst_addr, size_t fsize, loff_t *pos)
{
    return ka_fs_kernel_read(fp, (void *)dst_addr, fsize, pos);
}

/*
 *# cat /proc/sharepool/proc_overview
 * PID      COMM             SP_ALLOC  SP_K2U    SP_RES    Non-SP_RES Non-SP_Shm VIRT
 * 3536     process-manager  0         0         0         10624      7424       768220
 */
static int apm_get_used_mem_by_proc_view(const char *file_string, int slave_tgid, u64 *aicpu_size, u64 *sp_size)
{
    char *p_str = NULL, *ptr = NULL, *temp_ptr = NULL;
    u64 tmp_sp_size, tmp_aicpu_size;
    const char *mem_string = "PID";
    int pid;

    p_str = ka_base_strstr(file_string, mem_string);
    if (p_str == NULL) {
        apm_err("Can not find string. (mem_string=%s)\n", mem_string);
        return -EINVAL;
    }

    ptr = strtok_s(p_str, "\n", &temp_ptr);
    while (ptr != NULL) {
        ptr = strtok_s(NULL, "\n", &temp_ptr);
        if ((ptr != NULL) && (sscanf_s(ptr, "%d %*s %llu %*llu %*llu %llu",
            &pid, &tmp_sp_size, &tmp_aicpu_size) == SHAREPOOL_INFO_MEMBER_NUM)) {
            if (pid == slave_tgid) {
                *sp_size = tmp_sp_size * BYTES_PER_KB;
                *aicpu_size = tmp_aicpu_size * BYTES_PER_KB;
                apm_debug("slave_tgid=%d; sp_size=%llu; aicpu_size=%llu\n",
                    slave_tgid, tmp_sp_size, tmp_aicpu_size);
                return 0;
            }
        }
    }

    return 0;
}
#endif

static int apm_get_slave_malloc_and_sharepool_mem(int slave_tgid, u64 *aicpu_size, u64 *sp_size)
{
#ifndef EMU_ST
    ka_file_t *fp = NULL;
    char *file_name = SHAREPOOL_INFO_PATH;
    char *file_string = NULL;
    loff_t pos = 0;
    int ret;

    fp = ka_fs_filp_open(file_name, KA_O_RDONLY, 0);
    if (KA_IS_ERR((void const *)fp)) {
        apm_err("Open sharepool info file failed. (file=\"%s\", ret=%ld)\n",
            file_name, KA_PTR_ERR((void const *)fp));
        return -ENOENT;
    }

    file_string = (char *)apm_vzalloc(SHAREPOOL_INFO_MAX_LEN);
    if (file_string == NULL) {
        apm_err("Kvalloc fail.\n");
        ret = -ENOMEM;
        goto close_file;
    }

    if (apm_read_file(fp, file_string, SHAREPOOL_INFO_MAX_LEN - 1, &pos) < 0) {
        apm_err("Filestring not right. (file=\"%s\"; pos=%lld)\n", file_name, pos);
        ret = -EINVAL;
        goto free_mem;
    }

    ret = apm_get_used_mem_by_proc_view(file_string, slave_tgid, aicpu_size, sp_size);
    if (ret != 0) {
        apm_err("Can not find sharepool memsize. (file=\"%s\"; slave_tgid=%d; ret=%d)\n",
            file_name, slave_tgid, ret);
    }

free_mem:
    apm_vfree(file_string);
close_file:
    (void)ka_fs_filp_close(fp, NULL);
    return ret;
#else
    *aicpu_size = 1;
    return 0;
#endif
}

/* size = malloc + sharepool + kernel alloc_page */
static int apm_get_slave_used_mem(int slave_tgid, u64 *used_size)
{
    u64 aicpu_size = 0, sp_size = 0, proc_mem_query_size = 0;
    int ret;

    ret = apm_get_slave_malloc_and_sharepool_mem(slave_tgid, &aicpu_size, &sp_size);
    if(ret != 0) {
        return ret;
    }

#ifndef EMU_ST
    /* kernel alloc_page */
    ret = apm_proc_mem_query(slave_tgid, &proc_mem_query_size);
    if (ret != 0) {
        apm_err("Can not find svm alloc memsize. (ret=%d)\n", ret);
        return ret;
    }
#endif

    *used_size = aicpu_size + sp_size + proc_mem_query_size;
    return ret;
}

int apm_get_slave_meminfo(int slave_tgid, processMemType_t type, u64 *size)
{
    int ret = -EOPNOTSUPP;
    u64 tmp_size;

    switch (type) {
        case PROC_MEM_TYPE_ALL:
            ret = apm_get_slave_used_mem(slave_tgid, size);
            break;
        case PROC_MEM_TYPE_VMRSS:
            ret = apm_get_slave_malloc_and_sharepool_mem(slave_tgid, size, &tmp_size);
            break;
        case PROC_MEM_TYPE_SP:
            ret = apm_get_slave_malloc_and_sharepool_mem(slave_tgid, &tmp_size, size);
            break;
        default:
            break;
    }
    return ret;
}

