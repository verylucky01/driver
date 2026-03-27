# dcmi\_get\_device\_component\_list<a name="ZH-CN_TOPIC_0000002517558643"></a>

**函数原型<a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_toc533412077"></a>**

**int dcmi\_get\_device\_component\_list\(int card\_id, int device\_id, enum dcmi\_component\_type \*component\_table, unsigned int component\_count\)**

**功能说明<a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_toc533412078"></a>**

获取可升级组件列表，不包含recovery组件。

**参数说明<a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_toc533412079"></a>**

<a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p10021890"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p10021890"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17.130000000000003%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p6466753"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p6466753"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="15.870000000000001%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p54045009"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p54045009"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p15569626"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p15569626"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p36741947142813"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p36741947142813"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17.130000000000003%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p96741747122818"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p96741747122818"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.870000000000001%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p46747472287"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p46747472287"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p1467413474281"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p1467413474281"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p7711145152918"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p7711145152918"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17.130000000000003%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p671116522914"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p671116522914"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.870000000000001%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p1771116572910"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p1771116572910"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p87585814613"><a name="p87585814613"></a><a name="p87585814613"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p5522164215178"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p5522164215178"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p5522164215178"></a>component_table</p>
</td>
<td class="cellrowborder" valign="top" width="17.130000000000003%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p8522242101715"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p8522242101715"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p8522242101715"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="15.870000000000001%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p17522114220174"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p17522114220174"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p17522114220174"></a>enum dcmi_component_type *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p0522164231718"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p0522164231718"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p0522164231718"></a>返回可升级组件列表，具体值如下：</p>
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
<p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p4384142910594"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p4384142910594"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p4384142910594"></a>当前支持DCMI_COMPONENT_TYPE_HBOOT1_A、DCMI_COMPONENT_TYPE_HBOOT1_B、DCMI_COMPONENT_TYPE_HILINK、DCMI_COMPONENT_TYPE_HILINK2。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_row1056131515384"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p6574157389"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p6574157389"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p6574157389"></a>component_count</p>
</td>
<td class="cellrowborder" valign="top" width="17.130000000000003%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p75711152388"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p75711152388"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p75711152388"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.870000000000001%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p457111543818"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p457111543818"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p457111543818"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p145771510381"><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p145771510381"></a><a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_p145771510381"></a>“component_table”数组的长度，表示获取的组件个数。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row59025204"><th class="cellrowborder" valign="top" width="24.18%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="75.82%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row13362997"><td class="cellrowborder" valign="top" width="24.18%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="75.82%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a>处理结果：</p>
<a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><ul id="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"><li>成功：返回0。</li><li>失败：返回码请参见<a href="return_codes.md">return_codes</a>。</li></ul>
</td>
</tr>
</tbody>
</table>

**异常处理<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_toc533412081"></a>**

无。

