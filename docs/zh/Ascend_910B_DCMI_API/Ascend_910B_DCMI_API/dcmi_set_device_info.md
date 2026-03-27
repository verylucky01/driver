# dcmi\_set\_device\_info<a name="ZH-CN_TOPIC_0000002517615407"></a>

**函数原型<a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_toc533412077"></a>**

**int dcmi\_set\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, const void \*buf, unsigned int buf\_size\)**

**功能说明<a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_toc533412078"></a>**

设置device的信息的通用接口，对各模块信息进行配置。

**参数说明<a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_toc533412079"></a>**

<a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p10021890"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p10021890"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p6466753"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p6466753"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.28%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p54045009"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p54045009"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="51.7%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p15569626"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p15569626"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p36741947142813"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p36741947142813"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p96741747122818"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p96741747122818"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p46747472287"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p46747472287"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="51.7%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1467413474281"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1467413474281"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p7711145152918"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p7711145152918"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p671116522914"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p671116522914"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1771116572910"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1771116572910"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="51.7%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p144091119151715"><a name="p144091119151715"></a><a name="p144091119151715"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p5522164215178"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p5522164215178"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p5522164215178"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p8522242101715"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p8522242101715"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p17522114220174"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p17522114220174"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p17522114220174"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="51.7%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p179311148415"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p179311148415"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p179311148415"></a>模块cmd信息，执行用于获取对应模块的信息</p>
<p id="p18641935123215"><a name="p18641935123215"></a><a name="p18641935123215"></a>enum dcmi_main_cmd {</p>
<p id="p6641635183210"><a name="p6641635183210"></a><a name="p6641635183210"></a>DCMI_MAIN_CMD_DVPP = 0,</p>
<p id="p11641835113217"><a name="p11641835113217"></a><a name="p11641835113217"></a>DCMI_MAIN_CMD_ISP,</p>
<p id="p126453510320"><a name="p126453510320"></a><a name="p126453510320"></a>DCMI_MAIN_CMD_TS_GROUP_NUM,</p>
<p id="p86453573215"><a name="p86453573215"></a><a name="p86453573215"></a>DCMI_MAIN_CMD_CAN,</p>
<p id="p364163533220"><a name="p364163533220"></a><a name="p364163533220"></a>DCMI_MAIN_CMD_UART,</p>
<p id="p66473583218"><a name="p66473583218"></a><a name="p66473583218"></a>DCMI_MAIN_CMD_UPGRADE = 5,</p>
<p id="p166483523216"><a name="p166483523216"></a><a name="p166483523216"></a>DCMI_MAIN_CMD_UFS,</p>
<p id="p26433563214"><a name="p26433563214"></a><a name="p26433563214"></a>DCMI_MAIN_CMD_OS_POWER,</p>
<p id="p136493519325"><a name="p136493519325"></a><a name="p136493519325"></a>DCMI_MAIN_CMD_LP,</p>
<p id="p36443520320"><a name="p36443520320"></a><a name="p36443520320"></a>DCMI_MAIN_CMD_MEMORY,</p>
<p id="p106463512321"><a name="p106463512321"></a><a name="p106463512321"></a>DCMI_MAIN_CMD_RECOVERY,</p>
<p id="p1464435203212"><a name="p1464435203212"></a><a name="p1464435203212"></a>DCMI_MAIN_CMD_TS,</p>
<p id="p16433563212"><a name="p16433563212"></a><a name="p16433563212"></a>DCMI_MAIN_CMD_CHIP_INF,</p>
<p id="p064173517327"><a name="p064173517327"></a><a name="p064173517327"></a>DCMI_MAIN_CMD_QOS,</p>
<p id="p106463515320"><a name="p106463515320"></a><a name="p106463515320"></a>DCMI_MAIN_CMD_SOC_INFO,</p>
<p id="p46493593220"><a name="p46493593220"></a><a name="p46493593220"></a>DCMI_MAIN_CMD_SILS,</p>
<p id="p9641735183212"><a name="p9641735183212"></a><a name="p9641735183212"></a>DCMI_MAIN_CMD_HCCS,</p>
<p id="p1656073942014"><a name="p1656073942014"></a><a name="p1656073942014"></a>DCMI_MAIN_CMD_HOST_AICPU,</p>
<p id="p1864173593210"><a name="p1864173593210"></a><a name="p1864173593210"></a>DCMI_MAIN_CMD_TEMP = 50,</p>
<p id="p18641835203218"><a name="p18641835203218"></a><a name="p18641835203218"></a>DCMI_MAIN_CMD_SVM,</p>
<p id="p1864135193214"><a name="p1864135193214"></a><a name="p1864135193214"></a>DCMI_MAIN_CMD_VDEV_MNG,</p>
<p id="p56473573215"><a name="p56473573215"></a><a name="p56473573215"></a>DCMI_MAIN_CMD_SEC,</p>
<p id="p141374616226"><a name="p141374616226"></a><a name="p141374616226"></a>DCMI_MAIN_CMD_PCIE = 55,</p>
<p id="p113869303566"><a name="p113869303566"></a><a name="p113869303566"></a>DCMI_MAIN_CMD_SIO = 56,</p>
<p id="p186413351321"><a name="p186413351321"></a><a name="p186413351321"></a>DCMI_MAIN_CMD_EX_COMPUTING = 0x8000,</p>
<p id="p116417356329"><a name="p116417356329"></a><a name="p116417356329"></a>DCMI_MAIN_CMD_DEVICE_SHARE = 0x8001,</p>
<p id="p12529132435912"><a name="p12529132435912"></a><a name="p12529132435912"></a>DCMI_MAIN_CMD_EX_CERT = 0x8003,</p>
<p id="p86483533216"><a name="p86483533216"></a><a name="p86483533216"></a>DCMI_MAIN_CMD_MAX</p>
<p id="p19641735163210"><a name="p19641735163210"></a><a name="p19641735163210"></a>};</p>
<p id="p12961111210401"><a name="p12961111210401"></a><a name="p12961111210401"></a>仅支持如下模块主命令字：</p>
<p id="p11730164003910"><a name="p11730164003910"></a><a name="p11730164003910"></a>DCMI_MAIN_CMD_LP //低功耗模块主命令字</p>
<p id="p630816245918"><a name="p630816245918"></a><a name="p630816245918"></a>DCMI_MAIN_CMD_TS //ts任务调度模块主命令字</p>
<p id="p5295114993915"><a name="p5295114993915"></a><a name="p5295114993915"></a>DCMI_MAIN_CMD_QOS //qos模块主命令字</p>
<p id="p10587195583910"><a name="p10587195583910"></a><a name="p10587195583910"></a></p>
<p id="zh-cn_topic_0000001206307236_p1516935634520"><a name="zh-cn_topic_0000001206307236_p1516935634520"></a><a name="zh-cn_topic_0000001206307236_p1516935634520"></a>DCMI_MAIN_CMD_EX_CERT //证书管理模块主命令字</p>
<p id="p1923195813395"><a name="p1923195813395"></a><a name="p1923195813395"></a>DCMI_MAIN_CMD_DEVICE_SHARE //容器共享主命令字</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_row131655372019"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p19161539205"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p19161539205"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p19161539205"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1816115315209"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1816115315209"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1816115315209"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1216853142018"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1216853142018"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1216853142018"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="51.7%" headers="mcps1.1.5.1.4 "><p id="p96461212173514"><a name="p96461212173514"></a><a name="p96461212173514"></a>详细参见子章节中的功能说明。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_row9864105062010"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p8864185062018"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p8864185062018"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p8864185062018"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p886417500202"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p886417500202"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p886417500202"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1386445092020"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1386445092020"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1386445092020"></a>const void *</p>
</td>
<td class="cellrowborder" valign="top" width="51.7%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p15864145012200"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p15864145012200"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p15864145012200"></a>用于配置相应设备的配置信息。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_row177483489204"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p7749184816202"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p7749184816202"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p7749184816202"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p0749104811207"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p0749104811207"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p0749104811207"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1374944862018"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1374944862018"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p1374944862018"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="51.7%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p074915488209"><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p074915488209"></a><a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_p074915488209"></a>buf数组的长度。</p>
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

