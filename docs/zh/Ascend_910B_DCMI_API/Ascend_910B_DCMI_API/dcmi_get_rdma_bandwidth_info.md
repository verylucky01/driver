# dcmi\_get\_rdma\_bandwidth\_info<a name="ZH-CN_TOPIC_0000002517615377"></a>

**函数原型<a name="section196251231145019"></a>**

**int dcmi\_get\_rdma\_bandwidth\_info\(int card\_id, int device\_id, int port\_id,** **unsigned int prof\_time, struct dcmi\_network\_rdma\_bandwidth\_info \*network\_rdma\_bandwidth\_info\)**

**功能说明<a name="section116271631195017"></a>**

查询指定采样时间内，NPU设备网口的rdma带宽信息。

**参数说明<a name="section11627103114506"></a>**

<a name="table196551731155016"></a>
<table><thead align="left"><tr id="row6708031165017"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="p17708123120507"><a name="p17708123120507"></a><a name="p17708123120507"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.2"><p id="p13708143185017"><a name="p13708143185017"></a><a name="p13708143185017"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.5.1.3"><p id="p67081031185017"><a name="p67081031185017"></a><a name="p67081031185017"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="p3708131145014"><a name="p3708131145014"></a><a name="p3708131145014"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row270819311506"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p3708193115501"><a name="p3708193115501"></a><a name="p3708193115501"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p1170863112507"><a name="p1170863112507"></a><a name="p1170863112507"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p17081231205013"><a name="p17081231205013"></a><a name="p17081231205013"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p27082313505"><a name="p27082313505"></a><a name="p27082313505"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1170873116504"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p11708131185016"><a name="p11708131185016"></a><a name="p11708131185016"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p1070811311504"><a name="p1070811311504"></a><a name="p1070811311504"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p970973119509"><a name="p970973119509"></a><a name="p970973119509"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p870915316504"><a name="p870915316504"></a><a name="p870915316504"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p157132715473"><a name="p157132715473"></a><a name="p157132715473"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row89976111742"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p1824103514419"><a name="p1824103514419"></a><a name="p1824103514419"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p424143514417"><a name="p424143514417"></a><a name="p424143514417"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p1424112357410"><a name="p1424112357410"></a><a name="p1424112357410"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1224183544119"><a name="p1224183544119"></a><a name="p1224183544119"></a>NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="row2498185195919"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p145961171904"><a name="p145961171904"></a><a name="p145961171904"></a>prof_time</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p55965171505"><a name="p55965171505"></a><a name="p55965171505"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p145963175011"><a name="p145963175011"></a><a name="p145963175011"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p11596317609"><a name="p11596317609"></a><a name="p11596317609"></a>采样时间，取值范围：100ms~10000ms。</p>
</td>
</tr>
<tr id="row15998411242"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p6597191716014"><a name="p6597191716014"></a><a name="p6597191716014"></a>network_rdma_bandwidth_info</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p10597717308"><a name="p10597717308"></a><a name="p10597717308"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p4597101712011"><a name="p4597101712011"></a><a name="p4597101712011"></a>dcmi_network_rdma_bandwidth_info</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p195971017809"><a name="p195971017809"></a><a name="p195971017809"></a>struct dcmi_network_rdma_bandwidth_info {</p>
<p id="p165801422016"><a name="p165801422016"></a><a name="p165801422016"></a>unsigned int tx_bandwidth; //发送方向带宽</p>
<p id="p1559718170017"><a name="p1559718170017"></a><a name="p1559718170017"></a>unsigned int rx_bandwidth; //接收方向带宽</p>
<p id="p35970176020"><a name="p35970176020"></a><a name="p35970176020"></a>};</p>
<p id="p1597111719014"><a name="p1597111719014"></a><a name="p1597111719014"></a>单位为MB/s。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section3642631135017"></a>**

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

**异常处理<a name="section1464620315507"></a>**

无。

