/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include "securec.h"

#include "ioctl_comm_def.h"
#include "lingqu-dcmi-log.h"
#include "lingqu-dcmi.h"

#define DEV_NAME "/dev/lqdcmi_pcidev"
#define MAX_SUBCRIBLE_NUMS 15
#define MAX_FAULT_LEVEL 3
#define MAX_FAULT_NUMS 65535

#define COM_IOCTL_CMD 100

#define PCI_BAR_ID 4
#define PCI_BUS_ID 4

#define THREAD_STACK_SIZE 256
#define THREAD_PRIORITY (10)
#define MULTIPLE 2

#define MILL 1000

#ifndef STATIC_SKIP
    #define STATIC static
#else
    #define STATIC
#endif

unsigned int g_faultlist_size = 0;
STATIC char *g_shared_memory_address;
int g_lqdcmi_dev_fd = -1;
STATIC EventMapping mapping[] = EVENT_MAPPING_INITIALIZER;

// pci bar空间地址读输入参数信息
typedef struct {
    unsigned int bus; // 这条 PCI 总线的总线编号
    unsigned int device;
    unsigned int function;
    unsigned int bar; // bar id
    unsigned int addr; // bar中的地址
    unsigned int dataLen;
} PCI_BAR_RD_BUFFER;

STATIC FaultEventNodeTable g_user_event_table[CAPACITY] = {0};

int TcIoctlGetHeadInfo(void *pdata)
{
    IOCTL_CMD_S ioctlCmd = {0};
    PCI_BAR_RD_BUFFER paraIn;

    const int fd = open(DEV_NAME, O_RDWR, 0);
    if (fd < 0) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "open %s err! errno = %d\n", DEV_NAME, fd);
        return LQ_DCMI_ERR_CODE_DEVICE_OPEN_FAIL;
    }

    paraIn.bar = PCI_BAR_ID;
    paraIn.bus = PCI_BUS_ID;
    paraIn.device = 0;
    paraIn.function = 0;
    paraIn.dataLen = 0; // 需要更新为 32K 每次更新

    ioctlCmd.cmd = IOCTL_GET_HEAD_INFO;
    ioctlCmd.len = 0;
    ioctlCmd.in_addr = (void *) &paraIn;
    ioctlCmd.out_addr = pdata;
    const int ret = ioctl(fd, COM_IOCTL_CMD, (void *) (&ioctlCmd)); // 这里用户态无需感知 head, tail, 返回新增的节点信息即可
    (void) close(fd);
    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "rdflash ioctl cmd[%d] fail, err %d\n", ioctlCmd.cmd, ret);
        return LQ_DCMI_ERR_CODE_IOCTL_FAIL;
    }
    return LQ_DCMI_OK;
}

int TcIoctlGetAllFault(FaultEventNodeTable *event_table)
{
    IOCTL_CMD_S ioctlCmd = {0};

    const int fd = open(DEV_NAME, O_RDWR, 0);
    if (fd < 0) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "open %s err! errno = %d\n", DEV_NAME, fd);
        return LQ_DCMI_ERR_CODE_DEVICE_OPEN_FAIL;
    }

    PCI_BAR_RD_BUFFER paraIn;

    paraIn.bar = PCI_BAR_ID;
    paraIn.bus = PCI_BUS_ID;
    paraIn.device = 0;
    paraIn.function = 0;
    paraIn.dataLen = 0; // 需要更新为 32K 每次更新

    ioctlCmd.cmd = IOCTL_GET_NODE_INFO;
    ioctlCmd.len = 0;
    ioctlCmd.out_size = sizeof(FaultEventNodeTable) * CAPACITY;
    ioctlCmd.in_addr = (void *) &paraIn;
    ioctlCmd.out_addr = event_table;
    const int ret = ioctl(fd, COM_IOCTL_CMD, (void *) (&ioctlCmd)); // 这里用户态无需感知 head, tail, 返回新增的节点信息即可
    (void) close(fd);

    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "rd fault ioctl cmd[%d] fail, err %d\n", ioctlCmd.cmd, ret);
        return LQ_DCMI_ERR_CODE_IOCTL_FAIL;
    }

    return LQ_DCMI_OK;
}

