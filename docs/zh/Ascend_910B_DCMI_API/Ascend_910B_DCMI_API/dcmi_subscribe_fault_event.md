# dcmi\_subscribe\_fault\_event<a name="ZH-CN_TOPIC_0000002517615351"></a>

**函数原型<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412077"></a>**

**int dcmi\_subscribe\_fault\_event\(int card\_id, int device\_id, struct dcmi\_event\_filter filter, dcmi\_fault\_event\_callback handler\)**

**功能说明<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412078"></a>**

订阅设备故障或恢复事件的接口。

**参数说明<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412079"></a>**

<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_row7580267"><th class="cellrowborder" valign="top" width="15.72%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p10021890"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p10021890"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="13.850000000000001%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p6466753"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p6466753"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.05%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p54045009"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p54045009"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="54.379999999999995%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p15569626"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p15569626"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_row10560021192510"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="p7782194112263"><a name="p7782194112263"></a><a name="p7782194112263"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="p11782124113268"><a name="p11782124113268"></a><a name="p11782124113268"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="p678254192617"><a name="p678254192617"></a><a name="p678254192617"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="p117821641162613"><a name="p117821641162613"></a><a name="p117821641162613"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_row15462171542913"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="p1378274110266"><a name="p1378274110266"></a><a name="p1378274110266"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="p107831541162615"><a name="p107831541162615"></a><a name="p107831541162615"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="p197831141162619"><a name="p197831141162619"></a><a name="p197831141162619"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="p4783641142619"><a name="p4783641142619"></a><a name="p4783641142619"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p18783124122616"><a name="p18783124122616"></a><a name="p18783124122616"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p0640132624616"><a name="p0640132624616"></a><a name="p0640132624616"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_row029010598584"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="p177831941142620"><a name="p177831941142620"></a><a name="p177831941142620"></a>filter</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="p67839413264"><a name="p67839413264"></a><a name="p67839413264"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="p6783441172611"><a name="p6783441172611"></a><a name="p6783441172611"></a>struct dcmi_event_filter</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="p107837415267"><a name="p107837415267"></a><a name="p107837415267"></a>可只订阅满足指定条件的事件，过滤条件如下：</p>
<p id="p478324114261"><a name="p478324114261"></a><a name="p478324114261"></a>#define DCMI_EVENT_FILTER_FLAG_EVENT_ID (1UL &lt;&lt; 0)</p>
<p id="p07830411263"><a name="p07830411263"></a><a name="p07830411263"></a>#define DCMI_EVENT_FILTER_FLAG_SERVERITY (1UL &lt;&lt; 1)</p>
<p id="p18783144192618"><a name="p18783144192618"></a><a name="p18783144192618"></a>#define DCMI_EVENT_FILTER_FLAG_NODE_TYPE (1UL &lt;&lt; 2)</p>
<p id="p15783641152618"><a name="p15783641152618"></a><a name="p15783641152618"></a>#define DCMI_MAX_EVENT_RESV_LENGTH 32</p>
<p id="p7783164122618"><a name="p7783164122618"></a><a name="p7783164122618"></a>struct dcmi_event_filter {</p>
<p id="p10783941102611"><a name="p10783941102611"></a><a name="p10783941102611"></a>unsigned long long filter_flag; //可单独使能某个过滤条件，也可将全部条件同时使能，过滤条件如下：</p>
<p id="p147834411264"><a name="p147834411264"></a><a name="p147834411264"></a>0：不使能过滤条件</p>
<p id="p10783114114268"><a name="p10783114114268"></a><a name="p10783114114268"></a>DCMI_EVENT_FILTER_FLAG_EVENT_ID：只接收指定的事件</p>
<p id="p27831841172610"><a name="p27831841172610"></a><a name="p27831841172610"></a>DCMI_EVENT_FILTER_FLAG_SERVERITY：只接收指定级别及以上的事件</p>
<p id="p1078314142613"><a name="p1078314142613"></a><a name="p1078314142613"></a>DCMI_EVENT_FILTER_FLAG_NODE_TYPE：只接收指定节点类型的事件</p>
<p id="zh-cn_topic_0000001549080826_p478314142614"><a name="zh-cn_topic_0000001549080826_p478314142614"></a><a name="zh-cn_topic_0000001549080826_p478314142614"></a>unsigned int event_id;  //接收指定的事件：参考《健康管理故障定义》</p>
<p id="zh-cn_topic_0000001549080826_p1078313417269"><a name="zh-cn_topic_0000001549080826_p1078313417269"></a><a name="zh-cn_topic_0000001549080826_p1078313417269"></a>unsigned char severity; //接收指定级别及以上的事件：见struct dcmi_dms_fault_event结构体中severity定义</p>
<p id="zh-cn_topic_0000001549080826_p12783174114261"><a name="zh-cn_topic_0000001549080826_p12783174114261"></a><a name="zh-cn_topic_0000001549080826_p12783174114261"></a>unsigned char node_type;  //接收指定节点类型的事件：参考《健康管理故障定义》</p>
<p id="zh-cn_topic_0000001549080826_p6783184122615"><a name="zh-cn_topic_0000001549080826_p6783184122615"></a><a name="zh-cn_topic_0000001549080826_p6783184122615"></a>unsigned char resv[DCMI_MAX_EVENT_RESV_LENGTH];//保留</p>
<p id="p6783184122615"><a name="p6783184122615"></a><a name="p6783184122615"></a></p>
<p id="p20783541132610"><a name="p20783541132610"></a><a name="p20783541132610"></a>};</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_row1813413245919"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="p8783141102611"><a name="p8783141102611"></a><a name="p8783141102611"></a>handler</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="p378364152617"><a name="p378364152617"></a><a name="p378364152617"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="p187831641182612"><a name="p187831641182612"></a><a name="p187831641182612"></a>dcmi_fault_event_callback</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="p16783641182611"><a name="p16783641182611"></a><a name="p16783641182611"></a>typedef void (*dcmi_fault_event_callback)(struct dcmi_event *event);</p>
<p id="p12783174113266"><a name="p12783174113266"></a><a name="p12783174113266"></a>输出事件结构体定义如下：</p>
<p id="p15783114122620"><a name="p15783114122620"></a><a name="p15783114122620"></a>struct dcmi_event {</p>
<p id="p478394112263"><a name="p478394112263"></a><a name="p478394112263"></a>enum dcmi_event_type type; //事件类型</p>
<p id="p0783194115263"><a name="p0783194115263"></a><a name="p0783194115263"></a>union {</p>
<p id="p37836415262"><a name="p37836415262"></a><a name="p37836415262"></a>struct dcmi_dms_fault_event dms_event; //事件内容</p>
<p id="p20783144119263"><a name="p20783144119263"></a><a name="p20783144119263"></a>} event_t;</p>
<p id="p19783134192614"><a name="p19783134192614"></a><a name="p19783134192614"></a>};</p>
<p id="p778334111264"><a name="p778334111264"></a><a name="p778334111264"></a><strong id="b1978364113263"><a name="b1978364113263"></a><a name="b1978364113263"></a>type：</strong></p>
<p id="p1778318416263"><a name="p1778318416263"></a><a name="p1778318416263"></a>当前支持DCMI_DMS_FAULT_EVENT类型，枚举定义如下：</p>
<p id="p137831641202617"><a name="p137831641202617"></a><a name="p137831641202617"></a>enum dcmi_event_type {</p>
<p id="p10783541132612"><a name="p10783541132612"></a><a name="p10783541132612"></a>DCMI_DMS_FAULT_EVENT = 0,</p>
<p id="p1783041102617"><a name="p1783041102617"></a><a name="p1783041102617"></a>DCMI_EVENT_TYPE_MAX</p>
<p id="p1278364152611"><a name="p1278364152611"></a><a name="p1278364152611"></a>};</p>
<p id="p1578354172618"><a name="p1578354172618"></a><a name="p1578354172618"></a><strong id="b9784164117264"><a name="b9784164117264"></a><a name="b9784164117264"></a>dms_event：</strong></p>
<p id="p13784641192613"><a name="p13784641192613"></a><a name="p13784641192613"></a>DCMI_DMS_FAULT_EVENT类型对应的事件内容定义如下：</p>
<p id="p207841241192618"><a name="p207841241192618"></a><a name="p207841241192618"></a>#define DCMI_MAX_EVENT_NAME_LENGTH 256</p>
<p id="p157841141112617"><a name="p157841141112617"></a><a name="p157841141112617"></a>#define DCMI_MAX_EVENT_DATA_LENGTH 32</p>
<p id="p8784341132612"><a name="p8784341132612"></a><a name="p8784341132612"></a>#define DCMI_MAX_EVENT_RESV_LENGTH 32</p>
<p id="p978474113269"><a name="p978474113269"></a><a name="p978474113269"></a>struct dcmi_dms_fault_event {</p>
<p id="zh-cn_topic_0000001549080826_p97841641112619"><a name="zh-cn_topic_0000001549080826_p97841641112619"></a><a name="zh-cn_topic_0000001549080826_p97841641112619"></a>unsigned int event_id; //事件ID</p>
<p id="zh-cn_topic_0000001549080826_p778464122611"><a name="zh-cn_topic_0000001549080826_p778464122611"></a><a name="zh-cn_topic_0000001549080826_p778464122611"></a>unsigned short deviceid;  //设备号</p>
<p id="zh-cn_topic_0000001549080826_p117841641152615"><a name="zh-cn_topic_0000001549080826_p117841641152615"></a><a name="zh-cn_topic_0000001549080826_p117841641152615"></a>unsigned char node_type;  //节点类型</p>
<p id="zh-cn_topic_0000001549080826_p19784144119266"><a name="zh-cn_topic_0000001549080826_p19784144119266"></a><a name="zh-cn_topic_0000001549080826_p19784144119266"></a>unsigned char node_id;  //节点ID</p>
<p id="zh-cn_topic_0000001549080826_p14784104111269"><a name="zh-cn_topic_0000001549080826_p14784104111269"></a><a name="zh-cn_topic_0000001549080826_p14784104111269"></a>unsigned char sub_node_type; //子节点类型</p>
<p id="zh-cn_topic_0000001549080826_p1578444117262"><a name="zh-cn_topic_0000001549080826_p1578444117262"></a><a name="zh-cn_topic_0000001549080826_p1578444117262"></a>unsigned char sub_node_id; //子节点ID</p>
<p id="zh-cn_topic_0000001549080826_p19784154182614"><a name="zh-cn_topic_0000001549080826_p19784154182614"></a><a name="zh-cn_topic_0000001549080826_p19784154182614"></a>unsigned char severity;  // 事件级别 0：提示，1：次要，2：重要，3：紧急</p>
<p id="zh-cn_topic_0000001549080826_p978494122619"><a name="zh-cn_topic_0000001549080826_p978494122619"></a><a name="zh-cn_topic_0000001549080826_p978494122619"></a>unsigned char assertion; // 事件类型0：故障恢复，1：故障产生，2：一次性事件</p>
<p id="zh-cn_topic_0000001549080826_p1278494113268"><a name="zh-cn_topic_0000001549080826_p1278494113268"></a><a name="zh-cn_topic_0000001549080826_p1278494113268"></a>int event_serial_num;  //告警序列号</p>
<p id="zh-cn_topic_0000001549080826_p16784204172616"><a name="zh-cn_topic_0000001549080826_p16784204172616"></a><a name="zh-cn_topic_0000001549080826_p16784204172616"></a>int notify_serial_num;  //通知序列号</p>
<p id="zh-cn_topic_0000001549080826_p19784204132613"><a name="zh-cn_topic_0000001549080826_p19784204132613"></a><a name="zh-cn_topic_0000001549080826_p19784204132613"></a>unsigned long long alarm_raised_time;   //事件产生时间：自1970年1月1日0点0分0秒开始至今的毫秒数</p>
<p id="zh-cn_topic_0000001549080826_p177841641122616"><a name="zh-cn_topic_0000001549080826_p177841641122616"></a><a name="zh-cn_topic_0000001549080826_p177841641122616"></a>char event_name[DCMI_MAX_EVENT_NAME_LENGTH];        //事件描述信息</p>
<p id="zh-cn_topic_0000001549080826_p2078474112612"><a name="zh-cn_topic_0000001549080826_p2078474112612"></a><a name="zh-cn_topic_0000001549080826_p2078474112612"></a>char additional_info[DCMI_MAX_EVENT_DATA_LENGTH]; //事件附加信息</p>
<p id="zh-cn_topic_0000001549080826_p47841641172620"><a name="zh-cn_topic_0000001549080826_p47841641172620"></a><a name="zh-cn_topic_0000001549080826_p47841641172620"></a>unsigned char resv[DCMI_MAX_EVENT_RESV_LENGTH];     //保留</p>
<p id="p157849419265"><a name="p157849419265"></a><a name="p157849419265"></a>};</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row59025204"><th class="cellrowborder" valign="top" width="24.18%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="75.82%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row13362997"><td class="cellrowborder" valign="top" width="24.18%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="75.82%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a>处理结果：</p>
<a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><a name="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><ul id="zh-cn_topic_0000002485295458_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"><li>成功：返回0。</li><li>失败：返回码请参见<a href="return_codes.md">return_codes</a>。</li></ul>
</td>
</tr>
</tbody>
</table>

