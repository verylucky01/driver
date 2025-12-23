# dcmi\_get\_card\_elabel<a name="ZH-CN_TOPIC_0000002485478792"></a>

**函数原型<a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_toc533412077"></a>**

**int dcmi\_get\_card\_elabel\(int card\_id, struct dcmi\_elabel\_info\_stru \*elabel\_info\)**

**功能说明<a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_toc533412078"></a>**

获取NPU设备的电子标签信息。

**参数说明<a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p10021890"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p10021890"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p6466753"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p6466753"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p54045009"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p54045009"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p15569626"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p15569626"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p36741947142813"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p36741947142813"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p96741747122818"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p96741747122818"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p46747472287"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p46747472287"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p1467413474281"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p1467413474281"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p36921291519"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p36921291519"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p36921291519"></a>elabel_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p2692329056"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p2692329056"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p2692329056"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p116921929858"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p116921929858"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p116921929858"></a>struct dcmi_elabel_info_stru*</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p46583287398"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p46583287398"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p46583287398"></a>电子标签信息。</p>
<p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p156920291951"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p156920291951"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p156920291951"></a>#define MAX_LENTH 256</p>
<p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p6513191911390"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p6513191911390"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p6513191911390"></a>struct dcmi_elabel_info_stru {</p>
<p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p125131319113911"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p125131319113911"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p125131319113911"></a>char product_name[MAX_LENTH]; //产品名称</p>
<p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p051351913918"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p051351913918"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p051351913918"></a>char model[MAX_LENTH]; //产品型号</p>
<p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p155131219113910"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p155131219113910"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p155131219113910"></a>char manufacturer[MAX_LENTH]; //生产厂家</p>
<p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p851391917394"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p851391917394"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p851391917394"></a>char serial_number[MAX_LENTH]; //产品序列号</p>
<p id="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p2513151918392"><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p2513151918392"></a><a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_p2513151918392"></a>}</p>
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

**约束说明<a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_toc533412082"></a>**

该接口在后续版本将会删除，推荐使用[dcmi\_get\_card\_elabel\_v2](dcmi_get_card_elabel_v2.md)。

**表 1** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_row119194469302"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p1842495418394"><a name="zh-cn_topic_0000002485318818_p1842495418394"></a><a name="zh-cn_topic_0000002485318818_p1842495418394"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p1642455415393"><a name="zh-cn_topic_0000002485318818_p1642455415393"></a><a name="zh-cn_topic_0000002485318818_p1642455415393"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p114246541390"><a name="zh-cn_topic_0000002485318818_p114246541390"></a><a name="zh-cn_topic_0000002485318818_p114246541390"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p64241654123916"><a name="zh-cn_topic_0000002485318818_p64241654123916"></a><a name="zh-cn_topic_0000002485318818_p64241654123916"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p1378374019445"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p1378374019445"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p1378374019445"></a><span id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text262011415447"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text262011415447"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text262011415447"></a><span id="zh-cn_topic_0000002485318818_text1564782665814"><a name="zh-cn_topic_0000002485318818_text1564782665814"></a><a name="zh-cn_topic_0000002485318818_text1564782665814"></a>Atlas 900 A3 SuperPoD 超节点</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p11683124944919"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p11683124944919"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p11683124944919"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p36831949194913"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p36831949194913"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p36831949194913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p2683144919499"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p2683144919499"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_p2683144919499"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_row82952324359"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p137832403444"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p137832403444"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_p137832403444"></a><span id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text6814122411310"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text6814122411310"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001819869974_text6814122411310"></a><span id="zh-cn_topic_0000002485318818_text1123515513517"><a name="zh-cn_topic_0000002485318818_text1123515513517"></a><a name="zh-cn_topic_0000002485318818_text1123515513517"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1285753073419"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1285753073419"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1285753073419"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1985743013346"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1985743013346"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p1985743013346"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p0857203013412"><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p0857203013412"></a><a name="zh-cn_topic_0000002485318818_zh-cn_topic_0000001820029662_zh-cn_topic_0000001251227155_zh-cn_topic_0000001178213210_zh-cn_topic_0000001147728951_p0857203013412"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row12693311552"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p17872181618166"><a name="zh-cn_topic_0000002485318818_p17872181618166"></a><a name="zh-cn_topic_0000002485318818_p17872181618166"></a><span id="zh-cn_topic_0000002485318818_text17872121641614"><a name="zh-cn_topic_0000002485318818_text17872121641614"></a><a name="zh-cn_topic_0000002485318818_text17872121641614"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p177521042115520"><a name="zh-cn_topic_0000002485318818_p177521042115520"></a><a name="zh-cn_topic_0000002485318818_p177521042115520"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p12752114215559"><a name="zh-cn_topic_0000002485318818_p12752114215559"></a><a name="zh-cn_topic_0000002485318818_p12752114215559"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p18752104275512"><a name="zh-cn_topic_0000002485318818_p18752104275512"></a><a name="zh-cn_topic_0000002485318818_p18752104275512"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row179407221298"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p696322918294"><a name="zh-cn_topic_0000002485318818_p696322918294"></a><a name="zh-cn_topic_0000002485318818_p696322918294"></a><span id="zh-cn_topic_0000002485318818_text15963162972914"><a name="zh-cn_topic_0000002485318818_text15963162972914"></a><a name="zh-cn_topic_0000002485318818_text15963162972914"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1560813335296"><a name="zh-cn_topic_0000002485318818_p1560813335296"></a><a name="zh-cn_topic_0000002485318818_p1560813335296"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p260873392918"><a name="zh-cn_topic_0000002485318818_p260873392918"></a><a name="zh-cn_topic_0000002485318818_p260873392918"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p5608333162914"><a name="zh-cn_topic_0000002485318818_p5608333162914"></a><a name="zh-cn_topic_0000002485318818_p5608333162914"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row19346183355516"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p0905133713551"><a name="zh-cn_topic_0000002485318818_p0905133713551"></a><a name="zh-cn_topic_0000002485318818_p0905133713551"></a><span id="zh-cn_topic_0000002485318818_text1190563775520"><a name="zh-cn_topic_0000002485318818_text1190563775520"></a><a name="zh-cn_topic_0000002485318818_text1190563775520"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p7204194395516"><a name="zh-cn_topic_0000002485318818_p7204194395516"></a><a name="zh-cn_topic_0000002485318818_p7204194395516"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p13205104310553"><a name="zh-cn_topic_0000002485318818_p13205104310553"></a><a name="zh-cn_topic_0000002485318818_p13205104310553"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p32051943125519"><a name="zh-cn_topic_0000002485318818_p32051943125519"></a><a name="zh-cn_topic_0000002485318818_p32051943125519"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1158213431480"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p65418471681"><a name="zh-cn_topic_0000002485318818_p65418471681"></a><a name="zh-cn_topic_0000002485318818_p65418471681"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001206467194_zh-cn_topic_0000001223494367_zh-cn_topic_0000001102270938_toc533412083"></a>**

```
… 
struct dcmi_elabel_info_stru elabelInfo;
int ret = 0;
int card_id = 0;
memset(&elabelInfo, 0, sizeof(elabelInfo));
ret = dcmi_get_card_elabel(card_id, &elabelInfo);
if (ret != 0) {
    //todo:记录日志
    return ERROR;
}
…
```

