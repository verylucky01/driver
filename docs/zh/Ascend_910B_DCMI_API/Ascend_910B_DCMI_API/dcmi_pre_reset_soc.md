# dcmi\_pre\_reset\_soc<a name="ZH-CN_TOPIC_0000002485455428"></a>

**函数原型<a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_toc533412077"></a>**

**int dcmi\_pre\_reset\_soc\(int card\_id, int device\_id\)**

**功能说明<a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_toc533412078"></a>**

芯片预复位，发起芯片预复位接口，预复位目的是解除上层驱动及软件对此芯片的依赖。必须在预复位接口返回成功后，才能对此芯片进行隔离或实际复位操作。

**参数说明<a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_toc533412079"></a>**

<a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p10021890"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p10021890"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p6466753"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p6466753"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p54045009"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p54045009"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p15569626"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p15569626"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p36741947142813"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p36741947142813"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p96741747122818"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p96741747122818"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p46747472287"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p46747472287"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p1467413474281"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p1467413474281"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p7711145152918"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p7711145152918"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p671116522914"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p671116522914"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p1771116572910"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p1771116572910"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251307175_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p613011215589"><a name="p613011215589"></a><a name="p613011215589"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
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

**约束说明<a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_toc533412082"></a>**

带外标卡复位功能依赖ipmitool软件，需要提前下载并加载驱动。详细操作请参见[准备ipmitool软件](准备ipmitool软件.md)章节。

该接口在后续版本将会删除，推荐使用[dcmi\_set\_device\_pre\_reset](dcmi_set_device_pre_reset.md)。

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

**调用示例<a name="zh-cn_topic_0000001251307175_zh-cn_topic_0000001178054648_zh-cn_topic_0000001148439041_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
ret = dcmi_pre_reset_soc(card_id, device_id);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

