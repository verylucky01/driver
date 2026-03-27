# dcmi\_get\_device\_component\_static\_version<a name="ZH-CN_TOPIC_0000002517535393"></a>

**函数原型<a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_toc533412077"></a>**

**int dcmi\_get\_device\_component\_static\_version\(int card\_id, int device\_id, enum dcmi\_component\_type component\_type, unsigned char \*version\_str, unsigned int len\)**

**功能说明<a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_toc533412078"></a>**

查询静态组件版本。

**参数说明<a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_toc533412079"></a>**

<a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p10021890"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p10021890"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17.1%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p6466753"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p6466753"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="15.879999999999999%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p54045009"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p54045009"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.019999999999996%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p15569626"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p15569626"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p36741947142813"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p36741947142813"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p96741747122818"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p96741747122818"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.879999999999999%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p46747472287"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p46747472287"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1467413474281"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1467413474281"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p7711145152918"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p7711145152918"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p671116522914"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p671116522914"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.879999999999999%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1771116572910"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1771116572910"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p127554961713"><a name="p127554961713"></a><a name="p127554961713"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p5522164215178"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p5522164215178"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p5522164215178"></a>component_type</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p8522242101715"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p8522242101715"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.879999999999999%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1931913910520"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1931913910520"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1931913910520"></a>enum dcmi_component_type</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p0522164231718"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p0522164231718"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p0522164231718"></a>固件类型</p>
<p id="p1462094752517"><a name="p1462094752517"></a><a name="p1462094752517"></a>enum dcmi_component_type {</p>
<p id="p1562119476256"><a name="p1562119476256"></a><a name="p1562119476256"></a>DCMI_COMPONENT_TYPE_NVE,</p>
<p id="p126211247102515"><a name="p126211247102515"></a><a name="p126211247102515"></a>DCMI_COMPONENT_TYPE_XLOADER,</p>
<p id="p9621647112520"><a name="p9621647112520"></a><a name="p9621647112520"></a>DCMI_COMPONENT_TYPE_M3FW,</p>
<p id="p1562134772520"><a name="p1562134772520"></a><a name="p1562134772520"></a>DCMI_COMPONENT_TYPE_UEFI,</p>
<p id="p16621164742519"><a name="p16621164742519"></a><a name="p16621164742519"></a>DCMI_COMPONENT_TYPE_TEE,</p>
<p id="p18621124712515"><a name="p18621124712515"></a><a name="p18621124712515"></a>DCMI_COMPONENT_TYPE_KERNEL,</p>
<p id="p26211147192517"><a name="p26211147192517"></a><a name="p26211147192517"></a>DCMI_COMPONENT_TYPE_DTB,</p>
<p id="p12621144718257"><a name="p12621144718257"></a><a name="p12621144718257"></a>DCMI_COMPONENT_TYPE_ROOTFS,</p>
<p id="p1462184720251"><a name="p1462184720251"></a><a name="p1462184720251"></a>DCMI_COMPONENT_TYPE_IMU,</p>
<p id="p0621164712257"><a name="p0621164712257"></a><a name="p0621164712257"></a>DCMI_COMPONENT_TYPE_IMP,</p>
<p id="p1762115479251"><a name="p1762115479251"></a><a name="p1762115479251"></a>DCMI_COMPONENT_TYPE_AICPU,</p>
<p id="p18621104762519"><a name="p18621104762519"></a><a name="p18621104762519"></a>DCMI_COMPONENT_TYPE_HBOOT1_A,</p>
<p id="p1621124772518"><a name="p1621124772518"></a><a name="p1621124772518"></a>DCMI_COMPONENT_TYPE_HBOOT1_B,</p>
<p id="p1962124742516"><a name="p1962124742516"></a><a name="p1962124742516"></a>DCMI_COMPONENT_TYPE_HBOOT2,</p>
<p id="p862112477254"><a name="p862112477254"></a><a name="p862112477254"></a>DCMI_COMPONENT_TYPE_DDR,</p>
<p id="p17621747152514"><a name="p17621747152514"></a><a name="p17621747152514"></a>DCMI_COMPONENT_TYPE_LP,</p>
<p id="p17621104702511"><a name="p17621104702511"></a><a name="p17621104702511"></a>DCMI_COMPONENT_TYPE_HSM,</p>
<p id="p662112479252"><a name="p662112479252"></a><a name="p662112479252"></a>DCMI_COMPONENT_TYPE_SAFETY_ISLAND,</p>
<p id="p11621347112510"><a name="p11621347112510"></a><a name="p11621347112510"></a>DCMI_COMPONENT_TYPE_HILINK,</p>
<p id="p156219475255"><a name="p156219475255"></a><a name="p156219475255"></a>DCMI_COMPONENT_TYPE_RAWDATA,</p>
<p id="p10621164719256"><a name="p10621164719256"></a><a name="p10621164719256"></a>DCMI_COMPONENT_TYPE_SYSDRV,</p>
<p id="p156211547172519"><a name="p156211547172519"></a><a name="p156211547172519"></a>DCMI_COMPONENT_TYPE_ADSAPP,</p>
<p id="p18621847112518"><a name="p18621847112518"></a><a name="p18621847112518"></a>DCMI_COMPONENT_TYPE_COMISOLATOR,</p>
<p id="p66211247122511"><a name="p66211247122511"></a><a name="p66211247122511"></a>DCMI_COMPONENT_TYPE_CLUSTER,</p>
<p id="p1462117475259"><a name="p1462117475259"></a><a name="p1462117475259"></a>DCMI_COMPONENT_TYPE_CUSTOMIZED,</p>
<p id="p26216475253"><a name="p26216475253"></a><a name="p26216475253"></a>DCMI_COMPONENT_TYPE_SYS_BASE_CONFIG,</p>
<p id="p36211047122514"><a name="p36211047122514"></a><a name="p36211047122514"></a>DCMI_COMPONENT_TYPE_RECOVERY,</p>
<p id="p962194732515"><a name="p962194732515"></a><a name="p962194732515"></a>DCMI_COMPONENT_TYPE_HILINK2,</p>
<p id="p196211447142518"><a name="p196211447142518"></a><a name="p196211447142518"></a>DCMI_COMPONENT_TYPE_LOGIC_BIST,</p>
<p id="p186219475253"><a name="p186219475253"></a><a name="p186219475253"></a>DCMI_COMPONENT_TYPE_MEMORY_BIST,</p>
<p id="p36211347192511"><a name="p36211347192511"></a><a name="p36211347192511"></a>DCMI_COMPONENT_TYPE_ATF,</p>
<p id="p66211647132518"><a name="p66211647132518"></a><a name="p66211647132518"></a>DCMI_COMPONENT_TYPE_USER_BASE_CONFIG,</p>
<p id="p14621184752513"><a name="p14621184752513"></a><a name="p14621184752513"></a>DCMI_COMPONENT_TYPE_BOOTROM,</p>
<p id="p11621194711259"><a name="p11621194711259"></a><a name="p11621194711259"></a>DCMI_COMPONENT_TYPE_MAX,</p>
<p id="p3621104752520"><a name="p3621104752520"></a><a name="p3621104752520"></a>DCMI_UPGRADE_AND_RESET_ALL_COMPONENT = 0xFFFFFFF7,</p>
<p id="p1862115472258"><a name="p1862115472258"></a><a name="p1862115472258"></a>DCMI_UPGRADE_ALL_IMAGE_COMPONENT = 0xFFFFFFFD,</p>
<p id="p136211476254"><a name="p136211476254"></a><a name="p136211476254"></a>DCMI_UPGRADE_ALL_FIRMWARE_COMPONENT = 0xFFFFFFFE,</p>
<p id="p15621747102519"><a name="p15621747102519"></a><a name="p15621747102519"></a>DCMI_UPGRADE_ALL_COMPONENT = 0xFFFFFFFF</p>
<p id="p162194716251"><a name="p162194716251"></a><a name="p162194716251"></a>};</p>
<p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1272985332918"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1272985332918"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1272985332918"></a>当前支持DCMI_COMPONENT_TYPE_HBOOT1_A、DCMI_COMPONENT_TYPE_HBOOT1_B、DCMI_COMPONENT_TYPE_HILINK、DCMI_COMPONENT_TYPE_HILINK2。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_row19217191855315"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1221710182537"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1221710182537"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p1221710182537"></a>version_str</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p921717182538"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p921717182538"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p921717182538"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="15.879999999999999%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p92171618185316"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p92171618185316"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p92171618185316"></a>unsigned char *</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p13217121855318"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p13217121855318"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p13217121855318"></a>用户申请的空间，存放返回的固件版本号。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_row5220616145310"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p102211216115314"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p102211216115314"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p102211216115314"></a>len</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p8221171625318"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p8221171625318"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p8221171625318"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.879999999999999%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p19221111619530"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p19221111619530"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p19221111619530"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p2022171620532"><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p2022171620532"></a><a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_p2022171620532"></a>version_str的内存大小，大小不能小于64Byte。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000001917887173_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row59025204"><th class="cellrowborder" valign="top" width="24.18%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="75.82%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row13362997"><td class="cellrowborder" valign="top" width="24.18%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="75.82%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a>处理结果：</p>
<a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><ul id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"><li>成功：返回0。</li><li>失败：返回码请参见<a href="return_codes.md">return_codes</a>。</li></ul>
</td>
</tr>
</tbody>
</table>

