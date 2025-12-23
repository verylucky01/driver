# dcmi\_start\_hccsping\_mesh<a name="ZH-CN_TOPIC_0000002485478680"></a>

**函数原型<a name="section141261468499"></a>**

**int dcmi\_start\_hccsping\_mesh \(int card\_id, int device\_id, int port\_id, struct dcmi\_hccsping\_mesh\_operate \*hccsping\_mesh\)**

**功能说明<a name="section141289624910"></a>**

开启ping mesh常驻任务。

**参数说明<a name="section212966134919"></a>**

<a name="table317013604918"></a>
<table><thead align="left"><tr id="row10250156104917"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p525013624918"><a name="p525013624918"></a><a name="p525013624918"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p1225011617491"><a name="p1225011617491"></a><a name="p1225011617491"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="p52501614493"><a name="p52501614493"></a><a name="p52501614493"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p82504616492"><a name="p82504616492"></a><a name="p82504616492"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row125017610497"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p122501661492"><a name="p122501661492"></a><a name="p122501661492"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p825020674912"><a name="p825020674912"></a><a name="p825020674912"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p625019613493"><a name="p625019613493"></a><a name="p625019613493"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p92501269490"><a name="p92501269490"></a><a name="p92501269490"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1325026204911"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p425056184914"><a name="p425056184914"></a><a name="p425056184914"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p1125115634916"><a name="p1125115634916"></a><a name="p1125115634916"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p1025126134914"><a name="p1025126134914"></a><a name="p1025126134914"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p12511669493"><a name="p12511669493"></a><a name="p12511669493"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p12251176204918"><a name="p12251176204918"></a><a name="p12251176204918"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p142422917200"><a name="p142422917200"></a><a name="p142422917200"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row132516624913"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p52518654916"><a name="p52518654916"></a><a name="p52518654916"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p162512694916"><a name="p162512694916"></a><a name="p162512694916"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p32513613495"><a name="p32513613495"></a><a name="p32513613495"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p1025116614913"><a name="p1025116614913"></a><a name="p1025116614913"></a>预留，NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="row112529634916"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p125216154914"><a name="p125216154914"></a><a name="p125216154914"></a>hccsping_mesh</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p14252116164919"><a name="p14252116164919"></a><a name="p14252116164919"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p1525218618492"><a name="p1525218618492"></a><a name="p1525218618492"></a>struct dcmi_hccsping_mesh_operate *</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p152531065495"><a name="p152531065495"></a><a name="p152531065495"></a>开启pingmesh常驻任务的相关信息，包含dst_addr_list、pkt_size、pkt_send_num、pkt_interval、task_interval、task_id、timeout。</p>
<p id="p783344182011"><a name="p783344182011"></a><a name="p783344182011"></a>#define IP_ADDR_LIST_LEN 1024</p>
<p id="p4152722001"><a name="p4152722001"></a><a name="p4152722001"></a>struct dcmi_hccsping_mesh_operate {</p>
<p id="p61525210010"><a name="p61525210010"></a><a name="p61525210010"></a>char dst_addr_list[IP_ADDR_LIST_LEN];//IPv4地址/sdid列表，二者选其一，以英文逗号分割</p>
<p id="p615242601"><a name="p615242601"></a><a name="p615242601"></a>int pkt_size;//ping报文大小，取值范围[1792, 3000]，单位Byte。</p>
<p id="p18231655203917"><a name="p18231655203917"></a><a name="p18231655203917"></a>其中报文头大小为1024Byte，后续1025Byte~pkt_size字段进行正确性校验，每1024Byte的字段类型分布为：0xFF(256Byte)、0x00(256Byte)、0xAA(256Byte)、0x55(256Byte)</p>
<p id="p141521021400"><a name="p141521021400"></a><a name="p141521021400"></a>int pkt_send_num;//ping报文发送数量，取值范围[1, 1000]</p>
<p id="p1215211211019"><a name="p1215211211019"></a><a name="p1215211211019"></a>int pkt_interval;//ping报文间隔，取值范围[0, 1000]，单位ms</p>
<p id="p69366361678"><a name="p69366361678"></a><a name="p69366361678"></a>int timeout;//预留，报文超时时间，timeout&gt;=0</p>
<p id="p11152621109"><a name="p11152621109"></a><a name="p11152621109"></a>int task_interval;//常驻任务轮询间隔，取值范围[1, 60]，单位s</p>
<p id="p915216211016"><a name="p915216211016"></a><a name="p915216211016"></a>int task_id;//任务号，取值范围[0, 1]</p>
<p id="p10152521109"><a name="p10152521109"></a><a name="p10152521109"></a>};</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section141481563491"></a>**

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

**异常处理<a name="section1116014613495"></a>**

无。

**约束说明<a name="section15160963493"></a>**

