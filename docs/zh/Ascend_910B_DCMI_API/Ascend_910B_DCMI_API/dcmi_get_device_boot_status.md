# dcmi\_get\_device\_boot\_status<a name="ZH-CN_TOPIC_0000002485295384"></a>

**函数原型<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412077"></a>**

**int dcmi\_get\_device\_boot\_status\(int card\_id, int device\_id, enum dcmi\_boot\_status \*boot\_status\)**

**功能说明<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412078"></a>**

获取设备的启动状态。

**参数说明<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412079"></a>**

<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.99%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.009999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p17306180164620"><a name="p17306180164620"></a><a name="p17306180164620"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p8175616161117"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p8175616161117"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p8175616161117"></a>boot_status</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"></a>enum dcmi_boot_status *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1317541616112"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1317541616112"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1317541616112"></a>enum dcmi_boot_status</p>
<p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1917515160111"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1917515160111"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1917515160111"></a>{</p>
<p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p51751216151114"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p51751216151114"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p51751216151114"></a>DCMI_BOOT_STATUS_UNINIT = 0, //未初始化</p>
<p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p51751816101118"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p51751816101118"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p51751816101118"></a>DCMI_BOOT_STATUS_BIOS, //BIOS启动中</p>
<p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p161757162117"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p161757162117"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p161757162117"></a>DCMI_BOOT_STATUS_OS, //OS启动中</p>
<p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1175131618114"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1175131618114"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1175131618114"></a>DCMI_BOOT_STATUS_FINISH //启动完成</p>
<p id="p383616372382"><a name="p383616372382"></a><a name="p383616372382"></a>DCMI_SYSTEM_START_FINISH = 16, //dcmi服务进程启动完成</p>
<p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1217571621117"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1217571621117"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1217571621117"></a>}</p>
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

