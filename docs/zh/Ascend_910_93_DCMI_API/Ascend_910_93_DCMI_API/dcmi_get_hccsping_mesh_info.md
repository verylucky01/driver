# dcmi\_get\_hccsping\_mesh\_info<a name="ZH-CN_TOPIC_0000002517638661"></a>

**函数原型<a name="section12766142411492"></a>**

**int dcmi\_get\_hccsping\_mesh\_info \(int card\_id, int device\_id, int port\_id, unsigned int task\_id, struct dcmi\_hccsping\_mesh\_info \*hccsping\_mesh\_reply\)**

**功能说明<a name="section6767424194915"></a>**

查询ping mesh统计信息。

**参数说明<a name="section13767132484917"></a>**

<a name="table187981424174912"></a>
<table><thead align="left"><tr id="row987812404919"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p11878132484912"><a name="p11878132484912"></a><a name="p11878132484912"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p13878124174912"><a name="p13878124174912"></a><a name="p13878124174912"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.39%" id="mcps1.1.5.1.3"><p id="p17878182444918"><a name="p17878182444918"></a><a name="p17878182444918"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.29%" id="mcps1.1.5.1.4"><p id="p2087882410494"><a name="p2087882410494"></a><a name="p2087882410494"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row587822454918"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p18781243499"><a name="p18781243499"></a><a name="p18781243499"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p18878224164919"><a name="p18878224164919"></a><a name="p18878224164919"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="p787862411495"><a name="p787862411495"></a><a name="p787862411495"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="p128787249495"><a name="p128787249495"></a><a name="p128787249495"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row687852416499"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p158781924154917"><a name="p158781924154917"></a><a name="p158781924154917"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p14878124114920"><a name="p14878124114920"></a><a name="p14878124114920"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="p118786247493"><a name="p118786247493"></a><a name="p118786247493"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="p187872444910"><a name="p187872444910"></a><a name="p187872444910"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p687882424914"><a name="p687882424914"></a><a name="p687882424914"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p13208143216206"><a name="p13208143216206"></a><a name="p13208143216206"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row10878132414910"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p68781424134910"><a name="p68781424134910"></a><a name="p68781424134910"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p387812242498"><a name="p387812242498"></a><a name="p387812242498"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="p6878424104910"><a name="p6878424104910"></a><a name="p6878424104910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="p18879624114919"><a name="p18879624114919"></a><a name="p18879624114919"></a>预留，NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="row58791324154910"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p4879172444919"><a name="p4879172444919"></a><a name="p4879172444919"></a>task_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p887916240499"><a name="p887916240499"></a><a name="p887916240499"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="p287972414494"><a name="p287972414494"></a><a name="p287972414494"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="p18791524154916"><a name="p18791524154916"></a><a name="p18791524154916"></a>任务号，取值范围[0, 1]。与dcmi_start_hccsping_mesh传入的任务号对应。</p>
</td>
</tr>
<tr id="row58791424194912"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p787952434918"><a name="p787952434918"></a><a name="p787952434918"></a>hccsping_mesh_reply</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p08791624124911"><a name="p08791624124911"></a><a name="p08791624124911"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.39%" headers="mcps1.1.5.1.3 "><p id="p15879152413494"><a name="p15879152413494"></a><a name="p15879152413494"></a>struct dcmi_hccsping_mesh_info *</p>
</td>
<td class="cellrowborder" valign="top" width="50.29%" headers="mcps1.1.5.1.4 "><p id="p1687918249497"><a name="p1687918249497"></a><a name="p1687918249497"></a>指定一个ping mesh常驻任务的统计信息，包含dst_addr、suc_pkt_num、fail_pkt_num、max_time、min_time、avg_time、tp95_time、reply_stat_num、ping_total_num、dest_num：</p>
<p id="p135692064411"><a name="p135692064411"></a><a name="p135692064411"></a>struct dcmi_hccsping_mesh_info {</p>
<p id="p135691613412"><a name="p135691613412"></a><a name="p135691613412"></a>char dst_addr[HCCS_PING_MESH_MAX_NUM][ADDR_MAX_LEN];//目的地址列表</p>
<p id="p20569762045"><a name="p20569762045"></a><a name="p20569762045"></a>unsigned int suc_pkt_num[HCCS_PING_MESH_MAX_NUM];//ping成功次数</p>
<p id="p25691962048"><a name="p25691962048"></a><a name="p25691962048"></a>unsigned int fail_pkt_num[HCCS_PING_MESH_MAX_NUM];//ping失败次数</p>
<p id="p156912615417"><a name="p156912615417"></a><a name="p156912615417"></a>long max_time[HCCS_PING_MESH_MAX_NUM];//ping最大时延</p>
<p id="p145691665420"><a name="p145691665420"></a><a name="p145691665420"></a>long min_time[HCCS_PING_MESH_MAX_NUM];//最小时延</p>
<p id="p556996744"><a name="p556996744"></a><a name="p556996744"></a>long avg_time[HCCS_PING_MESH_MAX_NUM];//ping平均时延</p>
<p id="p1156910618417"><a name="p1156910618417"></a><a name="p1156910618417"></a>long tp95_time[HCCS_PING_MESH_MAX_NUM];//ping时延TP95分位数</p>
<p id="p15569463417"><a name="p15569463417"></a><a name="p15569463417"></a>int reply_stat_num[HCCS_PING_MESH_MAX_NUM];//统计结果所用样本轮数</p>
<p id="p8569262417"><a name="p8569262417"></a><a name="p8569262417"></a>unsigned long long ping_total_num[HCCS_PING_MESH_MAX_NUM];//常驻任务开启以来ping的总轮数</p>
<p id="p1656946246"><a name="p1656946246"></a><a name="p1656946246"></a>int dest_num;//目的地址数量</p>
<p id="p135697614416"><a name="p135697614416"></a><a name="p135697614416"></a>};</p>
<p id="p4976183316206"><a name="p4976183316206"></a><a name="p4976183316206"></a></p>
<div class="note" id="note8393718184"><a name="note8393718184"></a><a name="note8393718184"></a><span class="notetitle"> 说明： </span><div class="notebody"><a name="ul97018984016"></a><a name="ul97018984016"></a><ul id="ul97018984016"><li>输出的结构体中，目的地址在dst_addr的下标与ping该地址的统计结果在各个数组中的下标相同。例如，ping dst_addr[0]的成功次数是suc_pkt_num[0]，失败次数是fail_pkt_num[0]，以此类推。</li><li>当max_time、min_time、avg_time、tp95_time的值为-1时，表示对应目的地址未ping通。</li></ul>
</div></div>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section117841124144913"></a>**

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

**异常处理<a name="section1079016243498"></a>**

无。

**约束说明<a name="section137900244496"></a>**

指定设备与目的设备的NPU驱动版本号需保持一致。

开启常驻任务后，需要等待1个task\_interval（常驻任务轮询间隔）及1轮ping报文发送周期后（至少等待pkt\_send\_num \* pkt\_interval ms）才能查询到数据。

每次使用该接口读取的数据都是最新统计结果。

该接口支持在物理机+特权容器场景下使用。

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

**调用示例<a name="section27965246494"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
unsigned int task_id = 0;
struct dcmi_hccsping_mesh_info hccsping_mesh_reply = {0};
ret = dcmi_get_hccsping_mesh_info (card_id, device_id, port_id, task_id, &hccsping_mesh_reply);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

