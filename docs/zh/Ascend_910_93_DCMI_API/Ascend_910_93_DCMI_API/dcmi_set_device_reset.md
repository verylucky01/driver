# dcmi\_set\_device\_reset<a name="ZH-CN_TOPIC_0000002485478736"></a>

**函数原型<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412077"></a>**

**int dcmi\_set\_device\_reset\(int card\_id, int device\_id, enum dcmi\_reset\_channel channel\_type\)**

**功能说明<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412078"></a>**

复位芯片。

**参数说明<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p10021890"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p10021890"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.99%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p6466753"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p6466753"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.009999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p54045009"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p54045009"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p15569626"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p15569626"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row5908907"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p36741947142813"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p36741947142813"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p96741747122818"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p96741747122818"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p46747472287"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p46747472287"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1467413474281"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1467413474281"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
<p id="p1369718431238"><a name="p1369718431238"></a><a name="p1369718431238"></a></p>
<div class="note" id="note9881134113272"><a name="note9881134113272"></a><a name="note9881134113272"></a><span class="notetitle"> 说明： </span><div class="notebody"><a name="ul6966185610185"></a><a name="ul6966185610185"></a><ul id="ul6966185610185"><li>使用该接口进行芯片带内复位时，物理机场景（裸机）root用户和物理机+特权容器场景支持card_id传入0xFF，进行全卡复位。</li><li>使用该接口进行芯片带外复位时，不支持card_id传入0xFF。</li></ul>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row45631627"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p7711145152918"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p7711145152918"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p671116522914"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p671116522914"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1771116572910"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1771116572910"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p10757141201716"><a name="p10757141201716"></a><a name="p10757141201716"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1622474582312"><a name="p1622474582312"></a><a name="p1622474582312"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_row137415397316"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1622474112317"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1622474112317"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1622474112317"></a>channel_type</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p19225441193116"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p19225441193116"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p19225441193116"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p102251441133110"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p102251441133110"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p102251441133110"></a>enum dcmi_reset_channel</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3511101013286"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3511101013286"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3511101013286"></a>复位通道。</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p136522222313"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p136522222313"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p136522222313"></a>enum dcmi_reset_channel {</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p365822172318"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p365822172318"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p365822172318"></a>OUTBAND_CHANNEL = 0, //带外复位</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3662222235"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3662222235"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p3662222235"></a>INBAND_CHANNEL = 1 //带内复位</p>
<p id="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1066112202319"><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1066112202319"></a><a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_p1066112202319"></a>}</p>
<p id="p594524792318"><a name="p594524792318"></a><a name="p594524792318"></a></p>
<div class="note" id="note20188544163013"><a name="note20188544163013"></a><a name="note20188544163013"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p159001111195110"><a name="p159001111195110"></a><a name="p159001111195110"></a><span id="text910053102312"><a name="text910053102312"></a><a name="text910053102312"></a>A200T A3 Box8 超节点服务器</span>不支持OUTBAND_CHANNEL。</p>
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

**约束说明<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412082"></a>**

-   带外复位芯片功能依赖ipmitool软件，需要提前下载并加载驱动。详细操作请参见[准备ipmitool软件](准备ipmitool软件.md)章节。
-   iBMC版本为5.8.3.43及之后的版本支持带外复位芯片功能。
-   带外复位芯片依赖[dcmi\_get\_device\_outband\_channel\_state](dcmi_get_device_outband_channel_state.md)和[dcmi\_pre\_reset\_soc](dcmi_pre_reset_soc.md)接口，请先确保带外通道状态为正常并且芯片预复位成功，再调用该接口进行带外复位。
-   复位指定芯片：对于Atlas 9000 A3 SuperPoD 集群算力系统会复位指定芯片所在的NPU模组；对于Atlas 900 A3 SuperPoD 超节点、Atlas 800T A3 超节点、Atlas 800I A3 超节点、A200T A3 Box8 超节点服务器会复位指定芯片所在的NPU模组及与其具备网口互助关系的NPU模组，网口互助关系的模组查询请参见[dcmi\_get\_netdev\_brother\_device](dcmi_get_netdev_brother_device.md)。

