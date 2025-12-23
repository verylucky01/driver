# dcmi\_get\_device\_compatibility<a name="ZH-CN_TOPIC_0000002485478732"></a>

**函数原型<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412077"></a>**

**int dcmi\_get\_device\_compatibility \(int card\_id, int device\_id, enum dcmi\_device\_compat \*compatibility\)**

**功能说明<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412078"></a>**

查询NPU芯片驱动与固件兼容性。

**参数说明<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412079"></a>**

<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p10021890"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p10021890"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.99%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p6466753"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p6466753"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.009999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p54045009"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p54045009"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p15569626"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p15569626"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_row5908907"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p36741947142813"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p36741947142813"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p96741747122818"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p96741747122818"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p46747472287"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p46747472287"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p1467413474281"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p1467413474281"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_row45631627"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p7711145152918"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p7711145152918"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p671116522914"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p671116522914"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p1771116572910"><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p1771116572910"></a><a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p37761235101814"><a name="p37761235101814"></a><a name="p37761235101814"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row427910719196"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p753616536191"><a name="p753616536191"></a><a name="p753616536191"></a>compatibility</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="p45363532194"><a name="p45363532194"></a><a name="p45363532194"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="p453695381911"><a name="p453695381911"></a><a name="p453695381911"></a>enum dcmi_device_compat *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p14536195311912"><a name="p14536195311912"></a><a name="p14536195311912"></a>enum dcmi_device_compat{</p>
<p id="p115365533199"><a name="p115365533199"></a><a name="p115365533199"></a>DCMI_COMPAT_OK = 1,</p>
<p id="p453625319193"><a name="p453625319193"></a><a name="p453625319193"></a>DCMI_COMPAT_NOK = 2,</p>
<p id="p145361553101913"><a name="p145361553101913"></a><a name="p145361553101913"></a>DCMI_COMPAT_UNKNOWN = 3</p>
<p id="p195366537192"><a name="p195366537192"></a><a name="p195366537192"></a>};</p>
<p id="p5536195314195"><a name="p5536195314195"></a><a name="p5536195314195"></a>输出说明如下：</p>
<a name="ul1694121910205"></a><a name="ul1694121910205"></a><ul id="ul1694121910205"><li>DCMI_COMPAT_OK (1) 表示NPU驱动和NPU固件兼容。</li><li>DCMI_COMPAT_NOK (2) 表示NPU驱动和NPU固件不兼容。</li><li>DCMI_COMPAT_UNKNOWN (3) 表示NPU驱动和NPU固件配套关系未知。</li></ul>
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

**约束说明<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412082"></a>**

调用该接口前NPU固件和NPU驱动的安装升级必须生效。

**表 1** 不同部署场景下的支持情况

