/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include "uda_notifier.h"
#include "pbl_mem_alloc_interface.h"
#include "uda_dev.h"

static struct mutex uda_dev_mutex;
static rwlock_t uda_dev_lock;
static struct uda_dev_inst *dev_insts[UDA_UDEV_MAX_NUM];
struct uda_reorder_insts reorder_insts;
struct udevid_reorder_para *reorder_para = NULL;
static bool uda_udevid_reorder_flag = false;

void uda_type_to_string(struct uda_dev_type *type, char buf[], u32 buf_len)
{
    int ret = 0;
    static const char *uda_hw_name[UDA_HW_MAX] = {
        [UDA_DAVINCI] = "davinci",
        [UDA_KUNPENG] = "kunpeng",
    };

    static const char *uda_obj_name[UDA_OBJECT_MAX] = {
        [UDA_ENTITY] = "entity",
        [UDA_AGENT] = "agent",
    };

    static const char *uda_loc_name[UDA_LOCATION_MAX] = {
        [UDA_LOCAL] = "local",
        [UDA_NEAR] = "near",
        [UDA_REMOTE] = "remote",
    };

    static const char *uda_prop_name[UDA_PROP_MAX] = {
        [UDA_REAL] = "real",
        [UDA_VIRTUAL] = "virtual",
        [UDA_REAL_SEC_EH] = "real_sec_eh",
    };

    ret = sprintf_s(buf, buf_len, "%s %s %s %s",
        uda_hw_name[type->hw], uda_obj_name[type->object], uda_loc_name[type->location], uda_prop_name[type->prop]);
    if (ret <= 0) {
        uda_err("Invoke sprintf_s error.\n");
    }
}

u32 uda_get_udev_max_num(void)
{
    return UDA_UDEV_MAX_NUM;
}
EXPORT_SYMBOL(uda_get_udev_max_num);

u32 uda_get_remote_udev_max_num(void)
{
    return UDA_REMOTE_UDEV_MAX_NUM;
}
EXPORT_SYMBOL(uda_get_remote_udev_max_num);

bool hal_kernel_uda_is_phy_dev(u32 udevid)
{
    return uda_is_phy_dev(udevid);
}
EXPORT_SYMBOL(hal_kernel_uda_is_phy_dev);

bool uda_is_phy_dev(u32 udevid)
{
    return (udevid < UDA_MAX_PHY_DEV_NUM);
}
EXPORT_SYMBOL(uda_is_phy_dev);

void uda_set_udevid_reorder_flag(bool flag)
{
    uda_udevid_reorder_flag = flag;
}
EXPORT_SYMBOL(uda_set_udevid_reorder_flag);

bool uda_is_pf_dev(u32 udevid)
{
    struct uda_dev_inst *dev_inst = NULL;
    bool is_pf = true;

    if (udevid >= UDA_MAX_PHY_DEV_NUM) {
        return false;
    }

    dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return false;
    }

    is_pf = dev_inst->para.pf_flag == 1 ? true : false;
    uda_dev_inst_put(dev_inst);
    return is_pf;
}
EXPORT_SYMBOL(uda_is_pf_dev);

static int uda_alloc_udevid(u32 start, u32 end, u32 *udevid) // [start, end)
{
    u32 i;

    for (i = start; i < end; i++) {
        if (dev_insts[i] == NULL) {
            *udevid = i;
            return 0;
        }
    }

    return -ENOSPC;
}

static u32 uda_make_udevid(struct uda_mia_dev_para *mia_para)
{
    return mia_para->phy_devid * UDA_SUB_DEV_MAX_NUM + mia_para->sub_devid + UDA_MIA_UDEV_OFFSET;
}

static struct uda_dev_inst *uda_create_dev_inst(struct uda_dev_type *type, struct uda_dev_para *para)
{
    static u32 inst_id = 0;
    struct uda_dev_inst *dev_inst = dbl_kzalloc(sizeof(struct uda_dev_inst), GFP_KERNEL | __GFP_ACCOUNT);
    if (dev_inst == NULL) {
        uda_err("Out of memory.\n");
        return NULL;
    }

    dev_inst->type = *type;
    dev_inst->para = *para;
    dev_inst->agent_flag = 0;
    dev_inst->inst_id = inst_id++;
    kref_init(&dev_inst->ref);

    return dev_inst;
}

static void uda_destroy_dev_inst(struct uda_dev_inst *dev_inst)
{
    dbl_kfree(dev_inst);
}

struct uda_dev_inst *uda_dev_inst_get(u32 udevid)
{
    struct uda_dev_inst *dev_inst = NULL;

    if (udevid >= UDA_UDEV_MAX_NUM) {
        return NULL;
    }

    read_lock_bh(&uda_dev_lock);
    dev_inst = dev_insts[udevid];
    if (dev_inst != NULL) {
        kref_get(&dev_inst->ref);
    }
    read_unlock_bh(&uda_dev_lock);

    return dev_inst;
}

struct uda_dev_inst *uda_dev_inst_get_ex(u32 udevid)
{
    struct uda_dev_inst *dev_inst = NULL;

    if (udevid >= UDA_UDEV_MAX_NUM) {
        return NULL;
    }

