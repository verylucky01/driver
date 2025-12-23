# dcmi\_get\_extra\_statistics\_info<a name="ZH-CN_TOPIC_0000002517535329"></a>

**函数原型<a name="section52891528074"></a>**

**int dcmi\_get\_extra\_statistics\_info\(int card\_id, int device\_id, int port\_id, struct dcmi\_extra\_statistics\_info \*info\)**

**功能说明<a name="section1882718362712"></a>**

查询网口扩展统计信息，支持查询端口链路状态以下计数：包含pcs err、纠前误码统计、纠后误码统计、未纠正误码统计。

**参数说明<a name="section1961744213719"></a>**

<a name="table1730213792411"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002338977294_row1134310167588"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000002338977294_p143431616195810"><a name="zh-cn_topic_0000002338977294_p143431616195810"></a><a name="zh-cn_topic_0000002338977294_p143431616195810"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000002338977294_p134391615810"><a name="zh-cn_topic_0000002338977294_p134391615810"></a><a name="zh-cn_topic_0000002338977294_p134391615810"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000002338977294_p13343101635816"><a name="zh-cn_topic_0000002338977294_p13343101635816"></a><a name="zh-cn_topic_0000002338977294_p13343101635816"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000002338977294_p7343111612584"><a name="zh-cn_topic_0000002338977294_p7343111612584"></a><a name="zh-cn_topic_0000002338977294_p7343111612584"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002338977294_row183431816175818"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002338977294_p14343111675811"><a name="zh-cn_topic_0000002338977294_p14343111675811"></a><a name="zh-cn_topic_0000002338977294_p14343111675811"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002338977294_p934317169586"><a name="zh-cn_topic_0000002338977294_p934317169586"></a><a name="zh-cn_topic_0000002338977294_p934317169586"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002338977294_p43435167585"><a name="zh-cn_topic_0000002338977294_p43435167585"></a><a name="zh-cn_topic_0000002338977294_p43435167585"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002338977294_p33431716105818"><a name="zh-cn_topic_0000002338977294_p33431716105818"></a><a name="zh-cn_topic_0000002338977294_p33431716105818"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002338977294_row4343131613584"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002338977294_p14343216145818"><a name="zh-cn_topic_0000002338977294_p14343216145818"></a><a name="zh-cn_topic_0000002338977294_p14343216145818"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002338977294_p0343161610582"><a name="zh-cn_topic_0000002338977294_p0343161610582"></a><a name="zh-cn_topic_0000002338977294_p0343161610582"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002338977294_p73432016195812"><a name="zh-cn_topic_0000002338977294_p73432016195812"></a><a name="zh-cn_topic_0000002338977294_p73432016195812"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002338977294_p173432016105817"><a name="zh-cn_topic_0000002338977294_p173432016105817"></a><a name="zh-cn_topic_0000002338977294_p173432016105817"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000002338977294_p1534331611582"><a name="zh-cn_topic_0000002338977294_p1534331611582"></a><a name="zh-cn_topic_0000002338977294_p1534331611582"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p244411717486"><a name="p244411717486"></a><a name="p244411717486"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000002338977294_row7344151655814"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002338977294_p1434471614585"><a name="zh-cn_topic_0000002338977294_p1434471614585"></a><a name="zh-cn_topic_0000002338977294_p1434471614585"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002338977294_p13444161584"><a name="zh-cn_topic_0000002338977294_p13444161584"></a><a name="zh-cn_topic_0000002338977294_p13444161584"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002338977294_p234431619587"><a name="zh-cn_topic_0000002338977294_p234431619587"></a><a name="zh-cn_topic_0000002338977294_p234431619587"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002338977294_p9359145314538"><a name="zh-cn_topic_0000002338977294_p9359145314538"></a><a name="zh-cn_topic_0000002338977294_p9359145314538"></a>NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002338977294_row1234451655813"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002338977294_p534419161587"><a name="zh-cn_topic_0000002338977294_p534419161587"></a><a name="zh-cn_topic_0000002338977294_p534419161587"></a>info</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002338977294_p934481611585"><a name="zh-cn_topic_0000002338977294_p934481611585"></a><a name="zh-cn_topic_0000002338977294_p934481611585"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002338977294_p1934401695815"><a name="zh-cn_topic_0000002338977294_p1934401695815"></a><a name="zh-cn_topic_0000002338977294_p1934401695815"></a>struct dcmi_extra_statistics_info *</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002338977294_p1791511213156"><a name="zh-cn_topic_0000002338977294_p1791511213156"></a><a name="zh-cn_topic_0000002338977294_p1791511213156"></a>struct dcmi_extra_statistics_info {</p>
<p id="zh-cn_topic_0000002338977294_p1691522111155"><a name="zh-cn_topic_0000002338977294_p1691522111155"></a><a name="zh-cn_topic_0000002338977294_p1691522111155"></a>unsigned long long cw_total_cnt; //码字总数</p>
<p id="zh-cn_topic_0000002338977294_p3915172171510"><a name="zh-cn_topic_0000002338977294_p3915172171510"></a><a name="zh-cn_topic_0000002338977294_p3915172171510"></a>unsigned long long cw_before_correct_cnt; //码字纠前误码统计</p>
<p id="zh-cn_topic_0000002338977294_p9915122181510"><a name="zh-cn_topic_0000002338977294_p9915122181510"></a><a name="zh-cn_topic_0000002338977294_p9915122181510"></a>unsigned long long cw_correct_cnt; //码字可纠误码统计</p>
<p id="zh-cn_topic_0000002338977294_p119151621151518"><a name="zh-cn_topic_0000002338977294_p119151621151518"></a><a name="zh-cn_topic_0000002338977294_p119151621151518"></a>unsigned long long cw_uncorrect_cnt; //码字不可纠误码统计</p>
<p id="zh-cn_topic_0000002338977294_p191592116158"><a name="zh-cn_topic_0000002338977294_p191592116158"></a><a name="zh-cn_topic_0000002338977294_p191592116158"></a>unsigned long long cw_bad_cnt; //cw_bad事件次数</p>
<p id="zh-cn_topic_0000002338977294_p1791562113150"><a name="zh-cn_topic_0000002338977294_p1791562113150"></a><a name="zh-cn_topic_0000002338977294_p1791562113150"></a>unsigned long long trans_total_bit; //传输的总bit数</p>
<p id="zh-cn_topic_0000002338977294_p391582171510"><a name="zh-cn_topic_0000002338977294_p391582171510"></a><a name="zh-cn_topic_0000002338977294_p391582171510"></a>unsigned long long cw_total_correct_bit; //码字可纠的总bit数</p>
<p id="zh-cn_topic_0000002338977294_p291512191519"><a name="zh-cn_topic_0000002338977294_p291512191519"></a><a name="zh-cn_topic_0000002338977294_p291512191519"></a>unsigned long long rx_full_drop_cnt; //接收方向buffer满后的丢包计数</p>
<p id="zh-cn_topic_0000002338977294_p17916122113159"><a name="zh-cn_topic_0000002338977294_p17916122113159"></a><a name="zh-cn_topic_0000002338977294_p17916122113159"></a>unsigned long long pcs_err_cnt; //pcs层错误块计数</p>
<p id="zh-cn_topic_0000002338977294_p149165218158"><a name="zh-cn_topic_0000002338977294_p149165218158"></a><a name="zh-cn_topic_0000002338977294_p149165218158"></a>unsigned long long rx_send_app_good_pkts; //发送到APP侧的好帧总数，不包括fc-consumed-error帧。fc-consumed-error：MAC发送标记了abort的流控帧</p>
<p id="zh-cn_topic_0000002338977294_p5916821121518"><a name="zh-cn_topic_0000002338977294_p5916821121518"></a><a name="zh-cn_topic_0000002338977294_p5916821121518"></a>unsigned long long rx_send_app_bad_pkts; //发送到APP侧的坏帧或fc-consumed-error帧总数</p>
<p id="zh-cn_topic_0000002338977294_p18916122141519"><a name="zh-cn_topic_0000002338977294_p18916122141519"></a><a name="zh-cn_topic_0000002338977294_p18916122141519"></a>double correcting_bit_rate; //可纠比特率</p>
<p id="zh-cn_topic_0000002338977294_p169161921181511"><a name="zh-cn_topic_0000002338977294_p169161921181511"></a><a name="zh-cn_topic_0000002338977294_p169161921181511"></a>};</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section132614482713"></a>**

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

**异常处理<a name="section19537911783"></a>**

无

**约束说明<a name="section16624106086"></a>**

对于Atlas 200T A2 Box16 异构子框、Atlas 800T A2 训练服务器、Atlas 800I A2 推理服务器、Atlas 900 A2 PoD 集群基础单元、A200I A2 Box 异构组件，该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table167103125114"></a>
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

**调用示例<a name="section121365111485"></a>**

```
…  
int ret = 0; 
int card_id = 0; 
int device_id = 0; 
int port_id = 0;
struct dcmi_extra_statistics_info info = {0}; 
ret = dcmi_get_extra_statistics_info (card_id, device_id, port_id, &info); 
if (ret != 0){
     //todo：记录日志
     return ret;
} 
…
```

