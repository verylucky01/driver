# dcmi\_get\_device\_errorcode\_string<a name="ZH-CN_TOPIC_0000002517638695"></a>

**函数原型<a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_toc533412077"></a>**

**int dcmi\_get\_device\_errorcode\_string\(int card\_id, int device\_id, unsigned int error\_code, unsigned char \*error\_info, int buf\_size\)**

**功能说明<a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_toc533412078"></a>**

查询设备故障描述。

**参数说明<a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_toc533412079"></a>**

<a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_row7580267"><th class="cellrowborder" valign="top" width="17.000000000000004%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p10021890"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p10021890"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.990000000000002%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p6466753"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p6466753"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="15.960000000000003%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p54045009"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p54045009"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.050000000000004%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p15569626"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p15569626"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_row10560021192510"><td class="cellrowborder" valign="top" width="17.000000000000004%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p36741947142813"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p36741947142813"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.990000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p96741747122818"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p96741747122818"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.960000000000003%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p46747472287"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p46747472287"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.050000000000004%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1467413474281"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1467413474281"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_row1155711494235"><td class="cellrowborder" valign="top" width="17.000000000000004%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p7711145152918"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p7711145152918"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.990000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p671116522914"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p671116522914"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.960000000000003%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1771116572910"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1771116572910"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.050000000000004%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p1377514432141"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p147768432143"><a name="zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p147768432143"></a><a name="zh-cn_topic_0000001147723703_zh-cn_topic_0000001148530297_p147768432143"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="zh-cn_topic_0000001147723703_p46986266814"><a name="zh-cn_topic_0000001147723703_p46986266814"></a><a name="zh-cn_topic_0000001147723703_p46986266814"></a>MCU芯片：mcu_id。</p>
<p id="p1611512213317"><a name="p1611512213317"></a><a name="p1611512213317"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_row15462171542913"><td class="cellrowborder" valign="top" width="17.000000000000004%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p72389712563"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p72389712563"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p72389712563"></a>error_code</p>
</td>
<td class="cellrowborder" valign="top" width="16.990000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p112389725620"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p112389725620"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p112389725620"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.960000000000003%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p8238871561"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p8238871561"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p8238871561"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50.050000000000004%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p162392075566"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p162392075566"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p162392075566"></a>要查询的错误码，通过<a href="dcmi_get_device_errorcode_v2.md">dcmi_get_device_errorcode_v2</a>接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_row194331899543"><td class="cellrowborder" valign="top" width="17.000000000000004%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p523913745616"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p523913745616"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p523913745616"></a>error_info</p>
</td>
<td class="cellrowborder" valign="top" width="16.990000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1323919719563"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1323919719563"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p1323919719563"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="15.960000000000003%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p112391711565"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p112391711565"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p112391711565"></a>unsigned char *</p>
</td>
<td class="cellrowborder" valign="top" width="50.050000000000004%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p172395717564"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p172395717564"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p172395717564"></a>对应的错误描述。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_row769671118543"><td class="cellrowborder" valign="top" width="17.000000000000004%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p12239772564"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p12239772564"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p12239772564"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="16.990000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p2239476561"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p2239476561"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p2239476561"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.960000000000003%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p6239157175610"><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p6239157175610"></a><a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_p6239157175610"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.050000000000004%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001178373142_p1661902431918"><a name="zh-cn_topic_0000001178373142_p1661902431918"></a><a name="zh-cn_topic_0000001178373142_p1661902431918"></a>传入的error_info取值范围是大于等于48Byte。</p>
<a name="zh-cn_topic_0000001178373142_ul1060910207250"></a><a name="zh-cn_topic_0000001178373142_ul1060910207250"></a><ul id="zh-cn_topic_0000001178373142_ul1060910207250"><li>若设置的error_info小于48Byte，则系统报错。</li><li>若设置的error_info在48~255Byte之间，则在《健康管理故障定义》中的故障码，查询出来的故障信息为简化信息。</li><li>若设置的error_info大于等于256Byte，则查询出来的故障信息为实际故障信息。</li></ul>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**约束说明<a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_toc533412082"></a>**

调用该接口查询到的信息仅代表当前芯片设计了这种错误码类型，具有上报这种故障类型的能力，但不代表当前已经使用这个错误码。当前芯片已经支持的错误码请按照[参数说明](#zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_toc533412079)表中error\_code参数的描述获取。

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

**调用示例<a name="zh-cn_topic_0000001178373142_zh-cn_topic_0000001147610857_toc533412083"></a>**

```
#define ERROR_CODE_MAX_NUM  (128)
#define BUF_SIZE          (256)
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int errorcount = 0;
unsigned int error_code_list[ERROR_CODE_MAX_NUM] = {0};
unsigned char error_info[BUF_SIZE] = {0};
ret = dcmi_get_device_errorcode_v2(card_id, device_id, &errorcount, error_code_list, ERROR_CODE_MAX_NUM);
if ((ret != 0) || (errorcount == 0)){
    //todo:记录日志
    return ret;
} 
ret = dcmi_get_device_errorcode_string(card_id, device_id, error_code_list[0], error_info, BUF_SIZE);
if (ret != 0) { 
    //todo:记录日志
    return ret;
}
…
```

