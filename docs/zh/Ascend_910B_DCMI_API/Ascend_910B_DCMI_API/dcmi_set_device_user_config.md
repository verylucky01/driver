# dcmi\_set\_device\_user\_config<a name="ZH-CN_TOPIC_0000002485455374"></a>

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
<p id="p62341346195419"><a name="p62341346195419"></a><a name="p62341346195419"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
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
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p18198579293"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p18198579293"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_zh-cn_topic_0204328569_zh-cn_topic_0158027668_p18198579293"></a>buf长度，单位为Byte，最大长度为1K Byte。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p33509511202"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p33509511202"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p33509511202"></a>支持mac_info配置。</p>
<p id="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p5350155152014"><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p5350155152014"></a><a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_p5350155152014"></a>目前支持处理的配置项名称如下：</p>
<p id="p1065535661610"><a name="p1065535661610"></a><a name="p1065535661610"></a>如果配置"mac_info" ：buf_size参数固定配置为16。</p>
<p id="p7865034114310"><a name="p7865034114310"></a><a name="p7865034114310"></a>除了如上固定的名称外，其他最大长度不超过1024 Byte。</p>
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

**约束说明<a name="zh-cn_topic_0000001206467198_zh-cn_topic_0000001223494375_zh-cn_topic_0000001148770187_toc533412082"></a>**

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

