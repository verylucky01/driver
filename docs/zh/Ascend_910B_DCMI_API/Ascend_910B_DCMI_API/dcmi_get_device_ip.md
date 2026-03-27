# dcmi\_get\_device\_ip<a name="ZH-CN_TOPIC_0000002517615417"></a>

**函数原型<a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_toc533412077"></a>**

**int dcmi\_get\_device\_ip\(int card\_id, int device\_id, enum dcmi\_port\_type input\_type, int port\_id, struct dcmi\_ip\_addr \*ip,** **struct dcmi\_ip\_addr \*mask\)**

**功能说明<a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_toc533412078"></a>**

获取IP地址和mask地址。

**参数说明<a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_toc533412079"></a>**

<a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p10021890"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p10021890"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p6466753"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p6466753"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p54045009"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p54045009"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p15569626"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p15569626"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p36741947142813"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p36741947142813"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p96741747122818"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p96741747122818"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p46747472287"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p46747472287"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1467413474281"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1467413474281"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p7711145152918"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p7711145152918"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p671116522914"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p671116522914"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1771116572910"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1771116572910"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p144821518175411"><a name="p144821518175411"></a><a name="p144821518175411"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5522164215178"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5522164215178"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5522164215178"></a>input_type</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p8522242101715"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p8522242101715"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p17522114220174"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p17522114220174"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p17522114220174"></a>enum dcmi_port_type</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p0522164231718"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p0522164231718"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p0522164231718"></a>指定网口类型。</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p139261310201818"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p139261310201818"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p139261310201818"></a>enum dcmi_port_type {</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5926810151810"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5926810151810"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5926810151810"></a>DCMI_VNIC_PORT = 0, //虚拟网口</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p4926121041817"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p4926121041817"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p4926121041817"></a>DCMI_ROCE_PORT = 1, //ROCE网口</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p159260105182"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p159260105182"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p159260105182"></a>DCMI_INVALID_PORT</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p169267108184"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p169267108184"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p169267108184"></a>};</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_row1382112338184"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p2082216331186"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p2082216331186"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p2082216331186"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1982217333181"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1982217333181"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1982217333181"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p118221533101811"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p118221533101811"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p118221533101811"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1182253319187"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1182253319187"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1182253319187"></a>指定网口号，保留字段。取值范围：[0, 255]。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_row14113163220181"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1911443271820"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1911443271820"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1911443271820"></a>ip</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p10114432171816"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p10114432171816"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p10114432171816"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p15114123220187"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p15114123220187"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p15114123220187"></a>struct dcmi_ip_addr *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p496716258191"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p496716258191"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p496716258191"></a>IP地址</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p11821111510193"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p11821111510193"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p11821111510193"></a>struct dcmi_ip_addr {</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p178211153196"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p178211153196"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p178211153196"></a>union {</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p168212015141919"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p168212015141919"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p168212015141919"></a>unsigned char ip6[16]; //IPv6地址</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p2821191541917"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p2821191541917"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p2821191541917"></a>unsigned char ip4[4]; //IPv4地址</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p158211715161914"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p158211715161914"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p158211715161914"></a>} u_addr;</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1382110151199"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1382110151199"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1382110151199"></a>enum dcmi_ip_addr_type ip_type;  //IP类型</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p382111158199"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p382111158199"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p382111158199"></a>};</p>
<p id="p1959415205541"><a name="p1959415205541"></a><a name="p1959415205541"></a></p>
<div class="note" id="note06326822712"><a name="note06326822712"></a><a name="note06326822712"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p206320810278"><a name="p206320810278"></a><a name="p206320810278"></a>支持IPv6协议，ip6数组中第一位元素的值为该IP信息的前缀，合法值为0~128，其他元素无意义。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_row18578115413351"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5731143173615"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5731143173615"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p5731143173615"></a>mask</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p7732036361"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p7732036361"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p7732036361"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p107323315363"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p107323315363"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p107323315363"></a>struct dcmi_ip_addr *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1373217317365"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1373217317365"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1373217317365"></a>mask地址</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p137321334364"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p137321334364"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p137321334364"></a>struct dcmi_ip_addr {</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p87320310363"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p87320310363"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p87320310363"></a>union {</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p147321636366"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p147321636366"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p147321636366"></a>unsigned char ip6[16]; //IPv6地址</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p07321437363"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p07321437363"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p07321437363"></a>unsigned char ip4[4]; //IPv4地址</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1473253103611"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1473253103611"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1473253103611"></a>} u_addr;</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1473218314366"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1473218314366"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1473218314366"></a>enum dcmi_ip_addr_type ip_type; //IP类型</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p873243133614"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p873243133614"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p873243133614"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table1113417173519"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1723193692019"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p135791655203618"><a name="zh-cn_topic_0000002485295476_p135791655203618"></a><a name="zh-cn_topic_0000002485295476_p135791655203618"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p386754617313"><a name="zh-cn_topic_0000002485295476_p386754617313"></a><a name="zh-cn_topic_0000002485295476_p386754617313"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p11867346143119"><a name="zh-cn_topic_0000002485295476_p11867346143119"></a><a name="zh-cn_topic_0000002485295476_p11867346143119"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p178671246173119"><a name="zh-cn_topic_0000002485295476_p178671246173119"></a><a name="zh-cn_topic_0000002485295476_p178671246173119"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row187242365207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p11724836152019"><a name="zh-cn_topic_0000002485295476_p11724836152019"></a><a name="zh-cn_topic_0000002485295476_p11724836152019"></a><span id="zh-cn_topic_0000002485295476_ph20724336102017"><a name="zh-cn_topic_0000002485295476_ph20724336102017"></a><a name="zh-cn_topic_0000002485295476_ph20724336102017"></a><span id="zh-cn_topic_0000002485295476_text1672443611202"><a name="zh-cn_topic_0000002485295476_text1672443611202"></a><a name="zh-cn_topic_0000002485295476_text1672443611202"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p6724636152017"><a name="zh-cn_topic_0000002485295476_p6724636152017"></a><a name="zh-cn_topic_0000002485295476_p6724636152017"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p072493613204"><a name="zh-cn_topic_0000002485295476_p072493613204"></a><a name="zh-cn_topic_0000002485295476_p072493613204"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15724113616209"><a name="zh-cn_topic_0000002485295476_p15724113616209"></a><a name="zh-cn_topic_0000002485295476_p15724113616209"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row0724133613207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p172433614201"><a name="zh-cn_topic_0000002485295476_p172433614201"></a><a name="zh-cn_topic_0000002485295476_p172433614201"></a><span id="zh-cn_topic_0000002485295476_text8724103642014"><a name="zh-cn_topic_0000002485295476_text8724103642014"></a><a name="zh-cn_topic_0000002485295476_text8724103642014"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p197241836152010"><a name="zh-cn_topic_0000002485295476_p197241836152010"></a><a name="zh-cn_topic_0000002485295476_p197241836152010"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p19724123672017"><a name="zh-cn_topic_0000002485295476_p19724123672017"></a><a name="zh-cn_topic_0000002485295476_p19724123672017"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1972453612205"><a name="zh-cn_topic_0000002485295476_p1972453612205"></a><a name="zh-cn_topic_0000002485295476_p1972453612205"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row77243362207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1472463611209"><a name="zh-cn_topic_0000002485295476_p1472463611209"></a><a name="zh-cn_topic_0000002485295476_p1472463611209"></a><span id="zh-cn_topic_0000002485295476_text1872453612205"><a name="zh-cn_topic_0000002485295476_text1872453612205"></a><a name="zh-cn_topic_0000002485295476_text1872453612205"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1972433616206"><a name="zh-cn_topic_0000002485295476_p1972433616206"></a><a name="zh-cn_topic_0000002485295476_p1972433616206"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1072423612012"><a name="zh-cn_topic_0000002485295476_p1072423612012"></a><a name="zh-cn_topic_0000002485295476_p1072423612012"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1972463672019"><a name="zh-cn_topic_0000002485295476_p1972463672019"></a><a name="zh-cn_topic_0000002485295476_p1972463672019"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row17724133613204"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p67241036122013"><a name="zh-cn_topic_0000002485295476_p67241036122013"></a><a name="zh-cn_topic_0000002485295476_p67241036122013"></a><span id="zh-cn_topic_0000002485295476_text772414362202"><a name="zh-cn_topic_0000002485295476_text772414362202"></a><a name="zh-cn_topic_0000002485295476_text772414362202"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p772417367200"><a name="zh-cn_topic_0000002485295476_p772417367200"></a><a name="zh-cn_topic_0000002485295476_p772417367200"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p672443632016"><a name="zh-cn_topic_0000002485295476_p672443632016"></a><a name="zh-cn_topic_0000002485295476_p672443632016"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1872417364208"><a name="zh-cn_topic_0000002485295476_p1872417364208"></a><a name="zh-cn_topic_0000002485295476_p1872417364208"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1772493672010"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p272403610201"><a name="zh-cn_topic_0000002485295476_p272403610201"></a><a name="zh-cn_topic_0000002485295476_p272403610201"></a><span id="zh-cn_topic_0000002485295476_text12724153619206"><a name="zh-cn_topic_0000002485295476_text12724153619206"></a><a name="zh-cn_topic_0000002485295476_text12724153619206"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p127241736142014"><a name="zh-cn_topic_0000002485295476_p127241736142014"></a><a name="zh-cn_topic_0000002485295476_p127241736142014"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p5724636162015"><a name="zh-cn_topic_0000002485295476_p5724636162015"></a><a name="zh-cn_topic_0000002485295476_p5724636162015"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p6724143611201"><a name="zh-cn_topic_0000002485295476_p6724143611201"></a><a name="zh-cn_topic_0000002485295476_p6724143611201"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row47241236102018"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1272463632016"><a name="zh-cn_topic_0000002485295476_p1272463632016"></a><a name="zh-cn_topic_0000002485295476_p1272463632016"></a><span id="zh-cn_topic_0000002485295476_text13724123617201"><a name="zh-cn_topic_0000002485295476_text13724123617201"></a><a name="zh-cn_topic_0000002485295476_text13724123617201"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p43171355112010"><a name="zh-cn_topic_0000002485295476_p43171355112010"></a><a name="zh-cn_topic_0000002485295476_p43171355112010"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p0330115562011"><a name="zh-cn_topic_0000002485295476_p0330115562011"></a><a name="zh-cn_topic_0000002485295476_p0330115562011"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p193431559206"><a name="zh-cn_topic_0000002485295476_p193431559206"></a><a name="zh-cn_topic_0000002485295476_p193431559206"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row197254368207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p6725133615205"><a name="zh-cn_topic_0000002485295476_p6725133615205"></a><a name="zh-cn_topic_0000002485295476_p6725133615205"></a><span id="zh-cn_topic_0000002485295476_text17256369202"><a name="zh-cn_topic_0000002485295476_text17256369202"></a><a name="zh-cn_topic_0000002485295476_text17256369202"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1735585510205"><a name="zh-cn_topic_0000002485295476_p1735585510205"></a><a name="zh-cn_topic_0000002485295476_p1735585510205"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p123731555122014"><a name="zh-cn_topic_0000002485295476_p123731555122014"></a><a name="zh-cn_topic_0000002485295476_p123731555122014"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p3391255152012"><a name="zh-cn_topic_0000002485295476_p3391255152012"></a><a name="zh-cn_topic_0000002485295476_p3391255152012"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row15725536162012"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p6725193618208"><a name="zh-cn_topic_0000002485295476_p6725193618208"></a><a name="zh-cn_topic_0000002485295476_p6725193618208"></a><span id="zh-cn_topic_0000002485295476_ph66771792553"><a name="zh-cn_topic_0000002485295476_ph66771792553"></a><a name="zh-cn_topic_0000002485295476_ph66771792553"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
struct dcmi_ip_addr ip_address = {0};
struct dcmi_ip_addr ip_mask_address = {0};
ret = dcmi_get_device_ip(card_id,device_id, DCMI_ROCE_PORT, port_id, &ip_address, &ip_mask_address);
if (ret != 0) {
    //todo:记录日志
    return ret;
}
…
```

