# dcmi\_clear\_pfc\_duration<a name="ZH-CN_TOPIC_0000002485295424"></a>

**函数原型<a name="section12365583546"></a>**

**int dcmi\_clear\_pfc\_duration\(int card\_id, int device\_id\)**

**功能说明<a name="section142361658175418"></a>**

清除指定设备的PFC反压帧持续时间统计值。

**参数说明<a name="section32371758125411"></a>**

<a name="table2274145813545"></a>
<table><thead align="left"><tr id="row106575945418"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p665759125418"><a name="p665759125418"></a><a name="p665759125418"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p10659593544"><a name="p10659593544"></a><a name="p10659593544"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="p1265259155414"><a name="p1265259155414"></a><a name="p1265259155414"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p196611598540"><a name="p196611598540"></a><a name="p196611598540"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row106685915545"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p76613598548"><a name="p76613598548"></a><a name="p76613598548"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p196625985410"><a name="p196625985410"></a><a name="p196625985410"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p4661459185416"><a name="p4661459185416"></a><a name="p4661459185416"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p8662594544"><a name="p8662594544"></a><a name="p8662594544"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1662595540"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p16610593540"><a name="p16610593540"></a><a name="p16610593540"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p06675913543"><a name="p06675913543"></a><a name="p06675913543"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p1666159175411"><a name="p1666159175411"></a><a name="p1666159175411"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p176616594547"><a name="p176616594547"></a><a name="p176616594547"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p2066359185415"><a name="p2066359185415"></a><a name="p2066359185415"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p79931049174715"><a name="p79931049174715"></a><a name="p79931049174715"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section124813589544"></a>**

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

**异常处理<a name="section1225565895419"></a>**

无。

**约束说明<a name="section1625614581546"></a>**