**约束说明<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_table79191646143019"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row112361107329"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p830835414314"><a name="zh-cn_topic_0000002485295476_p830835414314"></a><a name="zh-cn_topic_0000002485295476_p830835414314"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p83081544318"><a name="zh-cn_topic_0000002485295476_p83081544318"></a><a name="zh-cn_topic_0000002485295476_p83081544318"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="24.97%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p193083549315"><a name="zh-cn_topic_0000002485295476_p193083549315"></a><a name="zh-cn_topic_0000002485295476_p193083549315"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25.03%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p730815541319"><a name="zh-cn_topic_0000002485295476_p730815541319"></a><a name="zh-cn_topic_0000002485295476_p730815541319"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row7236101023217"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p623641016329"><a name="zh-cn_topic_0000002485295476_p623641016329"></a><a name="zh-cn_topic_0000002485295476_p623641016329"></a><span id="zh-cn_topic_0000002485295476_ph8236121063219"><a name="zh-cn_topic_0000002485295476_ph8236121063219"></a><a name="zh-cn_topic_0000002485295476_ph8236121063219"></a><span id="zh-cn_topic_0000002485295476_text102364103324"><a name="zh-cn_topic_0000002485295476_text102364103324"></a><a name="zh-cn_topic_0000002485295476_text102364103324"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p16236141013216"><a name="zh-cn_topic_0000002485295476_p16236141013216"></a><a name="zh-cn_topic_0000002485295476_p16236141013216"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p323621073219"><a name="zh-cn_topic_0000002485295476_p323621073219"></a><a name="zh-cn_topic_0000002485295476_p323621073219"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1124714309329"><a name="zh-cn_topic_0000002485295476_p1124714309329"></a><a name="zh-cn_topic_0000002485295476_p1124714309329"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row9236191063213"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p132367107328"><a name="zh-cn_topic_0000002485295476_p132367107328"></a><a name="zh-cn_topic_0000002485295476_p132367107328"></a><span id="zh-cn_topic_0000002485295476_text182361710173214"><a name="zh-cn_topic_0000002485295476_text182361710173214"></a><a name="zh-cn_topic_0000002485295476_text182361710173214"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1223651073215"><a name="zh-cn_topic_0000002485295476_p1223651073215"></a><a name="zh-cn_topic_0000002485295476_p1223651073215"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p4236131093211"><a name="zh-cn_topic_0000002485295476_p4236131093211"></a><a name="zh-cn_topic_0000002485295476_p4236131093211"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1025623073219"><a name="zh-cn_topic_0000002485295476_p1025623073219"></a><a name="zh-cn_topic_0000002485295476_p1025623073219"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row13236121010328"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p323716106322"><a name="zh-cn_topic_0000002485295476_p323716106322"></a><a name="zh-cn_topic_0000002485295476_p323716106322"></a><span id="zh-cn_topic_0000002485295476_text823771020320"><a name="zh-cn_topic_0000002485295476_text823771020320"></a><a name="zh-cn_topic_0000002485295476_text823771020320"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p192371108322"><a name="zh-cn_topic_0000002485295476_p192371108322"></a><a name="zh-cn_topic_0000002485295476_p192371108322"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p192371510173210"><a name="zh-cn_topic_0000002485295476_p192371510173210"></a><a name="zh-cn_topic_0000002485295476_p192371510173210"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1026963083210"><a name="zh-cn_topic_0000002485295476_p1026963083210"></a><a name="zh-cn_topic_0000002485295476_p1026963083210"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row423781043218"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p9237110133213"><a name="zh-cn_topic_0000002485295476_p9237110133213"></a><a name="zh-cn_topic_0000002485295476_p9237110133213"></a><span id="zh-cn_topic_0000002485295476_text723791011326"><a name="zh-cn_topic_0000002485295476_text723791011326"></a><a name="zh-cn_topic_0000002485295476_text723791011326"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p623719101321"><a name="zh-cn_topic_0000002485295476_p623719101321"></a><a name="zh-cn_topic_0000002485295476_p623719101321"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p623717104322"><a name="zh-cn_topic_0000002485295476_p623717104322"></a><a name="zh-cn_topic_0000002485295476_p623717104322"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p182841630173218"><a name="zh-cn_topic_0000002485295476_p182841630173218"></a><a name="zh-cn_topic_0000002485295476_p182841630173218"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row52377108326"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p92371110103215"><a name="zh-cn_topic_0000002485295476_p92371110103215"></a><a name="zh-cn_topic_0000002485295476_p92371110103215"></a><span id="zh-cn_topic_0000002485295476_text823710106326"><a name="zh-cn_topic_0000002485295476_text823710106326"></a><a name="zh-cn_topic_0000002485295476_text823710106326"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p32372010143214"><a name="zh-cn_topic_0000002485295476_p32372010143214"></a><a name="zh-cn_topic_0000002485295476_p32372010143214"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p423717106328"><a name="zh-cn_topic_0000002485295476_p423717106328"></a><a name="zh-cn_topic_0000002485295476_p423717106328"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p6300830133214"><a name="zh-cn_topic_0000002485295476_p6300830133214"></a><a name="zh-cn_topic_0000002485295476_p6300830133214"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1023761053212"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p523761093214"><a name="zh-cn_topic_0000002485295476_p523761093214"></a><a name="zh-cn_topic_0000002485295476_p523761093214"></a><span id="zh-cn_topic_0000002485295476_text162373104322"><a name="zh-cn_topic_0000002485295476_text162373104322"></a><a name="zh-cn_topic_0000002485295476_text162373104322"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p523715105325"><a name="zh-cn_topic_0000002485295476_p523715105325"></a><a name="zh-cn_topic_0000002485295476_p523715105325"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p5237110133220"><a name="zh-cn_topic_0000002485295476_p5237110133220"></a><a name="zh-cn_topic_0000002485295476_p5237110133220"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1901195083313"><a name="zh-cn_topic_0000002485295476_p1901195083313"></a><a name="zh-cn_topic_0000002485295476_p1901195083313"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1237151073213"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p32371810163213"><a name="zh-cn_topic_0000002485295476_p32371810163213"></a><a name="zh-cn_topic_0000002485295476_p32371810163213"></a><span id="zh-cn_topic_0000002485295476_text423716102326"><a name="zh-cn_topic_0000002485295476_text423716102326"></a><a name="zh-cn_topic_0000002485295476_text423716102326"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p20237161012323"><a name="zh-cn_topic_0000002485295476_p20237161012323"></a><a name="zh-cn_topic_0000002485295476_p20237161012323"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="24.97%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p2023791019328"><a name="zh-cn_topic_0000002485295476_p2023791019328"></a><a name="zh-cn_topic_0000002485295476_p2023791019328"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25.03%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p19105505335"><a name="zh-cn_topic_0000002485295476_p19105505335"></a><a name="zh-cn_topic_0000002485295476_p19105505335"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row623791012325"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1237131016321"><a name="zh-cn_topic_0000002485295476_p1237131016321"></a><a name="zh-cn_topic_0000002485295476_p1237131016321"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
enum dcmi_boot_status boot_status = 0;
ret = dcmi_get_device_boot_status(card_id, device_id, &boot_status);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

