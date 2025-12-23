# dcmi\_rescan\_soc<a name="ZH-CN_TOPIC_0000002517558647"></a>

**函数原型<a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_toc533412077"></a>**

**int dcmi\_rescan\_soc\(int card\_id, int device\_id\)**

**功能说明<a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_toc533412078"></a>**

对指定芯片重新扫描。

**参数说明<a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_toc533412079"></a>**

<a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p10021890"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p10021890"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p6466753"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p6466753"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p54045009"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p54045009"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p15569626"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p15569626"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p36741947142813"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p36741947142813"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p96741747122818"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p96741747122818"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p46747472287"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p46747472287"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p1467413474281"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p1467413474281"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p7711145152918"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p7711145152918"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p671116522914"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p671116522914"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p1771116572910"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p1771116572910"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206307244_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206307244_p12926144711404"><a name="zh-cn_topic_0000001206307244_p12926144711404"></a><a name="zh-cn_topic_0000001206307244_p12926144711404"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p175231159192315"><a name="p175231159192315"></a><a name="p175231159192315"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row59025204"><th class="cellrowborder" valign="top" width="24.18%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="75.82%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row13362997"><td class="cellrowborder" valign="top" width="24.18%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="75.82%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a>处理结果：</p>
<a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><ul id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"><li>成功：返回0。</li><li>失败：返回码请参见<a href="return_codes.md">return_codes</a>。</li></ul>
</td>
</tr>
</tbody>
</table>

**异常处理<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_toc533412081"></a>**

无。

**约束说明<a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_toc533412082"></a>**

