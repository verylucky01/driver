# dcmi\_create\_capability\_group<a name="ZH-CN_TOPIC_0000002485318710"></a>

**函数原型<a name="section5631123462011"></a>**

**int dcmi\_create\_capability\_group\(int card\_id, int device\_id, int ts\_id, struct dcmi\_capability\_group\_info \*group\_info\)**

**功能说明<a name="section19632193492013"></a>**

创建昇腾虚拟化实例配置信息。

**参数说明<a name="section9633113472012"></a>**

<a name="table766115345203"></a>
<table><thead align="left"><tr id="row197331434162017"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p20733143416202"><a name="p20733143416202"></a><a name="p20733143416202"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p2733734202019"><a name="p2733734202019"></a><a name="p2733734202019"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.150000000000002%" id="mcps1.1.5.1.3"><p id="p14733034152020"><a name="p14733034152020"></a><a name="p14733034152020"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.529999999999994%" id="mcps1.1.5.1.4"><p id="p1173393411206"><a name="p1173393411206"></a><a name="p1173393411206"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4733163412208"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p13733163432013"><a name="p13733163432013"></a><a name="p13733163432013"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p573323413208"><a name="p573323413208"></a><a name="p573323413208"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.150000000000002%" headers="mcps1.1.5.1.3 "><p id="p77331534152011"><a name="p77331534152011"></a><a name="p77331534152011"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.529999999999994%" headers="mcps1.1.5.1.4 "><p id="p073313492011"><a name="p073313492011"></a><a name="p073313492011"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row0733113462010"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p1073383411207"><a name="p1073383411207"></a><a name="p1073383411207"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p167344341203"><a name="p167344341203"></a><a name="p167344341203"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.150000000000002%" headers="mcps1.1.5.1.3 "><p id="p1173423420208"><a name="p1173423420208"></a><a name="p1173423420208"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.529999999999994%" headers="mcps1.1.5.1.4 "><p id="p9734103492015"><a name="p9734103492015"></a><a name="p9734103492015"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p0734183415202"><a name="p0734183415202"></a><a name="p0734183415202"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p68581227172214"><a name="p68581227172214"></a><a name="p68581227172214"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row1873493415205"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p18734173402017"><a name="p18734173402017"></a><a name="p18734173402017"></a>ts_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p1734143452017"><a name="p1734143452017"></a><a name="p1734143452017"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.150000000000002%" headers="mcps1.1.5.1.3 "><p id="p187341434172015"><a name="p187341434172015"></a><a name="p187341434172015"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.529999999999994%" headers="mcps1.1.5.1.4 "><p id="p9734113472015"><a name="p9734113472015"></a><a name="p9734113472015"></a>算力类型ID：</p>
<p id="p1573453410204"><a name="p1573453410204"></a><a name="p1573453410204"></a>typedef enum {</p>
<p id="p273423442020"><a name="p273423442020"></a><a name="p273423442020"></a>DCMI_TS_AICORE = 0,</p>
<p id="p1873453415201"><a name="p1873453415201"></a><a name="p1873453415201"></a>DCMI_TS_AIVECTOR,</p>
<p id="p6734834192015"><a name="p6734834192015"></a><a name="p6734834192015"></a>} DCMI_TS_ID;</p>
<p id="p13734103432012"><a name="p13734103432012"></a><a name="p13734103432012"></a>目前不支持DCMI_TS_AIVECTOR</p>
</td>
</tr>
<tr id="row973413418206"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p1873443410208"><a name="p1873443410208"></a><a name="p1873443410208"></a>group_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p073411342202"><a name="p073411342202"></a><a name="p073411342202"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.150000000000002%" headers="mcps1.1.5.1.3 "><p id="p473412343208"><a name="p473412343208"></a><a name="p473412343208"></a>struct dcmi_capability_group_info *</p>
</td>
<td class="cellrowborder" valign="top" width="50.529999999999994%" headers="mcps1.1.5.1.4 "><p id="p1873415347208"><a name="p1873415347208"></a><a name="p1873415347208"></a>算力组信息。</p>
<p id="p67342346202"><a name="p67342346202"></a><a name="p67342346202"></a>算力信息结构体为：</p>
<p id="p97347342201"><a name="p97347342201"></a><a name="p97347342201"></a>#define DCMI_COMPUTE_GROUP_INFO_RES_NUM - AICORE_MASK_NUM 8</p>
<p id="p373473415203"><a name="p373473415203"></a><a name="p373473415203"></a>#define DCMI_AICORE_MASK_NUM 2</p>
<p id="p20734143412020"><a name="p20734143412020"></a><a name="p20734143412020"></a>struct dcmi_capability_group_info {</p>
<p id="p173416342208"><a name="p173416342208"></a><a name="p173416342208"></a>unsigned int  group_id;</p>
<p id="p127345347206"><a name="p127345347206"></a><a name="p127345347206"></a>unsigned int  state;</p>
<p id="p167344343200"><a name="p167344343200"></a><a name="p167344343200"></a>unsigned int  extend_attribute;</p>
<p id="p473413402014"><a name="p473413402014"></a><a name="p473413402014"></a>unsigned int  aicore_number;</p>
<p id="p16734834152014"><a name="p16734834152014"></a><a name="p16734834152014"></a>unsigned int  aivector_number;</p>
<p id="p13734183418205"><a name="p13734183418205"></a><a name="p13734183418205"></a>unsigned int  sdma_number;</p>
<p id="p18734634152016"><a name="p18734634152016"></a><a name="p18734634152016"></a>unsigned int  aicpu_number;</p>
<p id="p167341134162016"><a name="p167341134162016"></a><a name="p167341134162016"></a>unsigned int  active_sq_number;</p>
<p id="p973413492019"><a name="p973413492019"></a><a name="p973413492019"></a>unsigned int  aicore_mask [DCMI_AICORE_MASK_NUM];</p>
<p id="p1673403411209"><a name="p1673403411209"></a><a name="p1673403411209"></a>unsigned int  res[DCMI_COMPUTE_GROUP_INFO_RES_NUM - DCMI_AICORE_MASK_NUM];</p>
<p id="p37345343205"><a name="p37345343205"></a><a name="p37345343205"></a>};</p>
<a name="ul87874572324"></a><a name="ul87874572324"></a><ul id="ul87874572324"><li>group_id表示算力组ID，全局唯一。可配置范围：0~3，创建重复失败。</li></ul>
<a name="ul11464113418334"></a><a name="ul11464113418334"></a><ul id="ul11464113418334"><li>state表示算力组创建状态。0：not created, 1: created。该字段作为输入时，无意义。</li><li>extend_attribute表示默认昇腾虚拟化实例标志，1：表示系统默认使用该昇腾虚拟化实例，其他值表示不使用该昇腾虚拟化实例为默认组，缺省值：0。</li><li>aicore_number可配置范围：0~8或255，具体参照如下说明。</li><li>aivector_number、sdma_number、aicpu_number、active_sq_number仅支持配置为255，表示配置为剩余可用值，不支持切分。</li><li>aicore_mask表示aicore的编号掩码，其中每个bit表示一个core。1：该算力组内aicore，0：非算力组内aicore。该字段作为输入时，无意义。</li><li>res表示预留参数。<div class="p" id="p144411234102218"><a name="p144411234102218"></a><a name="p144411234102218"></a><div class="note" id="note188303923512"><a name="note188303923512"></a><a name="note188303923512"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p653145383518"><a name="p653145383518"></a><a name="p653145383518"></a>group_info参数的约束说明：</p>
<a name="ul34731046183513"></a><a name="ul34731046183513"></a><ul id="ul34731046183513"><li>group_id取值范围为0~3，且每次创建时需要使用未使用过的group_id，否则将会返回失败。</li><li>当创建不同分组时，各参数变量需要满足以下条件：<a name="ul97411537203318"></a><a name="ul97411537203318"></a><ul id="ul97411537203318"><li><a name="image15114194043912"></a><a name="image15114194043912"></a><math display="block" xmlns="http://www.w3.org/1998/Math/MathML"><mrow><mrow><munder><mo>&#x2211;</mo><mrow><mi mathvariant="italic">g</mi><mi mathvariant="italic">r</mi><mi mathvariant="italic">o</mi><mi mathvariant="italic">u</mi><mi mathvariant="italic">p</mi><mtext>_</mtext><mi mathvariant="italic">i</mi><mi mathvariant="italic">d</mi></mrow></munder><mrow><mi mathvariant="italic">a</mi><mi mathvariant="italic">i</mi><mi mathvariant="italic">c</mi><mi mathvariant="italic">o</mi><mi mathvariant="italic">r</mi><mi mathvariant="italic">e</mi><mtext>_</mtext><mi mathvariant="italic">n</mi><mi mathvariant="italic">u</mi><mi mathvariant="italic">m</mi><mi mathvariant="italic">b</mi><mi mathvariant="italic">e</mi><mi mathvariant="italic">r</mi><msub><mrow></mrow><mrow><mi mathvariant="italic">g</mi><mi mathvariant="italic">r</mi><mi mathvariant="italic">o</mi><mi mathvariant="italic">u</mi><mi mathvariant="italic">p</mi><mtext>_</mtext><mi mathvariant="italic">i</mi><mi mathvariant="italic">d</mi><mtext>&#x2009;</mtext></mrow></msub></mrow></mrow><mrow><mo>&#x3C;</mo><mrow><mo>&#x3D;</mo><mrow><mi mathvariant="italic">t</mi><mi mathvariant="italic">o</mi><mi mathvariant="italic">t</mi><mi mathvariant="italic">a</mi><mi mathvariant="italic">l</mi><mtext>_</mtext><mi mathvariant="italic">a</mi><mi mathvariant="italic">i</mi><mi mathvariant="italic">c</mi><mi mathvariant="italic">o</mi><mi mathvariant="italic">r</mi><mi mathvariant="italic">e</mi><mtext>_</mtext><mi mathvariant="italic">n</mi><mi mathvariant="italic">u</mi><mi mathvariant="italic">m</mi><mi mathvariant="italic">b</mi><mi mathvariant="italic">e</mi><mi mathvariant="italic">r</mi></mrow></mrow></mrow></mrow></math></li><li>total_aicore_number为8。</li><li>当aicore_number配置为255时，表示配置为剩余可用值，如果多个group配置255，这几个group共用剩余可用值。</li></ul>
</li></ul>
</div></div>
</div>
</li></ul>
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

**约束说明<a name="section3125938142713"></a>**

-   仅Control CPU开放形态使用该接口。
-   调用该接口前，请先调用[dcmi\_get\_capability\_group\_info](dcmi_get_capability_group_info.md)查询已配置的资源信息，结合ts\_id参数的说明和group\_info参数的约束说明进行配置，否则可能出现接口调用失败。如果需要删除已有配置，可以调用[dcmi\_delete\_capability\_group](dcmi_delete_capability_group.md)接口进行删除。

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

**调用示例<a name="section206582034172018"></a>**

```
… 
int ret = 0;
int card_id = 0;
int dev_id = 0;
int ts_id = 0;
struct dcmi_capability_group_info group_info = {0};
group_info.group_id = 0;
group_info.aivector_number = 255;
group_info.sdma_number = 255;
group_info.aicpu_number = 255;
group_info.active_sq_number = 255;
group_info.aicore_number = 8;
 
ret = dcmi_create_capability_group(card_id, dev_id, ts_id, &group_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

