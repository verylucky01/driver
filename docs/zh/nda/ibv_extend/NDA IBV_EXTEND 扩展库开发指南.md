# 目录

- [目录](#目录)
- [简介](#简介)
  - [组件介绍](#组件介绍)
  - [应用场景](#应用场景)
- [安装](#安装)
  - [方式一：通过源码编译安装](#方式一通过源码编译安装)
  - [方式二：通过HDK包安装](#方式二通过hdk包安装)
- [使用](#使用)
- [API接口参考](#api接口参考)
  - [通用接口](#通用接口)
    - [ibv\_extend\_get\_version](#ibv_extend_get_version)
    - [ibv\_extend\_check\_version](#ibv_extend_check_version)
  - [北向接口（应用层调用）](#北向接口应用层调用)
    - [ibv\_open\_extend](#ibv_open_extend)
    - [ibv\_close\_extend](#ibv_close_extend)
    - [ibv\_query\_device\_extend](#ibv_query_device_extend)
    - [ibv\_create\_qp\_extend](#ibv_create_qp_extend)
    - [ibv\_create\_cq\_extend](#ibv_create_cq_extend)
    - [ibv\_create\_srq\_extend](#ibv_create_srq_extend)
    - [ibv\_destroy\_qp\_extend](#ibv_destroy_qp_extend)
    - [ibv\_destroy\_cq\_extend](#ibv_destroy_cq_extend)
    - [ibv\_destroy\_srq\_extend](#ibv_destroy_srq_extend)
  - [南向接口（驱动调用）](#南向接口驱动调用)
    - [verbs\_register\_driver\_extend](#verbs_register_driver_extend)
- [关键结构体说明](#关键结构体说明)
  - [枚举类型定义](#枚举类型定义)
    - [queue\_buf\_dma\_mode](#queue_buf_dma_mode)
    - [doorbell\_map\_mode](#doorbell_map_mode)
    - [memcpy\_direction](#memcpy_direction)
    - [ibv\_qp\_init\_cap](#ibv_qp_init_cap)
    - [ibv\_extend\_device\_cap](#ibv_extend_device_cap)
  - [核心数据结构](#核心数据结构)
    - [doorbell\_map\_desc](#doorbell_map_desc)
    - [ibv\_extend\_ops](#ibv_extend_ops)
    - [queue\_buf](#queue_buf)
    - [queue\_info](#queue_info)
    - [ibv\_qp\_extend](#ibv_qp_extend)
    - [ibv\_cq\_extend](#ibv_cq_extend)
    - [ibv\_srq\_extend](#ibv_srq_extend)
    - [ibv\_qp\_init\_attr\_extend](#ibv_qp_init_attr_extend)
    - [ibv\_cq\_init\_attr\_extend](#ibv_cq_init_attr_extend)
    - [ibv\_srq\_init\_attr\_extend](#ibv_srq_init_attr_extend)
    - [ibv\_device\_attr\_extend](#ibv_device_attr_extend)
    - [ibv\_context\_extend](#ibv_context_extend)
    - [ibv\_context\_extend\_ops](#ibv_context_extend_ops)
    - [verbs\_device\_extend\_ops](#verbs_device_extend_ops)
- [FAQ](#faq)
  - [调用ibv\_open\_extend接口初始化扩展库报错](#调用ibv_open_extend接口初始化扩展库报错)

# 简介

## 组件介绍

ibverbs\_extend（下文简称ibv\_extend）组件是标准RDMA verbs接口的扩展，专门用于支持NDA（NPUDirect Async）场景下的直驱RDMA通信。

ibv\_extend组件通过统一接口实现南北向解耦，如下图所示：

- 北向屏蔽底层网卡硬件差异，为HCOMM等通信传输层提供透明访问；
- 南向对接1825、云脉等网卡扩展驱动，使其无需感知上层计算与内存资源差异。

![ibv_extend逻辑框图](figures/ibv_extend逻辑框图.png)

ibv\_extend组件提供了以下核心功能：

- 扩展驱动程序的动态加载和管理
- 扩展QP/CQ/SRQ的创建和销毁
- 支持南北向通过回调函数机制自定义硬件操作

## 应用场景

本组件主要应用于以下场景：

- NPU与RDMA网卡之间的直接数据传输。
- 绕过Host CPU、NPU与RDMA网卡的控制面直驱。

![应用场景图](figures/应用场景.png)

# 安装

## 方式一：通过源码编译安装

1. 获取源码，并进入到组件源码目录。

    方式1：使用git clone源码。

    ```bash
    git clone https://gitcode.com/cann/driver.git
    cd driver/src/custom/nda/ibv_extend/
    ```

    方式2：网页下载源码。

    浏览器进入[driver仓](https://gitcode.com/cann/driver)，点击“下载zip”按钮获取最新源码，上传到服务器后执行如下命令。

    ```bash
    unzip driver-master.zip
    cd driver-master/src/custom/nda/ibv_extend/
    ```

2. 编译依赖。

    For openEuler/CentOS/RHEL：

    ```bash
    yum install gcc gcc-c++ make cmake libnl3-devel libudev-devel pkgconfig python3-devel
    ```

    For Ubuntu/Debian：

    ```bash
    apt install gcc g++ make cmake libnl-3-dev libnl-route-3-dev libudev-dev pkg-config python3-dev
    ```

3. 编译安装。

    ```bash
    bash ./build.sh
    ```

    **表 1**  编译参数说明

    <table>
    <tr>
        <th valign="top" width="18%"><p>参数</p></th>
        <th valign="top" width="18%"><p>长参数</p></th>
        <th valign="top" width="33%"><p>说明</p></th>
        <th valign="top" width="31%"><p>使用示例</p></th>
    </tr>
    <tr>
        <td><p>-t=TYPE</p></td>
        <td><p>--type=TYPE</p></td>
        <td><p>编译类型：</p>
            <ul>
            <li>release ：默认值，此时自动下载 rdma-core。</li>
            <li>debug：调试类型。</li>
            </ul>
        </td>
        <td><p>debug 模式编译。</p><code>bash ./build.sh -t=debug</code></td>
    </tr>
    <tr>
        <td><p>-b=PATH</p></td>
        <td><p>--libibverbs-build-dir=PATH</p></td>
        <td><p>指定预编译的 rdma-core 构建目录。与 <strong>-s</strong> 参数互斥，不能同时使用。</p></td>
        <td><p>使用预编译的 rdma-core 构建库。</p><code>bash ./build.sh -b=/path/to/rdma-core/build</code></td>
    </tr>
    <tr>
        <td><p>-s=PATH</p>    </td>
        <td><p>--libibverbs-source-dir=PATH</p>    </td>
        <td><p>指定 rdma-core 源码目录。与 <strong>-b</strong> 参数互斥，不能同时使用。</p></td>
        <td><p>使用自定义 rdma-core 源码。</p><code>bash ./build.sh -s=/path/to/rdma-core</code></td>
    </tr>
    <tr>
        <td><p>-x=PATH</p></td>
        <td><p>--libboundscheck-build-dir=PATH</p></td>
        <td><p>指定预编译的 libboundscheck 构建目录。与 <strong>-e</strong> 参数互斥，不能同时使用。</p></td>
        <td><p>使用预编译的 libboundscheck 构建库。</p><code>bash ./build.sh -x=/path/to/libboundscheck</code></td>
    </tr>
    <tr>
        <td><p>-e=PATH</p>    </td>
        <td><p>--libboundscheck-source-dir=PATH</p>    </td>
        <td><p>指定 libboundscheck 源码目录。与 <strong>-x</strong> 参数互斥，不能同时使用。</p></td>
        <td><p>使用自定义 libboundscheck 源码。</p><code>bash ./build.sh -e=/path/to/libboundscheck</code></td>
    </tr>
    <tr>
        <td><p>-h</p></td>
        <td><p>--help</p></td>
        <td><p>显示帮助信息。</p></td>
        <td><p>查看帮助信息。</p><code>bash ./build.sh -h</code></td>
    </tr>
    </table>

4. 编译完成后，检查输出文件。

    ```text
    output/
    ├── lib/
    │    └── libibv_extend.so    # 共享库
    └── include/
    │    └── ibv_extend.h        # 头文件
    ```

5. 运行时需动态指定环境变量。

    ```bash
    export LD_LIBRARY_PATH=/path/to/ibv_extend/output/lib:$LD_LIBRARY_PATH
    ```

    > **说明：** 
    >
    > - /path/to/为变量，请替换为实际路径。
    > - 用户也可自行拷贝output/lib下的动态库或output/include下的头文件到其他自定义路径或系统路径，用于后续使用。

6. 编译安全函数库。

    进入[libboundscheck发布页](https://gitcode.com/cann-src-third-party/libboundscheck/releases)获取最新源码压缩包，例如下载[libboundscheck-v1.1.16.tar.gz](https://gitcode.com/cann-src-third-party/libboundscheck/releases/download/v1.1.16/libboundscheck-v1.1.16.tar.gz)。

    编译libc_sec.so动态库。

    ```bash
    # 解压到当前目录
    tar zxvf libboundscheck-v1.1.16.tar.gz
    # 进入当前目录
    cd libboundscheck-v1.1.16
    # 编译
    make -j
    # 编译成功后会在lib目录下生成动态链接库，需要重命名为libc_sec.so。
    cp lib/libboundscheck.so lib/libc_sec.so
    ```

    使用时需要依赖libc_sec.so动态库。

    ```bash
    export LD_LIBRARY_PATH=/path/to/libboundscheck-v1.1.16/lib:$LD_LIBRARY_PATH
    ```

## 方式二：通过HDK包安装

1. 安装支持NDA特性的HDK软件包。
    1. 获取[支持NDA特性的HDK软件包](https://support.huawei.com/enterprise/zh/ascend-computing/ascend-hdk-pid-252764743/software/267204993?idAbsPath=fixnode01|23710424|251366513|254884019|261408772|252764743)。
    2. 参考对应硬件产品的《[NPU驱动和固件安装指南](https://support.huawei.com/enterprise/zh/ascend-computing/ascend-hdk-pid-252764743?category=installation-upgrade)》完成HDK驱动包的安装。
   
2. 安装后，检查输出文件。
    1. libibv\_extend.so 在 /usr/local/Ascend/driver/lib64/driver/ 目录下。
    2. ibv\_extend.h 在 /usr/local/Ascend/driver/include/ 目录下。

3. 运行时需加载环境变量。
    - 如果已安装cann-toolkit等开发工具包，可执行如下命令加载环境变量（如下命令以root用户安装为例，请以实际CANN安装路径为准）。

        ```bash
        source /usr/local/Ascend/cann/set_env.sh
        ```

    - 如果未安装cann-toolkit等开发工具包，可执行如下命令加载环境变量。

        ```bash
        export LD_LIBRARY_PATH=/usr/local/Ascend/driver/lib64/driver:/usr/local/Ascend/driver/lib64/common:$LD_LIBRARY_PATH
        ```

# 使用

ibv\_extend组件允许上层通信库通过**配置环境变量**或**配置文件**两种方式动态加载扩展驱动。

运行前可通过配置环境变量`export IBV_EXTEND_SHOW_WARNINGS=1`，让ibv\_extend组件打印Warning级别日志到控制台，Warning级别日志默认不打印。

运行前需要确保已正确安装libibverbs.so、libibv\_extend.so，具体操作请参考下表。

**表 1**  运行前检查

<table>
    <tr>
        <th>系统类型</th>
        <th>检查/安装libibverbs.so</th>
        <th>检查/安装libibv_extend.so</th>  
    </tr >
    <tr >
        <td>openEuler/CentOS/RHEL</td>
        <td>
            <ol>
                <li>检查libibverbs.so是否已安装。<br><code>ldconfig -p | grep libibverbs</code><br>如果有回显，则表示libibverbs.so已安装，结束。<br>如果无回显，则继续。</li>               
                <li>安装libibverbs。<br><code>sudo yum install rdma-core</code></li>
            </ol>
        </td>
        <td rowspan="2">
            <ol>
                <li>检查libibv_extend.so是否已安装。<br><code>ll /path/to/ibv_extend/output/lib</code><br><code>ll /usr/local/Ascend/driver/lib64/driver/ | grep libibv_extend.so</code><br>如果任意一个有回显，则表示libibv_extend已安装，结束。<br>如果无回显，则继续。</li>
                <li>安装libibv_extend，请参考<a href="#安装">安装</a>。</li>
            </ol>
        </td>
    </tr>
    <tr >
        <td>Ubuntu/Debian</td>
        <td>
            <ol>
                <li>检查libibverbs.so是否已安装。<br><code>ldconfig -p | grep libibverbs</code><br>如果有回显，则表示libibverbs.so已安装，结束。<br>如果无回显，则继续。</li>               
                <li> 安装libibverbs。<br><code>sudo apt update</code><br><code>sudo apt install libibverbs1 ibverbs-utils rdma-core</code> </li>
            </ol>
        </td>
    </tr>
</table>

**配置环境变量**

上层应用在运行前可通过配置**IBV_EXTEND_DRIVERS**环境变量，让ibv\_extend组件查找并加载指定的驱动扩展库（比如1825、YunMai等网卡扩展驱动），多个驱动用 <strong>:</strong> 或 <strong>;</strong> 分隔。

示例：`export IBV_EXTEND_DRIVERS="/path1/my_ext_driver1.so:/path2/my_ext_driver2.so"`

**配置文件**

1. 创建扩展驱动配置文件目录。

    ```bash
    mkdir -p /etc/libibverbs_extend.d/
    ```

2. 进入扩展驱动配置文件目录。

    ```bash
    cd /etc/libibverbs_extend.d/
    ```

3. 创建扩展驱动配置文件。配置文件内容格式为：driver <driver\_name\>。

    假设驱动扩展库为libmy\_ext\_driver.so，则使用如下命令生成配置文件。

    ```bash
    echo "driver my_ext_driver" > my_ext_driver.conf
    ```

# API接口参考

## 通用接口

### ibv\_extend\_get\_version

**函数原型**

`const char *ibv_extend_get_version(uint32_t *major, uint32_t *minor, uint32_t *patch)`

**功能**

获取ibv_extend动态库版本号。

**参数**

- major：主版本号。输出参数。可以为空，空表示不获取，接口内不额外分配内存。
- minor：次版本号。输出参数。可以为空，空表示不获取，接口内不额外分配内存。
- patch：补丁版本号。输出参数。可以为空，空表示不获取，接口内不额外分配内存。
  
**返回值**

版本字符串指针（静态字符串，无需释放）。

**使用示例**

```c
// 调用函数获取版本信息
version_str = ibv_extend_get_version(&major, &minor, &patch);
// 打印版本信息
printf("版本字符串: %s\n", version_str);
printf("版本号: %u.%u.%u\n", major, minor, patch);
```

### ibv\_extend\_check\_version

**函数原型**

`int ibv_extend_check_version(uint32_t driver_major, uint32_t driver_minor, uint32_t driver_patch)`

**功能**

检查版本兼容性，此函数在运行时检查驱动和上层应用编译时使用的头文件版本与当前库版本是否兼容。建议在驱动和上层应用初始化时调用此函数进行版本检查。

兼容性规则：

1. 主版本号完全一致，则兼容。
2. 软件次版本号 <= 库的次版本号，则兼容。
3. 如果次版本号相同，且软件的补丁版本号 <= 库的补丁版本号，则兼容。

**参数**

- driver_major：主版本号，建议传入IBV_EXTEND_VERSION_MAJOR。
- driver_minor：次版本号，建议传入IBV_EXTEND_VERSION_MINOR。
- driver_patch：补丁版本号，建议传入IBV_EXTEND_VERSION_PATCH。

**返回值**

- 0：兼容。
- -1：不兼容。

**使用示例**

```c
// 调用函数检查版本兼容性
ret = ibv_extend_check_version(IBV_EXTEND_VERSION_MAJOR, IBV_EXTEND_VERSION_MINOR, IBV_EXTEND_VERSION_PATCH);
// 检查返回值
if (ret == 0) {
    printf("版本兼容，可以正常使用\n");
    // 继续后续初始化操作
} else {
    printf("版本不兼容，驱动编译时版本与当前库版本不匹配\n");
    // 拒绝运行
    return -1;
}
```

## 北向接口（应用层调用）

### ibv\_open\_extend

**函数原型**

`struct ibv_context_extend *ibv_open_extend(struct ibv_context *context)`

**功能**

调用驱动注册的接口来创建扩展上下文，调用其他接口前必须调用ibv_open_extend创建扩展上下文。

**参数**

context：标准ibv上下文，使用ibv_open_device创建，非空。

**返回值**

- 扩展上下文指针：创建扩展上下文成功。
- NULL：创建扩展上下文失败。

**使用示例**

```c
// 获取设备列表
int num_devices = 0;
struct ibv_device **dev_list = ibv_get_device_list(&num_devices);
if (!dev_list) {
    fprintf(stderr, "Failed to get device list\n");
    return -1;
}

// 检查是否有可用设备
if (num_devices <= 0) {
    fprintf(stderr, "No device found\n");
    ibv_free_device_list(dev_list);
    return -1;
}

// 选择第一个设备
struct ibv_device *device = dev_list[0];

// 打开设备
struct ibv_context *ctx = ibv_open_device(device);
if (!ctx) {
    fprintf(stderr, "Failed to open device\n");
    ibv_free_device_list(dev_list);
    return -1;
}

// 打开扩展上下文
struct ibv_context_extend *ext_ctx = ibv_open_extend(ctx);
if (!ext_ctx) {
    fprintf(stderr, "Failed to open extend context\n");
    ibv_close_device(ctx);
    ibv_free_device_list(dev_list);
    return -1;
}
```

### ibv\_close\_extend

**函数原型**

`int ibv_close_extend(struct ibv_context_extend *context)`

**功能**

调用驱动注册的接口来释放扩展上下文资源，ibv_close_extend调用之后不能再使用该扩展上下文资源。

**参数**

context：扩展上下文，必须使用ibv_open_extend创建，非空。

**返回值**

- 0：成功。
- EINVAL：参数无效。
- ENOENT：未找到匹配的驱动。
- 驱动异常返回值。

**使用示例**

```c
int ret = ibv_close_extend(ext_ctx);
if (ret != 0) {
    fprintf(stderr, "Failed to close extend context: %d\n", ret);
}
```

### ibv\_query\_device\_extend

**函数原型**

`int ibv_query_device_extend(struct ibv_context_extend *context, struct ibv_device_attr_extend *ext_dev_attr)`

**功能**

查询设备支持的扩展属性。

**参数**

- context：扩展上下文，必须使用ibv_open_extend创建，非空。
- ext_dev_attr：返回的设备扩展属性，内部不额外分配此内存，非空。

**返回值**

- 0：成功。
- EINVAL：参数无效。
- 驱动异常返回值。

**使用示例**

```c
int ret = ibv_query_device_extend (ext_ctx, &ext_dev_attr);
if (ret != 0) {
    fprintf(stderr, "Failed to query extend context: %d\n", ret);
}
if (ext_dev_attr.ext_cap & IBV_EXTEND_DEV_NDA) {
/* 设备支持NDA */
}
```

### ibv\_create\_qp\_extend

**函数原型**

`struct ibv_qp_extend *ibv_create_qp_extend(struct ibv_context_extend *context, struct ibv_qp_init_attr_extend *qp_init_attr)`

**功能**

调用底层驱动的create_qp创建扩展QP。

**参数**

- context：扩展上下文，必须使用ibv_open_extend创建，非空。
- qp_init_attr：QP初始化配置（包含qbuf dma模式和资源回调函数），非空。

**返回值**

- QP扩展结构体指针：创建扩展QP成功。
- NULL：创建扩展QP失败。

**使用示例**

```c
struct ibv_qp_init_attr_extend qp_attr = {
    .pd = pd,
    .attr = {
    .cap = { .max_send_wr = 128, .max_recv_wr = 128 },
    .qp_type = IBV_QPT_RC,
    },
    .type = QU_BUF_DMA_MODE_DEFAULT,
    .ops = &my_extend_ops,
};
struct ibv_qp_extend *qp_ext = ibv_create_qp_extend(ext_ctx, &qp_attr);
```

### ibv\_create\_cq\_extend

**函数原型**

`struct ibv_cq_extend *ibv_create_cq_extend(struct ibv_context_extend *context, struct ibv_cq_init_attr_extend *cq_init_attr)`

**功能**

调用底层驱动的create_cq创建扩展CQ。

**参数**

- context：扩展上下文，必须使用ibv_open_extend创建，非空。
- cq_init_attr：CQ初始化配置（包含qbuf dma模式和资源回调函数），非空。

**返回值**

- CQ扩展结构体指针：创建扩展CQ成功。
- NULL：创建扩展CQ失败。

**使用示例**

```c
struct ibv_cq_init_attr_extend cq_attr = {
    .attr {
        .cqe = 128,
    },
    .type = QU_BUF_DMA_MODE_DEFAULT,
    .ops = &my_extend_ops,
};
struct ibv_cq_extend *cq_ext = ibv_create_cq_extend(ext_ctx, &cq_attr);
```

### ibv\_create\_srq\_extend

**函数原型**

`struct ibv_srq_extend *ibv_create_srq_extend(struct ibv_context_extend *context, struct ibv_srq_init_attr_extend *srq_init_attr)`

**功能**

调用底层驱动的create_srq创建扩展SRQ。

**参数**

- context：扩展上下文，必须使用ibv_open_extend创建，非空。
- srq_init_attr：SRQ初始化配置（包含qbuf dma模式和资源回调函数），非空。

**返回值**

- SRQ扩展结构体指针：创建扩展SRQ成功。
- NULL：创建扩展SRQ失败。

**使用示例**

```c
struct ibv_srq_init_attr_extend srq_attr = {
    .pd = pd,
    .attr {
        .attr {
            max_wr = 16,
        },
    },
    .type = QU_BUF_DMA_MODE_DEFAULT,
    .ops = &my_extend_ops,
};
struct ibv_srq_extend *cq_ext = ibv_create_srq_extend(ext_ctx, &srq_attr);
```

### ibv\_destroy\_qp\_extend

**函数原型**

`int ibv_destroy_qp_extend(struct ibv_context_extend *context, struct ibv_qp_extend *qp_extend)`

**功能**

调用底层驱动的destroy_qp销毁扩展QP。

**参数**

- context：扩展上下文，必须使用ibv_open_extend创建，非空。
- qp_extend：QP扩展结构体，必须由ibv_create_qp_extend创建。

**返回值**

- 0：成功。
- EINVAL：参数无效。
- 驱动异常返回值。

**使用示例**

```c
int destroy_qp_safely(struct ibv_context_extend *ext_context, struct ibv_qp_extend *qp_extend)
{
    if (!ext_context || !qp_extend) {
        return -1;
    }
 
    // 销毁QP
    int ret = ibv_destroy_qp_extend(ext_context, qp_extend);
    if (ret != 0) {
        fprintf(stderr, "ibv_destroy_qp_extend failed: %d\n", ret);
        return ret;
    }
 
    printf("NDA QP销毁成功\n");
    return 0;
}

int ret = destroy_qp_safely(ext_context, qp_extend);</span>
if (ret != 0) {
    // 处理错误
}
```

### ibv\_destroy\_cq\_extend

**函数原型**

`int ibv_destroy_cq_extend(struct ibv_context_extend *context, struct ibv_cq_extend *cq_extend)`

**功能**

调用底层驱动的destroy_cq销毁扩展CQ。

**参数**

- context：扩展上下文，必须使用ibv_open_extend创建，非空。
- cq_extend：CQ扩展结构体，必须由ibv_create_cq_extend创建。

**返回值**

- 0：成功。
- EINVAL：参数无效。
- 驱动异常返回值。

**使用示例**

```c
int destroy_cq_safely(struct ibv_context_extend *ext_context,</span>struct ibv_cq_extend *cq_extend)
{
    if (!ext_context || !cq_extend) {
        return -1;
    }
 
    // 销毁CQ
    int ret = ibv_destroy_cq_extend(ext_context, cq_extend);
    if (ret != 0) {
        fprintf(stderr, "ibv_destroy_cq_extend failed: %d\n", ret);
        return ret;
    }
 
    printf("NDA CQ销毁成功\n");
    return 0;
}

int ret = destroy_cq_safely(ext_context, cq_extend);
if (ret != 0) {
     // 处理错误
}
```

### ibv\_destroy\_srq\_extend

**函数原型**

`int ibv_destroy_srq_extend(struct ibv_context_extend *context, struct ibv_srq_extend *srq_extend)`

**功能**

调用底层驱动的destroy_srq销毁扩展SRQ。

**参数**

- context：扩展上下文，必须使用ibv_open_extend创建，非空。
- srq_extend：SRQ扩展结构体，必须使用ibv_create_srq_extend创建。

**返回值**

- 0：成功。
- EINVAL：参数无效。
- 驱动异常返回值。

**使用示例**

```c
int destroy_srq</span><span>_safely(struct ibv_context_extend *ext_context,</span>struct ibv_srq_extend *srq_extend){
    if (!ext_context || !srq_extend) {
        return -1;
    }
    // 销毁SRQ
    int ret = ibv_destroy_srq_extend(ext_context, srq_extend);
    if (ret != 0) {
        fprintf(stderr, "ibv_destroy_srq_extend failed: %d\n", ret);
        return ret;
    }

    printf("NDA SRQ销毁成功\n");
    return 0;
}

int ret = destroy_srq_safely(ext_context, srq_extend);
if (ret != 0) {
     // 处理错误
}
```

## 南向接口（驱动调用）

### verbs\_register\_driver\_extend

**函数原型**

`void verbs_register_driver_extend(const struct verbs_device_extend_ops *ops)`或`PROVIDER_EXTEND_DRIVER(drv)`

**功能**

将驱动注册到extend_driver_list链表，可以使用封装的PROVIDER_EXTEND_DRIVER宏。

**参数**

- ops：设备扩展操作结构体指针，非空，内部不额外分配地址，必须为全局静态变量。
- drv：必须为struct verbs_device_extend_ops类型。

**返回值**

无。

**使用示例**

```c
static const struct verbs_device_extend_ops my_driver_ops = {
    .name = "my_driver",    // 必须与标准驱动保持一致
    .alloc_context = my_alloc_context,
    .free_context = my_free_context,
};
PROVIDER_EXTEND_DRIVER(my_driver_ops);  // 使用宏自动注册
```

# 关键结构体说明

## 枚举类型定义

### queue\_buf\_dma\_mode

在创建扩展的QP/CQ/SRQ资源过程中，针对这些资源，驱动需要建立host地址和device地址的映射，以便device可以直接驱动网卡资源，当前支持两种映射模式。

|  变量   | 值  | 说明  |
|  ----  | ----  | ----  |
| QU_BUF_DMA_MODE_DEFAULT  | 0 | PCIe映射模式，该模式支持通用硬件形态，网卡和NPU都是PCIe设备，通常使用peerMem方式映射。 |
| QU_BUF_DMA_MODE_INDEP_UB  | 1 | UB映射模式，网卡和NPU虽是PCIe设备，但是数据通路使用独立的UB总线寻址，通常使用UbShmem的方式映射。 |

### doorbell\_map\_mode

在创建扩展的QP/CQ/SRQ资源过程中，驱动会分配doorbell地址，同时需要将host侧分配的doorbell地址映射到NPU卡侧，映射的逻辑（db\_mmap）由上层实现，上层需要根据驱动提供的映射类型进行映射。

|  变量   | 值  | 说明  |
|  ----  | ----  | ----  |
| DB_MAP_MODE_HOST_VA  | 0 | 基于Host虚拟地址映射，通常为三方网卡的资源映射方式。 |
| DB_MAP_MODE_UB_RES  | 1 | 使用UB设备的资源描述符进行映射，1825网卡使用的映射方式。 |

### memcpy\_direction

在驱动使用上层注册的memcpy\_s拷贝内存时，需指明拷贝的方向，以允许驱动对设备内存的读写访问。

|  变量   | 值  | 说明  |
|  ----  | ----  | ----  |
| MEMCPY_DIR_HOST_TO_HOST | 0 | memcpy_s参数，Host内复制。 |
| MEMCPY_DIR_HOST_TO_DEVICE  | 1 | memcpy_s参数，Host到Device（NPU）的内存复制。 |
| MEMCPY_DIR_DEVICE_TO_HOST | 2 | memcpy_s参数，Device（NPU）到Host的内存复制。 |
| MEMCPY_DIR_DEVICE_TO_DEVICE  | 3 | memcpy_s参数，Device（NPU）内或Device（NPU）间的内存复制。 |

### ibv\_qp\_init\_cap

在创建扩展QP时上层通信库可以指定额外的扩展配置，可选。

|  变量   | 值  | 说明  |
|  ----  | ----  | ----  |
| QP_ENABLE_DIRECT_WQE  | 1 | 如果网卡支持direct_wqe的能力，通过配置该字段，可在调用ibv_create_qp_extend接口创建sq_info时使能网卡direct_wqe的能力。 |

### ibv\_extend\_device\_cap

设备支持的扩展能力，由扩展query接口返回，有驱动查询当前设备是否支持NDR/NDA。

|  变量   | 值  | 说明  |
|  ----  | ----  | ----  |
| IBV_EXTEND_NDR  | 1 | 设备支持NDR（NPUDirect Rdma）能力。 |
| IBV_EXTEND_NDA  | 2 | 设备支持NDA（NPUDirect Async）能力。 |

## 核心数据结构

### doorbell\_map\_desc

doorbell\_map描述符用于驱动侧为硬件doorbell地址做映射使用，驱动分配出doorbell之后通过上层传递的db\_map回调映射到NPU，供NPU直驱。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| hva  | uint64_t | doorbell在host侧的虚拟地址，PCIe网卡使用，地址必须有效。 |
| ub_res  | struct | UB映射方式使用，使用UB的资源描述符，地址必须有效。详见下面的结构体描述。|
| size  | uint64_t | doorbell映射的长度。 |
| type  | uint32_t | doorbell的映射类型（doorbell_map_mode类型）。 |
| resv  | uint32_t | 预留。 |

```c
struct {
    uint64_t guid_l;   /* GUID低64位*/
    uint64_t guid_h;   /* GUID高64位*/
    struct {
        uint64_t resource_id : 4; /* 对应GUID中的res_id */
        uint64_t offset : 32;  /* 对应某个GUID中某个页id中的偏移量 */
        uint64_t resv : 28;   /* 预留 */
    } bits;
} ub_res;
```

### ibv\_extend\_ops

该系列函数用于在创建QP/CQ/SRQ等资源时的内存分配和映射，上层应用实现，供底层驱动回调使用。

| 函数原型  |  功能说明  |  入参说明  |  返回值说明  |
| ----  |  ----  | ----  | ----  |
| `void *(*alloc)(size_t size)`  | 用于底层驱动申请内存资源，可以是NPU内存资源，也可以是host内存资源。 | size：分配的长度，单位字节，大于0。 |  分配好的内存指针，分配失败返回空。  |
| `void (*free)(void *ptr)` | 用于底层驱动对内存的释放。内存必须是alloc回调函数申请的地址。|ptr：使用alloc接口分配的内存指针。 |  无返回值。  |
| `void (*memset_s)(void *dst, int value, size_t count)`  | 用于底层驱动对内存的初始化操作。内存必须是alloc回调函数申请的地址。 | <li>dst：使用alloc接口分配的内存指针，memset_s并不对指针ptr指向的内存区域做边界检查，因此使用时需要确保ptr指向的内存区域足够大，避免发生越界访问。</li><li>value：通常是一个 int 类型的值，但实际上只使用了该值的低8位。这意味着在范围 0 到 255 之外的其他值可能会产生未定义的行为。</li><li>count：表示要设置的字节数，通常是通过 sizeof() 或其他手段计算得到的。count不能大于alloc时申请的size。</li> |  无返回值。  |
| `int (*memcpy_s)(void *dst, size_t dst_max_size, void *src, size_t size, uint32_t direct)` | 用于底层驱动对内存的拷贝操作。内存必须是alloc回调函数申请的地址。 | <li>dst：拷贝的目标地址，可以是host侧分配的地址，也可以是alloc分配的地址。</li><li>dst_max_size：为拷贝安全，需传入dst目标地址的最大长度。</li><li>src：拷贝的源地址，可以是host侧分配的地址，也可以是alloc分配的地址。</li><li>size：src中实际拷贝的长度，单位为字节，不能超过src分配的长度。</li><li>direct：拷贝方向，类型为enum memcpy_direction。</li> |  成功返回0，失败返回负数错误码。  |
| `void *(*db_mmap)(struct doorbell_map_desc *desc)`  | 用于doorbell映射逻辑，通常实现为host上doorbell地址映射到NPU内存的逻辑。<br>实现约束：必须支持相同desc的重复映射，即相同desc地址多次映射必须返回相同的映射地址。 | desc：doorbell资源映射描述符。 |  映射后的地址指针，失败返回空。  |
| `int (*db_unmap)(void *ptr , struct doorbell_map_desc *desc)` | 用于解除映射的逻辑。 | <li>ptr：调用db_mmap的接口映射的地址。</li><li>desc：db_mmap时的输入desc。</li> |  成功返回0，失败返回负数错误码。  |

### queue\_buf

用于描述RDMA队列的基础信息，包含首地址和长度，本结构体包含在queue\_info结构体中。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| base  | uint64_t | 队列的首地址，NPU内可访问的地址，非空。 |
| entry_cnt  | uint32_t | 队列中单元的个数（WQE/CQE个数），受驱动的规格约束。 |
| entry_size  | uint32_t | 队列中每个单元的大小（WQE/CQE大小），受驱动的规格约束。 |
| resv  | uint64_t [] | 预留。 |

### queue\_info

描述队列的完整信息，包括qbuf和相应保存pi、ci指针的地址，本结构体包含在ibv扩展资源结构体中。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| qbuf  | struct queue_buf | 队列基础信息。 |
| dbr_pi_va  | struct iov_addr_desc | pi地址，NPU内可访问地址，地址指向的内容保存head，地址非空。 |
| dbr_ci_va  | struct iov_addr_desc | ci地址，NPU内可访问地址，地址指向的内容保存tail，地址非空。 |
| db_hw_va  | struct iov_addr_desc | 映射后的doorbell的地址，用于NPU内直驱doorbell，地址非空。 |
| resv  | uint64_t [] | 预留。 |

### ibv\_qp\_extend

QP创建成功后返回给应用的扩展信息，该内存为驱动分配的host内存。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| qp  | struct ibv_qp * | 标准QP句柄，需底层驱动根据入参创建，可参考[ibv_create_qp](https://man7.org/linux/man-pages/man3/ibv_create_qp.3.html)，非空。 |
| sq_info  | struct queue_info | SQ队列信息。 |
| rq_info  | struct queue_info | RQ队列信息。 |
| resv  | uint64_t [] | 保留字段。 |

### ibv\_cq\_extend

CQ创建成功后返回给应用的扩展信息，该内存为驱动分配的host内存。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| cq  | struct ibv_cq * | 标准CQ句柄，需底层驱动根据入参创建，可参考[ibv_create_cq_ex](https://man7.org/linux/man-pages/man3/ibv_create_cq_ex.3.html) 或 [ibv_create_cq](https://man7.org/linux/man-pages/man3/ibv_create_cq.3.html)，非空。 |
| cq_info  | struct queue_info | CQ队列信息。 |
| resv  | uint64_t [] | 保留字段。 |

### ibv\_srq\_extend

SRQ创建成功后返回给应用的扩展信息，该内存为驱动分配的host内存。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| srq  | struct ibv_srq * | 标准SRQ句柄，需底层驱动根据入参创建，可参考[ibv_create_srq](https://www.man7.org/linux/man-pages/man3/ibv_destroy_srq.3.html)，非空。 |
| srq_info  | struct queue_info | SRQ队列信息。 |
| resv  | uint64_t [] | 保留字段。 |

### ibv\_qp\_init\_attr\_extend

创建QP时的输入参数。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| pd  | struct ibv_pd * | 标准pd句柄，业务传入，创建该资源请参考[ibv_alloc_pd](https://man7.org/linux/man-pages/man3/ibv_alloc_pd.3.html)。 |
| attr  | struct ibv_qp_init_attr | 标准QP初始化参数，业务传入，该属性的填写请参考[ibv_create_qp](https://man7.org/linux/man-pages/man3/ibv_create_qp.3.html)。 |
| qp_cap_flag  | uint32_t | 扩展QP的属性配置，ibv_qp_init_cap类型。 |
| type  | enum queue_buf_dma_mode | queue_buf的dma寻址类型，参考[queue_buf_dma_mode](#queue_buf_dma_mode)。 |
| ops  | struct ibv_extend_ops * | 回调函数集，用于资源的创建与db的映射，非空。 |
| resv  | uint64_t [] | 预留。 |

### ibv\_cq\_init\_attr\_extend

创建CQ时的输入参数。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| attr  | struct ibv_cq_init_attr_ex | 标准cq_ex初始化参数，业务传入，该属性的填写请参考[ibv_create_cq_ex](https://man7.org/linux/man-pages/man3/ibv_create_cq_ex.3.html)。 |
| cq_cap_flag  | uint32_t | 扩展CQ的属性配置，当前预留。 |
| type  | enum queue_buf_dma_mode | queue_buf的dma寻址类型，参考[queue_buf_dma_mode](#queue_buf_dma_mode)。 |
| ops  | struct ibv_extend_ops * | 回调函数集，用于资源的创建与db的映射，非空。 |
| resv  | uint64_t [] | 预留。 |

### ibv\_srq\_init\_attr\_extend

创建SRQ时的输入参数。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| pd  | struct ibv_pd * | 标准pd句柄，业务传入，创建该资源请参考[ibv_alloc_pd](https://man7.org/linux/man-pages/man3/ibv_alloc_pd.3.html)。 |
| srq_init_attr  | struct ibv_srq_init_attr | 标准SRQ初始化参数，业务传入，该属性的填写请参考[ibv_create_srq](https://man7.org/linux/man-pages/man3/ibv_create_srq.3.html)。 |
| comp_mask  | uint32_t | 兼容性掩码。 |
| srq_cap_flag  | uint32_t | 扩展SRQ的属性配置，当前预留。 |
| type  | enum queue_buf_dma_mode | queue_buf的dma寻址类型，参考[queue_buf_dma_mode](#queue_buf_dma_mode)。 |
| ops  | struct ibv_extend_ops * | 回调函数集，用于资源的创建与db的映射，非空。 |
| resv  | uint64_t [] | 预留。 |

### ibv\_device\_attr\_extend

扩展设备的属性，用于ibv\_query\_device\_extend查询。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| ext_cap  | uint32_t | 查询出的设备扩展属性，可查询出多种属性，类型约束为ibv_extend_device_cap。 |
| resv  | uint32_t[] | 预留字段。 |

### ibv\_context\_extend

扩展上下文，包含标准上下文和扩展操作接口。

|  变量   | 类型  | 说明  |
|  ----  | ----  | ----  |
| context  | struct ibv_context * | 标准ibv上下文，驱动赋值，不能为空。 |
| ops  | struct ibv_context_extend_ops * | 扩展操作接口，包含驱动注册的QP/CQ/SRQ的创建和销毁，非空，通常为全局静态变量。 |

### ibv\_context\_extend\_ops

南向接口，由设备驱动实现，供ibv\_extend模块调用。

|  回调函数指针   | 说明  |
|  ----  | ----  |
| `struct ibv_qp_extend *(*create_qp)(struct ibv_context *context, struct ibv_qp_init_attr_extend *qp_init_attr)`  | 扩展QP创建接口，驱动实现，空表示不支持。 |
| `struct ibv_cq_extend *(*create_cq)(struct ibv_context *context, struct ibv_cq_init_attr_extend *cq_init_attr)`  | 扩展CQ创建接口，驱动实现，空表示不支持。 |
| `struct ibv_srq_extend *(*create_srq)(struct ibv_context *context, struct ibv_srq_init_attr_extend *srq_init_attr)`  | 扩展SRQ创建接口，驱动实现，空表示不支持。 |
| `int (*destroy_qp)(struct ibv_qp_extend *qp_extend)`  | 扩展QP销毁接口，驱动实现，空表示不支持。 |
| `int (*destroy_cq)(struct ibv_cq_extend *cq_extend)`  | 扩展CQ销毁接口，驱动实现，空表示不支持。 |
| `int (*destroy_srq)(struct ibv_srq_extend *srq_extend)`  | 扩展SRQ销毁接口，驱动实现，空表示不支持。 |
| `int (*query_device)(struct ibv_context *context, struct ibv_device_attr_extend *ext_dev_attr)`  | 查询设备扩展属性接口，驱动实现，空表示不支持。 |

### verbs\_device\_extend\_ops

设备级扩展操作接口，由设备驱动实现，name字段必须与标准驱动一致，用于扩展驱动注册时匹配标准驱动。

|  回调函数指针   | 说明  |
|  ----  | ----  |
| `const char *name`  | 用于匹配驱动原本ibv_context，必须与驱动厂商中verbs_device_ops定义的name一致，不能为空。 |
| `struct ibv_context_extend *(*alloc_context)(struct ibv_context *context)`  | 用于分配扩展上下文，驱动实现，空表示不支持。 |
| `void (*free_context)(struct ibv_context_extend *context)`  | 用于释放扩展上下文，驱动实现，空表示不支持。 |

# FAQ

## 调用ibv\_open\_extend接口初始化扩展库报错

**问题描述**

上层通信组件调用ibv\_open\_extend接口返回空指针，在配置**IBV_EXTEND_SHOW_WARNINGS**环境变量后，报错如下：

```text
ibv_extend: Warning: no available ops for open extend context
```

**可能原因**

ibv\_extend扩展库无法找到对应网卡扩展驱动或者dlopen对应网卡扩展驱动报错。

**解决方案**

1. 参考[使用](#使用)，使ibv\_extend能够找到对应网卡扩展驱动的位置。
2. 如果使用dlopen方式加载libibv\_extend.so，需要通过RTLD\_GLOBAL导出符号到全局符号表，使用示例如下。

    ```c
    void *handle = dlopen("libibv_extend.so", RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        fprintf(stderr, "dlopen libibv_extend.so error: %s\n", dlerror());
        return;
    }
    ```

3. 如果仍然报错，请联系对应网卡扩展驱动厂商确认扩展驱动包是否正确。