    read_lock_irq(&uda_dev_lock);
    dev_inst = dev_insts[udevid];
    if (dev_inst != NULL) {
        kref_get(&dev_inst->ref);
    }
    read_unlock_irq(&uda_dev_lock);

    return dev_inst;
}

static void uda_dev_inst_release(struct kref *kref)
{
    struct uda_dev_inst *dev_inst = container_of(kref, struct uda_dev_inst, ref);

    uda_info("Remove dev success. (udevid=%u; agent_flag=%u)\n", dev_inst->udevid, dev_inst->agent_flag);
    uda_destroy_dev_inst(dev_inst);
}

void uda_dev_inst_put(struct uda_dev_inst *dev_inst)
{
    kref_put(&dev_inst->ref, uda_dev_inst_release);
}

static int _uda_add_dev_ex(struct uda_dev_type *type, struct uda_dev_para *para,
    struct uda_mia_dev_para *mia_para, u32 *udevid)
{
    struct uda_dev_inst *dev_inst = NULL;
    int ret;

    if (type->object == UDA_ENTITY) {
        if ((para->udevid != UDA_INVALID_UDEVID) || (mia_para != NULL)) {
            *udevid = (mia_para == NULL) ? para->udevid : uda_make_udevid(mia_para);
            if (dev_insts[*udevid] != NULL) {
                uda_warn("Has add entity dev. (udevid=%u)\n", *udevid);
                dev_insts[*udevid]->para.dev = para->dev;
                dev_insts[*udevid]->status = 0;
                return 0;
            }
        } else {
            ret = uda_alloc_udevid(0, UDA_MIA_UDEV_OFFSET, udevid);
            if (ret != 0) {
                uda_err("No udevid res\n");
                return ret;
            }
        }
    } else { /* UDA_AGENT */
        *udevid = para->udevid;
        if (dev_insts[para->udevid] == NULL) {
            uda_err("No entity dev. (udevid=%u)\n", para->udevid);
            return -EINVAL;
        }

        if (dev_insts[para->udevid]->agent_dev != NULL) {
            uda_warn("Has add agent dev. (udevid=%u)\n", para->udevid);
            ((struct uda_dev_inst *)dev_insts[para->udevid]->agent_dev)->para.dev = para->dev;
            ((struct uda_dev_inst *)dev_insts[para->udevid]->agent_dev)->status = 0;
            return 0;
        }
    }

    dev_inst = uda_create_dev_inst(type, para);
    if (dev_inst == NULL) {
        return -ENOMEM;
    }

    dev_inst->udevid = *udevid;
    if (mia_para != NULL) {
        dev_inst->mia_para = *mia_para;
    }

    if (type->object == UDA_ENTITY) {
#ifdef CFG_FEATURE_DEVID_TRANS
        ret = uda_access_add_dev(dev_inst->udevid, &dev_inst->access);
        if (ret != 0) {
            uda_destroy_dev_inst(dev_inst);
            return ret;
        }
#endif
        dev_insts[*udevid] = dev_inst;
    } else {
        dev_inst->agent_flag = 1;
        dev_insts[para->udevid]->agent_dev = dev_inst;
        if (dev_inst->para.remote_udevid != UDA_INVALID_UDEVID) {
            dev_insts[para->udevid]->para.remote_udevid = dev_inst->para.remote_udevid;
        }
    }

    uda_info("Add dev success. (udevid=%u; agent_flag=%u)\n", *udevid, dev_inst->agent_flag);
    uda_show_type("Add", type);
    return 0;
}

int uda_guid_compare(u32 *guid, u32 *other_guid)
{
    int i;

    for (i = 0; i < UDA_GUID_LEN; i++) {
        if (guid[UDA_GUID_LEN - i - 1] != other_guid[UDA_GUID_LEN - i - 1]) {
            if (guid[UDA_GUID_LEN - i - 1] > other_guid[UDA_GUID_LEN - i - 1]) {
                return 1;
            } else {
                return -1;
            }
        }
    }
    return 0;
}

void uda_guid_sort(unsigned int start, unsigned int end)
{
    struct uda_dev_para tmp_para;
    unsigned int *guid1 = NULL;
    unsigned int *guid2 = NULL;
    unsigned int i, j;

    for (i = start; i < end; i++) {
        guid1 = reorder_para[reorder_insts.dev_para[i].add_id].guid;
        for (j = i + 1; j < end; j++) {
            guid2 = reorder_para[reorder_insts.dev_para[j].add_id].guid;
            if (uda_guid_compare(guid1, guid2) > 0) {
                tmp_para = reorder_insts.dev_para[i];
                reorder_insts.dev_para[i] = reorder_insts.dev_para[j];
                reorder_insts.dev_para[j] = tmp_para;
            }
        }
    }
}

