# dcmi\_mcu\_get\_chip\_temperature<a name="ZH-CN_TOPIC_0000002485478768"></a>

**函数原型<a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_toc533412077"></a>**

**int dcmi\_mcu\_get\_chip\_temperature\(int card\_id, char \*data\_info, int buf\_size, int \*data\_len\)**

**功能说明<a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_toc533412078"></a>**

查询NPU卡设备温度。

**参数说明<a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_toc533412079"></a>**

<a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p10021890"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p10021890"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p6466753"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p6466753"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p54045009"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p54045009"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p15569626"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p15569626"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p36741947142813"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p36741947142813"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p96741747122818"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p96741747122818"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p46747472287"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p46747472287"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p1467413474281"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p1467413474281"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_row154711839124318"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p112060413163"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p112060413163"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p112060413163"></a>data_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p202071441111616"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p202071441111616"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p202071441111616"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p42071141181610"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p42071141181610"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p42071141181610"></a>char *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p920794161619"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p920794161619"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p920794161619"></a>BYTE[0]：温度传感器个数</p>
<p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p520714117167"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p520714117167"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p520714117167"></a>BYTE[1:8]：温度传感器1名称</p>
<p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p5207134118163"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p5207134118163"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p5207134118163"></a>BYTE[9:10]：温度传感器1的温度值</p>
<p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p2020774151618"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p2020774151618"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p2020774151618"></a>…</p>
<p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p52074412162"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p52074412162"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p52074412162"></a>BYTE[10n-9:10n-2]：温度传感器n名称</p>
<p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p850293114581"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p850293114581"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p850293114581"></a>BYTE[10n-1:10n]：温度传感器n的温度值。</p>
<p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p7121111013579"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p7121111013579"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p7121111013579"></a>温度传感器名称请参见相应产品的带外管理接口说明。</p>
<p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p11207741131615"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p11207741131615"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p11207741131615"></a>针对I2C协议，如果温度为无效数据则为0x7ffd，如果温度读取失败则为0x7fff</p>
<p id="p1169071612173"><a name="p1169071612173"></a><a name="p1169071612173"></a></p>
<div class="note" id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_note1323383615211"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_note1323383615211"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_note1323383615211"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p855195112550"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p855195112550"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p855195112550"></a>OPTICAL温度传感器在未插入光模块或者接铜缆时，返回的温度为无效值0x7ffd。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_row12161237155118"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p1616113775116"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p1616113775116"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p1616113775116"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p19161737145119"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p19161737145119"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p19161737145119"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p6161193715112"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p6161193715112"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p6161193715112"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p14161337125112"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p14161337125112"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p14161337125112"></a>data_info空间的最大长度。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p36921291519"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p36921291519"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p36921291519"></a>data_len</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p2692329056"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p2692329056"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p2692329056"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p116921929858"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p116921929858"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p116921929858"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p156871648162"><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p156871648162"></a><a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_p156871648162"></a>输出数据长度。</p>
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