**约束说明<a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_toc533412082"></a>**

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

**调用示例<a name="zh-cn_topic_0000001206307236_zh-cn_topic_0000001178054672_zh-cn_topic_0000001147964365_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int buf = 0;
unsigned int size = sizeof(int);
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_DVPP, 0, &buf, size);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

## DCMI\_MAIN\_CMD\_LP命令说明<a name="ZH-CN_TOPIC_0000002485295386"></a>

**函数原型<a name="section152647575211"></a>**

**int dcmi\_set\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, const void \*buf, unsigned int buf\_size\)**

**功能说明<a name="section172644579210"></a>**

设置LP相关配置。

**参数说明<a name="section1626515570213"></a>**

<a name="table22732571629"></a>
<table><thead align="left"><tr id="row530855719214"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p143081457622"><a name="p143081457622"></a><a name="p143081457622"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p113081579216"><a name="p113081579216"></a><a name="p113081579216"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.16%" id="mcps1.1.5.1.3"><p id="p1309357222"><a name="p1309357222"></a><a name="p1309357222"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="51.519999999999996%" id="mcps1.1.5.1.4"><p id="p1230925714210"><a name="p1230925714210"></a><a name="p1230925714210"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row11309175718219"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p173091571528"><a name="p173091571528"></a><a name="p173091571528"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p23091157422"><a name="p23091157422"></a><a name="p23091157422"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p6309145718212"><a name="p6309145718212"></a><a name="p6309145718212"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p43098579215"><a name="p43098579215"></a><a name="p43098579215"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row183091657724"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p430910572213"><a name="p430910572213"></a><a name="p430910572213"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p83092571127"><a name="p83092571127"></a><a name="p83092571127"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p93093571822"><a name="p93093571822"></a><a name="p93093571822"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p15309157129"><a name="p15309157129"></a><a name="p15309157129"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下</p>
<p id="p20309757127"><a name="p20309757127"></a><a name="p20309757127"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1586518222174"><a name="p1586518222174"></a><a name="p1586518222174"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row1530917576214"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p1830975719210"><a name="p1830975719210"></a><a name="p1830975719210"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p03091257720"><a name="p03091257720"></a><a name="p03091257720"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p1630911571521"><a name="p1630911571521"></a><a name="p1630911571521"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p163094574212"><a name="p163094574212"></a><a name="p163094574212"></a>DCMI_MAIN_CMD_LP</p>
</td>
</tr>
<tr id="row19309757129"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p143091657124"><a name="p143091657124"></a><a name="p143091657124"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p1030912570211"><a name="p1030912570211"></a><a name="p1030912570211"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p1930965713212"><a name="p1930965713212"></a><a name="p1930965713212"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><pre class="codeblock" id="codeblock866420763315"><a name="codeblock866420763315"></a><a name="codeblock866420763315"></a>typedef enum {
    //获取AICORE电压电流的寄存器值
    DCMI_LP_SUB_CMD_AICORE_VOLTAGE_CURRENT = 0,
    //获取HYBIRD电压电流的寄存器值
    DCMI_LP_SUB_CMD_HYBIRD_VOLTAGE_CURRENT,
    //获取CPU电压电流的寄存器值
    DCMI_LP_SUB_CMD_TAISHAN_VOLTAGE_CURRENT,
    //获取DDR电压电流的寄存器值
    DCMI_LP_SUB_CMD_DDR_VOLTAGE_CURRENT,
    //获取ACG调频计数值
    DCMI_LP_SUB_CMD_ACG,
    //获取低功耗总状态
    DCMI_LP_SUB_CMD_STATUS,
    //获取所有工作档位
    DCMI_LP_SUB_CMD_TOPS_DETAILS,
    //设置工作档位
    DCMI_LP_SUB_CMD_SET_WORK_TOPS,
    //获取当前工作档位
    DCMI_LP_SUB_CMD_GET_WORK_TOPS,
    //获取当前降频原因
    DCMI_LP_SUB_CMD_AICORE_FREQREDUC_CAUSE,
    //获取功耗信息
    DCMI_LP_SUB_CMD_GET_POWER_INFO,
    //设置IDLE模式开关
    DCMI_LP_SUB_CMD_SET_IDLE_SWITCH,
    DCMI_LP_SUB_CMD_MAX,
} DCMI_LP_SUB_CMD;</pre>
<p id="p9830153811229"><a name="p9830153811229"></a><a name="p9830153811229"></a>当前仅支持DCMI_LP_SUB_CMD_SET_IDLE_SWITCH命令。</p>
</td>
</tr>
<tr id="row1030917571727"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p123108571525"><a name="p123108571525"></a><a name="p123108571525"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p13102579214"><a name="p13102579214"></a><a name="p13102579214"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p331005717211"><a name="p331005717211"></a><a name="p331005717211"></a>const void *</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p12310115715214"><a name="p12310115715214"></a><a name="p12310115715214"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row83101557124"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p1231019570213"><a name="p1231019570213"></a><a name="p1231019570213"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p12310115715213"><a name="p12310115715213"></a><a name="p12310115715213"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p1531045710217"><a name="p1531045710217"></a><a name="p1531045710217"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p13107576214"><a name="p13107576214"></a><a name="p13107576214"></a>buf数组的长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section2014284413016"></a>**

<a name="table85163468101"></a>
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

**异常处理<a name="section135813338518"></a>**

无。

**约束说明<a name="section13120154195114"></a>**

**表 1**  sub\_cmd对应的buf格式

<a name="table152227367311"></a>
<table><thead align="left"><tr id="row102361036833"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p142361636536"><a name="p142361636536"></a><a name="p142361636536"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p172364361439"><a name="p172364361439"></a><a name="p172364361439"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p22369362034"><a name="p22369362034"></a><a name="p22369362034"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p102362362037"><a name="p102362362037"></a><a name="p102362362037"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row923653618312"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p02366360313"><a name="p02366360313"></a><a name="p02366360313"></a>DCMI_LP_SUB_CMD_SET_IDLE_SWITCH</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p132368365313"><a name="p132368365313"></a><a name="p132368365313"></a>unsigned char</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p202374363319"><a name="p202374363319"></a><a name="p202374363319"></a>长度为：sizeof(unsigned char)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1023715361335"><a name="p1023715361335"></a><a name="p1023715361335"></a>0：表示idle模式关闭</p>
<p id="p32371364310"><a name="p32371364310"></a><a name="p32371364310"></a>1：表示idle模式开启</p>
<p id="p823733620312"><a name="p823733620312"></a><a name="p823733620312"></a>其他值无效。</p>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

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

**调用示例<a name="section168848176545"></a>**

```
…
int ret;
int card_id = 0;
int device_id = 0;
DCMI_MAIN_CMD main_cmd = DCMI_MAIN_CMD_LP;
unsigned int sub_cmd = DCMI_LP_SUB_CMD_SET_WORK_TOPS;
DCMI_LP_WORK_TOPS_STRU set_works_tops_lv = {0};
set_works_tops_lv.work_tops = 1;
set_works_tops_lv.is_in_flash = 0;
unsigned int size = sizeof(DCMI_LP_WORK_TOPS_STRU);
ret = dcmi_set_device_info(card_id, device_id, main_cmd, sub_cmd, (void *)&set_works_tops_lv, size);
if (ret) {
// todo
}
// todo
…
```


## DCMI\_MAIN\_CMD\_QOS命令说明<a name="ZH-CN_TOPIC_0000002485455426"></a>

**函数原型<a name="section1042115192610"></a>**

**int dcmi\_set\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, const void \*buf, unsigned int buf\_size\)**

**功能说明<a name="section204318582611"></a>**

