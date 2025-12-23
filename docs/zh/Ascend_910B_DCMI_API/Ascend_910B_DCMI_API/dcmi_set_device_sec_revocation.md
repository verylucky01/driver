# dcmi\_set\_device\_sec\_revocation<a name="ZH-CN_TOPIC_0000002485455442"></a>

**函数原型<a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_toc533412077"></a>**

**int dcmi\_set\_device\_sec\_revocation\(int card\_id, int device\_id, enum dcmi\_revo\_type input\_type, const unsigned char \*file\_data, unsigned int file\_size\)**

**功能说明<a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_toc533412078"></a>**

实现密钥吊销功能。

**参数说明<a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_toc533412079"></a>**

<a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p10021890"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p10021890"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p6466753"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p6466753"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.02%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p54045009"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p54045009"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.96%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p15569626"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p15569626"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p36741947142813"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p36741947142813"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p96741747122818"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p96741747122818"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.02%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p46747472287"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p46747472287"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.96%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1467413474281"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1467413474281"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p7711145152918"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p7711145152918"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p671116522914"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p671116522914"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.02%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1771116572910"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1771116572910"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.96%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p12383253154418"><a name="p12383253154418"></a><a name="p12383253154418"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p5522164215178"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p5522164215178"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p5522164215178"></a>input_type</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p8522242101715"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p8522242101715"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.02%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p17522114220174"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p17522114220174"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p17522114220174"></a>enum dcmi_revo_type</p>
</td>
<td class="cellrowborder" valign="top" width="48.96%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p0522164231718"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p0522164231718"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p0522164231718"></a>吊销类型。</p>
<p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1014115454529"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1014115454529"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1014115454529"></a>enum dcmi_revo_type {</p>
<p id="p212016451686"><a name="p212016451686"></a><a name="p212016451686"></a>DCMI_REVOCATION_TYPE_SOC = 0,  //用于吊销SOC密钥</p>
<p id="p181202459812"><a name="p181202459812"></a><a name="p181202459812"></a>DCMI_REVOCATION_TYPE_CMS_CRL = 1,  //用于MDC CMS CRL文件升级</p>
<p id="p339720234533"><a name="p339720234533"></a><a name="p339720234533"></a>DCMI_REVOCATION_TYPE_CMS_CRL_EXT = 2, //用于扩展CRL文件升级</p>
<p id="p75053511085"><a name="p75053511085"></a><a name="p75053511085"></a>DCMI_REVOCATION_TYPE_MAX</p>
<p id="p1227717173228"><a name="p1227717173228"></a><a name="p1227717173228"></a>};</p>
<p id="p171201345786"><a name="p171201345786"></a><a name="p171201345786"></a>当前仅支持DCMI_REVOCATION_TYPE_SOC。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_row46671240105418"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p14667114012547"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p14667114012547"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p14667114012547"></a>file_data</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p7668154095417"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p7668154095417"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p7668154095417"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.02%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p566818401548"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p566818401548"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p566818401548"></a>const unsigned char *</p>
</td>
<td class="cellrowborder" valign="top" width="48.96%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1366819403540"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1366819403540"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p1366819403540"></a>吊销文件的数据地址。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_row1306946165516"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p16307146105520"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p16307146105520"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p16307146105520"></a>file_size</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p830764614555"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p830764614555"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p830764614555"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.02%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p16307746155515"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p16307746155515"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p16307746155515"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.96%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p630704645512"><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p630704645512"></a><a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_p630704645512"></a>吊销文件的数据长度，SOC二级密钥吊销操作的文件长度固定为544Byte。</p>
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

**约束说明<a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_toc533412082"></a>**

