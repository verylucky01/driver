/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef IOCTL_COMMON_DEF_H
#define IOCTL_COMMON_DEF_H

#define IOCTL_GET_NODE_INFO     0x0101
#define IOCTL_GET_HEAD_INFO     0x0102

#define HEADER_VERSION 0x2001001
#define INVALID_PORTID 0xFFFF
#define INDEX_NOT_FOUND ((unsigned int)-1)
#define SHM_SIZE  (256 + 7 * 1024 * 256)
#define MAX_PROCESS_NUM 15
#define PROC_ID_POINTER_OFFSET 2
#define THREAD_SLEEP (300 * 1000)

#define ODA_REPORT_FAULT_FATAL 155907
#define ODA_REPORT_FAULT_PCIE 155649
#define ODA_REPORT_FAULT_MEM_MULTI 155904
#define ODA_REPORT_FAULT_BLOCK_C 132134
#define ODA_REPORT_FAULT_M7 155911
#define ODA_REPORT_FAULT_CONFIG 155908
#define ODA_REPORT_FAULT_BY_DEVICE 155909
#define ODA_REPORT_PORT_FAULT_FAIL 155912
#define ODA_REPORT_PORT_FAULT_UNSTABLE 155913
#define ODA_REPORT_PORT_FAULT_INVALID_PKG 155914
#define ODA_REPORT_PORT_FAULT_DOWN 0xFFFFFFFF
#define ODA_REPORT_PORT_FAULT_LANE_REDUCE_HALF 132332
#define ODA_REPORT_PORT_FAULT_LANE_REDUCE_QUARTER 132333

#define LQ_DCMI_OK 0
#define LQ_DCMI_ERR_CODE_INVALID_PARAMETER (-8001)
#define LQ_DCMI_ERR_CODE_LOG_CREATE_FAIL (-8002)
#define LQ_DCMI_ERR_CODE_MEM_CREATE_FAIL (-8003)
#define LQ_DCMI_ERR_CODE_DEVICE_OPEN_FAIL (-8004)
#define LQ_DCMI_ERR_CODE_IOCTL_FAIL (-8005)
#define LQ_DCMI_ERR_CODE_LOCK_CREATE_FAIL (-8006)
#define LQ_DCMI_ERR_CODE_UPDATETHREAD_CREATE_FAIL (-8007)
#define LQ_DCMI_ERR_CODE_VERSION_IMCOMPATIBLE (-8008)
#define LQ_DCMI_ERR_CODE_INTERFACE_INIT_FAIL (-8009)
#define LQ_DCMI_ERR_CODE_MAX_PROCESS_EXCEEDED (-8010)
#define LQ_DCMI_ERR_CODE_MMAP_FAIL (-8011)
#define LQ_DCMI_ERR_CODE_MEM_SET_FAIL (-8012)
#define LQ_DCMI_ERR_CODE_MEM_COPY_FAIL (-8013)
#define LQ_DCMI_ERR_CODE_MAX_SUBCRIBLE_NUMS_EXCEEDED (-8014)
#define LQ_DCMI_ERR_CODE_INVALID_FILTER (-8015)
#define LQ_DCMI_ERR_CODE_UNSUBSCRIBE_FAIL (-8016)
#define LQ_DCMI_ERR_CODE_INTERFACE_INIT_DUPLICATE (-8017)

#define NUM_CHIP 7
#define NUM_PORTS 48
#define CAPACITY 30

typedef struct {
    unsigned int version;
    unsigned int length;
    unsigned int nodeSize;
    unsigned int nodeNum;
    unsigned int nodeHead;
    unsigned int nodeTail;
    unsigned long startTimeMs;
    unsigned int disable;
    unsigned int overflowflag;
    unsigned char resverd[216];
} SramDescCtlHeader;

typedef struct {
    unsigned int msgId; // 事件ID，0表示上报故障，1代表xx
    unsigned int devId; // 故障时对应的CPU/NPU、XPU ID
    unsigned char res[8];
} SramFaultEventHead;

/* 256 Byte */
typedef struct {
    SramFaultEventHead head; // 事件头信息
    unsigned char data[240];
} SramFaultEventData;

