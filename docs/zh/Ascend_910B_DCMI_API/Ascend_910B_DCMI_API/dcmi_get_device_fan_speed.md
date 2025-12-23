# dcmi\_get\_device\_fan\_speed<a name="ZH-CN_TOPIC_0000002485295402"></a>

**函数原型<a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_toc533412077"></a>**

**int dcmi\_get\_device\_fan\_speed\(int card\_id, int device\_id, int fan\_id, int \*speed\)**

**功能说明<a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_toc533412078"></a>**

查询指定风扇的转速，为风扇实际转速，单位RPM。

**参数说明<a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_toc533412079"></a>**

<a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p10021890"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p10021890"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p6466753"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p6466753"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p54045009"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p54045009"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p15569626"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p15569626"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p36741947142813"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p36741947142813"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p96741747122818"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p96741747122818"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p46747472287"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p46747472287"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1467413474281"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1467413474281"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p7711145152918"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p7711145152918"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p671116522914"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p671116522914"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1771116572910"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1771116572910"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p141566134517"><a name="p141566134517"></a><a name="p141566134517"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p5522164215178"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p5522164215178"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p5522164215178"></a>fan_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p8522242101715"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p8522242101715"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p17522114220174"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p17522114220174"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p17522114220174"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p0522164231718"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p0522164231718"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p0522164231718"></a>如果有多个风扇，fan_id从1开始编号，大于等于1表示查询指定fan_id的风扇转速。</p>
<p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p450283104014"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p450283104014"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p450283104014"></a>如果fan_id为0，则表示查询所有风扇的平均速度。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_row10368134310403"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1436844319404"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1436844319404"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1436844319404"></a>speed</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p7369343114012"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p7369343114012"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p7369343114012"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p12369114324019"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p12369114324019"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p12369114324019"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1264911134110"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1264911134110"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p1264911134110"></a>输出风扇转速值数组，由调用者申请。</p>
<p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p065013117411"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p065013117411"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p065013117411"></a>调用成功后，该空间存储为风扇转速，单位为RPM，即转/分钟。</p>
<p id="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p965019134117"><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p965019134117"></a><a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_p965019134117"></a>取值范围：0~(18000&plusmn;10%)</p>
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

**约束说明<a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_toc533412082"></a>**

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

**调用示例<a name="zh-cn_topic_0000001251307147_zh-cn_topic_0000001223414421_zh-cn_topic_0000001147971449_toc533412083"></a>**

```
… 
int ret = 0;
int count = 0;
int card_id = 0;
int device_id = 0;
int speed;
ret = dcmi_get_device_fan_count(card_id, device_id, &count);
… 
for (i = 1; i <= count; i++){
    speed = 0;
    ret = dcmi_get_device_fan_speed(card_id, device_id, i, &speed);
    if (ret != 0){
        //todo
        return ret;
    }
… 
}
…
```

