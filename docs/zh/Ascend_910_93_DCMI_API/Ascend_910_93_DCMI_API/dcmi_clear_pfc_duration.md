# dcmi\_clear\_pfc\_duration<a name="ZH-CN_TOPIC_0000002517638699"></a>

**函数原型<a name="section12365583546"></a>**

**int dcmi\_clear\_pfc\_duration\(int card\_id, int device\_id\)**

**功能说明<a name="section142361658175418"></a>**

清除指定设备的PFC反压帧持续时间统计值。

**参数说明<a name="section32371758125411"></a>**

<a name="table2274145813545"></a>
<table><thead align="left"><tr id="row106575945418"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p665759125418"><a name="p665759125418"></a><a name="p665759125418"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p10659593544"><a name="p10659593544"></a><a name="p10659593544"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="p1265259155414"><a name="p1265259155414"></a><a name="p1265259155414"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p196611598540"><a name="p196611598540"></a><a name="p196611598540"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row106685915545"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p76613598548"><a name="p76613598548"></a><a name="p76613598548"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p196625985410"><a name="p196625985410"></a><a name="p196625985410"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p4661459185416"><a name="p4661459185416"></a><a name="p4661459185416"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p8662594544"><a name="p8662594544"></a><a name="p8662594544"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1662595540"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p16610593540"><a name="p16610593540"></a><a name="p16610593540"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p06675913543"><a name="p06675913543"></a><a name="p06675913543"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p1666159175411"><a name="p1666159175411"></a><a name="p1666159175411"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p176616594547"><a name="p176616594547"></a><a name="p176616594547"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p2066359185415"><a name="p2066359185415"></a><a name="p2066359185415"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p43834568196"><a name="p43834568196"></a><a name="p43834568196"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section124813589544"></a>**

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

**异常处理<a name="section1225565895419"></a>**

无。

**约束说明<a name="section1625614581546"></a>**

该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table4146152515545"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row2051415544912"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p5723115216395"><a name="zh-cn_topic_0000002485318818_p5723115216395"></a><a name="zh-cn_topic_0000002485318818_p5723115216395"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p67235521393"><a name="zh-cn_topic_0000002485318818_p67235521393"></a><a name="zh-cn_topic_0000002485318818_p67235521393"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p1072317520390"><a name="zh-cn_topic_0000002485318818_p1072317520390"></a><a name="zh-cn_topic_0000002485318818_p1072317520390"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p272335243919"><a name="zh-cn_topic_0000002485318818_p272335243919"></a><a name="zh-cn_topic_0000002485318818_p272335243919"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row135148510490"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p11514155490"><a name="zh-cn_topic_0000002485318818_p11514155490"></a><a name="zh-cn_topic_0000002485318818_p11514155490"></a><span id="zh-cn_topic_0000002485318818_text551465114917"><a name="zh-cn_topic_0000002485318818_text551465114917"></a><a name="zh-cn_topic_0000002485318818_text551465114917"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p185143564913"><a name="zh-cn_topic_0000002485318818_p185143564913"></a><a name="zh-cn_topic_0000002485318818_p185143564913"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p185145519497"><a name="zh-cn_topic_0000002485318818_p185145519497"></a><a name="zh-cn_topic_0000002485318818_p185145519497"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1751413513498"><a name="zh-cn_topic_0000002485318818_p1751413513498"></a><a name="zh-cn_topic_0000002485318818_p1751413513498"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row135141553491"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1351411524913"><a name="zh-cn_topic_0000002485318818_p1351411524913"></a><a name="zh-cn_topic_0000002485318818_p1351411524913"></a><span id="zh-cn_topic_0000002485318818_text175141751499"><a name="zh-cn_topic_0000002485318818_text175141751499"></a><a name="zh-cn_topic_0000002485318818_text175141751499"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1034521615495"><a name="zh-cn_topic_0000002485318818_p1034521615495"></a><a name="zh-cn_topic_0000002485318818_p1034521615495"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p6514145154913"><a name="zh-cn_topic_0000002485318818_p6514145154913"></a><a name="zh-cn_topic_0000002485318818_p6514145154913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p17514145184914"><a name="zh-cn_topic_0000002485318818_p17514145184914"></a><a name="zh-cn_topic_0000002485318818_p17514145184914"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row183721515155518"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p923812139165"><a name="zh-cn_topic_0000002485318818_p923812139165"></a><a name="zh-cn_topic_0000002485318818_p923812139165"></a><span id="zh-cn_topic_0000002485318818_text8238111381610"><a name="zh-cn_topic_0000002485318818_text8238111381610"></a><a name="zh-cn_topic_0000002485318818_text8238111381610"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1858019263555"><a name="zh-cn_topic_0000002485318818_p1858019263555"></a><a name="zh-cn_topic_0000002485318818_p1858019263555"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p125801926195517"><a name="zh-cn_topic_0000002485318818_p125801926195517"></a><a name="zh-cn_topic_0000002485318818_p125801926195517"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p125808265554"><a name="zh-cn_topic_0000002485318818_p125808265554"></a><a name="zh-cn_topic_0000002485318818_p125808265554"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1828812107299"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p4288010192914"><a name="zh-cn_topic_0000002485318818_p4288010192914"></a><a name="zh-cn_topic_0000002485318818_p4288010192914"></a><span id="zh-cn_topic_0000002485318818_text17252201592911"><a name="zh-cn_topic_0000002485318818_text17252201592911"></a><a name="zh-cn_topic_0000002485318818_text17252201592911"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p14729171810297"><a name="zh-cn_topic_0000002485318818_p14729171810297"></a><a name="zh-cn_topic_0000002485318818_p14729171810297"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p187294185296"><a name="zh-cn_topic_0000002485318818_p187294185296"></a><a name="zh-cn_topic_0000002485318818_p187294185296"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p12729918182916"><a name="zh-cn_topic_0000002485318818_p12729918182916"></a><a name="zh-cn_topic_0000002485318818_p12729918182916"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row8768181557"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1563212213554"><a name="zh-cn_topic_0000002485318818_p1563212213554"></a><a name="zh-cn_topic_0000002485318818_p1563212213554"></a><span id="zh-cn_topic_0000002485318818_text1963217219554"><a name="zh-cn_topic_0000002485318818_text1963217219554"></a><a name="zh-cn_topic_0000002485318818_text1963217219554"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p18671727165520"><a name="zh-cn_topic_0000002485318818_p18671727165520"></a><a name="zh-cn_topic_0000002485318818_p18671727165520"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p967152775514"><a name="zh-cn_topic_0000002485318818_p967152775514"></a><a name="zh-cn_topic_0000002485318818_p967152775514"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p667192712555"><a name="zh-cn_topic_0000002485318818_p667192712555"></a><a name="zh-cn_topic_0000002485318818_p667192712555"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row207580351817"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1339643920818"><a name="zh-cn_topic_0000002485318818_p1339643920818"></a><a name="zh-cn_topic_0000002485318818_p1339643920818"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section192671558125418"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
ret = dcmi_clear_pfc_duration(card_id, device_id);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

