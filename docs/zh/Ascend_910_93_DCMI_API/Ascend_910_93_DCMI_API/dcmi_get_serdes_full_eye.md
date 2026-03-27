# dcmi\_get\_serdes\_full\_eye<a name="ZH-CN_TOPIC_0000002485478708"></a>

**函数原型<a name="zh-cn_topic_0000001206147244_zh-cn_topic_0000001223414441_zh-cn_topic_0000001099619850_toc533412077"></a>**

**int dcmi\_get\_serdes\_full\_eye\(int card\_id, int device\_id, struct dcmi\_serdes\_full\_eye \*serdes\_full\_eye\_info\)**

**功能说明<a name="section1468480762"></a>**

查询指定NPU某个macro端口一条lane的全眼图信息，当前仅支持查询内眼垂直扫描模式下的复合眼图。

**参数说明<a name="zh-cn_topic_0000001206147244_zh-cn_topic_0000001223414441_zh-cn_topic_0000001099619850_toc533412079"></a>**

<a name="table2736627173213"></a>
<table><thead align="left"><tr id="row37511727123212"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="p3751827143219"><a name="p3751827143219"></a><a name="p3751827143219"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.2"><p id="p10751142783216"><a name="p10751142783216"></a><a name="p10751142783216"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.5.1.3"><p id="p1575112718322"><a name="p1575112718322"></a><a name="p1575112718322"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="p13751142783210"><a name="p13751142783210"></a><a name="p13751142783210"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row875132713328"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p1751927173218"><a name="p1751927173218"></a><a name="p1751927173218"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p275182733212"><a name="p275182733212"></a><a name="p275182733212"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p375112713322"><a name="p375112713322"></a><a name="p375112713322"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p12751627153211"><a name="p12751627153211"></a><a name="p12751627153211"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="row87511127163211"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p157511227143210"><a name="p157511227143210"></a><a name="p157511227143210"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p875111272328"><a name="p875111272328"></a><a name="p875111272328"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p6751162712320"><a name="p6751162712320"></a><a name="p6751162712320"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p075152718324"><a name="p075152718324"></a><a name="p075152718324"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p875116277323"><a name="p875116277323"></a><a name="p875116277323"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p8232151011213"><a name="p8232151011213"></a><a name="p8232151011213"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row7751112703217"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p1434513331784"><a name="p1434513331784"></a><a name="p1434513331784"></a>serdes_full_eye_info</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.2 "><p id="p334593312811"><a name="p334593312811"></a><a name="p334593312811"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.5.1.3 "><p id="p15345143311818"><a name="p15345143311818"></a><a name="p15345143311818"></a>dcmi_serdes_full_eye *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p126724643516"><a name="p126724643516"></a><a name="p126724643516"></a>查询全眼图信息的结构体。该全眼图为内眼垂直扫描模式下的复合眼图。</p>
<p id="p15151250141116"><a name="p15151250141116"></a><a name="p15151250141116"></a>每个lane的全眼图数据最大包含256组数据点，每组数据里包含偏移量和偏移量对应的两个坐标数据。</p>
<p id="p1969445582"><a name="p1969445582"></a><a name="p1969445582"></a>相关结构体定义如下</p>
<p id="p1982514210276"><a name="p1982514210276"></a><a name="p1982514210276"></a>#define SERDES_FULL_EYE_INFO_NUM 256</p>
<p id="p882554272718"><a name="p882554272718"></a><a name="p882554272718"></a>#define SERDES_FULL_EYE_RESERVED_LEN 8</p>
<p id="p182564252713"><a name="p182564252713"></a><a name="p182564252713"></a>struct dcmi_serdes_full_eye_base {</p>
<p id="p2825742192718"><a name="p2825742192718"></a><a name="p2825742192718"></a>int offset; // 偏移量</p>
<p id="p18825242182719"><a name="p18825242182719"></a><a name="p18825242182719"></a>int eye_diagram_0; // 该偏移量对应的坐标数据0</p>
<p id="p1682524214277"><a name="p1682524214277"></a><a name="p1682524214277"></a>int eye_diagram_1; // 该偏移量对应的坐标数据1</p>
<p id="p15825104262715"><a name="p15825104262715"></a><a name="p15825104262715"></a>};</p>
<p id="p1825742132711"><a name="p1825742132711"></a><a name="p1825742132711"></a>typedef struct dcmi_serdes_full_eye {</p>
<p id="p1082512424277"><a name="p1082512424277"></a><a name="p1082512424277"></a>unsigned int macro_id; // 需查询的macro端口号，仅支持h32 macro端口，取值为9或10（仅A200T A3 Box8 超节点服务器支持）</p>
<p id="p1882514423271"><a name="p1882514423271"></a><a name="p1882514423271"></a>unsigned int lane_id; // 需查询的lane</p>
<p id="p15825342112713"><a name="p15825342112713"></a><a name="p15825342112713"></a>unsigned int reserved[SERDES_FULL_EYE_RESERVED_LEN]; // 预留字段</p>
<p id="p98251942182712"><a name="p98251942182712"></a><a name="p98251942182712"></a>unsigned int info_size; // 返回的全眼图结果数据长度</p>
<p id="p15825104219279"><a name="p15825104219279"></a><a name="p15825104219279"></a>struct dcmi_serdes_full_eye_base serdes_full_eye[SERDES_FULL_EYE_INFO_NUM]; // 每组全眼图数据信息</p>
<p id="p3825174219273"><a name="p3825174219273"></a><a name="p3825174219273"></a>} DCMI_SERDES_FULL_EYE;</p>
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

**约束说明<a name="section8765321374"></a>**

