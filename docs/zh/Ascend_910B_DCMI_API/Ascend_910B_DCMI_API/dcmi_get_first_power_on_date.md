# dcmi\_get\_first\_power\_on\_date<a name="ZH-CN_TOPIC_0000002485455422"></a>

**函数原型<a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_toc533412077"></a>**

**int dcmi\_get\_first\_power\_on\_date\(int card\_id, unsigned int \*first\_power\_on\_date\)**

**功能说明<a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_toc533412078"></a>**

获取指定设备的首次上电日期。

**参数说明<a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_toc533412079"></a>**

<a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_row7580267"><th class="cellrowborder" valign="top" width="15.72%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p10021890"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p10021890"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="13.850000000000001%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p6466753"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p6466753"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.06%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p54045009"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p54045009"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="54.37%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p15569626"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p15569626"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_row10560021192510"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p36741947142813"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p36741947142813"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p96741747122818"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p96741747122818"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.06%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p46747472287"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p46747472287"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.37%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p1467413474281"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p1467413474281"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_row15462171542913"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_p445315176517"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_p445315176517"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_p445315176517"></a>first_power_on_date</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p1926232982914"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p1926232982914"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p1926232982914"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.06%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p826217292293"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p826217292293"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_p826217292293"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="54.37%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_p9460125611527"><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_p9460125611527"></a><a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_p9460125611527"></a>首次上电日期信息为1970/1/1 00:00:00到当前时间的秒数，时间精确到日。全0表示当前还未将首次上电时间写入flash，需要等待24小时后查询。</p>
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

