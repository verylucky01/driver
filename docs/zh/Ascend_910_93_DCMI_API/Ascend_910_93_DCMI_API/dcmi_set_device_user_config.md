# dcmi\_set\_device\_user\_config<a name="ZH-CN_TOPIC_0000002517638685"></a>

**函数原型<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_toc533412077"></a>**

**int dcmi\_set\_device\_user\_config\(int card\_id, int device\_id, const char \*config\_name, unsigned int buf\_size, char \*buf\)**

**功能说明<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_toc533412078"></a>**

设置用户配置。

**参数说明<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p10021890"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p10021890"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p6466753"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p6466753"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p54045009"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p54045009"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p15569626"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p15569626"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p36741947142813"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p36741947142813"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p96741747122818"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p96741747122818"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p46747472287"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p46747472287"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1467413474281"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1467413474281"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p7711145152918"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p7711145152918"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p671116522914"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p671116522914"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1771116572910"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1771116572910"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p64347718224"><a name="p64347718224"></a><a name="p64347718224"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1762123612289"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1762123612289"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1762123612289"></a>config_name</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p46211236182811"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p46211236182811"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p46211236182811"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1662112368286"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1662112368286"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1662112368286"></a>const char *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p182531815143013"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p182531815143013"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p182531815143013"></a>目前支持处理的配置项名称如下，配置项名称的字符串长度最大为32。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1795311111557"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1795311111557"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1795311111557"></a>已实现功能的配置项：mac_info。支持用户自定义名称配置。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p189533165513"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p189533165513"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p189533165513"></a>配置项功能说明如下：</p>
<p id="p13747195920471"><a name="p13747195920471"></a><a name="p13747195920471"></a>“mac_info” ：用于设置MAC地址。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_row677353102819"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p347295412284"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p347295412284"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p347295412284"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1847275412814"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1847275412814"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1847275412814"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1472195462814"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1472195462814"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1472195462814"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p18198579293"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p18198579293"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p18198579293"></a>buf长度，单位为byte，最大长度为1K byte。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p33509511202"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p33509511202"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p33509511202"></a>支持mac_info配置。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p5350155152014"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p5350155152014"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p5350155152014"></a>目前支持处理的配置项名称如下：</p>
<p id="p2542749172219"><a name="p2542749172219"></a><a name="p2542749172219"></a>如果配置"mac_info" ：buf_size参数固定配置为16。</p>
<p id="p7865034114310"><a name="p7865034114310"></a><a name="p7865034114310"></a>除了如上固定的名称外，其他最大长度不超过1024 byte。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_row19625184713240"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p176251747192415"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p176251747192415"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p176251747192415"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1862534712243"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1862534712243"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1862534712243"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p462554719244"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p462554719244"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p462554719244"></a>char *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p32012572298"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p32012572298"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p32012572298"></a>buf指针，指向配置项内容。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1398395362310"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1398395362310"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p1398395362310"></a>支持mac_info配置。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p189915342233"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p189915342233"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p189915342233"></a>目前支持处理的配置项名称如下：</p>
<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_ul626216442429"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_ul626216442429"></a><ul id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_ul626216442429"><li>针对"mac_info"配置项，默认值为空，包含内容如下：<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_ul10163124817104"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_ul10163124817104"></a><ul id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_ul10163124817104"><li>buf[0]~buf[1]： crc_value，对应于buf[2]~buf[10]的CRC校验值；</li><li>buf[2]：data_length，固定为9；</li><li>buf[3]： mac_id，对应于MAC_ID；</li><li>buf[4]：mac_type，对应于MAC类型；</li><li>buf[5]~buf[10]：mac_addr，对应于MAC地址，占6个Byte；</li><li>buf[11]~buf[15]：预留内容，当前没有使用，可置为全0。</li></ul>
</li></ul>
<p id="p1289194120432"><a name="p1289194120432"></a><a name="p1289194120432"></a>除了如上固定的名称外，其他配置项内容请根据实际情况配置。</p>
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

**约束说明<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
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

**调用示例<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_toc533412083"></a>**

```
… 
#define BUF_SIZE 1
int ret = 0;
int card_id = 0;
int device_id = 0;
char *config_name = "mac_info";
char buf[BUF_SIZE] = {0};
ret=dcmi_set_device_user_config(card_id, device_id,config_name, BUF_SIZE, buf);
if (ret != 0) {
    //todo:记录日志
    return ret;
}
…
```

