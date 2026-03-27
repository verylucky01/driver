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

#ifndef PBL_KREF_SAFE_H
#define PBL_KREF_SAFE_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_task_pub.h"
#include "ka_system_pub.h"

struct kref_safe {
    ka_kref_t kref;
    void (*release)(struct kref_safe *kref_s);
    ka_work_struct_t work;
};

static void kref_safe_release_work(ka_work_struct_t *work)
{
    struct kref_safe *kref_s = ka_container_of(work, struct kref_safe, work);
    kref_s->release(kref_s);
}

static inline void kref_safe_init(struct kref_safe *kref_s)
{
    KA_TASK_INIT_WORK(&kref_s->work, kref_safe_release_work);
	ka_base_kref_init(&kref_s->kref);
}

static inline unsigned int kref_safe_read(const struct kref_safe *kref_s)
{
	return ka_base_kref_read((const ka_kref_t *)&kref_s->kref);
}

static inline void kref_safe_get(struct kref_safe *kref_s)
{
	ka_base_kref_get(&kref_s->kref);
}

static inline int kref_safe_put(struct kref_safe *kref_s, void (*release)(struct kref_safe *kref_s))
{
    ka_kref_t *kref = &kref_s->kref;

    if (ka_base_refcount_dec_and_test(&kref->refcount)) {
        if (ka_system_in_interrupt()) {
            /* The blocking function may be invoked in release.
               If the blocking function is invoked during the
               interruption, an exception occurs. Therefore,
               the release work is started. */
            kref_s->release = release;
            ka_task_schedule_work(&kref_s->work);
        } else {
            release(kref_s);
        }
		return 1;
	}
	return 0;
}

static inline int __must_check kref_safe_get_unless_zero(struct kref_safe *kref_s)
{
	return ka_base_kref_get_unless_zero(&kref_s->kref);
}

#endif

