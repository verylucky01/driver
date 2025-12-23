# dcmi\_clear\_device\_user\_config<a name="ZH-CN_TOPIC_0000002485318750"></a>

**函数原型<a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_toc533412077"></a>**

**int dcmi\_clear\_device\_user\_config\(int card\_id, int device\_id, const char \*config\_name\)**

**功能说明<a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_toc533412078"></a>**

重置用户配置。

默认值请参见[dcmi\_set\_device\_user\_config](dcmi_set_device_user_config.md)。

**参数说明<a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_toc533412079"></a>**

<a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p10021890"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p10021890"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p6466753"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p6466753"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="18%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p54045009"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p54045009"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p15569626"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p15569626"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p36741947142813"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p36741947142813"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p96741747122818"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p96741747122818"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p46747472287"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p46747472287"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1467413474281"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1467413474281"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p7711145152918"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p7711145152918"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p671116522914"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p671116522914"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1771116572910"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1771116572910"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1380131818222"><a name="p1380131818222"></a><a name="p1380131818222"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_row6326181581618"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p146993253912"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p146993253912"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p146993253912"></a>config_name</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p469916256914"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p469916256914"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p469916256914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1633516491298"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1633516491298"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1633516491298"></a>const char *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p182531815143013"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p182531815143013"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p182531815143013"></a>目前支持处理的配置项名称如下，配置项名称的字符串长度最大为32。</p>
<p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1795311111557"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1795311111557"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1795311111557"></a>已实现功能的配置项：mac_info。支持用户自定义名称配置。</p>
<p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p189533165513"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p189533165513"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p189533165513"></a>配置项功能说明如下：</p>
<p id="p51008561557"><a name="p51008561557"></a><a name="p51008561557"></a>"mac_info" ：用于设置MAC地址。</p>
<p id="p179441619152212"><a name="p179441619152212"></a><a name="p179441619152212"></a></p>
<div class="note" id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_zh-cn_topic_0204328569_note1462701516448"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_zh-cn_topic_0204328569_note1462701516448"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_zh-cn_topic_0204328569_note1462701516448"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1096212366311"><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1096212366311"></a><a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_p1096212366311"></a>调用该接口配置标记后，需要复位芯片，配置才能生效。配置生效后，可通过<a href="dcmi_get_user_config.md">dcmi_get_user_config</a>接口查看配置结果。</p>
</div></div>
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

**约束说明<a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_toc533412082"></a>**

该接口设置的内容会涉及Flash擦写，由于Flash擦写次数是有限制的，所以不建议频繁调用该接口。

该接口需调入TEEOS，耗时较长，不支持在接口调用时触发休眠唤醒，如果触发休眠，有较大可能造成休眠失败。

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

**调用示例<a name="zh-cn_topic_0000001206627180_zh-cn_topic_0000001178054664_zh-cn_topic_0000001102358324_toc533412083"></a>**

```
…  
int ret = 0;
int card_id = 0;
int device_id = 0;
const char *config_name = "mac_info";
ret = dcmi_clear_device_user_config(card_id, device_id, config_name);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

