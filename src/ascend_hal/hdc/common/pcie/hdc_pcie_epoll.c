/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdint.h>

#include "ascend_hal.h"
#include "hdc_cmn.h"
#include "hdc_pcie_drv.h"
#include "hdc_epoll.h"

STATIC drvError_t drv_hdc_pcie_epoll_create(struct hdc_epoll_head *epoll_head, signed int size)
{
    signed int ret;

    epoll_head->bind_fd = hdc_pcie_create_bind_fd();
    if (epoll_head->bind_fd == (mmProcess)EN_ERROR) {
        HDC_LOG_ERR("Open pcie device failed. (strerror=\"%s\")\n", strerror(errno));
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = hdc_pcie_epoll_alloc_fd(epoll_head->bind_fd, size, (signed int *)&epoll_head->epfd);
    if (ret != 0) {
        hdc_pcie_close_bind_fd(epoll_head->bind_fd);
        epoll_head->bind_fd = HDC_EPOLL_FD_INVALID;
        HDC_LOG_ERR("Epoll alloc fd failed. (ret=%d)\n", ret);
        return DRV_ERROR_OVER_LIMIT;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_hdc_pcie_epoll_ctl(struct hdc_epoll_head *epoll_head,
    signed int op, void *target, const struct drvHdcEvent *event)
{
    signed int para1, para2 = 0;
    struct hdc_server_head *server_head = (struct hdc_server_head *)target;
    struct hdc_session *session_head = (struct hdc_session *)target;
    struct hdcdrv_cmd_epoll_ctl epollctl;

    if (event->events & HDC_EPOLL_CONN_IN) {
        para1 = server_head->deviceId;
        para2 = server_head->serviceType;
    } else {
        para1 = session_head->sockfd;  /* hdc connect session */
    }

    epollctl.epfd = epoll_head->epfd;
    epollctl.dev_id = server_head->deviceId;
    epollctl.op = op;
    epollctl.para1 = para1;
    epollctl.para2 = para2;

    return hdc_pcie_epoll_ctl(epoll_head->bind_fd, &epollctl, event);
}

STATIC drvError_t drv_hdc_pcie_epoll_wait(const struct hdc_epoll_head *epoll_head,
    struct drvHdcEvent *events, signed int maxevents, signed int timeout, signed int *eventnum)
{
    struct hdcdrv_event *epoll_events = NULL;
    struct hdcdrv_cmd_epoll_wait epollwait;
    signed int ret, i;

    epoll_events = (struct hdcdrv_event *)malloc((size_t)maxevents * sizeof(struct hdcdrv_event));
    if (epoll_events == NULL) {
        HDC_LOG_ERR("Call malloc failed.\n");
        return DRV_ERROR_MALLOC_FAIL;
    }

    epollwait.epfd = epoll_head->epfd;
    epollwait.dev_id = HDCDRV_DEFAULT_DEV_ID;
    epollwait.timeout = timeout;
    epollwait.maxevents = maxevents;
    epollwait.event = epoll_events;
    ret = hdc_pcie_epoll_wait(epoll_head->bind_fd, &epollwait);
    if (ret != 0) {
        if (ret == (-HDCDRV_EPOLL_CLOSE)) {
            ret = DRV_ERROR_EPOLL_CLOSE;
            goto out;
        }
        HDC_LOG_ERR("Call epoll_wait failed. (epfd=%d; ret=%d)\n", epoll_head->epfd, ret);
        ret = DRV_ERROR_IOCRL_FAIL;
        goto out;
    }
    *eventnum = epollwait.ready_event;
    if ((*eventnum < 0) || (*eventnum > HDCDRV_EPOLL_FD_EVENT_NUM)) {
        HDC_LOG_ERR("Input parameter eventnum is error. (epfd=%d; eventnum=%d)\n", epoll_head->epfd, *eventnum);
        ret = DRV_ERROR_IOCRL_FAIL;
        goto out;
    }
    for (i = 0; i < *eventnum; i++) {
        events[i].events = epoll_events[i].events;
        events[i].data = (uintptr_t)(epoll_events[i].data);
    }

out:
    free(epoll_events);
    epoll_events = NULL;

    return ret;
}

STATIC drvError_t drv_hdc_pcie_epoll_close(struct hdc_epoll_head *epoll_head)
{
    signed int ret;
    signed int epfd;

    epfd = (signed int)epoll_head->epfd;

    ret = hdc_pcie_epoll_free_fd(epoll_head->bind_fd, epfd);
    if (ret != 0) {
        HDC_LOG_ERR("Epoll free fd failed. (epfd=%d; ret=%d)\n", epfd, ret);
        return DRV_ERROR_SOCKET_CLOSE;
    }

    hdc_pcie_close_bind_fd(epoll_head->bind_fd);
    epoll_head->bind_fd = HDC_EPOLL_FD_INVALID;
    epoll_head->epfd = 0;

    return DRV_ERROR_NONE;
}

struct hdc_epoll_ops *drv_get_hdc_pcie_epoll_ops(void)
{
    static struct hdc_epoll_ops pcie_epoll_ops = {
        drv_hdc_pcie_epoll_create,
        drv_hdc_pcie_epoll_ctl,
        drv_hdc_pcie_epoll_wait,
        drv_hdc_pcie_epoll_close,
    };

    return &pcie_epoll_ops;
}