-   对于Atlas 9000 A3 SuperPoD 集群算力系统、Atlas 900 A3 SuperPoD 超节点、Atlas 800T A3 超节点和Atlas 800I A3 超节点，该接口支持在物理机+特权容器场景下使用；对于A200T A3 Box8 超节点服务器，该接口支持在物理机+特权容器场景下进行芯片带内复位，不支持在物理机+特权容器场景下进行芯片带外复位。

**表 1** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002165418008_row18113171210"><th class="cellrowborder" rowspan="2" valign="top" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002165418008_p1881934126"><a name="zh-cn_topic_0000002165418008_p1881934126"></a><a name="zh-cn_topic_0000002165418008_p1881934126"></a>产品形态</p>
</th>
<th class="cellrowborder" colspan="2" valign="top" id="mcps1.2.5.1.2"><p id="p1153744312441"><a name="p1153744312441"></a><a name="p1153744312441"></a>物理机场景（裸机）</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002234309644_p92326610111"><a name="zh-cn_topic_0000002234309644_p92326610111"></a><a name="zh-cn_topic_0000002234309644_p92326610111"></a>物理机+普通容器场景</p>
</th>
</tr>
<tr id="zh-cn_topic_0000002165418008_row681203121213"><th class="cellrowborder" valign="top" id="mcps1.2.5.2.1"><p id="zh-cn_topic_0000002165418008_p11812310123"><a name="zh-cn_topic_0000002165418008_p11812310123"></a><a name="zh-cn_topic_0000002165418008_p11812310123"></a>root用户</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.2.5.2.2"><p id="zh-cn_topic_0000002165418008_p188215311122"><a name="zh-cn_topic_0000002165418008_p188215311122"></a><a name="zh-cn_topic_0000002165418008_p188215311122"></a>运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.2.5.2.3"><p id="zh-cn_topic_0000002165418008_p58213311210"><a name="zh-cn_topic_0000002165418008_p58213311210"></a><a name="zh-cn_topic_0000002165418008_p58213311210"></a>root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002165418008_row38243131214"><td class="cellrowborder" valign="top" width="24.86%" headers="mcps1.2.5.1.1 mcps1.2.5.2.1 "><p id="zh-cn_topic_0000002165418008_p48210361212"><a name="zh-cn_topic_0000002165418008_p48210361212"></a><a name="zh-cn_topic_0000002165418008_p48210361212"></a><span id="zh-cn_topic_0000002165418008_text16821238124"><a name="zh-cn_topic_0000002165418008_text16821238124"></a><a name="zh-cn_topic_0000002165418008_text16821238124"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.14%" headers="mcps1.2.5.1.2 mcps1.2.5.2.2 "><p id="zh-cn_topic_0000002165418008_p1990911717125"><a name="zh-cn_topic_0000002165418008_p1990911717125"></a><a name="zh-cn_topic_0000002165418008_p1990911717125"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 mcps1.2.5.2.3 "><p id="zh-cn_topic_0000002165418008_p1590914714124"><a name="zh-cn_topic_0000002165418008_p1590914714124"></a><a name="zh-cn_topic_0000002165418008_p1590914714124"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p750721011231"><a name="p750721011231"></a><a name="p750721011231"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002165418008_row982437122"><td class="cellrowborder" valign="top" width="24.86%" headers="mcps1.2.5.1.1 mcps1.2.5.2.1 "><p id="zh-cn_topic_0000002165418008_p4829381220"><a name="zh-cn_topic_0000002165418008_p4829381220"></a><a name="zh-cn_topic_0000002165418008_p4829381220"></a><span id="zh-cn_topic_0000002165418008_text582173201218"><a name="zh-cn_topic_0000002165418008_text582173201218"></a><a name="zh-cn_topic_0000002165418008_text582173201218"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.14%" headers="mcps1.2.5.1.2 mcps1.2.5.2.2 "><p id="zh-cn_topic_0000002165418008_p59098719121"><a name="zh-cn_topic_0000002165418008_p59098719121"></a><a name="zh-cn_topic_0000002165418008_p59098719121"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 mcps1.2.5.2.3 "><p id="zh-cn_topic_0000002165418008_p20909197161211"><a name="zh-cn_topic_0000002165418008_p20909197161211"></a><a name="zh-cn_topic_0000002165418008_p20909197161211"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p9507171062314"><a name="p9507171062314"></a><a name="p9507171062314"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002165418008_row1782915383548"><td class="cellrowborder" valign="top" width="24.86%" headers="mcps1.2.5.1.1 mcps1.2.5.2.1 "><p id="zh-cn_topic_0000002165418008_p1465274619544"><a name="zh-cn_topic_0000002165418008_p1465274619544"></a><a name="zh-cn_topic_0000002165418008_p1465274619544"></a><span id="zh-cn_topic_0000002165418008_text10652134635412"><a name="zh-cn_topic_0000002165418008_text10652134635412"></a><a name="zh-cn_topic_0000002165418008_text10652134635412"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.14%" headers="mcps1.2.5.1.2 mcps1.2.5.2.2 "><p id="zh-cn_topic_0000002165418008_p107731653135415"><a name="zh-cn_topic_0000002165418008_p107731653135415"></a><a name="zh-cn_topic_0000002165418008_p107731653135415"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 mcps1.2.5.2.3 "><p id="zh-cn_topic_0000002165418008_p77730531545"><a name="zh-cn_topic_0000002165418008_p77730531545"></a><a name="zh-cn_topic_0000002165418008_p77730531545"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p14507121019238"><a name="p14507121019238"></a><a name="p14507121019238"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002165418008_row19683944122816"><td class="cellrowborder" valign="top" width="24.86%" headers="mcps1.2.5.1.1 mcps1.2.5.2.1 "><p id="zh-cn_topic_0000002165418008_p19717154811286"><a name="zh-cn_topic_0000002165418008_p19717154811286"></a><a name="zh-cn_topic_0000002165418008_p19717154811286"></a><span id="zh-cn_topic_0000002165418008_text5717204812817"><a name="zh-cn_topic_0000002165418008_text5717204812817"></a><a name="zh-cn_topic_0000002165418008_text5717204812817"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.14%" headers="mcps1.2.5.1.2 mcps1.2.5.2.2 "><p id="zh-cn_topic_0000002165418008_p13998205142815"><a name="zh-cn_topic_0000002165418008_p13998205142815"></a><a name="zh-cn_topic_0000002165418008_p13998205142815"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 mcps1.2.5.2.3 "><p id="zh-cn_topic_0000002165418008_p49983512287"><a name="zh-cn_topic_0000002165418008_p49983512287"></a><a name="zh-cn_topic_0000002165418008_p49983512287"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p7507171012231"><a name="p7507171012231"></a><a name="p7507171012231"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002165418008_row06711416544"><td class="cellrowborder" valign="top" width="24.86%" headers="mcps1.2.5.1.1 mcps1.2.5.2.1 "><p id="zh-cn_topic_0000002165418008_p1765294655411"><a name="zh-cn_topic_0000002165418008_p1765294655411"></a><a name="zh-cn_topic_0000002165418008_p1765294655411"></a><span id="zh-cn_topic_0000002165418008_text19652946185414"><a name="zh-cn_topic_0000002165418008_text19652946185414"></a><a name="zh-cn_topic_0000002165418008_text19652946185414"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25.14%" headers="mcps1.2.5.1.2 mcps1.2.5.2.2 "><p id="zh-cn_topic_0000002165418008_p428811549547"><a name="zh-cn_topic_0000002165418008_p428811549547"></a><a name="zh-cn_topic_0000002165418008_p428811549547"></a>Y（仅支持带内）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 mcps1.2.5.2.3 "><p id="zh-cn_topic_0000002165418008_p15288954155420"><a name="zh-cn_topic_0000002165418008_p15288954155420"></a><a name="zh-cn_topic_0000002165418008_p15288954155420"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002165418008_p192881754115412"><a name="zh-cn_topic_0000002165418008_p192881754115412"></a><a name="zh-cn_topic_0000002165418008_p192881754115412"></a>N</p>
</td>
</tr>
<tr id="row191917542215"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.2.1 mcps1.2.5.2.2 mcps1.2.5.2.3 "><p id="p1550115418718"><a name="p1550115418718"></a><a name="p1550115418718"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206467196_zh-cn_topic_0000001177894694_zh-cn_topic_0000001099300038_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int channel_state = 0;
enum dcmi_reset_channel channel_type = OUTBAND_CHANNEL;
ret = dcmi_get_device_outband_channel_state(card_id, device_id, &channel_state);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
ret = dcmi_pre_reset_soc(card_id, device_id);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
ret = dcmi_set_device_reset(card_id, device_id, channel_type);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

