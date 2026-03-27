# device manager

## 描述
本样例展示了如何查询设备数量信息并且开启设备间P2P功能

## 支持的产品型号
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
- 昇腾950PR处理器/昇腾950DT处理器

## 编译运行
环境安装详情以及运行详情请见example目录下的[README](../../README.md)。

## CANN Driver API
在该example中，涉及的关键功能点及其关键接口，如下所示：
- 查询设备数量和ID信息
    - 调用drvGetDevNum接口查询设备数量。
    - 调用drvGetDevIDs接口查询设备ID列表。
    - 调用drvDeviceGetPhyIdByIndex进行逻辑ID和物理ID转换。
- P2P使能管理
    - 调用halDeviceCanAccessPeer接口查询Device之间是否支持数据交互。
    - 调用halDeviceEnableP2P接口使能当前Device与指定Device之间的数据交互。
    - 调用halDeviceDisableP2P接口关闭当前Device与指定Device之间的数据交互功能。

## 已知issue

   暂无