配置QoS相关信息，包括配置mpamid对应的QoS信息、配置master对应的QoS信息、配置带宽统计功能。

**参数说明<a name="section204395112610"></a>**

<a name="table14836522617"></a>
<table><thead align="left"><tr id="row81941750263"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p101942582619"><a name="p101942582619"></a><a name="p101942582619"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p1819414572612"><a name="p1819414572612"></a><a name="p1819414572612"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.16%" id="mcps1.1.5.1.3"><p id="p9194205132614"><a name="p9194205132614"></a><a name="p9194205132614"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="51.519999999999996%" id="mcps1.1.5.1.4"><p id="p1119511592612"><a name="p1119511592612"></a><a name="p1119511592612"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row21958532610"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p121951513267"><a name="p121951513267"></a><a name="p121951513267"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p11956515261"><a name="p11956515261"></a><a name="p11956515261"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p1919510562619"><a name="p1919510562619"></a><a name="p1919510562619"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p1619510511265"><a name="p1619510511265"></a><a name="p1619510511265"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row4195175152616"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p191957519266"><a name="p191957519266"></a><a name="p191957519266"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p1319517522613"><a name="p1319517522613"></a><a name="p1319517522613"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p419514512261"><a name="p419514512261"></a><a name="p419514512261"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p161951552616"><a name="p161951552616"></a><a name="p161951552616"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p8195155132618"><a name="p8195155132618"></a><a name="p8195155132618"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1778223001716"><a name="p1778223001716"></a><a name="p1778223001716"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row1419520512612"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p7195135182617"><a name="p7195135182617"></a><a name="p7195135182617"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p2019515513267"><a name="p2019515513267"></a><a name="p2019515513267"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p1519518513263"><a name="p1519518513263"></a><a name="p1519518513263"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p619605112614"><a name="p619605112614"></a><a name="p619605112614"></a>DCMI_MAIN_CMD_QOS</p>
</td>
</tr>
<tr id="row17196195102619"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p519619517267"><a name="p519619517267"></a><a name="p519619517267"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p819611592612"><a name="p819611592612"></a><a name="p819611592612"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p17196155202616"><a name="p17196155202616"></a><a name="p17196155202616"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><pre class="codeblock" id="codeblock147418363249"><a name="codeblock147418363249"></a><a name="codeblock147418363249"></a>typedef enum {
//配置指定mpamid的信息
DCMI_QOS_SUB_MATA_CONFIG,
//配置指定master的信息
DCMI_QOS_SUB_MASTER_CONFIG,
//配置带宽的统计功能
 DCMI_QOS_SUB_BW_DATA,
//配置通用信息
DCMI_QOS_SUB_GLOBAL_CONFIG,
} DCMI_QOS_SUB_INFO;</pre>
</td>
</tr>
<tr id="row201979522617"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p191972511267"><a name="p191972511267"></a><a name="p191972511267"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p31971150262"><a name="p31971150262"></a><a name="p31971150262"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p61971451268"><a name="p61971451268"></a><a name="p61971451268"></a>const void *</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p141972512616"><a name="p141972512616"></a><a name="p141972512616"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row31971555262"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p18197158261"><a name="p18197158261"></a><a name="p18197158261"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p1419719510263"><a name="p1419719510263"></a><a name="p1419719510263"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.16%" headers="mcps1.1.5.1.3 "><p id="p1019725162615"><a name="p1019725162615"></a><a name="p1019725162615"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="51.519999999999996%" headers="mcps1.1.5.1.4 "><p id="p1619712516264"><a name="p1619712516264"></a><a name="p1619712516264"></a>buf数组的长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section2014284413016"></a>**

<a name="table85163468101"></a>
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

**异常处理<a name="section122651535145210"></a>**

无。

**约束说明<a name="section72205384522"></a>**

**表 1**  sub\_cmd对应的buf格式