static void _uda_udevid_reorder(void)
{
    struct uda_dev_para tmp_para;
    unsigned int ub_link_count = 0;
    unsigned int group_dev_num = 1; /* 1p */
    unsigned int *guid = NULL;
    unsigned int tmp_index = 0;
    unsigned int udevid = 0;
    unsigned int i, j;

    for (i = 0; i < reorder_insts.dev_add_num; i += group_dev_num) {
        tmp_index = i + 1;
        if ((reorder_para[reorder_insts.dev_para[i].add_id].group_dev_num <= UDA_REORDER_GROUP_DEV_MAX_NUM) &&
            (reorder_para[reorder_insts.dev_para[i].add_id].group_dev_num != 0)) {
            group_dev_num = reorder_para[reorder_insts.dev_para[i].add_id].group_dev_num;
        } else {
            group_dev_num = 1; /* 1p */
        }
        /* 1:Establish links on all ports */
        if (reorder_para[reorder_insts.dev_para[i].add_id].ub_link_status == 1) {
            ub_link_count++;
        }
        /* Devices within a group are placed together. */
        for (j = tmp_index; j < reorder_insts.dev_add_num; j++) {
            guid = reorder_para[reorder_insts.dev_para[j].add_id].guid;
            if ((uda_guid_compare(guid, reorder_para[reorder_insts.dev_para[i].add_id].guid1) == 0) ||
                (uda_guid_compare(guid, reorder_para[reorder_insts.dev_para[i].add_id].guid2) == 0) ||
                (uda_guid_compare(guid, reorder_para[reorder_insts.dev_para[i].add_id].guid3) == 0)) {
                tmp_para = reorder_insts.dev_para[tmp_index];
                reorder_insts.dev_para[tmp_index] = reorder_insts.dev_para[j];
                reorder_insts.dev_para[j] = tmp_para;
                tmp_index++;
                /* 1:Establish links on all ports */
                if (reorder_para[reorder_insts.dev_para[j].add_id].ub_link_status == 1) {
                    ub_link_count++;
                }
            }
            if ((tmp_index - i) == group_dev_num) {
                break;
            }
        }
        uda_guid_sort(i, i + group_dev_num);
        if (ub_link_count == group_dev_num) {
            for (j = i; j < group_dev_num; j++) {
                reorder_insts.dev_para[j].udevid = udevid;
                udevid++;
            }
        }
    }

    for (i = 0; i < reorder_insts.dev_add_num; i++) {
        if (reorder_insts.dev_para[i].udevid == UDA_INVALID_UDEVID) {
            reorder_insts.dev_para[i].udevid = udevid;
            udevid++;
        }
    }
}

#ifdef EMU_ST
#define UDA_REORDER_WAIT_CNT          1
#else
#define UDA_REORDER_WAIT_CNT          200
#endif
#define UDA_REORDER_WAIT_TIME         1000
#define UDA_UB_PORT_ALL_LINK 1
#define UDA_UB_PORT_NOT_LINK 3
static void uda_udevid_reorder_work(struct work_struct *work)
{
    unsigned int link_all_count = 0;
    unsigned int udevid = 0;
    unsigned int i;
    int wait_cnt = 0;
    int ret;

    while (wait_cnt < UDA_REORDER_WAIT_CNT) {
        link_all_count = 0;
        /* 1:links on all ports; 3:No need to establish a link on the port. */
        for (i = 0; i < reorder_insts.dev_add_num; i++) {
            if ((reorder_para[reorder_insts.dev_para[i].add_id].ub_link_status == UDA_UB_PORT_ALL_LINK) ||
                (reorder_para[reorder_insts.dev_para[i].add_id].ub_link_status == UDA_UB_PORT_NOT_LINK)) {
                link_all_count++;
            }
        }
        if ((link_all_count == uda_get_detected_phy_dev_num()) || (uda_get_detected_phy_dev_num() == 1)) {
            break;
        }
        msleep(UDA_REORDER_WAIT_TIME);
        wait_cnt++;
    }
    uda_info("Reorder wait end. (dev_add_num=%u; detected_phy_dev_num=%u)\n",
        reorder_insts.dev_add_num, uda_get_detected_phy_dev_num());

    _uda_udevid_reorder();
    for (i = 0; i < reorder_insts.dev_add_num; i++) {
        mutex_lock(&uda_dev_mutex);
        ret = _uda_add_dev_ex(&reorder_insts.dev_type, &reorder_insts.dev_para[i], NULL, &udevid);
        mutex_unlock(&uda_dev_mutex);
        if (ret == 0) {
            (void)uda_notifier_call(udevid, &reorder_insts.dev_type, UDA_INIT);
        }
    }
    reorder_insts.dev_add_num = 0;
    uda_info("Reorder task end.\n");
}

static int uda_udevid_reorder(struct uda_dev_type *type, struct uda_dev_para *para)
{
    unsigned int all_dev_num = uda_get_detected_phy_dev_num();

    if (reorder_insts.dev_para == NULL) {
        reorder_insts.dev_para = dbl_kzalloc(sizeof(struct uda_dev_para) * all_dev_num, GFP_KERNEL | __GFP_ACCOUNT);
        if (reorder_insts.dev_para == NULL) {
            uda_err("Out of memory.\n");
            return -ENOMEM;
        }

        reorder_insts.dev_type = *type;
        reorder_insts.dev_add_num = 0;
    }
    if (reorder_insts.dev_add_num >= all_dev_num) {
        uda_err("Reorder insts is full. (dev_add_num=%u; all_dev_num=%u)\n", reorder_insts.dev_add_num, all_dev_num);
        return -ENOMEM;
    }
    reorder_insts.dev_para[reorder_insts.dev_add_num] = *para;
    reorder_insts.dev_para[reorder_insts.dev_add_num].add_id = para->udevid;
    reorder_insts.dev_para[reorder_insts.dev_add_num].udevid = UDA_INVALID_UDEVID;
    reorder_insts.dev_add_num++;
    /* 1:Add the first device to start work */
    if (reorder_insts.dev_add_num == 1) {
        schedule_work(&reorder_insts.udevid_reorder_work);
    }
    uda_info("Reorder add success. (dev_add_num=%u)\n", reorder_insts.dev_add_num);

    return 0;
}

