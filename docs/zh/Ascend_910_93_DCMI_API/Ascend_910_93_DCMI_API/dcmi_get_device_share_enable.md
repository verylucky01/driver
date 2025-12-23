# dcmi\_get\_device\_share\_enable<a name="ZH-CN_TOPIC_0000002517558651"></a>

**函数原型<a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_toc533412077"></a>**

**int dcmi\_get\_device\_share\_enable\(int card\_id, int device\_id, int \*enable\_flag\)**

**功能说明<a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_toc533412078"></a>**

查询指定芯片的容器共享使能标记。

**参数说明<a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_toc533412079"></a>**

<a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p10021890"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p10021890"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p6466753"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p6466753"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p54045009"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p54045009"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p15569626"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p15569626"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p36741947142813"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p36741947142813"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p96741747122818"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p96741747122818"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p46747472287"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p46747472287"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1467413474281"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1467413474281"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p7711145152918"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p7711145152918"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p671116522914"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p671116522914"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1771116572910"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1771116572910"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p11128195202014"><a name="p11128195202014"></a><a name="p11128195202014"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_row189061455124014"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p3873105644017"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p3873105644017"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p3873105644017"></a>enable_flag</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p12873105644020"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p12873105644020"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p12873105644020"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1187365654013"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1187365654013"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p1187365654013"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p687315614406"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p687315614406"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p687315614406"></a>1：使能</p>
<p id="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p08731556124015"><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p08731556124015"></a><a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_p08731556124015"></a>0：禁用</p>
<p id="p2751111174515"><a name="p2751111174515"></a><a name="p2751111174515"></a>默认禁用。</p>
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

**约束说明<a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_toc533412082"></a>**

-   设置容器共享模式后，重启系统后容器使能状态默认为禁用。
-   该接口支持在物理机+特权容器场景下使用。

**表 1** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
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

**调用示例<a name="zh-cn_topic_0000001251227179_zh-cn_topic_0000001223292897_zh-cn_topic_0000001101830678_toc533412083"></a>**

```
… 
int ret = 0;
int enable_flag = -1;
int card_id = 0;
int device_id = 0;
ret = dcmi_get_device_share_enable(card_id, device_id, &enable_flag);
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