<a name="table590205182617"></a>
<table><thead align="left"><tr id="row919713512265"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p619712522612"><a name="p619712522612"></a><a name="p619712522612"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p1819814542619"><a name="p1819814542619"></a><a name="p1819814542619"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p1119815515263"><a name="p1119815515263"></a><a name="p1119815515263"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p31981351265"><a name="p31981351265"></a><a name="p31981351265"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row819805122613"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p7198115112610"><a name="p7198115112610"></a><a name="p7198115112610"></a>DCMI_QOS_SUB_MATA_CONFIG</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p14198155262"><a name="p14198155262"></a><a name="p14198155262"></a>struct dcmi_qos_mata_config {</p>
<p id="p10198350263"><a name="p10198350263"></a><a name="p10198350263"></a>int mpamid;</p>
<p id="p11985511263"><a name="p11985511263"></a><a name="p11985511263"></a>unsigned int bw_high;</p>
<p id="p16198165162618"><a name="p16198165162618"></a><a name="p16198165162618"></a>unsigned int bw_low;</p>
<p id="p171988510267"><a name="p171988510267"></a><a name="p171988510267"></a>int hardlimit;</p>
<p id="p181982532614"><a name="p181982532614"></a><a name="p181982532614"></a>int reserved[DCMI_QOS_CFG_RESERVED_LEN];</p>
<p id="p0198155142616"><a name="p0198155142616"></a><a name="p0198155142616"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1419814519266"><a name="p1419814519266"></a><a name="p1419814519266"></a>sizeof(dcmi_qos_mata_config)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><a name="ul101981155262"></a><a name="ul101981155262"></a><ul id="ul101981155262"><li>mpamid：取值范围是[0,127]。</li><li>bw_high：上水线(GB/s)，取值范围是[0,1638]。【注意】取值范围上限随颗粒主频变化，计算公式为：bw_high_max=ddr_freq * 2 * max_chn * 8 / 1000，以实际主频和通道数为准。</li><li>bw_low：下水线(GB/s)，取值范围是[0, bw_high]</li><li>hardlimit ：<a name="ul1282591431120"></a><a name="ul1282591431120"></a><ul id="ul1282591431120"><li>1表示开启</li><li>0表示不开启</li></ul>
</li></ul>
<p id="p8557317102714"><a name="p8557317102714"></a><a name="p8557317102714"></a>用户通过该接口读取水线，可能同先前用户配置值存在误差。误差值计算方式为：处理器最大带宽/MAX_REG_VALUE。其中，MAX_REG_VALUE取值为1024。</p>
</td>
</tr>
<tr id="row51993532614"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p15199950269"><a name="p15199950269"></a><a name="p15199950269"></a>DCMI_QOS_SUB_MASTER_CONFIG</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1219910515263"><a name="p1219910515263"></a><a name="p1219910515263"></a>struct dcmi_qos_master_config {</p>
<p id="p121991756266"><a name="p121991756266"></a><a name="p121991756266"></a>int master;</p>
<p id="p219915162618"><a name="p219915162618"></a><a name="p219915162618"></a>int mpamid;</p>
<p id="p0199155142610"><a name="p0199155142610"></a><a name="p0199155142610"></a>int qos;</p>
<p id="p219913517269"><a name="p219913517269"></a><a name="p219913517269"></a>int pmg;</p>
<p id="p7199159265"><a name="p7199159265"></a><a name="p7199159265"></a>unsigned long long bitmap[4]; /* max support 64 * 4  */</p>
<p id="p21991651268"><a name="p21991651268"></a><a name="p21991651268"></a>int reserved[DCMI_QOS_CFG_RESERVED_LEN];</p>
<p id="p31991553269"><a name="p31991553269"></a><a name="p31991553269"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p191999513267"><a name="p191999513267"></a><a name="p191999513267"></a>sizeof(dcmi_qos_master_config)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><a name="ul1519965122612"></a><a name="ul1519965122612"></a><ul id="ul1519965122612"><li>master：master ID，支持配置的项为：vdec=1,vpc=2,jpge=3,jpgd=4,pcie=7,sdma=13</li><li>mpamid：取值范围是[0,127]。</li><li>qos：带宽调度优先级，取值范围[0,7]，0作为hardlimit专用qos，7为调度绿色通道qos</li><li>pmg：mpamid分组，取值范围是[0,3]</li><li>bitmap：因框架限制，不支持。</li></ul>
</td>
</tr>
<tr id="row132000512266"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p320020518263"><a name="p320020518263"></a><a name="p320020518263"></a>DCMI_QOS_SUB_BW_DATA</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p12812438112"><a name="p12812438112"></a><a name="p12812438112"></a>struct dcmi_qos_bw_config</p>
<p id="p14281743121113"><a name="p14281743121113"></a><a name="p14281743121113"></a>{</p>
<p id="p628124319115"><a name="p628124319115"></a><a name="p628124319115"></a>u8 mode;</p>
<p id="p16282643161119"><a name="p16282643161119"></a><a name="p16282643161119"></a>u8 state;</p>
<p id="p1928254312115"><a name="p1928254312115"></a><a name="p1928254312115"></a>u8 cnt;</p>
<p id="p1528264311112"><a name="p1528264311112"></a><a name="p1528264311112"></a>u8 method;</p>
<p id="p6282164361119"><a name="p6282164361119"></a><a name="p6282164361119"></a>u32 interval;</p>
<p id="p1282943101118"><a name="p1282943101118"></a><a name="p1282943101118"></a>u32 target_set[16];</p>
<p id="p15282154351114"><a name="p15282154351114"></a><a name="p15282154351114"></a>int reserved_1[8];</p>
<p id="p1228294316114"><a name="p1228294316114"></a><a name="p1228294316114"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p16795420161114"><a name="p16795420161114"></a><a name="p16795420161114"></a>sizeof(dcmi_qos_bw_config)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p72381261113"><a name="p72381261113"></a><a name="p72381261113"></a>interval：带宽采样时间间隔（单位us），需要配置大于1000。</p>
<a name="ul14758119151018"></a><a name="ul14758119151018"></a><ul id="ul14758119151018"><li>mode：带宽统计的模式，0表示自动模式，1表示手动模式。该配置只有在MATA/DHA侧生效，DDRC侧配置不生效且只支持手动模式。</li><li>method：选择带宽统计的节点，0表示在MATA/DHA侧对带宽进行统计。</li><li>state：带宽采样下发命令。<a name="ul19778747111012"></a><a name="ul19778747111012"></a><ul id="ul19778747111012"><li>2表示开启，此时读取的值有效。</li><li>1表示初始化。</li><li>0表示关闭。</li></ul>
</li><li>target_set：采样对象的mpamid，最多可存放16个，实际支持的数量与昇腾AI处理器有关。</li><li>cnt：有效带宽统计对象的数量，最多可支持16个。</li></ul>
</td>
</tr>
<tr id="row152004512610"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p3200558269"><a name="p3200558269"></a><a name="p3200558269"></a>DCMI_QOS_SUB_GLOBAL_CONFIG</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p122007517269"><a name="p122007517269"></a><a name="p122007517269"></a>struct dcmi_qos_gbl_config {</p>
<p id="p820012512616"><a name="p820012512616"></a><a name="p820012512616"></a>unsigned int enable;</p>
<p id="p820055122617"><a name="p820055122617"></a><a name="p820055122617"></a>unsigned int autoqos_fuse_en;         /* 0--enable, 1--disable */</p>
<p id="p720018502617"><a name="p720018502617"></a><a name="p720018502617"></a>unsigned int mpamqos_fuse_mode;       /* 0--average, 1--max, 2--replace */</p>
<p id="p620119511260"><a name="p620119511260"></a><a name="p620119511260"></a>unsigned int mpam_subtype;            /* 0--all, 1--wr, 2--rd, 3--none */</p>
<p id="p16201353266"><a name="p16201353266"></a><a name="p16201353266"></a>int reserved[DCMI_QOS_CFG_RESERVED_LEN];</p>
<p id="p172015562613"><a name="p172015562613"></a><a name="p172015562613"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p72011655263"><a name="p72011655263"></a><a name="p72011655263"></a>sizeof(dcmi_qos_gbl_config)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><a name="ul1520114516263"></a><a name="ul1520114516263"></a><ul id="ul1520114516263"><li>enable：是否使能QoS功能。<a name="ul9734199111712"></a><a name="ul9734199111712"></a><ul id="ul9734199111712"><li>0表示不使能。</li><li>1表示使能。</li></ul>
</li><li>autoqos_fuse_en：qos的融合开关。当前不支持设置autoqos_fuse_en，默认值为1。<a name="ul483832316173"></a><a name="ul483832316173"></a><ul id="ul483832316173"><li>0表示关闭qos融合。</li><li>1表示开始qos融合。</li></ul>
</li><li>mpamqos_fuse_mode：qos的融合模式，autoqos_fuse_en开启的条件下生效。<a name="ul469217432179"></a><a name="ul469217432179"></a><ul id="ul469217432179"><li>0表示均值融合。</li><li>1表示取随路qos和mpamqos之间的最大值作为融合结果。</li><li>2表示使用随路qos替换mpamqos。</li></ul>
</li><li>mpam_subtype：带宽统计的模式。<a name="ul1691317543171"></a><a name="ul1691317543171"></a><ul id="ul1691317543171"><li>0表示统计读+写带宽。</li><li>1表示统计写带宽。</li><li>2表示统计读带宽。</li></ul>
</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

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

**调用示例<a name="section854716251547"></a>**

```
…
int32_t ret;
int card_id = 0;
int device_id = 0;
// 配置mpamid上下水线，用于带宽限制
struct qos_mata_config mataCfg = {0};
mataCfg.mpamid = 0; // 举例，mpamid配置为0
mataCfg.bw_high = 20;
mataCfg.bw_low = 10;
mataCfg.hardlimit = 1;
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_QOS,static_cast<uint32_t>(DCMI_QOS_SUB_MATA_CONFIG),
static_cast<void *>(&mataCfg), sizeof(struct qos_mata_config));
if (ret != 0) {
printf("[dev:%d]set mata qos config failed, ret = %d\n", devId, ret);
return ret;
}
// 配置master对应的mpamid、qos、pmg
struct qos_master_config masterCfg = {0};
masterCfg.master = 0;
masterCfg.mpamid = 1; // 举例，mpamid配置为1
masterCfg.qos = 3;
masterCfg.pmg = 0;
masterCfg.bitmap[0] = 0xffff0000;
masterCfg.bitmap[1] = 0xffff;
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_QOS, static_cast<uint32_t>(DCMI_QOS_SUB_MASTER_CONFIG),
static_cast<void *>(&cfg), sizeof(struct qos_master_config));
if (ret != 0) {
printf("[dev:%d]set master qos config failed, ret = %d\n", devId, ret);
return ret;
}
// 以手动模式开启带宽监控：需指定监控对象
struct qos_bw_config bwCfg = {0};
bwCfg.mode = 1;
bwCfg.state = 2;
bwCfg.target_set[0] = 1; // 举例,监控对象[0]配置为1
bwCfg.target_set[1] = 2; // 举例,监控对象[1]配置为2
bwCfg.cnt = 2;
bwCfg.interval = 1000;
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_QOS, DCMI_QOS_SUB_BW_DATA, \
static_cast<void *>(&(bwCfg)), sizeof(struct qos_bw_config));
if (ret != 0) {
printf("[dev:%d]set mbwu failed, ret = %d\n", devId, ret);
return ret;
}
// 终止带宽监控
bwCfg.state = 0;
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_QOS, DCMI_QOS_SUB_BW_DATA, \
static_cast<void *>(&(bwCfg)), sizeof(bwCfg));
if (ret != 0) {
printf("[dev:%d]set mbwu failed, ret = %d\n", devId, ret);
return ret;
}
// 以自动模式开启带宽监控：无需指定监控对象，程序自动读取配置过的监控对象
bwCfg.mode = 0;
bwCfg.interval = 1000;
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_QOS, DCMI_QOS_SUB_BW_DATA, \
static_cast<void *>(&(bwCfg)), sizeof(bwCfg));
if (ret != 0) {
printf("[dev:%d]set mbwu failed, ret = %d\n", devId, ret);
return ret;
}
// 终止带宽监控
bwCfg.state = 0;
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_QOS, DCMI_QOS_SUB_BW_DATA, \
static_cast<void *>(&(bwCfg)), sizeof(bwCfg));
if (ret != 0) {
printf("[dev:%d]set mbwu failed, ret = %d\n", devId, ret);
return ret;
}
// 配置全局开关：开启qos功能
struct qos_gbl_config gblCfg = {0};
gblCfg.enable = 1;
ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_QOS, static_cast<uint32_t>(DCMI_QOS_SUB_GLOBAL_CONFIG),
static_cast<void *>(&gblCfg), sizeof(struct qos_gbl_config));
if (ret != 0) {
QOS_LOG_ERROR("[dev:%d]set gbl qos config failed, ret = %d\n", devId, ret);
return ret;
}
…
```


## DCMI\_MAIN\_CMD\_EX\_CERT命令说明<a name="ZH-CN_TOPIC_0000002485455416"></a>

**函数原型<a name="section280203719273"></a>**

**int dcmi\_set\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, const void \*buf, unsigned int buf\_size\)**

**功能说明<a name="section7351204942711"></a>**

用于配置TLS证书。

**参数说明<a name="section13117595271"></a>**

<a name="table1212611922719"></a>
<table><thead align="left"><tr id="row1725841910272"><th class="cellrowborder" valign="top" width="10.100000000000001%" id="mcps1.1.5.1.1"><p id="p6258151914271"><a name="p6258151914271"></a><a name="p6258151914271"></a><strong id="b17258111915278"><a name="b17258111915278"></a><a name="b17258111915278"></a>参数名</strong></p>
</th>
<th class="cellrowborder" valign="top" width="8.08%" id="mcps1.1.5.1.2"><p id="p325881915274"><a name="p325881915274"></a><a name="p325881915274"></a><strong id="b32581619142716"><a name="b32581619142716"></a><a name="b32581619142716"></a>输入/输出</strong></p>
</th>
<th class="cellrowborder" valign="top" width="19.189999999999998%" id="mcps1.1.5.1.3"><p id="p825881922713"><a name="p825881922713"></a><a name="p825881922713"></a><strong id="b925801962719"><a name="b925801962719"></a><a name="b925801962719"></a>类型</strong></p>
</th>
<th class="cellrowborder" valign="top" width="62.629999999999995%" id="mcps1.1.5.1.4"><p id="p1925851972714"><a name="p1925851972714"></a><a name="p1925851972714"></a><strong id="b1025911192270"><a name="b1025911192270"></a><a name="b1025911192270"></a>描述</strong></p>
</th>
</tr>
</thead>
<tbody><tr id="row2259131912277"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p10259121972712"><a name="p10259121972712"></a><a name="p10259121972712"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="8.08%" headers="mcps1.1.5.1.2 "><p id="p425991914275"><a name="p425991914275"></a><a name="p425991914275"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.189999999999998%" headers="mcps1.1.5.1.3 "><p id="p20259111918272"><a name="p20259111918272"></a><a name="p20259111918272"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="62.629999999999995%" headers="mcps1.1.5.1.4 "><p id="p7259101910272"><a name="p7259101910272"></a><a name="p7259101910272"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row425911918273"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p17259111992716"><a name="p17259111992716"></a><a name="p17259111992716"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="8.08%" headers="mcps1.1.5.1.2 "><p id="p122590195273"><a name="p122590195273"></a><a name="p122590195273"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.189999999999998%" headers="mcps1.1.5.1.3 "><p id="p725912195278"><a name="p725912195278"></a><a name="p725912195278"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="62.629999999999995%" headers="mcps1.1.5.1.4 "><p id="p4259141982712"><a name="p4259141982712"></a><a name="p4259141982712"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p19259121912278"><a name="p19259121912278"></a><a name="p19259121912278"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p4865204631719"><a name="p4865204631719"></a><a name="p4865204631719"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row1225918191279"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p13259131972713"><a name="p13259131972713"></a><a name="p13259131972713"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="8.08%" headers="mcps1.1.5.1.2 "><p id="p14259161942711"><a name="p14259161942711"></a><a name="p14259161942711"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.189999999999998%" headers="mcps1.1.5.1.3 "><p id="p182599197275"><a name="p182599197275"></a><a name="p182599197275"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="62.629999999999995%" headers="mcps1.1.5.1.4 "><p id="p112592191279"><a name="p112592191279"></a><a name="p112592191279"></a>DCMI_MAIN_CMD_EX_CERT</p>
</td>
</tr>
<tr id="row15259319182712"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p162602195273"><a name="p162602195273"></a><a name="p162602195273"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="8.08%" headers="mcps1.1.5.1.2 "><p id="p4260161911275"><a name="p4260161911275"></a><a name="p4260161911275"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.189999999999998%" headers="mcps1.1.5.1.3 "><p id="p526081962717"><a name="p526081962717"></a><a name="p526081962717"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="62.629999999999995%" headers="mcps1.1.5.1.4 "><p id="p860220171714"><a name="p860220171714"></a><a name="p860220171714"></a>typedef enum {</p>
<p id="p86022010177"><a name="p86022010177"></a><a name="p86022010177"></a>DCMI_CERT_SUB_CMD_INIT_TLS_PUB_KEY = 0,//初始化TLS证书（用于获取CSR）</p>
<p id="p360172071712"><a name="p360172071712"></a><a name="p360172071712"></a>DCMI_CERT_SUB_CMD_INIT_RESERVE,//初始化预留字段</p>
<p id="p060172071719"><a name="p060172071719"></a><a name="p060172071719"></a>DCMI_CERT_SUB_CMD_TLS_CERT_INFO,//预置/更新/获取TLS证书信息</p>
<p id="p186062061716"><a name="p186062061716"></a><a name="p186062061716"></a>DCMI_CERT_SUB_CMD_MAX,</p>
<p id="p66012200175"><a name="p66012200175"></a><a name="p66012200175"></a>} DCMI_EX_CERT_SUB_CMD;</p>
<p id="p102606193279"><a name="p102606193279"></a><a name="p102606193279"></a>只支持DCMI_CERT_SUB_CMD_TLS_CERT_INFO</p>
</td>
</tr>
<tr id="row14260919172712"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p5260719122714"><a name="p5260719122714"></a><a name="p5260719122714"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="8.08%" headers="mcps1.1.5.1.2 "><p id="p826014191272"><a name="p826014191272"></a><a name="p826014191272"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.189999999999998%" headers="mcps1.1.5.1.3 "><p id="p10260181942719"><a name="p10260181942719"></a><a name="p10260181942719"></a>const void *</p>
</td>
<td class="cellrowborder" valign="top" width="62.629999999999995%" headers="mcps1.1.5.1.4 "><p id="p1226013197274"><a name="p1226013197274"></a><a name="p1226013197274"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row92604199273"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p112604198273"><a name="p112604198273"></a><a name="p112604198273"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="8.08%" headers="mcps1.1.5.1.2 "><p id="p112608196276"><a name="p112608196276"></a><a name="p112608196276"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.189999999999998%" headers="mcps1.1.5.1.3 "><p id="p22605196271"><a name="p22605196271"></a><a name="p22605196271"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="62.629999999999995%" headers="mcps1.1.5.1.4 "><p id="p12260719112716"><a name="p12260719112716"></a><a name="p12260719112716"></a>buf数组的长度。</p>
</td>
</tr>
</tbody>
</table>

**表 1**  sub\_cmd对应的buf格式

<a name="table20149119132713"></a>
<table><thead align="left"><tr id="row62609194278"><th class="cellrowborder" valign="top" width="26.02%" id="mcps1.2.5.1.1"><p id="p02603193273"><a name="p02603193273"></a><a name="p02603193273"></a><strong id="b426091952718"><a name="b426091952718"></a><a name="b426091952718"></a>sub_cmd</strong></p>
</th>
<th class="cellrowborder" valign="top" width="42.11%" id="mcps1.2.5.1.2"><p id="p92617193277"><a name="p92617193277"></a><a name="p92617193277"></a><strong id="b152611019112716"><a name="b152611019112716"></a><a name="b152611019112716"></a>buf对应的数据类型</strong></p>
</th>
<th class="cellrowborder" valign="top" width="22.900000000000002%" id="mcps1.2.5.1.3"><p id="p2026117191273"><a name="p2026117191273"></a><a name="p2026117191273"></a><strong id="b1261191916273"><a name="b1261191916273"></a><a name="b1261191916273"></a>size</strong></p>
</th>
<th class="cellrowborder" valign="top" width="8.97%" id="mcps1.2.5.1.4"><p id="p1226114195270"><a name="p1226114195270"></a><a name="p1226114195270"></a><strong id="b14261151932714"><a name="b14261151932714"></a><a name="b14261151932714"></a>参数说明</strong></p>
</th>
</tr>
</thead>
<tbody><tr id="row1126110195279"><td class="cellrowborder" valign="top" width="26.02%" headers="mcps1.2.5.1.1 "><p id="p202611619122710"><a name="p202611619122710"></a><a name="p202611619122710"></a>DCMI_CERT_SUB_CMD_TLS_CERT_INFO</p>
</td>
<td class="cellrowborder" valign="top" width="42.11%" headers="mcps1.2.5.1.2 "><p id="p5261619102711"><a name="p5261619102711"></a><a name="p5261619102711"></a>struct dcmi_certs_chain_data {</p>
<p id="p16261171910275"><a name="p16261171910275"></a><a name="p16261171910275"></a>unsigned int count; //证书数量</p>
<p id="p132611196275"><a name="p132611196275"></a><a name="p132611196275"></a>unsigned int data_len[MAX_CERT_COUNT]; //证书长度</p>
<p id="p2026191912717"><a name="p2026191912717"></a><a name="p2026191912717"></a>unsigned char data[0]; //证书内容</p>
<p id="p1526115198272"><a name="p1526115198272"></a><a name="p1526115198272"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="22.900000000000002%" headers="mcps1.2.5.1.3 "><p id="p1261319192718"><a name="p1261319192718"></a><a name="p1261319192718"></a>sizeof(struct dcmi_certs_chain_data) + cert_count * NPU_CERT_MAX_SIZE</p>
</td>
<td class="cellrowborder" valign="top" width="8.97%" headers="mcps1.2.5.1.4 "><p id="p14261181922711"><a name="p14261181922711"></a><a name="p14261181922711"></a>无</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section5782161012308"></a>**

<a name="table201555196276"></a>
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

**异常处理<a name="section191916302309"></a>**

无

**约束说明<a name="section598124210309"></a>**

**表 2** 不同部署场景下的支持情况

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

**调用示例<a name="section14483105193112"></a>**

```
…
int ret;
int card_id = 0;
int device_id = 0;
struct dcmi_certs_chain_data certs_chain_data ={0};
DCMI_MAIN_CMD main_cmd = DCMI_MAIN_CMD_EX_CERT;
unsigned int sub_cmd = DCMI_CERT_SUB_CMD_INIT_TLS_PUB_KEY;
unsigned int size = sizeof(struct dcmi_certs_chain_data) + cert_count * NPU_CERT_MAX_SIZE;
ret = dcmi_set_device_info(card_id, device_id, main_cmd, sub_cmd, (void *)&certs_chain_data, size);
if (ret) {
    // todo
}
// todo
…
```


## DCMI\_MAIN\_CMD\_TS命令说明<a name="ZH-CN_TOPIC_0000002517535317"></a>

**函数原型<a name="section782265716308"></a>**

**int dcmi\_set\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, const void \*buf, unsigned int buf\_size\)**

**功能说明<a name="section1181162344514"></a>**

配置系统中TS相关信息。

**参数说明<a name="section188227576305"></a>**

<a name="table584175773016"></a>
<table><thead align="left"><tr id="row158991057133012"><th class="cellrowborder" valign="top" width="17.17171717171717%" id="mcps1.1.5.1.1"><p id="p1189945783015"><a name="p1189945783015"></a><a name="p1189945783015"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.151515151515152%" id="mcps1.1.5.1.2"><p id="p13899165713012"><a name="p13899165713012"></a><a name="p13899165713012"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.19191919191919%" id="mcps1.1.5.1.3"><p id="p789919571306"><a name="p789919571306"></a><a name="p789919571306"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.484848484848484%" id="mcps1.1.5.1.4"><p id="p38991757123015"><a name="p38991757123015"></a><a name="p38991757123015"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14899115710307"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p289935717300"><a name="p289935717300"></a><a name="p289935717300"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p158990579308"><a name="p158990579308"></a><a name="p158990579308"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p890025715306"><a name="p890025715306"></a><a name="p890025715306"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p1290095773011"><a name="p1290095773011"></a><a name="p1290095773011"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1990019577303"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p109001457133015"><a name="p109001457133015"></a><a name="p109001457133015"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p1900175783013"><a name="p1900175783013"></a><a name="p1900175783013"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p13900185710307"><a name="p13900185710307"></a><a name="p13900185710307"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p18900957193015"><a name="p18900957193015"></a><a name="p18900957193015"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p390015577304"><a name="p390015577304"></a><a name="p390015577304"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p20312175112171"><a name="p20312175112171"></a><a name="p20312175112171"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row19900125763017"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p18900657173012"><a name="p18900657173012"></a><a name="p18900657173012"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p5900155783016"><a name="p5900155783016"></a><a name="p5900155783016"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p89002574308"><a name="p89002574308"></a><a name="p89002574308"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p590010578307"><a name="p590010578307"></a><a name="p590010578307"></a>DCMI_MAIN_CMD_TS。</p>
</td>
</tr>
<tr id="row0900155743018"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p139001157183018"><a name="p139001157183018"></a><a name="p139001157183018"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p18900165743017"><a name="p18900165743017"></a><a name="p18900165743017"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p1690018576308"><a name="p1690018576308"></a><a name="p1690018576308"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><pre class="codeblock" id="zh-cn_topic_0000002481387068_codeblock84586272352"><a name="zh-cn_topic_0000002481387068_codeblock84586272352"></a><a name="zh-cn_topic_0000002481387068_codeblock84586272352"></a>typedef enum {
// 获取 AICORE 的单核利用率，正常值范围0-100
DCMI_TS_SUB_CMD_AICORE_UTILIZATION_RATE = 0,
// 获取 VECTOR CORE 的单核利用率/获取AICORE单核中Vector单元利用率，正常值范围0-100
DCMI_TS_SUB_CMD_VECTORCORE_UTILIZATION_RATE,
// 获取FFTS或者FFTS+的类型，0表示FFTS，1表示FFTS+
DCMI_TS_SUB_CMD_FFTS_TYPE,
// 设置硬件屏蔽AICORE ERR的掩码
DCMI_TS_SUB_CMD_SET_FAULT_MASK,
// 获取硬件屏蔽AICORE ERR的掩码
DCMI_TS_SUB_CMD_GET_FAULT_MASK,
// 设置算子超时时间刻度值，取值范围为[20，32]。
DCMI_TS_SUB_CMD_COMMON_MSG = 11,
DCMI_TS_SUB_CMD_MAX,
} DCMI_TS_SUB_CMD;</pre>
<p id="zh-cn_topic_0000002481387068_p7900115703014"><a name="zh-cn_topic_0000002481387068_p7900115703014"></a><a name="zh-cn_topic_0000002481387068_p7900115703014"></a>仅支持DCMI_TS_SUB_CMD_COMMON_MSG。</p>
</td>
</tr>
<tr id="row1190010575303"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p1190018574307"><a name="p1190018574307"></a><a name="p1190018574307"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p19007577307"><a name="p19007577307"></a><a name="p19007577307"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p17900175711302"><a name="p17900175711302"></a><a name="p17900175711302"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p390012575308"><a name="p390012575308"></a><a name="p390012575308"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row16900145719304"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p9900155716309"><a name="p9900155716309"></a><a name="p9900155716309"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p0900185719304"><a name="p0900185719304"></a><a name="p0900185719304"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p6901257173010"><a name="p6901257173010"></a><a name="p6901257173010"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p090113577301"><a name="p090113577301"></a><a name="p090113577301"></a>buf数组的长度/返回结果数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section5782161012308"></a>**

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

**异常处理<a name="section6438163417415"></a>**

无。

**约束说明<a name="section5948183954914"></a>**

**表 1**  sub\_cmd对应的buf格式

<a name="table08529574303"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002481387068_row19012578309"><th class="cellrowborder" valign="top" width="39.269999999999996%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002481387068_p59011457173015"><a name="zh-cn_topic_0000002481387068_p59011457173015"></a><a name="zh-cn_topic_0000002481387068_p59011457173015"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="36.96%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002481387068_p159011357193019"><a name="zh-cn_topic_0000002481387068_p159011357193019"></a><a name="zh-cn_topic_0000002481387068_p159011357193019"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="23.77%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002481387068_p1990165743011"><a name="zh-cn_topic_0000002481387068_p1990165743011"></a><a name="zh-cn_topic_0000002481387068_p1990165743011"></a>size</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002481387068_row16617143112276"><td class="cellrowborder" valign="top" width="39.269999999999996%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002481387068_p1047117132015"><a name="zh-cn_topic_0000002481387068_p1047117132015"></a><a name="zh-cn_topic_0000002481387068_p1047117132015"></a>DCMI_TS_SUB_CMD_COMMON_MSG</p>
</td>
<td class="cellrowborder" valign="top" width="36.96%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002481387068_p19469111244813"><a name="zh-cn_topic_0000002481387068_p19469111244813"></a><a name="zh-cn_topic_0000002481387068_p19469111244813"></a>struct ts_dcmi_ctrl_msg_body_t {</p>
<p id="zh-cn_topic_0000002481387068_p1246961219488"><a name="zh-cn_topic_0000002481387068_p1246961219488"></a><a name="zh-cn_topic_0000002481387068_p1246961219488"></a>unsigned int msg_type;</p>
<p id="zh-cn_topic_0000002481387068_p246915123487"><a name="zh-cn_topic_0000002481387068_p246915123487"></a><a name="zh-cn_topic_0000002481387068_p246915123487"></a>union {</p>
<p id="zh-cn_topic_0000002481387068_p1746901219481"><a name="zh-cn_topic_0000002481387068_p1746901219481"></a><a name="zh-cn_topic_0000002481387068_p1746901219481"></a>ts_dcmi_task_timeout_t set_task_timeout_info;</p>
<p id="zh-cn_topic_0000002481387068_p64691512154816"><a name="zh-cn_topic_0000002481387068_p64691512154816"></a><a name="zh-cn_topic_0000002481387068_p64691512154816"></a>ts_dcmi_task_timeout_t get_task_timeout_info;</p>
<p id="zh-cn_topic_0000002481387068_p13469131244812"><a name="zh-cn_topic_0000002481387068_p13469131244812"></a><a name="zh-cn_topic_0000002481387068_p13469131244812"></a>} u;</p>
<p id="zh-cn_topic_0000002481387068_p134691512204817"><a name="zh-cn_topic_0000002481387068_p134691512204817"></a><a name="zh-cn_topic_0000002481387068_p134691512204817"></a>};</p>
<p id="zh-cn_topic_0000002481387068_p1719316326501"><a name="zh-cn_topic_0000002481387068_p1719316326501"></a><a name="zh-cn_topic_0000002481387068_p1719316326501"></a>ts_dcmi_task_timeout_t结构体如下：</p>
<p id="zh-cn_topic_0000002481387068_p522516213502"><a name="zh-cn_topic_0000002481387068_p522516213502"></a><a name="zh-cn_topic_0000002481387068_p522516213502"></a>typedef struct {</p>
<p id="zh-cn_topic_0000002481387068_p32251323501"><a name="zh-cn_topic_0000002481387068_p32251323501"></a><a name="zh-cn_topic_0000002481387068_p32251323501"></a>unsigned int timeout_limit_exp; // 取值范围为[20，32]</p>
<p id="zh-cn_topic_0000002481387068_p222512155013"><a name="zh-cn_topic_0000002481387068_p222512155013"></a><a name="zh-cn_topic_0000002481387068_p222512155013"></a>unsigned char rsev[TS_RSEV_MAX_LENTH]; //TS_RSEV_MAX_LENTH为36</p>
<p id="zh-cn_topic_0000002481387068_p10225725506"><a name="zh-cn_topic_0000002481387068_p10225725506"></a><a name="zh-cn_topic_0000002481387068_p10225725506"></a>} ts_dcmi_task_timeout_t;</p>
</td>
<td class="cellrowborder" valign="top" width="23.77%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002481387068_p19359252485"><a name="zh-cn_topic_0000002481387068_p19359252485"></a><a name="zh-cn_topic_0000002481387068_p19359252485"></a>作为入参时表示buf的大小，buf至少为44Byte。</p>
<p id="zh-cn_topic_0000002481387068_p19218195310"><a name="zh-cn_topic_0000002481387068_p19218195310"></a><a name="zh-cn_topic_0000002481387068_p19218195310"></a>msg_type取值为0或1。</p>
<a name="zh-cn_topic_0000002481387068_ul16848433145315"></a><a name="zh-cn_topic_0000002481387068_ul16848433145315"></a><ul id="zh-cn_topic_0000002481387068_ul16848433145315"><li>1表示设置AI CPU算子超时时间刻度。</li><li>0表示查询AI CPU算子超时时间刻度。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

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

**调用示例<a name="section49611431655"></a>**

```
…
int ret;
int card_id = 0;
int dev_id = 0;
void *buf = NULL;
int buf_size = 10;
buf = calloc(buf_size, sizeof(char));
if (buf == NULL) {
printf("calloc buf failed.\n");
return -1;
}
ret = dcmi_set_device_info(card_id,dev_id,DCMI_MAIN_CMD_TS, DCMI_TS_SUB_CMD_AICORE_UTILIZATION_RATE, buf, &buf_size);
if (ret != 0) {
printf("dcmi_set_device_info failed, ret = %d.\n", ret);
return -1;
}
return 0;
…
```


## DCMI\_MAIN\_CMD\_DEVICE\_SHARE命令说明<a name="ZH-CN_TOPIC_0000002517615381"></a>

**函数原型<a name="section782265716308"></a>**

**int dcmi\_set\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, const void \*buf, unsigned int buf\_size\)**

