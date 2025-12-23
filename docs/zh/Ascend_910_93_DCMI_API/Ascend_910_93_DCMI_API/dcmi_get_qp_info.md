# dcmi\_get\_qp\_info<a name="ZH-CN_TOPIC_0000002485318778"></a>

**函数原型<a name="section1478319523594"></a>**

**int dcmi\_get\_qp\_info \(int card\_id, int device\_id, int port\_id, unsigned int qpn, struct dcmi\_qp\_info \*qp\_info\)**

**功能说明<a name="section1784175216599"></a>**

根据qpn获取qp context信息。

**参数说明<a name="section12785135213593"></a>**

<a name="table1782512522592"></a>
<table><thead align="left"><tr id="row18777115310597"><th class="cellrowborder" valign="top" width="21.04210421042104%" id="mcps1.1.5.1.1"><p id="p1277720536595"><a name="p1277720536595"></a><a name="p1277720536595"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="13.701370137013702%" id="mcps1.1.5.1.2"><p id="p97773536593"><a name="p97773536593"></a><a name="p97773536593"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="26.772677267726774%" id="mcps1.1.5.1.3"><p id="p107778533599"><a name="p107778533599"></a><a name="p107778533599"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="38.48384838483848%" id="mcps1.1.5.1.4"><p id="p7777185345918"><a name="p7777185345918"></a><a name="p7777185345918"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row191479201919"><td class="cellrowborder" valign="top" width="21.04210421042104%" headers="mcps1.1.5.1.1 "><p id="p831314310448"><a name="p831314310448"></a><a name="p831314310448"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.701370137013702%" headers="mcps1.1.5.1.2 "><p id="p7313183184415"><a name="p7313183184415"></a><a name="p7313183184415"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="26.772677267726774%" headers="mcps1.1.5.1.3 "><p id="p20313333443"><a name="p20313333443"></a><a name="p20313333443"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="38.48384838483848%" headers="mcps1.1.5.1.4 "><p id="p63137316446"><a name="p63137316446"></a><a name="p63137316446"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1277725315914"><td class="cellrowborder" valign="top" width="21.04210421042104%" headers="mcps1.1.5.1.1 "><p id="p15777135325912"><a name="p15777135325912"></a><a name="p15777135325912"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.701370137013702%" headers="mcps1.1.5.1.2 "><p id="p14777135375917"><a name="p14777135375917"></a><a name="p14777135375917"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="26.772677267726774%" headers="mcps1.1.5.1.3 "><p id="p4777153135914"><a name="p4777153135914"></a><a name="p4777153135914"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="38.48384838483848%" headers="mcps1.1.5.1.4 "><p id="p167774534592"><a name="p167774534592"></a><a name="p167774534592"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p15777353135919"><a name="p15777353135919"></a><a name="p15777353135919"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p759731172012"><a name="p759731172012"></a><a name="p759731172012"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row16777145355910"><td class="cellrowborder" valign="top" width="21.04210421042104%" headers="mcps1.1.5.1.1 "><p id="p377719536592"><a name="p377719536592"></a><a name="p377719536592"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.701370137013702%" headers="mcps1.1.5.1.2 "><p id="p777745375915"><a name="p777745375915"></a><a name="p777745375915"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="26.772677267726774%" headers="mcps1.1.5.1.3 "><p id="p6778115313599"><a name="p6778115313599"></a><a name="p6778115313599"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="38.48384838483848%" headers="mcps1.1.5.1.4 "><p id="p67471340155912"><a name="p67471340155912"></a><a name="p67471340155912"></a>NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="row977805317595"><td class="cellrowborder" valign="top" width="21.04210421042104%" headers="mcps1.1.5.1.1 "><p id="p9397624205320"><a name="p9397624205320"></a><a name="p9397624205320"></a>qpn</p>
</td>
<td class="cellrowborder" valign="top" width="13.701370137013702%" headers="mcps1.1.5.1.2 "><p id="p16398102495311"><a name="p16398102495311"></a><a name="p16398102495311"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="26.772677267726774%" headers="mcps1.1.5.1.3 "><p id="p103981242531"><a name="p103981242531"></a><a name="p103981242531"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="38.48384838483848%" headers="mcps1.1.5.1.4 "><p id="p53985249531"><a name="p53985249531"></a><a name="p53985249531"></a>qp number</p>
</td>
</tr>
<tr id="row277885355910"><td class="cellrowborder" valign="top" width="21.04210421042104%" headers="mcps1.1.5.1.1 "><p id="p14398162445314"><a name="p14398162445314"></a><a name="p14398162445314"></a>qp_info</p>
</td>
<td class="cellrowborder" valign="top" width="13.701370137013702%" headers="mcps1.1.5.1.2 "><p id="p15398624115315"><a name="p15398624115315"></a><a name="p15398624115315"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="26.772677267726774%" headers="mcps1.1.5.1.3 "><p id="p173983244533"><a name="p173983244533"></a><a name="p173983244533"></a>struct dcmi_qp_info*</p>
</td>
<td class="cellrowborder" valign="top" width="38.48384838483848%" headers="mcps1.1.5.1.4 "><p id="p33981024185315"><a name="p33981024185315"></a><a name="p33981024185315"></a>查询到的qp信息</p>
<p id="p13985242533"><a name="p13985242533"></a><a name="p13985242533"></a>#define IP_ADDRESS_LEN   40</p>
<p id="p1639862410533"><a name="p1639862410533"></a><a name="p1639862410533"></a>struct dcmi_qp_info {</p>
<p id="p7398624185310"><a name="p7398624185310"></a><a name="p7398624185310"></a>unsigned char status;  // qp状态</p>
<p id="p1739872412536"><a name="p1739872412536"></a><a name="p1739872412536"></a>unsigned char type; // qp类型</p>
<p id="p1339817247535"><a name="p1339817247535"></a><a name="p1339817247535"></a>char ip[IP_ADDRESS_LEN];// 目标ip地址</p>
<p id="p939872405317"><a name="p939872405317"></a><a name="p939872405317"></a>unsigned short src_port; // 源端口</p>
<p id="p239882475318"><a name="p239882475318"></a><a name="p239882475318"></a>unsigned int src_qpn; // 源qp号</p>
<p id="p7398192418533"><a name="p7398192418533"></a><a name="p7398192418533"></a>unsigned int dst_qpn; // 目标qp号</p>
<p id="p539812241533"><a name="p539812241533"></a><a name="p539812241533"></a>unsigned int send_psn;  // 发送的psn</p>
<p id="p639812414531"><a name="p639812414531"></a><a name="p639812414531"></a>unsigned int recv_psn; // 接收的psn</p>
<p id="p3398112419537"><a name="p3398112419537"></a><a name="p3398112419537"></a>char reserved[32]; // 保留字段</p>
<p id="p1839815243538"><a name="p1839815243538"></a><a name="p1839815243538"></a>};</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section11804352115915"></a>**

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

**异常处理<a name="section1381515275916"></a>**

无。

**约束说明<a name="section4816185215596"></a>**

该接口支持在物理机+特权容器场景下使用。

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

**调用示例<a name="section78173521596"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
unsigned int qpn = 0;
struct dcmi_qp_info qp_info = {0};
ret = dcmi_get_qp_info(card_id, device_id, port_id, qpn, &qp_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

