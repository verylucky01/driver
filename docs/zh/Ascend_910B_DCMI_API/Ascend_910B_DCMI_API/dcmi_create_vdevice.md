# dcmi\_create\_vdevice<a name="ZH-CN_TOPIC_0000002485295464"></a>

**函数原型<a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_toc533412077"></a>**

**int dcmi\_create\_vdevice\(int card\_id, int device\_id, struct dcmi\_create\_vdev\_res\_stru\*vdev, struct dcmi\_create\_vdev\_out \*out\)**

**功能说明<a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_toc533412078"></a>**

创建指定NPU单元的vNPU设备。

**参数说明<a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_toc533412079"></a>**

<a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_row7580267"><th class="cellrowborder" valign="top" width="11.53%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p10021890"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p10021890"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="11.18%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p6466753"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p6466753"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="21.29%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p54045009"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p54045009"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="56.00000000000001%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p15569626"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p15569626"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_row10560021192510"><td class="cellrowborder" valign="top" width="11.53%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p36741947142813"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p36741947142813"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="11.18%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p96741747122818"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p96741747122818"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="21.29%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p46747472287"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p46747472287"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p1467413474281"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p1467413474281"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_row1155711494235"><td class="cellrowborder" valign="top" width="11.53%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p7711145152918"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p7711145152918"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="11.18%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p671116522914"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p671116522914"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="21.29%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p1771116572910"><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p1771116572910"></a><a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p188678373589"><a name="p188678373589"></a><a name="p188678373589"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row4179849112917"><td class="cellrowborder" valign="top" width="11.53%" headers="mcps1.1.5.1.1 "><p id="p11804491298"><a name="p11804491298"></a><a name="p11804491298"></a>vdev</p>
</td>
<td class="cellrowborder" valign="top" width="11.18%" headers="mcps1.1.5.1.2 "><p id="p7180049112912"><a name="p7180049112912"></a><a name="p7180049112912"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="21.29%" headers="mcps1.1.5.1.3 "><p id="p6180184962912"><a name="p6180184962912"></a><a name="p6180184962912"></a>struct dcmi_create_vdev_res_stru*</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.5.1.4 "><p id="p139261914173420"><a name="p139261914173420"></a><a name="p139261914173420"></a>输入创建虚拟设备信息。</p>
<p id="p1434917620323"><a name="p1434917620323"></a><a name="p1434917620323"></a>struct dcmi_create_vdev_res_stru {</p>
<p id="p734918614326"><a name="p734918614326"></a><a name="p734918614326"></a>unsigned int vdev_id;</p>
<p id="p93491164325"><a name="p93491164325"></a><a name="p93491164325"></a>unsigned int vfg_id;</p>
<p id="p14349166143211"><a name="p14349166143211"></a><a name="p14349166143211"></a>char template_name[32];</p>
<p id="p143491766329"><a name="p143491766329"></a><a name="p143491766329"></a>unsigned char reserved[64];</p>
<p id="p73491361325"><a name="p73491361325"></a><a name="p73491361325"></a>};</p>
<p id="p15221183945813"><a name="p15221183945813"></a><a name="p15221183945813"></a></p>
<div class="note" id="note1261114528356"><a name="note1261114528356"></a><a name="note1261114528356"></a><span class="notetitle"> 说明： </span><div class="notebody"><a name="ul093734367"></a><a name="ul093734367"></a><ul id="ul093734367"><li>vdev_id表示指定虚拟设备对应的虚拟设备ID，默认自动分配，默认值为0xFFFFFFFF。</li><li>vfg_id表示指定虚拟设备所属的虚拟分组ID，默认自动分配，默认值为0xFFFFFFFF。<p id="p1829441315414"><a name="p1829441315414"></a><a name="p1829441315414"></a>vfg_id为预留参数。</p>
</li><li>template_name表示<span id="ph5309125710415"><a name="ph5309125710415"></a><a name="ph5309125710415"></a>昇腾虚拟化实例（AVI）</span>模板名称。可通过<strong id="b4730181201514"><a name="b4730181201514"></a><a name="b4730181201514"></a>npu-smi info -t template-info -i</strong>命令查询其详细信息。其中<em id="i7876201616131"><a name="i7876201616131"></a><a name="i7876201616131"></a>xxx</em>B4可通过<strong id="zh-cn_topic_0000001264656717_zh-cn_topic_0000001117597270_b162223445115"><a name="zh-cn_topic_0000001264656717_zh-cn_topic_0000001117597270_b162223445115"></a><a name="zh-cn_topic_0000001264656717_zh-cn_topic_0000001117597270_b162223445115"></a>npu-smi info</strong>命令Name字段信息获取。<p id="p2901442445"><a name="p2901442445"></a><a name="p2901442445"></a>回显中<em id="i97466410131"><a name="i97466410131"></a><a name="i97466410131"></a>xxx</em>B4产品，有如下<span id="ph16477512445"><a name="ph16477512445"></a><a name="ph16477512445"></a>昇腾虚拟化实例（AVI）</span>模板：</p>
<a name="ul829584110441"></a><a name="ul829584110441"></a><ul id="ul829584110441"><li>vir10_3c_32g</li><li>vir05_1c_16g</li></ul>
</li><li>reserved表示预留参数。</li></ul>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147238_row5113182419158"><td class="cellrowborder" valign="top" width="11.53%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147238_p6473348131719"><a name="zh-cn_topic_0000001206147238_p6473348131719"></a><a name="zh-cn_topic_0000001206147238_p6473348131719"></a>out</p>
</td>
<td class="cellrowborder" valign="top" width="11.18%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147238_p64731848161713"><a name="zh-cn_topic_0000001206147238_p64731848161713"></a><a name="zh-cn_topic_0000001206147238_p64731848161713"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="21.29%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147238_p14473948161716"><a name="zh-cn_topic_0000001206147238_p14473948161716"></a><a name="zh-cn_topic_0000001206147238_p14473948161716"></a>struct dcmi_create_vdev_out *</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147238_p54731248191713"><a name="zh-cn_topic_0000001206147238_p54731248191713"></a><a name="zh-cn_topic_0000001206147238_p54731248191713"></a>输出创建的虚拟设备信息。当前只支持vdev_id。</p>
<p id="zh-cn_topic_0000001206147238_p8473154819174"><a name="zh-cn_topic_0000001206147238_p8473154819174"></a><a name="zh-cn_topic_0000001206147238_p8473154819174"></a>struct dcmi_create_vdev_out {</p>
<p id="zh-cn_topic_0000001206147238_p14473134821712"><a name="zh-cn_topic_0000001206147238_p14473134821712"></a><a name="zh-cn_topic_0000001206147238_p14473134821712"></a>unsigned int vdev_id;</p>
<p id="zh-cn_topic_0000001206147238_p947334801715"><a name="zh-cn_topic_0000001206147238_p947334801715"></a><a name="zh-cn_topic_0000001206147238_p947334801715"></a>unsigned int pcie_bus;</p>
<p id="zh-cn_topic_0000001206147238_p147384891717"><a name="zh-cn_topic_0000001206147238_p147384891717"></a><a name="zh-cn_topic_0000001206147238_p147384891717"></a>unsigned int pcie_device;</p>
<p id="zh-cn_topic_0000001206147238_p1547374881718"><a name="zh-cn_topic_0000001206147238_p1547374881718"></a><a name="zh-cn_topic_0000001206147238_p1547374881718"></a>unsigned int pcie_func;</p>
<p id="zh-cn_topic_0000001206147238_p15473948111719"><a name="zh-cn_topic_0000001206147238_p15473948111719"></a><a name="zh-cn_topic_0000001206147238_p15473948111719"></a>unsigned int vfg_id;</p>
<p id="zh-cn_topic_0000001206147238_p18473948181713"><a name="zh-cn_topic_0000001206147238_p18473948181713"></a><a name="zh-cn_topic_0000001206147238_p18473948181713"></a>unsigned char reserved[DCMI_VDEV_FOR_RESERVE];</p>
<p id="zh-cn_topic_0000001206147238_p1747324813170"><a name="zh-cn_topic_0000001206147238_p1747324813170"></a><a name="zh-cn_topic_0000001206147238_p1747324813170"></a>};</p>
<p id="p9319104114586"><a name="p9319104114586"></a><a name="p9319104114586"></a></p>
<div class="note" id="zh-cn_topic_0000001206147238_note192181030121814"><a name="zh-cn_topic_0000001206147238_note192181030121814"></a><a name="zh-cn_topic_0000001206147238_note192181030121814"></a><span class="notetitle"> 说明： </span><div class="notebody"><a name="ul938602520443"></a><a name="ul938602520443"></a><ul id="ul938602520443"><li>vdev_id表示指定虚拟设备对应的虚拟设备ID，默认自动分配，默认值为0xFFFFFFFF。</li><li>vfg_id表示指定虚拟设备所属的虚拟分组ID，默认自动分配，默认值为0xFFFFFFFF。</li><li>pcie_bus、pcie_device、pcie_func、vfg_id为预留参数。</li></ul>
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