**功能说明<a name="section156751813192414"></a>**

配置指定芯片的容器共享使能标记为使能或禁用。

**参数说明<a name="section5671124612912"></a>**

<a name="zh-cn_topic_0257495783_table45028263"></a>
<table><thead align="left"><tr id="zh-cn_topic_0257495783_row60695621"><th class="cellrowborder" valign="top" width="10.100000000000001%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0257495783_p17398306"><a name="zh-cn_topic_0257495783_p17398306"></a><a name="zh-cn_topic_0257495783_p17398306"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="8.41%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0257495783_p67085535"><a name="zh-cn_topic_0257495783_p67085535"></a><a name="zh-cn_topic_0257495783_p67085535"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.470000000000002%" id="mcps1.1.5.1.3"><p id="p153645385214"><a name="p153645385214"></a><a name="p153645385214"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="62.019999999999996%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0257495783_p48266778"><a name="zh-cn_topic_0257495783_p48266778"></a><a name="zh-cn_topic_0257495783_p48266778"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row884163595218"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p121951513267"><a name="p121951513267"></a><a name="p121951513267"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="8.41%" headers="mcps1.1.5.1.2 "><p id="p11956515261"><a name="p11956515261"></a><a name="p11956515261"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.470000000000002%" headers="mcps1.1.5.1.3 "><p id="p1919510562619"><a name="p1919510562619"></a><a name="p1919510562619"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="62.019999999999996%" headers="mcps1.1.5.1.4 "><p id="p1619510511265"><a name="p1619510511265"></a><a name="p1619510511265"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0257495783_row31747823"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p191957519266"><a name="p191957519266"></a><a name="p191957519266"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="8.41%" headers="mcps1.1.5.1.2 "><p id="p1319517522613"><a name="p1319517522613"></a><a name="p1319517522613"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.470000000000002%" headers="mcps1.1.5.1.3 "><p id="p419514512261"><a name="p419514512261"></a><a name="p419514512261"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="62.019999999999996%" headers="mcps1.1.5.1.4 "><p id="p161951552616"><a name="p161951552616"></a><a name="p161951552616"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p8195155132618"><a name="p8195155132618"></a><a name="p8195155132618"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p0968356111712"><a name="p0968356111712"></a><a name="p0968356111712"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0257495783_row7743426585"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0257495783_p15756282814"><a name="zh-cn_topic_0257495783_p15756282814"></a><a name="zh-cn_topic_0257495783_p15756282814"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="8.41%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0257495783_p1574417263816"><a name="zh-cn_topic_0257495783_p1574417263816"></a><a name="zh-cn_topic_0257495783_p1574417263816"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.470000000000002%" headers="mcps1.1.5.1.3 "><p id="p1519518513263"><a name="p1519518513263"></a><a name="p1519518513263"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="62.019999999999996%" headers="mcps1.1.5.1.4 "><p id="p466445534710"><a name="p466445534710"></a><a name="p466445534710"></a>DCMI_MAIN_CMD_DEVICE_SHARE</p>
<p id="p1549135919171"><a name="p1549135919171"></a><a name="p1549135919171"></a></p>
<div class="note" id="note1218455521019"><a name="note1218455521019"></a><a name="note1218455521019"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p14728105710819"><a name="p14728105710819"></a><a name="p14728105710819"></a>配置容器共享功能也可参见<a href="dcmi_set_device_share_enable.md">dcmi_set_device_share_enable</a>。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0257495783_row42644663"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0257495783_p851813475811"><a name="zh-cn_topic_0257495783_p851813475811"></a><a name="zh-cn_topic_0257495783_p851813475811"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="8.41%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0257495783_p133891038900"><a name="zh-cn_topic_0257495783_p133891038900"></a><a name="zh-cn_topic_0257495783_p133891038900"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.470000000000002%" headers="mcps1.1.5.1.3 "><p id="p17196155202616"><a name="p17196155202616"></a><a name="p17196155202616"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="62.019999999999996%" headers="mcps1.1.5.1.4 "><p id="p1528419185118"><a name="p1528419185118"></a><a name="p1528419185118"></a>typedef enum {</p>
<p id="p1228415916510"><a name="p1228415916510"></a><a name="p1228415916510"></a>DCMI_DEVICE_SHARE_SUB_CMD_COMMON = 0,</p>
<p id="p1028409165114"><a name="p1028409165114"></a><a name="p1028409165114"></a>DCMI_DEVICE_SHARE_SUB_CMD_MAX,</p>
<p id="p328411917514"><a name="p328411917514"></a><a name="p328411917514"></a>} DCMI_DEVICE_SHARE_SUB_CMD;</p>
<p id="p1610911191813"><a name="p1610911191813"></a><a name="p1610911191813"></a></p>
<div class="note" id="note690717511525"><a name="note690717511525"></a><a name="note690717511525"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p12648821111120"><a name="p12648821111120"></a><a name="p12648821111120"></a>当前接口仅支持DCMI_DEVICE_SHARE_SUB_CMD_COMMON命令。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0257495783_row17526454687"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p123108571525"><a name="p123108571525"></a><a name="p123108571525"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="8.41%" headers="mcps1.1.5.1.2 "><p id="p13102579214"><a name="p13102579214"></a><a name="p13102579214"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.470000000000002%" headers="mcps1.1.5.1.3 "><p id="p331005717211"><a name="p331005717211"></a><a name="p331005717211"></a>const void *</p>
</td>
<td class="cellrowborder" valign="top" width="62.019999999999996%" headers="mcps1.1.5.1.4 "><p id="p12310115715214"><a name="p12310115715214"></a><a name="p12310115715214"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="zh-cn_topic_0257495783_row19825197914"><td class="cellrowborder" valign="top" width="10.100000000000001%" headers="mcps1.1.5.1.1 "><p id="p1231019570213"><a name="p1231019570213"></a><a name="p1231019570213"></a>buf_size</p>
</td>
<td class="cellrowborder" valign="top" width="8.41%" headers="mcps1.1.5.1.2 "><p id="p12310115715213"><a name="p12310115715213"></a><a name="p12310115715213"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.470000000000002%" headers="mcps1.1.5.1.3 "><p id="p1531045710217"><a name="p1531045710217"></a><a name="p1531045710217"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="62.019999999999996%" headers="mcps1.1.5.1.4 "><p id="p13107576214"><a name="p13107576214"></a><a name="p13107576214"></a>buf数组的长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section283355715302"></a>**

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

