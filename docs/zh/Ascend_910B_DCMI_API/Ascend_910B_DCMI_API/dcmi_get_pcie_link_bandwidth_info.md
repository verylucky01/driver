# dcmi\_get\_pcie\_link\_bandwidth\_info<a name="ZH-CN_TOPIC_0000002517615307"></a>

**函数原型<a name="section196251231145019"></a>**

**int dcmi\_get\_pcie\_link\_bandwidth\_info \(int card\_id, int device\_id, struct dcmi\_pcie\_link\_bandwidth\_info \*pcie\_link\_bandwidth\_info\)**

**功能说明<a name="section116271631195017"></a>**

查询指定采样时间内，NPU设备与host OS间PCIe带宽信息。

**参数说明<a name="section11627103114506"></a>**

<a name="table196551731155016"></a>
<table><thead align="left"><tr id="row6708031165017"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="p17708123120507"><a name="p17708123120507"></a><a name="p17708123120507"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.2"><p id="p13708143185017"><a name="p13708143185017"></a><a name="p13708143185017"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.5.1.3"><p id="p67081031185017"><a name="p67081031185017"></a><a name="p67081031185017"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="p3708131145014"><a name="p3708131145014"></a><a name="p3708131145014"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row270819311506"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p3708193115501"><a name="p3708193115501"></a><a name="p3708193115501"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p1170863112507"><a name="p1170863112507"></a><a name="p1170863112507"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p17081231205013"><a name="p17081231205013"></a><a name="p17081231205013"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p27082313505"><a name="p27082313505"></a><a name="p27082313505"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1170873116504"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p11708131185016"><a name="p11708131185016"></a><a name="p11708131185016"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p1070811311504"><a name="p1070811311504"></a><a name="p1070811311504"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p970973119509"><a name="p970973119509"></a><a name="p970973119509"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p870915316504"><a name="p870915316504"></a><a name="p870915316504"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1168124917462"><a name="p1168124917462"></a><a name="p1168124917462"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row1170919312507"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p2709331125015"><a name="p2709331125015"></a><a name="p2709331125015"></a>pcie_link_bandwidth_info</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p5709153165015"><a name="p5709153165015"></a><a name="p5709153165015"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p270923119508"><a name="p270923119508"></a><a name="p270923119508"></a>dcmi_pcie_link_bandwidth_info</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p147094314502"><a name="p147094314502"></a><a name="p147094314502"></a>struct dcmi_pcie_link_bandwidth_info {</p>
<p id="p15709103111504"><a name="p15709103111504"></a><a name="p15709103111504"></a>int profiling_time; //输入，控制采样时长</p>
<p id="p8709731105013"><a name="p8709731105013"></a><a name="p8709731105013"></a>unsigned int tx_p_bw[AGENTDRV_PROF_DATA_NUM]; //输出，device设备向远端写带宽</p>
<p id="p5709431135014"><a name="p5709431135014"></a><a name="p5709431135014"></a>unsigned int tx_np_bw[AGENTDRV_PROF_DATA_NUM]; //输出，device设备从远端读带宽</p>
<p id="p770913120509"><a name="p770913120509"></a><a name="p770913120509"></a>unsigned int tx_cpl_bw[AGENTDRV_PROF_DATA_NUM]; //输出，device设备回复远端读操作CPL的带宽</p>
<p id="p67096319500"><a name="p67096319500"></a><a name="p67096319500"></a>unsigned int tx_np_lantency[AGENTDRV_PROF_DATA_NUM];</p>
<p id="p18709631165013"><a name="p18709631165013"></a><a name="p18709631165013"></a>//输出，device设备从远端读的延时（ns）</p>
<p id="p19709173185010"><a name="p19709173185010"></a><a name="p19709173185010"></a>unsigned int rx_p_bw[AGENTDRV_PROF_DATA_NUM]; //输出，device设备接收远端写的带宽</p>
<p id="p15709631135012"><a name="p15709631135012"></a><a name="p15709631135012"></a>unsigned int rx_np_bw[AGENTDRV_PROF_DATA_NUM]; //输出，device设备接收远端读的带宽</p>
<p id="p27091312507"><a name="p27091312507"></a><a name="p27091312507"></a>unsigned int rx_cpl_bw[AGENTDRV_PROF_DATA_NUM]; //输出，device设备从远端读收到CPL回复的带宽</p>
<p id="p5709173115012"><a name="p5709173115012"></a><a name="p5709173115012"></a>};</p>
<p id="p157098315509"><a name="p157098315509"></a><a name="p157098315509"></a>profiling_time取值范围：0ms~2000ms。</p>
<p id="p207091631185018"><a name="p207091631185018"></a><a name="p207091631185018"></a>带宽单位均为MB/ms。</p>
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