static int uda_add_dev_ex(struct uda_dev_type *type, struct uda_dev_para *para,
    struct uda_mia_dev_para *mia_para, u32 *udevid)
{
    int ret;

    if ((type == NULL) || (para == NULL) || (udevid == NULL)) {
        uda_err("Null ptr.\n");
        return -EINVAL;
    }

    ret = uda_dev_type_valid_check(type);
    if (ret != 0) {
        uda_err("Invalid type.\n");
        return ret;
    }

    if (type->object == UDA_ENTITY) {
        if (((para->udevid != UDA_INVALID_UDEVID) && !uda_is_phy_dev(para->udevid))) {
            uda_err("Invalid para. (udevid=%u)\n", para->udevid);
            return -EINVAL;
        }
    } else {
        if (para->udevid >= UDA_UDEV_MAX_NUM) {
            uda_err("Invalid para. (udevid=%u)\n", para->udevid);
            return -EINVAL;
        }
    }

    if (uda_udevid_reorder_flag && para->pf_flag == 1) {
        return uda_udevid_reorder(type, para);
    } else if (para->pf_flag == 0 && mia_para != NULL) {
        para->add_id = uda_make_udevid(mia_para);
    } else {
        para->add_id = para->udevid;
    }

    mutex_lock(&uda_dev_mutex);
    ret = _uda_add_dev_ex(type, para, mia_para, udevid);
    mutex_unlock(&uda_dev_mutex);

    if (ret == 0) {
        (void)uda_notifier_call(*udevid, type, UDA_INIT);
    }

    return ret;
}

int uda_add_dev(struct uda_dev_type *type, struct uda_dev_para *para, u32 *udevid)
{
    return uda_add_dev_ex(type, para, NULL, udevid);
}
EXPORT_SYMBOL(uda_add_dev);

static int uda_mia_dev_check(struct uda_mia_dev_para *mia_para)
{
    if (!uda_is_phy_dev(mia_para->phy_devid) || (mia_para->sub_devid >= UDA_SUB_DEV_MAX_NUM)
        || (uda_make_udevid(mia_para) >= UDA_UDEV_MAX_NUM)) {
        uda_err("Invalid para. (phy_devid=%u; sub_devid=%u)\n", mia_para->phy_devid, mia_para->sub_devid);
        return -EINVAL;
    }

    return 0;
}

int uda_add_mia_dev(struct uda_dev_type *type, struct uda_dev_para *para,
    struct uda_mia_dev_para *mia_para, u32 *udevid)
{
    if ((para == NULL) || (mia_para == NULL)) {
        uda_err("Null ptr.\n");
        return -EINVAL;
    }

    if (para->udevid != UDA_INVALID_UDEVID) {
        uda_err("Mia udevid cannot be specified. (udevid=%u)\n", para->udevid);
        return -EINVAL;
    }

    if (uda_mia_dev_check(mia_para) != 0) {
        return -EINVAL;
    }

    para->pf_flag = 0;
    return uda_add_dev_ex(type, para, mia_para, udevid);
}
EXPORT_SYMBOL(uda_add_mia_dev);

static int _uda_remove_dev(struct uda_dev_inst *dev_inst, struct uda_dev_type *type)
{
    if (uda_dev_type_is_match(type, &dev_inst->type)) {
#ifdef CFG_FEATURE_DEVID_TRANS
        int ret = uda_access_remove_dev(dev_inst->udevid, &dev_inst->access);
        if (ret != 0) {
            return ret;
        }
#endif
        write_lock_bh(&uda_dev_lock);
        dev_insts[dev_inst->udevid] = NULL;  /* set inst invalid, so other thread will not get it */
        write_unlock_bh(&uda_dev_lock);
        if (dev_inst->agent_dev != NULL) {
            struct uda_dev_inst *agent_dev_inst = dev_inst->agent_dev;
            uda_warn("Not remove agent dev. (udevid=%d)\n", dev_inst->udevid);
            kref_put(&agent_dev_inst->ref, uda_dev_inst_release);
        }
        kref_put(&dev_inst->ref, uda_dev_inst_release);
    } else {
        struct uda_dev_inst *agent_dev_inst = dev_inst->agent_dev;
        dev_inst->agent_dev = NULL;
        kref_put(&agent_dev_inst->ref, uda_dev_inst_release);
    }

    return 0;
}

