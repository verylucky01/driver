/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UT_TEST
#include "prof_adapt.h"
#include "prof_buff.h"
#include "prof_event_master.h"
#include "prof_hdc.h"
#include "prof_adapt_hdc.h"

drvError_t prof_hdc_kernel_get_channels(unsigned int dev_id, struct prof_channel_list *channels)
{
    drvError_t ret;

    ret = prof_hdc_get_channels(dev_id, channels);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to get channels. (devid=%u, ret=%d)\n", dev_id, (int)ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static drvError_t prof_hdc_chan_init(uint32_t dev_id, uint32_t chan_id, bool event_flag, char **priv)
{
    (void)event_flag;
    uint8_t *buff = NULL;
    drvError_t ret;

    ret = prof_buff_init(chan_id, &buff);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to init buff. (dev_id=%u, chan_id=%u)\n", dev_id, chan_id);
        return ret;
    }

    *priv = (char *)buff;
    return DRV_ERROR_NONE;
}

static void prof_hdc_chan_uninit(char **priv)
{
    uint8_t *buff = (uint8_t *)(*priv);

    prof_buff_uninit(&buff);

    *priv = NULL;
}

static drvError_t prof_hdc_chan_start(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para, char *priv)
{
    (void)priv;
    drvError_t ret;

    ret = prof_hdc_start(dev_id, chan_id, para);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to start. (dev_id=%u, chan_id=%u, ret=%d)\n", dev_id, chan_id, (int)ret);
        return ret;
    }

    if (para->remote_pid != 0) {
        ret = prof_event_start(dev_id, chan_id, para);
        if (ret != DRV_ERROR_NONE) {
            (void)prof_hdc_stop(dev_id, chan_id);
            PROF_ERR("Failed to send start event. (dev_id=%u, chan_id=%u, ret=%d)\n", dev_id, chan_id, (int)ret);
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

static drvError_t prof_hdc_chan_stop(uint32_t dev_id, uint32_t chan_id, struct prof_user_stop_para *para, char *priv)
{
    uint8_t *buff = (uint8_t *)priv;
    drvError_t ret;

    if (para->remote_pid != 0) {
        ret = prof_event_stop(dev_id, chan_id, para);
        if (ret != DRV_ERROR_NONE) {
            PROF_ERR("Failed to send stop event. (dev_id=%u, chan_id=%u, ret=%d)\n", dev_id, chan_id, (int)ret);
            /* send failed maybe cp or adda proc had exited, continue stop kernel chan */
        }
    }

    ret = prof_hdc_stop(dev_id, chan_id);

    prof_buff_wait_read_empty(buff, dev_id, chan_id);

    return ret;
}

static drvError_t prof_hdc_chan_flush(uint32_t dev_id, uint32_t chan_id, uint32_t *data_len, char *priv)
{
    uint8_t *buff = (uint8_t *)priv;
    drvError_t ret;

    ret = prof_hdc_flush(dev_id, chan_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    *data_len = prof_buff_get_data_len(buff);
    return DRV_ERROR_NONE;
}

static int prof_hdc_chan_read(uint32_t dev_id, uint32_t chan_id, prof_user_read_para_t *read_para, char *priv)
{
    (void)dev_id;
    (void)chan_id;
    uint8_t *buff = (uint8_t *)priv;

    return prof_buff_read(buff, read_para->out_buf, read_para->buf_size);
}

static drvError_t prof_hdc_chan_query(unsigned int dev_id, unsigned int chan_id, unsigned int *avail_len, char *priv)
{
    (void)dev_id;
    (void)chan_id;
    (void)avail_len;
    (void)priv;
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t prof_hdc_chan_report(unsigned int dev_id, unsigned int chan_id, void *data, unsigned int data_len, char *priv)
{
    uint8_t *buff = (uint8_t *)priv;
    struct prof_adapt_core_notifier *notifier = NULL;
    drvError_t ret;

    ret = prof_buff_write(buff, data, data_len);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to write data to local buff. (dev_id=%u, chan_id=%u, data_len=%u, ret=%d)\n",
            dev_id, chan_id, data_len, (int)ret);
        return ret;
    }

    notifier = prof_adapt_get_notifier();
    notifier->poll_report(dev_id, chan_id);
    return DRV_ERROR_NONE;
}

static struct prof_chan_ops prof_hdc_chan_ops = {
    .init = prof_hdc_chan_init,
    .uninit = prof_hdc_chan_uninit,
    .start = prof_hdc_chan_start,
    .stop = prof_hdc_chan_stop,
    .flush = prof_hdc_chan_flush,
    .read = prof_hdc_chan_read,
    .query = prof_hdc_chan_query,
    .report = prof_hdc_chan_report,
};

drvError_t prof_hdc_get_chan_ops(struct prof_chan_ops **ops)
{
    *ops = &prof_hdc_chan_ops;
    return DRV_ERROR_NONE;
}

#else
int prof_adapt_hdc_ut_test(void)
{
    return 0;
}
#endif
