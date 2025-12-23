# dcmi\_get\_hccsping\_mesh\_info\_v2<a name="ZH-CN_TOPIC_0000002517615389"></a>

**函数原型<a name="section1950145543919"></a>**

**int dcmi\_get\_hccsping\_mesh\_info\_v2 \(int card\_id, int device\_id, int port\_id, unsigned int task\_id, struct dcmi\_hccsping\_mesh\_info\_v2 \*hccsping\_mesh\_reply\)**

**功能说明<a name="section975353882910"></a>**

查询ping mesh统计信息。

**参数说明<a name="section3137164492920"></a>**

<a name="zh-cn_topic_0000002466505445_table187981424174912"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002466505445_row987812404919"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000002466505445_p11878132484912"><a name="zh-cn_topic_0000002466505445_p11878132484912"></a><a name="zh-cn_topic_0000002466505445_p11878132484912"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000002466505445_p13878124174912"><a name="zh-cn_topic_0000002466505445_p13878124174912"></a><a name="zh-cn_topic_0000002466505445_p13878124174912"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.39%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000002466505445_p17878182444918"><a name="zh-cn_topic_0000002466505445_p17878182444918"></a><a name="zh-cn_topic_0000002466505445_p17878182444918"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.29%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000002466505445_p2087882410494"><a name="zh-cn_topic_0000002466505445_p2087882410494"></a><a name="zh-cn_topic_0000002466505445_p2087882410494"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002466505445_row587822454918"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002466505445_p18781243499"><a name="zh-cn_topic_0000002466505445_p18781243499"></a><a name="zh-cn_topic_0000002466505445_p18781243499"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002466505445_p18878224164919"><a name="zh-cn_topic_0000002466505445_p18878224164919"></a><a name="zh-cn_topic_0000002466505445_p18878224164919"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002466505445_p787862411495"><a name="zh-cn_topic_0000002466505445_p787862411495"></a><a name="zh-cn_topic_0000002466505445_p787862411495"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002466505445_p128787249495"><a name="zh-cn_topic_0000002466505445_p128787249495"></a><a name="zh-cn_topic_0000002466505445_p128787249495"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002466505445_row687852416499"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002466505445_p158781924154917"><a name="zh-cn_topic_0000002466505445_p158781924154917"></a><a name="zh-cn_topic_0000002466505445_p158781924154917"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002466505445_p14878124114920"><a name="zh-cn_topic_0000002466505445_p14878124114920"></a><a name="zh-cn_topic_0000002466505445_p14878124114920"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002466505445_p118786247493"><a name="zh-cn_topic_0000002466505445_p118786247493"></a><a name="zh-cn_topic_0000002466505445_p118786247493"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002466505445_p187872444910"><a name="zh-cn_topic_0000002466505445_p187872444910"></a><a name="zh-cn_topic_0000002466505445_p187872444910"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000002466505445_p687882424914"><a name="zh-cn_topic_0000002466505445_p687882424914"></a><a name="zh-cn_topic_0000002466505445_p687882424914"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p6808132911484"><a name="p6808132911484"></a><a name="p6808132911484"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000002466505445_row10878132414910"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002466505445_p68781424134910"><a name="zh-cn_topic_0000002466505445_p68781424134910"></a><a name="zh-cn_topic_0000002466505445_p68781424134910"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002466505445_p387812242498"><a name="zh-cn_topic_0000002466505445_p387812242498"></a><a name="zh-cn_topic_0000002466505445_p387812242498"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002466505445_p6878424104910"><a name="zh-cn_topic_0000002466505445_p6878424104910"></a><a name="zh-cn_topic_0000002466505445_p6878424104910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002466505445_p18879624114919"><a name="zh-cn_topic_0000002466505445_p18879624114919"></a><a name="zh-cn_topic_0000002466505445_p18879624114919"></a>预留，NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002466505445_row58791324154910"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002466505445_p4879172444919"><a name="zh-cn_topic_0000002466505445_p4879172444919"></a><a name="zh-cn_topic_0000002466505445_p4879172444919"></a>task_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002466505445_p887916240499"><a name="zh-cn_topic_0000002466505445_p887916240499"></a><a name="zh-cn_topic_0000002466505445_p887916240499"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002466505445_p287972414494"><a name="zh-cn_topic_0000002466505445_p287972414494"></a><a name="zh-cn_topic_0000002466505445_p287972414494"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002466505445_p18791524154916"><a name="zh-cn_topic_0000002466505445_p18791524154916"></a><a name="zh-cn_topic_0000002466505445_p18791524154916"></a>任务号，取值范围[0, 1]。与dcmi_start_hccsping_mesh传入的任务号对应。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002466505445_row58791424194912"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002466505445_p787952434918"><a name="zh-cn_topic_0000002466505445_p787952434918"></a><a name="zh-cn_topic_0000002466505445_p787952434918"></a>hccsping_mesh_reply</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002466505445_p08791624124911"><a name="zh-cn_topic_0000002466505445_p08791624124911"></a><a name="zh-cn_topic_0000002466505445_p08791624124911"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002466505445_p15879152413494"><a name="zh-cn_topic_0000002466505445_p15879152413494"></a><a name="zh-cn_topic_0000002466505445_p15879152413494"></a>struct dcmi_hccsping_mesh_info <strong id="zh-cn_topic_0000002466505445_b4722927152411"><a name="zh-cn_topic_0000002466505445_b4722927152411"></a><a name="zh-cn_topic_0000002466505445_b4722927152411"></a>_</strong>v2*</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002466505445_p1687918249497"><a name="zh-cn_topic_0000002466505445_p1687918249497"></a><a name="zh-cn_topic_0000002466505445_p1687918249497"></a>指定一个ping mesh常驻任务的统计信息，包含dst_addr、suc_pkt_num、fail_pkt_num、max_time、min_time、avg_time、tp95_time、reply_stat_num、ping_total_num、dest_num。</p>
<p id="zh-cn_topic_0000002466505445_p135692064411"><a name="zh-cn_topic_0000002466505445_p135692064411"></a><a name="zh-cn_topic_0000002466505445_p135692064411"></a>struct dcmi_hccsping_mesh_info {</p>
<p id="zh-cn_topic_0000002466505445_p135691613412"><a name="zh-cn_topic_0000002466505445_p135691613412"></a><a name="zh-cn_topic_0000002466505445_p135691613412"></a>char dst_addr[HCCS_PING_MESH_MAX_NUM][ADDR_MAX_LEN];//目的地址列表</p>
<p id="zh-cn_topic_0000002466505445_p20569762045"><a name="zh-cn_topic_0000002466505445_p20569762045"></a><a name="zh-cn_topic_0000002466505445_p20569762045"></a>unsigned int suc_pkt_num[HCCS_PING_MESH_MAX_NUM];//ping成功次数</p>
<p id="zh-cn_topic_0000002466505445_p25691962048"><a name="zh-cn_topic_0000002466505445_p25691962048"></a><a name="zh-cn_topic_0000002466505445_p25691962048"></a>unsigned int fail_pkt_num[HCCS_PING_MESH_MAX_NUM];//ping失败次数</p>
<p id="zh-cn_topic_0000002466505445_p156912615417"><a name="zh-cn_topic_0000002466505445_p156912615417"></a><a name="zh-cn_topic_0000002466505445_p156912615417"></a>long max_time[HCCS_PING_MESH_MAX_NUM];//ping最大时延</p>
<p id="zh-cn_topic_0000002466505445_p145691665420"><a name="zh-cn_topic_0000002466505445_p145691665420"></a><a name="zh-cn_topic_0000002466505445_p145691665420"></a>long min_time[HCCS_PING_MESH_MAX_NUM];//最小时延</p>
<p id="zh-cn_topic_0000002466505445_p556996744"><a name="zh-cn_topic_0000002466505445_p556996744"></a><a name="zh-cn_topic_0000002466505445_p556996744"></a>long avg_time[HCCS_PING_MESH_MAX_NUM];//ping平均时延</p>
<p id="zh-cn_topic_0000002466505445_p1156910618417"><a name="zh-cn_topic_0000002466505445_p1156910618417"></a><a name="zh-cn_topic_0000002466505445_p1156910618417"></a>long tp95_time[HCCS_PING_MESH_MAX_NUM];//ping时延TP95分位数</p>
<p id="zh-cn_topic_0000002466505445_p15569463417"><a name="zh-cn_topic_0000002466505445_p15569463417"></a><a name="zh-cn_topic_0000002466505445_p15569463417"></a>int reply_stat_num[HCCS_PING_MESH_MAX_NUM];//统计结果所用样本轮数</p>
<p id="zh-cn_topic_0000002466505445_p8569262417"><a name="zh-cn_topic_0000002466505445_p8569262417"></a><a name="zh-cn_topic_0000002466505445_p8569262417"></a>unsigned long long ping_total_num[HCCS_PING_MESH_MAX_NUM];//常驻任务开启以来ping的总轮数</p>
<p id="zh-cn_topic_0000002466505445_p1656946246"><a name="zh-cn_topic_0000002466505445_p1656946246"></a><a name="zh-cn_topic_0000002466505445_p1656946246"></a>int dest_num;//目的地址数量</p>
<p id="zh-cn_topic_0000002466505445_p135697614416"><a name="zh-cn_topic_0000002466505445_p135697614416"></a><a name="zh-cn_topic_0000002466505445_p135697614416"></a>};</p>
<p id="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p0170135110169"><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p0170135110169"></a><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p0170135110169"></a>struct dcmi_hccsping_mesh_info_v2 {</p>
<p id="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p1317014517162"><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p1317014517162"></a><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p1317014517162"></a>struct dcmi_hccsping_mesh_info info;</p>
<p id="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p131707517164"><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p131707517164"></a><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p131707517164"></a>unsigned char L1_plane_check_res[HCCS_PING_MESH_MAX_NUM]; // 到每个目的地址的HCCS链路存在故障的L1平面汇总</p>
<p id="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p141701851121613"><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p141701851121613"></a><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p141701851121613"></a>unsigned char reserved[HCCS_PING_MESH_MAX_NUM];//预留</p>
<p id="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p16170185113169"><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p16170185113169"></a><a name="zh-cn_topic_0000002466505445_zh-cn_topic_0000002338976614_p16170185113169"></a>};</p>
<p id="p664173410486"><a name="p664173410486"></a><a name="p664173410486"></a></p>
<div class="note" id="zh-cn_topic_0000002466505445_note8393718184"><a name="zh-cn_topic_0000002466505445_note8393718184"></a><a name="zh-cn_topic_0000002466505445_note8393718184"></a><span class="notetitle"> 说明： </span><div class="notebody"><a name="zh-cn_topic_0000002466505445_ul97018984016"></a><a name="zh-cn_topic_0000002466505445_ul97018984016"></a><ul id="zh-cn_topic_0000002466505445_ul97018984016"><li>输出的结构体中，目的地址在dst_addr的下标与ping该地址的统计结果在各个数组中的下标相同。例如，ping dst_addr[0]的成功次数是suc_pkt_num[0]，失败次数是fail_pkt_num[0]，以此类推。</li><li>当max_time、min_time、avg_time、tp95_time的值为-1时，表示对应目的地址未ping通。</li></ul>
</div></div>
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

**调用示例<a name="section53462402304"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
unsigned int task_id = 0;
struct dcmi_hccsping_mesh_info_v2 hccsping_mesh_reply = {0};
ret = dcmi_get_hccsping_mesh_info_v2 (card_id, device_id, port_id, task_id, &hccsping_mesh_reply);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