int uda_remove_dev(struct uda_dev_type *type, u32 udevid)
{
    struct uda_dev_inst *dev_inst = NULL;
    int ret;

    if ((type == NULL) || (udevid >= UDA_UDEV_MAX_NUM)) {
        uda_err("Invalid para. (udevid=%u)\n", udevid);
        return  -EINVAL;
    }

    ret = uda_dev_type_valid_check(type);
    if (ret != 0) {
        uda_err("Invalid type.\n");
        return ret;
    }

    dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    if (!uda_dev_type_is_match(type, &dev_inst->type)) {
        struct uda_dev_inst *agent_dev_inst = dev_inst->agent_dev;
        if ((agent_dev_inst == NULL) || (!uda_dev_type_is_match(type, &agent_dev_inst->type))) {
            uda_err("Remove type mismatch. (udevid=%d)\n", udevid);
            uda_show_type("Remove type", type);
            uda_show_type("Dev type", &dev_inst->type);
            uda_dev_inst_put(dev_inst);
            return -ENODEV;
        }
    }

    (void)uda_notifier_call(udevid, type, UDA_UNINIT);

#ifndef EMU_ST
    if (!uda_is_phy_dev(udevid)) {
#else
    {
#endif
        mutex_lock(&uda_dev_mutex);
        ret = _uda_remove_dev(dev_inst, type);
        mutex_unlock(&uda_dev_mutex);
    }
    dev_inst->para.dev = NULL;
    uda_update_status_by_action(&dev_inst->status, UDA_REMOVE);

    if (ret != 0) {
        (void)uda_notifier_call(udevid, type, UDA_INIT);
    }

    uda_dev_inst_put(dev_inst);

    return ret;
}
EXPORT_SYMBOL(uda_remove_dev);

int uda_get_action_para(u32 udevid, enum uda_notified_action action, u32 *val)
{
    struct uda_dev_inst *dev_inst = NULL;

    if ((action < 0) || (action >= UDA_ACTION_MAX)) {
        uda_err("Invalid action. (udevid=%u; action=%d)\n", udevid, action);
        return -EINVAL;
    }

    if (val == NULL) {
        uda_err("Null ptr. (udevid=%u; action=%d)\n", udevid, action);
        return -EINVAL;
    }

    dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        return -EINVAL;
    }

    *val = dev_inst->action_para[action];
    uda_dev_inst_put(dev_inst);

    return 0;
}
EXPORT_SYMBOL(uda_get_action_para);

static void uda_dev_ctrl_restore(struct uda_dev_inst *dev_inst, enum uda_dev_ctrl_cmd cmd, u32 val)
{
    static enum uda_notified_action ctrl_cmd_to_restore_action[UDA_CTRL_MAX] = {
        [UDA_CTRL_SUSPEND] = UDA_RESUME,
        [UDA_CTRL_RESUME] = UDA_SUSPEND,
        [UDA_CTRL_TO_MIA] = UDA_TO_SIA,
        [UDA_CTRL_TO_SIA] = UDA_TO_MIA,
        [UDA_CTRL_MIA_CHANGE] = UDA_ACTION_MAX,
        [UDA_CTRL_REMOVE] = UDA_ACTION_MAX,
        [UDA_CTRL_HOTRESET] = UDA_HOTRESET_CANCEL,
        [UDA_CTRL_HOTRESET_CANCEL] = UDA_ACTION_MAX,
        [UDA_CTRL_SHUTDOWN] = UDA_ACTION_MAX,
        [UDA_CTRL_PRE_HOTRESET] = UDA_PRE_HOTRESET_CANCEL,
        [UDA_CTRL_PRE_HOTRESET_CANCEL] = UDA_ACTION_MAX,
        [UDA_CTRL_ADD_GROUP] = UDA_DEL_GROUP,
        [UDA_CTRL_DEL_GROUP] = UDA_ACTION_MAX,
        [UDA_CTRL_UPDATE_P2P_ADDR] = UDA_ACTION_MAX,
        [UDA_CTRL_COMMUNICATION_LOST] = UDA_ACTION_MAX,
        [UDA_CTRL_AIC_ATU_CFG] = UDA_ACTION_MAX,
    };

    if (ctrl_cmd_to_restore_action[cmd] != UDA_ACTION_MAX) {
        dev_inst->action_para[ctrl_cmd_to_restore_action[cmd]] = val;
        (void)uda_notifier_call(dev_inst->udevid, &dev_inst->type, ctrl_cmd_to_restore_action[cmd]);
    }
}