**异常处理<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_toc533412081"></a>**

无。

**约束说明<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412082"></a>**

-   当card\_id和device\_id的取值都为-1时，支持订阅当前环境中所有设备的故障事件。
-   该接口可获取故障产生时正在上报故障或恢复事件，不能获取已经产生的历史事件。
-   该接口支持多进程不支持多线程，最大支持64个进程同时调用。同一个进程中，不能与dcmi\_get\_fault\_event接口同时调用。
-   一个进程内只能调用一次，且物理机和特权容器共享64个进程资源，进程退出时才会释放该进程资源。
-   在SMP模式下，对于OS类的故障，只能从SMP下头节点的device\_id订阅到该OS类故障。

**表 1** 不同部署场景下的支持情况

<a name="table1113417173519"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1548132517501"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p16154843125019"><a name="zh-cn_topic_0000002485295476_p16154843125019"></a><a name="zh-cn_topic_0000002485295476_p16154843125019"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p3123182915595"><a name="zh-cn_topic_0000002485295476_p3123182915595"></a><a name="zh-cn_topic_0000002485295476_p3123182915595"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p6271950600"><a name="zh-cn_topic_0000002485295476_p6271950600"></a><a name="zh-cn_topic_0000002485295476_p6271950600"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p41541743195013"><a name="zh-cn_topic_0000002485295476_p41541743195013"></a><a name="zh-cn_topic_0000002485295476_p41541743195013"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p19444162914516"><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><span id="zh-cn_topic_0000002485295476_ph1944412296514"><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><span id="zh-cn_topic_0000002485295476_text944432913516"><a name="zh-cn_topic_0000002485295476_text944432913516"></a><a name="zh-cn_topic_0000002485295476_text944432913516"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row6920114611302"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p194441629165117"><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><span id="zh-cn_topic_0000002485295476_text124449291511"><a name="zh-cn_topic_0000002485295476_text124449291511"></a><a name="zh-cn_topic_0000002485295476_text124449291511"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p461320458219"><a name="zh-cn_topic_0000002485295476_p461320458219"></a><a name="zh-cn_topic_0000002485295476_p461320458219"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13613134519220"><a name="zh-cn_topic_0000002485295476_p13613134519220"></a><a name="zh-cn_topic_0000002485295476_p13613134519220"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p96133454211"><a name="zh-cn_topic_0000002485295476_p96133454211"></a><a name="zh-cn_topic_0000002485295476_p96133454211"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row14873174604910"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p10444329155118"><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><span id="zh-cn_topic_0000002485295476_text1044482925119"><a name="zh-cn_topic_0000002485295476_text1044482925119"></a><a name="zh-cn_topic_0000002485295476_text1044482925119"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p896391111511"><a name="zh-cn_topic_0000002485295476_p896391111511"></a><a name="zh-cn_topic_0000002485295476_p896391111511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p119682011105113"><a name="zh-cn_topic_0000002485295476_p119682011105113"></a><a name="zh-cn_topic_0000002485295476_p119682011105113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p11975911195114"><a name="zh-cn_topic_0000002485295476_p11975911195114"></a><a name="zh-cn_topic_0000002485295476_p11975911195114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row23051923114915"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p174441129175117"><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><span id="zh-cn_topic_0000002485295476_text34441729175119"><a name="zh-cn_topic_0000002485295476_text34441729175119"></a><a name="zh-cn_topic_0000002485295476_text34441729175119"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1298231135114"><a name="zh-cn_topic_0000002485295476_p1298231135114"></a><a name="zh-cn_topic_0000002485295476_p1298231135114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1298921117511"><a name="zh-cn_topic_0000002485295476_p1298921117511"></a><a name="zh-cn_topic_0000002485295476_p1298921117511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1799561165114"><a name="zh-cn_topic_0000002485295476_p1799561165114"></a><a name="zh-cn_topic_0000002485295476_p1799561165114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row44814564912"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1044414295516"><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><span id="zh-cn_topic_0000002485295476_text44441829105114"><a name="zh-cn_topic_0000002485295476_text44441829105114"></a><a name="zh-cn_topic_0000002485295476_text44441829105114"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p17181285116"><a name="zh-cn_topic_0000002485295476_p17181285116"></a><a name="zh-cn_topic_0000002485295476_p17181285116"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15131255114"><a name="zh-cn_topic_0000002485295476_p15131255114"></a><a name="zh-cn_topic_0000002485295476_p15131255114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15917127513"><a name="zh-cn_topic_0000002485295476_p15917127513"></a><a name="zh-cn_topic_0000002485295476_p15917127513"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row11171321114917"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p94441829165113"><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><span id="zh-cn_topic_0000002485295476_text644410297515"><a name="zh-cn_topic_0000002485295476_text644410297515"></a><a name="zh-cn_topic_0000002485295476_text644410297515"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p01211275113"><a name="zh-cn_topic_0000002485295476_p01211275113"></a><a name="zh-cn_topic_0000002485295476_p01211275113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p916161212512"><a name="zh-cn_topic_0000002485295476_p916161212512"></a><a name="zh-cn_topic_0000002485295476_p916161212512"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p191911211511"><a name="zh-cn_topic_0000002485295476_p191911211511"></a><a name="zh-cn_topic_0000002485295476_p191911211511"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row18385115320492"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1644572918513"><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><span id="zh-cn_topic_0000002485295476_text1744532925112"><a name="zh-cn_topic_0000002485295476_text1744532925112"></a><a name="zh-cn_topic_0000002485295476_text1744532925112"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1923112115119"><a name="zh-cn_topic_0000002485295476_p1923112115119"></a><a name="zh-cn_topic_0000002485295476_p1923112115119"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1269122515"><a name="zh-cn_topic_0000002485295476_p1269122515"></a><a name="zh-cn_topic_0000002485295476_p1269122515"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p43014126516"><a name="zh-cn_topic_0000002485295476_p43014126516"></a><a name="zh-cn_topic_0000002485295476_p43014126516"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row557915117191"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p2042757191916"><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><span id="zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412083"></a>**

```
void event_handler(struct dcmi_event *event){
    // todo
}
struct dcmi_event_filter filter;
filter.filter_flag = DCMI_EVENT_FILTER_FLAG_SERVERITY | DCMI_EVENT_FILTER_FLAG_NODE_TYPE;
filter.severity = 1; /* 只订阅2~3级别的事件 */
filter.node_type = 112;
struct dcmi_event event = {0};
int dev_id, ret;
……
ret = dcmi_subscribe_fault_event(card_id_list[card_id], device_id, filter, event_handler);
if (ret != 0) {
} else {
}
```