typedef struct {
    unsigned int eventType;            // 消息类型
    unsigned int subType;              // 故障消息子类型
    unsigned short peerportDevice;     // 对接设备类型
    unsigned short peerportId;         // 对接设备ID号
    unsigned short switchChipid;       // ID号 从0开始编号
    unsigned short switchPortid;       // 端口号 从0开始编号
    unsigned char severity;            // 故障等级：0: 提示 1：次要(一般) 2:(重要) 3：紧急
    unsigned char assertion;           // 事件类型 0：故障恢复 1：故障产生 2：通知类事件
    char res[6];                       // 6: 8字节对齐
    unsigned int eventSerialNum;       // 事件产生的序列号 ，可选填写
    unsigned int notifySerialNum;      // 事件上报的序列号 ，可选填写
    unsigned long alarmRaisedTime;     // 故障/事件产生时间
    unsigned char additionalParam[40]; // 故障附带的参数，host用于对特殊故障进行解析进行处理.需要针对特殊故障，做特殊处理
    char additionalInfo[32];           // 做一个字符串，用于信息打印，运维用
} LqDcmiEvent;

typedef enum {
    RECOVERY = 0,
    FAULT = 1
} Assertion;

typedef enum {
    CHIP_ALARM = 1,
    PORT_ALARM = 2
} AlarmType;

typedef struct {
    unsigned int portId;
    unsigned int alarmFlag;
    LqDcmiEvent portNodeInfo;
} PortFaultInfo;

typedef struct {
    unsigned int chipId;
    unsigned int alarmFlag;
    LqDcmiEvent chipNodeInfo;
    PortFaultInfo portFaultInfo[NUM_PORTS];
} ChipFaultInfo;

typedef struct {
    unsigned int subType;
    unsigned int alarmFlag;
    unsigned int chipId;
    ChipFaultInfo chipFaultInfo[NUM_CHIP];
} FaultEventNodeTable;

typedef struct {
    int cmd; // 命令控制字
    int len;
    void *in_addr; // 用户空间输入参数地址
    void *out_addr; // 用户空间输出参数地址
    size_t out_size; // 用户空间输出参数长度
} IOCTL_CMD_S;

/* ioct命令结构体 */
typedef struct IOCTL_CMD_INFO_tag {
    int cmd;
    int (*cmd_fun_pre)(void);
    int (*cmd_fun)(IOCTL_CMD_S *ioctl_cmd);
} IOCTL_CMD_INFO_S;

typedef struct {
    unsigned int event_type;
    unsigned int sub_type;
    unsigned int index;
} EventMapping;

#define EVENT_MAPPING_INITIALIZER \
{ \
    {0x00f1fef5, ODA_REPORT_PORT_FAULT_INVALID_PKG, 0}, \
    {0x00f1fef5, ODA_REPORT_PORT_FAULT_UNSTABLE, 1}, \
    {0x00f1fef5, ODA_REPORT_PORT_FAULT_FAIL, 2}, \
    {0x00f103b6, ODA_REPORT_FAULT_BY_DEVICE, 3}, \
    {0x00f103b6, ODA_REPORT_FAULT_CONFIG, 4}, \
    {0x00f1ff06, ODA_REPORT_FAULT_M7, 5}, \
    {0x00f1ff06, ODA_REPORT_FAULT_BLOCK_C, 6}, \
    {0x00f103b0, ODA_REPORT_FAULT_MEM_MULTI, 7}, \
    {0x00f103b0, ODA_REPORT_FAULT_PCIE, 8}, \
    {0x00f103b0, ODA_REPORT_FAULT_FATAL, 9}, \
    {0x08520003, ODA_REPORT_PORT_FAULT_DOWN, 10}, \
    {0x00f10509, ODA_REPORT_PORT_FAULT_LANE_REDUCE_HALF, 11}, \
    {0x00f10509, ODA_REPORT_PORT_FAULT_LANE_REDUCE_QUARTER, 12} \
}

static inline unsigned int find_index_by_event_type(EventMapping *mapping, unsigned int map_cnt, unsigned int event_type)
{
    size_t i = 0; 
    for (i = 0; i < map_cnt; i++) {
        if (mapping[i].event_type == event_type) {
            return mapping[i].index;
        }
    }
 
    return INDEX_NOT_FOUND;
}

static inline unsigned int find_index_by_sub_type(EventMapping *mapping, unsigned int map_cnt,unsigned int sub_type)
{
    size_t i = 0;
    for (i = 0; i < map_cnt; i++) {
        if (mapping[i].sub_type == sub_type) {
            return mapping[i].index;
        }
    }
 
    return INDEX_NOT_FOUND;
}

static inline bool IsPortNodeInfoZero(LqDcmiEvent event)
{
    LqDcmiEvent zeroEvent = {0};
    return memcmp(&event, &zeroEvent, sizeof(event)) == 0;
}

static inline int GetAlarmType(LqDcmiEvent *event)
{
    if (event->switchPortid == INVALID_PORTID) {
        return CHIP_ALARM;
    } else {
        return PORT_ALARM;
    }
}

#endif