static int _uda_dev_ctrl(struct uda_dev_inst *dev_inst, enum uda_dev_ctrl_cmd cmd, u32 val)
{
    int ret;
    static enum uda_notified_action ctrl_cmd_to_action[UDA_CTRL_MAX] = {
        [UDA_CTRL_SUSPEND] = UDA_SUSPEND,
        [UDA_CTRL_RESUME] = UDA_RESUME,
        [UDA_CTRL_TO_MIA] = UDA_TO_MIA,
        [UDA_CTRL_TO_SIA] = UDA_TO_SIA,
        [UDA_CTRL_MIA_CHANGE] = UDA_MIA_CHANGE,
        [UDA_CTRL_REMOVE] = UDA_REMOVE,
        [UDA_CTRL_HOTRESET] = UDA_HOTRESET,
        [UDA_CTRL_HOTRESET_CANCEL] = UDA_HOTRESET_CANCEL,
        [UDA_CTRL_SHUTDOWN] = UDA_SHUTDOWN,
        [UDA_CTRL_PRE_HOTRESET] = UDA_PRE_HOTRESET,
        [UDA_CTRL_PRE_HOTRESET_CANCEL] = UDA_PRE_HOTRESET_CANCEL,
        [UDA_CTRL_ADD_GROUP] = UDA_ADD_GROUP,
        [UDA_CTRL_DEL_GROUP] = UDA_DEL_GROUP,
        [UDA_CTRL_UPDATE_P2P_ADDR] = UDA_UPDATE_P2P_ADDR,
        [UDA_CTRL_COMMUNICATION_LOST] = UDA_COMMUNICATION_LOST,
        [UDA_CTRL_AIC_ATU_CFG] = UDA_AIC_ATU_CFG,
    };

    if (uda_is_action_conflict(dev_inst->status, ctrl_cmd_to_action[cmd])) {
        uda_err("Invalid cmd. (udevid=%u; cmd=%d; status=%x)\n", dev_inst->udevid, cmd, dev_inst->status);
        return -EINVAL;
    }

    dev_inst->action_para[ctrl_cmd_to_action[cmd]] = val;
    ret = uda_notifier_call(dev_inst->udevid, &dev_inst->type, ctrl_cmd_to_action[cmd]);
    if (ret != 0) {
        uda_dev_ctrl_restore(dev_inst, cmd, val);
        uda_debug("Notifier call failed. (udevid=%d; cmd=%d; ret=%d)\n", dev_inst->udevid, cmd, ret);
        return ret;
    }

    uda_update_status_by_action(&dev_inst->status, ctrl_cmd_to_action[cmd]);

    uda_info("Dev ctrl success. (udevid=%d; cmd=%d)\n", dev_inst->udevid, cmd);
    return 0;
}

int uda_dev_ctrl_ex(u32 udevid, enum uda_dev_ctrl_cmd cmd, u32 val)
{
    struct uda_dev_inst *dev_inst = NULL;
    int ret;

    if ((cmd < 0) || (cmd >= UDA_CTRL_MAX)) {
        uda_err("Invalid cmd. (udevid=%u; cmd=%d)\n", udevid, cmd);
        return -EINVAL;
    }

    if (uda_is_phy_dev(udevid)) {
        if ((cmd == UDA_CTRL_MIA_CHANGE) || (cmd == UDA_CTRL_REMOVE)) {
            uda_err("Phy dev not support cmd. (udevid=%u; cmd=%d)\n", udevid, cmd);
            return -EINVAL;
        }
    } else {
        if ((cmd == UDA_CTRL_SUSPEND) || (cmd == UDA_CTRL_RESUME)
            || (cmd == UDA_CTRL_TO_MIA) || (cmd == UDA_CTRL_TO_SIA)
            || (cmd == UDA_CTRL_SHUTDOWN)) {
            uda_err("Mia dev not support cmd. (udevid=%u; cmd=%d)\n", udevid, cmd);
            return -EINVAL;
        }
    }

    dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        return -EINVAL;
    }

    ret = _uda_dev_ctrl(dev_inst, cmd, val);
    uda_dev_inst_put(dev_inst);

    return ret;
}
EXPORT_SYMBOL(uda_dev_ctrl_ex);

int uda_dev_ctrl(u32 udevid, enum uda_dev_ctrl_cmd cmd)
{
    return uda_dev_ctrl_ex(udevid, cmd, 0);
}
EXPORT_SYMBOL(uda_dev_ctrl);

int uda_agent_dev_ctrl(u32 udevid, enum uda_dev_ctrl_cmd cmd)
{
    struct uda_dev_inst *dev_inst = NULL;
    int ret;

    if ((cmd < 0) || (cmd >= UDA_CTRL_MAX)) {
        uda_err("Invalid cmd. (udevid=%u; cmd=%d)\n", udevid, cmd);
        return -EINVAL;
    }

    if (uda_is_phy_dev(udevid)) {
        if ((cmd == UDA_CTRL_MIA_CHANGE) || (cmd == UDA_CTRL_REMOVE)) {
            uda_err("Phy dev not support cmd. (udevid=%u; cmd=%d)\n", udevid, cmd);
            return -EINVAL;
        }
    } else {
        if ((cmd == UDA_CTRL_SUSPEND) || (cmd == UDA_CTRL_RESUME)
            || (cmd == UDA_CTRL_TO_MIA) || (cmd == UDA_CTRL_TO_SIA)) {
            uda_err("Mia dev not support cmd. (udevid=%u; cmd=%d)\n", udevid, cmd);
            return -EINVAL;
        }
    }

    dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("uda_dev_inst_get fail. (udevid=%u; cmd=%d)\n", udevid, cmd);
        return -EINVAL;
    }

    if (dev_inst->agent_dev == NULL) {
        uda_err("agent dev does not exist. (udevid=%u; cmd=%d)\n", udevid, cmd);
        uda_dev_inst_put(dev_inst);
        return -EINVAL;
    }

    ret = _uda_dev_ctrl((struct uda_dev_inst *)dev_inst->agent_dev, cmd, 0);
    uda_dev_inst_put(dev_inst);

    return ret;
}
EXPORT_SYMBOL(uda_agent_dev_ctrl);

int uda_dev_set_remote_udevid(u32 udevid, u32 remote_udevid)
{
    struct uda_dev_inst *dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    dev_inst->para.remote_udevid = remote_udevid;
    uda_dev_inst_put(dev_inst);

    return 0;
}
EXPORT_SYMBOL(uda_dev_set_remote_udevid);

