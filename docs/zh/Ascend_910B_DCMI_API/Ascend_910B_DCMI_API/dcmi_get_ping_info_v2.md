# dcmi\_get\_ping\_info\_v2<a name="ZH-CN_TOPIC_0000002517615313"></a>

**函数原型<a name="section1950145543919"></a>**

**int dcmi\_get\_ping\_info\_v2 \(int card\_id, int device\_id, int port\_id, struct dcmi\_ping\_operate\_info \*dcmi\_ping, struct dcmi\_ping\_reply\_info\_v2 \*dcmi\_reply\)**

**功能说明<a name="section148881940132513"></a>**

获取指定设备到目的地址的链路连通信息。

**参数说明<a name="section1922412251265"></a>**

<a name="zh-cn_topic_0000002432973666_table410945503914"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002432973666_row822915516396"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000002432973666_p14230355203914"><a name="zh-cn_topic_0000002432973666_p14230355203914"></a><a name="zh-cn_topic_0000002432973666_p14230355203914"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000002432973666_p1223075553912"><a name="zh-cn_topic_0000002432973666_p1223075553912"></a><a name="zh-cn_topic_0000002432973666_p1223075553912"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000002432973666_p72301355133916"><a name="zh-cn_topic_0000002432973666_p72301355133916"></a><a name="zh-cn_topic_0000002432973666_p72301355133916"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000002432973666_p723055516397"><a name="zh-cn_topic_0000002432973666_p723055516397"></a><a name="zh-cn_topic_0000002432973666_p723055516397"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002432973666_row102301855133919"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002432973666_p16230185510391"><a name="zh-cn_topic_0000002432973666_p16230185510391"></a><a name="zh-cn_topic_0000002432973666_p16230185510391"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002432973666_p1723045513391"><a name="zh-cn_topic_0000002432973666_p1723045513391"></a><a name="zh-cn_topic_0000002432973666_p1723045513391"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002432973666_p1230185573915"><a name="zh-cn_topic_0000002432973666_p1230185573915"></a><a name="zh-cn_topic_0000002432973666_p1230185573915"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002432973666_p192301855143912"><a name="zh-cn_topic_0000002432973666_p192301855143912"></a><a name="zh-cn_topic_0000002432973666_p192301855143912"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002432973666_row15230175510398"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002432973666_p523017556393"><a name="zh-cn_topic_0000002432973666_p523017556393"></a><a name="zh-cn_topic_0000002432973666_p523017556393"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002432973666_p19230125518397"><a name="zh-cn_topic_0000002432973666_p19230125518397"></a><a name="zh-cn_topic_0000002432973666_p19230125518397"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002432973666_p0230205553916"><a name="zh-cn_topic_0000002432973666_p0230205553916"></a><a name="zh-cn_topic_0000002432973666_p0230205553916"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002432973666_p1423015555395"><a name="zh-cn_topic_0000002432973666_p1423015555395"></a><a name="zh-cn_topic_0000002432973666_p1423015555395"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000002432973666_p11230125511398"><a name="zh-cn_topic_0000002432973666_p11230125511398"></a><a name="zh-cn_topic_0000002432973666_p11230125511398"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="zh-cn_topic_0000002432973666_p723015512391"><a name="zh-cn_topic_0000002432973666_p723015512391"></a><a name="zh-cn_topic_0000002432973666_p723015512391"></a>MCU芯片：mcu_id。</p>
<p id="p340893364718"><a name="p340893364718"></a><a name="p340893364718"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000002432973666_row22309557394"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002432973666_p02301655203910"><a name="zh-cn_topic_0000002432973666_p02301655203910"></a><a name="zh-cn_topic_0000002432973666_p02301655203910"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002432973666_p22301655193914"><a name="zh-cn_topic_0000002432973666_p22301655193914"></a><a name="zh-cn_topic_0000002432973666_p22301655193914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002432973666_p12301455113918"><a name="zh-cn_topic_0000002432973666_p12301455113918"></a><a name="zh-cn_topic_0000002432973666_p12301455113918"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002432973666_p82316551391"><a name="zh-cn_topic_0000002432973666_p82316551391"></a><a name="zh-cn_topic_0000002432973666_p82316551391"></a>NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002432973666_row1223110556396"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002432973666_p15231155163916"><a name="zh-cn_topic_0000002432973666_p15231155163916"></a><a name="zh-cn_topic_0000002432973666_p15231155163916"></a>dcmi_ping</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002432973666_p823111559399"><a name="zh-cn_topic_0000002432973666_p823111559399"></a><a name="zh-cn_topic_0000002432973666_p823111559399"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002432973666_p15231105512392"><a name="zh-cn_topic_0000002432973666_p15231105512392"></a><a name="zh-cn_topic_0000002432973666_p15231105512392"></a>struct dcmi_ping_operate_info*</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002432973666_p12313554399"><a name="zh-cn_topic_0000002432973666_p12313554399"></a><a name="zh-cn_topic_0000002432973666_p12313554399"></a>ping操作信息。</p>
<p id="zh-cn_topic_0000002432973666_p132311755153910"><a name="zh-cn_topic_0000002432973666_p132311755153910"></a><a name="zh-cn_topic_0000002432973666_p132311755153910"></a>struct dcmi_ping_operate_info {</p>
<p id="zh-cn_topic_0000002432973666_p182311355123911"><a name="zh-cn_topic_0000002432973666_p182311355123911"></a><a name="zh-cn_topic_0000002432973666_p182311355123911"></a>char dst_addr[IP_ADDR_LEN];//指定ping设备的IPv4目的地址，其中IP_ADDR_LEN为16</p>
<p id="zh-cn_topic_0000002432973666_p1123112551393"><a name="zh-cn_topic_0000002432973666_p1123112551393"></a><a name="zh-cn_topic_0000002432973666_p1123112551393"></a>unsigned int sdid;//指定ping设备的sdid值。配置目的地址时，sdid和dst_addr二选一。如果两者都进行了配置，以dst_addr为准</p>
<p id="zh-cn_topic_0000002432973666_p18231655203917"><a name="zh-cn_topic_0000002432973666_p18231655203917"></a><a name="zh-cn_topic_0000002432973666_p18231655203917"></a>unsigned int packet_size;//指定ping的数据包的数据大小，取值范围：1792~3000Byte。其中报文头大小为1024Byte，后续1025Byte~packet_size字段进行正确性校验，每1024Byte的字段类型分布为：0xFF(256Byte)、0x00(256Byte)、0xAA(256Byte)、0x55(256Byte)</p>
<p id="zh-cn_topic_0000002432973666_p923114557399"><a name="zh-cn_topic_0000002432973666_p923114557399"></a><a name="zh-cn_topic_0000002432973666_p923114557399"></a>unsigned int packet_send_num;//指定ping的数据包数量，取值范围：1~1000</p>
<p id="zh-cn_topic_0000002432973666_p1223185553919"><a name="zh-cn_topic_0000002432973666_p1223185553919"></a><a name="zh-cn_topic_0000002432973666_p1223185553919"></a>unsigned int packet_interval;//指定ping的延迟发包间隔，取值范围：0~10ms，0代表收到应答后立即发送</p>
<p id="zh-cn_topic_0000002432973666_p72311556394"><a name="zh-cn_topic_0000002432973666_p72311556394"></a><a name="zh-cn_topic_0000002432973666_p72311556394"></a>unsigned int timeout;//指定ping的每包超时等待时间，取值范围1~20000ms，配置项暂不生效</p>
<p id="zh-cn_topic_0000002432973666_p19231455173915"><a name="zh-cn_topic_0000002432973666_p19231455173915"></a><a name="zh-cn_topic_0000002432973666_p19231455173915"></a>unsigned char reserved[32];//预留字段，暂未使用</p>
<p id="zh-cn_topic_0000002432973666_p5231255153917"><a name="zh-cn_topic_0000002432973666_p5231255153917"></a><a name="zh-cn_topic_0000002432973666_p5231255153917"></a>};</p>
<p id="zh-cn_topic_0000002432973666_p143091626124318"><a name="zh-cn_topic_0000002432973666_p143091626124318"></a><a name="zh-cn_topic_0000002432973666_p143091626124318"></a>注意：</p>
<p id="zh-cn_topic_0000002432973666_p956221181420"><a name="zh-cn_topic_0000002432973666_p956221181420"></a><a name="zh-cn_topic_0000002432973666_p956221181420"></a>25.0.RC1之前的版本以上参数使用时存在如下约束：</p>
<p id="zh-cn_topic_0000002432973666_p1126692311438"><a name="zh-cn_topic_0000002432973666_p1126692311438"></a><a name="zh-cn_topic_0000002432973666_p1126692311438"></a>packet_send_num* (packet_interval+timeout)&lt;=20000ms</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002432973666_row823111553398"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002432973666_p1523195523910"><a name="zh-cn_topic_0000002432973666_p1523195523910"></a><a name="zh-cn_topic_0000002432973666_p1523195523910"></a>dcmi_reply</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002432973666_p11231455143911"><a name="zh-cn_topic_0000002432973666_p11231455143911"></a><a name="zh-cn_topic_0000002432973666_p11231455143911"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002432973666_p623125510397"><a name="zh-cn_topic_0000002432973666_p623125510397"></a><a name="zh-cn_topic_0000002432973666_p623125510397"></a>struct dcmi_ping_reply_info<strong id="zh-cn_topic_0000002432973666_b13513553244"><a name="zh-cn_topic_0000002432973666_b13513553244"></a><a name="zh-cn_topic_0000002432973666_b13513553244"></a>_</strong>v2*</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002432973666_p72325553399"><a name="zh-cn_topic_0000002432973666_p72325553399"></a><a name="zh-cn_topic_0000002432973666_p72325553399"></a>ping结果信息。</p>
<p id="zh-cn_topic_0000002432973666_p17232115533920"><a name="zh-cn_topic_0000002432973666_p17232115533920"></a><a name="zh-cn_topic_0000002432973666_p17232115533920"></a>struct dcmi_ping_reply_info {</p>
<p id="zh-cn_topic_0000002432973666_p20232175573916"><a name="zh-cn_topic_0000002432973666_p20232175573916"></a><a name="zh-cn_topic_0000002432973666_p20232175573916"></a>char dst_addr[IP_ADDR_LEN];//ping设备的IPv4目的地址</p>
<p id="zh-cn_topic_0000002432973666_p42321755193919"><a name="zh-cn_topic_0000002432973666_p42321755193919"></a><a name="zh-cn_topic_0000002432973666_p42321755193919"></a>enum dcmi_ping_result ret[DCMI_PING_PACKET_NUM_MAX];//每个数据包ping结果。其中，dcmi_ping_result枚举值包括：0（未发起ping操作）；1（ping报文收发成功）；2（报文发送失败）；3（报文接收失败）。DCMI_PING_PACKET_NUM_MAX为1000</p>
<p id="zh-cn_topic_0000002432973666_p192321955123919"><a name="zh-cn_topic_0000002432973666_p192321955123919"></a><a name="zh-cn_topic_0000002432973666_p192321955123919"></a>unsigned int total_packet_send_num;//发送计数</p>
<p id="zh-cn_topic_0000002432973666_p223220555392"><a name="zh-cn_topic_0000002432973666_p223220555392"></a><a name="zh-cn_topic_0000002432973666_p223220555392"></a>unsigned int total_packet_recv_num;//接收计数</p>
<p id="zh-cn_topic_0000002432973666_p023215550392"><a name="zh-cn_topic_0000002432973666_p023215550392"></a><a name="zh-cn_topic_0000002432973666_p023215550392"></a>long start_tv_sec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作开始时间，s</p>
<p id="zh-cn_topic_0000002432973666_p122321055153912"><a name="zh-cn_topic_0000002432973666_p122321055153912"></a><a name="zh-cn_topic_0000002432973666_p122321055153912"></a>long start_tv_usec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作开始时间，us。s和us搭配，进行时间计算</p>
<p id="zh-cn_topic_0000002432973666_p8232655153920"><a name="zh-cn_topic_0000002432973666_p8232655153920"></a><a name="zh-cn_topic_0000002432973666_p8232655153920"></a>long end_tv_sec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作结束时间，s</p>
<p id="zh-cn_topic_0000002432973666_p02325553390"><a name="zh-cn_topic_0000002432973666_p02325553390"></a><a name="zh-cn_topic_0000002432973666_p02325553390"></a>long end_tv_usec[DCMI_PING_PACKET_NUM_MAX];//每包ping操作结束时间，us。s和us搭配，进行时间计算</p>
<p id="zh-cn_topic_0000002432973666_p18232155514391"><a name="zh-cn_topic_0000002432973666_p18232155514391"></a><a name="zh-cn_topic_0000002432973666_p18232155514391"></a>};</p>
<p id="zh-cn_topic_0000002432973666_p124352592916"><a name="zh-cn_topic_0000002432973666_p124352592916"></a><a name="zh-cn_topic_0000002432973666_p124352592916"></a>struct dcmi_ping_reply_info_v2 {</p>
<p id="zh-cn_topic_0000002432973666_p94351459396"><a name="zh-cn_topic_0000002432973666_p94351459396"></a><a name="zh-cn_topic_0000002432973666_p94351459396"></a>struct dcmi_ping_reply_info info;</p>
<p id="zh-cn_topic_0000002432973666_p144359591598"><a name="zh-cn_topic_0000002432973666_p144359591598"></a><a name="zh-cn_topic_0000002432973666_p144359591598"></a>unsigned char L1_plane_check_res [DCMI_PING_PACKET_NUM_MAX];//每次ping时，接收数据存在故障的L1平面</p>
<p id="zh-cn_topic_0000002432973666_p124351659392"><a name="zh-cn_topic_0000002432973666_p124351659392"></a><a name="zh-cn_topic_0000002432973666_p124351659392"></a>unsigned char reserved[DCMI_PING_PACKET_NUM_MAX];//预留</p>
<p id="zh-cn_topic_0000002432973666_p14435205915917"><a name="zh-cn_topic_0000002432973666_p14435205915917"></a><a name="zh-cn_topic_0000002432973666_p14435205915917"></a>};</p>
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