该接口在物理机+特权容器场景下支持使用。

**表 1** 不同部署场景下的支持情况

<a name="table20788125825517"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row6501201391416"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p96364216405"><a name="zh-cn_topic_0000002485318818_p96364216405"></a><a name="zh-cn_topic_0000002485318818_p96364216405"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p166362224014"><a name="zh-cn_topic_0000002485318818_p166362224014"></a><a name="zh-cn_topic_0000002485318818_p166362224014"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p26368274016"><a name="zh-cn_topic_0000002485318818_p26368274016"></a><a name="zh-cn_topic_0000002485318818_p26368274016"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p46363264012"><a name="zh-cn_topic_0000002485318818_p46363264012"></a><a name="zh-cn_topic_0000002485318818_p46363264012"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row7501101371412"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p85021133146"><a name="zh-cn_topic_0000002485318818_p85021133146"></a><a name="zh-cn_topic_0000002485318818_p85021133146"></a><span id="zh-cn_topic_0000002485318818_text14502413151414"><a name="zh-cn_topic_0000002485318818_text14502413151414"></a><a name="zh-cn_topic_0000002485318818_text14502413151414"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p126134434150"><a name="zh-cn_topic_0000002485318818_p126134434150"></a><a name="zh-cn_topic_0000002485318818_p126134434150"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p20502181319141"><a name="zh-cn_topic_0000002485318818_p20502181319141"></a><a name="zh-cn_topic_0000002485318818_p20502181319141"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1250281314143"><a name="zh-cn_topic_0000002485318818_p1250281314143"></a><a name="zh-cn_topic_0000002485318818_p1250281314143"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row14502111318141"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1850241391415"><a name="zh-cn_topic_0000002485318818_p1850241391415"></a><a name="zh-cn_topic_0000002485318818_p1850241391415"></a><span id="zh-cn_topic_0000002485318818_text1550201371412"><a name="zh-cn_topic_0000002485318818_text1550201371412"></a><a name="zh-cn_topic_0000002485318818_text1550201371412"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1563314438157"><a name="zh-cn_topic_0000002485318818_p1563314438157"></a><a name="zh-cn_topic_0000002485318818_p1563314438157"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p450261315143"><a name="zh-cn_topic_0000002485318818_p450261315143"></a><a name="zh-cn_topic_0000002485318818_p450261315143"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1750213136147"><a name="zh-cn_topic_0000002485318818_p1750213136147"></a><a name="zh-cn_topic_0000002485318818_p1750213136147"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1350261315144"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p165021213171419"><a name="zh-cn_topic_0000002485318818_p165021213171419"></a><a name="zh-cn_topic_0000002485318818_p165021213171419"></a><span id="zh-cn_topic_0000002485318818_text65024132144"><a name="zh-cn_topic_0000002485318818_text65024132144"></a><a name="zh-cn_topic_0000002485318818_text65024132144"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p9651124313150"><a name="zh-cn_topic_0000002485318818_p9651124313150"></a><a name="zh-cn_topic_0000002485318818_p9651124313150"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p45021313141413"><a name="zh-cn_topic_0000002485318818_p45021313141413"></a><a name="zh-cn_topic_0000002485318818_p45021313141413"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p850261315146"><a name="zh-cn_topic_0000002485318818_p850261315146"></a><a name="zh-cn_topic_0000002485318818_p850261315146"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1150210135144"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p85021713181415"><a name="zh-cn_topic_0000002485318818_p85021713181415"></a><a name="zh-cn_topic_0000002485318818_p85021713181415"></a><span id="zh-cn_topic_0000002485318818_text75023132148"><a name="zh-cn_topic_0000002485318818_text75023132148"></a><a name="zh-cn_topic_0000002485318818_text75023132148"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p16571443161520"><a name="zh-cn_topic_0000002485318818_p16571443161520"></a><a name="zh-cn_topic_0000002485318818_p16571443161520"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p150251391416"><a name="zh-cn_topic_0000002485318818_p150251391416"></a><a name="zh-cn_topic_0000002485318818_p150251391416"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p8502111319143"><a name="zh-cn_topic_0000002485318818_p8502111319143"></a><a name="zh-cn_topic_0000002485318818_p8502111319143"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row4502913101412"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p25023136145"><a name="zh-cn_topic_0000002485318818_p25023136145"></a><a name="zh-cn_topic_0000002485318818_p25023136145"></a><span id="zh-cn_topic_0000002485318818_text4502191331419"><a name="zh-cn_topic_0000002485318818_text4502191331419"></a><a name="zh-cn_topic_0000002485318818_text4502191331419"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p250291313144"><a name="zh-cn_topic_0000002485318818_p250291313144"></a><a name="zh-cn_topic_0000002485318818_p250291313144"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p19502713101415"><a name="zh-cn_topic_0000002485318818_p19502713101415"></a><a name="zh-cn_topic_0000002485318818_p19502713101415"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1150261316146"><a name="zh-cn_topic_0000002485318818_p1150261316146"></a><a name="zh-cn_topic_0000002485318818_p1150261316146"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row19502151319148"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p10502191381420"><a name="zh-cn_topic_0000002485318818_p10502191381420"></a><a name="zh-cn_topic_0000002485318818_p10502191381420"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section1888234213618"></a>**

```
…   
int ret = 0;  
 int card_id = 0;  
 int device_id = 0;  
 struct dcmi_serdes_full_eye serdes_full_eye_info = {
          .macro_id = 9,
          .lane_id = 0,
          .serdes_full_eye = {0} 
 } 
 ret = dcmi_get_serdes_full_eye(card_id, device_id, &serdes_full_eye_info);  
…
```