**约束说明<a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table05161849203916"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row2045415135512"><th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p15636143423210"><a name="zh-cn_topic_0000002485295476_p15636143423210"></a><a name="zh-cn_topic_0000002485295476_p15636143423210"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="18.94%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p2636334183218"><a name="zh-cn_topic_0000002485295476_p2636334183218"></a><a name="zh-cn_topic_0000002485295476_p2636334183218"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p1063683420326"><a name="zh-cn_topic_0000002485295476_p1063683420326"></a><a name="zh-cn_topic_0000002485295476_p1063683420326"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p363653418324"><a name="zh-cn_topic_0000002485295476_p363653418324"></a><a name="zh-cn_topic_0000002485295476_p363653418324"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row1645410105516"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p117941512103817"><a name="zh-cn_topic_0000002485295476_p117941512103817"></a><a name="zh-cn_topic_0000002485295476_p117941512103817"></a><span id="zh-cn_topic_0000002485295476_ph1645441320389"><a name="zh-cn_topic_0000002485295476_ph1645441320389"></a><a name="zh-cn_topic_0000002485295476_ph1645441320389"></a><span id="zh-cn_topic_0000002485295476_text545418133385"><a name="zh-cn_topic_0000002485295476_text545418133385"></a><a name="zh-cn_topic_0000002485295476_text545418133385"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p5916205813388"><a name="zh-cn_topic_0000002485295476_p5916205813388"></a><a name="zh-cn_topic_0000002485295476_p5916205813388"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p19925115819389"><a name="zh-cn_topic_0000002485295476_p19925115819389"></a><a name="zh-cn_topic_0000002485295476_p19925115819389"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1933175817386"><a name="zh-cn_topic_0000002485295476_p1933175817386"></a><a name="zh-cn_topic_0000002485295476_p1933175817386"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row645461155513"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p345401105515"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p345401105515"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p345401105515"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text14454161145518"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text14454161145518"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text14454161145518"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p199362580386"><a name="zh-cn_topic_0000002485295476_p199362580386"></a><a name="zh-cn_topic_0000002485295476_p199362580386"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1394013583382"><a name="zh-cn_topic_0000002485295476_p1394013583382"></a><a name="zh-cn_topic_0000002485295476_p1394013583382"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p139430581386"><a name="zh-cn_topic_0000002485295476_p139430581386"></a><a name="zh-cn_topic_0000002485295476_p139430581386"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row245413115520"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p9454171165510"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p9454171165510"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p9454171165510"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text04544155514"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text04544155514"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text04544155514"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p15946145817381"><a name="zh-cn_topic_0000002485295476_p15946145817381"></a><a name="zh-cn_topic_0000002485295476_p15946145817381"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p194935873816"><a name="zh-cn_topic_0000002485295476_p194935873816"></a><a name="zh-cn_topic_0000002485295476_p194935873816"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p7952165812388"><a name="zh-cn_topic_0000002485295476_p7952165812388"></a><a name="zh-cn_topic_0000002485295476_p7952165812388"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row34540110555"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p845481115513"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p845481115513"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p845481115513"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text19454171145517"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text19454171145517"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text19454171145517"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p11954858173813"><a name="zh-cn_topic_0000002485295476_p11954858173813"></a><a name="zh-cn_topic_0000002485295476_p11954858173813"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p8956658123817"><a name="zh-cn_topic_0000002485295476_p8956658123817"></a><a name="zh-cn_topic_0000002485295476_p8956658123817"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p8958185843817"><a name="zh-cn_topic_0000002485295476_p8958185843817"></a><a name="zh-cn_topic_0000002485295476_p8958185843817"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row145510110556"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p1645512114557"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p1645512114557"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p1645512114557"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text445514113555"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text445514113555"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text445514113555"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p13959205863810"><a name="zh-cn_topic_0000002485295476_p13959205863810"></a><a name="zh-cn_topic_0000002485295476_p13959205863810"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1396185818385"><a name="zh-cn_topic_0000002485295476_p1396185818385"></a><a name="zh-cn_topic_0000002485295476_p1396185818385"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p8963358173816"><a name="zh-cn_topic_0000002485295476_p8963358173816"></a><a name="zh-cn_topic_0000002485295476_p8963358173816"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row77142361118"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p4168142915111"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p4168142915111"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p4168142915111"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text8168172919111"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text8168172919111"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text8168172919111"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p139651958173812"><a name="zh-cn_topic_0000002485295476_p139651958173812"></a><a name="zh-cn_topic_0000002485295476_p139651958173812"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p09671358173813"><a name="zh-cn_topic_0000002485295476_p09671358173813"></a><a name="zh-cn_topic_0000002485295476_p09671358173813"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p296855813383"><a name="zh-cn_topic_0000002485295476_p296855813383"></a><a name="zh-cn_topic_0000002485295476_p296855813383"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row1341920320316"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p164191634319"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p164191634319"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_p164191634319"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text199741574318"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text199741574318"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_text199741574318"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p149705587385"><a name="zh-cn_topic_0000002485295476_p149705587385"></a><a name="zh-cn_topic_0000002485295476_p149705587385"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p16972165817381"><a name="zh-cn_topic_0000002485295476_p16972165817381"></a><a name="zh-cn_topic_0000002485295476_p16972165817381"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p19741058163819"><a name="zh-cn_topic_0000002485295476_p19741058163819"></a><a name="zh-cn_topic_0000002485295476_p19741058163819"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002481386422_row96090408452"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p13701112414381"><a name="zh-cn_topic_0000002485295476_p13701112414381"></a><a name="zh-cn_topic_0000002485295476_p13701112414381"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206147238_zh-cn_topic_0000001177894680_zh-cn_topic_0000001121038486_toc533412083"></a>**

```
int card_id = 0; 
int device_id = 0; 
struct dcmi_create_vdev_out out = {0}; 
int ret; 
struct dcmi_create_vdev_res_stru vdev = {0};
vdev.vdev_id = 0xFFFFFFFF;
strncpy_s(vdev.template_name, sizeof(vdev.template_name), "vir10_3c_32g", strlen("vir10_3c_32g")); 
ret = dcmi_create_vdevice(card_id, device_id, &vdev, &out); 
if (ret != 0) { 
    //todo 
    return ret; 
}
```