**约束说明<a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row119194469302"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p67828402447"><a name="zh-cn_topic_0000002485318818_p67828402447"></a><a name="zh-cn_topic_0000002485318818_p67828402447"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p16315103817414"><a name="zh-cn_topic_0000002485318818_p16315103817414"></a><a name="zh-cn_topic_0000002485318818_p16315103817414"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p9976837183014"><a name="zh-cn_topic_0000002485318818_p9976837183014"></a><a name="zh-cn_topic_0000002485318818_p9976837183014"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p992084633012"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p992084633012"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p992084633012"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row125012052104311"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1378374019445"><a name="zh-cn_topic_0000002485318818_p1378374019445"></a><a name="zh-cn_topic_0000002485318818_p1378374019445"></a><span id="zh-cn_topic_0000002485318818_text262011415447"><a name="zh-cn_topic_0000002485318818_text262011415447"></a><a name="zh-cn_topic_0000002485318818_text262011415447"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p3696134919457"><a name="zh-cn_topic_0000002485318818_p3696134919457"></a><a name="zh-cn_topic_0000002485318818_p3696134919457"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1069610491457"><a name="zh-cn_topic_0000002485318818_p1069610491457"></a><a name="zh-cn_topic_0000002485318818_p1069610491457"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p14744104011463"><a name="zh-cn_topic_0000002485318818_p14744104011463"></a><a name="zh-cn_topic_0000002485318818_p14744104011463"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row6920114611302"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p137832403444"><a name="zh-cn_topic_0000002485318818_p137832403444"></a><a name="zh-cn_topic_0000002485318818_p137832403444"></a><span id="zh-cn_topic_0000002485318818_text1480012462513"><a name="zh-cn_topic_0000002485318818_text1480012462513"></a><a name="zh-cn_topic_0000002485318818_text1480012462513"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p17744194024612"><a name="zh-cn_topic_0000002485318818_p17744194024612"></a><a name="zh-cn_topic_0000002485318818_p17744194024612"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row195142220363"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p19150201815714"><a name="zh-cn_topic_0000002485318818_p19150201815714"></a><a name="zh-cn_topic_0000002485318818_p19150201815714"></a><span id="zh-cn_topic_0000002485318818_text1615010187716"><a name="zh-cn_topic_0000002485318818_text1615010187716"></a><a name="zh-cn_topic_0000002485318818_text1615010187716"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1831093744715"><a name="zh-cn_topic_0000002485318818_p1831093744715"></a><a name="zh-cn_topic_0000002485318818_p1831093744715"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p9310337164720"><a name="zh-cn_topic_0000002485318818_p9310337164720"></a><a name="zh-cn_topic_0000002485318818_p9310337164720"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p10310113714473"><a name="zh-cn_topic_0000002485318818_p10310113714473"></a><a name="zh-cn_topic_0000002485318818_p10310113714473"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1024616413271"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1824612411277"><a name="zh-cn_topic_0000002485318818_p1824612411277"></a><a name="zh-cn_topic_0000002485318818_p1824612411277"></a><span id="zh-cn_topic_0000002485318818_text712252182813"><a name="zh-cn_topic_0000002485318818_text712252182813"></a><a name="zh-cn_topic_0000002485318818_text712252182813"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p436771642813"><a name="zh-cn_topic_0000002485318818_p436771642813"></a><a name="zh-cn_topic_0000002485318818_p436771642813"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p436710164281"><a name="zh-cn_topic_0000002485318818_p436710164281"></a><a name="zh-cn_topic_0000002485318818_p436710164281"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p193671116172816"><a name="zh-cn_topic_0000002485318818_p193671116172816"></a><a name="zh-cn_topic_0000002485318818_p193671116172816"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row11011140184711"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1010254064715"><a name="zh-cn_topic_0000002485318818_p1010254064715"></a><a name="zh-cn_topic_0000002485318818_p1010254064715"></a><span id="zh-cn_topic_0000002485318818_text15548246175319"><a name="zh-cn_topic_0000002485318818_text15548246175319"></a><a name="zh-cn_topic_0000002485318818_text15548246175319"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p0636155115534"><a name="zh-cn_topic_0000002485318818_p0636155115534"></a><a name="zh-cn_topic_0000002485318818_p0636155115534"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p16636951125317"><a name="zh-cn_topic_0000002485318818_p16636951125317"></a><a name="zh-cn_topic_0000002485318818_p16636951125317"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p163685125315"><a name="zh-cn_topic_0000002485318818_p163685125315"></a><a name="zh-cn_topic_0000002485318818_p163685125315"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row7398193211715"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p12881158154715"><a name="zh-cn_topic_0000002485318818_p12881158154715"></a><a name="zh-cn_topic_0000002485318818_p12881158154715"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251427213_zh-cn_topic_0000001178213194_zh-cn_topic_0000001114162132_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int enable_flag = 0;
char data_info[256] = {0};
int data_len = 0;
ret = dcmi_mcu_get_chip_temperature(card_id, data_info, sizeof(data_info), &data_len);
if (ret != 0) {
    //todo:记录日志
    return ERROR;  
}
…
```

