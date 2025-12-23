# dcmi\_set\_device\_gateway<a name="ZH-CN_TOPIC_0000002485318714"></a>

**函数原型<a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_toc533412077"></a>**

**int dcmi\_set\_device\_gateway\(int card\_id, int device\_id, enum dcmi\_port\_type input\_type, int port\_id, struct dcmi\_ip\_addr \*gateway\)**

**功能说明<a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_toc533412078"></a>**

设置网关地址。

**参数说明<a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p10021890"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p10021890"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p6466753"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p6466753"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p54045009"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p54045009"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p15569626"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p15569626"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p36741947142813"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p36741947142813"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p96741747122818"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p96741747122818"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p46747472287"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p46747472287"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1467413474281"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1467413474281"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p7711145152918"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p7711145152918"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p671116522914"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p671116522914"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1771116572910"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1771116572910"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p5640437122110"><a name="p5640437122110"></a><a name="p5640437122110"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p5522164215178"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p5522164215178"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p5522164215178"></a>input_type</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p8522242101715"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p8522242101715"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p17522114220174"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p17522114220174"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p17522114220174"></a>enum dcmi_port_type</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p0522164231718"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p0522164231718"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p0522164231718"></a>指定网口类型。</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p139261310201818"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p139261310201818"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p139261310201818"></a>enum dcmi_port_type {</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p5926810151810"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p5926810151810"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p5926810151810"></a>DCMI_VNIC_PORT = 0, //虚拟网口</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p4926121041817"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p4926121041817"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p4926121041817"></a>DCMI_ROCE_PORT = 1, //ROCE网口</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p159260105182"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p159260105182"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p159260105182"></a>DCMI_INVALID_PORT</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p169267108184"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p169267108184"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p169267108184"></a>};</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p8907162231913"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p8907162231913"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p8907162231913"></a>不支持DCMI_VNIC_PORT。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_row1382112338184"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p2082216331186"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p2082216331186"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p2082216331186"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1982217333181"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1982217333181"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1982217333181"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p118221533101811"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p118221533101811"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p118221533101811"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1182253319187"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1182253319187"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1182253319187"></a>指定网口号，保留字段。取值范围：[0, 255]。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_row14113163220181"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1911443271820"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1911443271820"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1911443271820"></a>gateway</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p10114432171816"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p10114432171816"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p10114432171816"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p15114123220187"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p15114123220187"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p15114123220187"></a>struct dcmi_ip_addr *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p496716258191"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p496716258191"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p496716258191"></a>网关地址</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p11821111510193"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p11821111510193"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p11821111510193"></a>struct dcmi_ip_addr {</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p178211153196"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p178211153196"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p178211153196"></a>union {</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p168212015141919"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p168212015141919"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p168212015141919"></a>unsigned char ip6[16]; //IPv6地址</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p2821191541917"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p2821191541917"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p2821191541917"></a>unsigned char ip4[4]; //IPv4地址</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p158211715161914"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p158211715161914"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p158211715161914"></a>} u_addr;</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1382110151199"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1382110151199"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p1382110151199"></a>enum dcmi_ip_addr_type ip_type; //IP类型</p>
<p id="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p382111158199"><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p382111158199"></a><a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_p382111158199"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row163024263717"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p1041510392"><a name="zh-cn_topic_0000002485318818_p1041510392"></a><a name="zh-cn_topic_0000002485318818_p1041510392"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p10417512392"><a name="zh-cn_topic_0000002485318818_p10417512392"></a><a name="zh-cn_topic_0000002485318818_p10417512392"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p1341851173915"><a name="zh-cn_topic_0000002485318818_p1341851173915"></a><a name="zh-cn_topic_0000002485318818_p1341851173915"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p3435114394"><a name="zh-cn_topic_0000002485318818_p3435114394"></a><a name="zh-cn_topic_0000002485318818_p3435114394"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row1030104243717"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1830184219376"><a name="zh-cn_topic_0000002485318818_p1830184219376"></a><a name="zh-cn_topic_0000002485318818_p1830184219376"></a><span id="zh-cn_topic_0000002485318818_text83017424375"><a name="zh-cn_topic_0000002485318818_text83017424375"></a><a name="zh-cn_topic_0000002485318818_text83017424375"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p130242163712"><a name="zh-cn_topic_0000002485318818_p130242163712"></a><a name="zh-cn_topic_0000002485318818_p130242163712"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p9487578148"><a name="zh-cn_topic_0000002485318818_p9487578148"></a><a name="zh-cn_topic_0000002485318818_p9487578148"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1026715119156"><a name="zh-cn_topic_0000002485318818_p1026715119156"></a><a name="zh-cn_topic_0000002485318818_p1026715119156"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row10308422379"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p12301642143712"><a name="zh-cn_topic_0000002485318818_p12301642143712"></a><a name="zh-cn_topic_0000002485318818_p12301642143712"></a><span id="zh-cn_topic_0000002485318818_text1930114213371"><a name="zh-cn_topic_0000002485318818_text1930114213371"></a><a name="zh-cn_topic_0000002485318818_text1930114213371"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p03034253714"><a name="zh-cn_topic_0000002485318818_p03034253714"></a><a name="zh-cn_topic_0000002485318818_p03034253714"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p6481457181412"><a name="zh-cn_topic_0000002485318818_p6481457181412"></a><a name="zh-cn_topic_0000002485318818_p6481457181412"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p32683118153"><a name="zh-cn_topic_0000002485318818_p32683118153"></a><a name="zh-cn_topic_0000002485318818_p32683118153"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row14661158135410"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p23113818160"><a name="zh-cn_topic_0000002485318818_p23113818160"></a><a name="zh-cn_topic_0000002485318818_p23113818160"></a><span id="zh-cn_topic_0000002485318818_text12311789168"><a name="zh-cn_topic_0000002485318818_text12311789168"></a><a name="zh-cn_topic_0000002485318818_text12311789168"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p653191095518"><a name="zh-cn_topic_0000002485318818_p653191095518"></a><a name="zh-cn_topic_0000002485318818_p653191095518"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p18531101018557"><a name="zh-cn_topic_0000002485318818_p18531101018557"></a><a name="zh-cn_topic_0000002485318818_p18531101018557"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p85319108552"><a name="zh-cn_topic_0000002485318818_p85319108552"></a><a name="zh-cn_topic_0000002485318818_p85319108552"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row19188145615284"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p111821034299"><a name="zh-cn_topic_0000002485318818_p111821034299"></a><a name="zh-cn_topic_0000002485318818_p111821034299"></a><span id="zh-cn_topic_0000002485318818_text11821030299"><a name="zh-cn_topic_0000002485318818_text11821030299"></a><a name="zh-cn_topic_0000002485318818_text11821030299"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1365116614298"><a name="zh-cn_topic_0000002485318818_p1365116614298"></a><a name="zh-cn_topic_0000002485318818_p1365116614298"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p10651186172913"><a name="zh-cn_topic_0000002485318818_p10651186172913"></a><a name="zh-cn_topic_0000002485318818_p10651186172913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p14651176142917"><a name="zh-cn_topic_0000002485318818_p14651176142917"></a><a name="zh-cn_topic_0000002485318818_p14651176142917"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1472160165512"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p134575595519"><a name="zh-cn_topic_0000002485318818_p134575595519"></a><a name="zh-cn_topic_0000002485318818_p134575595519"></a><span id="zh-cn_topic_0000002485318818_text5457657553"><a name="zh-cn_topic_0000002485318818_text5457657553"></a><a name="zh-cn_topic_0000002485318818_text5457657553"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p71421111559"><a name="zh-cn_topic_0000002485318818_p71421111559"></a><a name="zh-cn_topic_0000002485318818_p71421111559"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p8141511165511"><a name="zh-cn_topic_0000002485318818_p8141511165511"></a><a name="zh-cn_topic_0000002485318818_p8141511165511"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1914171165517"><a name="zh-cn_topic_0000002485318818_p1914171165517"></a><a name="zh-cn_topic_0000002485318818_p1914171165517"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1983816248811"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1172291987"><a name="zh-cn_topic_0000002485318818_p1172291987"></a><a name="zh-cn_topic_0000002485318818_p1172291987"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206467184_zh-cn_topic_0000001223172937_zh-cn_topic_0000001148292021_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int port_id = 0;
unsigned int gateway_address = 0xC801A8C0; //192.168.1.200（仅用作示例，以实际配置网关地址为准）
struct dcmi_ip_addr ip_gateway_address = {0};
memcpy(&(ip_gateway_address.u_addr.ip4[0]),&gateway_address,4);
ret = dcmi_set_device_gateway(card_id,device_id, DCMI_ROCE_PORT, port_id, &ip_gateway_address);
if (ret != 0) {
    //todo:记录日志 
    return ret; 
}
…
```