int uda_dev_get_remote_udevid(u32 udevid, u32 *remote_udevid)
{
    return uda_udevid_to_remote_udevid(udevid, remote_udevid);
}
EXPORT_SYMBOL(uda_dev_get_remote_udevid);

bool uda_is_support_udev_mng(void)
{
#ifdef CFG_FEATURE_SURPORT_UDEV_MNG
    return true;
#else
    return false;
#endif
}
EXPORT_SYMBOL(uda_is_support_udev_mng);

static struct device *_uda_get_device(u32 udevid, bool is_agent)
{
    struct device *dev = NULL;
    struct uda_dev_inst *dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return NULL;
    }

    if (is_agent) {
        struct uda_dev_inst *agent_dev_inst = dev_inst->agent_dev;
        if (agent_dev_inst != NULL) {
            dev = agent_dev_inst->para.dev;
        }
    } else {
        dev = dev_inst->para.dev;
    }

    uda_dev_inst_put(dev_inst);

    return dev;
}

struct device *hal_kernel_uda_get_device(u32 udevid)
{
    return uda_get_device(udevid);
}
EXPORT_SYMBOL(hal_kernel_uda_get_device);

struct device *uda_get_device(u32 udevid)
{
    return _uda_get_device(udevid, false);
}
EXPORT_SYMBOL(uda_get_device);

struct device *uda_get_agent_device(u32 udevid)
{
    return _uda_get_device(udevid, true);
}
EXPORT_SYMBOL(uda_get_agent_device);

u32 uda_get_chip_type(u32 udevid)
{
    u32 chip_type = HISI_CHIP_UNKNOWN;
    struct uda_dev_inst *dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return HISI_CHIP_UNKNOWN;
    }

    chip_type = dev_inst->para.chip_type;
    uda_dev_inst_put(dev_inst);

    return chip_type;
}
EXPORT_SYMBOL(uda_get_chip_type);

u32 uda_get_master_id(u32 udevid)
{
    u32 master_id = UDA_INVALID_UDEVID;
    struct uda_dev_inst *dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return UDA_INVALID_UDEVID;
    }

    master_id = dev_inst->para.master_id;
    uda_dev_inst_put(dev_inst);

    return master_id;
}
EXPORT_SYMBOL(uda_get_master_id);

int uda_udevid_to_mia_devid(u32 udevid, struct uda_mia_dev_para *mia_para)
{
    struct uda_dev_inst *dev_inst = NULL;

    if (mia_para == NULL) {
        uda_err("Null ptr\n");
        return -EINVAL;
    }

    if (udevid < UDA_MIA_UDEV_OFFSET) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    *mia_para = dev_inst->mia_para;

    uda_dev_inst_put(dev_inst);
    return 0;
}
EXPORT_SYMBOL(uda_udevid_to_mia_devid);

int uda_mia_devid_to_udevid(struct uda_mia_dev_para *mia_para, u32 *udevid)
{
    struct uda_dev_inst *dev_inst = NULL;

    if ((mia_para == NULL) || (udevid == NULL)) {
        uda_err("Null ptr\n");
        return -EINVAL;
    }

    if (uda_mia_dev_check(mia_para) != 0) {
        return -EINVAL;
    }

    *udevid = uda_make_udevid(mia_para);

    dev_inst = uda_dev_inst_get(*udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid para. (phy_devid=%u; sub_devid=%u)\n", mia_para->phy_devid, mia_para->sub_devid);
        return -EINVAL;
    }

    uda_dev_inst_put(dev_inst);
    return 0;
}
EXPORT_SYMBOL(uda_mia_devid_to_udevid);

int uda_udevid_to_remote_udevid(u32 udevid, u32 *remote_udevid)
{
    struct uda_dev_inst *dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    /* current only support ep device, rc is itself */
    *remote_udevid = ((dev_inst->agent_dev != NULL) || (dev_inst->para.remote_udevid != UDA_INVALID_UDEVID)) ?
        dev_inst->para.remote_udevid : udevid;
    uda_dev_inst_put(dev_inst);

    if (*remote_udevid == UDA_INVALID_UDEVID) {
        uda_warn("Remote udevid not config. (udevid=%u)\n", udevid);
        return -EFAULT;
    }

    return 0;
}

int uda_remote_udevid_to_udevid(u32 remote_udevid, u32 *udevid)
{
    u32 i;

    for (i = 0; i < UDA_UDEV_MAX_NUM; i++) {
        struct uda_dev_inst *dev_inst = uda_dev_inst_get(i);
        if (dev_inst == NULL) {
            continue;
        }

        if (dev_inst->para.remote_udevid == remote_udevid) {
            *udevid = i;
            uda_dev_inst_put(dev_inst);
            return 0;
        }
        uda_dev_inst_put(dev_inst);
    }

    return -EINVAL;
}
EXPORT_SYMBOL(uda_remote_udevid_to_udevid);

u32 uda_get_dev_num(void)
{
    u32 udevid, udev_num = 0;

    for (udevid = 0; udevid < UDA_MAX_PHY_DEV_NUM; udevid++) {
        if (dev_insts[udevid] != NULL) {
            udev_num++;
        }
    }

    return udev_num;
}
EXPORT_SYMBOL(uda_get_dev_num);