**约束说明<a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table18698834121214"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row18113171210"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p9380249113917"><a name="zh-cn_topic_0000002485318818_p9380249113917"></a><a name="zh-cn_topic_0000002485318818_p9380249113917"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p13380749113915"><a name="zh-cn_topic_0000002485318818_p13380749113915"></a><a name="zh-cn_topic_0000002485318818_p13380749113915"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p10380349173915"><a name="zh-cn_topic_0000002485318818_p10380349173915"></a><a name="zh-cn_topic_0000002485318818_p10380349173915"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p03801649133914"><a name="zh-cn_topic_0000002485318818_p03801649133914"></a><a name="zh-cn_topic_0000002485318818_p03801649133914"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row38243131214"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p48210361212"><a name="zh-cn_topic_0000002485318818_p48210361212"></a><a name="zh-cn_topic_0000002485318818_p48210361212"></a><span id="zh-cn_topic_0000002485318818_text16821238124"><a name="zh-cn_topic_0000002485318818_text16821238124"></a><a name="zh-cn_topic_0000002485318818_text16821238124"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1990911717125"><a name="zh-cn_topic_0000002485318818_p1990911717125"></a><a name="zh-cn_topic_0000002485318818_p1990911717125"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1590914714124"><a name="zh-cn_topic_0000002485318818_p1590914714124"></a><a name="zh-cn_topic_0000002485318818_p1590914714124"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p9909273125"><a name="zh-cn_topic_0000002485318818_p9909273125"></a><a name="zh-cn_topic_0000002485318818_p9909273125"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row982437122"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p4829381220"><a name="zh-cn_topic_0000002485318818_p4829381220"></a><a name="zh-cn_topic_0000002485318818_p4829381220"></a><span id="zh-cn_topic_0000002485318818_text582173201218"><a name="zh-cn_topic_0000002485318818_text582173201218"></a><a name="zh-cn_topic_0000002485318818_text582173201218"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p59098719121"><a name="zh-cn_topic_0000002485318818_p59098719121"></a><a name="zh-cn_topic_0000002485318818_p59098719121"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p20909197161211"><a name="zh-cn_topic_0000002485318818_p20909197161211"></a><a name="zh-cn_topic_0000002485318818_p20909197161211"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p199091074123"><a name="zh-cn_topic_0000002485318818_p199091074123"></a><a name="zh-cn_topic_0000002485318818_p199091074123"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1782915383548"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p988163171613"><a name="zh-cn_topic_0000002485318818_p988163171613"></a><a name="zh-cn_topic_0000002485318818_p988163171613"></a><span id="zh-cn_topic_0000002485318818_text28812351611"><a name="zh-cn_topic_0000002485318818_text28812351611"></a><a name="zh-cn_topic_0000002485318818_text28812351611"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p107731653135415"><a name="zh-cn_topic_0000002485318818_p107731653135415"></a><a name="zh-cn_topic_0000002485318818_p107731653135415"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p77730531545"><a name="zh-cn_topic_0000002485318818_p77730531545"></a><a name="zh-cn_topic_0000002485318818_p77730531545"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p8773153175410"><a name="zh-cn_topic_0000002485318818_p8773153175410"></a><a name="zh-cn_topic_0000002485318818_p8773153175410"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row19683944122816"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p19717154811286"><a name="zh-cn_topic_0000002485318818_p19717154811286"></a><a name="zh-cn_topic_0000002485318818_p19717154811286"></a><span id="zh-cn_topic_0000002485318818_text5717204812817"><a name="zh-cn_topic_0000002485318818_text5717204812817"></a><a name="zh-cn_topic_0000002485318818_text5717204812817"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p13998205142815"><a name="zh-cn_topic_0000002485318818_p13998205142815"></a><a name="zh-cn_topic_0000002485318818_p13998205142815"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p49983512287"><a name="zh-cn_topic_0000002485318818_p49983512287"></a><a name="zh-cn_topic_0000002485318818_p49983512287"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p89988518280"><a name="zh-cn_topic_0000002485318818_p89988518280"></a><a name="zh-cn_topic_0000002485318818_p89988518280"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row06711416544"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1765294655411"><a name="zh-cn_topic_0000002485318818_p1765294655411"></a><a name="zh-cn_topic_0000002485318818_p1765294655411"></a><span id="zh-cn_topic_0000002485318818_text19652946185414"><a name="zh-cn_topic_0000002485318818_text19652946185414"></a><a name="zh-cn_topic_0000002485318818_text19652946185414"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p428811549547"><a name="zh-cn_topic_0000002485318818_p428811549547"></a><a name="zh-cn_topic_0000002485318818_p428811549547"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p15288954155420"><a name="zh-cn_topic_0000002485318818_p15288954155420"></a><a name="zh-cn_topic_0000002485318818_p15288954155420"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p192881754115412"><a name="zh-cn_topic_0000002485318818_p192881754115412"></a><a name="zh-cn_topic_0000002485318818_p192881754115412"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row13182101810819"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p104761521386"><a name="zh-cn_topic_0000002485318818_p104761521386"></a><a name="zh-cn_topic_0000002485318818_p104761521386"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251307179_zh-cn_topic_0000001223172965_zh-cn_topic_0000001101337144_toc533412083"></a>**

```
…
int ret = 0;
int card_id = 0;
int device_id = 0;
unsigned int component_num = 0;
enum dcmi_component_type *component_table = NULL;

ret = dcmi_get_device_component_count(card_id, device_id, &component_num);
if (ret != 0) {
    // todo：记录日志
    return ret;
}

component_table = (enum dcmi_component_type *)malloc(sizeof(enum dcmi_component_type) * component_num);
if (component_table == NULL) {
    // todo：记录日志
    return ret;
}

ret = dcmi_get_device_component_list(card_id, device_id, component_table, component_num);
if (ret != 0) {
    // todo：记录日志
    free(component_table);
    return ret;
}
…
```