-   密钥吊销操作是不可逆的过程，吊销操作执行成功后，无法再进行恢复，需要谨慎使用。
-   该接口需调入TEEOS，耗时可能较长，不支持在接口调用时触发休眠唤醒，如果触发休眠，有较大可能造成休眠失败。
-   对于SMP系统，在执行吊销操作前必须先获取设备个数，然后对所有的设备均执行吊销操作。
-   该接口在确定需要进行对应的吊销操作时才可以调用，并且需要正确的吊销文件才可以吊销成功，否则，调用该接口返回失败。
-   执行吊销操作成功后，设备不可用。

**表 1** 不同部署场景下的支持情况

<a name="table6665182042413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row192401338610"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p6884135713319"><a name="zh-cn_topic_0000002485295476_p6884135713319"></a><a name="zh-cn_topic_0000002485295476_p6884135713319"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p188841657113119"><a name="zh-cn_topic_0000002485295476_p188841657113119"></a><a name="zh-cn_topic_0000002485295476_p188841657113119"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p198849575317"><a name="zh-cn_topic_0000002485295476_p198849575317"></a><a name="zh-cn_topic_0000002485295476_p198849575317"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p288445716317"><a name="zh-cn_topic_0000002485295476_p288445716317"></a><a name="zh-cn_topic_0000002485295476_p288445716317"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_zh-cn_topic_0000001167913765_row82952324359"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1759118207718"><a name="zh-cn_topic_0000002485295476_p1759118207718"></a><a name="zh-cn_topic_0000002485295476_p1759118207718"></a><span id="zh-cn_topic_0000002485295476_ph05911020372"><a name="zh-cn_topic_0000002485295476_ph05911020372"></a><a name="zh-cn_topic_0000002485295476_ph05911020372"></a><span id="zh-cn_topic_0000002485295476_text12591192010713"><a name="zh-cn_topic_0000002485295476_text12591192010713"></a><a name="zh-cn_topic_0000002485295476_text12591192010713"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1018612250597"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1018612250597"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p1018612250597"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p16903175117312"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p16903175117312"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p16903175117312"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p89579531835"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p89579531835"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001251427187_zh-cn_topic_0000001188446388_p89579531835"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row72645420615"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p95911420177"><a name="zh-cn_topic_0000002485295476_p95911420177"></a><a name="zh-cn_topic_0000002485295476_p95911420177"></a><span id="zh-cn_topic_0000002485295476_text6591220876"><a name="zh-cn_topic_0000002485295476_text6591220876"></a><a name="zh-cn_topic_0000002485295476_text6591220876"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p291810521262"><a name="zh-cn_topic_0000002485295476_p291810521262"></a><a name="zh-cn_topic_0000002485295476_p291810521262"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p199181852061"><a name="zh-cn_topic_0000002485295476_p199181852061"></a><a name="zh-cn_topic_0000002485295476_p199181852061"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p89189521765"><a name="zh-cn_topic_0000002485295476_p89189521765"></a><a name="zh-cn_topic_0000002485295476_p89189521765"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row278413126616"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p559162010720"><a name="zh-cn_topic_0000002485295476_p559162010720"></a><a name="zh-cn_topic_0000002485295476_p559162010720"></a><span id="zh-cn_topic_0000002485295476_text165912204716"><a name="zh-cn_topic_0000002485295476_text165912204716"></a><a name="zh-cn_topic_0000002485295476_text165912204716"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1852110531661"><a name="zh-cn_topic_0000002485295476_p1852110531661"></a><a name="zh-cn_topic_0000002485295476_p1852110531661"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1252125311612"><a name="zh-cn_topic_0000002485295476_p1252125311612"></a><a name="zh-cn_topic_0000002485295476_p1252125311612"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p652117531767"><a name="zh-cn_topic_0000002485295476_p652117531767"></a><a name="zh-cn_topic_0000002485295476_p652117531767"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row878911101267"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p259119201579"><a name="zh-cn_topic_0000002485295476_p259119201579"></a><a name="zh-cn_topic_0000002485295476_p259119201579"></a><span id="zh-cn_topic_0000002485295476_text55915207713"><a name="zh-cn_topic_0000002485295476_text55915207713"></a><a name="zh-cn_topic_0000002485295476_text55915207713"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p18619541161"><a name="zh-cn_topic_0000002485295476_p18619541161"></a><a name="zh-cn_topic_0000002485295476_p18619541161"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p2864541766"><a name="zh-cn_topic_0000002485295476_p2864541766"></a><a name="zh-cn_topic_0000002485295476_p2864541766"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1186854962"><a name="zh-cn_topic_0000002485295476_p1186854962"></a><a name="zh-cn_topic_0000002485295476_p1186854962"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row283215811614"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p12591202014711"><a name="zh-cn_topic_0000002485295476_p12591202014711"></a><a name="zh-cn_topic_0000002485295476_p12591202014711"></a><span id="zh-cn_topic_0000002485295476_text65911120871"><a name="zh-cn_topic_0000002485295476_text65911120871"></a><a name="zh-cn_topic_0000002485295476_text65911120871"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p15638154267"><a name="zh-cn_topic_0000002485295476_p15638154267"></a><a name="zh-cn_topic_0000002485295476_p15638154267"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p18638354369"><a name="zh-cn_topic_0000002485295476_p18638354369"></a><a name="zh-cn_topic_0000002485295476_p18638354369"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p16638154060"><a name="zh-cn_topic_0000002485295476_p16638154060"></a><a name="zh-cn_topic_0000002485295476_p16638154060"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1057696667"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p959110201477"><a name="zh-cn_topic_0000002485295476_p959110201477"></a><a name="zh-cn_topic_0000002485295476_p959110201477"></a><span id="zh-cn_topic_0000002485295476_text35912020471"><a name="zh-cn_topic_0000002485295476_text35912020471"></a><a name="zh-cn_topic_0000002485295476_text35912020471"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p18230255869"><a name="zh-cn_topic_0000002485295476_p18230255869"></a><a name="zh-cn_topic_0000002485295476_p18230255869"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p123055512620"><a name="zh-cn_topic_0000002485295476_p123055512620"></a><a name="zh-cn_topic_0000002485295476_p123055512620"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p623011552618"><a name="zh-cn_topic_0000002485295476_p623011552618"></a><a name="zh-cn_topic_0000002485295476_p623011552618"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row8655214617"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p14591202016712"><a name="zh-cn_topic_0000002485295476_p14591202016712"></a><a name="zh-cn_topic_0000002485295476_p14591202016712"></a><span id="zh-cn_topic_0000002485295476_text1659110201379"><a name="zh-cn_topic_0000002485295476_text1659110201379"></a><a name="zh-cn_topic_0000002485295476_text1659110201379"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p38158552616"><a name="zh-cn_topic_0000002485295476_p38158552616"></a><a name="zh-cn_topic_0000002485295476_p38158552616"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p18158552613"><a name="zh-cn_topic_0000002485295476_p18158552613"></a><a name="zh-cn_topic_0000002485295476_p18158552613"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p19815255367"><a name="zh-cn_topic_0000002485295476_p19815255367"></a><a name="zh-cn_topic_0000002485295476_p19815255367"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row67064433311"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15712048183315"><a name="zh-cn_topic_0000002485295476_p15712048183315"></a><a name="zh-cn_topic_0000002485295476_p15712048183315"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251307177_zh-cn_topic_0000001178373148_zh-cn_topic_0000001101604520_toc533412083"></a>**

```
...
#define REVOCATION_FILE_LEN  544
int card_id = 0;
int dev_id = 0;
int ret = 0;
int dev_count = 0;
unsigned char revocation_file_buf[REVOCATION_FILE_LEN] = {0};
unsigned int buf_size = REVOCATION_FILE_LEN;
ret = dcmi_set_device_sec_revocation(card_id, dev_id, DCMI_REVOCATION_TYPE_SOC, (const unsigned char *)revocation_file_buf, buf_size);
if (ret != 0){
    // todo:记录日志
    return ret;
}
...
```

