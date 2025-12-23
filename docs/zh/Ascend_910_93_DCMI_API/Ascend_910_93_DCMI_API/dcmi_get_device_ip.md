# dcmi\_get\_device\_ip<a name="ZH-CN_TOPIC_0000002517638729"></a>

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
<p id="p9841174072119"><a name="p9841174072119"></a><a name="p9841174072119"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
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
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1382110151199"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1382110151199"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p1382110151199"></a>enum dcmi_ip_addr_type ip_type; //IP类型</p>
<p id="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p382111158199"><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p382111158199"></a><a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_p382111158199"></a>};</p>
<p id="p719284332113"><a name="p719284332113"></a><a name="p719284332113"></a></p>
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

<a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row59025204"><th class="cellrowborder" valign="top" width="24.18%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p16312252"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="75.82%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p46224020"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_row13362997"><td class="cellrowborder" valign="top" width="24.18%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p8661001"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="75.82%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_p79561110476"></a>处理结果：</p>
<a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><a name="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"></a><ul id="zh-cn_topic_0000002485478734_zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_zh-cn_topic_0160151812_ul55711364478"><li>成功：返回0。</li><li>失败：返回码请参见<a href="return_codes.md">return_codes</a>。</li></ul>
</td>
</tr>
</tbody>
</table>

**异常处理<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_toc533412081"></a>**

无。

**约束说明<a name="zh-cn_topic_0000001206147242_zh-cn_topic_0000001223414449_zh-cn_topic_0000001148173007_toc533412082"></a>**

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

