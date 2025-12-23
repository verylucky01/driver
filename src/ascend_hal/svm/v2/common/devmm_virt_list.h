/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_LIST_H
#define SVM_LIST_H

#ifndef NULL
#define NULL 0
#endif

struct devmm_virt_list_head {
    struct devmm_virt_list_head *next, *prev;
};


#define SVM_LIST_HEAD_INIT(name) \
    {                            \
        &(name), &(name)         \
    }

#define SVM_LIST_HEAD(name) (struct devmm_virt_list_head name = SVM_LIST_HEAD_INIT(name))

static inline void SVM_INIT_LIST_HEAD(struct devmm_virt_list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline void _devmm_virt_list_add(struct devmm_virt_list_head *new_, struct devmm_virt_list_head *prev,
                                        struct devmm_virt_list_head *next)
{
    next->prev = new_;
    new_->next = next;
    new_->prev = prev;
    prev->next = new_;
}

static inline void devmm_virt_list_add(struct devmm_virt_list_head *new_, struct devmm_virt_list_head *head)
{
    _devmm_virt_list_add(new_, head, head->next);
}

static inline void devmm_virt_list_add_tail(struct devmm_virt_list_head *new_, struct devmm_virt_list_head *head)
{
    _devmm_virt_list_add(new_, head->prev, head);
}

static inline void _devmm_virt_list_del(struct devmm_virt_list_head *prev, struct devmm_virt_list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void _devmm_virt_list_del_entry(struct devmm_virt_list_head *entry)
{
    _devmm_virt_list_del(entry->prev, entry->next);
}

#define LIST_POISON1 NULL
#define LIST_POISON2 NULL

static inline void devmm_virt_list_del(struct devmm_virt_list_head *entry)
{
    _devmm_virt_list_del(entry->prev, entry->next);
    entry->next = (struct devmm_virt_list_head *)LIST_POISON1;
    entry->prev = (struct devmm_virt_list_head *)LIST_POISON2;
}

static inline void devmm_virt_list_del_init(struct devmm_virt_list_head *entry)
{
    _devmm_virt_list_del_entry(entry);
    SVM_INIT_LIST_HEAD(entry);
}

static inline int devmm_virt_list_is_last(const struct devmm_virt_list_head *list,
                                          const struct devmm_virt_list_head *head)
{
    return list->next == head;
}

static inline int devmm_virt_list_empty(const struct devmm_virt_list_head *head)
{
    return head->next == head;
}

static inline int devmm_virt_list_empty_careful(const struct devmm_virt_list_head *head)
{
    struct devmm_virt_list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

#define devmm_virt_list_entry(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))

#define devmm_virt_list_first_entry(ptr, type, member) \
    devmm_virt_list_entry(((struct devmm_virt_list_head *)(ptr))->next, type, member)

#define devmm_virt_list_last_entry(ptr, type, member) \
    devmm_virt_list_entry(((struct devmm_virt_list_head *)(ptr))->prev, type, member)

#define devmm_virt_list_first_entry_or_null(ptr, type, member) \
    (!devmm_virt_list_empty(ptr) ? devmm_virt_list_first_entry(ptr, type, member) : NULL)

#define devmm_virt_list_next_entry_slab(pos, member) \
    devmm_virt_list_entry((pos)->member.next, struct devmm_virt_slab, member)

#define devmm_virt_list_for_each(pos, head) for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

#define devmm_virt_list_for_each_prev(pos, head) \
    for ((pos) = (head)->prev; (pos) != (head); (pos) = (pos)->prev)

#define devmm_virt_list_for_each_safe(pos, n, head) \
    for ((pos) = (head)->next, (n) = (pos)->next; (pos) != (head); (pos) = (n), (n) = (pos)->next)

#define devmm_virt_list_for_each_prev_safe(pos, n, head) \
    for ((pos) = (head)->prev, (n) = (pos)->prev; (pos) != (head); (pos) = (n), (n) = (pos)->prev)

#define devmm_virt_list_for_each_entry_slab(pos, head, member)                                                \
    for ((pos) = devmm_virt_list_first_entry(head, struct devmm_virt_slab, member); &(pos)->member != (head); \
         (pos) = devmm_virt_list_next_entry_slab(pos, member))

#endif /* _SVM_LIST_H_ */
