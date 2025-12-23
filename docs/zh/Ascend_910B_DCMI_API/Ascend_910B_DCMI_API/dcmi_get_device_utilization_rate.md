# dcmi\_get\_device\_utilization\_rate<a name="ZH-CN_TOPIC_0000002485455406"></a>

**函数原型<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_toc533412077"></a>**

**int dcmi\_get\_device\_utilization\_rate\(int card\_id, int device\_id, int input\_type, unsigned int \*utilization\_rate\)**

**功能说明<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_toc533412078"></a>**

获取芯片占用率。

**参数说明<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_toc533412079"></a>**

<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p10021890"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p10021890"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17.1%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p6466753"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p6466753"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="15.9%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p54045009"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p54045009"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p15569626"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p15569626"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p36741947142813"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p36741947142813"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p96741747122818"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p96741747122818"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.9%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p46747472287"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p46747472287"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1467413474281"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1467413474281"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p7711145152918"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p7711145152918"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p671116522914"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p671116522914"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.9%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1771116572910"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1771116572910"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p932083014157"><a name="p932083014157"></a><a name="p932083014157"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p89531449406"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p89531449406"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p89531449406"></a>input_type</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p795313444020"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p795313444020"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p795313444020"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="15.9%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p199530414407"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p199530414407"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p199530414407"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p666717157201"><a name="p666717157201"></a><a name="p666717157201"></a>设备类型，目前仅支持2、3、4、6、10、12、13这几种类型。</p>
<p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p20138046183612"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p20138046183612"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p20138046183612"></a>数值和具体设备类型对应如下：</p>
<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_zh-cn_topic_0204328591_zh-cn_topic_0146325092_ul5794145112018"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_zh-cn_topic_0204328591_zh-cn_topic_0146325092_ul5794145112018"></a><ul id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_zh-cn_topic_0204328591_zh-cn_topic_0146325092_ul5794145112018"><li>1：内存</li><li>2：AI Core</li><li>3：AI CPU</li><li>4：控制CPU</li><li>5：内存带宽</li><li>6：片上内存</li><li>8：DDR</li><li>10：片上内存带宽</li></ul>
<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_ul188012973618"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_ul188012973618"></a><ul id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_ul188012973618"><li>12：vector core</li><li>13：NPU整体利用率</li></ul>
<p id="p12537234101513"><a name="p12537234101513"></a><a name="p12537234101513"></a></p>
<div class="note" id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_note079734513131"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_note079734513131"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_note079734513131"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p19992205925611"><a name="p19992205925611"></a><a name="p19992205925611"></a>当设备类型为AI Core或vector core时开启profiling，查询占用率为0，实际无意义。</p>
<p id="p145284519146"><a name="p145284519146"></a><a name="p145284519146"></a>在直通虚拟机场景、直通虚拟机容器场景仅支持AI Core（2）、片上内存（6）、片上内存带宽（10）、vector core（12），其它参数返回不支持。该场景下获取的片上内存带宽为0，实际无意义。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_row186430244012"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1895344114020"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1895344114020"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p1895344114020"></a>utilization_rate</p>
</td>
<td class="cellrowborder" valign="top" width="17.1%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p16953134204011"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p16953134204011"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p16953134204011"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="15.9%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p109534410401"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p109534410401"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p109534410401"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p59531242402"><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p59531242402"></a><a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_p59531242402"></a>处理器利用率，单位：%。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000001917887173_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**约束说明<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_toc533412082"></a>**

Atlas 300I A2 推理卡、Atlas 300T A2 训练卡、Atlas 800I A2 推理服务器、Atlas 900 A2 PoD 集群基础单元、A200I A2 Box 异构组件的片上内存为32G，无业务时系统占用3G；Atlas 900 A2 PoD 集群基础单元、Atlas 800T A2 训练服务器、Atlas 200T A2 Box16 异构子框、Atlas 800I A2 推理服务器、A200I A2 Box 异构组件的片上内存为64G，无业务时系统占用4G。

**表 1** 不同部署场景下的支持情况

