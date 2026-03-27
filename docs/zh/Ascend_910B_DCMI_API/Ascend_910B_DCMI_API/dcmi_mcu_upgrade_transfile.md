# dcmi\_mcu\_upgrade\_transfile<a name="ZH-CN_TOPIC_0000002517615357"></a>

**函数原型<a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_toc533412077"></a>**

**int dcmi\_mcu\_upgrade\_transfile\(int card\_id, const char \*file\)**

**功能说明<a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_toc533412078"></a>**

将MCU的升级包传至MCU。

**参数说明<a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_toc533412079"></a>**

<a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p10021890"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p10021890"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p6466753"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p6466753"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p54045009"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p54045009"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p15569626"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p15569626"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p10983229111518"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p10983229111518"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p10983229111518"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p29831291158"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p29831291158"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p29831291158"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1298482921519"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1298482921519"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1298482921519"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p898413293156"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p898413293156"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p898413293156"></a>设备ID，当前实际支持的编号通过dcmi_get_card_num_list获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p3984202913157"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p3984202913157"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p3984202913157"></a>file</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1698411294151"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1698411294151"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1698411294151"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p7984029171516"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p7984029171516"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p7984029171516"></a>char *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1698411298159"><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1698411298159"></a><a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_p1698411298159"></a>MCU升级包对应的bin包。</p>
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

**约束说明<a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_toc533412082"></a>**

该接口在后续版本将会删除，推荐使用[dcmi\_set\_mcu\_upgrade\_file](dcmi_set_mcu_upgrade_file.md)。

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

**调用示例<a name="zh-cn_topic_0000001251427205_zh-cn_topic_0000001223414451_zh-cn_topic_0000001148639173_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
ret = dcmi_mcu_upgrade_transfile(card_id,"./mcu.bin"); 
if (ret != 0) {
    //todo：记录日志
    return ret;
}
…
```

