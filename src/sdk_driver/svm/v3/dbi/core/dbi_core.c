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
#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_memory_pub.h"
#include "ka_fs_pub.h"
#include "ka_compiler_pub.h"
#include "ka_kernel_def_pub.h"
#include "ka_pci_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_uda.h"

#include "framework_dev.h"
#include "framework_cmd.h"
#include "svm_kern_log.h"
#include "svm_ioctl_ex.h"
#include "svm_slab.h"
#include "kmc_h2d.h"
#include "dbi_msg.h"
#include "dbi_ioctl.h"
#include "dbi_kern.h"
#include "dbi_core.h"

static u32 dbi_feature_id;

#define DBI_OP_QUERY   0U
#define DBI_OP_SET     1U

#define SVM_HOST_HPAGE_SIZE (2 * SVM_BYTES_PER_MB)  /* host use 2M huge page size */
#define SVM_HOST_GPAGE_SIZE (1 * SVM_BYTES_PER_GB)  /* host use 1G giant page size */

static const ka_pci_device_id_t g_svm_pci_device_id[] = {
  { KA_PCI_VDEVICE(HUAWEI, 0xd806), 0 },
  { KA_PCI_VDEVICE(HUAWEI, 0xd807), 0 },
  {}
};
KA_MODULE_DEVICE_TABLE(pci, g_svm_pci_device_id);

static int dbi_op(u32 udevid, u32 op, struct svm_device_basic_info *dbi)
{
    void *dev_ctx = svm_dev_ctx_get(udevid);
    if (dev_ctx != NULL) {
        struct svm_device_basic_info *dbi_tmp = svm_dev_get_feature_priv(dev_ctx, dbi_feature_id);
        if (dbi_tmp != NULL) {
            if (op == DBI_OP_QUERY) {
                *dbi = *dbi_tmp;
            } else {
                *dbi_tmp = *dbi;
            }
            svm_dev_ctx_put(dev_ctx);
            return 0;
        }
        svm_dev_ctx_put(dev_ctx);
    }

    return -EINVAL;
}

static int dbi_set(u32 udevid, struct svm_device_basic_info *dbi)
{
    return dbi_op(udevid, DBI_OP_SET, dbi);
}

static int dbi_query(u32 udevid, struct svm_device_basic_info *dbi)
{
    return dbi_op(udevid, DBI_OP_QUERY, dbi);
}

static int dbi_msg_check(struct svm_device_basic_info *dbi)
{
    if ((dbi->npage_size == 0ULL) || (dbi->hpage_size == 0ULL) || (dbi->gpage_size == 0ULL)) {
        svm_err("Invalid page_size. (page_size=0x%llx; hpage_size=0x%llx; gpage_size=0x%llx)\n",
            dbi->npage_size, dbi->hpage_size, dbi->gpage_size);
        return -EINVAL;
    }

    return 0;
}