<a name="table1113417173519"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1548132517501"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p16154843125019"><a name="zh-cn_topic_0000002485295476_p16154843125019"></a><a name="zh-cn_topic_0000002485295476_p16154843125019"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p3123182915595"><a name="zh-cn_topic_0000002485295476_p3123182915595"></a><a name="zh-cn_topic_0000002485295476_p3123182915595"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p6271950600"><a name="zh-cn_topic_0000002485295476_p6271950600"></a><a name="zh-cn_topic_0000002485295476_p6271950600"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p41541743195013"><a name="zh-cn_topic_0000002485295476_p41541743195013"></a><a name="zh-cn_topic_0000002485295476_p41541743195013"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p19444162914516"><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><a name="zh-cn_topic_0000002485295476_p19444162914516"></a><span id="zh-cn_topic_0000002485295476_ph1944412296514"><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><a name="zh-cn_topic_0000002485295476_ph1944412296514"></a><span id="zh-cn_topic_0000002485295476_text944432913516"><a name="zh-cn_topic_0000002485295476_text944432913516"></a><a name="zh-cn_topic_0000002485295476_text944432913516"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001917887173_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row6920114611302"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p194441629165117"><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><a name="zh-cn_topic_0000002485295476_p194441629165117"></a><span id="zh-cn_topic_0000002485295476_text124449291511"><a name="zh-cn_topic_0000002485295476_text124449291511"></a><a name="zh-cn_topic_0000002485295476_text124449291511"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p461320458219"><a name="zh-cn_topic_0000002485295476_p461320458219"></a><a name="zh-cn_topic_0000002485295476_p461320458219"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13613134519220"><a name="zh-cn_topic_0000002485295476_p13613134519220"></a><a name="zh-cn_topic_0000002485295476_p13613134519220"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p96133454211"><a name="zh-cn_topic_0000002485295476_p96133454211"></a><a name="zh-cn_topic_0000002485295476_p96133454211"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row14873174604910"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p10444329155118"><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><a name="zh-cn_topic_0000002485295476_p10444329155118"></a><span id="zh-cn_topic_0000002485295476_text1044482925119"><a name="zh-cn_topic_0000002485295476_text1044482925119"></a><a name="zh-cn_topic_0000002485295476_text1044482925119"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p896391111511"><a name="zh-cn_topic_0000002485295476_p896391111511"></a><a name="zh-cn_topic_0000002485295476_p896391111511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p119682011105113"><a name="zh-cn_topic_0000002485295476_p119682011105113"></a><a name="zh-cn_topic_0000002485295476_p119682011105113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p11975911195114"><a name="zh-cn_topic_0000002485295476_p11975911195114"></a><a name="zh-cn_topic_0000002485295476_p11975911195114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row23051923114915"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p174441129175117"><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><a name="zh-cn_topic_0000002485295476_p174441129175117"></a><span id="zh-cn_topic_0000002485295476_text34441729175119"><a name="zh-cn_topic_0000002485295476_text34441729175119"></a><a name="zh-cn_topic_0000002485295476_text34441729175119"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1298231135114"><a name="zh-cn_topic_0000002485295476_p1298231135114"></a><a name="zh-cn_topic_0000002485295476_p1298231135114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1298921117511"><a name="zh-cn_topic_0000002485295476_p1298921117511"></a><a name="zh-cn_topic_0000002485295476_p1298921117511"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1799561165114"><a name="zh-cn_topic_0000002485295476_p1799561165114"></a><a name="zh-cn_topic_0000002485295476_p1799561165114"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row44814564912"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1044414295516"><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><a name="zh-cn_topic_0000002485295476_p1044414295516"></a><span id="zh-cn_topic_0000002485295476_text44441829105114"><a name="zh-cn_topic_0000002485295476_text44441829105114"></a><a name="zh-cn_topic_0000002485295476_text44441829105114"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p17181285116"><a name="zh-cn_topic_0000002485295476_p17181285116"></a><a name="zh-cn_topic_0000002485295476_p17181285116"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15131255114"><a name="zh-cn_topic_0000002485295476_p15131255114"></a><a name="zh-cn_topic_0000002485295476_p15131255114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15917127513"><a name="zh-cn_topic_0000002485295476_p15917127513"></a><a name="zh-cn_topic_0000002485295476_p15917127513"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row11171321114917"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p94441829165113"><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><a name="zh-cn_topic_0000002485295476_p94441829165113"></a><span id="zh-cn_topic_0000002485295476_text644410297515"><a name="zh-cn_topic_0000002485295476_text644410297515"></a><a name="zh-cn_topic_0000002485295476_text644410297515"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p01211275113"><a name="zh-cn_topic_0000002485295476_p01211275113"></a><a name="zh-cn_topic_0000002485295476_p01211275113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p916161212512"><a name="zh-cn_topic_0000002485295476_p916161212512"></a><a name="zh-cn_topic_0000002485295476_p916161212512"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p191911211511"><a name="zh-cn_topic_0000002485295476_p191911211511"></a><a name="zh-cn_topic_0000002485295476_p191911211511"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row18385115320492"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1644572918513"><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><a name="zh-cn_topic_0000002485295476_p1644572918513"></a><span id="zh-cn_topic_0000002485295476_text1744532925112"><a name="zh-cn_topic_0000002485295476_text1744532925112"></a><a name="zh-cn_topic_0000002485295476_text1744532925112"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1923112115119"><a name="zh-cn_topic_0000002485295476_p1923112115119"></a><a name="zh-cn_topic_0000002485295476_p1923112115119"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1269122515"><a name="zh-cn_topic_0000002485295476_p1269122515"></a><a name="zh-cn_topic_0000002485295476_p1269122515"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p43014126516"><a name="zh-cn_topic_0000002485295476_p43014126516"></a><a name="zh-cn_topic_0000002485295476_p43014126516"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row557915117191"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p2042757191916"><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><a name="zh-cn_topic_0000002485295476_p2042757191916"></a><span id="zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251227163_zh-cn_topic_0000001178054674_zh-cn_topic_0000001148257037_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0x3;
int device_id = 0;
unsigned int utilization_rate = 0;
int input_type = DCMI_UTILIZATION_RATE_DDR;
ret = dcmi_get_device_utilization_rate(card_id, device_id, input_type, &utilization_rate);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

