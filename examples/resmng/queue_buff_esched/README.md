# queue_buff_esched

## 描述
本样例展示了从buff/queue/esched初始化、创建queue、申请buff、创建esched组、队列入队、等待事件并出队释放buff主流程，是一个基础的使用queue/buff/esched的样例。

## 支持的产品型号
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas 800I A2 推理产品/A200I A2 Box 异构组件
- Atlas 200I/500 A2 推理产品
- Atlas 推理系列产品
- Atlas 训练系列产品

## 编译运行
环境安装详情以及运行详情请见examples目录下的[README](../../README.md)。

## QUEUE/BUFF/ESCHED API

在该sample中，涉及的关键功能点及其关键接口，如下所示：
- 初始化
    - 调用halGrpCreate、halGrpAddProc、halGrpAttach和halBuffInit接口完成buff组创建和初始化。
    - 调用halQueueSet、halQueueInit接口完成queue初始化。
    - 调用halEschedAttachDevice接口完成esched初始化。
- Queue创建
    - 调用halQueueCreate接口创建queue。
- Esched组创建
    - 调用halEschedCreateGrp接口创建esched组。
- 订阅出队事件并等待出队事件
    - 调用halQueueSubscribe接口订阅队列。
    - 调用halEschedSubscribeEvent接口订阅出队事件。
    - 调用halEschedWaitEvent接口等待出队事件。
- Buff申请和入队
    - 调用halMbufAlloc接口申请buff。
    - 调用halQueueEnQueue接口入队buff。
- 等到事件后出队并释放buff
    - halEschedWaitEvent接口返回值为0时，调用halQueueDeQueue接口出队buff。
    - 调用halMbufFree接口释放buff。
- queue信息查询
    - 调用halQueueGetStatus接口查询队列状态。
    - 调用halQueueQueryInfo接口查询队列信息。
- esched去初始化及queue销毁
    - 调用halEschedDettachDevice接口完成esched去初始化。
    - 调用halQueueDestroy接口完成队列销毁。

## 已知issue

  暂无

