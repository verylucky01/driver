# dcmi\_get\_rdma\_bandwidth\_info<a name="ZH-CN_TOPIC_0000002485318810"></a>

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
<p id="p450968185915"><a name="p450968185915"></a><a name="p450968185915"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p591115211918"><a name="p591115211918"></a><a name="p591115211918"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
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
<p id="p165801422016"><a name="p165801422016"></a><a name="p165801422016"></a>unsigned int tx_bandwidth;发送方向带宽</p>
<p id="p1559718170017"><a name="p1559718170017"></a><a name="p1559718170017"></a>unsigned int rx_bandwidth;接收方向带宽</p>
<p id="p35970176020"><a name="p35970176020"></a><a name="p35970176020"></a>};</p>
<p id="p1597111719014"><a name="p1597111719014"></a><a name="p1597111719014"></a>单位为MB/s。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**约束说明<a name="section364683165017"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table095520915213"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row12818154935117"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p181256193914"><a name="zh-cn_topic_0000002485318818_p181256193914"></a><a name="zh-cn_topic_0000002485318818_p181256193914"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p181115613399"><a name="zh-cn_topic_0000002485318818_p181115613399"></a><a name="zh-cn_topic_0000002485318818_p181115613399"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p4145619392"><a name="zh-cn_topic_0000002485318818_p4145619392"></a><a name="zh-cn_topic_0000002485318818_p4145619392"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p9165683914"><a name="zh-cn_topic_0000002485318818_p9165683914"></a><a name="zh-cn_topic_0000002485318818_p9165683914"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row1781874915512"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1681824913518"><a name="zh-cn_topic_0000002485318818_p1681824913518"></a><a name="zh-cn_topic_0000002485318818_p1681824913518"></a><span id="zh-cn_topic_0000002485318818_text1381814492513"><a name="zh-cn_topic_0000002485318818_text1381814492513"></a><a name="zh-cn_topic_0000002485318818_text1381814492513"></a><span id="zh-cn_topic_0000002485318818_text1781824975120"><a name="zh-cn_topic_0000002485318818_text1781824975120"></a><a name="zh-cn_topic_0000002485318818_text1781824975120"></a>Atlas 900 A3 SuperPoD 超节点</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p788675755111"><a name="zh-cn_topic_0000002485318818_p788675755111"></a><a name="zh-cn_topic_0000002485318818_p788675755111"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p088675795113"><a name="zh-cn_topic_0000002485318818_p088675795113"></a><a name="zh-cn_topic_0000002485318818_p088675795113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p688614575515"><a name="zh-cn_topic_0000002485318818_p688614575515"></a><a name="zh-cn_topic_0000002485318818_p688614575515"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row4818124912514"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p681824985116"><a name="zh-cn_topic_0000002485318818_p681824985116"></a><a name="zh-cn_topic_0000002485318818_p681824985116"></a><span id="zh-cn_topic_0000002485318818_text158181149145113"><a name="zh-cn_topic_0000002485318818_text158181149145113"></a><a name="zh-cn_topic_0000002485318818_text158181149145113"></a><span id="zh-cn_topic_0000002485318818_text081844985117"><a name="zh-cn_topic_0000002485318818_text081844985117"></a><a name="zh-cn_topic_0000002485318818_text081844985117"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p168861157145118"><a name="zh-cn_topic_0000002485318818_p168861157145118"></a><a name="zh-cn_topic_0000002485318818_p168861157145118"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p188861657125114"><a name="zh-cn_topic_0000002485318818_p188861657125114"></a><a name="zh-cn_topic_0000002485318818_p188861657125114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p688645745119"><a name="zh-cn_topic_0000002485318818_p688645745119"></a><a name="zh-cn_topic_0000002485318818_p688645745119"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row2202847205512"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p12706213161"><a name="zh-cn_topic_0000002485318818_p12706213161"></a><a name="zh-cn_topic_0000002485318818_p12706213161"></a><span id="zh-cn_topic_0000002485318818_text42718211164"><a name="zh-cn_topic_0000002485318818_text42718211164"></a><a name="zh-cn_topic_0000002485318818_text42718211164"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p55253416563"><a name="zh-cn_topic_0000002485318818_p55253416563"></a><a name="zh-cn_topic_0000002485318818_p55253416563"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p05251540564"><a name="zh-cn_topic_0000002485318818_p05251540564"></a><a name="zh-cn_topic_0000002485318818_p05251540564"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p175250435618"><a name="zh-cn_topic_0000002485318818_p175250435618"></a><a name="zh-cn_topic_0000002485318818_p175250435618"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1659123792914"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p365923772919"><a name="zh-cn_topic_0000002485318818_p365923772919"></a><a name="zh-cn_topic_0000002485318818_p365923772919"></a><span id="zh-cn_topic_0000002485318818_text7609428297"><a name="zh-cn_topic_0000002485318818_text7609428297"></a><a name="zh-cn_topic_0000002485318818_text7609428297"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p0342154513291"><a name="zh-cn_topic_0000002485318818_p0342154513291"></a><a name="zh-cn_topic_0000002485318818_p0342154513291"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p11342134582916"><a name="zh-cn_topic_0000002485318818_p11342134582916"></a><a name="zh-cn_topic_0000002485318818_p11342134582916"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p10342345102917"><a name="zh-cn_topic_0000002485318818_p10342345102917"></a><a name="zh-cn_topic_0000002485318818_p10342345102917"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row179571851165511"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p87312035620"><a name="zh-cn_topic_0000002485318818_p87312035620"></a><a name="zh-cn_topic_0000002485318818_p87312035620"></a><span id="zh-cn_topic_0000002485318818_text187313016562"><a name="zh-cn_topic_0000002485318818_text187313016562"></a><a name="zh-cn_topic_0000002485318818_text187313016562"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1667457567"><a name="zh-cn_topic_0000002485318818_p1667457567"></a><a name="zh-cn_topic_0000002485318818_p1667457567"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p36715575611"><a name="zh-cn_topic_0000002485318818_p36715575611"></a><a name="zh-cn_topic_0000002485318818_p36715575611"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p4671505616"><a name="zh-cn_topic_0000002485318818_p4671505616"></a><a name="zh-cn_topic_0000002485318818_p4671505616"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row5430115215816"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p84951356389"><a name="zh-cn_topic_0000002485318818_p84951356389"></a><a name="zh-cn_topic_0000002485318818_p84951356389"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
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