**约束说明<a name="section364683165017"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table629810360529"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row979954933113"><th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p259621143212"><a name="zh-cn_topic_0000002485295476_p259621143212"></a><a name="zh-cn_topic_0000002485295476_p259621143212"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="18.94%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p185961211133216"><a name="zh-cn_topic_0000002485295476_p185961211133216"></a><a name="zh-cn_topic_0000002485295476_p185961211133216"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p3596161118320"><a name="zh-cn_topic_0000002485295476_p3596161118320"></a><a name="zh-cn_topic_0000002485295476_p3596161118320"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p13596101193211"><a name="zh-cn_topic_0000002485295476_p13596101193211"></a><a name="zh-cn_topic_0000002485295476_p13596101193211"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row279917498314"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p879924993116"><a name="zh-cn_topic_0000002485295476_p879924993116"></a><a name="zh-cn_topic_0000002485295476_p879924993116"></a><span id="zh-cn_topic_0000002485295476_ph3799204963110"><a name="zh-cn_topic_0000002485295476_ph3799204963110"></a><a name="zh-cn_topic_0000002485295476_ph3799204963110"></a><span id="zh-cn_topic_0000002485295476_text579974910315"><a name="zh-cn_topic_0000002485295476_text579974910315"></a><a name="zh-cn_topic_0000002485295476_text579974910315"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p18370822173217"><a name="zh-cn_topic_0000002485295476_p18370822173217"></a><a name="zh-cn_topic_0000002485295476_p18370822173217"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1737914223329"><a name="zh-cn_topic_0000002485295476_p1737914223329"></a><a name="zh-cn_topic_0000002485295476_p1737914223329"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p78004497311"><a name="zh-cn_topic_0000002485295476_p78004497311"></a><a name="zh-cn_topic_0000002485295476_p78004497311"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row10800749163116"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p11800114913117"><a name="zh-cn_topic_0000002485295476_p11800114913117"></a><a name="zh-cn_topic_0000002485295476_p11800114913117"></a><span id="zh-cn_topic_0000002485295476_text8800164911319"><a name="zh-cn_topic_0000002485295476_text8800164911319"></a><a name="zh-cn_topic_0000002485295476_text8800164911319"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1638911222323"><a name="zh-cn_topic_0000002485295476_p1638911222323"></a><a name="zh-cn_topic_0000002485295476_p1638911222323"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p17392222113213"><a name="zh-cn_topic_0000002485295476_p17392222113213"></a><a name="zh-cn_topic_0000002485295476_p17392222113213"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p168008496318"><a name="zh-cn_topic_0000002485295476_p168008496318"></a><a name="zh-cn_topic_0000002485295476_p168008496318"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row5800184916315"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p9800164914316"><a name="zh-cn_topic_0000002485295476_p9800164914316"></a><a name="zh-cn_topic_0000002485295476_p9800164914316"></a><span id="zh-cn_topic_0000002485295476_text980012496318"><a name="zh-cn_topic_0000002485295476_text980012496318"></a><a name="zh-cn_topic_0000002485295476_text980012496318"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p7397222133210"><a name="zh-cn_topic_0000002485295476_p7397222133210"></a><a name="zh-cn_topic_0000002485295476_p7397222133210"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p194011622143211"><a name="zh-cn_topic_0000002485295476_p194011622143211"></a><a name="zh-cn_topic_0000002485295476_p194011622143211"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p178001449123113"><a name="zh-cn_topic_0000002485295476_p178001449123113"></a><a name="zh-cn_topic_0000002485295476_p178001449123113"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row7800104915312"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1580044933111"><a name="zh-cn_topic_0000002485295476_p1580044933111"></a><a name="zh-cn_topic_0000002485295476_p1580044933111"></a><span id="zh-cn_topic_0000002485295476_text9800184912315"><a name="zh-cn_topic_0000002485295476_text9800184912315"></a><a name="zh-cn_topic_0000002485295476_text9800184912315"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p104051222163215"><a name="zh-cn_topic_0000002485295476_p104051222163215"></a><a name="zh-cn_topic_0000002485295476_p104051222163215"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p440822211325"><a name="zh-cn_topic_0000002485295476_p440822211325"></a><a name="zh-cn_topic_0000002485295476_p440822211325"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p38001949193113"><a name="zh-cn_topic_0000002485295476_p38001949193113"></a><a name="zh-cn_topic_0000002485295476_p38001949193113"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row18800194983115"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1180013499315"><a name="zh-cn_topic_0000002485295476_p1180013499315"></a><a name="zh-cn_topic_0000002485295476_p1180013499315"></a><span id="zh-cn_topic_0000002485295476_text10800649183116"><a name="zh-cn_topic_0000002485295476_text10800649183116"></a><a name="zh-cn_topic_0000002485295476_text10800649183116"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p18410722203220"><a name="zh-cn_topic_0000002485295476_p18410722203220"></a><a name="zh-cn_topic_0000002485295476_p18410722203220"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p3413182217322"><a name="zh-cn_topic_0000002485295476_p3413182217322"></a><a name="zh-cn_topic_0000002485295476_p3413182217322"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1880012499317"><a name="zh-cn_topic_0000002485295476_p1880012499317"></a><a name="zh-cn_topic_0000002485295476_p1880012499317"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row13800174918315"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p7800124914316"><a name="zh-cn_topic_0000002485295476_p7800124914316"></a><a name="zh-cn_topic_0000002485295476_p7800124914316"></a><span id="zh-cn_topic_0000002485295476_text980013499315"><a name="zh-cn_topic_0000002485295476_text980013499315"></a><a name="zh-cn_topic_0000002485295476_text980013499315"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p18415112223217"><a name="zh-cn_topic_0000002485295476_p18415112223217"></a><a name="zh-cn_topic_0000002485295476_p18415112223217"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p10417422113216"><a name="zh-cn_topic_0000002485295476_p10417422113216"></a><a name="zh-cn_topic_0000002485295476_p10417422113216"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p14800449103115"><a name="zh-cn_topic_0000002485295476_p14800449103115"></a><a name="zh-cn_topic_0000002485295476_p14800449103115"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1680016498317"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p8800124920315"><a name="zh-cn_topic_0000002485295476_p8800124920315"></a><a name="zh-cn_topic_0000002485295476_p8800124920315"></a><span id="zh-cn_topic_0000002485295476_text2800174910316"><a name="zh-cn_topic_0000002485295476_text2800174910316"></a><a name="zh-cn_topic_0000002485295476_text2800174910316"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p134191622183215"><a name="zh-cn_topic_0000002485295476_p134191622183215"></a><a name="zh-cn_topic_0000002485295476_p134191622183215"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p0422822103214"><a name="zh-cn_topic_0000002485295476_p0422822103214"></a><a name="zh-cn_topic_0000002485295476_p0422822103214"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p19800144917310"><a name="zh-cn_topic_0000002485295476_p19800144917310"></a><a name="zh-cn_topic_0000002485295476_p19800144917310"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row2800104920311"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p4800194933110"><a name="zh-cn_topic_0000002485295476_p4800194933110"></a><a name="zh-cn_topic_0000002485295476_p4800194933110"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section13653173113509"></a>**

```
…
int ret = 0;
int card_id=0;
int device_id=0;
int port_id=0;
unsigned int prof_time = 1000;
struct dcmi_network_rdma_bandwidth_info bandwidth_info = {0};
ret = dcmi_get_rdma_bandwidth_info (card_id, device_id, port_id, prof_time, &bandwidth_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