**异常处理<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_toc533412081"></a>**

无。

**约束说明<a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_toc533412082"></a>**

该接口需调入TEEOS，耗时较长，不支持在接口调用时触发休眠唤醒，如果触发休眠，有较大可能造成休眠失败。

**表 1** 不同部署场景下的支持情况

<a name="table36641225112113"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row11448141417420"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p1713218513317"><a name="zh-cn_topic_0000002485295476_p1713218513317"></a><a name="zh-cn_topic_0000002485295476_p1713218513317"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p213245123114"><a name="zh-cn_topic_0000002485295476_p213245123114"></a><a name="zh-cn_topic_0000002485295476_p213245123114"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="24.97%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p13132115113113"><a name="zh-cn_topic_0000002485295476_p13132115113113"></a><a name="zh-cn_topic_0000002485295476_p13132115113113"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25.03%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p8132185173115"><a name="zh-cn_topic_0000002485295476_p8132185173115"></a><a name="zh-cn_topic_0000002485295476_p8132185173115"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_row728313421182"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p13757328759"><a name="zh-cn_topic_0000002485295476_p13757328759"></a><a name="zh-cn_topic_0000002485295476_p13757328759"></a><span id="zh-cn_topic_0000002485295476_ph375715283511"><a name="zh-cn_topic_0000002485295476_ph375715283511"></a><a name="zh-cn_topic_0000002485295476_ph375715283511"></a><span id="zh-cn_topic_0000002485295476_text147574281354"><a name="zh-cn_topic_0000002485295476_text147574281354"></a><a name="zh-cn_topic_0000002485295476_text147574281354"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p11366163913135"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p11366163913135"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p11366163913135"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p14366113915138"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p14366113915138"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p14366113915138"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p8366173991316"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p8366173991316"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p8366173991316"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row12848477419"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p47571728152"><a name="zh-cn_topic_0000002485295476_p47571728152"></a><a name="zh-cn_topic_0000002485295476_p47571728152"></a><span id="zh-cn_topic_0000002485295476_text57571728359"><a name="zh-cn_topic_0000002485295476_text57571728359"></a><a name="zh-cn_topic_0000002485295476_text57571728359"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p28504223417"><a name="zh-cn_topic_0000002485295476_p28504223417"></a><a name="zh-cn_topic_0000002485295476_p28504223417"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p17850172220413"><a name="zh-cn_topic_0000002485295476_p17850172220413"></a><a name="zh-cn_topic_0000002485295476_p17850172220413"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1385072213414"><a name="zh-cn_topic_0000002485295476_p1385072213414"></a><a name="zh-cn_topic_0000002485295476_p1385072213414"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row118411919445"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p177571128653"><a name="zh-cn_topic_0000002485295476_p177571128653"></a><a name="zh-cn_topic_0000002485295476_p177571128653"></a><span id="zh-cn_topic_0000002485295476_text15757628450"><a name="zh-cn_topic_0000002485295476_text15757628450"></a><a name="zh-cn_topic_0000002485295476_text15757628450"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p15469122314417"><a name="zh-cn_topic_0000002485295476_p15469122314417"></a><a name="zh-cn_topic_0000002485295476_p15469122314417"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p74698234414"><a name="zh-cn_topic_0000002485295476_p74698234414"></a><a name="zh-cn_topic_0000002485295476_p74698234414"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p346911236418"><a name="zh-cn_topic_0000002485295476_p346911236418"></a><a name="zh-cn_topic_0000002485295476_p346911236418"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1888071710410"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p375752819516"><a name="zh-cn_topic_0000002485295476_p375752819516"></a><a name="zh-cn_topic_0000002485295476_p375752819516"></a><span id="zh-cn_topic_0000002485295476_text157571128457"><a name="zh-cn_topic_0000002485295476_text157571128457"></a><a name="zh-cn_topic_0000002485295476_text157571128457"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1211817247419"><a name="zh-cn_topic_0000002485295476_p1211817247419"></a><a name="zh-cn_topic_0000002485295476_p1211817247419"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15118112412418"><a name="zh-cn_topic_0000002485295476_p15118112412418"></a><a name="zh-cn_topic_0000002485295476_p15118112412418"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p71183241449"><a name="zh-cn_topic_0000002485295476_p71183241449"></a><a name="zh-cn_topic_0000002485295476_p71183241449"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1276811111414"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p107581228856"><a name="zh-cn_topic_0000002485295476_p107581228856"></a><a name="zh-cn_topic_0000002485295476_p107581228856"></a><span id="zh-cn_topic_0000002485295476_text37581282519"><a name="zh-cn_topic_0000002485295476_text37581282519"></a><a name="zh-cn_topic_0000002485295476_text37581282519"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p120113251649"><a name="zh-cn_topic_0000002485295476_p120113251649"></a><a name="zh-cn_topic_0000002485295476_p120113251649"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p620114251847"><a name="zh-cn_topic_0000002485295476_p620114251847"></a><a name="zh-cn_topic_0000002485295476_p620114251847"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1120117251241"><a name="zh-cn_topic_0000002485295476_p1120117251241"></a><a name="zh-cn_topic_0000002485295476_p1120117251241"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row262419543"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p19758182817512"><a name="zh-cn_topic_0000002485295476_p19758182817512"></a><a name="zh-cn_topic_0000002485295476_p19758182817512"></a><span id="zh-cn_topic_0000002485295476_text7758828253"><a name="zh-cn_topic_0000002485295476_text7758828253"></a><a name="zh-cn_topic_0000002485295476_text7758828253"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p59851728544"><a name="zh-cn_topic_0000002485295476_p59851728544"></a><a name="zh-cn_topic_0000002485295476_p59851728544"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p11985828244"><a name="zh-cn_topic_0000002485295476_p11985828244"></a><a name="zh-cn_topic_0000002485295476_p11985828244"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p169851228148"><a name="zh-cn_topic_0000002485295476_p169851228148"></a><a name="zh-cn_topic_0000002485295476_p169851228148"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1994411513415"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p97581828659"><a name="zh-cn_topic_0000002485295476_p97581828659"></a><a name="zh-cn_topic_0000002485295476_p97581828659"></a><span id="zh-cn_topic_0000002485295476_text20758132816511"><a name="zh-cn_topic_0000002485295476_text20758132816511"></a><a name="zh-cn_topic_0000002485295476_text20758132816511"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p16036297418"><a name="zh-cn_topic_0000002485295476_p16036297418"></a><a name="zh-cn_topic_0000002485295476_p16036297418"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p8603182911412"><a name="zh-cn_topic_0000002485295476_p8603182911412"></a><a name="zh-cn_topic_0000002485295476_p8603182911412"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p96032298411"><a name="zh-cn_topic_0000002485295476_p96032298411"></a><a name="zh-cn_topic_0000002485295476_p96032298411"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row9870431183314"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p242563973319"><a name="zh-cn_topic_0000002485295476_p242563973319"></a><a name="zh-cn_topic_0000002485295476_p242563973319"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251227167_zh-cn_topic_0000001223414453_zh-cn_topic_0000001148137079_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
unsigned char version_str[64] = {0};
ret = dcmi_get_device_component_static_version(card_id, device_id, DCMI_COMPONENT_TYPE_NVE,version_str, 64);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

