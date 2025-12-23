# dcmi\_sm\_encrypt<a name="ZH-CN_TOPIC_0000002485455380"></a>

**函数原型<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412077"></a>**

**int dcmi\_sm\_encrypt\(int card\_id, int device\_id, struct dcmi\_sm\_parm\* parm, struct dcmi\_sm\_data \*data\)**

**功能说明<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412078"></a>**

调用此接口，输入需加密的明文、加密的密钥以及国密加密算法类型，获取加密后的密文。

**参数说明<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412079"></a>**

<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.97%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.03%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.97%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.03%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.97%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.03%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p5502112144615"><a name="p5502112144615"></a><a name="p5502112144615"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p9315182755815"><a name="p9315182755815"></a><a name="p9315182755815"></a>parm</p>
</td>
<td class="cellrowborder" valign="top" width="16.97%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.03%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"></a>struct dcmi_sm_parm*</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p12792271415"><a name="p12792271415"></a><a name="p12792271415"></a>struct sm_parm {</p>
<p id="p158072713117"><a name="p158072713117"></a><a name="p158072713117"></a>unsigned int key_type;</p>
<p id="p3809271110"><a name="p3809271110"></a><a name="p3809271110"></a>unsigned int key_len;//SM4密钥长度为16Byte，不涉及SM3</p>
<p id="p98011278111"><a name="p98011278111"></a><a name="p98011278111"></a>unsigned int iv_len;//SM4初始值长度为16Byte，不涉及SM3</p>
<p id="p880132711119"><a name="p880132711119"></a><a name="p880132711119"></a>unsigned int reserves;//预留</p>
<p id="p188042716114"><a name="p188042716114"></a><a name="p188042716114"></a>unsigned char iv[64];//CBC算法初始化向量</p>
<p id="p4806275111"><a name="p4806275111"></a><a name="p4806275111"></a>unsigned char key[512];//密钥</p>
<p id="p18801727611"><a name="p18801727611"></a><a name="p18801727611"></a>unsigned char reserved[512];//预留</p>
<p id="p168017271816"><a name="p168017271816"></a><a name="p168017271816"></a>};</p>
<p id="p168013271617"><a name="p168013271617"></a><a name="p168013271617"></a>key_type入参范围：</p>
<p id="p1804276118"><a name="p1804276118"></a><a name="p1804276118"></a>enum sm_key_type{</p>
<p id="p9673123415610"><a name="p9673123415610"></a><a name="p9673123415610"></a>SM3_NORMAL_SUMMARY = 0,//SM3杂凑算法操作</p>
<p id="p11673153485616"><a name="p11673153485616"></a><a name="p11673153485616"></a>SM4_CBC_ENCRYPT = 1, //SM4 CBC加密算法</p>
<p id="p1667393414568"><a name="p1667393414568"></a><a name="p1667393414568"></a>SM4_CBC_DECRYPT = 2,[x(1] //SM4 CBC解密算法</p>
<p id="p1580182710111"><a name="p1580182710111"></a><a name="p1580182710111"></a>};</p>
</td>
</tr>
<tr id="row15244163135814"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p32445315586"><a name="p32445315586"></a><a name="p32445315586"></a>data</p>
</td>
<td class="cellrowborder" valign="top" width="16.97%" headers="mcps1.1.5.1.2 "><p id="p15244731115812"><a name="p15244731115812"></a><a name="p15244731115812"></a>输入；输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.03%" headers="mcps1.1.5.1.3 "><p id="p17244731155814"><a name="p17244731155814"></a><a name="p17244731155814"></a>struct dcmi_sm_data *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1801527219"><a name="p1801527219"></a><a name="p1801527219"></a>struct sm_data {</p>
<p id="p78010271212"><a name="p78010271212"></a><a name="p78010271212"></a>const unsigned char *in_buf;</p>
<p id="p19803271713"><a name="p19803271713"></a><a name="p19803271713"></a>unsigned in_len;//SM3长度、SM4长度最多为3072Byte，且SM4长度必须为16Byte的整数倍</p>
<p id="p48221628153511"><a name="p48221628153511"></a><a name="p48221628153511"></a>unsigned char *out_buf;//输出缓存</p>
<p id="p16869334123513"><a name="p16869334123513"></a><a name="p16869334123513"></a>unsigned int *out_len;//输出缓存长度</p>
<p id="p88012277115"><a name="p88012277115"></a><a name="p88012277115"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table1993685321815"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1210513304816"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p558011817325"><a name="zh-cn_topic_0000002485295476_p558011817325"></a><a name="zh-cn_topic_0000002485295476_p558011817325"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p25806812321"><a name="zh-cn_topic_0000002485295476_p25806812321"></a><a name="zh-cn_topic_0000002485295476_p25806812321"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p558018833211"><a name="zh-cn_topic_0000002485295476_p558018833211"></a><a name="zh-cn_topic_0000002485295476_p558018833211"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p35801189326"><a name="zh-cn_topic_0000002485295476_p35801189326"></a><a name="zh-cn_topic_0000002485295476_p35801189326"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row14576182015105"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p13661881916"><a name="zh-cn_topic_0000002485295476_p13661881916"></a><a name="zh-cn_topic_0000002485295476_p13661881916"></a><span id="zh-cn_topic_0000002485295476_ph116612081298"><a name="zh-cn_topic_0000002485295476_ph116612081298"></a><a name="zh-cn_topic_0000002485295476_ph116612081298"></a><span id="zh-cn_topic_0000002485295476_text26611487916"><a name="zh-cn_topic_0000002485295476_text26611487916"></a><a name="zh-cn_topic_0000002485295476_text26611487916"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p057642081019"><a name="zh-cn_topic_0000002485295476_p057642081019"></a><a name="zh-cn_topic_0000002485295476_p057642081019"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p65761200102"><a name="zh-cn_topic_0000002485295476_p65761200102"></a><a name="zh-cn_topic_0000002485295476_p65761200102"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1357610206101"><a name="zh-cn_topic_0000002485295476_p1357610206101"></a><a name="zh-cn_topic_0000002485295476_p1357610206101"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row318512127818"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p86611581596"><a name="zh-cn_topic_0000002485295476_p86611581596"></a><a name="zh-cn_topic_0000002485295476_p86611581596"></a><span id="zh-cn_topic_0000002485295476_text66619818911"><a name="zh-cn_topic_0000002485295476_text66619818911"></a><a name="zh-cn_topic_0000002485295476_text66619818911"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1441443298"><a name="zh-cn_topic_0000002485295476_p1441443298"></a><a name="zh-cn_topic_0000002485295476_p1441443298"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p2420631892"><a name="zh-cn_topic_0000002485295476_p2420631892"></a><a name="zh-cn_topic_0000002485295476_p2420631892"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p18425123899"><a name="zh-cn_topic_0000002485295476_p18425123899"></a><a name="zh-cn_topic_0000002485295476_p18425123899"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1273713241084"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p56611781094"><a name="zh-cn_topic_0000002485295476_p56611781094"></a><a name="zh-cn_topic_0000002485295476_p56611781094"></a><span id="zh-cn_topic_0000002485295476_text56611281693"><a name="zh-cn_topic_0000002485295476_text56611281693"></a><a name="zh-cn_topic_0000002485295476_text56611281693"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p64311736915"><a name="zh-cn_topic_0000002485295476_p64311736915"></a><a name="zh-cn_topic_0000002485295476_p64311736915"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p04391434914"><a name="zh-cn_topic_0000002485295476_p04391434914"></a><a name="zh-cn_topic_0000002485295476_p04391434914"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p244917315919"><a name="zh-cn_topic_0000002485295476_p244917315919"></a><a name="zh-cn_topic_0000002485295476_p244917315919"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row7672192219815"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p166611689914"><a name="zh-cn_topic_0000002485295476_p166611689914"></a><a name="zh-cn_topic_0000002485295476_p166611689914"></a><span id="zh-cn_topic_0000002485295476_text126611581798"><a name="zh-cn_topic_0000002485295476_text126611581798"></a><a name="zh-cn_topic_0000002485295476_text126611581798"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p12459143996"><a name="zh-cn_topic_0000002485295476_p12459143996"></a><a name="zh-cn_topic_0000002485295476_p12459143996"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p10470531895"><a name="zh-cn_topic_0000002485295476_p10470531895"></a><a name="zh-cn_topic_0000002485295476_p10470531895"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p12476033912"><a name="zh-cn_topic_0000002485295476_p12476033912"></a><a name="zh-cn_topic_0000002485295476_p12476033912"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row153452020881"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p86611181098"><a name="zh-cn_topic_0000002485295476_p86611181098"></a><a name="zh-cn_topic_0000002485295476_p86611181098"></a><span id="zh-cn_topic_0000002485295476_text16611819911"><a name="zh-cn_topic_0000002485295476_text16611819911"></a><a name="zh-cn_topic_0000002485295476_text16611819911"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p134813311917"><a name="zh-cn_topic_0000002485295476_p134813311917"></a><a name="zh-cn_topic_0000002485295476_p134813311917"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p3486231596"><a name="zh-cn_topic_0000002485295476_p3486231596"></a><a name="zh-cn_topic_0000002485295476_p3486231596"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p44911135913"><a name="zh-cn_topic_0000002485295476_p44911135913"></a><a name="zh-cn_topic_0000002485295476_p44911135913"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row20496217988"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p86611689916"><a name="zh-cn_topic_0000002485295476_p86611689916"></a><a name="zh-cn_topic_0000002485295476_p86611689916"></a><span id="zh-cn_topic_0000002485295476_text966158896"><a name="zh-cn_topic_0000002485295476_text966158896"></a><a name="zh-cn_topic_0000002485295476_text966158896"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p649613392"><a name="zh-cn_topic_0000002485295476_p649613392"></a><a name="zh-cn_topic_0000002485295476_p649613392"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p2050114313914"><a name="zh-cn_topic_0000002485295476_p2050114313914"></a><a name="zh-cn_topic_0000002485295476_p2050114313914"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p175075312918"><a name="zh-cn_topic_0000002485295476_p175075312918"></a><a name="zh-cn_topic_0000002485295476_p175075312918"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1775216157819"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p766118812915"><a name="zh-cn_topic_0000002485295476_p766118812915"></a><a name="zh-cn_topic_0000002485295476_p766118812915"></a><span id="zh-cn_topic_0000002485295476_text136611481596"><a name="zh-cn_topic_0000002485295476_text136611481596"></a><a name="zh-cn_topic_0000002485295476_text136611481596"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p205121037920"><a name="zh-cn_topic_0000002485295476_p205121037920"></a><a name="zh-cn_topic_0000002485295476_p205121037920"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p5517131993"><a name="zh-cn_topic_0000002485295476_p5517131993"></a><a name="zh-cn_topic_0000002485295476_p5517131993"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p115221437916"><a name="zh-cn_topic_0000002485295476_p115221437916"></a><a name="zh-cn_topic_0000002485295476_p115221437916"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1499855417336"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p48161158173310"><a name="zh-cn_topic_0000002485295476_p48161158173310"></a><a name="zh-cn_topic_0000002485295476_p48161158173310"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_toc533412083"></a>**

```
 int ret;
    int card_id = 0;
    int device_id = 0;
    struct dcmi_sm_parm parm = {0};
    parm.key_type = 0;
    unsigned char hash1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    unsigned char *out_buf =  (unsigned char *)malloc(100);
    unsigned int *out_len =(unsigned int *)malloc(sizeof(unsigned int));
    *out_len = 100;
    struct dcmi_sm_data data = {(const unsigned char *)hash1, sizeof(hash1), out_buf, out_len};
    dcmi_init();
    ret = dcmi_sm_encrypt(0, 0, &parm, &data);
    if (ret != 0) {
        //todo:记录日志
        free(out_buf);
        free(out_len);
        return ret;
    }
    //data.out_buf中记录加密后的数据,data.out_len记录加密后的数据长度
    free(out_buf);
    free(out_len);
```