**异常处理<a name="section6438163417415"></a>**

无。

**约束说明<a name="section13645113118442"></a>**

-   设置容器共享模式后，重启系统后容器使能状态默认为禁用。
-   若物理机的NPU设备映射进普通容器且在该容器内对NPU执行过任意操作，则不支持修改容器共享模式。
-   device_id设置为0或1时，会同时设置设备的两个芯片的容器共享使能标记。
-   该接口支持在物理机+特权容器场景下使用。

**表 1**  sub\_cmd对应的buf格式

<a name="table08529574303"></a>
<table><thead align="left"><tr id="row19012578309"><th class="cellrowborder" valign="top" width="39.269999999999996%" id="mcps1.2.4.1.1"><p id="p59011457173015"><a name="p59011457173015"></a><a name="p59011457173015"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="36.96%" id="mcps1.2.4.1.2"><p id="p159011357193019"><a name="p159011357193019"></a><a name="p159011357193019"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="23.77%" id="mcps1.2.4.1.3"><p id="p1990165743011"><a name="p1990165743011"></a><a name="p1990165743011"></a>size</p>
</th>
</tr>
</thead>
<tbody><tr id="row15743174685218"><td class="cellrowborder" valign="top" width="39.269999999999996%" headers="mcps1.2.4.1.1 "><p id="p17744144675215"><a name="p17744144675215"></a><a name="p17744144675215"></a>DCMI_DEVICE_SHARE_SUB_CMD_COMMON</p>
</td>
<td class="cellrowborder" valign="top" width="36.96%" headers="mcps1.2.4.1.2 "><p id="p6898117165315"><a name="p6898117165315"></a><a name="p6898117165315"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="23.77%" headers="mcps1.2.4.1.3 "><p id="p789841705311"><a name="p789841705311"></a><a name="p789841705311"></a>unsigned int</p>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

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

**调用示例<a name="section593717142277"></a>**

```
    …
    int ret;
    int card_id = 0;
    int device_id = 0;
    enum dcmi_main_cmd  main_cmd = DCMI_MAIN_CMD_DEVICE_SHARE;
    unsigned int sub_cmd = DCMI_DEVICE_SHARE_SUB_CMD_COMMON;
    unsigned int enable_flag = 1;
    unsigned int size = sizeof(unsigned int);

    ret = dcmi_set_device_info(card_id, device_id, main_cmd, sub_cmd, (void *)&enable_flag, size);
    if (ret != DCMI_OK){
        //todo：记录日志
        return ret;
    }
    …
```


