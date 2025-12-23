# 环境部署及编译构建
## 环境准备
Driver支持源码编译，进行源码编译前，请根据如下步骤完成相关环境准备。
### 编译环境准备
   Driver源码编译安装依赖软件：

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

## 源码下载
执行如下命令，下载Driver仓源码：
```
git clone https://gitcode.com/cann/driver.git
```
## 源码编译及部署
编译依赖开源第三方库和Driver开源二进制库，启动编译后会自动下载，请保持网络畅通。
推荐OS版本使用linux v5.4、linux v5.10或linux v6.8。
<p style="line-height:1.5;">
<style>1. 按照如下命令完成run编译：<style>

```
bash build.sh --pkg --soc=${chip_type}
```
`${chip_type}`表示芯片类型，当前主要包括`ascend910b`和`ascend910_93`。

编译完成之后会在`build_out`目录下产生`Ascend-hdk-<chip_type>-driver-<version>_<os_version>-<arch>.run`软件包。

注意事项：重复执行该编译命令之前，需要手动清理上一次编译产生的文件，否则可能会有缓存。
```bash
#清理命令
bash build.sh --make_clean
```
<p style="line-height:1.5;">
2. 部署安装：
<p style="line-height:1.5;">
执行如下命令，安装Driver包：

```
./Ascend-hdk-<chip_type>-driver-<version>_<os_version>-<arch>.run --full
```
`<chip_type>`表示芯片类型，当前主要包括`910b`和`A3`；`<os_version>`表示操作系统发行版本，例如ubuntu20.04、openeuler22.03；`<arch>`表示芯片架构，取值包括x86_64与aarch64。

安装完成之后，用户编译生成的Driver软件包会替换已安装CANN开发套件包中的Driver相关软件。

如需要安装固件包，从[昇腾官网](https://www.hiascend.com/hardware/firmware-drivers/commercial)获取配套硬件产品的固件包，并按照配套版本的[安装指南](https://hiascend.com/document/redirect/CannCommunityInstSoftware)安装（选择安装场景后，参见“安装NPU驱动和固件”章节）。
<p style="line-height:1.5;"> 
3. 日志参考：

如需要查询运行日志，请查看配套版本的[日志参考](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850alpha002/maintenref/logreference/logreference_0001.html)。
## 其余功能
更多编译参数可以通过`bash build.sh -h`查看。
## 软件包卸载
按照配套版本的[卸载指导](https://hiascend.com/document/redirect/CannCommunityInstSoftware)卸载（选择安装指南后，参见“卸载”章节）。

## 本地验证（UT可选）
安装完编译生成的Driver驱动包后，可以通过如下命令执行UT用例。
```
cd test
bash run_test.sh -t
```