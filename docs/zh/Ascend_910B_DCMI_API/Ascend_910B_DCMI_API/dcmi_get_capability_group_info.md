# dcmi\_get\_capability\_group\_info<a name="ZH-CN_TOPIC_0000002485455402"></a>

**函数原型<a name="section14126237219"></a>**

**int dcmi\_get\_capability\_group\_info\(int card\_id, int device\_id, int ts\_id, int group\_id, struct dcmi\_capability\_group\_info \*group\_info, int group\_count\)**

**功能说明<a name="section1812132352116"></a>**

获取昇腾虚拟化实例配置信息。

**参数说明<a name="section5121523152116"></a>**

<a name="table936323172112"></a>
<table><thead align="left"><tr id="row9101923162117"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p81014235218"><a name="p81014235218"></a><a name="p81014235218"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.14%" id="mcps1.1.5.1.2"><p id="p16101192311218"><a name="p16101192311218"></a><a name="p16101192311218"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.18%" id="mcps1.1.5.1.3"><p id="p810192315214"><a name="p810192315214"></a><a name="p810192315214"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p11011423182111"><a name="p11011423182111"></a><a name="p11011423182111"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row181021230215"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p710211238213"><a name="p710211238213"></a><a name="p710211238213"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.14%" headers="mcps1.1.5.1.2 "><p id="p181021523202116"><a name="p181021523202116"></a><a name="p181021523202116"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.18%" headers="mcps1.1.5.1.3 "><p id="p10102182312211"><a name="p10102182312211"></a><a name="p10102182312211"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p16102823142117"><a name="p16102823142117"></a><a name="p16102823142117"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row121026234211"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p15102132316213"><a name="p15102132316213"></a><a name="p15102132316213"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.14%" headers="mcps1.1.5.1.2 "><p id="p1110202310211"><a name="p1110202310211"></a><a name="p1110202310211"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.18%" headers="mcps1.1.5.1.3 "><p id="p13102102313214"><a name="p13102102313214"></a><a name="p13102102313214"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p15102152315216"><a name="p15102152315216"></a><a name="p15102152315216"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p11102112342111"><a name="p11102112342111"></a><a name="p11102112342111"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p4892193220560"><a name="p4892193220560"></a><a name="p4892193220560"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row410272319218"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p18102202317214"><a name="p18102202317214"></a><a name="p18102202317214"></a>ts_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.14%" headers="mcps1.1.5.1.2 "><p id="p121021423192117"><a name="p121021423192117"></a><a name="p121021423192117"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.18%" headers="mcps1.1.5.1.3 "><p id="p1710222319213"><a name="p1710222319213"></a><a name="p1710222319213"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p1610210232215"><a name="p1610210232215"></a><a name="p1610210232215"></a>算力类型ID：</p>
<p id="p1102162302112"><a name="p1102162302112"></a><a name="p1102162302112"></a>typedef enum {</p>
<p id="p1210242310210"><a name="p1210242310210"></a><a name="p1210242310210"></a>DCMI_TS_AICORE = 0,</p>
<p id="p121026236213"><a name="p121026236213"></a><a name="p121026236213"></a>DCMI_TS_AIVECTOR,</p>
<p id="p01026238215"><a name="p01026238215"></a><a name="p01026238215"></a>} DCMI_TS_ID;</p>
<p id="p1210332311216"><a name="p1210332311216"></a><a name="p1210332311216"></a>目前不支持DCMI_TS_AIVECTOR</p>
</td>
</tr>
<tr id="row610319235213"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p11103102392114"><a name="p11103102392114"></a><a name="p11103102392114"></a>group_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.14%" headers="mcps1.1.5.1.2 "><p id="p1103132310216"><a name="p1103132310216"></a><a name="p1103132310216"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.18%" headers="mcps1.1.5.1.3 "><p id="p5103182318215"><a name="p5103182318215"></a><a name="p5103182318215"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p3103923102118"><a name="p3103923102118"></a><a name="p3103923102118"></a>算力组ID，范围：[0, 3]或者-1。当group_id为-1时获取所有的group信息。</p>
</td>
</tr>
<tr id="row151037235211"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p14103162382116"><a name="p14103162382116"></a><a name="p14103162382116"></a>group_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.14%" headers="mcps1.1.5.1.2 "><p id="p5103123132114"><a name="p5103123132114"></a><a name="p5103123132114"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.18%" headers="mcps1.1.5.1.3 "><p id="p41036232211"><a name="p41036232211"></a><a name="p41036232211"></a>struct dcmi_capability_group_info*</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p121033235212"><a name="p121033235212"></a><a name="p121033235212"></a>算力组信息。</p>
<p id="p110312239214"><a name="p110312239214"></a><a name="p110312239214"></a>算力信息结构体为：</p>
<p id="p15103182311215"><a name="p15103182311215"></a><a name="p15103182311215"></a>#define DCMI_COMPUTE_GROUP_INFO_RES_NUM</p>
<p id="p7103923112114"><a name="p7103923112114"></a><a name="p7103923112114"></a>#define DCMI_AICORE_MASK_NUM 2</p>
<p id="p5103123102120"><a name="p5103123102120"></a><a name="p5103123102120"></a>struct dcmi_capability_group_info {</p>
<p id="p610313237219"><a name="p610313237219"></a><a name="p610313237219"></a>unsigned int  group_id;</p>
<p id="p12103182316212"><a name="p12103182316212"></a><a name="p12103182316212"></a>unsigned int  state;</p>
<p id="p17103202342116"><a name="p17103202342116"></a><a name="p17103202342116"></a>unsigned int  extend_attribute;</p>
<p id="p810382320219"><a name="p810382320219"></a><a name="p810382320219"></a>unsigned int  aicore_number;</p>
<p id="p9103102382114"><a name="p9103102382114"></a><a name="p9103102382114"></a>unsigned int  aivector_number;</p>
<p id="p14103122332117"><a name="p14103122332117"></a><a name="p14103122332117"></a>unsigned int  sdma_number;</p>
<p id="p16103823122115"><a name="p16103823122115"></a><a name="p16103823122115"></a>unsigned int  aicpu_number;</p>
<p id="p1310362332118"><a name="p1310362332118"></a><a name="p1310362332118"></a>unsigned int  active_sq_number;</p>
<p id="p181033233214"><a name="p181033233214"></a><a name="p181033233214"></a>unsigned int  aicore_mask [DCMI_AICORE_MASK_NUM];</p>
<p id="p141034234211"><a name="p141034234211"></a><a name="p141034234211"></a>unsigned int  res[DCMI_COMPUTE_GROUP_INFO_RES_NUM - DCMI_AICORE_MASK_NUM];</p>
<p id="p20103182342117"><a name="p20103182342117"></a><a name="p20103182342117"></a>};</p>
<a name="ul12388191374017"></a><a name="ul12388191374017"></a><ul id="ul12388191374017"><li>group_id表示算力组ID，全局唯一。可配置范围：0~3。</li><li>state表示算力组创建状态。0: not created, 1: created。</li><li>extend_attribute表示默认昇腾虚拟化实例标志，1：系统默认使用该昇腾虚拟化实例，其他值表示不使用该昇腾虚拟化实例为默认组，缺省值：0。</li><li>aicore_number表示算力组的aicore数量。</li><li>aivector_number、sdma_number、aicpu_number、active_sq_number分别表示aivector、sdma、aicpu、active资源数量，目前会返回255，表示当前资源为共享资源，不支持切分。</li><li>aicore_mask表示aicore的编号掩码，其中每个bit表示一个core。1：该算力组内aicore，0：非算力组内aicore。</li><li>res表示预留参数。</li></ul>
</td>
</tr>
<tr id="row1410352362114"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p161031723122118"><a name="p161031723122118"></a><a name="p161031723122118"></a>group_count</p>
</td>
<td class="cellrowborder" valign="top" width="15.14%" headers="mcps1.1.5.1.2 "><p id="p1010372316219"><a name="p1010372316219"></a><a name="p1010372316219"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.18%" headers="mcps1.1.5.1.3 "><p id="p1210332314218"><a name="p1210332314218"></a><a name="p1210332314218"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p91032023202119"><a name="p91032023202119"></a><a name="p91032023202119"></a>参数group_info的数组长度。</p>
<p id="p410310237215"><a name="p410310237215"></a><a name="p410310237215"></a>在group_id为-1时，group_count需要不小于4。</p>
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

**约束说明<a name="section53132352111"></a>**

仅Control CPU开放形态使用该接口。

**表 1** 不同部署场景下的支持情况

<a name="table1993685321815"></a>
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

**调用示例<a name="section1134182310216"></a>**

```
… 
int ret = 0;
int card_id = 0;
int dev_id = 0;
int ts_id  = 0;
struct dcmi_capability_group_info group_info[4] = {0};
ret = dcmi_get_capability_group_info(card_id, dev_id, ts_id, -1, group_info, 4);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

