# dcmi\_set\_device\_ip<a name="ZH-CN_TOPIC_0000002517615309"></a>

**函数原型<a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_toc533412077"></a>**

**int dcmi\_set\_device\_ip\(int card\_id, int device\_id, enum dcmi\_port\_type input\_type, int port\_id, struct dcmi\_ip\_addr \*ip, struct dcmi\_ip\_addr \*mask\)**

**功能说明<a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_toc533412078"></a>**

设置IP地址和mask地址。

**参数说明<a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_toc533412079"></a>**

<a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p10021890"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p10021890"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p6466753"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p6466753"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p54045009"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p54045009"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p15569626"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p15569626"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p36741947142813"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p36741947142813"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p96741747122818"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p96741747122818"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p46747472287"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p46747472287"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1467413474281"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1467413474281"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p7711145152918"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p7711145152918"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p671116522914"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p671116522914"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1771116572910"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1771116572910"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p10819239548"><a name="p10819239548"></a><a name="p10819239548"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5522164215178"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5522164215178"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5522164215178"></a>input_type</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p8522242101715"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p8522242101715"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p17522114220174"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p17522114220174"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p17522114220174"></a>enum dcmi_port_type</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p0522164231718"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p0522164231718"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p0522164231718"></a>指定网口类型。</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p139261310201818"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p139261310201818"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p139261310201818"></a>enum dcmi_port_type {</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5926810151810"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5926810151810"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5926810151810"></a>DCMI_VNIC_PORT = 0, //虚拟网口</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p4926121041817"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p4926121041817"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p4926121041817"></a>DCMI_ROCE_PORT = 1, //ROCE网口</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p159260105182"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p159260105182"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p159260105182"></a>DCMI_INVALID_PORT</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p169267108184"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p169267108184"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p169267108184"></a>};</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p8907162231913"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p8907162231913"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p8907162231913"></a>不支持DCMI_VNIC_PORT。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_row1382112338184"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p2082216331186"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p2082216331186"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p2082216331186"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1982217333181"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1982217333181"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1982217333181"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p118221533101811"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p118221533101811"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p118221533101811"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1182253319187"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1182253319187"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1182253319187"></a>指定网口号，保留字段。取值范围：[0, 255]。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_row14113163220181"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1911443271820"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1911443271820"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1911443271820"></a>ip</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p10114432171816"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p10114432171816"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p10114432171816"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p15114123220187"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p15114123220187"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p15114123220187"></a>struct dcmi_ip_addr *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p496716258191"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p496716258191"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p496716258191"></a>IP地址</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p11821111510193"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p11821111510193"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p11821111510193"></a>struct dcmi_ip_addr {</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p178211153196"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p178211153196"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p178211153196"></a>union {</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p168212015141919"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p168212015141919"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p168212015141919"></a>unsigned char ip6[16]; //IPv6地址</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p2821191541917"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p2821191541917"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p2821191541917"></a>unsigned char ip4[4]; //IPv4地址</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p158211715161914"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p158211715161914"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p158211715161914"></a>} u_addr;</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1382110151199"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1382110151199"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1382110151199"></a>enum dcmi_ip_addr_type ip_type; //IP类型</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p382111158199"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p382111158199"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p382111158199"></a>};</p>
<p id="p12385162518545"><a name="p12385162518545"></a><a name="p12385162518545"></a></p>
<div class="note" id="note06326822712"><a name="note06326822712"></a><a name="note06326822712"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p206320810278"><a name="p206320810278"></a><a name="p206320810278"></a>支持IPv6协议，ip6数组中第一位元素的值为该IP信息的前缀，合法值为0~128，其他元素无意义。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_row5576903403"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5731143173615"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5731143173615"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p5731143173615"></a>mask</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p7732036361"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p7732036361"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p7732036361"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p107323315363"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p107323315363"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p107323315363"></a>struct dcmi_ip_addr *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1373217317365"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1373217317365"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1373217317365"></a>mask地址</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p137321334364"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p137321334364"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p137321334364"></a>struct dcmi_ip_addr {</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p87320310363"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p87320310363"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p87320310363"></a>union {</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p147321636366"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p147321636366"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p147321636366"></a>unsigned char ip6[16]; //IPv6地址</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p07321437363"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p07321437363"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p07321437363"></a>unsigned char ip4[4]; //IPv4地址</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1473253103611"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1473253103611"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1473253103611"></a>} u_addr;</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1473218314366"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1473218314366"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p1473218314366"></a>enum dcmi_ip_addr_type ip_type; //IP类型</p>
<p id="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p873243133614"><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p873243133614"></a><a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_p873243133614"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table6665182042413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row169301746431"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p455965912315"><a name="zh-cn_topic_0000002485295476_p455965912315"></a><a name="zh-cn_topic_0000002485295476_p455965912315"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p1855915599312"><a name="zh-cn_topic_0000002485295476_p1855915599312"></a><a name="zh-cn_topic_0000002485295476_p1855915599312"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p1155945933111"><a name="zh-cn_topic_0000002485295476_p1155945933111"></a><a name="zh-cn_topic_0000002485295476_p1155945933111"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p145593594316"><a name="zh-cn_topic_0000002485295476_p145593594316"></a><a name="zh-cn_topic_0000002485295476_p145593594316"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row1393017444310"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p793119413433"><a name="zh-cn_topic_0000002485295476_p793119413433"></a><a name="zh-cn_topic_0000002485295476_p793119413433"></a><span id="zh-cn_topic_0000002485295476_ph1893115424314"><a name="zh-cn_topic_0000002485295476_ph1893115424314"></a><a name="zh-cn_topic_0000002485295476_ph1893115424314"></a><span id="zh-cn_topic_0000002485295476_text99311140437"><a name="zh-cn_topic_0000002485295476_text99311140437"></a><a name="zh-cn_topic_0000002485295476_text99311140437"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p99311442430"><a name="zh-cn_topic_0000002485295476_p99311442430"></a><a name="zh-cn_topic_0000002485295476_p99311442430"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1093114411431"><a name="zh-cn_topic_0000002485295476_p1093114411431"></a><a name="zh-cn_topic_0000002485295476_p1093114411431"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p39315418437"><a name="zh-cn_topic_0000002485295476_p39315418437"></a><a name="zh-cn_topic_0000002485295476_p39315418437"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row79316404316"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p3931545437"><a name="zh-cn_topic_0000002485295476_p3931545437"></a><a name="zh-cn_topic_0000002485295476_p3931545437"></a><span id="zh-cn_topic_0000002485295476_text893113494316"><a name="zh-cn_topic_0000002485295476_text893113494316"></a><a name="zh-cn_topic_0000002485295476_text893113494316"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p49310454319"><a name="zh-cn_topic_0000002485295476_p49310454319"></a><a name="zh-cn_topic_0000002485295476_p49310454319"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p993111464319"><a name="zh-cn_topic_0000002485295476_p993111464319"></a><a name="zh-cn_topic_0000002485295476_p993111464319"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p893111419431"><a name="zh-cn_topic_0000002485295476_p893111419431"></a><a name="zh-cn_topic_0000002485295476_p893111419431"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row39314494319"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1893154144315"><a name="zh-cn_topic_0000002485295476_p1893154144315"></a><a name="zh-cn_topic_0000002485295476_p1893154144315"></a><span id="zh-cn_topic_0000002485295476_text69317417433"><a name="zh-cn_topic_0000002485295476_text69317417433"></a><a name="zh-cn_topic_0000002485295476_text69317417433"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p159312418432"><a name="zh-cn_topic_0000002485295476_p159312418432"></a><a name="zh-cn_topic_0000002485295476_p159312418432"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p11931046437"><a name="zh-cn_topic_0000002485295476_p11931046437"></a><a name="zh-cn_topic_0000002485295476_p11931046437"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1093117412433"><a name="zh-cn_topic_0000002485295476_p1093117412433"></a><a name="zh-cn_topic_0000002485295476_p1093117412433"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row129312414310"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p793114424311"><a name="zh-cn_topic_0000002485295476_p793114424311"></a><a name="zh-cn_topic_0000002485295476_p793114424311"></a><span id="zh-cn_topic_0000002485295476_text393110464310"><a name="zh-cn_topic_0000002485295476_text393110464310"></a><a name="zh-cn_topic_0000002485295476_text393110464310"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1893114154312"><a name="zh-cn_topic_0000002485295476_p1893114154312"></a><a name="zh-cn_topic_0000002485295476_p1893114154312"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p59316474318"><a name="zh-cn_topic_0000002485295476_p59316474318"></a><a name="zh-cn_topic_0000002485295476_p59316474318"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1693115417437"><a name="zh-cn_topic_0000002485295476_p1693115417437"></a><a name="zh-cn_topic_0000002485295476_p1693115417437"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row89311147437"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1893194154319"><a name="zh-cn_topic_0000002485295476_p1893194154319"></a><a name="zh-cn_topic_0000002485295476_p1893194154319"></a><span id="zh-cn_topic_0000002485295476_text2093119410435"><a name="zh-cn_topic_0000002485295476_text2093119410435"></a><a name="zh-cn_topic_0000002485295476_text2093119410435"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p10931104184313"><a name="zh-cn_topic_0000002485295476_p10931104184313"></a><a name="zh-cn_topic_0000002485295476_p10931104184313"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1593174124311"><a name="zh-cn_topic_0000002485295476_p1593174124311"></a><a name="zh-cn_topic_0000002485295476_p1593174124311"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1193110410433"><a name="zh-cn_topic_0000002485295476_p1193110410433"></a><a name="zh-cn_topic_0000002485295476_p1193110410433"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1593110424311"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p169318417439"><a name="zh-cn_topic_0000002485295476_p169318417439"></a><a name="zh-cn_topic_0000002485295476_p169318417439"></a><span id="zh-cn_topic_0000002485295476_text193112484314"><a name="zh-cn_topic_0000002485295476_text193112484314"></a><a name="zh-cn_topic_0000002485295476_text193112484314"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p813010149432"><a name="zh-cn_topic_0000002485295476_p813010149432"></a><a name="zh-cn_topic_0000002485295476_p813010149432"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p4931174104310"><a name="zh-cn_topic_0000002485295476_p4931174104310"></a><a name="zh-cn_topic_0000002485295476_p4931174104310"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p29311647436"><a name="zh-cn_topic_0000002485295476_p29311647436"></a><a name="zh-cn_topic_0000002485295476_p29311647436"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row109312418438"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p159327434319"><a name="zh-cn_topic_0000002485295476_p159327434319"></a><a name="zh-cn_topic_0000002485295476_p159327434319"></a><span id="zh-cn_topic_0000002485295476_text179321145437"><a name="zh-cn_topic_0000002485295476_text179321145437"></a><a name="zh-cn_topic_0000002485295476_text179321145437"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p714181416435"><a name="zh-cn_topic_0000002485295476_p714181416435"></a><a name="zh-cn_topic_0000002485295476_p714181416435"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p7932114174312"><a name="zh-cn_topic_0000002485295476_p7932114174312"></a><a name="zh-cn_topic_0000002485295476_p7932114174312"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p19323418433"><a name="zh-cn_topic_0000002485295476_p19323418433"></a><a name="zh-cn_topic_0000002485295476_p19323418433"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1993264174312"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1693212411435"><a name="zh-cn_topic_0000002485295476_p1693212411435"></a><a name="zh-cn_topic_0000002485295476_p1693212411435"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251227175_zh-cn_topic_0000001178373134_zh-cn_topic_0000001101213240_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
unsigned int ip_addr = 0xC801A8C0; //192.168.1.200（仅用作示例，以实际配置网关地址为准）
unsigned int mask_addr = 0x00FFFFFF; //255.255.255.0（仅用作示例，以实际配置网关地址为准）
struct dcmi_ip_addr ip_address = {0};
struct dcmi_ip_addr ip_mask_address = {0};
memcpy(&(ip_address.u_addr.ip4[0]),&ip_addr,4);
memcpy(&(ip_mask_address.u_addr.ip4[0]),&mask_addr,4);
ret = dcmi_set_device_ip(card_id,device_id, DCMI_ROCE_PORT, port_id, &ip_address, &ip_mask_address);
if (ret != 0) {
    //todo:记录日志
    return ret;
}
…
```

