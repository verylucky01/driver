# dcmi\_set\_device\_reset<a name="ZH-CN_TOPIC_0000002517535387"></a>

**函数原型<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412077"></a>**

**int dcmi\_set\_device\_reset\(int card\_id, int device\_id, enum dcmi\_reset\_channel channel\_type\)**

**功能说明<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412078"></a>**

复位芯片。

**参数说明<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p10021890"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p10021890"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.99%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p6466753"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p6466753"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.009999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p54045009"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p54045009"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p15569626"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p15569626"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row5908907"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p36741947142813"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p36741947142813"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p96741747122818"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p96741747122818"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p46747472287"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p46747472287"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1467413474281"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1467413474281"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row45631627"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p7711145152918"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p7711145152918"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p671116522914"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p671116522914"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1771116572910"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1771116572910"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p10757141201716"><a name="p10757141201716"></a><a name="p10757141201716"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1662044105817"><a name="p1662044105817"></a><a name="p1662044105817"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row137415397316"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1622474112317"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1622474112317"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1622474112317"></a>channel_type</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p19225441193116"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p19225441193116"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p19225441193116"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p102251441133110"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p102251441133110"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p102251441133110"></a>enum dcmi_reset_channel</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3511101013286"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3511101013286"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3511101013286"></a>复位通道。</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p136522222313"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p136522222313"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p136522222313"></a>enum dcmi_reset_channel {</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p365822172318"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p365822172318"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p365822172318"></a>OUTBAND_CHANNEL = 0, // 带外复位</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3662222235"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3662222235"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3662222235"></a>INBAND_CHANNEL  //带内复位</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1066112202319"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1066112202319"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1066112202319"></a>}</p>
<p id="p421168115817"><a name="p421168115817"></a><a name="p421168115817"></a></p>
<div class="note" id="note20188544163013"><a name="note20188544163013"></a><a name="note20188544163013"></a><span class="notetitle"> 说明： </span><div class="notebody"><a name="ul19530122133511"></a><a name="ul19530122133511"></a><ul id="ul19530122133511"><li><span id="text1411326925"><a name="text1411326925"></a><a name="text1411326925"></a>Atlas 200T A2 Box16 异构子框</span>、<span id="zh-cn_topic_0288842938_ph1246074112311"><a name="zh-cn_topic_0288842938_ph1246074112311"></a><a name="zh-cn_topic_0288842938_ph1246074112311"></a><span id="text78907496515"><a name="text78907496515"></a><a name="text78907496515"></a>Atlas 900 A2 PoD 集群基础单元</span></span>支持INBAND_CHANNEL。</li><li><span id="text1087997194719"><a name="text1087997194719"></a><a name="text1087997194719"></a>Atlas 300I A2 推理卡</span>、<span id="text1627131293511"><a name="text1627131293511"></a><a name="text1627131293511"></a>Atlas 300T A2 训练卡</span>支持OUTBAND_CHANNEL。</li></ul>
</div></div>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**约束说明<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412082"></a>**

-   带外标卡复位功能依赖ipmitool软件，需要提前下载并加载驱动。详细操作请参见\[faq\]\(faq.md\)章节的“准备ipmitool软件”。

-   Atlas 300I A2 推理卡、Atlas 300T A2 训练卡需要调用[dcmi\_set\_device\_pre\_reset](dcmi_set_device_pre_reset.md)预复位芯片并返回成功后，才能调用该接口进行带外复位。
-   对于Atlas 900 A2 PoD 集群基础单元和Atlas 200T A2 Box16 异构子框，正常启动的NPU不支持单NPU复位。若同一设备上的多个NPU中部分降P启动，部分正常启动，则降P启动的NPU支持单NPU复位；正常启动的NPU不支持单NPU复位，即执行单NPU复位命令时所有NPU都将被复位，包括被降P启动的NPU。查询指定NPU是否为降P启动的方法请参见《Atlas A2 中心推理和训练硬件 25.5.0 npu-smi 命令参考》中的“查询指定NPU是否为降P启动”章节，对指定NPU进行降P启动的方法请参见《Atlas A2 中心推理和训练硬件 25.5.0 npu-smi 命令参考》中的“对指定NPU进行降P启动”章节。
-   该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table6665182042413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row192401338610"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p6884135713319"><a name="zh-cn_topic_0000002485295476_p6884135713319"></a><a name="zh-cn_topic_0000002485295476_p6884135713319"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p188841657113119"><a name="zh-cn_topic_0000002485295476_p188841657113119"></a><a name="zh-cn_topic_0000002485295476_p188841657113119"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p198849575317"><a name="zh-cn_topic_0000002485295476_p198849575317"></a><a name="zh-cn_topic_0000002485295476_p198849575317"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p288445716317"><a name="zh-cn_topic_0000002485295476_p288445716317"></a><a name="zh-cn_topic_0000002485295476_p288445716317"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_row82952324359"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1759118207718"><a name="zh-cn_topic_0000002485295476_p1759118207718"></a><a name="zh-cn_topic_0000002485295476_p1759118207718"></a><span id="zh-cn_topic_0000002485295476_ph05911020372"><a name="zh-cn_topic_0000002485295476_ph05911020372"></a><a name="zh-cn_topic_0000002485295476_ph05911020372"></a><span id="zh-cn_topic_0000002485295476_text12591192010713"><a name="zh-cn_topic_0000002485295476_text12591192010713"></a><a name="zh-cn_topic_0000002485295476_text12591192010713"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1018612250597"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1018612250597"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1018612250597"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p16903175117312"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p16903175117312"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p16903175117312"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p89579531835"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p89579531835"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p89579531835"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row72645420615"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p95911420177"><a name="zh-cn_topic_0000002485295476_p95911420177"></a><a name="zh-cn_topic_0000002485295476_p95911420177"></a><span id="zh-cn_topic_0000002485295476_text6591220876"><a name="zh-cn_topic_0000002485295476_text6591220876"></a><a name="zh-cn_topic_0000002485295476_text6591220876"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p291810521262"><a name="zh-cn_topic_0000002485295476_p291810521262"></a><a name="zh-cn_topic_0000002485295476_p291810521262"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p199181852061"><a name="zh-cn_topic_0000002485295476_p199181852061"></a><a name="zh-cn_topic_0000002485295476_p199181852061"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p89189521765"><a name="zh-cn_topic_0000002485295476_p89189521765"></a><a name="zh-cn_topic_0000002485295476_p89189521765"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row278413126616"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p559162010720"><a name="zh-cn_topic_0000002485295476_p559162010720"></a><a name="zh-cn_topic_0000002485295476_p559162010720"></a><span id="zh-cn_topic_0000002485295476_text165912204716"><a name="zh-cn_topic_0000002485295476_text165912204716"></a><a name="zh-cn_topic_0000002485295476_text165912204716"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1852110531661"><a name="zh-cn_topic_0000002485295476_p1852110531661"></a><a name="zh-cn_topic_0000002485295476_p1852110531661"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1252125311612"><a name="zh-cn_topic_0000002485295476_p1252125311612"></a><a name="zh-cn_topic_0000002485295476_p1252125311612"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p652117531767"><a name="zh-cn_topic_0000002485295476_p652117531767"></a><a name="zh-cn_topic_0000002485295476_p652117531767"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row878911101267"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p259119201579"><a name="zh-cn_topic_0000002485295476_p259119201579"></a><a name="zh-cn_topic_0000002485295476_p259119201579"></a><span id="zh-cn_topic_0000002485295476_text55915207713"><a name="zh-cn_topic_0000002485295476_text55915207713"></a><a name="zh-cn_topic_0000002485295476_text55915207713"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p18619541161"><a name="zh-cn_topic_0000002485295476_p18619541161"></a><a name="zh-cn_topic_0000002485295476_p18619541161"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p2864541766"><a name="zh-cn_topic_0000002485295476_p2864541766"></a><a name="zh-cn_topic_0000002485295476_p2864541766"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1186854962"><a name="zh-cn_topic_0000002485295476_p1186854962"></a><a name="zh-cn_topic_0000002485295476_p1186854962"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row283215811614"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p12591202014711"><a name="zh-cn_topic_0000002485295476_p12591202014711"></a><a name="zh-cn_topic_0000002485295476_p12591202014711"></a><span id="zh-cn_topic_0000002485295476_text65911120871"><a name="zh-cn_topic_0000002485295476_text65911120871"></a><a name="zh-cn_topic_0000002485295476_text65911120871"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p15638154267"><a name="zh-cn_topic_0000002485295476_p15638154267"></a><a name="zh-cn_topic_0000002485295476_p15638154267"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p18638354369"><a name="zh-cn_topic_0000002485295476_p18638354369"></a><a name="zh-cn_topic_0000002485295476_p18638354369"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p16638154060"><a name="zh-cn_topic_0000002485295476_p16638154060"></a><a name="zh-cn_topic_0000002485295476_p16638154060"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1057696667"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p959110201477"><a name="zh-cn_topic_0000002485295476_p959110201477"></a><a name="zh-cn_topic_0000002485295476_p959110201477"></a><span id="zh-cn_topic_0000002485295476_text35912020471"><a name="zh-cn_topic_0000002485295476_text35912020471"></a><a name="zh-cn_topic_0000002485295476_text35912020471"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p18230255869"><a name="zh-cn_topic_0000002485295476_p18230255869"></a><a name="zh-cn_topic_0000002485295476_p18230255869"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p123055512620"><a name="zh-cn_topic_0000002485295476_p123055512620"></a><a name="zh-cn_topic_0000002485295476_p123055512620"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p623011552618"><a name="zh-cn_topic_0000002485295476_p623011552618"></a><a name="zh-cn_topic_0000002485295476_p623011552618"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row8655214617"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p14591202016712"><a name="zh-cn_topic_0000002485295476_p14591202016712"></a><a name="zh-cn_topic_0000002485295476_p14591202016712"></a><span id="zh-cn_topic_0000002485295476_text1659110201379"><a name="zh-cn_topic_0000002485295476_text1659110201379"></a><a name="zh-cn_topic_0000002485295476_text1659110201379"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p38158552616"><a name="zh-cn_topic_0000002485295476_p38158552616"></a><a name="zh-cn_topic_0000002485295476_p38158552616"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p18158552613"><a name="zh-cn_topic_0000002485295476_p18158552613"></a><a name="zh-cn_topic_0000002485295476_p18158552613"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p19815255367"><a name="zh-cn_topic_0000002485295476_p19815255367"></a><a name="zh-cn_topic_0000002485295476_p19815255367"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row67064433311"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15712048183315"><a name="zh-cn_topic_0000002485295476_p15712048183315"></a><a name="zh-cn_topic_0000002485295476_p15712048183315"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
enum dcmi_reset_channel channel_type = INBAND_CHANNEL;
ret = dcmi_set_device_reset(card_id, device_id, channel_type);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…  
```

