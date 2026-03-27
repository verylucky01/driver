# dcmi\_get\_affinity\_cpu\_info\_by\_device\_id<a name="ZH-CN_TOPIC_0000002485318804"></a>

**函数原型<a name="section0487181861417"></a>**

**int dcmi\_get\_affinity\_cpu\_info\_by\_device\_id\(int card\_id, int device\_id, char \*affinity\_cpu, int \*length\)**

**功能说明<a name="section248711861419"></a>**

查询指定NPU的CPU亲和性。

**参数说明<a name="section448711891417"></a>**

<a name="table348771812142"></a>
<table><thead align="left"><tr id="row1487918101413"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="p1048720181148"><a name="p1048720181148"></a><a name="p1048720181148"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.2"><p id="p11487151841419"><a name="p11487151841419"></a><a name="p11487151841419"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.5.1.3"><p id="p184876188146"><a name="p184876188146"></a><a name="p184876188146"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="p34881318151413"><a name="p34881318151413"></a><a name="p34881318151413"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row34882018191416"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p14488918161410"><a name="p14488918161410"></a><a name="p14488918161410"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p164889188148"><a name="p164889188148"></a><a name="p164889188148"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p10488151821418"><a name="p10488151821418"></a><a name="p10488151821418"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1148821831412"><a name="p1148821831412"></a><a name="p1148821831412"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1748881815146"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p74884186146"><a name="p74884186146"></a><a name="p74884186146"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p54881818151413"><a name="p54881818151413"></a><a name="p54881818151413"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p2488131815149"><a name="p2488131815149"></a><a name="p2488131815149"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p0488191813149"><a name="p0488191813149"></a><a name="p0488191813149"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p450968185915"><a name="p450968185915"></a><a name="p450968185915"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p138232054141812"><a name="p138232054141812"></a><a name="p138232054141812"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row048812180148"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p18489131814145"><a name="p18489131814145"></a><a name="p18489131814145"></a><span>affinity_cpu</span></p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p16489141813146"><a name="p16489141813146"></a><a name="p16489141813146"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p1148912180142"><a name="p1148912180142"></a><a name="p1148912180142"></a>char *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1048951841415"><a name="p1048951841415"></a><a name="p1048951841415"></a>获取指定NPU的CPU亲和性。</p>
</td>
</tr>
<tr id="row10489318191418"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p16489718161414"><a name="p16489718161414"></a><a name="p16489718161414"></a><span>length</span></p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p0489818161416"><a name="p0489818161416"></a><a name="p0489818161416"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p14489111811420"><a name="p14489111811420"></a><a name="p14489111811420"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1848911801411"><a name="p1848911801411"></a><a name="p1848911801411"></a>输出亲和性字符串的长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

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

**异常处理<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_toc533412081"></a>**

无。

