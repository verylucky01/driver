# dcmi\_get\_pcie\_error\_cnt<a name="ZH-CN_TOPIC_0000002517638713"></a>

**函数原型<a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_toc533412077"></a>**

**int dcmi\_get\_pcie\_error\_cnt\(int card\_id, int device\_id, struct dcmi\_chip\_pcie\_err\_rate\_stru \*pcie\_err\_code\_info\)**

**功能说明<a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_toc533412078"></a>**

查询芯片的PCIe（Peripheral Component Interconnect Express）链路误码信息。

**参数说明<a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_toc533412079"></a>**

<a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p10021890"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p10021890"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p6466753"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p6466753"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p54045009"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p54045009"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p15569626"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p15569626"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p36741947142813"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p36741947142813"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p96741747122818"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p96741747122818"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p46747472287"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p46747472287"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1467413474281"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1467413474281"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p7711145152918"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p7711145152918"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p671116522914"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p671116522914"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1771116572910"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1771116572910"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p154847210518"><a name="p154847210518"></a><a name="p154847210518"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096682333718"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096682333718"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096682333718"></a>pcie_err_code_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p14966423173711"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p14966423173711"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p14966423173711"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p3966023173711"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p3966023173711"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p3966023173711"></a>struct dcmi_chip_pcie_err_rate_stru *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p129666233374"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p129666233374"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p129666233374"></a>struct dcmi_chip_pcie_err_rate_stru {</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p12966112303716"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p12966112303716"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p12966112303716"></a>unsigned int reg_deskew_fifo_overflow_intr_status;</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p48305971916"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p48305971916"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p48305971916"></a>//是否发生deskew_fifo溢出：1表示已发生，0表示未发生。</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2966123133710"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2966123133710"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2966123133710"></a>unsigned int reg_symbol_unlock_intr_status;</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p198291723181916"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p198291723181916"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p198291723181916"></a>//是否发生symbol_unlock事件：1表示已发生，0表示未发生。</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p129667238376"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p129667238376"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p129667238376"></a>unsigned int reg_deskew_unlock_intr_status;</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1683033611919"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1683033611919"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1683033611919"></a>//是否发生deskew_unlock事件：1表示已发生，0表示未发生。</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p19966223143718"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p19966223143718"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p19966223143718"></a>unsigned int reg_phystatus_timeout_intr_status;</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p9803125910199"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p9803125910199"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p9803125910199"></a>//是否发生phystatus超时事件：1表示已发生，0表示未发生。</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p296620236378"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p296620236378"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p296620236378"></a>unsigned int symbol_unlock_counter;//symbol_unlock错误计数</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p14966923183714"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p14966923183714"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p14966923183714"></a>unsigned int pcs_rx_err_cnt;//PCS层接收错误计数</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1596619233375"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1596619233375"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1596619233375"></a>unsigned int phy_lane_err_counter;//lane错误计数</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p19665236371"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p19665236371"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p19665236371"></a>unsigned int pcs_rcv_err_status;//PCS层接收错误状态，每bit映射到每个使用的通道：1表示有错误，0表示正常。</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p296652317373"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p296652317373"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p296652317373"></a>unsigned int symbol_unlock_err_status;</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p6795195313207"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p6795195313207"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p6795195313207"></a>//symbol_unlock标志，每bit映射到每个使用的通道：1表示有错误，0表示正常。</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p596632393719"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p596632393719"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p596632393719"></a>unsigned int phy_lane_err_status;</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p189516902110"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p189516902110"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p189516902110"></a>//lane错误，每bit映射到每个使用的通道：1表示有错误，0表示正常。</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p096682312371"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p096682312371"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p096682312371"></a>unsigned int dl_lcrc_err_num; //PCIe DLLP Lcrc的错误计数</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1296612317373"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1296612317373"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p1296612317373"></a>unsigned int dl_dcrc_err_num; // PCIe DLLP Dcrc的错误计数</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096612313376"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096612313376"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096612313376"></a>}</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**约束说明<a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_toc533412082"></a>**

该接口在后续版本将会删除，推荐使用[dcmi\_get\_device\_pcie\_error\_cnt](dcmi_get_device_pcie_error_cnt.md)。

