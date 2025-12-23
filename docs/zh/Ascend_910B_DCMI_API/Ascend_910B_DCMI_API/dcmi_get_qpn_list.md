# dcmi\_get\_qpn\_list<a name="ZH-CN_TOPIC_0000002485455398"></a>

**函数原型<a name="section10646124018595"></a>**

**int dcmi\_get\_qpn\_list \(int card\_id, int device\_id, int port\_id, struct dcmi\_qpn\_list \*list\)**

**功能说明<a name="section26471040155919"></a>**

获取qp的数量和对应的qpn\_list。

**参数说明<a name="section264894085911"></a>**

<a name="table56741840185919"></a>
<table><thead align="left"><tr id="row1874519409598"><th class="cellrowborder" valign="top" width="16.328367163283673%" id="mcps1.1.5.1.1"><p id="p13745154019597"><a name="p13745154019597"></a><a name="p13745154019597"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="14.288571142885711%" id="mcps1.1.5.1.2"><p id="p137451740175917"><a name="p137451740175917"></a><a name="p137451740175917"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="25.507449255074494%" id="mcps1.1.5.1.3"><p id="p27451140125911"><a name="p27451140125911"></a><a name="p27451140125911"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="43.87561243875613%" id="mcps1.1.5.1.4"><p id="p127453403594"><a name="p127453403594"></a><a name="p127453403594"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4650194615265"><td class="cellrowborder" valign="top" width="16.328367163283673%" headers="mcps1.1.5.1.1 "><p id="p39310217444"><a name="p39310217444"></a><a name="p39310217444"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="14.288571142885711%" headers="mcps1.1.5.1.2 "><p id="p129314219448"><a name="p129314219448"></a><a name="p129314219448"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="25.507449255074494%" headers="mcps1.1.5.1.3 "><p id="p11933217448"><a name="p11933217448"></a><a name="p11933217448"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="43.87561243875613%" headers="mcps1.1.5.1.4 "><p id="p109362184410"><a name="p109362184410"></a><a name="p109362184410"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row14745640195914"><td class="cellrowborder" valign="top" width="16.328367163283673%" headers="mcps1.1.5.1.1 "><p id="p147452404592"><a name="p147452404592"></a><a name="p147452404592"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="14.288571142885711%" headers="mcps1.1.5.1.2 "><p id="p7746114055918"><a name="p7746114055918"></a><a name="p7746114055918"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="25.507449255074494%" headers="mcps1.1.5.1.3 "><p id="p177461240195910"><a name="p177461240195910"></a><a name="p177461240195910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="43.87561243875613%" headers="mcps1.1.5.1.4 "><p id="p117460409592"><a name="p117460409592"></a><a name="p117460409592"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p5746144035914"><a name="p5746144035914"></a><a name="p5746144035914"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p19641115816479"><a name="p19641115816479"></a><a name="p19641115816479"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row1574718405595"><td class="cellrowborder" valign="top" width="16.328367163283673%" headers="mcps1.1.5.1.1 "><p id="p16747204015594"><a name="p16747204015594"></a><a name="p16747204015594"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="14.288571142885711%" headers="mcps1.1.5.1.2 "><p id="p147471640115914"><a name="p147471640115914"></a><a name="p147471640115914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="25.507449255074494%" headers="mcps1.1.5.1.3 "><p id="p07477402591"><a name="p07477402591"></a><a name="p07477402591"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="43.87561243875613%" headers="mcps1.1.5.1.4 "><p id="p67471340155912"><a name="p67471340155912"></a><a name="p67471340155912"></a>NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="row12747204020598"><td class="cellrowborder" valign="top" width="16.328367163283673%" headers="mcps1.1.5.1.1 "><p id="p1974714010598"><a name="p1974714010598"></a><a name="p1974714010598"></a>list</p>
</td>
<td class="cellrowborder" valign="top" width="14.288571142885711%" headers="mcps1.1.5.1.2 "><p id="p127471740145913"><a name="p127471740145913"></a><a name="p127471740145913"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="25.507449255074494%" headers="mcps1.1.5.1.3 "><p id="p174718403599"><a name="p174718403599"></a><a name="p174718403599"></a>struct dcmi_qpn_list*</p>
</td>
<td class="cellrowborder" valign="top" width="43.87561243875613%" headers="mcps1.1.5.1.4 "><p id="p1679332921613"><a name="p1679332921613"></a><a name="p1679332921613"></a>#define MAX_QPN_NUM 8192//qpn列表最大支持的条数</p>
<p id="p17471440105919"><a name="p17471440105919"></a><a name="p17471440105919"></a>struct dcmi_qpn_list {</p>
<p id="p11747174017595"><a name="p11747174017595"></a><a name="p11747174017595"></a>unsigned int qp_num;//获取到active状态的qp的数量</p>
<p id="p77474407599"><a name="p77474407599"></a><a name="p77474407599"></a>unsigned int qpn_list[MAX_QPN_NUM];//与qp_num对应的qpn列表</p>
<p id="p074715409590"><a name="p074715409590"></a><a name="p074715409590"></a>};</p>
<p id="p1095331622510"><a name="p1095331622510"></a><a name="p1095331622510"></a>qpn：表示qp number。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section18661174035919"></a>**

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