int TcGetVersion(unsigned int *version)
{
    void *data = malloc(sizeof(SramDescCtlHeader));
    if (data == NULL) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "Memory allocation failed");
        return LQ_DCMI_ERR_CODE_MEM_CREATE_FAIL;
    }
    const int ret = TcIoctlGetHeadInfo(data);
    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "ioctl rdflash failed, ret=%d", ret);
        free(data);
        return ret;
    }

    const SramDescCtlHeader *header = (SramDescCtlHeader *) data;

    *version = header->version;
    g_faultlist_size = header->nodeNum;
    free(data);

    return LQ_DCMI_OK;
}

STATIC volatile bool g_initFlag = false;
STATIC volatile bool g_subscribeFlag = false;

typedef enum {
    THREAD_RUNNING,
    THREAD_STOPPED
} ThreadRunningState;

STATIC volatile ThreadRunningState g_globalThreadFlag = THREAD_STOPPED;

typedef struct {
    pthread_t threadId;
    pthread_mutex_t threadLock;
} UpdateThread;

STATIC UpdateThread g_globalUpdateThread = {
    .threadId = 0,
    .threadLock = PTHREAD_MUTEX_INITIALIZER,
};

typedef struct SubscribeAclNode {
    LqDcmiEventFilter filter;
    LqDcmiFaultEventCallback handler;
    struct SubscribeAclNode *next;
} SubscribeAclNode;

typedef struct {
    SubscribeAclNode *aclNode;
} SubscribeAclList;

STATIC SubscribeAclList g_subscribeAclList = {NULL};

int SubscribeAclListAdd(SubscribeAclList *list, LqDcmiEventFilter filter, LqDcmiFaultEventCallback handler)
{
    LQ_DCMI_TYPE_LOG(LOG_INFO,  "SubscribeAclListAdd");
    SubscribeAclNode *newNode = (SubscribeAclNode *)malloc(sizeof(SubscribeAclNode));
    if (newNode == NULL) {
        // 处理内存不足的情况
        return LQ_DCMI_ERR_CODE_MEM_CREATE_FAIL;
    }
    // 初始化新节点的数据
    newNode->filter = filter;
    newNode->handler = handler;
    newNode->next = NULL;

    if (list->aclNode == NULL) {
        // 链表为空，直接添加为头节点
        list->aclNode = newNode;
    } else {
        int length = 1;
        // 遍历到尾部节点
        SubscribeAclNode *current = list->aclNode;
        while (current->next != NULL) {
            length++;
            current = current->next;
        }
        if (length > MAX_SUBCRIBLE_NUMS) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "SubscribeAclListAdd:over max subscribe nums");
            return LQ_DCMI_ERR_CODE_MAX_SUBCRIBLE_NUMS_EXCEEDED;
        }
        // 将新节点连接到尾部
        current->next = newNode;
    }
    return LQ_DCMI_OK;
}

SubscribeAclNode *SubscribeAclListFind(SubscribeAclList *list, LqDcmiEventFilter filter)
{
    SubscribeAclNode *current = list->aclNode;
    while (current != NULL) {
        const LqDcmiEventFilter currentfilter = current->filter;
        const LqDcmiEventFilterFlag currentfilterFlag = current->filter.filterFlag;

        bool match = true;
        if (!(currentfilterFlag == filter.filterFlag)) { match = false; }

        if (match) {
            if (!(currentfilter.chipId == filter.chipId)) { match = false; }
            if (!(currentfilter.eventTypeId == filter.eventTypeId)) { match = false; }
            if (!(currentfilter.eventId == filter.eventId)) { match = false; }
            if (!(currentfilter.severity == filter.severity)) { match = false; }

            if (match) {
                return current;
            }
        }

        current = current->next;
    }
    return NULL; // 没有找到匹配的节点
}

unsigned int SubscribeAclListRemove(SubscribeAclList *list, LqDcmiEventFilter filter)
{
    LQ_DCMI_TYPE_LOG(LOG_INFO,  "SubscribeAclListRemove");
    if (list->aclNode == NULL) {
        // 链表为空，无需删除
        return LQ_DCMI_ERR_CODE_UNSUBSCRIBE_FAIL;
    }

    SubscribeAclNode *current1 = SubscribeAclListFind(list, filter);

    if (current1 == NULL) {
        return LQ_DCMI_ERR_CODE_UNSUBSCRIBE_FAIL;
    }

    // 删除头节点
    if (list->aclNode == current1) {
        SubscribeAclNode *temp = list->aclNode;
        list->aclNode = list->aclNode->next;
        free(temp);
        return LQ_DCMI_OK;
    }

    // 遍历链表，删除指定节点
    SubscribeAclNode *current = list->aclNode;
    while (current->next != NULL) {
        if (current->next == current1) {
            // 找到目标节点
            SubscribeAclNode *temp = current->next;
            current->next = current->next->next;
            free(temp);
            return LQ_DCMI_OK;
        }
        current = current->next;
    }
    return LQ_DCMI_OK;
}