u32 uda_get_mia_dev_num(void)
{
    u32 udevid, udev_num = 0;

    for (udevid = UDA_MIA_UDEV_OFFSET; udevid < UDA_UDEV_MAX_NUM; udevid++) {
        if (dev_insts[udevid] != NULL) {
            udev_num++;
        }
    }

    return udev_num;
}
EXPORT_SYMBOL(uda_get_mia_dev_num);

u32 uda_get_host_id(void)
{
    return UDA_HOST_ID;
}
EXPORT_SYMBOL(uda_get_host_id);

bool uda_is_udevid_exist(u32 udevid)
{
    struct uda_dev_inst *dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        return false;
    }
    uda_dev_inst_put(dev_inst);

    return true;
}
EXPORT_SYMBOL(uda_is_udevid_exist);

int uda_set_udevid_reorder_para(u32 add_id, struct udevid_reorder_para *para)
{
    if (add_id >= UDA_UDEV_MAX_NUM || para == NULL) {
        uda_err("Invalid add_id or reorder_para is NULL. (add_id=%u; para_is_NULL=%d)\n", add_id, (para == NULL));
        return -EINVAL;
    }

    if (reorder_para == NULL) {
        reorder_para = dbl_kzalloc(sizeof(struct udevid_reorder_para) * UDA_UDEV_MAX_NUM, GFP_KERNEL | __GFP_ACCOUNT);
        if (reorder_para == NULL) {
            uda_err("Out of memory.\n");
            return -ENOMEM;
        }
    }
    memcpy_fromio(&reorder_para[add_id], para, sizeof(struct udevid_reorder_para));
    return 0;
}
EXPORT_SYMBOL(uda_set_udevid_reorder_para);

int uda_get_udevid_reorder_para(u32 udevid, struct udevid_reorder_para *para)
{
    struct uda_dev_inst *dev_inst = NULL;

    if (udevid >= UDA_UDEV_MAX_NUM || para == NULL) {
        uda_err("Invalid udevid or reorder_para is NULL. (udevid=%u; info_is_NULL=%d)\n", udevid, (para == NULL));
        return -EINVAL;
    }

    dev_inst = uda_dev_inst_get(udevid);
    if (dev_inst == NULL) {
        uda_err("Invalid udevid. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    if (reorder_para == NULL) {
        uda_dev_inst_put(dev_inst);
        uda_err("Reorder_para is NULL.\n");
        return -EINVAL;
    }
    *para = reorder_para[dev_inst->para.add_id];
    uda_dev_inst_put(dev_inst);
    return 0;
}
EXPORT_SYMBOL(uda_get_udevid_reorder_para);

int uda_udevid_to_add_id(u32 udevid, u32 *add_id)
{
    struct uda_dev_inst *dev_inst = NULL;

    dev_inst = uda_dev_inst_get_ex(udevid);
    if (dev_inst == NULL) {
        *add_id = udevid;
        uda_debug("Invalid udevid. (udevid=%u)\n", udevid);
        return -EINVAL;
    }
    *add_id = dev_inst->para.add_id;
    uda_dev_inst_put(dev_inst);
    return 0;
}
EXPORT_SYMBOL(uda_udevid_to_add_id);

int uda_add_id_to_udevid(u32 add_id, u32 *udevid)
{
    struct uda_dev_inst *dev_inst = NULL;
    int i;

    if (udevid == NULL) {
        uda_err("Null ptr\n");
        return -EINVAL;
    }

    for (i = 0; i < UDA_UDEV_MAX_NUM; i++) {
        dev_inst = uda_dev_inst_get_ex(i);
        if (dev_inst == NULL) {
            continue;
        }
        if (dev_inst->para.add_id == add_id) {
            *udevid = i;
            uda_dev_inst_put(dev_inst);
            return 0;
        }
        uda_dev_inst_put(dev_inst);
    }
    *udevid = add_id;
    uda_debug("Invalid add ID. (add_id=%u)\n", add_id);
    return -EINVAL;
}
EXPORT_SYMBOL(uda_add_id_to_udevid);

int uda_dev_init(void)
{
    (void)memset_s(dev_insts, sizeof(dev_insts), 0, sizeof(dev_insts));
    mutex_init(&uda_dev_mutex);
    rwlock_init(&uda_dev_lock);
    INIT_WORK(&reorder_insts.udevid_reorder_work, uda_udevid_reorder_work);

    return 0;
}

void uda_dev_uninit(void)
{
    u32 udevid;

    (void)cancel_work_sync(&reorder_insts.udevid_reorder_work);
    if (reorder_insts.dev_para != NULL) {
        dbl_kfree(reorder_insts.dev_para);
        reorder_insts.dev_para = NULL;
    }

    if (reorder_para != NULL) {
        dbl_kfree(reorder_para);
        reorder_para = NULL;
    }

    for (udevid = 0; udevid < UDA_UDEV_MAX_NUM; udevid++) {
        if (dev_insts[udevid] != NULL) {
            if (dev_insts[udevid]->agent_dev != NULL) {
                uda_destroy_dev_inst(dev_insts[udevid]->agent_dev);
            }
            uda_destroy_dev_inst(dev_insts[udevid]);
        }
    }
}

