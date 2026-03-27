# dcmi\_get\_device\_id\_in\_card<a name="ZH-CN_TOPIC_0000002517558607"></a>

**函数原型<a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_toc533412077"></a>**

**int dcmi\_get\_device\_id\_in\_card\(int card\_id, int \*device\_id\_max, int \*mcu\_id, int \*cpu\_id\)**

**功能说明<a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_toc533412078"></a>**

查询指定设备上的芯片数量、MCU ID和CPU ID。

**参数说明<a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_toc533412079"></a>**

<a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_row7580267"><th class="cellrowborder" valign="top" width="17.07%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p10021890"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p10021890"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.919999999999998%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p6466753"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p6466753"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.009999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p54045009"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p54045009"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p15569626"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p15569626"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_row10560021192510"><td class="cellrowborder" valign="top" width="17.07%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p20163113651013"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p20163113651013"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p20163113651013"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.919999999999998%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p5163173611017"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p5163173611017"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p5163173611017"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14163153619104"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14163153619104"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14163153619104"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p19163336161013"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p19163336161013"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p19163336161013"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_row1155711494235"><td class="cellrowborder" valign="top" width="17.07%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p2163113616105"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p2163113616105"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p2163113616105"></a>device_id_max</p>
</td>
<td class="cellrowborder" valign="top" width="16.919999999999998%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p17163136171012"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p17163136171012"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p17163136171012"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p016343613104"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p016343613104"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p016343613104"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p7163173613102"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p7163173613102"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p7163173613102"></a>设备中NPU芯片最大个数。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_row18153156101319"><td class="cellrowborder" valign="top" width="17.07%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p18154456201313"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p18154456201313"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p18154456201313"></a>mcu_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.919999999999998%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p5154125611133"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p5154125611133"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p5154125611133"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p1815415621310"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p1815415621310"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p1815415621310"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p81545562136"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p81545562136"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p81545562136"></a>设备中MCU的ID。取值为-1，表示无MCU。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_row16457959111316"><td class="cellrowborder" valign="top" width="17.07%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p1345775911312"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p1345775911312"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p1345775911312"></a>cpu_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.919999999999998%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14457125941313"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14457125941313"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14457125941313"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p9457059141320"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p9457059141320"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p9457059141320"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14626131251519"><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14626131251519"></a><a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_p14626131251519"></a>设备中控制CPU的ID。默认取值为-1，无实际意义。</p>
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

**约束说明<a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_toc533412082"></a>**

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

**调用示例<a name="zh-cn_topic_0000001251227183_zh-cn_topic_0000001177894702_zh-cn_topic_0000001170697495_toc533412083"></a>**

```
… 
int ret;
int device_id_max = 0;
int mcu_id = 0;
int cpu_id = 0;
int card_id = 0;
ret = dcmi_get_device_id_in_card(card_id, &device_id_max, &mcu_id, &cpu_id);
…
```