**约束说明<a name="section18490181816141"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row20271184815710"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p105851457103916"><a name="zh-cn_topic_0000002485318818_p105851457103916"></a><a name="zh-cn_topic_0000002485318818_p105851457103916"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p1258655715399"><a name="zh-cn_topic_0000002485318818_p1258655715399"></a><a name="zh-cn_topic_0000002485318818_p1258655715399"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p55865575397"><a name="zh-cn_topic_0000002485318818_p55865575397"></a><a name="zh-cn_topic_0000002485318818_p55865575397"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p958625719394"><a name="zh-cn_topic_0000002485318818_p958625719394"></a><a name="zh-cn_topic_0000002485318818_p958625719394"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row1527104875720"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p13271248185720"><a name="zh-cn_topic_0000002485318818_p13271248185720"></a><a name="zh-cn_topic_0000002485318818_p13271248185720"></a><span id="zh-cn_topic_0000002485318818_text1527154812578"><a name="zh-cn_topic_0000002485318818_text1527154812578"></a><a name="zh-cn_topic_0000002485318818_text1527154812578"></a><span id="zh-cn_topic_0000002485318818_text2271148205710"><a name="zh-cn_topic_0000002485318818_text2271148205710"></a><a name="zh-cn_topic_0000002485318818_text2271148205710"></a>Atlas 900 A3 SuperPoD 超节点</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p10271248165717"><a name="zh-cn_topic_0000002485318818_p10271248165717"></a><a name="zh-cn_topic_0000002485318818_p10271248165717"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p11271348175719"><a name="zh-cn_topic_0000002485318818_p11271348175719"></a><a name="zh-cn_topic_0000002485318818_p11271348175719"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p6271144865712"><a name="zh-cn_topic_0000002485318818_p6271144865712"></a><a name="zh-cn_topic_0000002485318818_p6271144865712"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row527124819571"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p42711448205716"><a name="zh-cn_topic_0000002485318818_p42711448205716"></a><a name="zh-cn_topic_0000002485318818_p42711448205716"></a><span id="zh-cn_topic_0000002485318818_text8271134845715"><a name="zh-cn_topic_0000002485318818_text8271134845715"></a><a name="zh-cn_topic_0000002485318818_text8271134845715"></a><span id="zh-cn_topic_0000002485318818_text527164811575"><a name="zh-cn_topic_0000002485318818_text527164811575"></a><a name="zh-cn_topic_0000002485318818_text527164811575"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p12271164818576"><a name="zh-cn_topic_0000002485318818_p12271164818576"></a><a name="zh-cn_topic_0000002485318818_p12271164818576"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p427134816570"><a name="zh-cn_topic_0000002485318818_p427134816570"></a><a name="zh-cn_topic_0000002485318818_p427134816570"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p627114835717"><a name="zh-cn_topic_0000002485318818_p627114835717"></a><a name="zh-cn_topic_0000002485318818_p627114835717"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row18271184811578"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p79671124131618"><a name="zh-cn_topic_0000002485318818_p79671124131618"></a><a name="zh-cn_topic_0000002485318818_p79671124131618"></a><span id="zh-cn_topic_0000002485318818_text49671624141613"><a name="zh-cn_topic_0000002485318818_text49671624141613"></a><a name="zh-cn_topic_0000002485318818_text49671624141613"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p9271134885710"><a name="zh-cn_topic_0000002485318818_p9271134885710"></a><a name="zh-cn_topic_0000002485318818_p9271134885710"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p7271104814571"><a name="zh-cn_topic_0000002485318818_p7271104814571"></a><a name="zh-cn_topic_0000002485318818_p7271104814571"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p8271048185714"><a name="zh-cn_topic_0000002485318818_p8271048185714"></a><a name="zh-cn_topic_0000002485318818_p8271048185714"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row285204910295"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p15986185513296"><a name="zh-cn_topic_0000002485318818_p15986185513296"></a><a name="zh-cn_topic_0000002485318818_p15986185513296"></a><span id="zh-cn_topic_0000002485318818_text098685572912"><a name="zh-cn_topic_0000002485318818_text098685572912"></a><a name="zh-cn_topic_0000002485318818_text098685572912"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p105251859192911"><a name="zh-cn_topic_0000002485318818_p105251859192911"></a><a name="zh-cn_topic_0000002485318818_p105251859192911"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p7525145902913"><a name="zh-cn_topic_0000002485318818_p7525145902913"></a><a name="zh-cn_topic_0000002485318818_p7525145902913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p0525165962917"><a name="zh-cn_topic_0000002485318818_p0525165962917"></a><a name="zh-cn_topic_0000002485318818_p0525165962917"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row14271848145719"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p527118483579"><a name="zh-cn_topic_0000002485318818_p527118483579"></a><a name="zh-cn_topic_0000002485318818_p527118483579"></a><span id="zh-cn_topic_0000002485318818_text15271174815713"><a name="zh-cn_topic_0000002485318818_text15271174815713"></a><a name="zh-cn_topic_0000002485318818_text15271174815713"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p16271194813571"><a name="zh-cn_topic_0000002485318818_p16271194813571"></a><a name="zh-cn_topic_0000002485318818_p16271194813571"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1827154818570"><a name="zh-cn_topic_0000002485318818_p1827154818570"></a><a name="zh-cn_topic_0000002485318818_p1827154818570"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p72711448185716"><a name="zh-cn_topic_0000002485318818_p72711448185716"></a><a name="zh-cn_topic_0000002485318818_p72711448185716"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row886601693"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p17525251914"><a name="zh-cn_topic_0000002485318818_p17525251914"></a><a name="zh-cn_topic_0000002485318818_p17525251914"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section5491121812143"></a>**

```
…
int ret;
int card_id = 0;
int device_id = 0;
char affinity_cpu[TOPO_INFO_MAX_LENTH] ={0};
int length;
ret = dcmi_get_affinity_cpu_info_by_device_id(card_id, device_id, affinity_cpu, &length);
…
```

