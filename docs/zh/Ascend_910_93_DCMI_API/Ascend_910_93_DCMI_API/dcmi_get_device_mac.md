# dcmi\_get\_device\_mac<a name="ZH-CN_TOPIC_0000002517558681"></a>

**函数原型<a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_toc533412077"></a>**

**int dcmi\_get\_device\_mac\(int card\_id, int device\_id, int mac\_id, char \*mac\_addr, unsigned int len\)**

**功能说明<a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_toc533412078"></a>**

获取指定设备的MAC地址。

**参数说明<a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_toc533412079"></a>**

<a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p10021890"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p10021890"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p6466753"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p6466753"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p54045009"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p54045009"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p15569626"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p15569626"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p36741947142813"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p36741947142813"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p96741747122818"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p96741747122818"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p46747472287"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p46747472287"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1467413474281"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1467413474281"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p7711145152918"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p7711145152918"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p671116522914"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p671116522914"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1771116572910"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1771116572910"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p640953122114"><a name="p640953122114"></a><a name="p640953122114"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p204764516614"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p204764516614"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p204764516614"></a>mac_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p2471645764"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p2471645764"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p2471645764"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p114784516618"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p114784516618"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p114784516618"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p62541647414"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p62541647414"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p62541647414"></a>取值范围：0~3。</p>
<p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p6254741247"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p6254741247"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p6254741247"></a>该参数在当前版本不使用。默认值为0，请保持默认值即可。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_row592615412120"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p520011315116"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p520011315116"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p520011315116"></a>mac_addr</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1820016131118"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1820016131118"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p1820016131118"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p11200121321115"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p11200121321115"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p11200121321115"></a>char *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p5314104918611"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p5314104918611"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p5314104918611"></a>输出6个Byte的MAC地址。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_row94185112109"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p0139022673"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p0139022673"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p0139022673"></a>len</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p4140182218713"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p4140182218713"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p4140182218713"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p81401822173"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p81401822173"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p81401822173"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p214082212717"><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p214082212717"></a><a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_p214082212717"></a>MAC地址长度，固定长度6，单位byte。</p>
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

**约束说明<a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row913516862011"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p15076476392"><a name="zh-cn_topic_0000002485318818_p15076476392"></a><a name="zh-cn_topic_0000002485318818_p15076476392"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p16507547153913"><a name="zh-cn_topic_0000002485318818_p16507547153913"></a><a name="zh-cn_topic_0000002485318818_p16507547153913"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p1950718474398"><a name="zh-cn_topic_0000002485318818_p1950718474398"></a><a name="zh-cn_topic_0000002485318818_p1950718474398"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p165071047113911"><a name="zh-cn_topic_0000002485318818_p165071047113911"></a><a name="zh-cn_topic_0000002485318818_p165071047113911"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row31351488206"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p6135185208"><a name="zh-cn_topic_0000002485318818_p6135185208"></a><a name="zh-cn_topic_0000002485318818_p6135185208"></a><span id="zh-cn_topic_0000002485318818_text1213528172016"><a name="zh-cn_topic_0000002485318818_text1213528172016"></a><a name="zh-cn_topic_0000002485318818_text1213528172016"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1213511813205"><a name="zh-cn_topic_0000002485318818_p1213511813205"></a><a name="zh-cn_topic_0000002485318818_p1213511813205"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p613528102020"><a name="zh-cn_topic_0000002485318818_p613528102020"></a><a name="zh-cn_topic_0000002485318818_p613528102020"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p713512862013"><a name="zh-cn_topic_0000002485318818_p713512862013"></a><a name="zh-cn_topic_0000002485318818_p713512862013"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1813515811208"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p71354810207"><a name="zh-cn_topic_0000002485318818_p71354810207"></a><a name="zh-cn_topic_0000002485318818_p71354810207"></a><span id="zh-cn_topic_0000002485318818_text51358812207"><a name="zh-cn_topic_0000002485318818_text51358812207"></a><a name="zh-cn_topic_0000002485318818_text51358812207"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p213568152019"><a name="zh-cn_topic_0000002485318818_p213568152019"></a><a name="zh-cn_topic_0000002485318818_p213568152019"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1613548142019"><a name="zh-cn_topic_0000002485318818_p1613548142019"></a><a name="zh-cn_topic_0000002485318818_p1613548142019"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p11351087209"><a name="zh-cn_topic_0000002485318818_p11351087209"></a><a name="zh-cn_topic_0000002485318818_p11351087209"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row87611016195410"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p8296259161515"><a name="zh-cn_topic_0000002485318818_p8296259161515"></a><a name="zh-cn_topic_0000002485318818_p8296259161515"></a><span id="zh-cn_topic_0000002485318818_text14296259131514"><a name="zh-cn_topic_0000002485318818_text14296259131514"></a><a name="zh-cn_topic_0000002485318818_text14296259131514"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1451920314542"><a name="zh-cn_topic_0000002485318818_p1451920314542"></a><a name="zh-cn_topic_0000002485318818_p1451920314542"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p2519133155413"><a name="zh-cn_topic_0000002485318818_p2519133155413"></a><a name="zh-cn_topic_0000002485318818_p2519133155413"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p11519153110545"><a name="zh-cn_topic_0000002485318818_p11519153110545"></a><a name="zh-cn_topic_0000002485318818_p11519153110545"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row0323633152810"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p064011372281"><a name="zh-cn_topic_0000002485318818_p064011372281"></a><a name="zh-cn_topic_0000002485318818_p064011372281"></a><span id="zh-cn_topic_0000002485318818_text18640537162811"><a name="zh-cn_topic_0000002485318818_text18640537162811"></a><a name="zh-cn_topic_0000002485318818_text18640537162811"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p279104012287"><a name="zh-cn_topic_0000002485318818_p279104012287"></a><a name="zh-cn_topic_0000002485318818_p279104012287"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p779112409289"><a name="zh-cn_topic_0000002485318818_p779112409289"></a><a name="zh-cn_topic_0000002485318818_p779112409289"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1791140112816"><a name="zh-cn_topic_0000002485318818_p1791140112816"></a><a name="zh-cn_topic_0000002485318818_p1791140112816"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row49081418125411"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p512952510545"><a name="zh-cn_topic_0000002485318818_p512952510545"></a><a name="zh-cn_topic_0000002485318818_p512952510545"></a><span id="zh-cn_topic_0000002485318818_text012942512545"><a name="zh-cn_topic_0000002485318818_text012942512545"></a><a name="zh-cn_topic_0000002485318818_text012942512545"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1260932135413"><a name="zh-cn_topic_0000002485318818_p1260932135413"></a><a name="zh-cn_topic_0000002485318818_p1260932135413"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1660133285419"><a name="zh-cn_topic_0000002485318818_p1660133285419"></a><a name="zh-cn_topic_0000002485318818_p1660133285419"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p46017321545"><a name="zh-cn_topic_0000002485318818_p46017321545"></a><a name="zh-cn_topic_0000002485318818_p46017321545"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1625111184"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1386118141085"><a name="zh-cn_topic_0000002485318818_p1386118141085"></a><a name="zh-cn_topic_0000002485318818_p1386118141085"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206147254_zh-cn_topic_0000001223292885_zh-cn_topic_0000001101364542_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
char mac_addr[6] = {0};
ret = dcmi_get_device_mac(card_id, device_id, 0, mac_addr, 6);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