-   扫描指定芯片：对于Atlas 9000 A3 SuperPoD 集群算力系统会扫描指定芯片所在的NPU模组。
-   非带外复位情况下扫描指定芯片：对于Atlas 900 A3 SuperPoD 超节点、Atlas 800T A3 超节点、Atlas 800I A3 超节点会扫描指定芯片所在的NPU模组。
-   带外复位情况下扫描指定芯片：对于Atlas 900 A3 SuperPoD 超节点、Atlas 800T A3 超节点、Atlas 800I A3 超节点会扫描指定芯片所在的NPU模组及与其具备网口互助关系的NPU模组，网口互助关系的模组查询请参见[dcmi\_get\_netdev\_brother\_device](dcmi_get_netdev_brother_device.md)。
-   对于Atlas 9000 A3 SuperPoD 集群算力系统、Atlas 900 A3 SuperPoD 超节点、Atlas 800T A3 超节点和Atlas 800I A3 超节点，该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table4236192711819"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row0232969110"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p189461302408"><a name="zh-cn_topic_0000002485318818_p189461302408"></a><a name="zh-cn_topic_0000002485318818_p189461302408"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p49463084019"><a name="zh-cn_topic_0000002485318818_p49463084019"></a><a name="zh-cn_topic_0000002485318818_p49463084019"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p16946120124018"><a name="zh-cn_topic_0000002485318818_p16946120124018"></a><a name="zh-cn_topic_0000002485318818_p16946120124018"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p1794610013409"><a name="zh-cn_topic_0000002485318818_p1794610013409"></a><a name="zh-cn_topic_0000002485318818_p1794610013409"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row42321268116"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p132321461118"><a name="zh-cn_topic_0000002485318818_p132321461118"></a><a name="zh-cn_topic_0000002485318818_p132321461118"></a><span id="zh-cn_topic_0000002485318818_text32327612114"><a name="zh-cn_topic_0000002485318818_text32327612114"></a><a name="zh-cn_topic_0000002485318818_text32327612114"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p42321061216"><a name="zh-cn_topic_0000002485318818_p42321061216"></a><a name="zh-cn_topic_0000002485318818_p42321061216"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p2023212612117"><a name="zh-cn_topic_0000002485318818_p2023212612117"></a><a name="zh-cn_topic_0000002485318818_p2023212612117"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p18553459193013"><a name="zh-cn_topic_0000002485318818_p18553459193013"></a><a name="zh-cn_topic_0000002485318818_p18553459193013"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row3232661513"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p192331461410"><a name="zh-cn_topic_0000002485318818_p192331461410"></a><a name="zh-cn_topic_0000002485318818_p192331461410"></a><span id="zh-cn_topic_0000002485318818_text22331061317"><a name="zh-cn_topic_0000002485318818_text22331061317"></a><a name="zh-cn_topic_0000002485318818_text22331061317"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p12233961611"><a name="zh-cn_topic_0000002485318818_p12233961611"></a><a name="zh-cn_topic_0000002485318818_p12233961611"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p32331261514"><a name="zh-cn_topic_0000002485318818_p32331261514"></a><a name="zh-cn_topic_0000002485318818_p32331261514"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p9553559133013"><a name="zh-cn_topic_0000002485318818_p9553559133013"></a><a name="zh-cn_topic_0000002485318818_p9553559133013"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1233661415"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p139545514163"><a name="zh-cn_topic_0000002485318818_p139545514163"></a><a name="zh-cn_topic_0000002485318818_p139545514163"></a><span id="zh-cn_topic_0000002485318818_text3954185161611"><a name="zh-cn_topic_0000002485318818_text3954185161611"></a><a name="zh-cn_topic_0000002485318818_text3954185161611"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p122331168119"><a name="zh-cn_topic_0000002485318818_p122331168119"></a><a name="zh-cn_topic_0000002485318818_p122331168119"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p72333612115"><a name="zh-cn_topic_0000002485318818_p72333612115"></a><a name="zh-cn_topic_0000002485318818_p72333612115"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p16553159123012"><a name="zh-cn_topic_0000002485318818_p16553159123012"></a><a name="zh-cn_topic_0000002485318818_p16553159123012"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1233465119"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p12338612117"><a name="zh-cn_topic_0000002485318818_p12338612117"></a><a name="zh-cn_topic_0000002485318818_p12338612117"></a><span id="zh-cn_topic_0000002485318818_text17233661215"><a name="zh-cn_topic_0000002485318818_text17233661215"></a><a name="zh-cn_topic_0000002485318818_text17233661215"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p52331461012"><a name="zh-cn_topic_0000002485318818_p52331461012"></a><a name="zh-cn_topic_0000002485318818_p52331461012"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p15233564117"><a name="zh-cn_topic_0000002485318818_p15233564117"></a><a name="zh-cn_topic_0000002485318818_p15233564117"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p055316592308"><a name="zh-cn_topic_0000002485318818_p055316592308"></a><a name="zh-cn_topic_0000002485318818_p055316592308"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row22331614114"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p192333618117"><a name="zh-cn_topic_0000002485318818_p192333618117"></a><a name="zh-cn_topic_0000002485318818_p192333618117"></a><span id="zh-cn_topic_0000002485318818_text92331361411"><a name="zh-cn_topic_0000002485318818_text92331361411"></a><a name="zh-cn_topic_0000002485318818_text92331361411"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p12233461615"><a name="zh-cn_topic_0000002485318818_p12233461615"></a><a name="zh-cn_topic_0000002485318818_p12233461615"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p3233366111"><a name="zh-cn_topic_0000002485318818_p3233366111"></a><a name="zh-cn_topic_0000002485318818_p3233366111"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p52331166118"><a name="zh-cn_topic_0000002485318818_p52331166118"></a><a name="zh-cn_topic_0000002485318818_p52331166118"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1819971910919"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1172823598"><a name="zh-cn_topic_0000002485318818_p1172823598"></a><a name="zh-cn_topic_0000002485318818_p1172823598"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206307244_zh-cn_topic_0000001223292875_zh-cn_topic_0000001101959222_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
ret = dcmi_rescan_soc(card_id, device_id);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

