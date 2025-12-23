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
#include "hdcdrv_cmd_ioctl.h"
#include "hdc_epoll.h"

STATIC drvError_t drv_hdc_sock_epoll_create(struct hdc_epoll_head *epoll_head, signed int size)
{
    epoll_head->epoll_events = (struct epoll_event *)malloc((size_t)size * sizeof(struct epoll_event));
    if (epoll_head->epoll_events == NULL) {
        HDC_LOG_ERR("Call malloc socket_event failed.\n");
        return DRV_ERROR_MALLOC_FAIL;
    }

    epoll_head->epfd = epoll_create(size);
    if (epoll_head->epfd < 0) {
        free(epoll_head->epoll_events);
        epoll_head->epoll_events = NULL;
        HDC_LOG_ERR("Epoll create error. (strerror=\"%s\"; errno=%d)\n", strerror(errno), errno);
        return DRV_ERROR_SOCKET_CREATE;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_hdc_sock_epoll_ctl(struct hdc_epoll_head *epoll_head,
    signed int op, void *target, const struct drvHdcEvent *event)
{
    struct epoll_event socket_event;
    signed int epfd;
    signed int socket_op, socket_fd;
    struct hdc_server_head *server_head = (struct hdc_server_head *)target;
    struct hdc_session *session_head = (struct hdc_session *)target;

    epfd = (signed int)epoll_head->epfd;

    if (event->events & HDC_EPOLL_CONN_IN) {
        socket_fd = server_head->listenFd;
    } else {
        socket_fd = session_head->sockfd;
    }

    if (op == HDC_EPOLL_CTL_ADD) {
        socket_op = EPOLL_CTL_ADD;
    } else {
        socket_op = EPOLL_CTL_DEL;
    }

    socket_event.events = EPOLLIN;
    socket_event.data.ptr = (void *)event->data;

    return epoll_ctl(epfd, socket_op, socket_fd, &socket_event);
}

STATIC drvError_t drv_hdc_sock_epoll_wait(const struct hdc_epoll_head *epoll_head, struct drvHdcEvent *events,
    signed int maxevents, signed int timeout, signed int *eventnum)
{
    signed int i;
    signed int epfd = (signed int)epoll_head->epfd;

    do {
        *eventnum = epoll_wait(epfd, epoll_head->epoll_events, maxevents, timeout);
    } while ((*eventnum == -1) && (mm_get_error_code() == EINTR));

    if ((*eventnum < 0) || (*eventnum > HDCDRV_EPOLL_FD_EVENT_NUM)) {
        HDC_LOG_ERR("Call epoll_wait error. (epfd=%d; strerror=\"%s\"; errno=%d)\n", epfd, strerror(errno), errno);
        return DRV_ERROR_IOCRL_FAIL;
    }

    for (i = 0; i < *eventnum; i++) {
        events[i].events = epoll_head->epoll_events[i].events;
        events[i].data = (uintptr_t)(epoll_head->epoll_events[i].data.ptr);
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_hdc_sock_epoll_close(struct hdc_epoll_head *epoll_head)
{
    signed int ret;
    signed int epfd;

    epfd = (signed int)epoll_head->epfd;

    ret = close(epfd);
    if (ret != 0) {
        HDC_LOG_ERR("Close epfd error. (epfd=%d; strerror=\"%s\"; errno=%d)\n", epfd, strerror(errno), errno);
        return DRV_ERROR_SOCKET_CLOSE;
    }

    free(epoll_head->epoll_events);
    epoll_head->epoll_events = NULL;

    return DRV_ERROR_NONE;
}

struct hdc_epoll_ops *drv_get_hdc_sock_epoll_ops(void)
{
    static struct hdc_epoll_ops sock_epoll_ops = {
        drv_hdc_sock_epoll_create,
        drv_hdc_sock_epoll_ctl,
        drv_hdc_sock_epoll_wait,
        drv_hdc_sock_epoll_close,
    };

    return &sock_epoll_ops;
}