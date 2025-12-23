# dcmi\_get\_all\_device\_count<a name="ZH-CN_TOPIC_0000002485295492"></a>

**函数原型<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412077"></a>**

**int dcmi\_get\_all\_device\_count\(int \*all\_device\_count\)**

**功能说明<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412078"></a>**

host启动后，查询与host PCIe建链成功的昇腾AI处理器设备个数。

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
<tbody><tr id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_row5908907"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p14881939993"><a name="p14881939993"></a><a name="p14881939993"></a>all_device_count</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="p1488203916913"><a name="p1488203916913"></a><a name="p1488203916913"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="p18833919917"><a name="p18833919917"></a><a name="p18833919917"></a>int *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p118820391193"><a name="p118820391193"></a><a name="p118820391193"></a>查询到的设备个数。</p>
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

**约束说明<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412082"></a>**

调用该接口前NPU固件和NPU驱动的安装升级必须生效。

**表 1** 不同部署场景下的支持情况

<a name="table1831828113412"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row19901425175211"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p1555917570362"><a name="zh-cn_topic_0000002485295476_p1555917570362"></a><a name="zh-cn_topic_0000002485295476_p1555917570362"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p201384912311"><a name="zh-cn_topic_0000002485295476_p201384912311"></a><a name="zh-cn_topic_0000002485295476_p201384912311"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p513124915316"><a name="zh-cn_topic_0000002485295476_p513124915316"></a><a name="zh-cn_topic_0000002485295476_p513124915316"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p1131549173112"><a name="zh-cn_topic_0000002485295476_p1131549173112"></a><a name="zh-cn_topic_0000002485295476_p1131549173112"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row18920204653019"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p232525365317"><a name="zh-cn_topic_0000002485295476_p232525365317"></a><a name="zh-cn_topic_0000002485295476_p232525365317"></a><span id="zh-cn_topic_0000002485295476_ph3325165314533"><a name="zh-cn_topic_0000002485295476_ph3325165314533"></a><a name="zh-cn_topic_0000002485295476_ph3325165314533"></a><span id="zh-cn_topic_0000002485295476_text2032565316533"><a name="zh-cn_topic_0000002485295476_text2032565316533"></a><a name="zh-cn_topic_0000002485295476_text2032565316533"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p162792612478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p172742619478"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_p527162654713"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_zh-cn_topic_0000001777715122_zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_row6920114611302"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p33251753135313"><a name="zh-cn_topic_0000002485295476_p33251753135313"></a><a name="zh-cn_topic_0000002485295476_p33251753135313"></a><span id="zh-cn_topic_0000002485295476_text232555315538"><a name="zh-cn_topic_0000002485295476_text232555315538"></a><a name="zh-cn_topic_0000002485295476_text232555315538"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p862174514211"><a name="zh-cn_topic_0000002485295476_p862174514211"></a><a name="zh-cn_topic_0000002485295476_p862174514211"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p12621104519215"><a name="zh-cn_topic_0000002485295476_p12621104519215"></a><a name="zh-cn_topic_0000002485295476_p12621104519215"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p6621204519220"><a name="zh-cn_topic_0000002485295476_p6621204519220"></a><a name="zh-cn_topic_0000002485295476_p6621204519220"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row293019912529"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p532515315535"><a name="zh-cn_topic_0000002485295476_p532515315535"></a><a name="zh-cn_topic_0000002485295476_p532515315535"></a><span id="zh-cn_topic_0000002485295476_text732545315315"><a name="zh-cn_topic_0000002485295476_text732545315315"></a><a name="zh-cn_topic_0000002485295476_text732545315315"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p386884075314"><a name="zh-cn_topic_0000002485295476_p386884075314"></a><a name="zh-cn_topic_0000002485295476_p386884075314"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p12868184075310"><a name="zh-cn_topic_0000002485295476_p12868184075310"></a><a name="zh-cn_topic_0000002485295476_p12868184075310"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p986814025319"><a name="zh-cn_topic_0000002485295476_p986814025319"></a><a name="zh-cn_topic_0000002485295476_p986814025319"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row29531122175210"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1832516539533"><a name="zh-cn_topic_0000002485295476_p1832516539533"></a><a name="zh-cn_topic_0000002485295476_p1832516539533"></a><span id="zh-cn_topic_0000002485295476_text143251353185313"><a name="zh-cn_topic_0000002485295476_text143251353185313"></a><a name="zh-cn_topic_0000002485295476_text143251353185313"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p16576467538"><a name="zh-cn_topic_0000002485295476_p16576467538"></a><a name="zh-cn_topic_0000002485295476_p16576467538"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13657346135315"><a name="zh-cn_topic_0000002485295476_p13657346135315"></a><a name="zh-cn_topic_0000002485295476_p13657346135315"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p10657164695315"><a name="zh-cn_topic_0000002485295476_p10657164695315"></a><a name="zh-cn_topic_0000002485295476_p10657164695315"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row2737141919528"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1632695317538"><a name="zh-cn_topic_0000002485295476_p1632695317538"></a><a name="zh-cn_topic_0000002485295476_p1632695317538"></a><span id="zh-cn_topic_0000002485295476_text532612537537"><a name="zh-cn_topic_0000002485295476_text532612537537"></a><a name="zh-cn_topic_0000002485295476_text532612537537"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1844654714531"><a name="zh-cn_topic_0000002485295476_p1844654714531"></a><a name="zh-cn_topic_0000002485295476_p1844654714531"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p64461747205318"><a name="zh-cn_topic_0000002485295476_p64461747205318"></a><a name="zh-cn_topic_0000002485295476_p64461747205318"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1446184725320"><a name="zh-cn_topic_0000002485295476_p1446184725320"></a><a name="zh-cn_topic_0000002485295476_p1446184725320"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row78171617135218"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p3326853195318"><a name="zh-cn_topic_0000002485295476_p3326853195318"></a><a name="zh-cn_topic_0000002485295476_p3326853195318"></a><span id="zh-cn_topic_0000002485295476_text15326195345317"><a name="zh-cn_topic_0000002485295476_text15326195345317"></a><a name="zh-cn_topic_0000002485295476_text15326195345317"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p2967114725312"><a name="zh-cn_topic_0000002485295476_p2967114725312"></a><a name="zh-cn_topic_0000002485295476_p2967114725312"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1396754795312"><a name="zh-cn_topic_0000002485295476_p1396754795312"></a><a name="zh-cn_topic_0000002485295476_p1396754795312"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p596734725316"><a name="zh-cn_topic_0000002485295476_p596734725316"></a><a name="zh-cn_topic_0000002485295476_p596734725316"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1913121595213"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p532613533533"><a name="zh-cn_topic_0000002485295476_p532613533533"></a><a name="zh-cn_topic_0000002485295476_p532613533533"></a><span id="zh-cn_topic_0000002485295476_text332611538530"><a name="zh-cn_topic_0000002485295476_text332611538530"></a><a name="zh-cn_topic_0000002485295476_text332611538530"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1452484814533"><a name="zh-cn_topic_0000002485295476_p1452484814533"></a><a name="zh-cn_topic_0000002485295476_p1452484814533"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p19524174835320"><a name="zh-cn_topic_0000002485295476_p19524174835320"></a><a name="zh-cn_topic_0000002485295476_p19524174835320"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p652419482532"><a name="zh-cn_topic_0000002485295476_p652419482532"></a><a name="zh-cn_topic_0000002485295476_p652419482532"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row71831758163218"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p28486114331"><a name="zh-cn_topic_0000002485295476_p28486114331"></a><a name="zh-cn_topic_0000002485295476_p28486114331"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412083"></a>**

```
int ret = 0;
int all_device_count;
ret = dcmi_get_all_device_count(&all_device_count);
if(ret != 0) {
    //todo
    return ret;
}
…
```

