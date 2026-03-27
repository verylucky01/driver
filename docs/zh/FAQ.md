# FAQ 常见问题汇总

## 问题一
- 问题描述：安装driver开源run包失败，比如无权限、依赖找不到等问题。
- 解决方法：请优先参考配套版本的[安装指南](https://hiascend.com/document/redirect/CannCommunityInstSoftware)（选择安装场景后，参见“安装NPU驱动和固件”章节，以及“附录C：安装故障处理”章节）。

## 问题二
- 问题描述：安装run包失败，日志中报错“do not have root permission, operation failed, please use root permission”。
- 可能原因：使用非root用户执行了安装命令。
- 解决方法：切换到root用户后重新安装。

## 问题三
- 问题描述：环境首次安装，下载源码后执行`build.sh --pkg --soc=ascend910b`时出现报错，提示linux的/lib/modules/xxx/build目录不存在。例如：
```
/lib/modules/5.10.0-60.18.0.50.r865_35.hce2.aarch64/build: No Such file or directory.
```
- 可能原因：系统未安装与当前内核版本匹配的`linux-headers`软件包（使用`uname -r`可查询环境当前的内核版本）。
- 解决方法：
<br/>
1、安装与当前内核版本匹配的`linux-headers`软件包，例如在ubuntu环境下，执行`apt install -y linux-headers-$(uname -r)`。
<br/>
2、如果需要使用的内核版本与当前内核版本不同，可以在build命令中添加`-k $patch`指定内核版本路径（注意，重新编译前，需要先清除缓存）。
```bash
build.sh --make_clean
build.sh --pkg --soc=ascend910b -k $patch
```

## 问题四
- 问题描述：安装run包时失败，报“set username and usergroup, HwHiAiUser:HwHiAiUser”。
- 可能原因：
<br/>
1、首次安装，在不指定用户的情况下，默认使用HwHiAiUser属组，环境上没有添加HwHiAiUser属组。
<br/>
2、用户自定义了属组，安装时需要指定用户自定义的属组。
- 解决方法：
```bash
#添加 HwHiAiUser 属组方法如下
groupadd HwHiAiUser
useradd -g HwHiAiUser -d /home/HwHiAiUser -m HwHiAiUser -s /bin/bash
```
```bash
#指定用户自定义的属组方法如下（以HwHiAiUser为例）
xxx-driver-xxx.run --full --install-username=HwHiAiUser --install-usergrough=HwHiAiUser
```
注：run包安装命令可以使用 --help 获取，参数含义可以参考[安装指南](https://hiascend.com/document/redirect/CannCommunityInstSoftware)（参见“附录A：参考信息->参数说明”章节）。

## 问题五
- 问题描述：build脚本编译run包，包名格式为`Ascend-hdk-<chip_type>-driver-<version>_<os_version>-<arch>.run`，编译环境 uname 查询不是 ubuntu 环境，但是编译出的包名中os_version这里显示为ubuntu。
- 可能原因：uname 查询的内核信息不完整。
- 解决方法：编译run包时，os_version是从os-release信息中获取的，可以使用下面命令查询对应的os_version。
```bash
cat /etc/os-release
```

## 问题六
- 问题描述：昇腾的驱动层会对外开放API接口吗？
- 问题解答：昇腾Driver当前不会对外直接开放API接口，建议基于昇腾的AI应用或者框架使用ACL接口。
- 参考资料：[CANN社区版](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition)，可以选择对应的CANN版本，参考对应API章节。

## 问题七
- 问题描述：开源驱动支持哪些产品形态？
- 问题解答：社区Driver仓支持910系列的910B、910_93、950。
- 参考资料：其他产品形态参考昇腾社区。

## 问题八
- 问题描述：编译环境准备阶段，`apt install -y linux-headers-$(uname -r)`安装失败，会影响driver包源码编译吗？
- 可能原因：没有和当前内核匹配的linux-headers。
- 解决方法：如果环境上已经安装了其他版本的linux-headers，不重新安装匹配的linux-headers也可以编译；请参考`build.sh --help`帮助，增加-k参数指定内核头文件路径，如下所示：
```bash
-k Set kernel source path, default "/lib/modules/$(uname -r)/build"
```

## 问题九
- 问题描述：如何在编译CANN驱动时开启 DEBUG 宏开关，以启用 pr_debug 日志输出？
- 问题解答：pr_debug使用了linux原生的/sys/kernel/debug/dynamic_debug/control机制去动态开启关闭函数级或文件级的打印，可针对环境类型查询下具体开启方式。

## 问题十
- 问题描述：编译driver开源包后，再编译新的不同版本driver开源包，发现前面生成的driver开源包文件被删除。
- 问题解答：这种属于当前编译框架的正常现象，触发新的编译时会主动清除前面的编译缓存；所以触发新的编译时，需要把前一次编译的文件保存。