void SubScribeHook(LqDcmiEvent *event)
{
    if (g_subscribeFlag == false) { return; }

    SubscribeAclNode *current = g_subscribeAclList.aclNode;
    while (current != NULL) {
        const LqDcmiEventFilter filter = current->filter;
        const LqDcmiEventFilterFlag filterFlag = current->filter.filterFlag;

        if (filterFlag == 0) { // filterflag = 0表示故障全订阅
            LQ_DCMI_TYPE_LOG(LOG_INFO,  "SubScribeHook event, filter is 0");
            current->handler(event);
        } else {
            bool match = true;
            if (filterFlag & EVENT_TYPE_ID) { if (filter.eventTypeId != event->eventType) { match = false; } }
            if (filterFlag & EVENT_ID) { if (filter.eventId != event->subType) { match = false; } }
            if (filterFlag & SEVERITY) { if (filter.severity != event->severity) { match = false; } }
            if (filterFlag & CHIP_ID) { if (filter.chipId != event->switchChipid) { match = false; } }

            if (match) {
                LQ_DCMI_TYPE_LOG(LOG_INFO,  "SubScribeHook event, eventType=%d, subtype=%d,chipid=%d", event->eventType,
                    event->subType, event->switchChipid);
                current->handler(event);
            }
        }
        current = current->next;
    }
}

void SubscribeAclListRemoveAll(SubscribeAclList *list)
{
    LQ_DCMI_TYPE_LOG(LOG_INFO,  "SubscribeAclListRemoveAll");
    if (list == NULL) {
        // 处理 list 为空的情况，根据需求决定是否返回或处理
        return;
    }
    SubscribeAclNode *current = list->aclNode;
    SubscribeAclNode *nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next; // 保存下一个节点
        free(current); // 释放当前节点
        current = nextNode; // 移动到下一个节点
    }
    list->aclNode = NULL; // 将链表头设为 NULL，表示链表已清空
    return;
}

static void HandleFault(LqDcmiEvent *event, FaultEventNodeTable *currFault,
                        ChipFaultInfo *currChipFault, PortFaultInfo *currPortFault,
                        unsigned int alarmType)
{
    currFault->alarmFlag = 1;
    currFault->chipId = event->switchChipid;

    if (alarmType == CHIP_ALARM) {
        currChipFault->alarmFlag = 1;
        if (IsPortNodeInfoZero(currChipFault->chipNodeInfo)) {
            currChipFault->chipNodeInfo = *event;
            SubScribeHook(event);
        }
    } else if (alarmType == PORT_ALARM) {
        currChipFault->alarmFlag = 1;
        currPortFault->alarmFlag = 1;
        if (IsPortNodeInfoZero(currPortFault->portNodeInfo)) {
            currPortFault->portNodeInfo = *event;
            SubScribeHook(event);
        }
    }
}

static void HandleRecovery(LqDcmiEvent *event, __attribute__ ((unused)) FaultEventNodeTable *currFault,
                           ChipFaultInfo *currChipFault, PortFaultInfo *currPortFault,
                           unsigned int alarmType)
{
    unsigned int ret = 0;
    if (alarmType == CHIP_ALARM) {
        currChipFault->alarmFlag = 0;
        if (!IsPortNodeInfoZero(currChipFault->chipNodeInfo)) {
            ret = memset_s(&currChipFault->chipNodeInfo, sizeof(currChipFault->chipNodeInfo), 0,
                sizeof(currChipFault->chipNodeInfo));
            if (ret != 0) {
                LQ_DCMI_TYPE_LOG(LOG_ERR, "HandleRecovery memset_s fail");
                return;
            }
            SubScribeHook(event);
        }
    } else if (alarmType == PORT_ALARM) {
        currPortFault->alarmFlag = 0;
        if (!IsPortNodeInfoZero(currPortFault->portNodeInfo)) {
            ret = memset_s(&currPortFault->portNodeInfo, sizeof(currPortFault->portNodeInfo), 0,
                sizeof(currPortFault->portNodeInfo));
            if (ret != 0) {
                LQ_DCMI_TYPE_LOG(LOG_ERR, "HandleRecovery memset_s fail");
                return;
            }
            SubScribeHook(event);
        }

        int flag = 0;
        for (int i = 0; i < NUM_PORTS; ++i) { flag |= currChipFault->portFaultInfo[i].alarmFlag; }
        if (flag == 0) { currChipFault->alarmFlag = 0; }
    }
}

