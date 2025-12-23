# dcmi\_get\_pfc\_duration\_info<a name="ZH-CN_TOPIC_0000002485318746"></a>

**函数原型<a name="section1873810317462"></a>**

**int dcmi\_get\_pfc\_duration\_info\(int card\_id, int device\_id, struct dcmi\_pfc\_duration\_info \*pfc\_duration\_info\)**

**功能说明<a name="section474073164613"></a>**

获取指定设备的PFC反压帧持续时间统计值。

**参数说明<a name="section167411314464"></a>**

<a name="table87791434466"></a>
<table><thead align="left"><tr id="row285620314611"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p118569324610"><a name="p118569324610"></a><a name="p118569324610"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p6856113174610"><a name="p6856113174610"></a><a name="p6856113174610"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="p38568320467"><a name="p38568320467"></a><a name="p38568320467"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p1885620311466"><a name="p1885620311466"></a><a name="p1885620311466"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row78563334615"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p1085613312465"><a name="p1085613312465"></a><a name="p1085613312465"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p685614313466"><a name="p685614313466"></a><a name="p685614313466"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p485612319469"><a name="p485612319469"></a><a name="p485612319469"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p285611384617"><a name="p285611384617"></a><a name="p285611384617"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row485615313467"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p198565334613"><a name="p198565334613"></a><a name="p198565334613"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p16856183104614"><a name="p16856183104614"></a><a name="p16856183104614"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p138563311469"><a name="p138563311469"></a><a name="p138563311469"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p885719354617"><a name="p885719354617"></a><a name="p885719354617"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p9857837462"><a name="p9857837462"></a><a name="p9857837462"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p28645526195"><a name="p28645526195"></a><a name="p28645526195"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row085714312464"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p128571338465"><a name="p128571338465"></a><a name="p128571338465"></a>pfc_duration_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p18857634463"><a name="p18857634463"></a><a name="p18857634463"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p485712364616"><a name="p485712364616"></a><a name="p485712364616"></a>struct dcmi_pfc_duration_info*</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p168570364612"><a name="p168570364612"></a><a name="p168570364612"></a>PFC pause duration值，结构体内容为：</p>
<p id="p08573364614"><a name="p08573364614"></a><a name="p08573364614"></a>unsigned long long tx[PRI_NUM];//发送的反压帧持续时间</p>
<p id="p98572334615"><a name="p98572334615"></a><a name="p98572334615"></a>unsigned long long rx[PRI_NUM];//接收的反压帧持续时间</p>
<p id="p158581136465"><a name="p158581136465"></a><a name="p158581136465"></a>PRI_NUM=8;//8条优先级队列</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section197595384611"></a>**

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

**异常处理<a name="section11766193124614"></a>**

无。

**约束说明<a name="section14766153174617"></a>**