**约束说明<a name="section364683165017"></a>**

-   该接口在开启profiling场景下数据无意义。
-   该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table1831828113412"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row19901425175211"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p1555917570362"><a name="zh-cn_topic_0000002485295476_p1555917570362"></a><a name="zh-cn_topic_0000002485295476_p1555917570362"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p201384912311"><a name="zh-cn_topic_0000002485295476_p201384912311"></a><a name="zh-cn_topic_0000002485295476_p201384912311"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p513124915316"><a name="zh-cn_topic_0000002485295476_p513124915316"></a><a name="zh-cn_topic_0000002485295476_p513124915316"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p1131549173112"><a name="zh-cn_topic_0000002485295476_p1131549173112"></a><a name="zh-cn_topic_0000002485295476_p1131549173112"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p232525365317"><a name="zh-cn_topic_0000002485295476_p232525365317"></a><a name="zh-cn_topic_0000002485295476_p232525365317"></a><span id="zh-cn_topic_0000002485295476_ph3325165314533"><a name="zh-cn_topic_0000002485295476_ph3325165314533"></a><a name="zh-cn_topic_0000002485295476_ph3325165314533"></a><span id="zh-cn_topic_0000002485295476_text2032565316533"><a name="zh-cn_topic_0000002485295476_text2032565316533"></a><a name="zh-cn_topic_0000002485295476_text2032565316533"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row6920114611302"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p33251753135313"><a name="zh-cn_topic_0000002485295476_p33251753135313"></a><a name="zh-cn_topic_0000002485295476_p33251753135313"></a><span id="zh-cn_topic_0000002485295476_text232555315538"><a name="zh-cn_topic_0000002485295476_text232555315538"></a><a name="zh-cn_topic_0000002485295476_text232555315538"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p862174514211"><a name="zh-cn_topic_0000002485295476_p862174514211"></a><a name="zh-cn_topic_0000002485295476_p862174514211"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p12621104519215"><a name="zh-cn_topic_0000002485295476_p12621104519215"></a><a name="zh-cn_topic_0000002485295476_p12621104519215"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p6621204519220"><a name="zh-cn_topic_0000002485295476_p6621204519220"></a><a name="zh-cn_topic_0000002485295476_p6621204519220"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row293019912529"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p532515315535"><a name="zh-cn_topic_0000002485295476_p532515315535"></a><a name="zh-cn_topic_0000002485295476_p532515315535"></a><span id="zh-cn_topic_0000002485295476_text732545315315"><a name="zh-cn_topic_0000002485295476_text732545315315"></a><a name="zh-cn_topic_0000002485295476_text732545315315"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p386884075314"><a name="zh-cn_topic_0000002485295476_p386884075314"></a><a name="zh-cn_topic_0000002485295476_p386884075314"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p12868184075310"><a name="zh-cn_topic_0000002485295476_p12868184075310"></a><a name="zh-cn_topic_0000002485295476_p12868184075310"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p986814025319"><a name="zh-cn_topic_0000002485295476_p986814025319"></a><a name="zh-cn_topic_0000002485295476_p986814025319"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row29531122175210"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1832516539533"><a name="zh-cn_topic_0000002485295476_p1832516539533"></a><a name="zh-cn_topic_0000002485295476_p1832516539533"></a><span id="zh-cn_topic_0000002485295476_text143251353185313"><a name="zh-cn_topic_0000002485295476_text143251353185313"></a><a name="zh-cn_topic_0000002485295476_text143251353185313"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p16576467538"><a name="zh-cn_topic_0000002485295476_p16576467538"></a><a name="zh-cn_topic_0000002485295476_p16576467538"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13657346135315"><a name="zh-cn_topic_0000002485295476_p13657346135315"></a><a name="zh-cn_topic_0000002485295476_p13657346135315"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p10657164695315"><a name="zh-cn_topic_0000002485295476_p10657164695315"></a><a name="zh-cn_topic_0000002485295476_p10657164695315"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row2737141919528"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1632695317538"><a name="zh-cn_topic_0000002485295476_p1632695317538"></a><a name="zh-cn_topic_0000002485295476_p1632695317538"></a><span id="zh-cn_topic_0000002485295476_text532612537537"><a name="zh-cn_topic_0000002485295476_text532612537537"></a><a name="zh-cn_topic_0000002485295476_text532612537537"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1844654714531"><a name="zh-cn_topic_0000002485295476_p1844654714531"></a><a name="zh-cn_topic_0000002485295476_p1844654714531"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p64461747205318"><a name="zh-cn_topic_0000002485295476_p64461747205318"></a><a name="zh-cn_topic_0000002485295476_p64461747205318"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1446184725320"><a name="zh-cn_topic_0000002485295476_p1446184725320"></a><a name="zh-cn_topic_0000002485295476_p1446184725320"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row78171617135218"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p3326853195318"><a name="zh-cn_topic_0000002485295476_p3326853195318"></a><a name="zh-cn_topic_0000002485295476_p3326853195318"></a><span id="zh-cn_topic_0000002485295476_text15326195345317"><a name="zh-cn_topic_0000002485295476_text15326195345317"></a><a name="zh-cn_topic_0000002485295476_text15326195345317"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p2967114725312"><a name="zh-cn_topic_0000002485295476_p2967114725312"></a><a name="zh-cn_topic_0000002485295476_p2967114725312"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1396754795312"><a name="zh-cn_topic_0000002485295476_p1396754795312"></a><a name="zh-cn_topic_0000002485295476_p1396754795312"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p596734725316"><a name="zh-cn_topic_0000002485295476_p596734725316"></a><a name="zh-cn_topic_0000002485295476_p596734725316"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1913121595213"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p532613533533"><a name="zh-cn_topic_0000002485295476_p532613533533"></a><a name="zh-cn_topic_0000002485295476_p532613533533"></a><span id="zh-cn_topic_0000002485295476_text332611538530"><a name="zh-cn_topic_0000002485295476_text332611538530"></a><a name="zh-cn_topic_0000002485295476_text332611538530"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1452484814533"><a name="zh-cn_topic_0000002485295476_p1452484814533"></a><a name="zh-cn_topic_0000002485295476_p1452484814533"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p19524174835320"><a name="zh-cn_topic_0000002485295476_p19524174835320"></a><a name="zh-cn_topic_0000002485295476_p19524174835320"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p652419482532"><a name="zh-cn_topic_0000002485295476_p652419482532"></a><a name="zh-cn_topic_0000002485295476_p652419482532"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row71831758163218"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p28486114331"><a name="zh-cn_topic_0000002485295476_p28486114331"></a><a name="zh-cn_topic_0000002485295476_p28486114331"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section13653173113509"></a>**

```
…
int ret = 0;
int card_id = 0;
int device_id = 0;
struct dcmi_pcie_link_bandwidth_info bandwidth_info = {0};
bandwidth_info.profiling_time = 1000;
ret = dcmi_get_pcie_link_bandwidth_info(card_id, device_id, &bandwidth_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