static void EventHandler(LqDcmiEvent *event)
{
    PortFaultInfo *currPortFault = NULL;
    unsigned int subTypeIndex = find_index_by_sub_type(mapping, sizeof(mapping)/sizeof(mapping[0]), event->subType);
    if (subTypeIndex == INDEX_NOT_FOUND) {
        LQ_DCMI_TYPE_LOG(LOG_ERR,  "EventHandler index is not found");
        return;
    }

    const unsigned int index = subTypeIndex;
    const unsigned int assertion = event->assertion;
    const unsigned int chipId = event->switchChipid;
    const unsigned int portId = event->switchPortid;

    const unsigned int alarmType = GetAlarmType(event);

    g_user_event_table[index].subType = event->subType;
    FaultEventNodeTable *currFault = &g_user_event_table[index];

    if (chipId >= NUM_CHIP || (portId >= NUM_PORTS && portId != INVALID_PORTID)) {
        LQ_DCMI_TYPE_LOG(LOG_ERR,  "chipId or portId is invalid");
        return;
    }
    ChipFaultInfo *currChipFault = &currFault->chipFaultInfo[chipId];

    if (portId != INVALID_PORTID) {
        currPortFault = &currChipFault->portFaultInfo[portId];
    }
    if (assertion == FAULT) {
        HandleFault(event, currFault, currChipFault, currPortFault, alarmType);
    } else if (assertion == RECOVERY) {
        HandleRecovery(event, currFault, currChipFault, currPortFault, alarmType);
    }
}


int mapSharedMemory()
{
    // 1. 打开设备文件
    LQ_DCMI_TYPE_LOG(LOG_INFO,  "mapSharedMemory");
    g_lqdcmi_dev_fd = open(DEV_NAME, O_RDWR);
    if (g_lqdcmi_dev_fd < 0) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "open %s err! errno = %d\n", DEV_NAME, g_lqdcmi_dev_fd);
        return LQ_DCMI_ERR_CODE_DEVICE_OPEN_FAIL;
    }

    // 2. 调用 mmap 映射共享内存
    char *shared_mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, g_lqdcmi_dev_fd, 0);
    if (shared_mem == MAP_FAILED) {
        if (errno == ENOSPC) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "mmap failed: No space left on device (ENOSPC)");
            close(g_lqdcmi_dev_fd);
            g_lqdcmi_dev_fd = -1;
            return LQ_DCMI_ERR_CODE_MAX_PROCESS_EXCEEDED;
        }
        LQ_DCMI_TYPE_LOG(LOG_ERR, "mmap failed with error: %s", strerror(errno));
        close(g_lqdcmi_dev_fd);
        g_lqdcmi_dev_fd = -1;
        return LQ_DCMI_ERR_CODE_MMAP_FAIL;
    }
    g_shared_memory_address = shared_mem;

    LQ_DCMI_TYPE_LOG(LOG_INFO,  "mapSharedMemory:pro id:%d",
        *(int *)(g_shared_memory_address + PROC_ID_POINTER_OFFSET * sizeof(int)));

    return LQ_DCMI_OK;
}