**异常处理<a name="section14668104014594"></a>**

无。

**约束说明<a name="section966910400597"></a>**

该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table1113417173519"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row3169171463617"><th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p8558112793214"><a name="zh-cn_topic_0000002485295476_p8558112793214"></a><a name="zh-cn_topic_0000002485295476_p8558112793214"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="18.94%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p655852717326"><a name="zh-cn_topic_0000002485295476_p655852717326"></a><a name="zh-cn_topic_0000002485295476_p655852717326"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p10558122715327"><a name="zh-cn_topic_0000002485295476_p10558122715327"></a><a name="zh-cn_topic_0000002485295476_p10558122715327"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p15581927103214"><a name="zh-cn_topic_0000002485295476_p15581927103214"></a><a name="zh-cn_topic_0000002485295476_p15581927103214"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row1816915147366"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p6169151423619"><a name="zh-cn_topic_0000002485295476_p6169151423619"></a><a name="zh-cn_topic_0000002485295476_p6169151423619"></a><span id="zh-cn_topic_0000002485295476_ph116911144369"><a name="zh-cn_topic_0000002485295476_ph116911144369"></a><a name="zh-cn_topic_0000002485295476_ph116911144369"></a><span id="zh-cn_topic_0000002485295476_text71694149366"><a name="zh-cn_topic_0000002485295476_text71694149366"></a><a name="zh-cn_topic_0000002485295476_text71694149366"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p316911142362"><a name="zh-cn_topic_0000002485295476_p316911142362"></a><a name="zh-cn_topic_0000002485295476_p316911142362"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p816918145361"><a name="zh-cn_topic_0000002485295476_p816918145361"></a><a name="zh-cn_topic_0000002485295476_p816918145361"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p9472202011368"><a name="zh-cn_topic_0000002485295476_p9472202011368"></a><a name="zh-cn_topic_0000002485295476_p9472202011368"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row01692149364"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p10169141412361"><a name="zh-cn_topic_0000002485295476_p10169141412361"></a><a name="zh-cn_topic_0000002485295476_p10169141412361"></a><span id="zh-cn_topic_0000002485295476_text1516951453611"><a name="zh-cn_topic_0000002485295476_text1516951453611"></a><a name="zh-cn_topic_0000002485295476_text1516951453611"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p16169121403617"><a name="zh-cn_topic_0000002485295476_p16169121403617"></a><a name="zh-cn_topic_0000002485295476_p16169121403617"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p171698141361"><a name="zh-cn_topic_0000002485295476_p171698141361"></a><a name="zh-cn_topic_0000002485295476_p171698141361"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1348119209364"><a name="zh-cn_topic_0000002485295476_p1348119209364"></a><a name="zh-cn_topic_0000002485295476_p1348119209364"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row17169814163612"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p2169414113616"><a name="zh-cn_topic_0000002485295476_p2169414113616"></a><a name="zh-cn_topic_0000002485295476_p2169414113616"></a><span id="zh-cn_topic_0000002485295476_text2169111443615"><a name="zh-cn_topic_0000002485295476_text2169111443615"></a><a name="zh-cn_topic_0000002485295476_text2169111443615"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p141696146366"><a name="zh-cn_topic_0000002485295476_p141696146366"></a><a name="zh-cn_topic_0000002485295476_p141696146366"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p3169114113611"><a name="zh-cn_topic_0000002485295476_p3169114113611"></a><a name="zh-cn_topic_0000002485295476_p3169114113611"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p13489162017366"><a name="zh-cn_topic_0000002485295476_p13489162017366"></a><a name="zh-cn_topic_0000002485295476_p13489162017366"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row01691314183618"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1616951453614"><a name="zh-cn_topic_0000002485295476_p1616951453614"></a><a name="zh-cn_topic_0000002485295476_p1616951453614"></a><span id="zh-cn_topic_0000002485295476_text17169141413610"><a name="zh-cn_topic_0000002485295476_text17169141413610"></a><a name="zh-cn_topic_0000002485295476_text17169141413610"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p316911419369"><a name="zh-cn_topic_0000002485295476_p316911419369"></a><a name="zh-cn_topic_0000002485295476_p316911419369"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p141697141368"><a name="zh-cn_topic_0000002485295476_p141697141368"></a><a name="zh-cn_topic_0000002485295476_p141697141368"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1949213202364"><a name="zh-cn_topic_0000002485295476_p1949213202364"></a><a name="zh-cn_topic_0000002485295476_p1949213202364"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1169151413367"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1716931483615"><a name="zh-cn_topic_0000002485295476_p1716931483615"></a><a name="zh-cn_topic_0000002485295476_p1716931483615"></a><span id="zh-cn_topic_0000002485295476_text81691114103617"><a name="zh-cn_topic_0000002485295476_text81691114103617"></a><a name="zh-cn_topic_0000002485295476_text81691114103617"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p816912141365"><a name="zh-cn_topic_0000002485295476_p816912141365"></a><a name="zh-cn_topic_0000002485295476_p816912141365"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15169191411369"><a name="zh-cn_topic_0000002485295476_p15169191411369"></a><a name="zh-cn_topic_0000002485295476_p15169191411369"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1497112043619"><a name="zh-cn_topic_0000002485295476_p1497112043619"></a><a name="zh-cn_topic_0000002485295476_p1497112043619"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row111691014173614"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p19169514103610"><a name="zh-cn_topic_0000002485295476_p19169514103610"></a><a name="zh-cn_topic_0000002485295476_p19169514103610"></a><span id="zh-cn_topic_0000002485295476_text15169171403610"><a name="zh-cn_topic_0000002485295476_text15169171403610"></a><a name="zh-cn_topic_0000002485295476_text15169171403610"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p181691314163611"><a name="zh-cn_topic_0000002485295476_p181691314163611"></a><a name="zh-cn_topic_0000002485295476_p181691314163611"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p516961453612"><a name="zh-cn_topic_0000002485295476_p516961453612"></a><a name="zh-cn_topic_0000002485295476_p516961453612"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p25021520143613"><a name="zh-cn_topic_0000002485295476_p25021520143613"></a><a name="zh-cn_topic_0000002485295476_p25021520143613"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1816912147365"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1916941423613"><a name="zh-cn_topic_0000002485295476_p1916941423613"></a><a name="zh-cn_topic_0000002485295476_p1916941423613"></a><span id="zh-cn_topic_0000002485295476_text121691149365"><a name="zh-cn_topic_0000002485295476_text121691149365"></a><a name="zh-cn_topic_0000002485295476_text121691149365"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p14169101413366"><a name="zh-cn_topic_0000002485295476_p14169101413366"></a><a name="zh-cn_topic_0000002485295476_p14169101413366"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p10169614163611"><a name="zh-cn_topic_0000002485295476_p10169614163611"></a><a name="zh-cn_topic_0000002485295476_p10169614163611"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p12507102016363"><a name="zh-cn_topic_0000002485295476_p12507102016363"></a><a name="zh-cn_topic_0000002485295476_p12507102016363"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1317019147361"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p2170191493613"><a name="zh-cn_topic_0000002485295476_p2170191493613"></a><a name="zh-cn_topic_0000002485295476_p2170191493613"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section156701340105918"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
struct dcmi_qpn_list list = {0}; 
ret = dcmi_get_qpn_list(card_id, device_id, port_id, &list);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

