# dcmi\_get\_device\_flash\_info<a name="ZH-CN_TOPIC_0000002485295432"></a>

**函数原型<a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_toc533412077"></a>**

**int dcmi\_get\_device\_flash\_info\(int card\_id, int device\_id, unsigned int flash\_index, struct dcmi\_flash\_info\_stru \*flash\_info\)**

**功能说明<a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_toc533412078"></a>**

获取芯片内Flash的信息。

**参数说明<a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p10021890"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p10021890"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p6466753"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p6466753"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.96%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p54045009"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p54045009"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.019999999999996%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p15569626"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p15569626"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p36741947142813"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p36741947142813"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p96741747122818"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p96741747122818"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p46747472287"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p46747472287"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1467413474281"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1467413474281"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p7711145152918"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p7711145152918"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p671116522914"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p671116522914"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1771116572910"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1771116572910"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1633472511112"><a name="p1633472511112"></a><a name="p1633472511112"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1644818114917"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1644818114917"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1644818114917"></a>flash_index</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1144801174915"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1144801174915"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1144801174915"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p144812114911"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p144812114911"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p144812114911"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p194482014496"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p194482014496"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p194482014496"></a>Flash索引号，通过dcmi_get_device_flash_count获取。取值范围：[0, flash_count-1]</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_row257814447482"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p12448316499"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p12448316499"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p12448316499"></a>flash_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p444801114918"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p444801114918"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p444801114918"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1644810194914"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1644810194914"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1644810194914"></a>struct dcmi_flash_info_stru *</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p94481112494"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p94481112494"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p94481112494"></a>返回Flash信息。</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p144891114916"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p144891114916"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p144891114916"></a>Flash信息结构体定义：</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p610011201252"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p610011201252"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p610011201252"></a>struct dcmi_flash_info_stru {</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p31003206516"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p31003206516"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p31003206516"></a>unsigned long long flash_id; //Flash_id</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1910012201057"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1910012201057"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1910012201057"></a>unsigned short device_id; //设备ID</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p17100620256"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p17100620256"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p17100620256"></a>unsigned short vendor;  //厂商ID</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1197234302214"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1197234302214"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1197234302214"></a>unsigned int state;  //state=0时，表示flash正常，state≠0时，表示flash异常</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p131000201051"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p131000201051"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p131000201051"></a>unsigned long long size; //Flash的总大小</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p4100182020514"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p4100182020514"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p4100182020514"></a>unsigned int sector_count; //擦除单元数量</p>
<p id="p411852354319"><a name="p411852354319"></a><a name="p411852354319"></a>unsigned short manufacturer_id; //制造商ID</p>
<p id="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1910013201758"><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1910013201758"></a><a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_p1910013201758"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_toc533412082"></a>**

该接口在后续版本将会删除，推荐使用[dcmi\_get\_device\_flash\_info\_v2](dcmi_get_device_flash_info_v2.md)。

**表 1** 不同部署场景下的支持情况

<a name="table1113417173519"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1548132517501"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p16154843125019"><a name="zh-cn_topic_0000002485295476_p16154843125019"></a><a name="zh-cn_topic_0000002485295476_p16154843125019"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p3123182915595"><a name="zh-cn_topic_0000002485295476_p3123182915595"></a><a name="zh-cn_topic_0000002485295476_p3123182915595"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p6271950600"><a name="zh-cn_topic_0000002485295476_p6271950600"></a><a name="zh-cn_topic_0000002485295476_p6271950600"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p41541743195013"><a name="zh-cn_topic_0000002485295476_p41541743195013"></a><a name="zh-cn_topic_0000002485295476_p41541743195013"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p19444162914516"><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><span id="zh-cn_topic_0000002485295476_ph1944412296514"><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><span id="zh-cn_topic_0000002485295476_text944432913516"><a name="zh-cn_topic_0000002485295476_text944432913516"></a><a name="zh-cn_topic_0000002485295476_text944432913516"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row6920114611302"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p194441629165117"><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><span id="zh-cn_topic_0000002485295476_text124449291511"><a name="zh-cn_topic_0000002485295476_text124449291511"></a><a name="zh-cn_topic_0000002485295476_text124449291511"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p461320458219"><a name="zh-cn_topic_0000002485295476_p461320458219"></a><a name="zh-cn_topic_0000002485295476_p461320458219"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13613134519220"><a name="zh-cn_topic_0000002485295476_p13613134519220"></a><a name="zh-cn_topic_0000002485295476_p13613134519220"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p96133454211"><a name="zh-cn_topic_0000002485295476_p96133454211"></a><a name="zh-cn_topic_0000002485295476_p96133454211"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row14873174604910"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p10444329155118"><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><span id="zh-cn_topic_0000002485295476_text1044482925119"><a name="zh-cn_topic_0000002485295476_text1044482925119"></a><a name="zh-cn_topic_0000002485295476_text1044482925119"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p896391111511"><a name="zh-cn_topic_0000002485295476_p896391111511"></a><a name="zh-cn_topic_0000002485295476_p896391111511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p119682011105113"><a name="zh-cn_topic_0000002485295476_p119682011105113"></a><a name="zh-cn_topic_0000002485295476_p119682011105113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p11975911195114"><a name="zh-cn_topic_0000002485295476_p11975911195114"></a><a name="zh-cn_topic_0000002485295476_p11975911195114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row23051923114915"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p174441129175117"><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><span id="zh-cn_topic_0000002485295476_text34441729175119"><a name="zh-cn_topic_0000002485295476_text34441729175119"></a><a name="zh-cn_topic_0000002485295476_text34441729175119"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1298231135114"><a name="zh-cn_topic_0000002485295476_p1298231135114"></a><a name="zh-cn_topic_0000002485295476_p1298231135114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1298921117511"><a name="zh-cn_topic_0000002485295476_p1298921117511"></a><a name="zh-cn_topic_0000002485295476_p1298921117511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1799561165114"><a name="zh-cn_topic_0000002485295476_p1799561165114"></a><a name="zh-cn_topic_0000002485295476_p1799561165114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row44814564912"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1044414295516"><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><span id="zh-cn_topic_0000002485295476_text44441829105114"><a name="zh-cn_topic_0000002485295476_text44441829105114"></a><a name="zh-cn_topic_0000002485295476_text44441829105114"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p17181285116"><a name="zh-cn_topic_0000002485295476_p17181285116"></a><a name="zh-cn_topic_0000002485295476_p17181285116"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15131255114"><a name="zh-cn_topic_0000002485295476_p15131255114"></a><a name="zh-cn_topic_0000002485295476_p15131255114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15917127513"><a name="zh-cn_topic_0000002485295476_p15917127513"></a><a name="zh-cn_topic_0000002485295476_p15917127513"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row11171321114917"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p94441829165113"><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><span id="zh-cn_topic_0000002485295476_text644410297515"><a name="zh-cn_topic_0000002485295476_text644410297515"></a><a name="zh-cn_topic_0000002485295476_text644410297515"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p01211275113"><a name="zh-cn_topic_0000002485295476_p01211275113"></a><a name="zh-cn_topic_0000002485295476_p01211275113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p916161212512"><a name="zh-cn_topic_0000002485295476_p916161212512"></a><a name="zh-cn_topic_0000002485295476_p916161212512"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p191911211511"><a name="zh-cn_topic_0000002485295476_p191911211511"></a><a name="zh-cn_topic_0000002485295476_p191911211511"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row18385115320492"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1644572918513"><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><span id="zh-cn_topic_0000002485295476_text1744532925112"><a name="zh-cn_topic_0000002485295476_text1744532925112"></a><a name="zh-cn_topic_0000002485295476_text1744532925112"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1923112115119"><a name="zh-cn_topic_0000002485295476_p1923112115119"></a><a name="zh-cn_topic_0000002485295476_p1923112115119"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1269122515"><a name="zh-cn_topic_0000002485295476_p1269122515"></a><a name="zh-cn_topic_0000002485295476_p1269122515"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p43014126516"><a name="zh-cn_topic_0000002485295476_p43014126516"></a><a name="zh-cn_topic_0000002485295476_p43014126516"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row557915117191"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p2042757191916"><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><span id="zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206467206_zh-cn_topic_0000001178213222_zh-cn_topic_0000001148639169_toc533412083"></a>**

```
… 
int i;
int ret = 0;
int card_id = 0;
int device_id = 0;
struct dcmi_flash_info_stru flash_info = {0};
unsigned int flash_count = 0;
ret = dcmi_get_device_flash_count(card_id, device_id, &flash_count);
… 
For (i = 0; i < flash_count; i++){
    ret = dcmi_get_device_flash_info(card_id, device_id, i, &flash_info);
    if (ret != 0){
        //todo：记录日志
        return ret;
    } 
… 
}
…
```

