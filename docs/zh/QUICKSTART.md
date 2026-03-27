# 快速入门
本指南旨在帮助您快速上手driver仓的使用。驱动开发和贡献流程如下图，我们欢迎并鼓励您在社区贡献，共同丰富项目生态。

```mermaid
graph LR
subgraph 开发阶段
环境准备
开源仓代码下载
驱动开发
编译及部署
验证
end
subgraph 开源贡献阶段
提交PR
CI门禁
Committer检视
Maintainer检视合入
end

User-->环境准备-->开源仓代码下载-->驱动开发-->编译及部署-->验证-->提交PR-->CI门禁-->Committer检视-->Maintainer检视合入
```

# 目录导读
1.&emsp;***[环境部署及编译构建](#环境部署及编译构建)***：搭建开发和运行环境，编译源码并部署安装。

2.&emsp;***[调试验证](#调试验证)***：开发和运行过程中的日志查看及设置指导。

3.&emsp;***[开发指南](#开发指南)***：自定义开发驱动的指南，学习从零开发、编译及验证代码。

# <h2 id="环境部署及编译构建">一、环境部署及编译构建</h2>
## 1.编译环境准备
Driver支持源码编译，进行源码编译前，请根据如下步骤完成相关环境准备。


Driver源码编译需安装的依赖软件：

   - gcc
   - cmake
   - bash
   - kernel-headers
   - net-tools
   - openssl开发库
   - pkg-config
   - patch
   - googletest（可选，仅执行UT测试时依赖，建议版本[release-1.11.0](https://github.com/google/googletest/releases/tag/release-1.11.0))
   - makeself（执行本地编译run包时依赖，[下载链接](https://github.com/megastep/makeself))

  <br/>
   若未安装，请根据使用的linux发行版本，自行在相应操作系统厂商网站获取并安装。
  <br/>
  <br/>


```bash
#此处以openeuler环境为例：
yum install -y tar net-tools kernel-headers-$(uname -r) kernel-devel-$(uname -r) openssl-devel pkg-config patch
yum install -y gcc gcc-c++ g++ cmake make libffi libffi-devel binutils binutils-devel elfutils elfutils-devel elfutils-libelf-devel
```

```bash
#此处以ubuntu环境为例：
apt install -y tar net-tools linux-headers-$(uname -r) gcc g++ cmake make libffi-dev libssl-dev pkg-config patch
```

## 2.源码下载
执行如下命令，下载Driver仓源码：
```
git clone https://gitcode.com/cann/driver.git
```
## 3.源码编译及部署
编译依赖开源第三方库和Driver开源二进制库，启动编译后会自动下载，请保持网络畅通。
推荐OS版本使用linux v5.4、linux v5.10或linux v6.8。
<p style="line-height:1.5;">
1. 按照如下命令执行源码编译：

```
bash build.sh --pkg --soc=${chip_type}
```
`${chip_type}`表示芯片类型，当前主要包括`ascend910b`、`ascend910_93`和`ascend950`。

编译完成之后会在`build_out`目录下产生`Ascend-hdk-<chip_type>-driver-<version>_<os_version>-<arch>.run`软件包。

注意事项：
1、重复执行该编译命令之前，需要手动清理上一次编译产生的文件，否则可能会有缓存。
2、ascend950不支持在ARM环境上编译。
```bash
#清理命令
bash build.sh --make_clean
```
</p>
<p style="line-height:1.5;">
2. 部署安装：

执行如下命令，安装Driver包：

```
./Ascend-hdk-<chip_type>-driver-<version>_<os_version>-<arch>.run --full
```
`<chip_type>`表示芯片类型，当前主要包括`910b`、`A3`和`950`；`<version>`表示软件包版本号，例如8.5.0；`<os_version>`表示操作系统发行版本，例如Ubuntu20.04、openEuler22.03；`<arch>`表示芯片架构，取值包括x86_64与aarch64。

安装完成之后，用户编译生成的Driver软件包会替换已安装CANN开发套件包中的Driver相关软件。

如需要安装固件包，从[昇腾官网](https://www.hiascend.com/hardware/firmware-drivers/commercial)获取配套硬件产品的固件包，并按照配套版本的[安装指南](https://hiascend.com/document/redirect/CannCommunityInstSoftware)安装（选择安装场景后，参见“安装NPU驱动和固件”章节）。
</p>

## 4.其余功能
更多编译参数可以通过`bash build.sh -h`查看。
## 5.软件包卸载
按照配套版本的[卸载指导](https://hiascend.com/document/redirect/CannCommunityInstSoftware)卸载（选择安装指南后，参见“卸载”章节）。

# <h2 id="调试验证">二、调试验证</h2>

开发过程中若遇到程序调试问题，通常应先查看应用程序运行时在Host侧产生的用户态日志（以下简称应用类日志），再通过"dmesg"或系统日志文件查看Host侧的内核态日志。若仍无法定位问题，则需使用msnpureport工具导出更详细的Device侧日志（以下简称系统类日志）。

应用类日志和系统类日志默认记录级别为"ERROR"，若"ERROR"级别日志不足以支持问题定位，可设置更详细的日志级别（如"DEBUG"），以进行深入分析。

## 1.日志查看

- **查看应用类日志**：请查看配套版本的[日志参考](https://hiascend.com/document/redirect/CANNCommunitylogreflevel)，在“查看应用类日志”章节中获取详细说明。

- **查看Host侧内核日志**：

    - 查看"ERROR"级别日志，可执行以下命令：

        ```
        dmesg
        ```
        或根据操作系统查看"/var/log/messages"或"/var/log/syslog"。

    - 如需查看运行过程中产生的 "INFO"、"WARNING"、"EVENT"级别日志，请执行以下命令导出日志：

        ```
        /usr/local/Ascend/driver/tools/msnpureport -f
        ```
        执行后将生成一个以时间戳命名的目录，日志文件位于："./时间戳/slog/host/host_kernel.log"。

- **查看系统类日志**：请参考配套版本的[日志参考](https://hiascend.com/document/redirect/CANNCommunitylogreflevel)，在“查看系统类日志”章节中获取详细说明。

## 2.日志设置

- **应用类日志级别设置**：请参考[日志参考](https://hiascend.com/document/redirect/CANNCommunitylogreflevel)，在“设置应用类日志级别”章节中获取配置方法。

- **系统类日志级别设置**：请参考[日志参考](https://hiascend.com/document/redirect/CANNCommunitylogreflevel)，在“设置系统类日志级别”章节中获取配置方法。

# <h2 id="开发指南">三、开发指南</h2>

本阶段目的是通过在驱动代码中新增接口方式，熟悉驱动开发。这里以新增DCMI接口为例。

## 1. 修改驱动代码

在`driver/src/ascend_hal/dmc/dsmi/dsmi_common/dsmi_common_interface.c`文件中新增一个dsmi接口，代码如下：

```
int dsmi_get_host_device_connect_type(int device_id, unsigned int *connect_type)
{
    int ret;
    struct devdrv_device_info dev_info = { 0 };

    if (connect_type == NULL) {
        return DRV_ERROR_INVALID_VALUE;
    }

    /* drvGetDevInfo：获取npu的设备信息 */
    ret = drvGetDevInfo((unsigned int)device_id, &dev_info);
    if (ret == (int)DRV_ERROR_RESOURCE_OCCUPIED) {
        return DRV_ERROR_RESOURCE_OCCUPIED;
    }

    *connect_type = dev_info.host_device_connect_type;
    return 0;
}
```

同时在`driver/pkg_inc/dsmi_common_interface.h`文件中新增接口声明。

```
/**
* @ingroup driver
* @brief host-device connect types
* @attention null
* @param [in]  device_id  device id
* @param [out] connect_type  host-device connect types
* @return  0 for success, others for fail
*/
int dsmi_get_host_device_connect_type(int device_id, unsigned int *connect_type);
```

在`driver/src/custom/dev_prod/user/dcmi/dcmi_interface/src/dcmi_basic_info_intf.c`文件中新增一个dcmi接口，代码如下：

```
int dcmi_get_host_device_connect_type(int device_id, unsigned int *connect_type)
    {
        int ret;
    
        if (dcmi_get_run_env_init_flag() != TRUE) {
            gplog(LOG_ERR, "not init.");
            return DCMI_ERR_CODE_NOT_REDAY;
        }
    
        if ((connect_type == NULL) || (device_id < 0)) {
            gplog(LOG_ERR, "para is invalid");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
    
        ret = dsmi_get_host_device_connect_type(device_id, connect_type);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dsmi_get_host_device_connect_type failed. err is %d.", ret);
            return ret;
        }
    
        return DCMI_OK;
    }
```

## 2. 编译与更新驱动包

参考[环境部署及编译构建](#环境部署及编译构建)章节1~2步骤，重新编译以及安装驱动包

## 3. 验证

## 4. 执行结果