-   指定设备与目的设备的NPU驱动版本号需保持一致。
-   为了减少对业务的影响，建议用户业务迁出或停止业务后再使用该接口。
-   该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table1997161105818"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row163024263717"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p1041510392"><a name="zh-cn_topic_0000002485318818_p1041510392"></a><a name="zh-cn_topic_0000002485318818_p1041510392"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p10417512392"><a name="zh-cn_topic_0000002485318818_p10417512392"></a><a name="zh-cn_topic_0000002485318818_p10417512392"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p1341851173915"><a name="zh-cn_topic_0000002485318818_p1341851173915"></a><a name="zh-cn_topic_0000002485318818_p1341851173915"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p3435114394"><a name="zh-cn_topic_0000002485318818_p3435114394"></a><a name="zh-cn_topic_0000002485318818_p3435114394"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row1030104243717"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1830184219376"><a name="zh-cn_topic_0000002485318818_p1830184219376"></a><a name="zh-cn_topic_0000002485318818_p1830184219376"></a><span id="zh-cn_topic_0000002485318818_text83017424375"><a name="zh-cn_topic_0000002485318818_text83017424375"></a><a name="zh-cn_topic_0000002485318818_text83017424375"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p130242163712"><a name="zh-cn_topic_0000002485318818_p130242163712"></a><a name="zh-cn_topic_0000002485318818_p130242163712"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p9487578148"><a name="zh-cn_topic_0000002485318818_p9487578148"></a><a name="zh-cn_topic_0000002485318818_p9487578148"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1026715119156"><a name="zh-cn_topic_0000002485318818_p1026715119156"></a><a name="zh-cn_topic_0000002485318818_p1026715119156"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row10308422379"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p12301642143712"><a name="zh-cn_topic_0000002485318818_p12301642143712"></a><a name="zh-cn_topic_0000002485318818_p12301642143712"></a><span id="zh-cn_topic_0000002485318818_text1930114213371"><a name="zh-cn_topic_0000002485318818_text1930114213371"></a><a name="zh-cn_topic_0000002485318818_text1930114213371"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p03034253714"><a name="zh-cn_topic_0000002485318818_p03034253714"></a><a name="zh-cn_topic_0000002485318818_p03034253714"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p6481457181412"><a name="zh-cn_topic_0000002485318818_p6481457181412"></a><a name="zh-cn_topic_0000002485318818_p6481457181412"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p32683118153"><a name="zh-cn_topic_0000002485318818_p32683118153"></a><a name="zh-cn_topic_0000002485318818_p32683118153"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row14661158135410"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p23113818160"><a name="zh-cn_topic_0000002485318818_p23113818160"></a><a name="zh-cn_topic_0000002485318818_p23113818160"></a><span id="zh-cn_topic_0000002485318818_text12311789168"><a name="zh-cn_topic_0000002485318818_text12311789168"></a><a name="zh-cn_topic_0000002485318818_text12311789168"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p653191095518"><a name="zh-cn_topic_0000002485318818_p653191095518"></a><a name="zh-cn_topic_0000002485318818_p653191095518"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p18531101018557"><a name="zh-cn_topic_0000002485318818_p18531101018557"></a><a name="zh-cn_topic_0000002485318818_p18531101018557"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p85319108552"><a name="zh-cn_topic_0000002485318818_p85319108552"></a><a name="zh-cn_topic_0000002485318818_p85319108552"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row19188145615284"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p111821034299"><a name="zh-cn_topic_0000002485318818_p111821034299"></a><a name="zh-cn_topic_0000002485318818_p111821034299"></a><span id="zh-cn_topic_0000002485318818_text11821030299"><a name="zh-cn_topic_0000002485318818_text11821030299"></a><a name="zh-cn_topic_0000002485318818_text11821030299"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1365116614298"><a name="zh-cn_topic_0000002485318818_p1365116614298"></a><a name="zh-cn_topic_0000002485318818_p1365116614298"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p10651186172913"><a name="zh-cn_topic_0000002485318818_p10651186172913"></a><a name="zh-cn_topic_0000002485318818_p10651186172913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p14651176142917"><a name="zh-cn_topic_0000002485318818_p14651176142917"></a><a name="zh-cn_topic_0000002485318818_p14651176142917"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1472160165512"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p134575595519"><a name="zh-cn_topic_0000002485318818_p134575595519"></a><a name="zh-cn_topic_0000002485318818_p134575595519"></a><span id="zh-cn_topic_0000002485318818_text5457657553"><a name="zh-cn_topic_0000002485318818_text5457657553"></a><a name="zh-cn_topic_0000002485318818_text5457657553"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p71421111559"><a name="zh-cn_topic_0000002485318818_p71421111559"></a><a name="zh-cn_topic_0000002485318818_p71421111559"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p8141511165511"><a name="zh-cn_topic_0000002485318818_p8141511165511"></a><a name="zh-cn_topic_0000002485318818_p8141511165511"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1914171165517"><a name="zh-cn_topic_0000002485318818_p1914171165517"></a><a name="zh-cn_topic_0000002485318818_p1914171165517"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1983816248811"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1172291987"><a name="zh-cn_topic_0000002485318818_p1172291987"></a><a name="zh-cn_topic_0000002485318818_p1172291987"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section181654611496"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
struct dcmi_hccsping_mesh_operate hccsping_mesh = {
        .dst_addr_list = "192.xx.xx.1,192.xx.xx.2,192.xx.xx.3",
        .pkt_size = 1792,
        .pkt_send_num = 4,
        .pkt_interval = 1,
        .timeout = 1,
        .task_interval = 1,
        .task_id = 0
    };
ret = dcmi_start_hccsping_mesh (card_id, device_id, port_id, &hccsping_mesh);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

