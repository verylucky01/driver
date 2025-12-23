# dcmi\_get\_ping\_info<a name="ZH-CN_TOPIC_0000002485478744"></a>

**函数原型<a name="section1950145543919"></a>**

**int dcmi\_get\_ping\_info \(int card\_id, int device\_id, int port\_id, struct dcmi\_ping\_operate\_info \*dcmi\_ping, struct dcmi\_ping\_reply\_info \*dcmi\_reply\)**

**功能说明<a name="section651195511395"></a>**

获取指定设备到目的地址的链路连通信息。

**参数说明<a name="section2521655113914"></a>**

<a name="table410945503914"></a>
<table><thead align="left"><tr id="row822915516396"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="p14230355203914"><a name="p14230355203914"></a><a name="p14230355203914"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.2"><p id="p1223075553912"><a name="p1223075553912"></a><a name="p1223075553912"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.5.1.3"><p id="p72301355133916"><a name="p72301355133916"></a><a name="p72301355133916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="p723055516397"><a name="p723055516397"></a><a name="p723055516397"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row102301855133919"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p16230185510391"><a name="p16230185510391"></a><a name="p16230185510391"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p1723045513391"><a name="p1723045513391"></a><a name="p1723045513391"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p1230185573915"><a name="p1230185573915"></a><a name="p1230185573915"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p192301855143912"><a name="p192301855143912"></a><a name="p192301855143912"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row15230175510398"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p523017556393"><a name="p523017556393"></a><a name="p523017556393"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p19230125518397"><a name="p19230125518397"></a><a name="p19230125518397"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p0230205553916"><a name="p0230205553916"></a><a name="p0230205553916"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1423015555395"><a name="p1423015555395"></a><a name="p1423015555395"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p11230125511398"><a name="p11230125511398"></a><a name="p11230125511398"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p723015512391"><a name="p723015512391"></a><a name="p723015512391"></a>MCU芯片：mcu_id。</p>
<p id="p72075349199"><a name="p72075349199"></a><a name="p72075349199"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row22309557394"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p02301655203910"><a name="p02301655203910"></a><a name="p02301655203910"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p22301655193914"><a name="p22301655193914"></a><a name="p22301655193914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p12301455113918"><a name="p12301455113918"></a><a name="p12301455113918"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p82316551391"><a name="p82316551391"></a><a name="p82316551391"></a>NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="row1223110556396"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p15231155163916"><a name="p15231155163916"></a><a name="p15231155163916"></a>dcmi_ping</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p823111559399"><a name="p823111559399"></a><a name="p823111559399"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p15231105512392"><a name="p15231105512392"></a><a name="p15231105512392"></a>struct dcmi_ping_operate_info*</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p12313554399"><a name="p12313554399"></a><a name="p12313554399"></a>ping操作信息。</p>
<p id="p132311755153910"><a name="p132311755153910"></a><a name="p132311755153910"></a>struct dcmi_ping_operate_info {</p>
<p id="p182311355123911"><a name="p182311355123911"></a><a name="p182311355123911"></a>char dst_addr[IP_ADDR_LEN];//指定ping设备的IPv4目的地址，其中IP_ADDR_LEN为16</p>
<p id="p1123112551393"><a name="p1123112551393"></a><a name="p1123112551393"></a>unsigned int sdid;//指定ping设备的sdid值。配置目的地址时，sdid和dst_addr二选一。如果两者都进行了配置，以dst_addr为准</p>
<p id="p18231655203917"><a name="p18231655203917"></a><a name="p18231655203917"></a>unsigned int packet_size;//指定ping的数据包的数据大小，取值范围：1792~3000Byte。其中报文头大小为1024Byte，后续1025Byte~packet_size字段进行正确性校验，每1024Byte的字段类型分布为：0xFF(256Byte)、0x00(256Byte)、0xAA(256Byte)、0x55(256Byte)</p>
<p id="p923114557399"><a name="p923114557399"></a><a name="p923114557399"></a>unsigned int packet_send_num;//指定ping的数据包数量，取值范围：1~1000</p>
<p id="p1223185553919"><a name="p1223185553919"></a><a name="p1223185553919"></a>unsigned int packet_interval;//指定ping的延迟发包间隔，取值范围：0~10ms，0代表收到应答后立即发送</p>
<p id="p72311556394"><a name="p72311556394"></a><a name="p72311556394"></a>unsigned int timeout;//指定ping的每包超时等待时间，取值范围1~20000ms，配置项暂不生效</p>
<p id="p19231455173915"><a name="p19231455173915"></a><a name="p19231455173915"></a>unsigned char reserved[32];//预留字段，暂未使用</p>
<p id="p5231255153917"><a name="p5231255153917"></a><a name="p5231255153917"></a>};</p>
<p id="p143091626124318"><a name="p143091626124318"></a><a name="p143091626124318"></a>注意：</p>
<p id="p167933314147"><a name="p167933314147"></a><a name="p167933314147"></a>25.0.RC1之前的版本以上参数使用时存在如下约束：</p>
<p id="p1126692311438"><a name="p1126692311438"></a><a name="p1126692311438"></a>packet_send_num* (packet_interval+timeout)&lt;=20000ms</p>
</td>
</tr>
<tr id="row823111553398"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p1523195523910"><a name="p1523195523910"></a><a name="p1523195523910"></a>dcmi_reply</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p11231455143911"><a name="p11231455143911"></a><a name="p11231455143911"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p623125510397"><a name="p623125510397"></a><a name="p623125510397"></a>struct dcmi_ping_reply_info*</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p72325553399"><a name="p72325553399"></a><a name="p72325553399"></a>ping结果信息</p>
<p id="p17232115533920"><a name="p17232115533920"></a><a name="p17232115533920"></a>struct dcmi_ping_reply_info {</p>
<p id="p20232175573916"><a name="p20232175573916"></a><a name="p20232175573916"></a>char dst_addr[IP_ADDR_LEN];//ping设备的IPv4目的地址</p>
<p id="p42321755193919"><a name="p42321755193919"></a><a name="p42321755193919"></a>enum dcmi_ping_result ret[DCMI_PING_PACKET_NUM_MAX];//每个数据包ping结果。其中，dcmi_ping_result枚举值包括：0（未发起ping操作）；1（ping报文收发成功）；2（报文发送失败）；3（报文接收失败）。DCMI_PING_PACKET_NUM_MAX为1000</p>
<p id="p192321955123919"><a name="p192321955123919"></a><a name="p192321955123919"></a>unsigned int total_packet_send_num;//发送计数</p>
<p id="p223220555392"><a name="p223220555392"></a><a name="p223220555392"></a>unsigned int total_packet_recv_num;//接收计数</p>
<p id="p023215550392"><a name="p023215550392"></a><a name="p023215550392"></a>long start_tv_sec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作开始时间，s</p>
<p id="p122321055153912"><a name="p122321055153912"></a><a name="p122321055153912"></a>long start_tv_usec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作开始时间，us。s和us搭配，进行时间计算</p>
<p id="p8232655153920"><a name="p8232655153920"></a><a name="p8232655153920"></a>long end_tv_sec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作结束时间，s</p>
<p id="p02325553390"><a name="p02325553390"></a><a name="p02325553390"></a>long end_tv_usec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作结束时间，us。s和us搭配，进行时间计算</p>
<p id="p18232155514391"><a name="p18232155514391"></a><a name="p18232155514391"></a>};</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section3642631135017"></a>**

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

**异常处理<a name="section1464620315507"></a>**

无。

**约束说明<a name="section1100155515399"></a>**

指定设备与目的设备的NPU驱动版本号需保持一致。

**表 1** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section51051555133912"></a>**

```
…
int ret;
int card_id = 0;
int device_id = 0;
int port_id = 0;
struct dcmi_ping_operate_info ping_operate_info = {0};
struct dcmi_ping_reply_info ping_reply_info = {0};
ping_operate_info.sdid = 262146;
ping_operate_info.packet_size = 1792;
ping_operate_info.packet_send_num = 1000;
ping_operate_info.packet_interval = 0;
ping_operate_info.timeout = 20;
ret = dcmi_get_ping_info(card_id, device_id, port_id, &ping_operate_info, &ping_reply_info);
…
```

