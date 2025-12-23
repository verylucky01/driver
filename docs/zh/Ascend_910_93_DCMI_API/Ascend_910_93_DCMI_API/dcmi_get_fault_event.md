# dcmi\_get\_fault\_event<a name="ZH-CN_TOPIC_0000002485478688"></a>

**函数原型<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412077"></a>**

**int dcmi\_get\_fault\_event\(int card\_id, int device\_id, int timeout, struct dcmi\_event\_filter filter, struct dcmi\_event \*event\)**

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
<tbody><tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_row10560021192510"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p36741947142813"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p36741947142813"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p96741747122818"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p96741747122818"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p46747472287"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p46747472287"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p1467413474281"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p1467413474281"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p1467413474281"></a>设备ID，当前实际支持的ID通过<a href="dcmi_get_card_num_list.md">dcmi_get_card_num_list</a>接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_row15462171542913"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p445315176517"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p445315176517"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p445315176517"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p1926232982914"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p1926232982914"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p1926232982914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p826217292293"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p826217292293"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_p826217292293"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p989765018173"><a name="p989765018173"></a><a name="p989765018173"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_row029010598584"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1845492116012"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1845492116012"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1845492116012"></a>timeout</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p54545211409"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p54545211409"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p54545211409"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p17454182115010"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p17454182115010"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p17454182115010"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p13454821402"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p13454821402"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p13454821402"></a>timeout &gt;= 0：阻塞等待timeout(ms)时间，最大阻塞时间为30000ms(30s)； timeout = -1：阻塞等待永不超时。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_row1813413245919"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p3223737605"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p3223737605"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p3223737605"></a>filter</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p132231637306"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p132231637306"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p132231637306"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p92231037703"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p92231037703"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p92231037703"></a>struct dcmi_event_filter</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p677479218"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p677479218"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p677479218"></a>可只订阅满足指定条件的事件，过滤条件如下：</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p67749916113"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p67749916113"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p67749916113"></a>#define DCMI_EVENT_FILTER_FLAG_EVENT_ID (1UL &lt;&lt; 0)</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p577419916110"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p577419916110"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p577419916110"></a>#define DCMI_EVENT_FILTER_FLAG_SERVERITY (1UL &lt;&lt; 1)</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15774198112"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15774198112"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15774198112"></a>#define DCMI_EVENT_FILTER_FLAG_NODE_TYPE (1UL &lt;&lt; 2)</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p877419912118"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p877419912118"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p877419912118"></a>#define DCMI_MAX_EVENT_RESV_LENGTH 32</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p107744919118"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p107744919118"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p107744919118"></a>struct dcmi_event_filter {</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p11774692116"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p11774692116"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p11774692116"></a>unsigned long long filter_flag; // 可单独使能某个过滤条件，也可将全部条件同时使能，过滤条件如下：</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15774497114"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15774497114"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15774497114"></a>0：不使能过滤条件</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p207741891614"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p207741891614"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p207741891614"></a>DCMI_EVENT_FILTER_FLAG_EVENT_ID：只接收指定的事件</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p127741392011"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p127741392011"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p127741392011"></a>DCMI_EVENT_FILTER_FLAG_SERVERITY：只接收指定级别及以上的事件</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p6774169517"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p6774169517"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p6774169517"></a>DCMI_EVENT_FILTER_FLAG_NODE_TYPE：只接收指定节点类型的事件</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p127741596114"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p127741596114"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p127741596114"></a>unsigned int event_id;  //接收指定的事件：参考《健康管理故障定义》</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p077479911"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p077479911"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p077479911"></a>unsigned char severity; //接收指定级别及以上的事件：见struct dcmi_dms_fault_event结构体中severity定义</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p13774591615"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p13774591615"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p13774591615"></a>unsigned char node_type; //接收指定节点类型的事件：参考《健康管理故障定义》</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p62763213114"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p62763213114"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p62763213114"></a>unsigned char resv[DCMI_MAX_EVENT_RESV_LENGTH];    //保留</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1276152117115"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1276152117115"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1276152117115"></a>};</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_row122061951597"><td class="cellrowborder" valign="top" width="15.72%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p984413468115"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p984413468115"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p984413468115"></a>event</p>
</td>
<td class="cellrowborder" valign="top" width="13.850000000000001%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p984415461712"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p984415461712"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p984415461712"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p584444618112"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p584444618112"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p584444618112"></a>struct dcmi_event *</p>
</td>
<td class="cellrowborder" valign="top" width="54.379999999999995%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1294095716117"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1294095716117"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1294095716117"></a>输出事件结构体定义如下：</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p694014571715"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p694014571715"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p694014571715"></a>struct dcmi_event {</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1994013571112"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1994013571112"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1994013571112"></a>enum dcmi_event_type type;  // 事件类型</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1494018571111"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1494018571111"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1494018571111"></a>union {</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p199401657312"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p199401657312"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p199401657312"></a>struct dcmi_dms_fault_event dms_event;  //事件内容</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1194017571918"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1194017571918"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1194017571918"></a>} event_t;</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p494017571618"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p494017571618"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p494017571618"></a>};</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p29403577112"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p29403577112"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p29403577112"></a><strong id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_b99401057513"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_b99401057513"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_b99401057513"></a>type：</strong></p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p129401157216"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p129401157216"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p129401157216"></a>当前支持DCMI_DMS_FAULT_EVENT类型，枚举定义如下：</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p99401957313"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p99401957313"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p99401957313"></a>enum dcmi_event_type {</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p12940857414"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p12940857414"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p12940857414"></a>DCMI_DMS_FAULT_EVENT = 0,</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p159407571517"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p159407571517"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p159407571517"></a>DCMI_EVENT_TYPE_MAX</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p09408571311"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p09408571311"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p09408571311"></a>};</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p119402571215"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p119402571215"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p119402571215"></a><strong id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_b494095714115"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_b494095714115"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_b494095714115"></a>dms_event：</strong></p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p79405571618"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p79405571618"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p79405571618"></a>DCMI_DMS_FAULT_EVENT类型对应的事件内容定义如下：</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p8940457614"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p8940457614"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p8940457614"></a>#define DCMI_MAX_EVENT_NAME_LENGTH 256</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p7940165716118"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p7940165716118"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p7940165716118"></a>#define DCMI_MAX_EVENT_DATA_LENGTH 32</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p8940115720118"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p8940115720118"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p8940115720118"></a>#define DCMI_MAX_EVENT_RESV_LENGTH 32</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15940057419"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15940057419"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p15940057419"></a>struct dcmi_dms_fault_event {</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1294035720115"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1294035720115"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1294035720115"></a>unsigned int event_id; // 事件ID</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1094010574111"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1094010574111"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1094010574111"></a>unsigned short deviceid; //设备号</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p59408579120"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p59408579120"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p59408579120"></a>unsigned char node_type;  //节点类型</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p794015710110"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p794015710110"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p794015710110"></a>unsigned char node_id; //节点ID</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p694011575118"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p694011575118"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p694011575118"></a>unsigned char sub_node_type; //子节点类型</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1694017579119"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1694017579119"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1694017579119"></a>unsigned char sub_node_id; //子节点ID</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1794014571416"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1794014571416"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1794014571416"></a>unsigned char severity;  // 事件级别 0：提示，1：次要，2：重要，3：紧急</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p10940857219"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p10940857219"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p10940857219"></a>unsigned char assertion; //事件类型0：故障恢复，1：故障产生，2：一次性事件</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p2094019571318"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p2094019571318"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p2094019571318"></a>int event_serial_num; // 告警序列号</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p3940957718"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p3940957718"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p3940957718"></a>int notify_serial_num; // 通知序列号</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p69407571318"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p69407571318"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p69407571318"></a>unsigned long long alarm_raised_time; //事件产生时间：自1970年1月1日0点0分0秒开始至今的毫秒数</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1594010574111"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1594010574111"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1594010574111"></a>char event_name[DCMI_MAX_EVENT_NAME_LENGTH]; //事件描述信息</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p18940145715116"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p18940145715116"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p18940145715116"></a>char additional_info[DCMI_MAX_EVENT_DATA_LENGTH]; //事件附加信息</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p294011571113"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p294011571113"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p294011571113"></a>unsigned char resv[DCMI_MAX_EVENT_RESV_LENGTH]; //保留</p>
<p id="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p294010573116"><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p294010573116"></a><a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p294010573116"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412082"></a>**

-   该接口可获取故障产生时正在上报故障或恢复事件，不能获取已经产生的历史事件。
-   该接口支持多进程不支持多线程，最大支持64个进程同时调用。

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

**调用示例<a name="zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_toc533412083"></a>**

```
…  
int ret = 0; 
int card_id = 0; 
int device_id = 0;
int timeout = 1000;
struct dcmi_event_filter filter = {0};
struct dcmi_event event = {0};
filter.filter_flag = DCMI_EVENT_FILTER_FLAG_SERVERITY | DCMI_EVENT_FILTER_FLAG_NODE_TYPE; 
filter.severity = 2; /* 只订阅2~3级别的事件 */
filter.node_type = 0x40; /* 只订阅模块ID为SOC类型的事件 */
ret = dcmi_get_fault_event(card_id, device_id, timeout, filter, &event); 
if (ret != DCMI_OK) {
    printf("dcmi_get_fault_event failed. err is %d\n", ret);
}
// todo
…
```

