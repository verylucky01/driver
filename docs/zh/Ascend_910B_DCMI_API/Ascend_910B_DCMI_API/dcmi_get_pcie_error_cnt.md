# dcmi\_get\_pcie\_error\_cnt<a name="ZH-CN_TOPIC_0000002517615303"></a>

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
<p id="p1415311871313"><a name="p1415311871313"></a><a name="p1415311871313"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
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
<p id="zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p153851814202"><a name="zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p153851814202"></a><a name="zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p153851814202"></a>unsigned int dl_lcrc_err_num;//PCIe DLLP Lcrc的错误计数</p>
<p id="zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p438518141801"><a name="zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p438518141801"></a><a name="zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p438518141801"></a>unsigned int dl_dcrc_err_num;//PCIe DLLP Dcrc的错误计数</p>
<p id="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096612313376"><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096612313376"></a><a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_p2096612313376"></a>}</p>
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

**约束说明<a name="zh-cn_topic_0000001251307149_zh-cn_topic_0000001177894690_zh-cn_topic_0000001148430773_toc533412082"></a>**

该接口在后续版本将会删除，推荐使用[dcmi\_get\_device\_pcie\_error\_cnt](dcmi_get_device_pcie_error_cnt.md)。

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