void read_shared_memory()
{
    unsigned int ret = 0;
    int *ptr_head = (int *)(g_shared_memory_address);
    int head = *ptr_head;  // 获取头部
    int *ptr_tail = (int *)(g_shared_memory_address + sizeof(int));
    int tail = *ptr_tail; // 读取尾部
    SramFaultEventData sd = {0};

    if (head == tail) {
        return;  // 队列为空
    }

    // 检查队列是否满

    while (head != tail) {
        LQ_DCMI_TYPE_LOG(LOG_INFO,  "read_shared_memory:head=%d,tail=%d", head, tail);
        ret = memset_s(&sd, sizeof(SramFaultEventData), 0, sizeof(SramFaultEventData));
        if (ret != 0) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "read_shared_memory memset_s fail");
            return;
        }

        // 按照4字节读
        ret = memcpy_s(&sd, sizeof(SramFaultEventData),
            g_shared_memory_address + sizeof(SramFaultEventData) + head * sizeof(SramFaultEventData),
            sizeof(SramFaultEventData));
        if (ret != 0) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "read_shared_memory memcpy_s fail");
            return;
        }

        LqDcmiEvent *event = (LqDcmiEvent *) (sd.data);
        LQ_DCMI_TYPE_LOG(LOG_INFO,  "event_type: %x, sub_type: %u, peerport_device: %u, peerport_id: %u, "
            "switch_chipid: %u, switch_portid: %u, severity: %u, assertion: %u\n",
            event->eventType, event->subType, event->peerportDevice, event->peerportId,
            event->switchChipid, event->switchPortid, event->severity, event->assertion);

        EventHandler(event);

        head = (head + 1) % g_faultlist_size;

        int *ptr = (int *)g_shared_memory_address;
        *ptr = head; // 更新头节点位置
    }
    return;  // 成功返回 0
}

int initFaultList()
{
    int ret = TcIoctlGetAllFault(g_user_event_table);
    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "TcIoctlFault failed, ret=%d \n", ret);
        return ret;
    }

    return LQ_DCMI_OK;
}

void *update_thread_func()
{
    while (g_globalThreadFlag == THREAD_RUNNING) {
        read_shared_memory();
        (void) usleep(THREAD_SLEEP);
    }

    LQ_DCMI_TYPE_LOG(LOG_ERR,  "Something went wrong with update_thread_func!");
    return NULL;
}

int CreateUpdatePthread()
{
    UpdateThread *updateThread = &g_globalUpdateThread;
    struct sched_param threadSchedParam;
    pthread_attr_t attr;

    if (pthread_mutex_lock(&updateThread->threadLock) != 0) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "failed to lock mutex in lq_dcmi_init!");
        return LQ_DCMI_ERR_CODE_LOCK_CREATE_FAIL;
    }

    g_globalThreadFlag = THREAD_RUNNING;
    (void) pthread_attr_init(&attr);
    (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    (void) pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    (void) pthread_attr_setstacksize(&attr, (g_faultlist_size + NUM_CHIP) * THREAD_STACK_SIZE);
    (void) pthread_attr_setschedpolicy(&attr, SCHED_RR);
    threadSchedParam.sched_priority =
        sched_get_priority_min(SCHED_RR) + THREAD_PRIORITY;
    (void) pthread_attr_setschedparam(&attr, &threadSchedParam);

    if (pthread_create(&updateThread->threadId, &attr, update_thread_func, NULL) != 0) {
        pthread_attr_destroy(&attr);
        pthread_mutex_unlock(&updateThread->threadLock);
        LQ_DCMI_TYPE_LOG(LOG_ERR, "failed to create updateThread.");
        return LQ_DCMI_ERR_CODE_UPDATETHREAD_CREATE_FAIL;
    }

    (void) pthread_attr_destroy(&attr);
    (void) pthread_mutex_unlock(&updateThread->threadLock);

    g_initFlag = true;
    return LQ_DCMI_OK;
}

bool isVersionCompatible(unsigned int version)
{
    unsigned int headerMajor = HEADER_VERSION & 0xFF000000;
    return (version & 0xFF000000) == headerMajor;
}

int lq_dcmi_init()
{
    if (g_initFlag) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "lq_dcmi_init is duplicated called!");
        return LQ_DCMI_ERR_CODE_INTERFACE_INIT_DUPLICATE;
    }
    unsigned int ret;

    unsigned int version;
    ret = TcGetVersion(&version);
    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "TcGetVersion fail!");
        return ret;
    }

    if (!isVersionCompatible(version)) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "Incompatible version!");
        return LQ_DCMI_ERR_CODE_VERSION_IMCOMPATIBLE;
    }

    ret = initFaultList();
    if (ret != LQ_DCMI_OK) {
        return ret;
    }

    ret = mapSharedMemory();
    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "mapSharedMemory error!");
        return ret;
    }

    ret = CreateUpdatePthread();
    if (ret != LQ_DCMI_OK) {
        return ret;
    }

    LQ_DCMI_TYPE_LOG(LOG_INFO,  "lq_dcmi_init success!");

    return LQ_DCMI_OK;
}

