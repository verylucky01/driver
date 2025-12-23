# dcmi\_get\_device\_component\_count<a name="ZH-CN_TOPIC_0000002517535319"></a>

**函数原型<a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_toc533412077"></a>**

**int dcmi\_get\_device\_component\_count\(int card\_id, int device\_id, unsigned int \*component\_count\)**

**功能说明<a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_toc533412078"></a>**

获取可升级组件的个数。

**参数说明<a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_row7580267"><th class="cellrowborder" valign="top" width="19.62%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p10021890"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p10021890"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="14.48%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p6466753"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p6466753"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="15.9%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p54045009"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p54045009"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p15569626"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p15569626"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_row10560021192510"><td class="cellrowborder" valign="top" width="19.62%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p36741947142813"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p36741947142813"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="14.48%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p96741747122818"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p96741747122818"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.9%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p46747472287"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p46747472287"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1467413474281"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1467413474281"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_row1155711494235"><td class="cellrowborder" valign="top" width="19.62%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p7711145152918"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p7711145152918"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="14.48%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p671116522914"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p671116522914"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.9%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1771116572910"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1771116572910"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p56811534176"><a name="p56811534176"></a><a name="p56811534176"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_row15462171542913"><td class="cellrowborder" valign="top" width="19.62%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p5522164215178"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p5522164215178"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p5522164215178"></a>component_count</p>
</td>
<td class="cellrowborder" valign="top" width="14.48%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p8522242101715"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p8522242101715"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p8522242101715"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="15.9%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p17522114220174"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p17522114220174"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p17522114220174"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1682695217315"><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1682695217315"></a><a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_p1682695217315"></a>返回组件的个数。</p>
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

**约束说明<a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_toc533412082"></a>**

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

**调用示例<a name="zh-cn_topic_0000001206467216_zh-cn_topic_0000001178373140_zh-cn_topic_0000001101617122_toc533412083"></a>**

```
…
int ret = 0;
int card_id = 0;
int device_id = 0;
unsigned int component_num = 0;
ret =dcmi_get_device_component_count(card_id, device_id, &component_num);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