**表 1** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_row119194469302"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p1842495418394"><a name="zh-cn_topic_0000002485318818_p1842495418394"></a><a name="zh-cn_topic_0000002485318818_p1842495418394"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p1642455415393"><a name="zh-cn_topic_0000002485318818_p1642455415393"></a><a name="zh-cn_topic_0000002485318818_p1642455415393"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p114246541390"><a name="zh-cn_topic_0000002485318818_p114246541390"></a><a name="zh-cn_topic_0000002485318818_p114246541390"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p64241654123916"><a name="zh-cn_topic_0000002485318818_p64241654123916"></a><a name="zh-cn_topic_0000002485318818_p64241654123916"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p1378374019445"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p1378374019445"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p1378374019445"></a><span id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text262011415447"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text262011415447"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text262011415447"></a><span id="zh-cn_topic_0000002485318818_text1564782665814"><a name="zh-cn_topic_0000002485318818_text1564782665814"></a><a name="zh-cn_topic_0000002485318818_text1564782665814"></a>Atlas 900 A3 SuperPoD 超节点</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p11683124944919"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p11683124944919"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p11683124944919"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p36831949194913"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p36831949194913"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p36831949194913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p2683144919499"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p2683144919499"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p2683144919499"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_row82952324359"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p137832403444"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p137832403444"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p137832403444"></a><span id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text6814122411310"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text6814122411310"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text6814122411310"></a><span id="zh-cn_topic_0000002485318818_text1123515513517"><a name="zh-cn_topic_0000002485318818_text1123515513517"></a><a name="zh-cn_topic_0000002485318818_text1123515513517"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1285753073419"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1285753073419"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1285753073419"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1985743013346"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1985743013346"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1985743013346"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p0857203013412"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p0857203013412"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p0857203013412"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row12693311552"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p17872181618166"><a name="zh-cn_topic_0000002485318818_p17872181618166"></a><a name="zh-cn_topic_0000002485318818_p17872181618166"></a><span id="zh-cn_topic_0000002485318818_text17872121641614"><a name="zh-cn_topic_0000002485318818_text17872121641614"></a><a name="zh-cn_topic_0000002485318818_text17872121641614"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p177521042115520"><a name="zh-cn_topic_0000002485318818_p177521042115520"></a><a name="zh-cn_topic_0000002485318818_p177521042115520"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p12752114215559"><a name="zh-cn_topic_0000002485318818_p12752114215559"></a><a name="zh-cn_topic_0000002485318818_p12752114215559"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p18752104275512"><a name="zh-cn_topic_0000002485318818_p18752104275512"></a><a name="zh-cn_topic_0000002485318818_p18752104275512"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row179407221298"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p696322918294"><a name="zh-cn_topic_0000002485318818_p696322918294"></a><a name="zh-cn_topic_0000002485318818_p696322918294"></a><span id="zh-cn_topic_0000002485318818_text15963162972914"><a name="zh-cn_topic_0000002485318818_text15963162972914"></a><a name="zh-cn_topic_0000002485318818_text15963162972914"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1560813335296"><a name="zh-cn_topic_0000002485318818_p1560813335296"></a><a name="zh-cn_topic_0000002485318818_p1560813335296"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p260873392918"><a name="zh-cn_topic_0000002485318818_p260873392918"></a><a name="zh-cn_topic_0000002485318818_p260873392918"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p5608333162914"><a name="zh-cn_topic_0000002485318818_p5608333162914"></a><a name="zh-cn_topic_0000002485318818_p5608333162914"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row19346183355516"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p0905133713551"><a name="zh-cn_topic_0000002485318818_p0905133713551"></a><a name="zh-cn_topic_0000002485318818_p0905133713551"></a><span id="zh-cn_topic_0000002485318818_text1190563775520"><a name="zh-cn_topic_0000002485318818_text1190563775520"></a><a name="zh-cn_topic_0000002485318818_text1190563775520"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p7204194395516"><a name="zh-cn_topic_0000002485318818_p7204194395516"></a><a name="zh-cn_topic_0000002485318818_p7204194395516"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p13205104310553"><a name="zh-cn_topic_0000002485318818_p13205104310553"></a><a name="zh-cn_topic_0000002485318818_p13205104310553"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p32051943125519"><a name="zh-cn_topic_0000002485318818_p32051943125519"></a><a name="zh-cn_topic_0000002485318818_p32051943125519"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1158213431480"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p65418471681"><a name="zh-cn_topic_0000002485318818_p65418471681"></a><a name="zh-cn_topic_0000002485318818_p65418471681"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
struct dcmi_chip_pcie_err_rate_stru pcie_err_code_info = {0};
ret = dcmi_get_pcie_error_cnt(card_id, device_id, &pcie_err_code_info);
if (ret != 0){
    //todo:记录日志
    return ret;
}
…
```

