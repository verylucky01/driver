# dcmi\_get\_device\_elabel\_info<a name="ZH-CN_TOPIC_0000002517615415"></a>

**函数原型<a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_toc533412077"></a>**

**int dcmi\_get\_device\_elabel\_info\(int card\_id, int device\_id, struct dcmi\_elabel\_info \*elabel\_info\)**

**功能说明<a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_toc533412078"></a>**

获取设备的电子标签信息。

**参数说明<a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_toc533412079"></a>**

<a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p10021890"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p10021890"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.99%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p6466753"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p6466753"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p54045009"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p54045009"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.01%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15569626"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15569626"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p36741947142813"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p36741947142813"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p96741747122818"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p96741747122818"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p46747472287"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p46747472287"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.01%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1467413474281"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1467413474281"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p7711145152918"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p7711145152918"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p671116522914"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p671116522914"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1771116572910"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1771116572910"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.01%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p1377514432141"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p147768432143"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p147768432143"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p147768432143"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_p46986266814"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_p46986266814"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_zh-cn_topic_0000001147723703_p46986266814"></a>MCU芯片：mcu_id。</p>
<p id="p2432832111016"><a name="p2432832111016"></a><a name="p2432832111016"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p5262129152913"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p5262129152913"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p5262129152913"></a>elabel_info</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1926232982914"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1926232982914"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1926232982914"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p148231023185016"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p148231023185016"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p148231023185016"></a>struct dcmi_elabel_info *</p>
</td>
<td class="cellrowborder" valign="top" width="50.01%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1018413955820"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1018413955820"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1018413955820"></a>电子标签信息。</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p148613620474"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p148613620474"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p148613620474"></a>struct dcmi_elabel_info {</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1548693674720"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1548693674720"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1548693674720"></a>char product_name[MAX_LENTH]; //产品名称</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p9486143684713"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p9486143684713"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p9486143684713"></a>char model[MAX_LENTH]; //产品型号</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p4486153611473"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p4486153611473"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p4486153611473"></a>char manufacturer[MAX_LENTH]; //生产厂商</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1048653610476"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1048653610476"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1048653610476"></a>char manufacturer_date[MAX_LENTH]; //生产日期</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15486173611477"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15486173611477"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15486173611477"></a>char serial_number[MAX_LENTH]; //产品序列号</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15486236184717"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15486236184717"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p15486236184717"></a>};</p>
<p id="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1129264019172"><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1129264019172"></a><a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_p1129264019172"></a>其中manufacturer_date为预留字段，当前此字段无效。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000001917887173_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**约束说明<a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table1113417173519"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1548132517501"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p16154843125019"><a name="zh-cn_topic_0000002485295476_p16154843125019"></a><a name="zh-cn_topic_0000002485295476_p16154843125019"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p3123182915595"><a name="zh-cn_topic_0000002485295476_p3123182915595"></a><a name="zh-cn_topic_0000002485295476_p3123182915595"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p6271950600"><a name="zh-cn_topic_0000002485295476_p6271950600"></a><a name="zh-cn_topic_0000002485295476_p6271950600"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p41541743195013"><a name="zh-cn_topic_0000002485295476_p41541743195013"></a><a name="zh-cn_topic_0000002485295476_p41541743195013"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p19444162914516"><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><span id="zh-cn_topic_0000002485295476_ph1944412296514"><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><span id="zh-cn_topic_0000002485295476_text944432913516"><a name="zh-cn_topic_0000002485295476_text944432913516"></a><a name="zh-cn_topic_0000002485295476_text944432913516"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row6920114611302"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p194441629165117"><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><span id="zh-cn_topic_0000002485295476_text124449291511"><a name="zh-cn_topic_0000002485295476_text124449291511"></a><a name="zh-cn_topic_0000002485295476_text124449291511"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p461320458219"><a name="zh-cn_topic_0000002485295476_p461320458219"></a><a name="zh-cn_topic_0000002485295476_p461320458219"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13613134519220"><a name="zh-cn_topic_0000002485295476_p13613134519220"></a><a name="zh-cn_topic_0000002485295476_p13613134519220"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p96133454211"><a name="zh-cn_topic_0000002485295476_p96133454211"></a><a name="zh-cn_topic_0000002485295476_p96133454211"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row14873174604910"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p10444329155118"><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><span id="zh-cn_topic_0000002485295476_text1044482925119"><a name="zh-cn_topic_0000002485295476_text1044482925119"></a><a name="zh-cn_topic_0000002485295476_text1044482925119"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p896391111511"><a name="zh-cn_topic_0000002485295476_p896391111511"></a><a name="zh-cn_topic_0000002485295476_p896391111511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p119682011105113"><a name="zh-cn_topic_0000002485295476_p119682011105113"></a><a name="zh-cn_topic_0000002485295476_p119682011105113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p11975911195114"><a name="zh-cn_topic_0000002485295476_p11975911195114"></a><a name="zh-cn_topic_0000002485295476_p11975911195114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row23051923114915"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p174441129175117"><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><span id="zh-cn_topic_0000002485295476_text34441729175119"><a name="zh-cn_topic_0000002485295476_text34441729175119"></a><a name="zh-cn_topic_0000002485295476_text34441729175119"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1298231135114"><a name="zh-cn_topic_0000002485295476_p1298231135114"></a><a name="zh-cn_topic_0000002485295476_p1298231135114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1298921117511"><a name="zh-cn_topic_0000002485295476_p1298921117511"></a><a name="zh-cn_topic_0000002485295476_p1298921117511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1799561165114"><a name="zh-cn_topic_0000002485295476_p1799561165114"></a><a name="zh-cn_topic_0000002485295476_p1799561165114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row44814564912"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1044414295516"><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><span id="zh-cn_topic_0000002485295476_text44441829105114"><a name="zh-cn_topic_0000002485295476_text44441829105114"></a><a name="zh-cn_topic_0000002485295476_text44441829105114"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p17181285116"><a name="zh-cn_topic_0000002485295476_p17181285116"></a><a name="zh-cn_topic_0000002485295476_p17181285116"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15131255114"><a name="zh-cn_topic_0000002485295476_p15131255114"></a><a name="zh-cn_topic_0000002485295476_p15131255114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15917127513"><a name="zh-cn_topic_0000002485295476_p15917127513"></a><a name="zh-cn_topic_0000002485295476_p15917127513"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row11171321114917"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p94441829165113"><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><span id="zh-cn_topic_0000002485295476_text644410297515"><a name="zh-cn_topic_0000002485295476_text644410297515"></a><a name="zh-cn_topic_0000002485295476_text644410297515"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p01211275113"><a name="zh-cn_topic_0000002485295476_p01211275113"></a><a name="zh-cn_topic_0000002485295476_p01211275113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p916161212512"><a name="zh-cn_topic_0000002485295476_p916161212512"></a><a name="zh-cn_topic_0000002485295476_p916161212512"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p191911211511"><a name="zh-cn_topic_0000002485295476_p191911211511"></a><a name="zh-cn_topic_0000002485295476_p191911211511"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row18385115320492"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1644572918513"><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><span id="zh-cn_topic_0000002485295476_text1744532925112"><a name="zh-cn_topic_0000002485295476_text1744532925112"></a><a name="zh-cn_topic_0000002485295476_text1744532925112"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1923112115119"><a name="zh-cn_topic_0000002485295476_p1923112115119"></a><a name="zh-cn_topic_0000002485295476_p1923112115119"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1269122515"><a name="zh-cn_topic_0000002485295476_p1269122515"></a><a name="zh-cn_topic_0000002485295476_p1269122515"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p43014126516"><a name="zh-cn_topic_0000002485295476_p43014126516"></a><a name="zh-cn_topic_0000002485295476_p43014126516"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row557915117191"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p2042757191916"><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><span id="zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251427197_zh-cn_topic_0000001178054660_zh-cn_topic_0000001101443704_toc533412083"></a>**

```
…
struct dcmi_elabel_info elabel_info = {0};
int ret = 0;
int card_id = 0;
int device_id = 0;
ret = dcmi_get_device_elabel_info(card_id, device_id, &elabel_info);
…
```