**约束说明<a name="section1100155515399"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table18534941103615"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1210513304816"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p558011817325"><a name="zh-cn_topic_0000002485295476_p558011817325"></a><a name="zh-cn_topic_0000002485295476_p558011817325"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p25806812321"><a name="zh-cn_topic_0000002485295476_p25806812321"></a><a name="zh-cn_topic_0000002485295476_p25806812321"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p558018833211"><a name="zh-cn_topic_0000002485295476_p558018833211"></a><a name="zh-cn_topic_0000002485295476_p558018833211"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p35801189326"><a name="zh-cn_topic_0000002485295476_p35801189326"></a><a name="zh-cn_topic_0000002485295476_p35801189326"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row14576182015105"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p13661881916"><a name="zh-cn_topic_0000002485295476_p13661881916"></a><a name="zh-cn_topic_0000002485295476_p13661881916"></a><span id="zh-cn_topic_0000002485295476_ph116612081298"><a name="zh-cn_topic_0000002485295476_ph116612081298"></a><a name="zh-cn_topic_0000002485295476_ph116612081298"></a><span id="zh-cn_topic_0000002485295476_text26611487916"><a name="zh-cn_topic_0000002485295476_text26611487916"></a><a name="zh-cn_topic_0000002485295476_text26611487916"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p057642081019"><a name="zh-cn_topic_0000002485295476_p057642081019"></a><a name="zh-cn_topic_0000002485295476_p057642081019"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p65761200102"><a name="zh-cn_topic_0000002485295476_p65761200102"></a><a name="zh-cn_topic_0000002485295476_p65761200102"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1357610206101"><a name="zh-cn_topic_0000002485295476_p1357610206101"></a><a name="zh-cn_topic_0000002485295476_p1357610206101"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row318512127818"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p86611581596"><a name="zh-cn_topic_0000002485295476_p86611581596"></a><a name="zh-cn_topic_0000002485295476_p86611581596"></a><span id="zh-cn_topic_0000002485295476_text66619818911"><a name="zh-cn_topic_0000002485295476_text66619818911"></a><a name="zh-cn_topic_0000002485295476_text66619818911"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1441443298"><a name="zh-cn_topic_0000002485295476_p1441443298"></a><a name="zh-cn_topic_0000002485295476_p1441443298"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p2420631892"><a name="zh-cn_topic_0000002485295476_p2420631892"></a><a name="zh-cn_topic_0000002485295476_p2420631892"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p18425123899"><a name="zh-cn_topic_0000002485295476_p18425123899"></a><a name="zh-cn_topic_0000002485295476_p18425123899"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1273713241084"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p56611781094"><a name="zh-cn_topic_0000002485295476_p56611781094"></a><a name="zh-cn_topic_0000002485295476_p56611781094"></a><span id="zh-cn_topic_0000002485295476_text56611281693"><a name="zh-cn_topic_0000002485295476_text56611281693"></a><a name="zh-cn_topic_0000002485295476_text56611281693"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p64311736915"><a name="zh-cn_topic_0000002485295476_p64311736915"></a><a name="zh-cn_topic_0000002485295476_p64311736915"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p04391434914"><a name="zh-cn_topic_0000002485295476_p04391434914"></a><a name="zh-cn_topic_0000002485295476_p04391434914"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p244917315919"><a name="zh-cn_topic_0000002485295476_p244917315919"></a><a name="zh-cn_topic_0000002485295476_p244917315919"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row7672192219815"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p166611689914"><a name="zh-cn_topic_0000002485295476_p166611689914"></a><a name="zh-cn_topic_0000002485295476_p166611689914"></a><span id="zh-cn_topic_0000002485295476_text126611581798"><a name="zh-cn_topic_0000002485295476_text126611581798"></a><a name="zh-cn_topic_0000002485295476_text126611581798"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p12459143996"><a name="zh-cn_topic_0000002485295476_p12459143996"></a><a name="zh-cn_topic_0000002485295476_p12459143996"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p10470531895"><a name="zh-cn_topic_0000002485295476_p10470531895"></a><a name="zh-cn_topic_0000002485295476_p10470531895"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p12476033912"><a name="zh-cn_topic_0000002485295476_p12476033912"></a><a name="zh-cn_topic_0000002485295476_p12476033912"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row153452020881"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p86611181098"><a name="zh-cn_topic_0000002485295476_p86611181098"></a><a name="zh-cn_topic_0000002485295476_p86611181098"></a><span id="zh-cn_topic_0000002485295476_text16611819911"><a name="zh-cn_topic_0000002485295476_text16611819911"></a><a name="zh-cn_topic_0000002485295476_text16611819911"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p134813311917"><a name="zh-cn_topic_0000002485295476_p134813311917"></a><a name="zh-cn_topic_0000002485295476_p134813311917"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p3486231596"><a name="zh-cn_topic_0000002485295476_p3486231596"></a><a name="zh-cn_topic_0000002485295476_p3486231596"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p44911135913"><a name="zh-cn_topic_0000002485295476_p44911135913"></a><a name="zh-cn_topic_0000002485295476_p44911135913"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row20496217988"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p86611689916"><a name="zh-cn_topic_0000002485295476_p86611689916"></a><a name="zh-cn_topic_0000002485295476_p86611689916"></a><span id="zh-cn_topic_0000002485295476_text966158896"><a name="zh-cn_topic_0000002485295476_text966158896"></a><a name="zh-cn_topic_0000002485295476_text966158896"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p649613392"><a name="zh-cn_topic_0000002485295476_p649613392"></a><a name="zh-cn_topic_0000002485295476_p649613392"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p2050114313914"><a name="zh-cn_topic_0000002485295476_p2050114313914"></a><a name="zh-cn_topic_0000002485295476_p2050114313914"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p175075312918"><a name="zh-cn_topic_0000002485295476_p175075312918"></a><a name="zh-cn_topic_0000002485295476_p175075312918"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1775216157819"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p766118812915"><a name="zh-cn_topic_0000002485295476_p766118812915"></a><a name="zh-cn_topic_0000002485295476_p766118812915"></a><span id="zh-cn_topic_0000002485295476_text136611481596"><a name="zh-cn_topic_0000002485295476_text136611481596"></a><a name="zh-cn_topic_0000002485295476_text136611481596"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p205121037920"><a name="zh-cn_topic_0000002485295476_p205121037920"></a><a name="zh-cn_topic_0000002485295476_p205121037920"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p5517131993"><a name="zh-cn_topic_0000002485295476_p5517131993"></a><a name="zh-cn_topic_0000002485295476_p5517131993"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p115221437916"><a name="zh-cn_topic_0000002485295476_p115221437916"></a><a name="zh-cn_topic_0000002485295476_p115221437916"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1499855417336"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p48161158173310"><a name="zh-cn_topic_0000002485295476_p48161158173310"></a><a name="zh-cn_topic_0000002485295476_p48161158173310"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section20288321102711"></a>**

```
…
int ret;
int card_id = 0;
int device_id = 0;
int port_id = 0;
struct dcmi_ping_operate_info ping_operate_info = {0};
struct dcmi_ping_reply_info_v2 ping_reply_info = {0};
ping_operate_info.sdid = 262146;
ping_operate_info.packet_size = 1792;
ping_operate_info.packet_send_num = 1000;
ping_operate_info.packet_interval = 0;
ping_operate_info.timeout = 20;
ret = dcmi_get_ping_info_v2(card_id, device_id, port_id, &ping_operate_info, &ping_reply_info);
…
```

