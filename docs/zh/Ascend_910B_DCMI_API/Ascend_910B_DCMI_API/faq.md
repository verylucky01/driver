# faq<a name="ZH-CN_TOPIC_0000002517535339"></a>

## 准备ipmitool软件<a name="ZH-CN_TOPIC_0000002485455432"></a>

带外标卡复位功能依赖ipmitool软件，需要提前下载并加载驱动。

**操作步骤<a name="zh-cn_topic_0000001206147258_zh-cn_topic_0000001223172961_zh-cn_topic_0000001179877623_section12232175417"></a>**

1.  下载ipmitool软件，例如：
    -   CentOS：**yum install ipmitool**
    -   Ubuntu：**apt-get install ipmitool**

2.  <a name="zh-cn_topic_0000001206147258_zh-cn_topic_0000001223172961_zh-cn_topic_0000001179877623_li135416108536"></a>执行如下命令，加载驱动。

    **modprobe ipmi\_si**

    **modprobe ipmi\_devintf**

    **modprobe ipmi\_msghandler**

3.  <a name="zh-cn_topic_0000001206147258_zh-cn_topic_0000001223172961_zh-cn_topic_0000001179877623_li154541422181820"></a>执行如下命令，查看驱动是否加载成功。

    **lsmod | grep ipmi**

    如果没有加载成功，执行[2](#zh-cn_topic_0000001206147258_zh-cn_topic_0000001223172961_zh-cn_topic_0000001179877623_li135416108536)重新加载驱动。

    >![](public_sys-resources/icon-note.gif) **说明：** 
    >如果重启OS后，需要重新执行[2](#zh-cn_topic_0000001206147258_zh-cn_topic_0000001223172961_zh-cn_topic_0000001179877623_li135416108536)\~[3](#zh-cn_topic_0000001206147258_zh-cn_topic_0000001223172961_zh-cn_topic_0000001179877623_li154541422181820)。


## 查询NPU业务进程<a name="ZH-CN_TOPIC_0000002517535293"></a>

可使用fuser软件查询NPU业务进程。

**操作步骤<a name="section162691339587"></a>**

1.  安装fuser软件，例如：
    -   CentOS：**yum install psmisc**
    -   Ubuntu：**apt-get install psmisc**

2.  执行如下命令查询NPU当前时刻的业务进程。

    **fuser -uv /dev/davinci\* /dev/devmm\_svm /dev/hisi\_hdc**

    若无回显信息，表示当前时刻无业务进程。


## 调用dcmi\_prbs\_operate接口执行prbs打流返回错误码-8006<a name="ZH-CN_TOPIC_0000002517615395"></a>

**问题描述<a name="section1258661685713"></a>**

在调用dcmi接口执行prbs打流功能时，接口可能会返回错误码-8006，导致prbs打流功能无法正常使用。

**可能原因<a name="section1186052310578"></a>**

开启prbs打流功能时，可能会由于i2c资源抢占失败，导致功能执行失败。

**解决方案<a name="section197601727115714"></a>**

建议在出现该错误码时，重新下发prbs打流命令进行重试。