int lq_dcmi_subscribe_fault_event(const LqDcmiEventFilter filter,
                                  const LqDcmiFaultEventCallback handler)
{
    unsigned int ret = LQ_DCMI_OK;
    if (!g_initFlag) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "lq_dcmi_init should be called before subscribe!");
        return LQ_DCMI_ERR_CODE_INTERFACE_INIT_FAIL;
    }

    if (filter.filterFlag & EVENT_TYPE_ID) {
        if (find_index_by_event_type(mapping, sizeof(mapping)/sizeof(mapping[0]), filter.eventTypeId) == INDEX_NOT_FOUND) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "event type id is not found");
            return LQ_DCMI_ERR_CODE_INVALID_FILTER;
        }
    }

    if (filter.filterFlag & EVENT_ID) {
        if (find_index_by_sub_type(mapping, sizeof(mapping)/sizeof(mapping[0]), filter.eventId) == INDEX_NOT_FOUND) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "sub type id is not found");
            return LQ_DCMI_ERR_CODE_INVALID_FILTER;
        }
    }

    if (filter.filterFlag & SEVERITY) {
        if (filter.severity > MAX_FAULT_LEVEL) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "Invalid severity level: must be between 0 and 3.");
            return LQ_DCMI_ERR_CODE_INVALID_FILTER;
        }
    }

    if (filter.filterFlag & CHIP_ID) {
        if (filter.chipId > NUM_CHIP - 1) {
            LQ_DCMI_TYPE_LOG(LOG_ERR, "Invalid  filer chip id: must be between 0 and 6.");
            return LQ_DCMI_ERR_CODE_INVALID_FILTER;
        }
    }

    if (handler == NULL) {
        ret = SubscribeAclListRemove(&g_subscribeAclList, filter);
        if (ret != LQ_DCMI_OK) {
            return ret;
        }
        return LQ_DCMI_OK;
    }

    (void)SubscribeAclListRemove(&g_subscribeAclList, filter);

    ret = SubscribeAclListAdd(&g_subscribeAclList, filter, handler);
    if (ret != LQ_DCMI_OK) {
        return ret;
    }

    g_subscribeFlag = true;

    return LQ_DCMI_OK;
}

STATIC int GetAllFault(unsigned int eventListLen, LqDcmiEvent **list, unsigned int *listLen)
{
    unsigned int len = 0;
    unsigned int ret = 0;
    unsigned int cap = g_faultlist_size;
    LqDcmiEvent *faultList = malloc(cap * sizeof(LqDcmiEvent));
    if (faultList == NULL) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "malloc memory for fault list failed");
        return LQ_DCMI_ERR_CODE_MEM_CREATE_FAIL;
    }

    for (unsigned int i = 0; i < CAPACITY; ++i) {
        const FaultEventNodeTable curr = g_user_event_table[i];
        if (curr.alarmFlag == 0) { continue; }
        for (unsigned int j = 0; j < NUM_CHIP; ++j) {
            const ChipFaultInfo chipFault = curr.chipFaultInfo[j];
            if (chipFault.alarmFlag == 0) { continue; }
            if (!(IsPortNodeInfoZero(chipFault.chipNodeInfo))) {
                if (len + 1 > eventListLen) {
                    ret = memcpy_s(*list, len * sizeof(LqDcmiEvent), faultList,
                        len * sizeof(LqDcmiEvent));
                    if (ret != 0) {
                        free(faultList);
                        return LQ_DCMI_ERR_CODE_MEM_COPY_FAIL;
                    }
                    free(faultList);
                    *listLen = len;
                    return LQ_DCMI_OK;
                }
                faultList[len++] = chipFault.chipNodeInfo;
            } else {
                for (unsigned int k = 0; k < NUM_PORTS; ++k) {
                    const PortFaultInfo portFault = chipFault.portFaultInfo[k];
                    if (portFault.alarmFlag == 1 && !(IsPortNodeInfoZero(portFault.portNodeInfo))) {
                        if (len + 1 > eventListLen) {
                            ret = memcpy_s(*list, len * sizeof(LqDcmiEvent), faultList,
                                len * sizeof(LqDcmiEvent));
                            if (ret != 0) {
                                free(faultList);
                                return LQ_DCMI_ERR_CODE_MEM_COPY_FAIL;
                            }
                            free(faultList);
                            *listLen = len;
                            return LQ_DCMI_OK;
                        }
                        faultList[len++] = portFault.portNodeInfo;
                    }
                }
            }
        }
    }

    if (len == 0) {
        free(faultList);
        *listLen = len;
        return LQ_DCMI_OK;
    }

    ret = memcpy_s(*list, len * sizeof(LqDcmiEvent), faultList, len * sizeof(LqDcmiEvent));
    if (ret != 0) {
        free(faultList);
        return LQ_DCMI_ERR_CODE_MEM_COPY_FAIL;
    }
    free(faultList);

    *listLen = len;

    return LQ_DCMI_OK;
}