<a name="table64935615397"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row18113171210"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p9380249113917"><a name="zh-cn_topic_0000002485318818_p9380249113917"></a><a name="zh-cn_topic_0000002485318818_p9380249113917"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p13380749113915"><a name="zh-cn_topic_0000002485318818_p13380749113915"></a><a name="zh-cn_topic_0000002485318818_p13380749113915"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p10380349173915"><a name="zh-cn_topic_0000002485318818_p10380349173915"></a><a name="zh-cn_topic_0000002485318818_p10380349173915"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p03801649133914"><a name="zh-cn_topic_0000002485318818_p03801649133914"></a><a name="zh-cn_topic_0000002485318818_p03801649133914"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row38243131214"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p48210361212"><a name="zh-cn_topic_0000002485318818_p48210361212"></a><a name="zh-cn_topic_0000002485318818_p48210361212"></a><span id="zh-cn_topic_0000002485318818_text16821238124"><a name="zh-cn_topic_0000002485318818_text16821238124"></a><a name="zh-cn_topic_0000002485318818_text16821238124"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1990911717125"><a name="zh-cn_topic_0000002485318818_p1990911717125"></a><a name="zh-cn_topic_0000002485318818_p1990911717125"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1590914714124"><a name="zh-cn_topic_0000002485318818_p1590914714124"></a><a name="zh-cn_topic_0000002485318818_p1590914714124"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p9909273125"><a name="zh-cn_topic_0000002485318818_p9909273125"></a><a name="zh-cn_topic_0000002485318818_p9909273125"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row982437122"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p4829381220"><a name="zh-cn_topic_0000002485318818_p4829381220"></a><a name="zh-cn_topic_0000002485318818_p4829381220"></a><span id="zh-cn_topic_0000002485318818_text582173201218"><a name="zh-cn_topic_0000002485318818_text582173201218"></a><a name="zh-cn_topic_0000002485318818_text582173201218"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p59098719121"><a name="zh-cn_topic_0000002485318818_p59098719121"></a><a name="zh-cn_topic_0000002485318818_p59098719121"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p20909197161211"><a name="zh-cn_topic_0000002485318818_p20909197161211"></a><a name="zh-cn_topic_0000002485318818_p20909197161211"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p199091074123"><a name="zh-cn_topic_0000002485318818_p199091074123"></a><a name="zh-cn_topic_0000002485318818_p199091074123"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1782915383548"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p988163171613"><a name="zh-cn_topic_0000002485318818_p988163171613"></a><a name="zh-cn_topic_0000002485318818_p988163171613"></a><span id="zh-cn_topic_0000002485318818_text28812351611"><a name="zh-cn_topic_0000002485318818_text28812351611"></a><a name="zh-cn_topic_0000002485318818_text28812351611"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p107731653135415"><a name="zh-cn_topic_0000002485318818_p107731653135415"></a><a name="zh-cn_topic_0000002485318818_p107731653135415"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p77730531545"><a name="zh-cn_topic_0000002485318818_p77730531545"></a><a name="zh-cn_topic_0000002485318818_p77730531545"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p8773153175410"><a name="zh-cn_topic_0000002485318818_p8773153175410"></a><a name="zh-cn_topic_0000002485318818_p8773153175410"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row19683944122816"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p19717154811286"><a name="zh-cn_topic_0000002485318818_p19717154811286"></a><a name="zh-cn_topic_0000002485318818_p19717154811286"></a><span id="zh-cn_topic_0000002485318818_text5717204812817"><a name="zh-cn_topic_0000002485318818_text5717204812817"></a><a name="zh-cn_topic_0000002485318818_text5717204812817"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p13998205142815"><a name="zh-cn_topic_0000002485318818_p13998205142815"></a><a name="zh-cn_topic_0000002485318818_p13998205142815"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p49983512287"><a name="zh-cn_topic_0000002485318818_p49983512287"></a><a name="zh-cn_topic_0000002485318818_p49983512287"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p89988518280"><a name="zh-cn_topic_0000002485318818_p89988518280"></a><a name="zh-cn_topic_0000002485318818_p89988518280"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row06711416544"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1765294655411"><a name="zh-cn_topic_0000002485318818_p1765294655411"></a><a name="zh-cn_topic_0000002485318818_p1765294655411"></a><span id="zh-cn_topic_0000002485318818_text19652946185414"><a name="zh-cn_topic_0000002485318818_text19652946185414"></a><a name="zh-cn_topic_0000002485318818_text19652946185414"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p428811549547"><a name="zh-cn_topic_0000002485318818_p428811549547"></a><a name="zh-cn_topic_0000002485318818_p428811549547"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p15288954155420"><a name="zh-cn_topic_0000002485318818_p15288954155420"></a><a name="zh-cn_topic_0000002485318818_p15288954155420"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p192881754115412"><a name="zh-cn_topic_0000002485318818_p192881754115412"></a><a name="zh-cn_topic_0000002485318818_p192881754115412"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row13182101810819"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p104761521386"><a name="zh-cn_topic_0000002485318818_p104761521386"></a><a name="zh-cn_topic_0000002485318818_p104761521386"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412083"></a>**

```
int ret = 0;
int card_id = 0;
int device_id = 0;
enum dcmi_device_compat compatibility = DCMI_COMPAT_UNKNOWN;
ret = dcmi_get_device_compatibility (card_id, device_id, & compatibility);
…
```