对于Atlas 200T A2 Box16 异构子框、Atlas 800T A2 训练服务器、Atlas 800I A2 推理服务器、Atlas 900 A2 PoD 集群基础单元、A200I A2 Box 异构组件，该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table6665182042413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1514515016392"><th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p15286130203213"><a name="zh-cn_topic_0000002485295476_p15286130203213"></a><a name="zh-cn_topic_0000002485295476_p15286130203213"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="18.94%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p132861730113218"><a name="zh-cn_topic_0000002485295476_p132861730113218"></a><a name="zh-cn_topic_0000002485295476_p132861730113218"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p22861530183212"><a name="zh-cn_topic_0000002485295476_p22861530183212"></a><a name="zh-cn_topic_0000002485295476_p22861530183212"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p1928683003217"><a name="zh-cn_topic_0000002485295476_p1928683003217"></a><a name="zh-cn_topic_0000002485295476_p1928683003217"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row514512014397"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p14145100183919"><a name="zh-cn_topic_0000002485295476_p14145100183919"></a><a name="zh-cn_topic_0000002485295476_p14145100183919"></a><span id="zh-cn_topic_0000002485295476_ph15145206394"><a name="zh-cn_topic_0000002485295476_ph15145206394"></a><a name="zh-cn_topic_0000002485295476_ph15145206394"></a><span id="zh-cn_topic_0000002485295476_text141451204393"><a name="zh-cn_topic_0000002485295476_text141451204393"></a><a name="zh-cn_topic_0000002485295476_text141451204393"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p114519073913"><a name="zh-cn_topic_0000002485295476_p114519073913"></a><a name="zh-cn_topic_0000002485295476_p114519073913"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p672045395"><a name="zh-cn_topic_0000002485295476_p672045395"></a><a name="zh-cn_topic_0000002485295476_p672045395"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p14102046396"><a name="zh-cn_topic_0000002485295476_p14102046396"></a><a name="zh-cn_topic_0000002485295476_p14102046396"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1314590133918"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p914590143915"><a name="zh-cn_topic_0000002485295476_p914590143915"></a><a name="zh-cn_topic_0000002485295476_p914590143915"></a><span id="zh-cn_topic_0000002485295476_text151456017398"><a name="zh-cn_topic_0000002485295476_text151456017398"></a><a name="zh-cn_topic_0000002485295476_text151456017398"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p114512012392"><a name="zh-cn_topic_0000002485295476_p114512012392"></a><a name="zh-cn_topic_0000002485295476_p114512012392"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p161319419393"><a name="zh-cn_topic_0000002485295476_p161319419393"></a><a name="zh-cn_topic_0000002485295476_p161319419393"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1019184143917"><a name="zh-cn_topic_0000002485295476_p1019184143917"></a><a name="zh-cn_topic_0000002485295476_p1019184143917"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1814514011392"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p414590193920"><a name="zh-cn_topic_0000002485295476_p414590193920"></a><a name="zh-cn_topic_0000002485295476_p414590193920"></a><span id="zh-cn_topic_0000002485295476_text161451704394"><a name="zh-cn_topic_0000002485295476_text161451704394"></a><a name="zh-cn_topic_0000002485295476_text161451704394"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1514516018392"><a name="zh-cn_topic_0000002485295476_p1514516018392"></a><a name="zh-cn_topic_0000002485295476_p1514516018392"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p18242403910"><a name="zh-cn_topic_0000002485295476_p18242403910"></a><a name="zh-cn_topic_0000002485295476_p18242403910"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p18307415390"><a name="zh-cn_topic_0000002485295476_p18307415390"></a><a name="zh-cn_topic_0000002485295476_p18307415390"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1114514093913"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1145407399"><a name="zh-cn_topic_0000002485295476_p1145407399"></a><a name="zh-cn_topic_0000002485295476_p1145407399"></a><span id="zh-cn_topic_0000002485295476_text161451007395"><a name="zh-cn_topic_0000002485295476_text161451007395"></a><a name="zh-cn_topic_0000002485295476_text161451007395"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p314518015391"><a name="zh-cn_topic_0000002485295476_p314518015391"></a><a name="zh-cn_topic_0000002485295476_p314518015391"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13378413911"><a name="zh-cn_topic_0000002485295476_p13378413911"></a><a name="zh-cn_topic_0000002485295476_p13378413911"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p8431344398"><a name="zh-cn_topic_0000002485295476_p8431344398"></a><a name="zh-cn_topic_0000002485295476_p8431344398"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1114514017397"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1114510163910"><a name="zh-cn_topic_0000002485295476_p1114510163910"></a><a name="zh-cn_topic_0000002485295476_p1114510163910"></a><span id="zh-cn_topic_0000002485295476_text01466093919"><a name="zh-cn_topic_0000002485295476_text01466093919"></a><a name="zh-cn_topic_0000002485295476_text01466093919"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p214613010391"><a name="zh-cn_topic_0000002485295476_p214613010391"></a><a name="zh-cn_topic_0000002485295476_p214613010391"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15471840391"><a name="zh-cn_topic_0000002485295476_p15471840391"></a><a name="zh-cn_topic_0000002485295476_p15471840391"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p45014483920"><a name="zh-cn_topic_0000002485295476_p45014483920"></a><a name="zh-cn_topic_0000002485295476_p45014483920"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row3146140203913"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p0146160143914"><a name="zh-cn_topic_0000002485295476_p0146160143914"></a><a name="zh-cn_topic_0000002485295476_p0146160143914"></a><span id="zh-cn_topic_0000002485295476_text61462003390"><a name="zh-cn_topic_0000002485295476_text61462003390"></a><a name="zh-cn_topic_0000002485295476_text61462003390"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p141461506394"><a name="zh-cn_topic_0000002485295476_p141461506394"></a><a name="zh-cn_topic_0000002485295476_p141461506394"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p353114123912"><a name="zh-cn_topic_0000002485295476_p353114123912"></a><a name="zh-cn_topic_0000002485295476_p353114123912"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p135715415392"><a name="zh-cn_topic_0000002485295476_p135715415392"></a><a name="zh-cn_topic_0000002485295476_p135715415392"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row31461604397"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p714619015395"><a name="zh-cn_topic_0000002485295476_p714619015395"></a><a name="zh-cn_topic_0000002485295476_p714619015395"></a><span id="zh-cn_topic_0000002485295476_text111462014399"><a name="zh-cn_topic_0000002485295476_text111462014399"></a><a name="zh-cn_topic_0000002485295476_text111462014399"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1314610043913"><a name="zh-cn_topic_0000002485295476_p1314610043913"></a><a name="zh-cn_topic_0000002485295476_p1314610043913"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p8602483913"><a name="zh-cn_topic_0000002485295476_p8602483913"></a><a name="zh-cn_topic_0000002485295476_p8602483913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p11635413398"><a name="zh-cn_topic_0000002485295476_p11635413398"></a><a name="zh-cn_topic_0000002485295476_p11635413398"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row31464013910"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p5146120153917"><a name="zh-cn_topic_0000002485295476_p5146120153917"></a><a name="zh-cn_topic_0000002485295476_p5146120153917"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section192671558125418"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
ret = dcmi_clear_pfc_duration(card_id, device_id);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