int lq_dcmi_get_fault_info(unsigned int listLen, unsigned int *eventListLen,
                           LqDcmiEvent *eventList)
{
    if (!g_initFlag) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "lq_dcmi_init should be called before get_fault_info!");
        return LQ_DCMI_ERR_CODE_INTERFACE_INIT_FAIL;
    }

    if (eventList == NULL) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "eventList is NULL!");
        return LQ_DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (eventListLen == NULL) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "eventListLen is NULL!");
        return LQ_DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (listLen <= 0 || listLen > MAX_FAULT_NUMS) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "lisLen is invalid!");
        return LQ_DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    const int ret = GetAllFault(listLen, &eventList, eventListLen);
    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "get fault from table failed");
        return ret;
    }

    return LQ_DCMI_OK;
}

int lq_dcmi_get_version(unsigned int* lq_version, unsigned int* lqdcmi_version)
{
    if (lq_version == NULL || lqdcmi_version == NULL) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "lq_version or lqdcmi_version is NULL!");
        return LQ_DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    unsigned int ret;
    unsigned int version;

    ret = TcGetVersion(&version);
    if (ret != LQ_DCMI_OK) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "lq_dcmi_get_version fail!");
        return ret;
    }

    *lq_version = version;
    *lqdcmi_version = HEADER_VERSION;
    return LQ_DCMI_OK;
}

unsigned int count_ports_faults_in_chip(const ChipFaultInfo* chipFaultInfo)
{
    unsigned int port_faults = 0;
    for (unsigned int k = 0; k < NUM_PORTS; ++k) {
        const PortFaultInfo portFault = chipFaultInfo->portFaultInfo[k];
        if (portFault.alarmFlag == 1 && !IsPortNodeInfoZero(portFault.portNodeInfo)) {
            port_faults++;
        }
    }
    return port_faults;
}

int lq_dcmi_get_fault_nums(unsigned int* fault_nums)
{
    if (!g_initFlag) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "lq_dcmi_init should be called before lq_dcmi_get_fault_nums!");
        return LQ_DCMI_ERR_CODE_INTERFACE_INIT_FAIL;
    }

    if (fault_nums == NULL) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "fault_nums is NULL!");
        return LQ_DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    unsigned int len = 0;

    for (unsigned int i = 0; i < CAPACITY; ++i) {
        const FaultEventNodeTable curr = g_user_event_table[i];
        if (curr.alarmFlag == 0) { continue; }
        for (unsigned int j = 0; j < NUM_CHIP; ++j) {
            const ChipFaultInfo chipFault = curr.chipFaultInfo[j];
            if (chipFault.alarmFlag == 0) { continue;}
            if (!(IsPortNodeInfoZero(chipFault.chipNodeInfo))) {
                len++;
                continue;
            }
            len += count_ports_faults_in_chip(&chipFault);
        }
    }
    *fault_nums = len;
    return LQ_DCMI_OK;
}

__attribute__((destructor)) void Cleanup()
{
    LQ_DCMI_TYPE_LOG(LOG_INFO,  "Cleanup!");

    SubscribeAclListRemoveAll(&g_subscribeAclList);
    if (pthread_mutex_lock(&g_globalUpdateThread.threadLock) != 0) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "failed to lock mutex in cleanup_thread!");
        return;
    }

    while (g_globalThreadFlag == THREAD_RUNNING) {
        g_globalThreadFlag = THREAD_STOPPED;
    }

    if (g_globalUpdateThread.threadId != 0) {
        pthread_join(g_globalUpdateThread.threadId, NULL);
        g_globalUpdateThread.threadId = 0;
    }

    pthread_mutex_unlock(&g_globalUpdateThread.threadLock);

    if (munmap(g_shared_memory_address, SHM_SIZE) == -1) {
        LQ_DCMI_TYPE_LOG(LOG_ERR, "failed to munmap!");
    }

    g_shared_memory_address = NULL;
    close(g_lqdcmi_dev_fd);
    g_lqdcmi_dev_fd = -1;
}
