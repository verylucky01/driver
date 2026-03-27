# dcmi\_get\_capability\_group\_aicore\_usage<a name="ZH-CN_TOPIC_0000002485318772"></a>

**函数原型<a name="section19627143719212"></a>**

**int dcmi\_get\_capability\_group\_aicore\_usage\(int card\_id, int device\_id, int group\_id, int \*rate\)**

**功能说明<a name="section862833752111"></a>**

获取昇腾虚拟化实例的aicore利用率。

**参数说明<a name="section162843720213"></a>**

<a name="table17640173772119"></a>
<table><thead align="left"><tr id="row1468003711211"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p1168053719214"><a name="p1168053719214"></a><a name="p1168053719214"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p568020371216"><a name="p568020371216"></a><a name="p568020371216"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.16%" id="mcps1.1.5.1.3"><p id="p468053717213"><a name="p468053717213"></a><a name="p468053717213"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.519999999999996%" id="mcps1.1.5.1.4"><p id="p968053715217"><a name="p968053715217"></a><a name="p968053715217"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row136801137162117"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p768016375215"><a name="p768016375215"></a><a name="p768016375215"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p1868010371212"><a name="p1868010371212"></a><a name="p1868010371212"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.16%" headers="mcps1.1.5.1.3 "><p id="p156808377219"><a name="p156808377219"></a><a name="p156808377219"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p1968063714214"><a name="p1968063714214"></a><a name="p1968063714214"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row8680437132113"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p268063720219"><a name="p268063720219"></a><a name="p268063720219"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p368013718215"><a name="p368013718215"></a><a name="p368013718215"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.16%" headers="mcps1.1.5.1.3 "><p id="p14680163718212"><a name="p14680163718212"></a><a name="p14680163718212"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p468043772111"><a name="p468043772111"></a><a name="p468043772111"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p068043752110"><a name="p068043752110"></a><a name="p068043752110"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p892871072319"><a name="p892871072319"></a><a name="p892871072319"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row568010377211"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p4680937152110"><a name="p4680937152110"></a><a name="p4680937152110"></a>group_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p196801737122111"><a name="p196801737122111"></a><a name="p196801737122111"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.16%" headers="mcps1.1.5.1.3 "><p id="p19680537112114"><a name="p19680537112114"></a><a name="p19680537112114"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p126808378217"><a name="p126808378217"></a><a name="p126808378217"></a>算力组ID</p>
<p id="p1468003712215"><a name="p1468003712215"></a><a name="p1468003712215"></a>范围：[0, 3]。</p>
</td>
</tr>
<tr id="row1668053702114"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p176802037182116"><a name="p176802037182116"></a><a name="p176802037182116"></a>rate</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p106802373218"><a name="p106802373218"></a><a name="p106802373218"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.16%" headers="mcps1.1.5.1.3 "><p id="p568015374212"><a name="p568015374212"></a><a name="p568015374212"></a>int*</p>
</td>
<td class="cellrowborder" valign="top" width="50.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p568043722120"><a name="p568043722120"></a><a name="p568043722120"></a>aicore利用率。</p>
<p id="p76811837202112"><a name="p76811837202112"></a><a name="p76811837202112"></a>当前group的aicore_number为0，aicore利用率为0，无实际意义。</p>
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

**约束说明<a name="section17636163742116"></a>**

仅Control CPU开放形态使用该接口。

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

**调用示例<a name="section56391937122113"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int group_id = 0;
int rate = 0;
ret = dcmi_get_capability_group_aicore_usage(card_id, device_id, group_id, &rate);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

