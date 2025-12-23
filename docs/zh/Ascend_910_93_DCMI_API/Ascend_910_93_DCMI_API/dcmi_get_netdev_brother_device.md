# dcmi\_get\_netdev\_brother\_device<a name="ZH-CN_TOPIC_0000002517638671"></a>

**函数原型<a name="section1270910248495"></a>**

**int dcmi\_get\_netdev\_brother\_device\(int card\_id, int device\_id, int \*brother\_card\_id\)**

**功能说明<a name="section270992444920"></a>**

获取当前NPU模组中有网口互助关系的NPU模组。

**参数说明<a name="section2071012242493"></a>**

<a name="table3721102412494"></a>
<table><thead align="left"><tr id="row127491124114913"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p67494249498"><a name="p67494249498"></a><a name="p67494249498"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p37491124164919"><a name="p37491124164919"></a><a name="p37491124164919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="p117491224194917"><a name="p117491224194917"></a><a name="p117491224194917"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p127491124144914"><a name="p127491124144914"></a><a name="p127491124144914"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row12749132418498"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p107491624154917"><a name="p107491624154917"></a><a name="p107491624154917"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p12749122414916"><a name="p12749122414916"></a><a name="p12749122414916"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p6749162494917"><a name="p6749162494917"></a><a name="p6749162494917"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p57498247497"><a name="p57498247497"></a><a name="p57498247497"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row87494247496"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p10749142424913"><a name="p10749142424913"></a><a name="p10749142424913"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p117498243494"><a name="p117498243494"></a><a name="p117498243494"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p974912245493"><a name="p974912245493"></a><a name="p974912245493"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p1974915243493"><a name="p1974915243493"></a><a name="p1974915243493"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p5749132411491"><a name="p5749132411491"></a><a name="p5749132411491"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1352175782015"><a name="p1352175782015"></a><a name="p1352175782015"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row2749024144914"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p574912247499"><a name="p574912247499"></a><a name="p574912247499"></a>brother_card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p17749182413495"><a name="p17749182413495"></a><a name="p17749182413495"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p1874952414920"><a name="p1874952414920"></a><a name="p1874952414920"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p574992424916"><a name="p574992424916"></a><a name="p574992424916"></a>获取指定NPU模组对应的有网口互助关系的NPU模组。</p>
<p id="p85426599203"><a name="p85426599203"></a><a name="p85426599203"></a></p>
<div class="note" id="note1112231016819"><a name="note1112231016819"></a><a name="note1112231016819"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p137832403444"><a name="p137832403444"></a><a name="p137832403444"></a><span id="text1480012462513"><a name="text1480012462513"></a><a name="text1480012462513"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span>没有具备网口互助关系的NPU模组，返回值为-1。</p>
</div></div>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section2071417249494"></a>**

<a name="zh-cn_topic_0000002517558717_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**异常处理<a name="section67171124154919"></a>**

无

**约束说明<a name="section771717243498"></a>**

该接口不支持在所有NPU模组丢失或不存在的场景下使用。