**约束说明<a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table1993685321815"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row9195184392411"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p9107107193213"><a name="zh-cn_topic_0000002485295476_p9107107193213"></a><a name="zh-cn_topic_0000002485295476_p9107107193213"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p1810714713218"><a name="zh-cn_topic_0000002485295476_p1810714713218"></a><a name="zh-cn_topic_0000002485295476_p1810714713218"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p161071577328"><a name="zh-cn_topic_0000002485295476_p161071577328"></a><a name="zh-cn_topic_0000002485295476_p161071577328"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p91071679325"><a name="zh-cn_topic_0000002485295476_p91071679325"></a><a name="zh-cn_topic_0000002485295476_p91071679325"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row819554332411"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p8195114342417"><a name="zh-cn_topic_0000002485295476_p8195114342417"></a><a name="zh-cn_topic_0000002485295476_p8195114342417"></a><span id="zh-cn_topic_0000002485295476_ph21959437245"><a name="zh-cn_topic_0000002485295476_ph21959437245"></a><a name="zh-cn_topic_0000002485295476_ph21959437245"></a><span id="zh-cn_topic_0000002485295476_text719554332411"><a name="zh-cn_topic_0000002485295476_text719554332411"></a><a name="zh-cn_topic_0000002485295476_text719554332411"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1019515434246"><a name="zh-cn_topic_0000002485295476_p1019515434246"></a><a name="zh-cn_topic_0000002485295476_p1019515434246"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15196114318249"><a name="zh-cn_topic_0000002485295476_p15196114318249"></a><a name="zh-cn_topic_0000002485295476_p15196114318249"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p719611433243"><a name="zh-cn_topic_0000002485295476_p719611433243"></a><a name="zh-cn_topic_0000002485295476_p719611433243"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row01969439247"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p17196543192412"><a name="zh-cn_topic_0000002485295476_p17196543192412"></a><a name="zh-cn_topic_0000002485295476_p17196543192412"></a><span id="zh-cn_topic_0000002485295476_text1419612437243"><a name="zh-cn_topic_0000002485295476_text1419612437243"></a><a name="zh-cn_topic_0000002485295476_text1419612437243"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p10196104332415"><a name="zh-cn_topic_0000002485295476_p10196104332415"></a><a name="zh-cn_topic_0000002485295476_p10196104332415"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p141962043162415"><a name="zh-cn_topic_0000002485295476_p141962043162415"></a><a name="zh-cn_topic_0000002485295476_p141962043162415"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p219674362413"><a name="zh-cn_topic_0000002485295476_p219674362413"></a><a name="zh-cn_topic_0000002485295476_p219674362413"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row141962435246"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p18196114311245"><a name="zh-cn_topic_0000002485295476_p18196114311245"></a><a name="zh-cn_topic_0000002485295476_p18196114311245"></a><span id="zh-cn_topic_0000002485295476_text15196443142419"><a name="zh-cn_topic_0000002485295476_text15196443142419"></a><a name="zh-cn_topic_0000002485295476_text15196443142419"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p6196943142419"><a name="zh-cn_topic_0000002485295476_p6196943142419"></a><a name="zh-cn_topic_0000002485295476_p6196943142419"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p12196154313246"><a name="zh-cn_topic_0000002485295476_p12196154313246"></a><a name="zh-cn_topic_0000002485295476_p12196154313246"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p7196174314243"><a name="zh-cn_topic_0000002485295476_p7196174314243"></a><a name="zh-cn_topic_0000002485295476_p7196174314243"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row2196144316244"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1196204311249"><a name="zh-cn_topic_0000002485295476_p1196204311249"></a><a name="zh-cn_topic_0000002485295476_p1196204311249"></a><span id="zh-cn_topic_0000002485295476_text4196174322416"><a name="zh-cn_topic_0000002485295476_text4196174322416"></a><a name="zh-cn_topic_0000002485295476_text4196174322416"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p14196543132410"><a name="zh-cn_topic_0000002485295476_p14196543132410"></a><a name="zh-cn_topic_0000002485295476_p14196543132410"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p21961043142413"><a name="zh-cn_topic_0000002485295476_p21961043142413"></a><a name="zh-cn_topic_0000002485295476_p21961043142413"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p2196144313249"><a name="zh-cn_topic_0000002485295476_p2196144313249"></a><a name="zh-cn_topic_0000002485295476_p2196144313249"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row019694302419"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p0196184302411"><a name="zh-cn_topic_0000002485295476_p0196184302411"></a><a name="zh-cn_topic_0000002485295476_p0196184302411"></a><span id="zh-cn_topic_0000002485295476_text619654317244"><a name="zh-cn_topic_0000002485295476_text619654317244"></a><a name="zh-cn_topic_0000002485295476_text619654317244"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p4196124316249"><a name="zh-cn_topic_0000002485295476_p4196124316249"></a><a name="zh-cn_topic_0000002485295476_p4196124316249"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p3196154312417"><a name="zh-cn_topic_0000002485295476_p3196154312417"></a><a name="zh-cn_topic_0000002485295476_p3196154312417"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p161961043192418"><a name="zh-cn_topic_0000002485295476_p161961043192418"></a><a name="zh-cn_topic_0000002485295476_p161961043192418"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row3196743122417"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1319616436247"><a name="zh-cn_topic_0000002485295476_p1319616436247"></a><a name="zh-cn_topic_0000002485295476_p1319616436247"></a><span id="zh-cn_topic_0000002485295476_text519654318244"><a name="zh-cn_topic_0000002485295476_text519654318244"></a><a name="zh-cn_topic_0000002485295476_text519654318244"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p5196184312419"><a name="zh-cn_topic_0000002485295476_p5196184312419"></a><a name="zh-cn_topic_0000002485295476_p5196184312419"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p6196114382419"><a name="zh-cn_topic_0000002485295476_p6196114382419"></a><a name="zh-cn_topic_0000002485295476_p6196114382419"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p419634382413"><a name="zh-cn_topic_0000002485295476_p419634382413"></a><a name="zh-cn_topic_0000002485295476_p419634382413"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row21961435246"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p319644342412"><a name="zh-cn_topic_0000002485295476_p319644342412"></a><a name="zh-cn_topic_0000002485295476_p319644342412"></a><span id="zh-cn_topic_0000002485295476_text13196114320241"><a name="zh-cn_topic_0000002485295476_text13196114320241"></a><a name="zh-cn_topic_0000002485295476_text13196114320241"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p6196143112415"><a name="zh-cn_topic_0000002485295476_p6196143112415"></a><a name="zh-cn_topic_0000002485295476_p6196143112415"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p171961543192420"><a name="zh-cn_topic_0000002485295476_p171961543192420"></a><a name="zh-cn_topic_0000002485295476_p171961543192420"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p121960438242"><a name="zh-cn_topic_0000002485295476_p121960438242"></a><a name="zh-cn_topic_0000002485295476_p121960438242"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row121961943162411"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p31968432242"><a name="zh-cn_topic_0000002485295476_p31968432242"></a><a name="zh-cn_topic_0000002485295476_p31968432242"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251107201_zh-cn_topic_0000001184188806_zh-cn_topic_0000001167913765_toc533412083"></a>**

```
…  
int ret = 0; 
int card_id = 0; 
unsigned int first_power_on_date = 0; 
ret = dcmi_get_first_power_on_date(card_id, &first_power_on_date); 
…
```