static int dbi_device_query(u32 udevid, struct svm_device_basic_info *dbi)
{
    struct svm_query_dbi_msg msg;
    u32 reply_len;
    int ret;

    msg.head.msg_id = SVM_KMC_MSG_QUERY_DBI;
    msg.head.extend_num = 0;

    ret = svm_kmc_h2d_send(udevid, &msg, (u32)sizeof(msg), (u32)sizeof(msg), &reply_len);
    if (ret != 0) {
        svm_err("Kmc h2d failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = dbi_msg_check(&msg.dbi);
    if (ret != 0) {
        return ret;
    }

    *dbi = msg.dbi;

    return 0;
}

static int dbi_host_query(struct svm_device_basic_info *dbi)
{
    dbi->npage_size = KA_MM_PAGE_SIZE;
    dbi->hpage_size = SVM_HOST_HPAGE_SIZE;
    dbi->gpage_size = SVM_HOST_GPAGE_SIZE;
    dbi->cap_flag = 0;
    dbi->bus_inst_eid_flag = 0;
    dbi->d2h_acc_mask = 0;

    return 0;
}

static int dbi_client_query(u32 udevid, struct svm_device_basic_info *dbi)
{
    if (udevid == uda_get_host_id()) {
        return dbi_host_query(dbi);
    } else {
        return dbi_device_query(udevid, dbi);
    }
}

int svm_dbi_kern_query_npage_size(u32 udevid, u64 *npage_size)
{
    struct svm_device_basic_info dbi;
    int ret = dbi_query(udevid, &dbi);
    if (ret == 0) {
        *npage_size = dbi.npage_size;
    }

    return ret;
}

int svm_dbi_kern_query_hpage_size(u32 udevid, u64 *hpage_size)
{
    struct svm_device_basic_info dbi;
    int ret = dbi_query(udevid, &dbi);
    if (ret == 0) {
        *hpage_size = dbi.hpage_size;
    }

    return ret;
}

int svm_dbi_kern_query_gpage_size(u32 udevid, u64 *gpage_size)
{
    struct svm_device_basic_info dbi;
    int ret = dbi_query(udevid, &dbi);
    if (ret == 0) {
        *gpage_size = dbi.gpage_size;
    }

    return ret;
}

bool svm_dbi_kern_is_support_sva(u32 udevid)
{
    struct svm_device_basic_info dbi;
    int ret = dbi_query(udevid, &dbi);
    if (ret == 0) {
        return ((dbi.cap_flag & SVM_DEV_CAP_SVA) != 0);
    }

    return false;
}

bool svm_dbi_kern_is_support_ubmem(u32 udevid)
{
    struct svm_device_basic_info dbi;
    int ret = dbi_query(udevid, &dbi);
    if (ret == 0) {
        return ((dbi.cap_flag & SVM_DEV_CAP_UBMEM) != 0);
    }

    return false;
}

int svm_dbi_kern_query_bus_inst_eid(u32 udevid, dbi_bus_inst_eid_t *eid)
{
    struct svm_device_basic_info dbi = {0};
    int ret;

    ret = dbi_query(udevid, &dbi);
    if (ret != 0) {
        svm_err("Device uninit. (ret=%d; udevid=%u)\n", ret, udevid);
        return ret;
    }

    if (dbi.bus_inst_eid_flag == 0) {
        ret = dbi_client_query(udevid, &dbi);
        if (ret == 0) {
            (void)dbi_set(udevid, &dbi); /* Unlikely fail. */
        }
    }
    if (dbi.bus_inst_eid_flag == 0) {
        svm_err("Bus inst eid not config. (udevid=%u)\n", udevid);
        return DRV_ERROR_UNINIT;
    }

    *eid = dbi.bus_inst_eid;
    return DRV_ERROR_NONE;
}

static int svm_ioctl_dbi_query(u32 udevid, u32 cmd, unsigned long arg)
{
    struct svm_dbi_query_para para;
    int ret;

    ret = dbi_query(udevid, &para.dbi);
    if (ret != 0) {
        svm_err("Query fail. (udevid=%u)\n", udevid);
        return ret;
    }

    if (ka_base_copy_to_user((void __ka_user *)(uintptr_t)arg, &para, sizeof(para)) != 0) {
        svm_err("Do copy_to_user fail.\n");
        return -EFAULT;
    }

    return 0;
}

static int svm_enable_host_ubmem(u32 udevid)
{
    struct svm_device_basic_info dbi;
    int ret = dbi_query(udevid, &dbi);
    if (ret != 0) {
        return ret;
    }

    dbi.cap_flag |= SVM_DEV_CAP_UBMEM;
    return dbi_set(udevid, &dbi);
}

int svm_enable_ubmem(u32 udevid)
{
    if (udevid == uda_get_host_id()) {
        return svm_enable_host_ubmem(udevid);
    }

    return 0;
}

static int svm_update_dbi(u32 udevid, void *msg, u32 *reply_len)
{
    struct svm_update_dbi_msg *dbi_msg = (struct svm_update_dbi_msg *)msg;
    int ret;

    *reply_len = sizeof(struct svm_kmc_msg_head);

    ret = dbi_msg_check(&dbi_msg->dbi);
    if (ret != 0) {
        return ret;
    }

    return dbi_set(udevid, &dbi_msg->dbi);
}

static struct svm_kmc_d2h_recv_handle g_d2h_update_dbi = {
    .func = svm_update_dbi,
    .raw_msg_size = sizeof(struct svm_update_dbi_msg),
    .extend_gran_size = 0
};

void dbi_show_dev(u32 udevid, int feature_id, ka_seq_file_t *seq)
{
    void *dev_ctx = NULL;

    if (feature_id != (int)dbi_feature_id) {
        return;
    }

    dev_ctx = svm_dev_ctx_get(udevid);
    if (dev_ctx != NULL) {
        struct svm_device_basic_info *dbi = svm_dev_get_feature_priv(dev_ctx, dbi_feature_id);
        if (dbi != NULL) {
            ka_fs_seq_printf(seq, "    npage_size=0x%llx\n", dbi->npage_size);
            ka_fs_seq_printf(seq, "    hpage_size=0x%llx\n", dbi->hpage_size);
            ka_fs_seq_printf(seq, "    gpage_size=0x%llx\n", dbi->gpage_size);
            ka_fs_seq_printf(seq, "    support_sva=%d\n", ((dbi->cap_flag & SVM_DEV_CAP_SVA) != 0));
            ka_fs_seq_printf(seq, "    support_ubmem=%d\n", ((dbi->cap_flag & SVM_DEV_CAP_UBMEM) != 0));
            ka_fs_seq_printf(seq, "    support_assign_gap=%d\n", ((dbi->cap_flag & SVM_DEV_CAP_ASSIGN_GAP) != 0));
            ka_fs_seq_printf(seq, "    d2h_acc_mask=0x%llx\n", dbi->d2h_acc_mask);
            ka_fs_seq_printf(seq, "    bus_inst_eid_flag=%u\n", dbi->bus_inst_eid_flag);
            if (dbi->bus_inst_eid_flag == 1) {
                ka_fs_seq_printf(seq, "    bus_inst_eid="DBI_EID_FMT"\n", DBI_EID_ARGS(dbi->bus_inst_eid));
            }
        }
        svm_dev_ctx_put(dev_ctx);
    }
}
DECLAER_FEATURE_AUTO_SHOW_DEV(dbi_show_dev, FEATURE_LOADER_STAGE_1);

int dbi_init_dev(u32 udevid)
{
    struct svm_device_basic_info *dbi = NULL;
    void *dev_ctx = NULL;
    int ret;

    dbi = (struct svm_device_basic_info *)svm_vzalloc(sizeof(*dbi));
    if (dbi == NULL) {
        svm_err("Alloc failed failed.\n");
        return -ENOMEM;
    }

    ret = dbi_client_query(udevid, dbi);
    if (ret != 0) {
        svm_vfree(dbi);
        return ret;
    }

    dev_ctx = svm_dev_ctx_get(udevid);
    if (dev_ctx == NULL) {
        svm_vfree(dbi);
        return -EINVAL;
    }

    ret = svm_dev_set_feature_priv(dev_ctx, dbi_feature_id, "dbi", dbi);
    if (ret != 0) {
        svm_dev_ctx_put(dev_ctx);
        svm_vfree(dbi);
        svm_err("Set dev feature priv failed. (udevid=%u; ret=%d; feature_id=%u)\n", udevid, ret, dbi_feature_id);
        return ret;
    }

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_DEV(dbi_init_dev, FEATURE_LOADER_STAGE_1);

void dbi_uninit_dev(u32 udevid)
{
    void *dev_ctx = NULL;

    dev_ctx = svm_dev_ctx_get(udevid);
    if (dev_ctx != NULL) {
        struct svm_device_basic_info *dbi = svm_dev_get_feature_priv(dev_ctx, dbi_feature_id);
        (void)svm_dev_set_feature_priv(dev_ctx, dbi_feature_id, NULL, NULL);
        svm_dev_ctx_put(dev_ctx);
        svm_dev_ctx_put(dev_ctx); /* paired with init */
        if (dbi != NULL) {
            svm_vfree(dbi);
        }
    }
}
DECLAER_FEATURE_AUTO_UNINIT_DEV(dbi_uninit_dev, FEATURE_LOADER_STAGE_1);

int dbi_feature_init(void)
{
    svm_kmc_d2h_recv_handle_register(SVM_KMC_MSG_UPDATE_DBI, &g_d2h_update_dbi);

    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_DBI_QUERY), svm_ioctl_dbi_query);
    dbi_feature_id = svm_dev_obtain_feature_id();
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(dbi_feature_init, FEATURE_LOADER_STAGE_1);