该接口支持在特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table7724192414490"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row913516862011"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p15076476392"><a name="zh-cn_topic_0000002485318818_p15076476392"></a><a name="zh-cn_topic_0000002485318818_p15076476392"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p16507547153913"><a name="zh-cn_topic_0000002485318818_p16507547153913"></a><a name="zh-cn_topic_0000002485318818_p16507547153913"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p1950718474398"><a name="zh-cn_topic_0000002485318818_p1950718474398"></a><a name="zh-cn_topic_0000002485318818_p1950718474398"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p165071047113911"><a name="zh-cn_topic_0000002485318818_p165071047113911"></a><a name="zh-cn_topic_0000002485318818_p165071047113911"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row31351488206"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p6135185208"><a name="zh-cn_topic_0000002485318818_p6135185208"></a><a name="zh-cn_topic_0000002485318818_p6135185208"></a><span id="zh-cn_topic_0000002485318818_text1213528172016"><a name="zh-cn_topic_0000002485318818_text1213528172016"></a><a name="zh-cn_topic_0000002485318818_text1213528172016"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1213511813205"><a name="zh-cn_topic_0000002485318818_p1213511813205"></a><a name="zh-cn_topic_0000002485318818_p1213511813205"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p613528102020"><a name="zh-cn_topic_0000002485318818_p613528102020"></a><a name="zh-cn_topic_0000002485318818_p613528102020"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p713512862013"><a name="zh-cn_topic_0000002485318818_p713512862013"></a><a name="zh-cn_topic_0000002485318818_p713512862013"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1813515811208"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p71354810207"><a name="zh-cn_topic_0000002485318818_p71354810207"></a><a name="zh-cn_topic_0000002485318818_p71354810207"></a><span id="zh-cn_topic_0000002485318818_text51358812207"><a name="zh-cn_topic_0000002485318818_text51358812207"></a><a name="zh-cn_topic_0000002485318818_text51358812207"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p213568152019"><a name="zh-cn_topic_0000002485318818_p213568152019"></a><a name="zh-cn_topic_0000002485318818_p213568152019"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1613548142019"><a name="zh-cn_topic_0000002485318818_p1613548142019"></a><a name="zh-cn_topic_0000002485318818_p1613548142019"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p11351087209"><a name="zh-cn_topic_0000002485318818_p11351087209"></a><a name="zh-cn_topic_0000002485318818_p11351087209"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row87611016195410"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p8296259161515"><a name="zh-cn_topic_0000002485318818_p8296259161515"></a><a name="zh-cn_topic_0000002485318818_p8296259161515"></a><span id="zh-cn_topic_0000002485318818_text14296259131514"><a name="zh-cn_topic_0000002485318818_text14296259131514"></a><a name="zh-cn_topic_0000002485318818_text14296259131514"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1451920314542"><a name="zh-cn_topic_0000002485318818_p1451920314542"></a><a name="zh-cn_topic_0000002485318818_p1451920314542"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p2519133155413"><a name="zh-cn_topic_0000002485318818_p2519133155413"></a><a name="zh-cn_topic_0000002485318818_p2519133155413"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p11519153110545"><a name="zh-cn_topic_0000002485318818_p11519153110545"></a><a name="zh-cn_topic_0000002485318818_p11519153110545"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row0323633152810"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p064011372281"><a name="zh-cn_topic_0000002485318818_p064011372281"></a><a name="zh-cn_topic_0000002485318818_p064011372281"></a><span id="zh-cn_topic_0000002485318818_text18640537162811"><a name="zh-cn_topic_0000002485318818_text18640537162811"></a><a name="zh-cn_topic_0000002485318818_text18640537162811"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p279104012287"><a name="zh-cn_topic_0000002485318818_p279104012287"></a><a name="zh-cn_topic_0000002485318818_p279104012287"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p779112409289"><a name="zh-cn_topic_0000002485318818_p779112409289"></a><a name="zh-cn_topic_0000002485318818_p779112409289"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1791140112816"><a name="zh-cn_topic_0000002485318818_p1791140112816"></a><a name="zh-cn_topic_0000002485318818_p1791140112816"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row49081418125411"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p512952510545"><a name="zh-cn_topic_0000002485318818_p512952510545"></a><a name="zh-cn_topic_0000002485318818_p512952510545"></a><span id="zh-cn_topic_0000002485318818_text012942512545"><a name="zh-cn_topic_0000002485318818_text012942512545"></a><a name="zh-cn_topic_0000002485318818_text012942512545"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1260932135413"><a name="zh-cn_topic_0000002485318818_p1260932135413"></a><a name="zh-cn_topic_0000002485318818_p1260932135413"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1660133285419"><a name="zh-cn_topic_0000002485318818_p1660133285419"></a><a name="zh-cn_topic_0000002485318818_p1660133285419"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p46017321545"><a name="zh-cn_topic_0000002485318818_p46017321545"></a><a name="zh-cn_topic_0000002485318818_p46017321545"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1625111184"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1386118141085"><a name="zh-cn_topic_0000002485318818_p1386118141085"></a><a name="zh-cn_topic_0000002485318818_p1386118141085"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section9721024124910"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int brother_card_id = 0;
ret= dcmi_init();
if (ret != 0) {
    //todo：记录日志
    return ret;
}
ret = dcmi_get_netdev_brother_device (card_id, device_id, &brother_card_id);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

