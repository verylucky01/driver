# dcmi\_set\_vdevice\_mode<a name="ZH-CN_TOPIC_0000002517615331"></a>

**函数原型<a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_toc533412077"></a>**

**int dcmi\_set\_vdevice\_mode\(int mode\)**

**功能说明<a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_toc533412078"></a>**

设置昇腾虚拟化实例（AVI）模式。

**参数说明<a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_toc533412079"></a>**

<a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p10021890"><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p10021890"></a><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p6466753"><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p6466753"></a><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p54045009"><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p54045009"></a><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p15569626"><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p15569626"></a><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p36741947142813"><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p36741947142813"></a><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p36741947142813"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p96741747122818"><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p96741747122818"></a><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p46747472287"><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p46747472287"></a><a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p053942223515"><a name="p053942223515"></a><a name="p053942223515"></a><span id="ph5309125710415"><a name="ph5309125710415"></a><a name="ph5309125710415"></a>昇腾虚拟化实例（AVI）</span>模式。取值为0或1：</p>
<a name="ul7595625173516"></a><a name="ul7595625173516"></a><ul id="ul7595625173516"><li>0：<span id="ph12721132711424"><a name="ph12721132711424"></a><a name="ph12721132711424"></a>昇腾虚拟化实例（AVI）</span>容器模式</li><li>1：<span id="ph1781372913420"><a name="ph1781372913420"></a><a name="ph1781372913420"></a>昇腾虚拟化实例（AVI）</span>虚拟机模式</li></ul>
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

**约束说明<a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_toc533412082"></a>**

调用该接口前，请确保未创建vNPU，可通过[dcmi\_get\_device\_info](dcmi_get_device_info.md)进行查询。

**表 1** 不同部署场景下的支持情况

<a name="table05161849203916"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row2045415135512"><th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p15636143423210"><a name="zh-cn_topic_0000002485295476_p15636143423210"></a><a name="zh-cn_topic_0000002485295476_p15636143423210"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="18.94%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p2636334183218"><a name="zh-cn_topic_0000002485295476_p2636334183218"></a><a name="zh-cn_topic_0000002485295476_p2636334183218"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p1063683420326"><a name="zh-cn_topic_0000002485295476_p1063683420326"></a><a name="zh-cn_topic_0000002485295476_p1063683420326"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p363653418324"><a name="zh-cn_topic_0000002485295476_p363653418324"></a><a name="zh-cn_topic_0000002485295476_p363653418324"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row1645410105516"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p117941512103817"><a name="zh-cn_topic_0000002485295476_p117941512103817"></a><a name="zh-cn_topic_0000002485295476_p117941512103817"></a><span id="zh-cn_topic_0000002485295476_ph1645441320389"><a name="zh-cn_topic_0000002485295476_ph1645441320389"></a><a name="zh-cn_topic_0000002485295476_ph1645441320389"></a><span id="zh-cn_topic_0000002485295476_text545418133385"><a name="zh-cn_topic_0000002485295476_text545418133385"></a><a name="zh-cn_topic_0000002485295476_text545418133385"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p5916205813388"><a name="zh-cn_topic_0000002485295476_p5916205813388"></a><a name="zh-cn_topic_0000002485295476_p5916205813388"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p19925115819389"><a name="zh-cn_topic_0000002485295476_p19925115819389"></a><a name="zh-cn_topic_0000002485295476_p19925115819389"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1933175817386"><a name="zh-cn_topic_0000002485295476_p1933175817386"></a><a name="zh-cn_topic_0000002485295476_p1933175817386"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row645461155513"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p345401105515"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p345401105515"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p345401105515"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text14454161145518"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text14454161145518"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text14454161145518"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p199362580386"><a name="zh-cn_topic_0000002485295476_p199362580386"></a><a name="zh-cn_topic_0000002485295476_p199362580386"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1394013583382"><a name="zh-cn_topic_0000002485295476_p1394013583382"></a><a name="zh-cn_topic_0000002485295476_p1394013583382"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p139430581386"><a name="zh-cn_topic_0000002485295476_p139430581386"></a><a name="zh-cn_topic_0000002485295476_p139430581386"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row245413115520"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p9454171165510"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p9454171165510"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p9454171165510"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text04544155514"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text04544155514"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text04544155514"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p15946145817381"><a name="zh-cn_topic_0000002485295476_p15946145817381"></a><a name="zh-cn_topic_0000002485295476_p15946145817381"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p194935873816"><a name="zh-cn_topic_0000002485295476_p194935873816"></a><a name="zh-cn_topic_0000002485295476_p194935873816"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p7952165812388"><a name="zh-cn_topic_0000002485295476_p7952165812388"></a><a name="zh-cn_topic_0000002485295476_p7952165812388"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row34540110555"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p845481115513"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p845481115513"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p845481115513"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text19454171145517"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text19454171145517"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text19454171145517"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p11954858173813"><a name="zh-cn_topic_0000002485295476_p11954858173813"></a><a name="zh-cn_topic_0000002485295476_p11954858173813"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p8956658123817"><a name="zh-cn_topic_0000002485295476_p8956658123817"></a><a name="zh-cn_topic_0000002485295476_p8956658123817"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p8958185843817"><a name="zh-cn_topic_0000002485295476_p8958185843817"></a><a name="zh-cn_topic_0000002485295476_p8958185843817"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row145510110556"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p1645512114557"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p1645512114557"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p1645512114557"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text445514113555"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text445514113555"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text445514113555"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p13959205863810"><a name="zh-cn_topic_0000002485295476_p13959205863810"></a><a name="zh-cn_topic_0000002485295476_p13959205863810"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1396185818385"><a name="zh-cn_topic_0000002485295476_p1396185818385"></a><a name="zh-cn_topic_0000002485295476_p1396185818385"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p8963358173816"><a name="zh-cn_topic_0000002485295476_p8963358173816"></a><a name="zh-cn_topic_0000002485295476_p8963358173816"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row77142361118"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p4168142915111"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p4168142915111"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p4168142915111"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text8168172919111"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text8168172919111"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text8168172919111"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p139651958173812"><a name="zh-cn_topic_0000002485295476_p139651958173812"></a><a name="zh-cn_topic_0000002485295476_p139651958173812"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p09671358173813"><a name="zh-cn_topic_0000002485295476_p09671358173813"></a><a name="zh-cn_topic_0000002485295476_p09671358173813"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p296855813383"><a name="zh-cn_topic_0000002485295476_p296855813383"></a><a name="zh-cn_topic_0000002485295476_p296855813383"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row1341920320316"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p164191634319"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p164191634319"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p164191634319"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text199741574318"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text199741574318"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text199741574318"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p149705587385"><a name="zh-cn_topic_0000002485295476_p149705587385"></a><a name="zh-cn_topic_0000002485295476_p149705587385"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p16972165817381"><a name="zh-cn_topic_0000002485295476_p16972165817381"></a><a name="zh-cn_topic_0000002485295476_p16972165817381"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p19741058163819"><a name="zh-cn_topic_0000002485295476_p19741058163819"></a><a name="zh-cn_topic_0000002485295476_p19741058163819"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row96090408452"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p13701112414381"><a name="zh-cn_topic_0000002485295476_p13701112414381"></a><a name="zh-cn_topic_0000002485295476_p13701112414381"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001277289337_zh-cn_topic_0000001178213212_zh-cn_topic_0000001167998163_toc533412083"></a>**

```
… 
int mode = 0; 
ret= dcmi_set_vdevice_mode(mode);
if (ret != 0){ 
    //todo:记录日志 
    return ret; 
} 
…
```