该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table4259182819534"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row167572745119"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p11930442123919"><a name="zh-cn_topic_0000002485318818_p11930442123919"></a><a name="zh-cn_topic_0000002485318818_p11930442123919"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p12930164273918"><a name="zh-cn_topic_0000002485318818_p12930164273918"></a><a name="zh-cn_topic_0000002485318818_p12930164273918"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p11930114253913"><a name="zh-cn_topic_0000002485318818_p11930114253913"></a><a name="zh-cn_topic_0000002485318818_p11930114253913"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p1093184218399"><a name="zh-cn_topic_0000002485318818_p1093184218399"></a><a name="zh-cn_topic_0000002485318818_p1093184218399"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row375192710515"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p875927195114"><a name="zh-cn_topic_0000002485318818_p875927195114"></a><a name="zh-cn_topic_0000002485318818_p875927195114"></a><span id="zh-cn_topic_0000002485318818_text1675627145111"><a name="zh-cn_topic_0000002485318818_text1675627145111"></a><a name="zh-cn_topic_0000002485318818_text1675627145111"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p6407361513"><a name="zh-cn_topic_0000002485318818_p6407361513"></a><a name="zh-cn_topic_0000002485318818_p6407361513"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p940636125119"><a name="zh-cn_topic_0000002485318818_p940636125119"></a><a name="zh-cn_topic_0000002485318818_p940636125119"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p9696114984512"><a name="zh-cn_topic_0000002485318818_p9696114984512"></a><a name="zh-cn_topic_0000002485318818_p9696114984512"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row275102717519"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p2075132716516"><a name="zh-cn_topic_0000002485318818_p2075132716516"></a><a name="zh-cn_topic_0000002485318818_p2075132716516"></a><span id="zh-cn_topic_0000002485318818_text13751027165113"><a name="zh-cn_topic_0000002485318818_text13751027165113"></a><a name="zh-cn_topic_0000002485318818_text13751027165113"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1940103619515"><a name="zh-cn_topic_0000002485318818_p1940103619515"></a><a name="zh-cn_topic_0000002485318818_p1940103619515"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p540163695113"><a name="zh-cn_topic_0000002485318818_p540163695113"></a><a name="zh-cn_topic_0000002485318818_p540163695113"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row187731356165317"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p117351854181512"><a name="zh-cn_topic_0000002485318818_p117351854181512"></a><a name="zh-cn_topic_0000002485318818_p117351854181512"></a><span id="zh-cn_topic_0000002485318818_text2735195461519"><a name="zh-cn_topic_0000002485318818_text2735195461519"></a><a name="zh-cn_topic_0000002485318818_text2735195461519"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p634151145413"><a name="zh-cn_topic_0000002485318818_p634151145413"></a><a name="zh-cn_topic_0000002485318818_p634151145413"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p23451112546"><a name="zh-cn_topic_0000002485318818_p23451112546"></a><a name="zh-cn_topic_0000002485318818_p23451112546"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p17341711125412"><a name="zh-cn_topic_0000002485318818_p17341711125412"></a><a name="zh-cn_topic_0000002485318818_p17341711125412"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row8563102502812"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p652216261281"><a name="zh-cn_topic_0000002485318818_p652216261281"></a><a name="zh-cn_topic_0000002485318818_p652216261281"></a><span id="zh-cn_topic_0000002485318818_text1052312672813"><a name="zh-cn_topic_0000002485318818_text1052312672813"></a><a name="zh-cn_topic_0000002485318818_text1052312672813"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1831493072811"><a name="zh-cn_topic_0000002485318818_p1831493072811"></a><a name="zh-cn_topic_0000002485318818_p1831493072811"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p9314630112816"><a name="zh-cn_topic_0000002485318818_p9314630112816"></a><a name="zh-cn_topic_0000002485318818_p9314630112816"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p143141630152813"><a name="zh-cn_topic_0000002485318818_p143141630152813"></a><a name="zh-cn_topic_0000002485318818_p143141630152813"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row14552134547"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p743211475410"><a name="zh-cn_topic_0000002485318818_p743211475410"></a><a name="zh-cn_topic_0000002485318818_p743211475410"></a><span id="zh-cn_topic_0000002485318818_text243216485414"><a name="zh-cn_topic_0000002485318818_text243216485414"></a><a name="zh-cn_topic_0000002485318818_text243216485414"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p16629121175419"><a name="zh-cn_topic_0000002485318818_p16629121175419"></a><a name="zh-cn_topic_0000002485318818_p16629121175419"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1162991125418"><a name="zh-cn_topic_0000002485318818_p1162991125418"></a><a name="zh-cn_topic_0000002485318818_p1162991125418"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p8629121114549"><a name="zh-cn_topic_0000002485318818_p8629121114549"></a><a name="zh-cn_topic_0000002485318818_p8629121114549"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row14550155011712"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1550115418718"><a name="zh-cn_topic_0000002485318818_p1550115418718"></a><a name="zh-cn_topic_0000002485318818_p1550115418718"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section12776336460"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
struct dcmi_pfc_duration_info pfc_duration_info = {0};
ret = dcmi_get_pfc_duration_info(card_id, device_id, &pfc_duration_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

