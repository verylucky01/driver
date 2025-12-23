# dcmi\_get\_device\_info<a name="ZH-CN_TOPIC_0000002485318782"></a>

**函数原型<a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_toc533412077"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_toc533412078"></a>**

获取device的信息的通用接口，获取各模块中的状态信息。

**参数说明<a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_toc533412079"></a>**

<a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p10021890"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p10021890"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p6466753"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p6466753"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.96%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p54045009"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p54045009"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.019999999999996%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p15569626"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p15569626"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p36741947142813"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p36741947142813"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p96741747122818"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p96741747122818"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p46747472287"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p46747472287"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1467413474281"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1467413474281"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p7711145152918"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p7711145152918"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p671116522914"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p671116522914"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1771116572910"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1771116572910"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1291918916817"><a name="p1291918916817"></a><a name="p1291918916817"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p5522164215178"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p5522164215178"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p5522164215178"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p8522242101715"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p8522242101715"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p8522242101715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p17522114220174"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p17522114220174"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p17522114220174"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p167001165556"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p167001165556"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p167001165556"></a>指定查询项对应主命令字。</p>
<p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p37001865558"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p37001865558"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p37001865558"></a>模块cmd信息，执行用于获取对应模块的信息</p>
<p id="p15764525481"><a name="p15764525481"></a><a name="p15764525481"></a>enum dcmi_main_cmd {</p>
<p id="p61731150986"><a name="p61731150986"></a><a name="p61731150986"></a>DCMI_MAIN_CMD_DVPP = 0,</p>
<p id="p14418615117"><a name="p14418615117"></a><a name="p14418615117"></a>DCMI_MAIN_CMD_ISP,</p>
<p id="p1312911171213"><a name="p1312911171213"></a><a name="p1312911171213"></a>DCMI_MAIN_CMD_TS_GROUP_NUM,</p>
<p id="p1591117521484"><a name="p1591117521484"></a><a name="p1591117521484"></a>DCMI_MAIN_CMD_CAN,</p>
<p id="p76221333125"><a name="p76221333125"></a><a name="p76221333125"></a>DCMI_MAIN_CMD_UART,</p>
<p id="p1492915181210"><a name="p1492915181210"></a><a name="p1492915181210"></a>DCMI_MAIN_CMD_UPGRADE = 5,</p>
<p id="p157832073125"><a name="p157832073125"></a><a name="p157832073125"></a>DCMI_MAIN_CMD_UFS,</p>
<p id="p132481911217"><a name="p132481911217"></a><a name="p132481911217"></a>DCMI_MAIN_CMD_OS_POWER,</p>
<p id="p11495571288"><a name="p11495571288"></a><a name="p11495571288"></a>DCMI_MAIN_CMD_LP,</p>
<p id="p1610001115121"><a name="p1610001115121"></a><a name="p1610001115121"></a>DCMI_MAIN_CMD_MEMORY,</p>
<p id="p186982123123"><a name="p186982123123"></a><a name="p186982123123"></a>DCMI_MAIN_CMD_RECOVERY,</p>
<p id="p3457195910816"><a name="p3457195910816"></a><a name="p3457195910816"></a>DCMI_MAIN_CMD_TS,</p>
<p id="p9823111501212"><a name="p9823111501212"></a><a name="p9823111501212"></a>DCMI_MAIN_CMD_CHIP_INF,</p>
<p id="p667381711214"><a name="p667381711214"></a><a name="p667381711214"></a>DCMI_MAIN_CMD_QOS,</p>
<p id="p1219517209126"><a name="p1219517209126"></a><a name="p1219517209126"></a>DCMI_MAIN_CMD_SOC_INFO,</p>
<p id="p532617119912"><a name="p532617119912"></a><a name="p532617119912"></a>DCMI_MAIN_CMD_SILS,</p>
<p id="p41674228121"><a name="p41674228121"></a><a name="p41674228121"></a>DCMI_MAIN_CMD_HCCS,</p>
<p id="p0713162516122"><a name="p0713162516122"></a><a name="p0713162516122"></a>DCMI_MAIN_CMD_HOST_AICPU,</p>
<p id="p8688112731212"><a name="p8688112731212"></a><a name="p8688112731212"></a>DCMI_MAIN_CMD_TEMP = 50,</p>
<p id="p05801129181214"><a name="p05801129181214"></a><a name="p05801129181214"></a>DCMI_MAIN_CMD_SVM,</p>
<p id="p1453863211214"><a name="p1453863211214"></a><a name="p1453863211214"></a>DCMI_MAIN_CMD_VDEV_MNG,</p>
<p id="p7623193451211"><a name="p7623193451211"></a><a name="p7623193451211"></a>DCMI_MAIN_CMD_SEC,</p>
<p id="p194422345516"><a name="p194422345516"></a><a name="p194422345516"></a>DCMI_MAIN_CMD_PCIE = 55,</p>
<p id="p1896837121219"><a name="p1896837121219"></a><a name="p1896837121219"></a>DCMI_MAIN_CMD_SIO = 56,</p>
<p id="p18590722192310"><a name="p18590722192310"></a><a name="p18590722192310"></a>DCMI_MAIN_CMD_EX_COMPUTING = 0x8000,    DCMI_MAIN_CMD_DEVICE_SHARE = 0x8001,</p>
<p id="p1032616302563"><a name="p1032616302563"></a><a name="p1032616302563"></a>DCMI_MAIN_CMD_EX_CERT = 0x8003,</p>
<p id="p692433982"><a name="p692433982"></a><a name="p692433982"></a>DCMI_MAIN_CMD_MAX</p>
<p id="p223161612813"><a name="p223161612813"></a><a name="p223161612813"></a>};</p>
<p id="p9932195894319"><a name="p9932195894319"></a><a name="p9932195894319"></a>仅支持如下模块主命令字：</p>
<p id="p1028842114915"><a name="p1028842114915"></a><a name="p1028842114915"></a>DCMI_MAIN_CMD_DVPP //dvpp算子模块主命令字</p>
<p id="p889911226920"><a name="p889911226920"></a><a name="p889911226920"></a>DCMI_MAIN_CMD_LP //lp低功耗模块主命令字</p>
<p id="p630816245918"><a name="p630816245918"></a><a name="p630816245918"></a>DCMI_MAIN_CMD_TS //ts任务调度模块主命令字</p>
<p id="p77158266913"><a name="p77158266913"></a><a name="p77158266913"></a>DCMI_MAIN_CMD_QOS //QoS模块主命令字</p>
<p id="p1072315596121"><a name="p1072315596121"></a><a name="p1072315596121"></a>DCMI_MAIN_CMD_HCCS //hccs模块主命令字</p>
<p id="p1334062061315"><a name="p1334062061315"></a><a name="p1334062061315"></a>DCMI_MAIN_CMD_EX_COMPUTING //算力扩展模块主命令字</p>
<p id="p121340251314"><a name="p121340251314"></a><a name="p121340251314"></a>DCMI_MAIN_CMD_VDEV_MNG //<span id="ph11173195663615"><a name="ph11173195663615"></a><a name="ph11173195663615"></a>昇腾虚拟化实例（AVI）</span>模块主命令字</p>
<p id="p193245812437"><a name="p193245812437"></a><a name="p193245812437"></a>DCMI_MAIN_CMD_CHIP_INF //查询超节点信息</p>
<p id="p15875609615"><a name="p15875609615"></a><a name="p15875609615"></a>DCMI_MAIN_CMD_SIO //查询die间SIO状态</p>
<p id="p123541463434"><a name="p123541463434"></a><a name="p123541463434"></a>DCMI_MAIN_CMD_PCIE //获取PCIe相关信息</p>
<p id="p12452171290"><a name="p12452171290"></a><a name="p12452171290"></a>DCMI_MAIN_CMD_SOC_INFO //获取SOC相关信息</p>
<p id="p2029414595275"><a name="p2029414595275"></a><a name="p2029414595275"></a>DCMI_MAIN_CMD_DEVICE_SHARE //容器共享主命令字</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_row118352016305"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p19161539205"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p19161539205"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p19161539205"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1816115315209"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1816115315209"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1816115315209"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1216853142018"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1216853142018"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1216853142018"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p75475304476"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p75475304476"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p75475304476"></a>详细参见子章节中的功能说明。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_row148973243016"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p8864185062018"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p8864185062018"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p8864185062018"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p886417500202"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p886417500202"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p886417500202"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1386445092020"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1386445092020"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1386445092020"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p15864145012200"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p15864145012200"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p15864145012200"></a>用于输入指定获取信息，并接收设备信息的返回数据。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_row20221172313016"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p7749184816202"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p7749184816202"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p7749184816202"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p0749104811207"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p0749104811207"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p0749104811207"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.96%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1374944862018"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1374944862018"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p1374944862018"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="50.019999999999996%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p074915488209"><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p074915488209"></a><a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_p074915488209"></a>buf数组的输入/输出长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="zh-cn_topic_0000001206627196_zh-cn_topic_0000001178213188_zh-cn_topic_0000001146459833_section1256282115569"></a>**

<a name="zh-cn_topic_0000001819869974_zh-cn_topic_0000001251227149_zh-cn_topic_0000001178213202_zh-cn_topic_0000001097675636_zh-cn_topic_0000001170223803_table19654399"></a>
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

**约束说明<a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="zh-cn_topic_0000001206467202_zh-cn_topic_0000001178373156_zh-cn_topic_0000001101204718_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
int buf = 0;
unsigned int size = sizeof(int);
unsigned int sub_cmd = 0;
ret = dcmi_get_device_info(card_id, device_id, DCMI_MAIN_CMD_DVPP, sub_cmd, &buf, &size);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

## DCMI\_MAIN\_CMD\_DVPP命令说明<a name="ZH-CN_TOPIC_0000002517638697"></a>

**函数原型<a name="section1045115919305"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section184521595304"></a>**

获取dvpp相关状态，配置信息。

**参数说明<a name="section104533963015"></a>**

<a name="table104768953018"></a>
<table><thead align="left"><tr id="row19544594301"><th class="cellrowborder" valign="top" width="17.17171717171717%" id="mcps1.1.5.1.1"><p id="p115440913011"><a name="p115440913011"></a><a name="p115440913011"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.151515151515152%" id="mcps1.1.5.1.2"><p id="p754417916309"><a name="p754417916309"></a><a name="p754417916309"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="23.232323232323232%" id="mcps1.1.5.1.3"><p id="p135449903012"><a name="p135449903012"></a><a name="p135449903012"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="44.44444444444445%" id="mcps1.1.5.1.4"><p id="p254412913309"><a name="p254412913309"></a><a name="p254412913309"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row55442993013"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p12544109113015"><a name="p12544109113015"></a><a name="p12544109113015"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p185448903012"><a name="p185448903012"></a><a name="p185448903012"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="23.232323232323232%" headers="mcps1.1.5.1.3 "><p id="p195441497307"><a name="p195441497307"></a><a name="p195441497307"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="44.44444444444445%" headers="mcps1.1.5.1.4 "><p id="p105445933016"><a name="p105445933016"></a><a name="p105445933016"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1544592304"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p195445917304"><a name="p195445917304"></a><a name="p195445917304"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p185446910306"><a name="p185446910306"></a><a name="p185446910306"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="23.232323232323232%" headers="mcps1.1.5.1.3 "><p id="p175441294306"><a name="p175441294306"></a><a name="p175441294306"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="44.44444444444445%" headers="mcps1.1.5.1.4 "><p id="p1854417933011"><a name="p1854417933011"></a><a name="p1854417933011"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p1054449203011"><a name="p1054449203011"></a><a name="p1054449203011"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p205512141488"><a name="p205512141488"></a><a name="p205512141488"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row35441953014"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p1254413913016"><a name="p1254413913016"></a><a name="p1254413913016"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p154509173018"><a name="p154509173018"></a><a name="p154509173018"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="23.232323232323232%" headers="mcps1.1.5.1.3 "><p id="p135451198309"><a name="p135451198309"></a><a name="p135451198309"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="44.44444444444445%" headers="mcps1.1.5.1.4 "><p id="p165456953016"><a name="p165456953016"></a><a name="p165456953016"></a>DCMI_MAIN_CMD_DVPP</p>
</td>
</tr>
<tr id="row754518963011"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p85451396307"><a name="p85451396307"></a><a name="p85451396307"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p1654518910302"><a name="p1654518910302"></a><a name="p1654518910302"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="23.232323232323232%" headers="mcps1.1.5.1.3 "><p id="p354519916301"><a name="p354519916301"></a><a name="p354519916301"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="44.44444444444445%" headers="mcps1.1.5.1.4 "><p id="p135451197302"><a name="p135451197302"></a><a name="p135451197302"></a>sub_cmd获取对应模块下子属性信息。</p>
<p id="p1154589163019"><a name="p1154589163019"></a><a name="p1154589163019"></a>/* DCMI sub command for DVPP module */</p>
<p id="p145452913308"><a name="p145452913308"></a><a name="p145452913308"></a>#define DCMI_SUB_CMD_DVPP_STATUS 0  // dvpp状态，buf为0表示状态正常，非0表示状态异常</p>
<p id="p17545699303"><a name="p17545699303"></a><a name="p17545699303"></a>#define DCMI_SUB_CMD_DVPP_VDEC_RATE 1  // vdec利用率，正常值范围0-100</p>
<p id="p0545119193019"><a name="p0545119193019"></a><a name="p0545119193019"></a>#define DCMI_SUB_CMD_DVPP_VPC_RATE 2 // vpc利用率，正常值范围0-100</p>
<p id="p13545397302"><a name="p13545397302"></a><a name="p13545397302"></a>#define DCMI_SUB_CMD_DVPP_VENC_RATE 3 // venc利用率，正常值范围0-100</p>
<p id="p154509103011"><a name="p154509103011"></a><a name="p154509103011"></a>#define DCMI_SUB_CMD_DVPP_JPEGE_RATE 4 // jpege利用率，正常值范围0-100</p>
<p id="p1054579163010"><a name="p1054579163010"></a><a name="p1054579163010"></a>#define DCMI_SUB_CMD_DVPP_JPEGD_RATE 5 // jpegd利用率，正常值范围0-100</p>
<p id="p1754517943015"><a name="p1754517943015"></a><a name="p1754517943015"></a>目前不支持DCMI_SUB_CMD_DVPP_VENC_RATE命令的查询。</p>
</td>
</tr>
<tr id="row1954515903016"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p155455915303"><a name="p155455915303"></a><a name="p155455915303"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p554510903013"><a name="p554510903013"></a><a name="p554510903013"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="23.232323232323232%" headers="mcps1.1.5.1.3 "><p id="p19545129143015"><a name="p19545129143015"></a><a name="p19545129143015"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="44.44444444444445%" headers="mcps1.1.5.1.4 "><p id="p185454953015"><a name="p185454953015"></a><a name="p185454953015"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row2054549113013"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p25451983018"><a name="p25451983018"></a><a name="p25451983018"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p12545169113011"><a name="p12545169113011"></a><a name="p12545169113011"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="23.232323232323232%" headers="mcps1.1.5.1.3 "><p id="p1754515943013"><a name="p1754515943013"></a><a name="p1754515943013"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="44.44444444444445%" headers="mcps1.1.5.1.4 "><p id="p2054559153019"><a name="p2054559153019"></a><a name="p2054559153019"></a>buf数组的长度/返回结果实际数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section104687916306"></a>**

<a name="table34861192309"></a>
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

**异常处理<a name="section8924415552"></a>**

无。

**约束说明<a name="section20247271550"></a>**

获取dvpp设备时sub\_cmd，buf和size之间必须要满足以下关系，如果不满足会导致接口调用失败。

**表 1** **s**ub\_cmd对应的buf格式

<a name="table34881973017"></a>
<table><thead align="left"><tr id="row654609173016"><th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.2.4.1.1"><p id="p654609173010"><a name="p654609173010"></a><a name="p654609173010"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.2.4.1.2"><p id="p1254617983019"><a name="p1254617983019"></a><a name="p1254617983019"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.2.4.1.3"><p id="p2054620963017"><a name="p2054620963017"></a><a name="p2054620963017"></a>size</p>
</th>
</tr>
</thead>
<tbody><tr id="row1954613919308"><td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.1 "><p id="p45462912305"><a name="p45462912305"></a><a name="p45462912305"></a>DCMI_SUB_CMD_DVPP_STATUS</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.2 "><p id="p1554659163015"><a name="p1554659163015"></a><a name="p1554659163015"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.3 "><p id="p354616913304"><a name="p354616913304"></a><a name="p354616913304"></a>长度为：sizeof(unsigned int)</p>
</td>
</tr>
<tr id="row2054611914303"><td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.1 "><p id="p1754613917303"><a name="p1754613917303"></a><a name="p1754613917303"></a>DCMI_SUB_CMD_DVPP_VDEC_RATE</p>
<p id="p954615953017"><a name="p954615953017"></a><a name="p954615953017"></a>DCMI_SUB_CMD_DVPP_VPC_RATE</p>
<p id="p754649173014"><a name="p754649173014"></a><a name="p754649173014"></a>DCMI_SUB_CMD_DVPP_VENC_RATE</p>
<p id="p15462094304"><a name="p15462094304"></a><a name="p15462094304"></a>DCMI_SUB_CMD_DVPP_JPEGE_RATE</p>
<p id="p85468914304"><a name="p85468914304"></a><a name="p85468914304"></a>DCMI_SUB_CMD_DVPP_JPEGD_RATE</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.2 "><p id="p135461398309"><a name="p135461398309"></a><a name="p135461398309"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.3 "><p id="p1254699103018"><a name="p1254699103018"></a><a name="p1254699103018"></a>长度为：sizeof(unsigned int)</p>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section2133155511568"></a>**

```
… 
int ret = 0;
int card_id = 0;
int dev_id = 0;
int ratio = 0;
int sub_cmd = 0;
unsigned int ratio_size = sizeof(int);
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_DVPP, sub_cmd, (void *)&ratio, &ratio_size);
if (ret != 0) {
    // todo
    return ret;
} else {
    // todo
    return ret;
}
…
```


## DCMI\_MAIN\_CMD\_LP命令说明<a name="ZH-CN_TOPIC_0000002485318736"></a>

**函数原型<a name="section158495435301"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section7850154317304"></a>**

获取系统中AICORE、HYBIRD、CPU和DDR的电压和电流的寄存器值等LP相关信息。

**参数说明<a name="section28501043183017"></a>**

<a name="table15882143133020"></a>
<table><thead align="left"><tr id="row11974164343015"><th class="cellrowborder" valign="top" width="17.17171717171717%" id="mcps1.1.5.1.1"><p id="p797519436303"><a name="p797519436303"></a><a name="p797519436303"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.151515151515152%" id="mcps1.1.5.1.2"><p id="p697512439305"><a name="p697512439305"></a><a name="p697512439305"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.19191919191919%" id="mcps1.1.5.1.3"><p id="p19975543163019"><a name="p19975543163019"></a><a name="p19975543163019"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.484848484848484%" id="mcps1.1.5.1.4"><p id="p15975184353016"><a name="p15975184353016"></a><a name="p15975184353016"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row159751431302"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p497524313301"><a name="p497524313301"></a><a name="p497524313301"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p1397564318309"><a name="p1397564318309"></a><a name="p1397564318309"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p1697510439301"><a name="p1697510439301"></a><a name="p1697510439301"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p15975114317309"><a name="p15975114317309"></a><a name="p15975114317309"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row109751843163011"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p39751943113018"><a name="p39751943113018"></a><a name="p39751943113018"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p2975124363011"><a name="p2975124363011"></a><a name="p2975124363011"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p997554315307"><a name="p997554315307"></a><a name="p997554315307"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p49753435303"><a name="p49753435303"></a><a name="p49753435303"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p69753439303"><a name="p69753439303"></a><a name="p69753439303"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p139755172084"><a name="p139755172084"></a><a name="p139755172084"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row12975443183013"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p1097534333012"><a name="p1097534333012"></a><a name="p1097534333012"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p19975124303012"><a name="p19975124303012"></a><a name="p19975124303012"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p2975184311304"><a name="p2975184311304"></a><a name="p2975184311304"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p9975194310307"><a name="p9975194310307"></a><a name="p9975194310307"></a>DCMI_MAIN_CMD_LP</p>
</td>
</tr>
<tr id="row2975194333013"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p1497534343010"><a name="p1497534343010"></a><a name="p1497534343010"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p597594314306"><a name="p597594314306"></a><a name="p597594314306"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p6975743153011"><a name="p6975743153011"></a><a name="p6975743153011"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p18975343113017"><a name="p18975343113017"></a><a name="p18975343113017"></a>/* DCMI sub commond for Low power */</p>
<pre class="codeblock" id="codeblock971395520334"><a name="codeblock971395520334"></a><a name="codeblock971395520334"></a>typedef enum {
// 获取AICORE电压电流的寄存器值
DCMI_LP_SUB_CMD_AICORE_VOLTAGE_CURRENT = 0,
// 获取HYBIRD电压电流的寄存器值
DCMI_LP_SUB_CMD_HYBIRD_VOLTAGE_CURRENT,
// 获取CPU电压电流的寄存器值
DCMI_LP_SUB_CMD_TAISHAN_VOLTAGE_CURRENT,
// 获取DDR电压电流的寄存器值
DCMI_LP_SUB_CMD_DDR_VOLTAGE_CURRENT,
// 获取ACG调频计数值
DCMI_LP_SUB_CMD_ACG,
// 获取低功耗总状态
DCMI_LP_SUB_CMD_STATUS,
// 获取所有工作档位
DCMI_LP_SUB_CMD_TOPS_DETAILS,
// 设置工作档位
DCMI_LP_SUB_CMD_SET_WORK_TOPS,
// 获取当前工作档位
DCMI_LP_SUB_CMD_GET_WORK_TOPS,
// 获取当前降频原因
DCMI_LP_SUB_CMD_AICORE_FREQREDUC_CAUSE,
// 获取功耗信息
DCMI_LP_SUB_CMD_GET_POWER_INFO,
// 设置IDLE模式开关
DCMI_LP_SUB_CMD_SET_IDLE_SWITCH,
DCMI_LP_SUB_CMD_MAX,
} DCMI_LP_SUB_CMD;</pre>
<p id="p0976643203017"><a name="p0976643203017"></a><a name="p0976643203017"></a>支持DCMI_LP_SUB_CMD_AICORE_FREQREDUC_CAUSE、DCMI_LP_SUB_CMD_GET_POWER_INFO、DCMI_LP_SUB_CMD_STATUS命令。</p>
</td>
</tr>
<tr id="row119761843173017"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p0976184343012"><a name="p0976184343012"></a><a name="p0976184343012"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p397614363010"><a name="p397614363010"></a><a name="p397614363010"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p29762043173012"><a name="p29762043173012"></a><a name="p29762043173012"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p179761943143013"><a name="p179761943143013"></a><a name="p179761943143013"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row6976543103019"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p119765433306"><a name="p119765433306"></a><a name="p119765433306"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p79761543173017"><a name="p79761543173017"></a><a name="p79761543173017"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p1976204317307"><a name="p1976204317307"></a><a name="p1976204317307"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p2976943123017"><a name="p2976943123017"></a><a name="p2976943123017"></a>buf数组的长度/返回结果实际数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section386564317309"></a>**

<a name="table1689294312308"></a>
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

**异常处理<a name="section581311341081"></a>**

无。

**约束说明<a name="section216210381817"></a>**

-   通过本接口获取电压和电流信息为寄存器数值。
-   AI Core降频原因每100ms更新一次，不能查询历史降频原因。

**表 1**  sub\_cmd对应的buf格式

<a name="table9893154314306"></a>
<table><thead align="left"><tr id="row1097794373014"><th class="cellrowborder" valign="top" width="32.629999999999995%" id="mcps1.2.5.1.1"><p id="p597724343018"><a name="p597724343018"></a><a name="p597724343018"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="23.200000000000003%" id="mcps1.2.5.1.2"><p id="p15977743113017"><a name="p15977743113017"></a><a name="p15977743113017"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="18.6%" id="mcps1.2.5.1.3"><p id="p1977104363011"><a name="p1977104363011"></a><a name="p1977104363011"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="25.569999999999997%" id="mcps1.2.5.1.4"><p id="p1097718435302"><a name="p1097718435302"></a><a name="p1097718435302"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row497744313309"><td class="cellrowborder" valign="top" width="32.629999999999995%" headers="mcps1.2.5.1.1 "><p id="p1897794353018"><a name="p1897794353018"></a><a name="p1897794353018"></a>DCMI_LP_SUB_CMD_AICORE_FREQREDUC_CAUSE</p>
</td>
<td class="cellrowborder" valign="top" width="23.200000000000003%" headers="mcps1.2.5.1.2 "><p id="p159771243123013"><a name="p159771243123013"></a><a name="p159771243123013"></a>Unsigned long long</p>
</td>
<td class="cellrowborder" valign="top" width="18.6%" headers="mcps1.2.5.1.3 "><p id="p15977204323010"><a name="p15977204323010"></a><a name="p15977204323010"></a>sizeof(unsigned long long)</p>
</td>
<td class="cellrowborder" valign="top" width="25.569999999999997%" headers="mcps1.2.5.1.4 "><p id="p197784315304"><a name="p197784315304"></a><a name="p197784315304"></a>buf为8Byte内存空间，每个bit对应一种降频原因</p>
</td>
</tr>
<tr id="row697713436302"><td class="cellrowborder" valign="top" width="32.629999999999995%" headers="mcps1.2.5.1.1 "><p id="p1797774310308"><a name="p1797774310308"></a><a name="p1797774310308"></a>DCMI_LP_SUB_CMD_GET_POWER_INFO</p>
</td>
<td class="cellrowborder" valign="top" width="23.200000000000003%" headers="mcps1.2.5.1.2 "><p id="p1977043163011"><a name="p1977043163011"></a><a name="p1977043163011"></a>DCMI_LP_POWER_INFO_STRU</p>
</td>
<td class="cellrowborder" valign="top" width="18.6%" headers="mcps1.2.5.1.3 "><p id="p6977144314302"><a name="p6977144314302"></a><a name="p6977144314302"></a>sizeof(DCMI_LP_POWER_INFO_STRU)</p>
</td>
<td class="cellrowborder" valign="top" width="25.569999999999997%" headers="mcps1.2.5.1.4 "><p id="p397764343016"><a name="p397764343016"></a><a name="p397764343016"></a>#define DCMI_LP_POWER_RESERVED_LEN 32</p>
<p id="p12977124373012"><a name="p12977124373012"></a><a name="p12977124373012"></a>typedef struct DCMI_lp_power_info {</p>
<p id="p1697714323019"><a name="p1697714323019"></a><a name="p1697714323019"></a>unsigned int soc_rated_power;</p>
<p id="p797713439301"><a name="p797713439301"></a><a name="p797713439301"></a>unsigned char reserved[DCMI_LP_POWER_RESERVED_LEN];</p>
<p id="p197774318301"><a name="p197774318301"></a><a name="p197774318301"></a>} DCMI_LP_POWER_INFO_STRU;</p>
<p id="p1197794310304"><a name="p1197794310304"></a><a name="p1197794310304"></a>其中，soc_rated_power表示soc额定功率，其余为预留扩展空间。</p>
<p id="p7977184393012"><a name="p7977184393012"></a><a name="p7977184393012"></a>soc额定功率正确范围[150000,600000]</p>
</td>
</tr>
<tr id="row4977124333016"><td class="cellrowborder" valign="top" width="32.629999999999995%" headers="mcps1.2.5.1.1 "><p id="p12977743173013"><a name="p12977743173013"></a><a name="p12977743173013"></a>DCMI_LP_SUB_CMD_STATUS</p>
</td>
<td class="cellrowborder" valign="top" width="23.200000000000003%" headers="mcps1.2.5.1.2 "><p id="p597754320305"><a name="p597754320305"></a><a name="p597754320305"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="18.6%" headers="mcps1.2.5.1.3 "><p id="p097724315305"><a name="p097724315305"></a><a name="p097724315305"></a>sizeof(unsigned int)</p>
</td>
<td class="cellrowborder" valign="top" width="25.569999999999997%" headers="mcps1.2.5.1.4 "><p id="p16977243183018"><a name="p16977243183018"></a><a name="p16977243183018"></a>非空闲：0；空闲：1</p>
</td>
</tr>
</tbody>
</table>

子命令DCMI\_LP\_SUB\_CMD\_AICORE\_FREQREDUC\_CAUSE下，AI Core降频原因由一个64位的值表示，每个bit对应一种降频原因。当值为0时，说明AI Core以额定的频率运行。当值为1时，说明是由于某种原因引起AI Core不能以额定频率运行。同时由于降频原因可能由多个因素引起，所以可能存在多个bit被同时置1的情况。

**表 2**  buf值各bit对应的含义

<a name="table188972043193011"></a>
<table><thead align="left"><tr id="row8977543133019"><th class="cellrowborder" valign="top" width="25.27%" id="mcps1.2.4.1.1"><p id="p7978843103017"><a name="p7978843103017"></a><a name="p7978843103017"></a>名称</p>
</th>
<th class="cellrowborder" valign="top" width="25.27%" id="mcps1.2.4.1.2"><p id="p59781843103015"><a name="p59781843103015"></a><a name="p59781843103015"></a>Bit位</p>
</th>
<th class="cellrowborder" valign="top" width="49.46%" id="mcps1.2.4.1.3"><p id="p797834319309"><a name="p797834319309"></a><a name="p797834319309"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row7978143143012"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="p1497834315307"><a name="p1497834315307"></a><a name="p1497834315307"></a>IDLE</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="p129787433303"><a name="p129787433303"></a><a name="p129787433303"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p17978164383013"><a name="p17978164383013"></a><a name="p17978164383013"></a>AI Core处于空闲状态，通过将AI Core频率降低到空闲时的频率来降低功耗。空闲状态需要持续一段时间，频率才会切换。</p>
</td>
</tr>
<tr id="row397814314309"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="p12978194317301"><a name="p12978194317301"></a><a name="p12978194317301"></a>THERMAL</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="p16978194383019"><a name="p16978194383019"></a><a name="p16978194383019"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p897894393014"><a name="p897894393014"></a><a name="p897894393014"></a>昇腾AI处理器温度超过了允许的范围导致底层软件将AI Core的频率限制在一定的范围，从而来降低芯片的温度。</p>
</td>
</tr>
<tr id="row297804343016"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="p6978124373015"><a name="p6978124373015"></a><a name="p6978124373015"></a>SW_EDP</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="p1097824314304"><a name="p1097824314304"></a><a name="p1097824314304"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p997864373012"><a name="p997864373012"></a><a name="p997864373012"></a>在昇腾AI处理器上，AI Core模块的供电电流超过了允许的范围导致底层软件将AI Core的频率限制在一定的范围，保证AI Core模块供电稳定。</p>
</td>
</tr>
<tr id="row1897894313010"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="p1897816431303"><a name="p1897816431303"></a><a name="p1897816431303"></a>HW_EDP</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="p59781343123016"><a name="p59781343123016"></a><a name="p59781343123016"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p1497804343013"><a name="p1497804343013"></a><a name="p1497804343013"></a>在昇腾AI处理器上，AI Core模块的瞬态供电电流超过了允许的范围导致主板上的电流传感器触发AI Core模块快速降频。</p>
</td>
</tr>
<tr id="row119787436308"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="p197813434307"><a name="p197813434307"></a><a name="p197813434307"></a>POWER_BREAK</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="p4978144310304"><a name="p4978144310304"></a><a name="p4978144310304"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p597894323019"><a name="p597894323019"></a><a name="p597894323019"></a>主板上的功率监测模块监测到供电功率超过了允许的最大上限，通知昇腾AI处理器将AI Core的频率快速降低，维持供电稳定。</p>
</td>
</tr>
<tr id="row4978144318302"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="p897874303013"><a name="p897874303013"></a><a name="p897874303013"></a>SVFD</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="p11978134316303"><a name="p11978134316303"></a><a name="p11978134316303"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p997804363012"><a name="p997804363012"></a><a name="p997804363012"></a>昇腾AI处理器上的AI Core供电监测模块监测到AI Core模块的供电不稳（有噪声），触发AI Core快速降频，维持供电稳定。</p>
</td>
</tr>
<tr id="row730384616819"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000001312397937_p11311634212"><a name="zh-cn_topic_0000001312397937_p11311634212"></a><a name="zh-cn_topic_0000001312397937_p11311634212"></a>POWERCAPPING</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000001312397937_p169341949144114"><a name="zh-cn_topic_0000001312397937_p169341949144114"></a><a name="zh-cn_topic_0000001312397937_p169341949144114"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p13043467813"><a name="p13043467813"></a><a name="p13043467813"></a>昇腾AI处理器通过iBMC带外进行的功耗控制。</p>
</td>
</tr>
<tr id="row667034813815"><td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000001312397937_p1761405971619"><a name="zh-cn_topic_0000001312397937_p1761405971619"></a><a name="zh-cn_topic_0000001312397937_p1761405971619"></a>LOAD_AWARE</p>
</td>
<td class="cellrowborder" valign="top" width="25.27%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000001312397937_p161435931619"><a name="zh-cn_topic_0000001312397937_p161435931619"></a><a name="zh-cn_topic_0000001312397937_p161435931619"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="49.46%" headers="mcps1.2.4.1.3 "><p id="p116708488813"><a name="p116708488813"></a><a name="p116708488813"></a>昇腾AI处理器通过AI负载感知模块进行的功耗控制。</p>
</td>
</tr>
</tbody>
</table>

**表 3** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section207091751457"></a>**

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
int sub_cmd=0;
unsigned int voltage_cruuent_buf = 0;
unsigned int buf_size = 8;
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_LP, sub_cmd, &voltage_cruuent_buf, &buf_size);
if (ret != 0) {
//todo
return ret;
} else {
// todo
return ret;
}
…
```


## DCMI\_MAIN\_CMD\_TS命令说明<a name="ZH-CN_TOPIC_0000002517558715"></a>

**函数原型<a name="section782265716308"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section19822105710305"></a>**

获取系统中TS相关信息。

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
<p id="p106451722987"><a name="p106451722987"></a><a name="p106451722987"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
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
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><pre class="codeblock" id="codeblock84586272352"><a name="codeblock84586272352"></a><a name="codeblock84586272352"></a>typedef enum {
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
// 获取算子超时时间刻度值，默认32，取值范围为[20,32]。
DCMI_TS_SUB_CMD_COMMON_MSG = 11,
DCMI_TS_SUB_CMD_MAX,
} DCMI_TS_SUB_CMD;</pre>
<p id="p20900857113015"><a name="p20900857113015"></a><a name="p20900857113015"></a>不支持DCMI_TS_SUB_CMD_SET_FAULT_MASK、DCMI_TS_SUB_CMD_GET_FAULT_MASK</p>
<p id="p7900115703014"><a name="p7900115703014"></a><a name="p7900115703014"></a>开启profiling时，查询单核利用率结果为0xEF。</p>
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

**返回值说明<a name="section283355715302"></a>**

<a name="table1985018574309"></a>
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

**异常处理<a name="section6438163417415"></a>**

无。

**约束说明<a name="section2889103794114"></a>**

查询Vector Core的单核利用率时，buf至少为50Byte内存空间，查询AI Core的单核利用率时，buf至少为25Byte空间。

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
<tbody><tr id="row12901185711304"><td class="cellrowborder" valign="top" width="39.269999999999996%" headers="mcps1.2.4.1.1 "><p id="p8901195718303"><a name="p8901195718303"></a><a name="p8901195718303"></a>DCMI_TS_SUB_CMD_AICORE_UTILIZATION_RATE、DCMI_TS_SUB_CMD_VECTORCORE_UTILIZATION_RATE</p>
</td>
<td class="cellrowborder" valign="top" width="36.96%" headers="mcps1.2.4.1.2 "><p id="p1290105717308"><a name="p1290105717308"></a><a name="p1290105717308"></a>查询Vector Core的单核利用率时，buf至少为50Byte内存空间，查询AI Core的单核利用率时，buf至少为25Byte空间。</p>
<p id="p79011657193018"><a name="p79011657193018"></a><a name="p79011657193018"></a>异常值：</p>
<p id="p29014575305"><a name="p29014575305"></a><a name="p29014575305"></a>0xEE：表示对应的core损坏；</p>
<p id="p4901115718309"><a name="p4901115718309"></a><a name="p4901115718309"></a>0xEF：无效值；</p>
<p id="p290185713306"><a name="p290185713306"></a><a name="p290185713306"></a>出参时每个字节表示一个核的利用率，正常范围0-100。</p>
</td>
<td class="cellrowborder" valign="top" width="23.77%" headers="mcps1.2.4.1.3 "><p id="p490115713018"><a name="p490115713018"></a><a name="p490115713018"></a>作为入参时表示buf的大小；</p>
<p id="p2090125710307"><a name="p2090125710307"></a><a name="p2090125710307"></a>作为出参时表示buf内填充的有效值的个数。</p>
</td>
</tr>
<tr id="row6901185719303"><td class="cellrowborder" valign="top" width="39.269999999999996%" headers="mcps1.2.4.1.1 "><p id="p179011357143018"><a name="p179011357143018"></a><a name="p179011357143018"></a>DCMI_TS_SUB_CMD_FFTS_TYPE</p>
</td>
<td class="cellrowborder" valign="top" width="36.96%" headers="mcps1.2.4.1.2 "><p id="p89011657113020"><a name="p89011657113020"></a><a name="p89011657113020"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="23.77%" headers="mcps1.2.4.1.3 "><p id="p12901115712303"><a name="p12901115712303"></a><a name="p12901115712303"></a>unsigned int</p>
</td>
</tr>
<tr id="row19471316207"><td class="cellrowborder" valign="top" width="39.269999999999996%" headers="mcps1.2.4.1.1 "><p id="p1047117132015"><a name="p1047117132015"></a><a name="p1047117132015"></a>DCMI_TS_SUB_CMD_COMMON_MSG</p>
</td>
<td class="cellrowborder" valign="top" width="36.96%" headers="mcps1.2.4.1.2 "><p id="p19469111244813"><a name="p19469111244813"></a><a name="p19469111244813"></a>struct ts_dcmi_ctrl_msg_body_t {</p>
<p id="p1246961219488"><a name="p1246961219488"></a><a name="p1246961219488"></a>unsigned int msg_type;</p>
<p id="p246915123487"><a name="p246915123487"></a><a name="p246915123487"></a>union {</p>
<p id="p1746901219481"><a name="p1746901219481"></a><a name="p1746901219481"></a>ts_dcmi_task_timeout_t set_task_timeout_info;</p>
<p id="p64691512154816"><a name="p64691512154816"></a><a name="p64691512154816"></a>ts_dcmi_task_timeout_t get_task_timeout_info;</p>
<p id="p13469131244812"><a name="p13469131244812"></a><a name="p13469131244812"></a>} u;</p>
<p id="p134691512204817"><a name="p134691512204817"></a><a name="p134691512204817"></a>};</p>
<p id="p1719316326501"><a name="p1719316326501"></a><a name="p1719316326501"></a>ts_dcmi_task_timeout_t结构体如下：</p>
<p id="p522516213502"><a name="p522516213502"></a><a name="p522516213502"></a>typedef struct {</p>
<p id="p32251323501"><a name="p32251323501"></a><a name="p32251323501"></a>unsigned int timeout_limit_exp; // 取值范围为[20，32]</p>
<p id="p222512155013"><a name="p222512155013"></a><a name="p222512155013"></a>unsigned char rsev[TS_RSEV_MAX_LENTH]; //TS_RSEV_MAX_LENTH为36</p>
<p id="p10225725506"><a name="p10225725506"></a><a name="p10225725506"></a>} ts_dcmi_task_timeout_t;</p>
</td>
<td class="cellrowborder" valign="top" width="23.77%" headers="mcps1.2.4.1.3 "><p id="p19359252485"><a name="p19359252485"></a><a name="p19359252485"></a>作为入参时表示buf的大小，buf至少为44Byte。</p>
<p id="p19218195310"><a name="p19218195310"></a><a name="p19218195310"></a>msg_type取值为0或1。</p>
<a name="ul16848433145315"></a><a name="ul16848433145315"></a><ul id="ul16848433145315"><li>1表示设置AI CPU算子超时时间刻度。</li><li>0表示查询AI CPU算子超时时间刻度。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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
ret = dcmi_get_device_info(card_id,dev_id,DCMI_MAIN_CMD_TS, DCMI_TS_SUB_CMD_AICORE_UTILIZATION_RATE, buf, &buf_size);
if (ret != 0) {
printf("dcmi_get_device_info failed, ret = %d.\n", ret);
return -1;
}
return 0;
…
```


## DCMI\_MAIN\_CMD\_QOS命令说明<a name="ZH-CN_TOPIC_0000002485318820"></a>

**函数原型<a name="section14467101743119"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section204687171315"></a>**

获取QoS相关配置，包含获取指定的mpamid对应的QoS配置、指定master对应的QoS配置、带宽的统计值。

**参数说明<a name="section1546861713117"></a>**

<a name="table150171718316"></a>
<table><thead align="left"><tr id="row45967179312"><th class="cellrowborder" valign="top" width="17.17171717171717%" id="mcps1.1.5.1.1"><p id="p1359641710311"><a name="p1359641710311"></a><a name="p1359641710311"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.151515151515152%" id="mcps1.1.5.1.2"><p id="p1596181753116"><a name="p1596181753116"></a><a name="p1596181753116"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.19191919191919%" id="mcps1.1.5.1.3"><p id="p13596161712315"><a name="p13596161712315"></a><a name="p13596161712315"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.484848484848484%" id="mcps1.1.5.1.4"><p id="p1359691733113"><a name="p1359691733113"></a><a name="p1359691733113"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1159661713116"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p259611174316"><a name="p259611174316"></a><a name="p259611174316"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p3596151773118"><a name="p3596151773118"></a><a name="p3596151773118"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p059611170317"><a name="p059611170317"></a><a name="p059611170317"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p959612177316"><a name="p959612177316"></a><a name="p959612177316"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row19596817133118"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p185961717183110"><a name="p185961717183110"></a><a name="p185961717183110"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p195961517143118"><a name="p195961517143118"></a><a name="p195961517143118"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p1359617171318"><a name="p1359617171318"></a><a name="p1359617171318"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p059671763113"><a name="p059671763113"></a><a name="p059671763113"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p20596131743115"><a name="p20596131743115"></a><a name="p20596131743115"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p45431335585"><a name="p45431335585"></a><a name="p45431335585"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row459661713310"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p105971917113120"><a name="p105971917113120"></a><a name="p105971917113120"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p165973173319"><a name="p165973173319"></a><a name="p165973173319"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p3597617163117"><a name="p3597617163117"></a><a name="p3597617163117"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p45971417163119"><a name="p45971417163119"></a><a name="p45971417163119"></a>DCMI_MAIN_CMD_QOS</p>
</td>
</tr>
<tr id="row25975175318"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p659741718311"><a name="p659741718311"></a><a name="p659741718311"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p185971617163113"><a name="p185971617163113"></a><a name="p185971617163113"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p175971117123115"><a name="p175971117123115"></a><a name="p175971117123115"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><pre class="codeblock" id="codeblock13311208103610"><a name="codeblock13311208103610"></a><a name="codeblock13311208103610"></a>typedef enum {
// 获取指定MATA的配置信息
DCMI_QOS_SUB_MATA_CONFIG,
// 获取指定master的配置信息
DCMI_QOS_SUB_MASTER_CONFIG,
// 获取带宽的统计信息
DCMI_QOS_SUB_BW_DATA,
// 获取通用配置信息
DCMI_QOS_SUB_GLOBAL_CONFIG,
// 配置完成指令
DCMI_QOS_SUB_CONFIG_DONE,
} DCMI_QOS_SUB_INFO;</pre>
<p id="p5929173053615"><a name="p5929173053615"></a><a name="p5929173053615"></a>当前仅支持DCMI_QOS_SUB_MATA_CONFIG、DCMI_QOS_SUB_MASTER_CONFIG、DCMI_QOS_SUB_BW_DATA、DCMI_QOS_SUB_GLOBAL_CONFIG。</p>
</td>
</tr>
<tr id="row1059751753117"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p165976179310"><a name="p165976179310"></a><a name="p165976179310"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p105971717153116"><a name="p105971717153116"></a><a name="p105971717153116"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p13597191753119"><a name="p13597191753119"></a><a name="p13597191753119"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p13597161723117"><a name="p13597161723117"></a><a name="p13597161723117"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row1597151743112"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p05974176319"><a name="p05974176319"></a><a name="p05974176319"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p45972017113117"><a name="p45972017113117"></a><a name="p45972017113117"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p8597717103117"><a name="p8597717103117"></a><a name="p8597717103117"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p1759791793116"><a name="p1759791793116"></a><a name="p1759791793116"></a>buf数组的长度/返回结果数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section34771217163116"></a>**

<a name="table165071517183118"></a>
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

**异常处理<a name="section9677135115517"></a>**

无。

**约束说明<a name="section2977135411511"></a>**

-   受soc特性约束，需要调用DCMI\_set\_device\_info后再调用相应get接口读取配置是否生效，否则读取值不可信。
-   resctrl获取带宽功能与DCMI接口获取实时带宽功能冲突，如果已经使能其中一种，请勿并行使用另外一种。

**表 1**  sub\_cmd对应的buf格式

<a name="table1450912173312"></a>
<table><thead align="left"><tr id="row1359861743110"><th class="cellrowborder" valign="top" width="31.403140314031404%" id="mcps1.2.5.1.1"><p id="p16598617103119"><a name="p16598617103119"></a><a name="p16598617103119"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="20.122012201220123%" id="mcps1.2.5.1.2"><p id="p8598131713110"><a name="p8598131713110"></a><a name="p8598131713110"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="15.701570157015702%" id="mcps1.2.5.1.3"><p id="p195981717153117"><a name="p195981717153117"></a><a name="p195981717153117"></a>buf_size</p>
</th>
<th class="cellrowborder" valign="top" width="32.77327732773277%" id="mcps1.2.5.1.4"><p id="p559831773117"><a name="p559831773117"></a><a name="p559831773117"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row1459821713316"><td class="cellrowborder" valign="top" width="31.403140314031404%" headers="mcps1.2.5.1.1 "><p id="p659871712318"><a name="p659871712318"></a><a name="p659871712318"></a>DCMI_QOS_SUB_MATA_CONFIG</p>
</td>
<td class="cellrowborder" valign="top" width="20.122012201220123%" headers="mcps1.2.5.1.2 "><p id="p105981117163113"><a name="p105981117163113"></a><a name="p105981117163113"></a>struct qos_mata_config</p>
<p id="p3598917133118"><a name="p3598917133118"></a><a name="p3598917133118"></a>{</p>
<p id="p15598181793119"><a name="p15598181793119"></a><a name="p15598181793119"></a>int mpamid;</p>
<p id="p14598121753118"><a name="p14598121753118"></a><a name="p14598121753118"></a>u32 bw_high;</p>
<p id="p105981917153115"><a name="p105981917153115"></a><a name="p105981917153115"></a>u32 bw_low;</p>
<p id="p7598117133110"><a name="p7598117133110"></a><a name="p7598117133110"></a>int hardlimit;</p>
<p id="p359881793112"><a name="p359881793112"></a><a name="p359881793112"></a>int reserved[8];</p>
<p id="p45981317203119"><a name="p45981317203119"></a><a name="p45981317203119"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="15.701570157015702%" headers="mcps1.2.5.1.3 "><p id="p85982017103119"><a name="p85982017103119"></a><a name="p85982017103119"></a>sizeof(qos_mata_</p>
<p id="p65987174315"><a name="p65987174315"></a><a name="p65987174315"></a>config)</p>
</td>
<td class="cellrowborder" valign="top" width="32.77327732773277%" headers="mcps1.2.5.1.4 "><p id="p55981017153116"><a name="p55981017153116"></a><a name="p55981017153116"></a>mpamid：取值范围是[0,127]。</p>
<p id="p959881723117"><a name="p959881723117"></a><a name="p959881723117"></a>bw_high：上水线(GB/s)</p>
<p id="p15598141713319"><a name="p15598141713319"></a><a name="p15598141713319"></a>取值范围是[0,1638]。</p>
<p id="p1959917172315"><a name="p1959917172315"></a><a name="p1959917172315"></a>bw_low：下水线(GB/s)，取值范围是[0, bw_high]</p>
<p id="p20599121793112"><a name="p20599121793112"></a><a name="p20599121793112"></a>hardlimit：1表示开启，0表示不开启</p>
<p id="p13599191773117"><a name="p13599191773117"></a><a name="p13599191773117"></a>用户通过该接口读取水线，可能同先前用户配置值存在误差。误差值计算方式为：处理器最大带宽/MAX_REG_VALUE。其中，MAX_REG_VALUE取值为1024。</p>
</td>
</tr>
<tr id="row1599121733110"><td class="cellrowborder" valign="top" width="31.403140314031404%" headers="mcps1.2.5.1.1 "><p id="p359910173318"><a name="p359910173318"></a><a name="p359910173318"></a>DCMI_QOS_SUB_MASTER_CONFIG</p>
</td>
<td class="cellrowborder" valign="top" width="20.122012201220123%" headers="mcps1.2.5.1.2 "><p id="p125995179319"><a name="p125995179319"></a><a name="p125995179319"></a>struct qos_master_config</p>
<p id="p1659941753115"><a name="p1659941753115"></a><a name="p1659941753115"></a>{</p>
<p id="p165991417173110"><a name="p165991417173110"></a><a name="p165991417173110"></a>int master;</p>
<p id="p205995175318"><a name="p205995175318"></a><a name="p205995175318"></a>int mpamid;</p>
<p id="p45991117143117"><a name="p45991117143117"></a><a name="p45991117143117"></a>int qos;</p>
<p id="p145992172312"><a name="p145992172312"></a><a name="p145992172312"></a>int pmg;</p>
<p id="p1859961783120"><a name="p1859961783120"></a><a name="p1859961783120"></a>u64 bitmap[4];</p>
<p id="p4599217193119"><a name="p4599217193119"></a><a name="p4599217193119"></a>int reserved[8];</p>
<p id="p4599171710317"><a name="p4599171710317"></a><a name="p4599171710317"></a>};</p>
<p id="p1599101711319"><a name="p1599101711319"></a><a name="p1599101711319"></a></p>
</td>
<td class="cellrowborder" valign="top" width="15.701570157015702%" headers="mcps1.2.5.1.3 "><p id="p259917179318"><a name="p259917179318"></a><a name="p259917179318"></a>sizeof(qos_master</p>
<p id="p18599121713116"><a name="p18599121713116"></a><a name="p18599121713116"></a>_config)</p>
</td>
<td class="cellrowborder" valign="top" width="32.77327732773277%" headers="mcps1.2.5.1.4 "><p id="p1659911753116"><a name="p1659911753116"></a><a name="p1659911753116"></a>master：master ID，支持配置的项为：</p>
<p id="p13599317193112"><a name="p13599317193112"></a><a name="p13599317193112"></a>vdec=1,vpc=2,jpge=3,</p>
<p id="p1759951773118"><a name="p1759951773118"></a><a name="p1759951773118"></a>jpgd=4,pcie=7,sdma=13</p>
<p id="p959901703114"><a name="p959901703114"></a><a name="p959901703114"></a>mpamid ：取值范围是[0,127]</p>
<p id="p9599517133119"><a name="p9599517133119"></a><a name="p9599517133119"></a>qos：带宽调度优先级，取值范围[0,7]，0作为hardlimit专用qos，7为调度绿色通道qos</p>
<p id="p1459901712318"><a name="p1459901712318"></a><a name="p1459901712318"></a>pmg：mpamid分组，取值范围是[0,3](当前不支持)</p>
<p id="p11599171718314"><a name="p11599171718314"></a><a name="p11599171718314"></a>bitmap：因框架限制，不支持</p>
</td>
</tr>
<tr id="row185991417153117"><td class="cellrowborder" valign="top" width="31.403140314031404%" headers="mcps1.2.5.1.1 "><p id="p259931753110"><a name="p259931753110"></a><a name="p259931753110"></a>DCMI_QOS_SUB_BW_DATA</p>
</td>
<td class="cellrowborder" valign="top" width="20.122012201220123%" headers="mcps1.2.5.1.2 "><p id="p8599817113120"><a name="p8599817113120"></a><a name="p8599817113120"></a>struct qos_bw_result</p>
<p id="p12600917113118"><a name="p12600917113118"></a><a name="p12600917113118"></a>{</p>
<p id="p18600131743116"><a name="p18600131743116"></a><a name="p18600131743116"></a>int mpamid;</p>
<p id="p4600101763114"><a name="p4600101763114"></a><a name="p4600101763114"></a>u32 curr;</p>
<p id="p11600191714316"><a name="p11600191714316"></a><a name="p11600191714316"></a>u32 bw_max;</p>
<p id="p060018172318"><a name="p060018172318"></a><a name="p060018172318"></a>u32 bw_min;</p>
<p id="p960051723117"><a name="p960051723117"></a><a name="p960051723117"></a>u32 bw_mean;</p>
<p id="p86000174315"><a name="p86000174315"></a><a name="p86000174315"></a>int reserved[8];</p>
<p id="p16003175318"><a name="p16003175318"></a><a name="p16003175318"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="15.701570157015702%" headers="mcps1.2.5.1.3 "><p id="p156001179310"><a name="p156001179310"></a><a name="p156001179310"></a>sizeof(qos_bw_result)</p>
</td>
<td class="cellrowborder" valign="top" width="32.77327732773277%" headers="mcps1.2.5.1.4 "><p id="p136002017103117"><a name="p136002017103117"></a><a name="p136002017103117"></a>mpamid：获取带宽的目标mpamid。</p>
<p id="p1860021719315"><a name="p1860021719315"></a><a name="p1860021719315"></a>取值范围是[0,127]。</p>
<p id="p1460019179315"><a name="p1460019179315"></a><a name="p1460019179315"></a>curr：最近时间点获取的带宽值(MB/s)</p>
<p id="p5600101793110"><a name="p5600101793110"></a><a name="p5600101793110"></a>bw_max：采样时间段内最大值(MB/s)</p>
<p id="p1060010171314"><a name="p1060010171314"></a><a name="p1060010171314"></a>bw_min：采样时间段内最小值(MB/s)</p>
<p id="p10600617153119"><a name="p10600617153119"></a><a name="p10600617153119"></a>bw_mean：采样时间段内的平均值(MB/s)</p>
</td>
</tr>
<tr id="row1560041716314"><td class="cellrowborder" valign="top" width="31.403140314031404%" headers="mcps1.2.5.1.1 "><p id="p12600917143112"><a name="p12600917143112"></a><a name="p12600917143112"></a>DCMI_QOS_SUB_GLOBAL_CONFIG</p>
</td>
<td class="cellrowborder" valign="top" width="20.122012201220123%" headers="mcps1.2.5.1.2 "><p id="p3600717123114"><a name="p3600717123114"></a><a name="p3600717123114"></a>struct qos_gbl_config</p>
<p id="p76008172312"><a name="p76008172312"></a><a name="p76008172312"></a>{</p>
<p id="p3600131783116"><a name="p3600131783116"></a><a name="p3600131783116"></a>u32 enable;</p>
<p id="p126002177317"><a name="p126002177317"></a><a name="p126002177317"></a>u32 autoqos_fuse_en;</p>
<p id="p860021763110"><a name="p860021763110"></a><a name="p860021763110"></a>u32 mpamqos_fuse_mode;</p>
<p id="p16001617133115"><a name="p16001617133115"></a><a name="p16001617133115"></a>u32 mpam_subtype;</p>
<p id="p1600117143115"><a name="p1600117143115"></a><a name="p1600117143115"></a>int reserved[8];</p>
<p id="p126006174314"><a name="p126006174314"></a><a name="p126006174314"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="15.701570157015702%" headers="mcps1.2.5.1.3 "><p id="p14600617163113"><a name="p14600617163113"></a><a name="p14600617163113"></a>sizeof(qos_gbl_config)</p>
</td>
<td class="cellrowborder" valign="top" width="32.77327732773277%" headers="mcps1.2.5.1.4 "><a name="ul15992846123713"></a><a name="ul15992846123713"></a><ul id="ul15992846123713"><li>enable：是否使能QoS功能<a name="ul1426935273713"></a><a name="ul1426935273713"></a><ul id="ul1426935273713"><li>0表示不使能</li><li>1表示使能</li></ul>
</li><li>autoqos_fuse_en：qos的融合开关<a name="ul161181811163814"></a><a name="ul161181811163814"></a><ul id="ul161181811163814"><li>0表示关闭qos融合</li><li>1表示开始qos融合</li></ul>
</li><li>mpamqos_fuse_mode：qos的融合模式，autoqos_fuse_en开启的条件下生效<a name="ul879644411386"></a><a name="ul879644411386"></a><ul id="ul879644411386"><li>0表示均值融合</li><li>1表示取随路qos和mpamqos之间的最大值作为融合结果</li><li>2表示使用随路qos替换mpamqos</li></ul>
</li><li>mpam_subtype：带宽统计的模式。<a name="ul16435328393"></a><a name="ul16435328393"></a><ul id="ul16435328393"><li>0表示统计读+写带宽</li><li>1表示统计写带宽</li><li>2表示统计读带宽</li></ul>
</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row913516862011"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p15076476392"><a name="zh-cn_topic_0000002485318818_p15076476392"></a><a name="zh-cn_topic_0000002485318818_p15076476392"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p16507547153913"><a name="zh-cn_topic_0000002485318818_p16507547153913"></a><a name="zh-cn_topic_0000002485318818_p16507547153913"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p1950718474398"><a name="zh-cn_topic_0000002485318818_p1950718474398"></a><a name="zh-cn_topic_0000002485318818_p1950718474398"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p165071047113911"><a name="zh-cn_topic_0000002485318818_p165071047113911"></a><a name="zh-cn_topic_0000002485318818_p165071047113911"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row31351488206"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p6135185208"><a name="zh-cn_topic_0000002485318818_p6135185208"></a><a name="zh-cn_topic_0000002485318818_p6135185208"></a><span id="zh-cn_topic_0000002485318818_text1213528172016"><a name="zh-cn_topic_0000002485318818_text1213528172016"></a><a name="zh-cn_topic_0000002485318818_text1213528172016"></a>Atlas 900 A3 SuperPoD 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1213511813205"><a name="zh-cn_topic_0000002485318818_p1213511813205"></a><a name="zh-cn_topic_0000002485318818_p1213511813205"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p613528102020"><a name="zh-cn_topic_0000002485318818_p613528102020"></a><a name="zh-cn_topic_0000002485318818_p613528102020"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p713512862013"><a name="zh-cn_topic_0000002485318818_p713512862013"></a><a name="zh-cn_topic_0000002485318818_p713512862013"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1813515811208"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p71354810207"><a name="zh-cn_topic_0000002485318818_p71354810207"></a><a name="zh-cn_topic_0000002485318818_p71354810207"></a><span id="zh-cn_topic_0000002485318818_text51358812207"><a name="zh-cn_topic_0000002485318818_text51358812207"></a><a name="zh-cn_topic_0000002485318818_text51358812207"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p213568152019"><a name="zh-cn_topic_0000002485318818_p213568152019"></a><a name="zh-cn_topic_0000002485318818_p213568152019"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1613548142019"><a name="zh-cn_topic_0000002485318818_p1613548142019"></a><a name="zh-cn_topic_0000002485318818_p1613548142019"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p11351087209"><a name="zh-cn_topic_0000002485318818_p11351087209"></a><a name="zh-cn_topic_0000002485318818_p11351087209"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row87611016195410"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p8296259161515"><a name="zh-cn_topic_0000002485318818_p8296259161515"></a><a name="zh-cn_topic_0000002485318818_p8296259161515"></a><span id="zh-cn_topic_0000002485318818_text14296259131514"><a name="zh-cn_topic_0000002485318818_text14296259131514"></a><a name="zh-cn_topic_0000002485318818_text14296259131514"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1451920314542"><a name="zh-cn_topic_0000002485318818_p1451920314542"></a><a name="zh-cn_topic_0000002485318818_p1451920314542"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p2519133155413"><a name="zh-cn_topic_0000002485318818_p2519133155413"></a><a name="zh-cn_topic_0000002485318818_p2519133155413"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p11519153110545"><a name="zh-cn_topic_0000002485318818_p11519153110545"></a><a name="zh-cn_topic_0000002485318818_p11519153110545"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row0323633152810"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p064011372281"><a name="zh-cn_topic_0000002485318818_p064011372281"></a><a name="zh-cn_topic_0000002485318818_p064011372281"></a><span id="zh-cn_topic_0000002485318818_text18640537162811"><a name="zh-cn_topic_0000002485318818_text18640537162811"></a><a name="zh-cn_topic_0000002485318818_text18640537162811"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p279104012287"><a name="zh-cn_topic_0000002485318818_p279104012287"></a><a name="zh-cn_topic_0000002485318818_p279104012287"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p779112409289"><a name="zh-cn_topic_0000002485318818_p779112409289"></a><a name="zh-cn_topic_0000002485318818_p779112409289"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1791140112816"><a name="zh-cn_topic_0000002485318818_p1791140112816"></a><a name="zh-cn_topic_0000002485318818_p1791140112816"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row49081418125411"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p512952510545"><a name="zh-cn_topic_0000002485318818_p512952510545"></a><a name="zh-cn_topic_0000002485318818_p512952510545"></a><span id="zh-cn_topic_0000002485318818_text012942512545"><a name="zh-cn_topic_0000002485318818_text012942512545"></a><a name="zh-cn_topic_0000002485318818_text012942512545"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1260932135413"><a name="zh-cn_topic_0000002485318818_p1260932135413"></a><a name="zh-cn_topic_0000002485318818_p1260932135413"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1660133285419"><a name="zh-cn_topic_0000002485318818_p1660133285419"></a><a name="zh-cn_topic_0000002485318818_p1660133285419"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p46017321545"><a name="zh-cn_topic_0000002485318818_p46017321545"></a><a name="zh-cn_topic_0000002485318818_p46017321545"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1625111184"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p1386118141085"><a name="zh-cn_topic_0000002485318818_p1386118141085"></a><a name="zh-cn_topic_0000002485318818_p1386118141085"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section176618596619"></a>**

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
int sub_cmd=0;
int size = sizeof(struct dcmi_qos_mata_config);
struct dcmi_qos_mata_config mataCfg = {0};
mataCfg.mpamid = 127;
unsigned int subCmd = (unsigned int)(DCMI_QOS_SUB_CMD_MAKE(mataCfg.mpamid, DCMI_QOS_SUB_MATA_CONFIG));
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_QOS, subCmd, (void *)&mataCfg, &size);
if (ret != 0) {
//todo
return ret;
} else {
// todo
return ret;
}
…
```


## DCMI\_MAIN\_CMD\_HCCS命令说明<a name="ZH-CN_TOPIC_0000002485478788"></a>

**函数原型<a name="section208203016314"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section15811309317"></a>**

获取HCCS信息。

**参数说明<a name="section198123020315"></a>**

<a name="table72213073115"></a>
<table><thead align="left"><tr id="row1672203023119"><th class="cellrowborder" valign="top" width="17.17171717171717%" id="mcps1.1.5.1.1"><p id="p187273012319"><a name="p187273012319"></a><a name="p187273012319"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.151515151515152%" id="mcps1.1.5.1.2"><p id="p173143013316"><a name="p173143013316"></a><a name="p173143013316"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.19191919191919%" id="mcps1.1.5.1.3"><p id="p873163083118"><a name="p873163083118"></a><a name="p873163083118"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.484848484848484%" id="mcps1.1.5.1.4"><p id="p2073630143115"><a name="p2073630143115"></a><a name="p2073630143115"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row12731304319"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p4730301317"><a name="p4730301317"></a><a name="p4730301317"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p12731730103114"><a name="p12731730103114"></a><a name="p12731730103114"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p20731830153112"><a name="p20731830153112"></a><a name="p20731830153112"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p1873930173118"><a name="p1873930173118"></a><a name="p1873930173118"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1073230183110"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p17731830153111"><a name="p17731830153111"></a><a name="p17731830153111"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p12731330203114"><a name="p12731330203114"></a><a name="p12731330203114"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p77314303319"><a name="p77314303319"></a><a name="p77314303319"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p117320301313"><a name="p117320301313"></a><a name="p117320301313"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p1073163019313"><a name="p1073163019313"></a><a name="p1073163019313"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p7350114610813"><a name="p7350114610813"></a><a name="p7350114610813"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row1973830183110"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p373130163117"><a name="p373130163117"></a><a name="p373130163117"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p127317307311"><a name="p127317307311"></a><a name="p127317307311"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p117310304314"><a name="p117310304314"></a><a name="p117310304314"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p1273193015315"><a name="p1273193015315"></a><a name="p1273193015315"></a>DCMI_MAIN_CMD_HCCS</p>
</td>
</tr>
<tr id="row373830113118"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p187333003113"><a name="p187333003113"></a><a name="p187333003113"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p873130123111"><a name="p873130123111"></a><a name="p873130123111"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p1773153003117"><a name="p1773153003117"></a><a name="p1773153003117"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p2591111610485"><a name="p2591111610485"></a><a name="p2591111610485"></a>typedef enum {</p>
<p id="p1659181664812"><a name="p1659181664812"></a><a name="p1659181664812"></a>DCMI_HCCS_CMD_GET_STATUS = 0,</p>
<p id="p18591416144813"><a name="p18591416144813"></a><a name="p18591416144813"></a>DCMI_HCCS_CMD_GET_LANE_INFO = 1,</p>
<p id="p10784121072814"><a name="p10784121072814"></a><a name="p10784121072814"></a>DCMI_HCCS_CMD_GET_STATISTIC_INFO = 3,</p>
<p id="p1828621611238"><a name="p1828621611238"></a><a name="p1828621611238"></a>DCMI_HCCS_CMD_GET_STATISTIC_INFO_U64 = 5,</p>
<p id="p125911116174815"><a name="p125911116174815"></a><a name="p125911116174815"></a>DCMI_HCCS_CMD_MAX,</p>
<p id="p359181694813"><a name="p359181694813"></a><a name="p359181694813"></a>} DCMI_HCCS_SUB_CMD;</p>
</td>
</tr>
<tr id="row10735307313"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p127343014311"><a name="p127343014311"></a><a name="p127343014311"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p37353017317"><a name="p37353017317"></a><a name="p37353017317"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p373193010317"><a name="p373193010317"></a><a name="p373193010317"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p107415309318"><a name="p107415309318"></a><a name="p107415309318"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row1074230113112"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p97423015311"><a name="p97423015311"></a><a name="p97423015311"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p674183073114"><a name="p674183073114"></a><a name="p674183073114"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p874183073116"><a name="p874183073116"></a><a name="p874183073116"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p11741830133113"><a name="p11741830133113"></a><a name="p11741830133113"></a>返回结果数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section111593003115"></a>**

<a name="table92723063116"></a>
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

**异常处理<a name="section77912615717"></a>**

无。

**约束说明<a name="section09411081372"></a>**

**表 1**  sub\_cmd对应的buf格式

<a name="table12297302311"></a>
<table><thead align="left"><tr id="row37413013317"><th class="cellrowborder" valign="top" width="23.22%" id="mcps1.2.5.1.1"><p id="p10741330113120"><a name="p10741330113120"></a><a name="p10741330113120"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="29.69%" id="mcps1.2.5.1.2"><p id="p1874030103112"><a name="p1874030103112"></a><a name="p1874030103112"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="11.709999999999999%" id="mcps1.2.5.1.3"><p id="p1374530193111"><a name="p1374530193111"></a><a name="p1374530193111"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="35.38%" id="mcps1.2.5.1.4"><p id="p1674030173116"><a name="p1674030173116"></a><a name="p1674030173116"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row1274193013316"><td class="cellrowborder" valign="top" width="23.22%" headers="mcps1.2.5.1.1 "><p id="p1274203019311"><a name="p1274203019311"></a><a name="p1274203019311"></a>DCMI_HCCS_CMD_GET_STATUS</p>
</td>
<td class="cellrowborder" valign="top" width="29.69%" headers="mcps1.2.5.1.2 "><p id="p156861185517"><a name="p156861185517"></a><a name="p156861185517"></a>struct dcmi_hccs_statues {</p>
<p id="p9681811135520"><a name="p9681811135520"></a><a name="p9681811135520"></a>unsigned int pcs_status;</p>
<p id="p11548325101116"><a name="p11548325101116"></a><a name="p11548325101116"></a>unsigned int hdlc_status;</p>
<p id="p1668151135514"><a name="p1668151135514"></a><a name="p1668151135514"></a>unsigned char reserve[DCMI_HCCS_STATUS_RESERVED_LEN];</p>
<p id="p1868151113554"><a name="p1868151113554"></a><a name="p1868151113554"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="11.709999999999999%" headers="mcps1.2.5.1.3 "><p id="p1474123013119"><a name="p1474123013119"></a><a name="p1474123013119"></a>sizeof(struct dcmi_hccs_statues)</p>
</td>
<td class="cellrowborder" valign="top" width="35.38%" headers="mcps1.2.5.1.4 "><p id="p275203013113"><a name="p275203013113"></a><a name="p275203013113"></a>查询HCCS状态，当前支持查询pcs和hdlc状态。</p>
<a name="ul5663163334913"></a><a name="ul5663163334913"></a><ul id="ul5663163334913"><li>pcs_status表示HCCS pcs状态，其值含义如下：<a name="ul13766848161417"></a><a name="ul13766848161417"></a><ul id="ul13766848161417"><li>0表示状态正常。</li><li>非0表示状态异常。</li></ul>
<p id="p2469153944918"><a name="p2469153944918"></a><a name="p2469153944918"></a>其中：</p>
<a name="ul1693262125011"></a><a name="ul1693262125011"></a><ul id="ul1693262125011"><li>0bit在状态异常下固定为1。</li><li>1-7bit当前固定为0，预留扩展。</li><li>8-15bit表示当前存在问题的PCS序号，当前序号的范围为0-7，此值表示从0开始第1个有问题的索引号。</li><li>16-23bit表示PCS当前发送的lane模式。<p id="p86611229115012"><a name="p86611229115012"></a><a name="p86611229115012"></a>0表示0lane。</p>
<p id="p176877310505"><a name="p176877310505"></a><a name="p176877310505"></a>1表示1lane。</p>
<p id="p0883143345017"><a name="p0883143345017"></a><a name="p0883143345017"></a>2表示4lane。</p>
<p id="p199651627105011"><a name="p199651627105011"></a><a name="p199651627105011"></a>3表示8lane。</p>
</li><li>24-31bit表示PCS当前发送的lane。</li></ul>
</li><li>hdlc_status表示HCCS hdlc状态，其值含义如下：<a name="ul15277145372620"></a><a name="ul15277145372620"></a><ul id="ul15277145372620"><li>2-31bit当前固定为0，预留扩展。</li><li>0-1bit表示HCCS hdlc初始化状态。<a name="ul11478295276"></a><a name="ul11478295276"></a><ul id="ul11478295276"><li>00表示等待初始化。</li><li>01表示收发初始化报文成功。</li><li>10表示等待结束初始化。</li><li>11表示初始化成功（HCCS hdlc状态正常）。</li></ul>
</li></ul>
</li></ul>
</td>
</tr>
<tr id="row3561143218479"><td class="cellrowborder" valign="top" width="23.22%" headers="mcps1.2.5.1.1 "><p id="p2266153518472"><a name="p2266153518472"></a><a name="p2266153518472"></a>DCMI_HCCS_CMD_GET_LANE_INFO</p>
</td>
<td class="cellrowborder" valign="top" width="29.69%" headers="mcps1.2.5.1.2 "><p id="p1126619351474"><a name="p1126619351474"></a><a name="p1126619351474"></a>struct dcmi_hccs_lane_info {</p>
<p id="p926603514474"><a name="p926603514474"></a><a name="p926603514474"></a>unsigned int hccs_port_pcs_bitmap;</p>
<p id="p11266133513473"><a name="p11266133513473"></a><a name="p11266133513473"></a>unsigned int pcs_lane_bitmap[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p626673519479"><a name="p626673519479"></a><a name="p626673519479"></a>unsigned int reserve[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p10266135194714"><a name="p10266135194714"></a><a name="p10266135194714"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="11.709999999999999%" headers="mcps1.2.5.1.3 "><p id="p14266435114716"><a name="p14266435114716"></a><a name="p14266435114716"></a>sizeof(struct dcmi_hccs_lane_info)</p>
</td>
<td class="cellrowborder" valign="top" width="35.38%" headers="mcps1.2.5.1.4 "><a name="ul8832161775120"></a><a name="ul8832161775120"></a><ul id="ul8832161775120"><li>hccs_port_pcs_bitmap：当前用于hccs连接PCS的bit位，对应序号从低位开始计数，写0代表非用于hccs，写1代表用于hccs。</li><li>pcs_lane_bitmap：当前用于hccs连接的lane信息数组，数组顺序与hccs_port_pcs_bitmap从低位开始写‘1’的bit位对应，其值含义如下：<a name="ul132662358472"></a><a name="ul132662358472"></a><ul id="ul132662358472"><li>0bit：是否切换到hccs完成。<p id="p88104120528"><a name="p88104120528"></a><a name="p88104120528"></a>1表示完成。</p>
<p id="p920790135211"><a name="p920790135211"></a><a name="p920790135211"></a>0表示其余bit位的值均无效。</p>
</li><li>1-8bit：当前hccs连接用的lane序号，从低位开始，计数范围0-3。<p id="p152591425165312"><a name="p152591425165312"></a><a name="p152591425165312"></a>值为1表示该lane为当前发送使用。</p>
<p id="p257122111530"><a name="p257122111530"></a><a name="p257122111530"></a>值为0表示未被使用。</p>
</li><li>9-10bit：当前hccs连接用的lane模式。<p id="p131154611532"><a name="p131154611532"></a><a name="p131154611532"></a>00为0lane。</p>
<p id="p5608347145319"><a name="p5608347145319"></a><a name="p5608347145319"></a>01为1lane。</p>
<p id="p0579103145413"><a name="p0579103145413"></a><a name="p0579103145413"></a>10为4lane。</p>
<p id="p184709442533"><a name="p184709442533"></a><a name="p184709442533"></a>11为2lane。</p>
</li></ul>
</li></ul>
</td>
</tr>
<tr id="row515051473110"><td class="cellrowborder" valign="top" width="23.22%" headers="mcps1.2.5.1.1 "><p id="p7151111413315"><a name="p7151111413315"></a><a name="p7151111413315"></a>DCMI_HCCS_CMD_GET_STATISTIC_INFO</p>
</td>
<td class="cellrowborder" valign="top" width="29.69%" headers="mcps1.2.5.1.2 "><p id="p290151111549"><a name="p290151111549"></a><a name="p290151111549"></a>struct dcmi_hccs_statistic_info {</p>
<p id="p190118116546"><a name="p190118116546"></a><a name="p190118116546"></a>unsigned int tx_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p18901161120541"><a name="p18901161120541"></a><a name="p18901161120541"></a>unsigned int rx_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p119013111542"><a name="p119013111542"></a><a name="p119013111542"></a>unsigned int crc_err_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p119015111545"><a name="p119015111545"></a><a name="p119015111545"></a>unsigned int retry_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p2090141185415"><a name="p2090141185415"></a><a name="p2090141185415"></a>unsigned int reserved_field_cnt[DCMI_HCCS_RES_FIELD_NUM];</p>
<p id="p8901191145412"><a name="p8901191145412"></a><a name="p8901191145412"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="11.709999999999999%" headers="mcps1.2.5.1.3 "><p id="p20151121412314"><a name="p20151121412314"></a><a name="p20151121412314"></a>sizeof(struct dcmi_hccs_statistic_info)</p>
</td>
<td class="cellrowborder" valign="top" width="35.38%" headers="mcps1.2.5.1.4 "><p id="p1311994219353"><a name="p1311994219353"></a><a name="p1311994219353"></a>获取HCCS收发报文和误码统计信息。</p>
<a name="ul18736115213512"></a><a name="ul18736115213512"></a><ul id="ul18736115213512"><li>当前芯片仅使用x4模式，共获取8个HDLC链路的统计信息（每device）。</li><li>每个链路的统计信息包含3个u32整数，分别是发送报文、接收报文、接收报文crc错误的计数，单位是flit。</li><li>tx_cnt：表示累积发包数量，范围：0~4,294,967,295。</li><li>rx_cnt：表示累积收包数量，范围：0~4,294,967,295。</li><li>crc_err_cnt：表示误码数量，范围：0~4,294,967,295。</li><li>retry_cnt：表示数据包的重传次数，范围：0~4,294,967,295。<a name="ul1352101671119"></a><a name="ul1352101671119"></a><ul id="ul1352101671119"><li>0：表示无重传</li><li>其他：表示重传的次数</li></ul>
</li></ul>
</td>
</tr>
<tr id="row1172342663911"><td class="cellrowborder" valign="top" width="23.22%" headers="mcps1.2.5.1.1 "><p id="p18308185552312"><a name="p18308185552312"></a><a name="p18308185552312"></a>DCMI_HCCS_CMD_GET_STATISTIC_INFO_U64</p>
</td>
<td class="cellrowborder" valign="top" width="29.69%" headers="mcps1.2.5.1.2 "><p id="p88016523368"><a name="p88016523368"></a><a name="p88016523368"></a>struct dcmi_hccs_statistic_info_u64 {</p>
<p id="p08015525362"><a name="p08015525362"></a><a name="p08015525362"></a>unsigned long long tx_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p18016526363"><a name="p18016526363"></a><a name="p18016526363"></a>unsigned long long rx_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p580145218362"><a name="p580145218362"></a><a name="p580145218362"></a>unsigned long long crc_err_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p1780155243612"><a name="p1780155243612"></a><a name="p1780155243612"></a>unsigned long long retry_cnt[DCMI_HCCS_MAX_PCS_NUM];</p>
<p id="p3801052183611"><a name="p3801052183611"></a><a name="p3801052183611"></a>unsigned long long reserved[DCMI_HCCS_RES_FIELD_NUM];  // 预留64个字段供后续扩充使用</p>
<p id="p980125217364"><a name="p980125217364"></a><a name="p980125217364"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="11.709999999999999%" headers="mcps1.2.5.1.3 "><p id="p10308155132311"><a name="p10308155132311"></a><a name="p10308155132311"></a>sizeof(struct dcmi_hccs_statistic_info_u64)</p>
</td>
<td class="cellrowborder" valign="top" width="35.38%" headers="mcps1.2.5.1.4 "><p id="p72181373246"><a name="p72181373246"></a><a name="p72181373246"></a>获取HCCS收发报文和误码统计信息。</p>
<a name="ul15381435182416"></a><a name="ul15381435182416"></a><ul id="ul15381435182416"><li>当前芯片仅使用x4模式，共获取8个HDLC链路的统计信息（每device）。</li><li>每个链路的统计信息包含3个u64整数，分别是发送报文、接收报文、接收报文crc错误的计数，单位是flit。</li><li>tx_cnt：表示累积发包数量，范围：0~18,446,744,073,709,551,615。</li><li>rx_cnt：表示累积收包数量，范围：0~18,446,744,073,709,551,615。</li><li>crc_err_cnt：表示误码数量，范围：0~18,446,744,073,709,551,615。</li><li>retry_cnt：表示数据包的重传次数，范围：0~18,446,744,073,709,551,615。<a name="ul18352890486"></a><a name="ul18352890486"></a><ul id="ul18352890486"><li>0：表示无重传。</li><li>其他：表示重传的次数。</li></ul>
</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section1782016178717"></a>**

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
struct dcmi_hccs_statues status = {0};
unsigned int buf_size = sizeof(struct dcmi_hccs_statues);
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_HCCS,
DCMI_HCCS_CMD_GET_STATUS, &status, &buf_size);
if (ret != 0) {
//todo
return ret;
} else {
// todo
return ret;
}
…
```


## DCMI\_MAIN\_CMD\_EX\_COMPUTING命令说明<a name="ZH-CN_TOPIC_0000002485478750"></a>

**函数原型<a name="section251144503117"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section4511154520314"></a>**

获取算力token值。

**参数说明<a name="section195111645193117"></a>**

<a name="table145283455316"></a>
<table><thead align="left"><tr id="row963614518316"><th class="cellrowborder" valign="top" width="17.17171717171717%" id="mcps1.1.5.1.1"><p id="p96361345163111"><a name="p96361345163111"></a><a name="p96361345163111"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.151515151515152%" id="mcps1.1.5.1.2"><p id="p1963644563115"><a name="p1963644563115"></a><a name="p1963644563115"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.19191919191919%" id="mcps1.1.5.1.3"><p id="p46361745193112"><a name="p46361745193112"></a><a name="p46361745193112"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.484848484848484%" id="mcps1.1.5.1.4"><p id="p15636144519316"><a name="p15636144519316"></a><a name="p15636144519316"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1663613450314"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p16367453317"><a name="p16367453317"></a><a name="p16367453317"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p1863654517314"><a name="p1863654517314"></a><a name="p1863654517314"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p17636345203114"><a name="p17636345203114"></a><a name="p17636345203114"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p14636945193116"><a name="p14636945193116"></a><a name="p14636945193116"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row163617450316"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p126362454318"><a name="p126362454318"></a><a name="p126362454318"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p6636174518315"><a name="p6636174518315"></a><a name="p6636174518315"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p4636194553115"><a name="p4636194553115"></a><a name="p4636194553115"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p1563674543113"><a name="p1563674543113"></a><a name="p1563674543113"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p36365456315"><a name="p36365456315"></a><a name="p36365456315"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p56782056389"><a name="p56782056389"></a><a name="p56782056389"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row166363454319"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p16636145193116"><a name="p16636145193116"></a><a name="p16636145193116"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p6637545163119"><a name="p6637545163119"></a><a name="p6637545163119"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p20637104516316"><a name="p20637104516316"></a><a name="p20637104516316"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p16371245203115"><a name="p16371245203115"></a><a name="p16371245203115"></a>DCMI_MAIN_CMD_EX_COMPUTING</p>
</td>
</tr>
<tr id="row663754533112"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p106371145153114"><a name="p106371145153114"></a><a name="p106371145153114"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p5637144510311"><a name="p5637144510311"></a><a name="p5637144510311"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p106374457316"><a name="p106374457316"></a><a name="p106374457316"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p11637204512315"><a name="p11637204512315"></a><a name="p11637204512315"></a>DCMI_EX_COMPUTING_SUB_CMD_TOKEN</p>
</td>
</tr>
<tr id="row20637114553111"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p1163734513119"><a name="p1163734513119"></a><a name="p1163734513119"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p17637545203112"><a name="p17637545203112"></a><a name="p17637545203112"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p1163719452314"><a name="p1163719452314"></a><a name="p1163719452314"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p106372452314"><a name="p106372452314"></a><a name="p106372452314"></a>详见本节约束说明。</p>
</td>
</tr>
<tr id="row146374453314"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p10637184563116"><a name="p10637184563116"></a><a name="p10637184563116"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p126373451313"><a name="p126373451313"></a><a name="p126373451313"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p13637445163120"><a name="p13637445163120"></a><a name="p13637445163120"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p3637184518311"><a name="p3637184518311"></a><a name="p3637184518311"></a>返回结果数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section1551994503119"></a>**

<a name="table6533174517314"></a>
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

**异常处理<a name="section15209424870"></a>**

无。

**约束说明<a name="section1535162716712"></a>**

**表 1**  sub\_cmd对应的buf格式

<a name="table353514454317"></a>
<table><thead align="left"><tr id="row263834543117"><th class="cellrowborder" valign="top" width="25.380000000000003%" id="mcps1.2.5.1.1"><p id="p166382458312"><a name="p166382458312"></a><a name="p166382458312"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="28.88%" id="mcps1.2.5.1.2"><p id="p16638645113120"><a name="p16638645113120"></a><a name="p16638645113120"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="18.69%" id="mcps1.2.5.1.3"><p id="p263819453310"><a name="p263819453310"></a><a name="p263819453310"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="27.05%" id="mcps1.2.5.1.4"><p id="p1638145183117"><a name="p1638145183117"></a><a name="p1638145183117"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row1263884503118"><td class="cellrowborder" valign="top" width="25.380000000000003%" headers="mcps1.2.5.1.1 "><p id="p763884573120"><a name="p763884573120"></a><a name="p763884573120"></a>DCMI_EX_COMPUTING</p>
<p id="p3638184563117"><a name="p3638184563117"></a><a name="p3638184563117"></a>_SUB_CMD_TOKEN</p>
</td>
<td class="cellrowborder" valign="top" width="28.88%" headers="mcps1.2.5.1.2 "><p id="p1963817451315"><a name="p1963817451315"></a><a name="p1963817451315"></a>struct dcmi_computing_token_stru {</p>
<p id="p1863844515316"><a name="p1863844515316"></a><a name="p1863844515316"></a>float value;</p>
<p id="p126384453317"><a name="p126384453317"></a><a name="p126384453317"></a>u8 type;</p>
<p id="p1563804518310"><a name="p1563804518310"></a><a name="p1563804518310"></a>u8 reserve_c;</p>
<p id="p763874510313"><a name="p763874510313"></a><a name="p763874510313"></a>u16 reserve_s;</p>
<p id="p56381459316"><a name="p56381459316"></a><a name="p56381459316"></a>}</p>
<p id="p6638164513117"><a name="p6638164513117"></a><a name="p6638164513117"></a>a. value表示算力值。</p>
<p id="p863819458316"><a name="p863819458316"></a><a name="p863819458316"></a>b. type表示license BOM编码转换后的类型值。</p>
<p id="p20638345123117"><a name="p20638345123117"></a><a name="p20638345123117"></a>c. reserve_c表示保留字，当前预留，不使用。</p>
<p id="p1663810451310"><a name="p1663810451310"></a><a name="p1663810451310"></a>d. reserve_s表示保留字，当前预留，不使用。</p>
</td>
<td class="cellrowborder" valign="top" width="18.69%" headers="mcps1.2.5.1.3 "><p id="p663854523113"><a name="p663854523113"></a><a name="p663854523113"></a>sizeof(dcmi_</p>
<p id="p46381445153118"><a name="p46381445153118"></a><a name="p46381445153118"></a>computing_token_stru)</p>
</td>
<td class="cellrowborder" valign="top" width="27.05%" headers="mcps1.2.5.1.4 "><p id="p1638144514312"><a name="p1638144514312"></a><a name="p1638144514312"></a>获取算力token值</p>
<p id="p163864515312"><a name="p163864515312"></a><a name="p163864515312"></a>value值的正确范围：[0,65535]</p>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section172981043978"></a>**

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
struct dcmi_computing_token_stru dcmi_computing_token = {0};
unsigned int buf_size = sizeof(struct dcmi_computing_token_stru);
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_EX_COMPUTING,
DCMI_EX_COMPUTING_SUB_CMD_TOKEN, &dcmi_computing_token, &buf_size);
if (ret != 0) {
//todo
return ret;
} else {
// todo
return ret;
}
…
```


## DCMI\_MAIN\_CMD\_VDEV\_MNG命令说明<a name="ZH-CN_TOPIC_0000002517638717"></a>

**函数原型<a name="section1189012593314"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section689185915314"></a>**

虚拟设备管理相关功能，包括虚拟设备资源信息获取，SOC设备总资源信息获取，SOC设备未使用资源信息获取等。

**参数说明<a name="section4891659133117"></a>**

<a name="table69831559113114"></a>
<table><thead align="left"><tr id="row5221130183210"><th class="cellrowborder" valign="top" width="17.17171717171717%" id="mcps1.1.5.1.1"><p id="p1122220113216"><a name="p1122220113216"></a><a name="p1122220113216"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.151515151515152%" id="mcps1.1.5.1.2"><p id="p142220033219"><a name="p142220033219"></a><a name="p142220033219"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.19191919191919%" id="mcps1.1.5.1.3"><p id="p122222083218"><a name="p122222083218"></a><a name="p122222083218"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.484848484848484%" id="mcps1.1.5.1.4"><p id="p1122217019327"><a name="p1122217019327"></a><a name="p1122217019327"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5222507323"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p15222203328"><a name="p15222203328"></a><a name="p15222203328"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p112226020327"><a name="p112226020327"></a><a name="p112226020327"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p822218012323"><a name="p822218012323"></a><a name="p822218012323"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p15222107328"><a name="p15222107328"></a><a name="p15222107328"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row3222180143219"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p92225043213"><a name="p92225043213"></a><a name="p92225043213"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p62221307320"><a name="p62221307320"></a><a name="p62221307320"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p72229016329"><a name="p72229016329"></a><a name="p72229016329"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p162221608326"><a name="p162221608326"></a><a name="p162221608326"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p112221406324"><a name="p112221406324"></a><a name="p112221406324"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1971072397"><a name="p1971072397"></a><a name="p1971072397"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row12223019320"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p42221019320"><a name="p42221019320"></a><a name="p42221019320"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p52224019323"><a name="p52224019323"></a><a name="p52224019323"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p122228015326"><a name="p122228015326"></a><a name="p122228015326"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p1622210113220"><a name="p1622210113220"></a><a name="p1622210113220"></a>DCMI_MAIN_CMD_VDEV_MNG</p>
</td>
</tr>
<tr id="row8222300322"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p1222160143216"><a name="p1222160143216"></a><a name="p1222160143216"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p122222014322"><a name="p122222014322"></a><a name="p122222014322"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p14222140103211"><a name="p14222140103211"></a><a name="p14222140103211"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p72227073218"><a name="p72227073218"></a><a name="p72227073218"></a>typedef enum {</p>
<p id="p1222407325"><a name="p1222407325"></a><a name="p1222407325"></a>DCMI_VMNG_SUB_CMD_GET_VDEV_RESOURCE,</p>
<p id="p52221309326"><a name="p52221309326"></a><a name="p52221309326"></a>DCMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE,</p>
<p id="p62221905329"><a name="p62221905329"></a><a name="p62221905329"></a>DCMI_VMNG_SUB_CMD_GET_FREE_RESOURCE,</p>
<p id="p1422215018326"><a name="p1422215018326"></a><a name="p1422215018326"></a>DCMI_VMNG_SUB_CMD_SET_SRIOV_SWITCH,</p>
<p id="p722210163215"><a name="p722210163215"></a><a name="p722210163215"></a>DCMI_VMNG_SUB_CMD_MAX,</p>
<p id="p522260133219"><a name="p522260133219"></a><a name="p522260133219"></a>} DCMI_VDEV_MNG_SUB_CMD;</p>
<p id="p192221307329"><a name="p192221307329"></a><a name="p192221307329"></a>DCMI_VMNG_SUB_CMD_GET_VDEV_RESOURCE表示获取单个虚拟设备的资源信息。</p>
<p id="p172224010327"><a name="p172224010327"></a><a name="p172224010327"></a>DCMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE表示获取指定SOC设备的总资源信息，用于在创建新虚拟设备时参考使用。</p>
<p id="p2022214023217"><a name="p2022214023217"></a><a name="p2022214023217"></a>DCMI_VMNG_SUB_CMD_GET_FREE_RESOURCE表示获取指定SOC设备的未使用资源信息，用于在创建新虚拟设备时参考使用。</p>
<p id="p10223110173213"><a name="p10223110173213"></a><a name="p10223110173213"></a>说明：</p>
<p id="p122315003214"><a name="p122315003214"></a><a name="p122315003214"></a>当前接口不支持DCMI_VMNG_SUB_CMD_SET_SRIOV_SWITCH命令。</p>
</td>
</tr>
<tr id="row182233017321"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p192231204324"><a name="p192231204324"></a><a name="p192231204324"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p152237093214"><a name="p152237093214"></a><a name="p152237093214"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p52231093215"><a name="p52231093215"></a><a name="p52231093215"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p1722380113216"><a name="p1722380113216"></a><a name="p1722380113216"></a>具体buf格式跟sub_cmd对应，格式见<a href="#table1499155916318">表1 sub_cmd对应的buf格式</a></p>
</td>
</tr>
<tr id="row1822390123215"><td class="cellrowborder" valign="top" width="17.17171717171717%" headers="mcps1.1.5.1.1 "><p id="p322311015323"><a name="p322311015323"></a><a name="p322311015323"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.151515151515152%" headers="mcps1.1.5.1.2 "><p id="p02233013326"><a name="p02233013326"></a><a name="p02233013326"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.19191919191919%" headers="mcps1.1.5.1.3 "><p id="p2223809324"><a name="p2223809324"></a><a name="p2223809324"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.484848484848484%" headers="mcps1.1.5.1.4 "><p id="p4223100103216"><a name="p4223100103216"></a><a name="p4223100103216"></a>返回结果对应<a href="#table1499155916318">表1 sub_cmd对应的buf格式</a>中的buf格式结构大小。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section189807595317"></a>**

<a name="table154712012328"></a>
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

**异常处理<a name="section88531421389"></a>**

查询基础和资源信息时，如果对应的产品形态不支持，返回的值参考[表2 基础和资源信息结构描述](#table7242615182313)中的“选项说明”。

**约束说明<a name="section194071571789"></a>**

-   调用该接口时sub\_cmd必须和[表1 sub\_cmd对应的buf格式](#table1499155916318)对应，buf\_size为对应buf数据结构大小。
-   使用该接口获取资源需配置环境为SRIOV模式。

**表 1**  sub\_cmd对应的buf格式

<a name="table1499155916318"></a>
<table><thead align="left"><tr id="row922320103219"><th class="cellrowborder" valign="top" width="30.080000000000002%" id="mcps1.2.4.1.1"><p id="p1722319017323"><a name="p1722319017323"></a><a name="p1722319017323"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="43.46%" id="mcps1.2.4.1.2"><p id="p02235018328"><a name="p02235018328"></a><a name="p02235018328"></a>buf格式</p>
</th>
<th class="cellrowborder" valign="top" width="26.46%" id="mcps1.2.4.1.3"><p id="p1883611521712"><a name="p1883611521712"></a><a name="p1883611521712"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row13223806324"><td class="cellrowborder" valign="top" width="30.080000000000002%" headers="mcps1.2.4.1.1 "><p id="p112234015324"><a name="p112234015324"></a><a name="p112234015324"></a>DCMI_VMNG_SUB_CMD_GET_VDEV_RESOURCE</p>
</td>
<td class="cellrowborder" valign="top" width="43.46%" headers="mcps1.2.4.1.2 "><p id="p14223100143214"><a name="p14223100143214"></a><a name="p14223100143214"></a>struct DCMI_vdev_query_stru {</p>
<p id="p172234013218"><a name="p172234013218"></a><a name="p172234013218"></a>unsigned int vdev_id;</p>
<p id="p22234043220"><a name="p22234043220"></a><a name="p22234043220"></a>struct DCMI_vdev_query_info query_info;</p>
<p id="p1822313043218"><a name="p1822313043218"></a><a name="p1822313043218"></a>}</p>
<p id="p52239019325"><a name="p52239019325"></a><a name="p52239019325"></a>其中，vdev_id表示虚拟设备号，作为输入信息；</p>
<p id="p1622316033219"><a name="p1622316033219"></a><a name="p1622316033219"></a>query_info表示查询到的虚拟设备信息，其具体结构如下：</p>
<p id="p1722340183213"><a name="p1722340183213"></a><a name="p1722340183213"></a>#define DCMI_VDEV_RES_NAME_LEN 16</p>
<p id="p322380103210"><a name="p322380103210"></a><a name="p322380103210"></a>struct DCMI_vdev_query_info {</p>
<p id="p172237014328"><a name="p172237014328"></a><a name="p172237014328"></a>char name[DCMI_VDEV_RES_NAME_LEN];</p>
<p id="p6223110183211"><a name="p6223110183211"></a><a name="p6223110183211"></a>unsigned int status;</p>
<p id="p62231809326"><a name="p62231809326"></a><a name="p62231809326"></a>unsigned int is_container_used;</p>
<p id="p32236018325"><a name="p32236018325"></a><a name="p32236018325"></a>unsigned int vfid;</p>
<p id="p92238053213"><a name="p92238053213"></a><a name="p92238053213"></a>unsigned int reserved;</p>
<p id="p1322370113218"><a name="p1322370113218"></a><a name="p1322370113218"></a>unsigned long long container_id;</p>
<p id="p172231004322"><a name="p172231004322"></a><a name="p172231004322"></a>struct DCMI_base_resource base;</p>
<p id="p1922316011328"><a name="p1922316011328"></a><a name="p1922316011328"></a>struct DCMI_computing_resource computing;</p>
<p id="p172231502321"><a name="p172231502321"></a><a name="p172231502321"></a>struct DCMI_media_resource media;</p>
<p id="p322313010325"><a name="p322313010325"></a><a name="p322313010325"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="26.46%" headers="mcps1.2.4.1.3 "><a name="ul1512412249211"></a><a name="ul1512412249211"></a><ul id="ul1512412249211"><li>name表示创建虚拟设备时指定的名称；</li><li>status表示虚拟设备的当前状态，当前不支持。</li><li>is_container_used表示当前容器是否已经开始使用；</li><li>vfid表示虚拟设备使用的VF的序号；</li><li>reserved表示预留字段，当前未使用；</li><li>container_id表示容器ID;</li><li>base表示虚拟设备的基础信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>；</li><li>computing表示虚拟设备的算力资源信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>；</li><li>media表示虚拟设备的媒体资源信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>。</li></ul>
</td>
</tr>
<tr id="row52243093216"><td class="cellrowborder" valign="top" width="30.080000000000002%" headers="mcps1.2.4.1.1 "><p id="p1422412015326"><a name="p1422412015326"></a><a name="p1422412015326"></a>DCMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE</p>
</td>
<td class="cellrowborder" valign="top" width="43.46%" headers="mcps1.2.4.1.2 "><p id="p12241005325"><a name="p12241005325"></a><a name="p12241005325"></a>#define DCMI_SOC_SPLIT_MAX 32</p>
<p id="p18224120193213"><a name="p18224120193213"></a><a name="p18224120193213"></a>struct DCMI_soc_total_resource {</p>
<p id="p1622420163211"><a name="p1622420163211"></a><a name="p1622420163211"></a>unsigned int vdev_num;</p>
<p id="p022415023211"><a name="p022415023211"></a><a name="p022415023211"></a>unsigned int vdev_id[DCMI_SOC_SPLIT_MAX];</p>
<p id="p62241015329"><a name="p62241015329"></a><a name="p62241015329"></a>unsigned int vfg_num;</p>
<p id="p162241007321"><a name="p162241007321"></a><a name="p162241007321"></a>unsigned int vfg_bitmap;</p>
<p id="p13224170163215"><a name="p13224170163215"></a><a name="p13224170163215"></a>struct DCMI_base_resource base;</p>
<p id="p192246033213"><a name="p192246033213"></a><a name="p192246033213"></a>struct DCMI_computing_resource computing;</p>
<p id="p182246093217"><a name="p182246093217"></a><a name="p182246093217"></a>struct DCMI_media_resource media;</p>
<p id="p1522415023214"><a name="p1522415023214"></a><a name="p1522415023214"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="26.46%" headers="mcps1.2.4.1.3 "><a name="ul20252101215414"></a><a name="ul20252101215414"></a><ul id="ul20252101215414"><li>vdev_num表示指定SOC设备中虚拟设备的数量；</li><li>vdev_id表示每个虚拟设备对应的虚拟设备ID;</li><li>vfg_num表示指定SOC设备中总VFG的数量，当前不支持；</li><li>vfg_bitmap表示设备的VFG位图表示，每个bit对应1个VFG，值为1表示有效，当前不支持；</li><li>base表示指定SOC设备的总基础信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>；</li><li>computing表示指定SOC设备总计算资源信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>；</li><li>media表示指定SOC设备的总媒体资源信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>。</li></ul>
</td>
</tr>
<tr id="row62242063213"><td class="cellrowborder" valign="top" width="30.080000000000002%" headers="mcps1.2.4.1.1 "><p id="p02241202328"><a name="p02241202328"></a><a name="p02241202328"></a>DCMI_VMNG_SUB_CMD_GET_FREE_RESOURCE</p>
</td>
<td class="cellrowborder" valign="top" width="43.46%" headers="mcps1.2.4.1.2 "><p id="p3224200143215"><a name="p3224200143215"></a><a name="p3224200143215"></a>struct DCMI_soc_free_resource {</p>
<p id="p5224130133213"><a name="p5224130133213"></a><a name="p5224130133213"></a>unsigned int vfg_num;</p>
<p id="p62249010329"><a name="p62249010329"></a><a name="p62249010329"></a>unsigned int vfg_bitmap;</p>
<p id="p1922417023219"><a name="p1922417023219"></a><a name="p1922417023219"></a>struct DCMI_base_resource base;</p>
<p id="p7224106328"><a name="p7224106328"></a><a name="p7224106328"></a>struct DCMI_computing_resource computing;</p>
<p id="p922413012328"><a name="p922413012328"></a><a name="p922413012328"></a>struct DCMI_media_resource media;</p>
<p id="p122411063216"><a name="p122411063216"></a><a name="p122411063216"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="26.46%" headers="mcps1.2.4.1.3 "><a name="ul14775720049"></a><a name="ul14775720049"></a><ul id="ul14775720049"><li>vfg_num表示指定SOC设备中未使用的VFG的数量，当前不支持；</li><li>vfg_bitmap表示设备的VFG位图表示，每个bit对应1个VFG，值为1表示未使用，当前不支持；</li><li>base表示指定SOC设备的未使用基础信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>；</li><li>computing表示指定SOC设备未使用计算资源信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>；</li><li>media表示指定SOC设备的剩余媒体资源信息，具体见<a href="#table7242615182313">表2 基础和资源信息结构描述</a>。</li></ul>
</td>
</tr>
<tr id="row1622520123217"><td class="cellrowborder" valign="top" width="30.080000000000002%" headers="mcps1.2.4.1.1 "><p id="p122590133210"><a name="p122590133210"></a><a name="p122590133210"></a>DCMI_VMNG_SUB_CMD_GET_TOPS_PERCENTAGE</p>
</td>
<td class="cellrowborder" valign="top" width="43.46%" headers="mcps1.2.4.1.2 "><p id="p112254063216"><a name="p112254063216"></a><a name="p112254063216"></a>/* for compute group ratio */</p>
<p id="p142251602327"><a name="p142251602327"></a><a name="p142251602327"></a>struct DCMI_soc_vdev_ratio {</p>
<p id="p82256012324"><a name="p82256012324"></a><a name="p82256012324"></a>unsigned int vdev_id;</p>
<p id="p922510123214"><a name="p922510123214"></a><a name="p922510123214"></a>unsigned int ratio;</p>
<p id="p142253033219"><a name="p142253033219"></a><a name="p142253033219"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="26.46%" headers="mcps1.2.4.1.3 "><a name="ul17751192613416"></a><a name="ul17751192613416"></a><ul id="ul17751192613416"><li>vdev_id表示虚拟设备ID；</li><li>ratio表示算力资源占比。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2**  基础和资源信息结构描述

<a name="table7242615182313"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002517615349_row14242141517236"><th class="cellrowborder" valign="top" width="9.29%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002517615349_p324216159237"><a name="zh-cn_topic_0000002517615349_p324216159237"></a><a name="zh-cn_topic_0000002517615349_p324216159237"></a>信息类型</p>
</th>
<th class="cellrowborder" valign="top" width="37.92%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002517615349_p32432015122316"><a name="zh-cn_topic_0000002517615349_p32432015122316"></a><a name="zh-cn_topic_0000002517615349_p32432015122316"></a>结构体表示</p>
</th>
<th class="cellrowborder" valign="top" width="52.790000000000006%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002517615349_p12437156239"><a name="zh-cn_topic_0000002517615349_p12437156239"></a><a name="zh-cn_topic_0000002517615349_p12437156239"></a>选项及说明</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002517615349_row4243415102316"><td class="cellrowborder" valign="top" width="9.29%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002517615349_p17243151552320"><a name="zh-cn_topic_0000002517615349_p17243151552320"></a><a name="zh-cn_topic_0000002517615349_p17243151552320"></a>基础信息</p>
</td>
<td class="cellrowborder" valign="top" width="37.92%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002517615349_p132431915132314"><a name="zh-cn_topic_0000002517615349_p132431915132314"></a><a name="zh-cn_topic_0000002517615349_p132431915132314"></a>#define DCMI_VDEV_FOR_RESERVE 32</p>
<p id="zh-cn_topic_0000002517615349_p7243815122310"><a name="zh-cn_topic_0000002517615349_p7243815122310"></a><a name="zh-cn_topic_0000002517615349_p7243815122310"></a>struct DCMI_base_resource {</p>
<p id="zh-cn_topic_0000002517615349_p16243141542320"><a name="zh-cn_topic_0000002517615349_p16243141542320"></a><a name="zh-cn_topic_0000002517615349_p16243141542320"></a>unsigned long long token;</p>
<p id="zh-cn_topic_0000002517615349_p132431615112310"><a name="zh-cn_topic_0000002517615349_p132431615112310"></a><a name="zh-cn_topic_0000002517615349_p132431615112310"></a>unsigned long long token_max;</p>
<p id="zh-cn_topic_0000002517615349_p16243151516231"><a name="zh-cn_topic_0000002517615349_p16243151516231"></a><a name="zh-cn_topic_0000002517615349_p16243151516231"></a>unsigned long long task_timeout;</p>
<p id="zh-cn_topic_0000002517615349_p72431215102311"><a name="zh-cn_topic_0000002517615349_p72431215102311"></a><a name="zh-cn_topic_0000002517615349_p72431215102311"></a>unsigned int vfg_id;</p>
<p id="zh-cn_topic_0000002517615349_p1024310154236"><a name="zh-cn_topic_0000002517615349_p1024310154236"></a><a name="zh-cn_topic_0000002517615349_p1024310154236"></a>unsigned char vip_mode;</p>
<p id="zh-cn_topic_0000002517615349_p4243191514232"><a name="zh-cn_topic_0000002517615349_p4243191514232"></a><a name="zh-cn_topic_0000002517615349_p4243191514232"></a>unsigned char reserved[DCMI_VDEV_FOR_RESERVE - 1];</p>
<p id="zh-cn_topic_0000002517615349_p42434155236"><a name="zh-cn_topic_0000002517615349_p42434155236"></a><a name="zh-cn_topic_0000002517615349_p42434155236"></a>/* bytes aligned */</p>
<p id="zh-cn_topic_0000002517615349_p324371542317"><a name="zh-cn_topic_0000002517615349_p324371542317"></a><a name="zh-cn_topic_0000002517615349_p324371542317"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="52.790000000000006%" headers="mcps1.2.4.1.3 "><a name="zh-cn_topic_0000002517615349_ul286153182410"></a><a name="zh-cn_topic_0000002517615349_ul286153182410"></a><ul id="zh-cn_topic_0000002517615349_ul286153182410"><li>token：VFG分时使用时虚拟设备占用周期数。不支持时默认返回0xFFFFFFFFFFFFFFFF。支持形态：无。</li><li>token_max：VFG分时使用时虚拟设备占用最大周期数。不支持时默认返回0xFFFFFFFFFFFFFFFF。支持形态：无。</li><li>task_timeout：VFG任务超时周期数。不支持时默认返回0xFFFFFFFFFFFFFFFF。支持形态：无。</li><li>vfg_id：虚拟设备使用的VFG ID。正常值：-1， 0-5。不支持时默认返回0xFFFFFFFF。支持形态：所有产品。</li><li>vip_mode：虚拟设备使用的VFG的模式。取值如下：<a name="zh-cn_topic_0000002517615349_ul131121530102714"></a><a name="zh-cn_topic_0000002517615349_ul131121530102714"></a><ul id="zh-cn_topic_0000002517615349_ul131121530102714"><li>0：虚拟设备之间共享VFG，可能存在共用资源的争抢，影响虚拟设备切换时的调度时延。</li><li>1：虚拟设备独占VFG，可以使用VFG的所有资源。不支持其他虚拟设备共享该VFG。</li></ul>
<p id="zh-cn_topic_0000002517615349_p1092194382815"><a name="zh-cn_topic_0000002517615349_p1092194382815"></a><a name="zh-cn_topic_0000002517615349_p1092194382815"></a>不支持时默认返回0xFF。支持形态：所有产品。</p>
</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002517615349_row4243151532312"><td class="cellrowborder" valign="top" width="9.29%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002517615349_p16243111572319"><a name="zh-cn_topic_0000002517615349_p16243111572319"></a><a name="zh-cn_topic_0000002517615349_p16243111572319"></a>算力资源信息</p>
</td>
<td class="cellrowborder" valign="top" width="37.92%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002517615349_p19243131512231"><a name="zh-cn_topic_0000002517615349_p19243131512231"></a><a name="zh-cn_topic_0000002517615349_p19243131512231"></a>struct DCMI_computing_resource {</p>
<p id="zh-cn_topic_0000002517615349_p1724314158239"><a name="zh-cn_topic_0000002517615349_p1724314158239"></a><a name="zh-cn_topic_0000002517615349_p1724314158239"></a>/* accelator resource */</p>
<p id="zh-cn_topic_0000002517615349_p52431715162312"><a name="zh-cn_topic_0000002517615349_p52431715162312"></a><a name="zh-cn_topic_0000002517615349_p52431715162312"></a>float aic;</p>
<p id="zh-cn_topic_0000002517615349_p17243181582316"><a name="zh-cn_topic_0000002517615349_p17243181582316"></a><a name="zh-cn_topic_0000002517615349_p17243181582316"></a>float aiv;</p>
<p id="zh-cn_topic_0000002517615349_p42431215122312"><a name="zh-cn_topic_0000002517615349_p42431215122312"></a><a name="zh-cn_topic_0000002517615349_p42431215122312"></a>unsigned short dsa;</p>
<p id="zh-cn_topic_0000002517615349_p8243131520238"><a name="zh-cn_topic_0000002517615349_p8243131520238"></a><a name="zh-cn_topic_0000002517615349_p8243131520238"></a>unsigned short rtsq;</p>
<p id="zh-cn_topic_0000002517615349_p42431315142318"><a name="zh-cn_topic_0000002517615349_p42431315142318"></a><a name="zh-cn_topic_0000002517615349_p42431315142318"></a>unsigned short acsq;</p>
<p id="zh-cn_topic_0000002517615349_p172431515112320"><a name="zh-cn_topic_0000002517615349_p172431515112320"></a><a name="zh-cn_topic_0000002517615349_p172431515112320"></a>unsigned short cdqm;</p>
<p id="zh-cn_topic_0000002517615349_p142430159234"><a name="zh-cn_topic_0000002517615349_p142430159234"></a><a name="zh-cn_topic_0000002517615349_p142430159234"></a>unsigned short c_core;</p>
<p id="zh-cn_topic_0000002517615349_p1524381552316"><a name="zh-cn_topic_0000002517615349_p1524381552316"></a><a name="zh-cn_topic_0000002517615349_p1524381552316"></a>unsigned short ffts;</p>
<p id="zh-cn_topic_0000002517615349_p32436154235"><a name="zh-cn_topic_0000002517615349_p32436154235"></a><a name="zh-cn_topic_0000002517615349_p32436154235"></a>unsigned short sdma;</p>
<p id="zh-cn_topic_0000002517615349_p4243181511231"><a name="zh-cn_topic_0000002517615349_p4243181511231"></a><a name="zh-cn_topic_0000002517615349_p4243181511231"></a>unsigned short pcie_dma;</p>
<p id="zh-cn_topic_0000002517615349_p22431815192313"><a name="zh-cn_topic_0000002517615349_p22431815192313"></a><a name="zh-cn_topic_0000002517615349_p22431815192313"></a>/* memory resource, MB as unit */</p>
<p id="zh-cn_topic_0000002517615349_p12243121552314"><a name="zh-cn_topic_0000002517615349_p12243121552314"></a><a name="zh-cn_topic_0000002517615349_p12243121552314"></a>unsigned long long memory_size;</p>
<p id="zh-cn_topic_0000002517615349_p1924321516233"><a name="zh-cn_topic_0000002517615349_p1924321516233"></a><a name="zh-cn_topic_0000002517615349_p1924321516233"></a>/* id resource */</p>
<p id="zh-cn_topic_0000002517615349_p14243115142310"><a name="zh-cn_topic_0000002517615349_p14243115142310"></a><a name="zh-cn_topic_0000002517615349_p14243115142310"></a>unsigned int event_id;</p>
<p id="zh-cn_topic_0000002517615349_p924331572316"><a name="zh-cn_topic_0000002517615349_p924331572316"></a><a name="zh-cn_topic_0000002517615349_p924331572316"></a>unsigned int notify_id;</p>
<p id="zh-cn_topic_0000002517615349_p1224331562311"><a name="zh-cn_topic_0000002517615349_p1224331562311"></a><a name="zh-cn_topic_0000002517615349_p1224331562311"></a>unsigned int stream_id;</p>
<p id="zh-cn_topic_0000002517615349_p924361519232"><a name="zh-cn_topic_0000002517615349_p924361519232"></a><a name="zh-cn_topic_0000002517615349_p924361519232"></a>unsigned int model_id;</p>
<p id="zh-cn_topic_0000002517615349_p20243715132314"><a name="zh-cn_topic_0000002517615349_p20243715132314"></a><a name="zh-cn_topic_0000002517615349_p20243715132314"></a>/* cpu resource */</p>
<p id="zh-cn_topic_0000002517615349_p1924381582319"><a name="zh-cn_topic_0000002517615349_p1924381582319"></a><a name="zh-cn_topic_0000002517615349_p1924381582319"></a>unsigned short topic_schedule_aicpu;</p>
<p id="zh-cn_topic_0000002517615349_p1243111510233"><a name="zh-cn_topic_0000002517615349_p1243111510233"></a><a name="zh-cn_topic_0000002517615349_p1243111510233"></a>unsigned short host_ctrl_cpu;</p>
<p id="zh-cn_topic_0000002517615349_p324312154239"><a name="zh-cn_topic_0000002517615349_p324312154239"></a><a name="zh-cn_topic_0000002517615349_p324312154239"></a>unsigned short host_aicpu;</p>
<p id="zh-cn_topic_0000002517615349_p92431915152317"><a name="zh-cn_topic_0000002517615349_p92431915152317"></a><a name="zh-cn_topic_0000002517615349_p92431915152317"></a>unsigned short device_aicpu;</p>
<p id="zh-cn_topic_0000002517615349_p11243111542312"><a name="zh-cn_topic_0000002517615349_p11243111542312"></a><a name="zh-cn_topic_0000002517615349_p11243111542312"></a>unsigned short topic_ctrl_cpu_slot;</p>
<p id="zh-cn_topic_0000002517615349_p102431715102314"><a name="zh-cn_topic_0000002517615349_p102431715102314"></a><a name="zh-cn_topic_0000002517615349_p102431715102314"></a>unsigned char reserved[DCMI_VDEV_FOR_RESERVE];</p>
<p id="zh-cn_topic_0000002517615349_p1724391502313"><a name="zh-cn_topic_0000002517615349_p1724391502313"></a><a name="zh-cn_topic_0000002517615349_p1724391502313"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="52.790000000000006%" headers="mcps1.2.4.1.3 "><a name="zh-cn_topic_0000002517615349_ul276844773015"></a><a name="zh-cn_topic_0000002517615349_ul276844773015"></a><ul id="zh-cn_topic_0000002517615349_ul276844773015"><li>aic：aicore数量。不支持时默认返回-1.0。支持形态：无。</li><li>aiv：ai_vector数量。不支持时默认返回-1.0。支持形态：无。</li><li>dsa：dsa数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>rtsq：rtsq数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>acsq：acsq数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>cdqm：cdqm数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>c_core：c_core数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>ffts：ffts数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>sdma：sdma通道数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>pcie_dma：pcie dma通道数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>memory_size：内存大小，MB为单位。不支持时默认返回0xFFFFFFFFFFFFFFFF。支持形态：所有产品。</li><li>event_id：event ID数量。不支持时默认返回0xFFFFFFFF。支持形态：无。</li><li>notify_id：notify ID数量。不支持时默认返回0xFFFFFFFF。支持形态：无。</li><li>stream_id：stream ID数量。不支持时默认返回0xFFFFFFFF。支持形态：无。</li><li>model_id：model ID数量。不支持时默认返回0xFFFFFFFF。支持形态：无。</li><li>topic_schedule_aicpu：topic schedule aicpu数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>host_ctrl_cpu：host ctrl cpu数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>host_ai_cpu：host aicpu数量。不支持时默认返回0xFFFF。支持形态：无。</li><li>device_aicpu：device aicpu数量。不支持时默认返回0xFFFF。支持形态：所有产品。</li><li>topic_ctrl_cpu_slot：topic ctrl cpu slot数量。不支持时默认返回0xFFFF。支持形态：无。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002517615349_row72451415112318"><td class="cellrowborder" valign="top" width="9.29%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002517615349_p1524581515235"><a name="zh-cn_topic_0000002517615349_p1524581515235"></a><a name="zh-cn_topic_0000002517615349_p1524581515235"></a>媒体资源信息</p>
</td>
<td class="cellrowborder" valign="top" width="37.92%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002517615349_p20245171517239"><a name="zh-cn_topic_0000002517615349_p20245171517239"></a><a name="zh-cn_topic_0000002517615349_p20245171517239"></a>struct DCMI_media_resource {</p>
<p id="zh-cn_topic_0000002517615349_p124531582317"><a name="zh-cn_topic_0000002517615349_p124531582317"></a><a name="zh-cn_topic_0000002517615349_p124531582317"></a>/* dvpp resource */</p>
<p id="zh-cn_topic_0000002517615349_p1924561542319"><a name="zh-cn_topic_0000002517615349_p1924561542319"></a><a name="zh-cn_topic_0000002517615349_p1924561542319"></a>float jpegd;</p>
<p id="zh-cn_topic_0000002517615349_p1024561513235"><a name="zh-cn_topic_0000002517615349_p1024561513235"></a><a name="zh-cn_topic_0000002517615349_p1024561513235"></a>float jpege;</p>
<p id="zh-cn_topic_0000002517615349_p14245215192320"><a name="zh-cn_topic_0000002517615349_p14245215192320"></a><a name="zh-cn_topic_0000002517615349_p14245215192320"></a>float vpc;</p>
<p id="zh-cn_topic_0000002517615349_p19245215142315"><a name="zh-cn_topic_0000002517615349_p19245215142315"></a><a name="zh-cn_topic_0000002517615349_p19245215142315"></a>float vdec;</p>
<p id="zh-cn_topic_0000002517615349_p6245191582310"><a name="zh-cn_topic_0000002517615349_p6245191582310"></a><a name="zh-cn_topic_0000002517615349_p6245191582310"></a>float pngd;</p>
<p id="zh-cn_topic_0000002517615349_p162451815102319"><a name="zh-cn_topic_0000002517615349_p162451815102319"></a><a name="zh-cn_topic_0000002517615349_p162451815102319"></a>float venc;</p>
<p id="zh-cn_topic_0000002517615349_p11245101519232"><a name="zh-cn_topic_0000002517615349_p11245101519232"></a><a name="zh-cn_topic_0000002517615349_p11245101519232"></a>unsigned char reserved[DCMI_VDEV_FOR_RESERVE];</p>
<p id="zh-cn_topic_0000002517615349_p0245515182316"><a name="zh-cn_topic_0000002517615349_p0245515182316"></a><a name="zh-cn_topic_0000002517615349_p0245515182316"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="52.790000000000006%" headers="mcps1.2.4.1.3 "><a name="zh-cn_topic_0000002517615349_ul8505223193110"></a><a name="zh-cn_topic_0000002517615349_ul8505223193110"></a><ul id="zh-cn_topic_0000002517615349_ul8505223193110"><li>jpegd：DVPP jpegd数量。不支持时默认返回-1.0。支持形态：所有产品。</li><li>jpege：DVPP jpege数量。不支持时默认返回-1.0。支持形态：所有产品。</li><li>vpc：DVPP vpc数量。不支持时默认返回-1.0。支持形态：所有产品。</li><li>vdev：DVPP vdev数量。不支持时默认返回-1.0。支持形态：所有产品。</li><li>pngd：DVPP pngd数量。不支持时默认返回-1.0。支持形态：无。</li><li>venc：DVPP venc数量。不支持时默认返回-1.0。支持形态：所有产品。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 3** 不同部署场景下的支持情况

<a name="table155158516230"></a>
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

**调用示例<a name="section15381171918820"></a>**

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
struct dcmi_domain_info domain_info = {0};
unsigned int size = sizeof(struct dcmi_domain_info);
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_SOC_INFO, DCMI_SOC_INFO_SUB_CMD_DOMAIN_INFO, &domain_info, &size);
if (ret != 0) {
printf("[failed] ret = %d\n", ret);
return -1;
}
…
```


## DCMI\_MAIN\_CMD\_CHIP\_INF命令说明<a name="ZH-CN_TOPIC_0000002517638683"></a>

**函数原型<a name="section88293413398"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section166811419153914"></a>**

查询超节点信息。

**参数说明<a name="section9437204012398"></a>**

<a name="table1317154133812"></a>
<table><thead align="left"><tr id="row181231554203812"><th class="cellrowborder" valign="top" width="17.078292170782923%" id="mcps1.1.5.1.1"><p id="p12123105418389"><a name="p12123105418389"></a><a name="p12123105418389"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.17848215178482%" id="mcps1.1.5.1.2"><p id="p1712314549389"><a name="p1712314549389"></a><a name="p1712314549389"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.168083191680832%" id="mcps1.1.5.1.3"><p id="p141231454103812"><a name="p141231454103812"></a><a name="p141231454103812"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.57514248575143%" id="mcps1.1.5.1.4"><p id="p11123185412382"><a name="p11123185412382"></a><a name="p11123185412382"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2123854183820"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p712365423813"><a name="p712365423813"></a><a name="p712365423813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p112313543386"><a name="p112313543386"></a><a name="p112313543386"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p15123125416386"><a name="p15123125416386"></a><a name="p15123125416386"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p1212455433818"><a name="p1212455433818"></a><a name="p1212455433818"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row17124954123815"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p16124165419387"><a name="p16124165419387"></a><a name="p16124165419387"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p212418549389"><a name="p212418549389"></a><a name="p212418549389"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p101241854153819"><a name="p101241854153819"></a><a name="p101241854153819"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p012485416387"><a name="p012485416387"></a><a name="p012485416387"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p31242054153818"><a name="p31242054153818"></a><a name="p31242054153818"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p82661412141610"><a name="p82661412141610"></a><a name="p82661412141610"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row1512419544383"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p2012485453813"><a name="p2012485453813"></a><a name="p2012485453813"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p1312405417387"><a name="p1312405417387"></a><a name="p1312405417387"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p31241754173815"><a name="p31241754173815"></a><a name="p31241754173815"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p16124105417386"><a name="p16124105417386"></a><a name="p16124105417386"></a>DCMI_MAIN_CMD_CHIP_INF</p>
</td>
</tr>
<tr id="row5124754193815"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p161249543383"><a name="p161249543383"></a><a name="p161249543383"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p111248547386"><a name="p111248547386"></a><a name="p111248547386"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p1812465411385"><a name="p1812465411385"></a><a name="p1812465411385"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><pre class="codeblock" id="codeblock117365575012"><a name="codeblock117365575012"></a><a name="codeblock117365575012"></a>typedef enum {
DCMI_CHIP_INF_SUB_CMD_CHIP_ID,
DCMI_CHIP_INF_SUB_CMD_SPOD_INFO,
DCMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS
DCMI_CHIP_INF_SUB_CMD_MAX = 0xFF,
} DCMI_CHIP_INFO_SUB_CMD;</pre>
</td>
</tr>
<tr id="row2012511546389"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p7125165483817"><a name="p7125165483817"></a><a name="p7125165483817"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p10125554153817"><a name="p10125554153817"></a><a name="p10125554153817"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p16125115414381"><a name="p16125115414381"></a><a name="p16125115414381"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p1612555420387"><a name="p1612555420387"></a><a name="p1612555420387"></a>详见本节<a href="#section1374114910415">约束说明</a>。</p>
</td>
</tr>
<tr id="row7125254143815"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p4125165433814"><a name="p4125165433814"></a><a name="p4125165433814"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p18125115416386"><a name="p18125115416386"></a><a name="p18125115416386"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p7125145419382"><a name="p7125145419382"></a><a name="p7125145419382"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p81251954153819"><a name="p81251954153819"></a><a name="p81251954153819"></a>返回结果数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section189807595317"></a>**

<a name="table154712012328"></a>
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

**异常处理<a name="section9677135115517"></a>**

无。

**约束说明<a name="section1374114910415"></a>**

SDID合法性说明：SDID表示32比特位的数值，可分为4个ID字段。22\~31比特表示server\_index，取值范围0\~47；18\~21比特表示Chip ID ，取值范围0\~7；16\~17比特表示DIE ID，取值范围0\~2；0\~15比特表示Device ID，取值为Chip ID乘以2+DIE ID。

**表 1**  sub\_cmd对应的buf格式

<a name="table1148155443816"></a>
<table><thead align="left"><tr id="row7126155473815"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p1312695412383"><a name="p1312695412383"></a><a name="p1312695412383"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p121262547386"><a name="p121262547386"></a><a name="p121262547386"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p0126254113814"><a name="p0126254113814"></a><a name="p0126254113814"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p36346131358"><a name="p36346131358"></a><a name="p36346131358"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row712655418385"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1912610545385"><a name="p1912610545385"></a><a name="p1912610545385"></a>DCMI_CHIP_INF_SUB_CMD_SPOD_INFO</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1412635410384"><a name="p1412635410384"></a><a name="p1412635410384"></a>struct dcmi_spod_info {</p>
<p id="p10126175423819"><a name="p10126175423819"></a><a name="p10126175423819"></a>unsigned int sdid;</p>
<p id="p21267542389"><a name="p21267542389"></a><a name="p21267542389"></a>unsigned int <span>super</span><span>_</span><span>pod</span><span>_</span><span>size</span>;</p>
<p id="p612695417388"><a name="p612695417388"></a><a name="p612695417388"></a>unsigned int super_pod_id;</p>
<p id="p14126185483815"><a name="p14126185483815"></a><a name="p14126185483815"></a>unsigned int server_index;</p>
<p id="p14126154173816"><a name="p14126154173816"></a><a name="p14126154173816"></a>unsigned int reserve[8];</p>
<p id="p8126195418380"><a name="p8126195418380"></a><a name="p8126195418380"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1712718547382"><a name="p1712718547382"></a><a name="p1712718547382"></a>sizeof(struct dcmi_spod_info)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><a name="ul1084711499719"></a><a name="ul1084711499719"></a><ul id="ul1084711499719"><li>sdid表示超节点系统里每个NPU的唯一标识，此处应传入目标NPU的SDID；</li><li><span>super</span><span>_</span><span>pod</span><span>_</span><span>size</span>表示超节点集群规模；</li><li>super_pod_id表示超节点ID，指整个超节点集群；</li><li>server_index表示计算服务器在超节点集群内index。</li></ul>
</td>
</tr>
<tr id="row1279662818243"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p0501134011248"><a name="p0501134011248"></a><a name="p0501134011248"></a>DCMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1250164092412"><a name="p1250164092412"></a><a name="p1250164092412"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1850154018240"><a name="p1850154018240"></a><a name="p1850154018240"></a>sizeof(unsigned int)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p11501940192410"><a name="p11501940192410"></a><a name="p11501940192410"></a>buf同时作为接口的入参和出参。</p>
<a name="ul1501154022414"></a><a name="ul1501154022414"></a><ul id="ul1501154022414"><li>入参时，buf表示待查询NPU的SDID。</li><li>出参时，buf表示SDID对应NPU的状态。<a name="ul1518995073513"></a><a name="ul1518995073513"></a><ul id="ul1518995073513"><li>1表示目标NPU状态异常，后续共享资源释放方式为：本端NPU强制释放共享给目标NPU的资源。</li><li>0表示目标NPU状态正常，后续共享资源释放方式为：本端NPU协商释放共享给目标NPU的资源。<p id="zh-cn_topic_0000002485318814_p13657192032314"><a name="zh-cn_topic_0000002485318814_p13657192032314"></a><a name="zh-cn_topic_0000002485318814_p13657192032314"></a></p>
</li></ul>
</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section083811564420"></a>**

查询card\_id和device\_id标识的NPU超节点信息。

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
struct dcmi_spod_info spod_info = {0};
unsigned int buf_size = sizeof(struct dcmi_spod_info);
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_CHIP_INF,
DCMI_CHIP_INF_SUB_CMD_SPOD_INFO, & spod_info, &buf_size);
if (ret != 0) {
//todo
return ret;
} else {
// todo
return ret;
}
…
```

查询card\_id和device\_id标识的NPU对SDID标识的NPU状态信息的记录。

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
union {
unsigned int sdid;
unsigned int status;
} para; 
unsigned int buf_size = sizeof(para); 
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_CHIP_INF, DCMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS, &para.sdid, &buf_size); 
if (ret != 0) { 
//todo return ret;
} 
else { 
printf(“status=%u\n”,para.status); 
return ret; 
} 
…
```


## DCMI\_MAIN\_CMD\_SIO命令说明<a name="ZH-CN_TOPIC_0000002517638725"></a>

**函数原型<a name="section88293413398"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf, unsigned int \*size\)**

**功能说明<a name="section166811419153914"></a>**

查询die间SIO状态。

**参数说明<a name="section9437204012398"></a>**

<a name="table1317154133812"></a>
<table><thead align="left"><tr id="row181231554203812"><th class="cellrowborder" valign="top" width="17.078292170782923%" id="mcps1.1.5.1.1"><p id="p12123105418389"><a name="p12123105418389"></a><a name="p12123105418389"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.17848215178482%" id="mcps1.1.5.1.2"><p id="p1712314549389"><a name="p1712314549389"></a><a name="p1712314549389"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="19.168083191680832%" id="mcps1.1.5.1.3"><p id="p141231454103812"><a name="p141231454103812"></a><a name="p141231454103812"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="48.57514248575143%" id="mcps1.1.5.1.4"><p id="p11123185412382"><a name="p11123185412382"></a><a name="p11123185412382"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2123854183820"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p712365423813"><a name="p712365423813"></a><a name="p712365423813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p112313543386"><a name="p112313543386"></a><a name="p112313543386"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p15123125416386"><a name="p15123125416386"></a><a name="p15123125416386"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p1212455433818"><a name="p1212455433818"></a><a name="p1212455433818"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row17124954123815"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p16124165419387"><a name="p16124165419387"></a><a name="p16124165419387"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p212418549389"><a name="p212418549389"></a><a name="p212418549389"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p101241854153819"><a name="p101241854153819"></a><a name="p101241854153819"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p012485416387"><a name="p012485416387"></a><a name="p012485416387"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p31242054153818"><a name="p31242054153818"></a><a name="p31242054153818"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p15758182171612"><a name="p15758182171612"></a><a name="p15758182171612"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row1512419544383"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p2012485453813"><a name="p2012485453813"></a><a name="p2012485453813"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p1312405417387"><a name="p1312405417387"></a><a name="p1312405417387"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p31241754173815"><a name="p31241754173815"></a><a name="p31241754173815"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p16124105417386"><a name="p16124105417386"></a><a name="p16124105417386"></a>DCMI_MAIN_CMD_SIO</p>
</td>
</tr>
<tr id="row5124754193815"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p161249543383"><a name="p161249543383"></a><a name="p161249543383"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p111248547386"><a name="p111248547386"></a><a name="p111248547386"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p1812465411385"><a name="p1812465411385"></a><a name="p1812465411385"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><pre class="codeblock" id="codeblock117365575012"><a name="codeblock117365575012"></a><a name="codeblock117365575012"></a>typedef enum {
DCMI_SIO_SUB_CMD_CRC_ERR_STATISTICS = 0
}DCMI_SIO_SUB_CMD;</pre>
</td>
</tr>
<tr id="row2012511546389"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p7125165483817"><a name="p7125165483817"></a><a name="p7125165483817"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p10125554153817"><a name="p10125554153817"></a><a name="p10125554153817"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p16125115414381"><a name="p16125115414381"></a><a name="p16125115414381"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p1612555420387"><a name="p1612555420387"></a><a name="p1612555420387"></a>详见本节<a href="DCMI_MAIN_CMD_CHIP_INF命令说明-4.md#section1374114910415">约束说明</a>。</p>
</td>
</tr>
<tr id="row7125254143815"><td class="cellrowborder" valign="top" width="17.078292170782923%" headers="mcps1.1.5.1.1 "><p id="p4125165433814"><a name="p4125165433814"></a><a name="p4125165433814"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.17848215178482%" headers="mcps1.1.5.1.2 "><p id="p18125115416386"><a name="p18125115416386"></a><a name="p18125115416386"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="19.168083191680832%" headers="mcps1.1.5.1.3 "><p id="p7125145419382"><a name="p7125145419382"></a><a name="p7125145419382"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="48.57514248575143%" headers="mcps1.1.5.1.4 "><p id="p81251954153819"><a name="p81251954153819"></a><a name="p81251954153819"></a>返回结果数据长度。</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section189807595317"></a>**

<a name="table154712012328"></a>
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

**异常处理<a name="section9677135115517"></a>**

无。

**约束说明<a name="section1374114910415"></a>**

**表 1**  sub\_cmd对应的buf格式

<a name="table1148155443816"></a>
<table><thead align="left"><tr id="row7126155473815"><th class="cellrowborder" valign="top" width="24.45%" id="mcps1.2.5.1.1"><p id="p1312695412383"><a name="p1312695412383"></a><a name="p1312695412383"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="32.99%" id="mcps1.2.5.1.2"><p id="p121262547386"><a name="p121262547386"></a><a name="p121262547386"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20.01%" id="mcps1.2.5.1.3"><p id="p0126254113814"><a name="p0126254113814"></a><a name="p0126254113814"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="22.55%" id="mcps1.2.5.1.4"><p id="p106461111681"><a name="p106461111681"></a><a name="p106461111681"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row712655418385"><td class="cellrowborder" valign="top" width="24.45%" headers="mcps1.2.5.1.1 "><p id="p1912610545385"><a name="p1912610545385"></a><a name="p1912610545385"></a>DCMI_CHIP_INF_SUB_CMD_SPOD_INFO</p>
</td>
<td class="cellrowborder" valign="top" width="32.99%" headers="mcps1.2.5.1.2 "><p id="p1664914023114"><a name="p1664914023114"></a><a name="p1664914023114"></a>struct dcmi_sio_crc_err_statistics_info {</p>
<p id="p195228943118"><a name="p195228943118"></a><a name="p195228943118"></a>unsigned short tx_error_count;</p>
<p id="p1959618125316"><a name="p1959618125316"></a><a name="p1959618125316"></a>unsigned short rx_error_count;</p>
<p id="p966122118317"><a name="p966122118317"></a><a name="p966122118317"></a>unsigned char reserved[8];</p>
<p id="p128675414305"><a name="p128675414305"></a><a name="p128675414305"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="20.01%" headers="mcps1.2.5.1.3 "><p id="p1712718547382"><a name="p1712718547382"></a><a name="p1712718547382"></a>sizeof(struct dcmi_sio_crc_err_statistics_info)</p>
</td>
<td class="cellrowborder" valign="top" width="22.55%" headers="mcps1.2.5.1.4 "><a name="ul89281736384"></a><a name="ul89281736384"></a><ul id="ul89281736384"><li>tx_error_count表示SIO发送错误的个数；</li><li>rx_error_count表示SIO接收错误的个数。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section083811564420"></a>**

```
…
int ret = 0;
int card_id = 0;
int dev_id = 0;
struct dcmi_sio_crc_err_statistics_info sio_info = {0};
unsigned int sio_info_size = sizeof(struct dcmi_sio_crc_err_statistics_info);
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_SIO, DCMI_SIO_SUB_CMD_CRC_ERR_STATISTICS, &sio_info, &sio_info_size);
if (ret != 0) {
//todo
return ret;
} else {
// todo
return ret;
}
…
```


## DCMI\_MAIN\_CMD\_PCIE命令说明<a name="ZH-CN_TOPIC_0000002517638667"></a>

**函数原型<a name="section224642513574"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf,unsigned int \*size\)**

**功能说明<a name="section162477254573"></a>**

获取PCIe相关信息。

**参数说明<a name="section16248625165719"></a>**

<a name="table23011825165718"></a>
<table><thead align="left"><tr id="row1644220264578"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p19442192665715"><a name="p19442192665715"></a><a name="p19442192665715"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p84426269577"><a name="p84426269577"></a><a name="p84426269577"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="p164426264575"><a name="p164426264575"></a><a name="p164426264575"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p14424263576"><a name="p14424263576"></a><a name="p14424263576"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row144428269571"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p11442192614578"><a name="p11442192614578"></a><a name="p11442192614578"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p544232625716"><a name="p544232625716"></a><a name="p544232625716"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p17442526165714"><a name="p17442526165714"></a><a name="p17442526165714"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p16442182635713"><a name="p16442182635713"></a><a name="p16442182635713"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row8442182617573"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p134421126155713"><a name="p134421126155713"></a><a name="p134421126155713"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p16442142613570"><a name="p16442142613570"></a><a name="p16442142613570"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p6442142615710"><a name="p6442142615710"></a><a name="p6442142615710"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p11442226105712"><a name="p11442226105712"></a><a name="p11442226105712"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p17442112616571"><a name="p17442112616571"></a><a name="p17442112616571"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p2975825161619"><a name="p2975825161619"></a><a name="p2975825161619"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row1544262610576"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p2044352611575"><a name="p2044352611575"></a><a name="p2044352611575"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p7443152613571"><a name="p7443152613571"></a><a name="p7443152613571"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p104439262577"><a name="p104439262577"></a><a name="p104439262577"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p844312611573"><a name="p844312611573"></a><a name="p844312611573"></a>DCMI_MAIN_CMD_PCIE</p>
</td>
</tr>
<tr id="row6443426185717"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p14431726185711"><a name="p14431726185711"></a><a name="p14431726185711"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p84431426165711"><a name="p84431426165711"></a><a name="p84431426165711"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p1844312619576"><a name="p1844312619576"></a><a name="p1844312619576"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p11443162655718"><a name="p11443162655718"></a><a name="p11443162655718"></a><span>DCMI_PCIE_SUB_CMD_PCIE_ERROR_INFO</span></p>
</td>
</tr>
<tr id="row144342617576"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p0443172665711"><a name="p0443172665711"></a><a name="p0443172665711"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p644362612579"><a name="p644362612579"></a><a name="p644362612579"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p154431526205714"><a name="p154431526205714"></a><a name="p154431526205714"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p194431626195715"><a name="p194431626195715"></a><a name="p194431626195715"></a>详见本章<a href="DCMI_MAIN_CMD_CHIP_INF命令说明-4.md#section1374114910415">约束说明</a>。</p>
</td>
</tr>
<tr id="row1144315264573"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p2443526125719"><a name="p2443526125719"></a><a name="p2443526125719"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p12443326145716"><a name="p12443326145716"></a><a name="p12443326145716"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p4443526135714"><a name="p4443526135714"></a><a name="p4443526135714"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p3443102675716"><a name="p3443102675716"></a><a name="p3443102675716"></a>Buf数组长度</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section1276192518575"></a>**

<a name="table154712012328"></a>
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

**异常处理<a name="section10284025165720"></a>**

无。

**约束说明<a name="section142847254570"></a>**

仅Control CPU开放形态使用该接口。

对于A200T A3 Box8 超节点服务器，该接口在物理机+特权容器场景下支持使用。

**表 1**  sub\_cmd对应的buf格式

<a name="table131652519571"></a>
<table><thead align="left"><tr id="row144441226125711"><th class="cellrowborder" valign="top" width="30.303030303030305%" id="mcps1.2.4.1.1"><p id="p64441826115711"><a name="p64441826115711"></a><a name="p64441826115711"></a><strong id="b3444192610574"><a name="b3444192610574"></a><a name="b3444192610574"></a>sub_cmd</strong></p>
</th>
<th class="cellrowborder" valign="top" width="47.474747474747474%" id="mcps1.2.4.1.2"><p id="p64444261570"><a name="p64444261570"></a><a name="p64444261570"></a>Buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="22.222222222222225%" id="mcps1.2.4.1.3"><p id="p1644432645711"><a name="p1644432645711"></a><a name="p1644432645711"></a><strong id="b13444726175712"><a name="b13444726175712"></a><a name="b13444726175712"></a>参数说明</strong></p>
</th>
</tr>
</thead>
<tbody><tr id="row12444826175711"><td class="cellrowborder" valign="top" width="30.303030303030305%" headers="mcps1.2.4.1.1 "><p id="p17444112611573"><a name="p17444112611573"></a><a name="p17444112611573"></a><span>DCMI_PCIE_SUB_CMD_PCIE_ERROR_INFO</span></p>
</td>
<td class="cellrowborder" valign="top" width="47.474747474747474%" headers="mcps1.2.4.1.2 "><p id="p124454268570"><a name="p124454268570"></a><a name="p124454268570"></a>struct dcmi_pcie_link_error_info {</p>
<p id="p94452262574"><a name="p94452262574"></a><a name="p94452262574"></a>unsigned int tx_err_cnt;</p>
<p id="p2044522615576"><a name="p2044522615576"></a><a name="p2044522615576"></a>unsigned int rx_err_cnt;</p>
<p id="p34456263573"><a name="p34456263573"></a><a name="p34456263573"></a>unsigned int lcrc_err_cnt;</p>
<p id="p34450264576"><a name="p34450264576"></a><a name="p34450264576"></a>unsigned int ecrc_err_cnt;</p>
<p id="p24455263576"><a name="p24455263576"></a><a name="p24455263576"></a>unsigned int retry_cnt;</p>
<p id="p15445152619575"><a name="p15445152619575"></a><a name="p15445152619575"></a>unsigned int rsv[32];</p>
<p id="p1445182613571"><a name="p1445182613571"></a><a name="p1445182613571"></a>};</p>
</td>
<td class="cellrowborder" valign="top" width="22.222222222222225%" headers="mcps1.2.4.1.3 "><p id="p7445142665715"><a name="p7445142665715"></a><a name="p7445142665715"></a>获取pcie_link_error相关值。</p>
<a name="ul19681694120"></a><a name="ul19681694120"></a><ul id="ul19681694120"><li>tx_err_cnt表示PCIe发送错误计数</li><li>rx_err_cnt表示PCIe接收错误计数</li><li>lcrc_err_cnt表示PCIe DLLP LCRC校验错误计数</li><li>ecrc_err_cnt表示PCIe TLP ECRC校验错误计数</li><li>retry_cnt表示PCIe链路重传计数</li><li>rsv保留字段</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table6183194116432"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row20271184815710"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p105851457103916"><a name="zh-cn_topic_0000002485318818_p105851457103916"></a><a name="zh-cn_topic_0000002485318818_p105851457103916"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p1258655715399"><a name="zh-cn_topic_0000002485318818_p1258655715399"></a><a name="zh-cn_topic_0000002485318818_p1258655715399"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p55865575397"><a name="zh-cn_topic_0000002485318818_p55865575397"></a><a name="zh-cn_topic_0000002485318818_p55865575397"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p958625719394"><a name="zh-cn_topic_0000002485318818_p958625719394"></a><a name="zh-cn_topic_0000002485318818_p958625719394"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row1527104875720"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p13271248185720"><a name="zh-cn_topic_0000002485318818_p13271248185720"></a><a name="zh-cn_topic_0000002485318818_p13271248185720"></a><span id="zh-cn_topic_0000002485318818_text1527154812578"><a name="zh-cn_topic_0000002485318818_text1527154812578"></a><a name="zh-cn_topic_0000002485318818_text1527154812578"></a><span id="zh-cn_topic_0000002485318818_text2271148205710"><a name="zh-cn_topic_0000002485318818_text2271148205710"></a><a name="zh-cn_topic_0000002485318818_text2271148205710"></a>Atlas 900 A3 SuperPoD 超节点</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p10271248165717"><a name="zh-cn_topic_0000002485318818_p10271248165717"></a><a name="zh-cn_topic_0000002485318818_p10271248165717"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p11271348175719"><a name="zh-cn_topic_0000002485318818_p11271348175719"></a><a name="zh-cn_topic_0000002485318818_p11271348175719"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p6271144865712"><a name="zh-cn_topic_0000002485318818_p6271144865712"></a><a name="zh-cn_topic_0000002485318818_p6271144865712"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row527124819571"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p42711448205716"><a name="zh-cn_topic_0000002485318818_p42711448205716"></a><a name="zh-cn_topic_0000002485318818_p42711448205716"></a><span id="zh-cn_topic_0000002485318818_text8271134845715"><a name="zh-cn_topic_0000002485318818_text8271134845715"></a><a name="zh-cn_topic_0000002485318818_text8271134845715"></a><span id="zh-cn_topic_0000002485318818_text527164811575"><a name="zh-cn_topic_0000002485318818_text527164811575"></a><a name="zh-cn_topic_0000002485318818_text527164811575"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p12271164818576"><a name="zh-cn_topic_0000002485318818_p12271164818576"></a><a name="zh-cn_topic_0000002485318818_p12271164818576"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p427134816570"><a name="zh-cn_topic_0000002485318818_p427134816570"></a><a name="zh-cn_topic_0000002485318818_p427134816570"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p627114835717"><a name="zh-cn_topic_0000002485318818_p627114835717"></a><a name="zh-cn_topic_0000002485318818_p627114835717"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row18271184811578"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p79671124131618"><a name="zh-cn_topic_0000002485318818_p79671124131618"></a><a name="zh-cn_topic_0000002485318818_p79671124131618"></a><span id="zh-cn_topic_0000002485318818_text49671624141613"><a name="zh-cn_topic_0000002485318818_text49671624141613"></a><a name="zh-cn_topic_0000002485318818_text49671624141613"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p9271134885710"><a name="zh-cn_topic_0000002485318818_p9271134885710"></a><a name="zh-cn_topic_0000002485318818_p9271134885710"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p7271104814571"><a name="zh-cn_topic_0000002485318818_p7271104814571"></a><a name="zh-cn_topic_0000002485318818_p7271104814571"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p8271048185714"><a name="zh-cn_topic_0000002485318818_p8271048185714"></a><a name="zh-cn_topic_0000002485318818_p8271048185714"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row285204910295"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p15986185513296"><a name="zh-cn_topic_0000002485318818_p15986185513296"></a><a name="zh-cn_topic_0000002485318818_p15986185513296"></a><span id="zh-cn_topic_0000002485318818_text098685572912"><a name="zh-cn_topic_0000002485318818_text098685572912"></a><a name="zh-cn_topic_0000002485318818_text098685572912"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p105251859192911"><a name="zh-cn_topic_0000002485318818_p105251859192911"></a><a name="zh-cn_topic_0000002485318818_p105251859192911"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p7525145902913"><a name="zh-cn_topic_0000002485318818_p7525145902913"></a><a name="zh-cn_topic_0000002485318818_p7525145902913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p0525165962917"><a name="zh-cn_topic_0000002485318818_p0525165962917"></a><a name="zh-cn_topic_0000002485318818_p0525165962917"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row14271848145719"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p527118483579"><a name="zh-cn_topic_0000002485318818_p527118483579"></a><a name="zh-cn_topic_0000002485318818_p527118483579"></a><span id="zh-cn_topic_0000002485318818_text15271174815713"><a name="zh-cn_topic_0000002485318818_text15271174815713"></a><a name="zh-cn_topic_0000002485318818_text15271174815713"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p16271194813571"><a name="zh-cn_topic_0000002485318818_p16271194813571"></a><a name="zh-cn_topic_0000002485318818_p16271194813571"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p1827154818570"><a name="zh-cn_topic_0000002485318818_p1827154818570"></a><a name="zh-cn_topic_0000002485318818_p1827154818570"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p72711448185716"><a name="zh-cn_topic_0000002485318818_p72711448185716"></a><a name="zh-cn_topic_0000002485318818_p72711448185716"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row886601693"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p17525251914"><a name="zh-cn_topic_0000002485318818_p17525251914"></a><a name="zh-cn_topic_0000002485318818_p17525251914"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section8297125115718"></a>**

```
… 
int ret = 0;
int card_id = 0;
int dev_id = 0;
struct dcmi_pcie_link_error_info pcie_link_error_info = {0};
unsigned int info_leng = sizeof(struct dcmi_pcie_link_error_info);
ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_PCIE, DCMI_PCIE_SUB_CMD_PCIE_ERROR_INFO, &pcie_link_error_info, &info_leng);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```


## DCMI\_MAIN\_CMD\_SOC\_INFO命令说明<a name="ZH-CN_TOPIC_0000002485478786"></a>

**函数原型<a name="section224642513574"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf,unsigned int \*size\)**

**功能说明<a name="section162477254573"></a>**

获取SOC相关信息。

**参数说明<a name="section16248625165719"></a>**

<a name="table23011825165718"></a>
<table><thead align="left"><tr id="row1644220264578"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p19442192665715"><a name="p19442192665715"></a><a name="p19442192665715"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.15%" id="mcps1.1.5.1.2"><p id="p84426269577"><a name="p84426269577"></a><a name="p84426269577"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.3"><p id="p164426264575"><a name="p164426264575"></a><a name="p164426264575"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50.51%" id="mcps1.1.5.1.4"><p id="p14424263576"><a name="p14424263576"></a><a name="p14424263576"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row144428269571"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p11442192614578"><a name="p11442192614578"></a><a name="p11442192614578"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p544232625716"><a name="p544232625716"></a><a name="p544232625716"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p17442526165714"><a name="p17442526165714"></a><a name="p17442526165714"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p16442182635713"><a name="p16442182635713"></a><a name="p16442182635713"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row8442182617573"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p134421126155713"><a name="p134421126155713"></a><a name="p134421126155713"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p16442142613570"><a name="p16442142613570"></a><a name="p16442142613570"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p6442142615710"><a name="p6442142615710"></a><a name="p6442142615710"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p11442226105712"><a name="p11442226105712"></a><a name="p11442226105712"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p17442112616571"><a name="p17442112616571"></a><a name="p17442112616571"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p15478182901618"><a name="p15478182901618"></a><a name="p15478182901618"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row1544262610576"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p2044352611575"><a name="p2044352611575"></a><a name="p2044352611575"></a>main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p7443152613571"><a name="p7443152613571"></a><a name="p7443152613571"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p104439262577"><a name="p104439262577"></a><a name="p104439262577"></a>enum dcmi_main_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p844312611573"><a name="p844312611573"></a><a name="p844312611573"></a>DCMI_MAIN_CMD_SOC_INFO</p>
</td>
</tr>
<tr id="row6443426185717"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p14431726185711"><a name="p14431726185711"></a><a name="p14431726185711"></a>sub_cmd</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p84431426165711"><a name="p84431426165711"></a><a name="p84431426165711"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p1844312619576"><a name="p1844312619576"></a><a name="p1844312619576"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p11443162655718"><a name="p11443162655718"></a><a name="p11443162655718"></a>DCMI_SOC_INFO_SUB_CMD_CUSTOM_OP</p>
</td>
</tr>
<tr id="row144342617576"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p0443172665711"><a name="p0443172665711"></a><a name="p0443172665711"></a>buf</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p644362612579"><a name="p644362612579"></a><a name="p644362612579"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p154431526205714"><a name="p154431526205714"></a><a name="p154431526205714"></a>void *</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p194431626195715"><a name="p194431626195715"></a><a name="p194431626195715"></a>详见本章<a href="#section961224610162">约束说明</a>。</p>
</td>
</tr>
<tr id="row1144315264573"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p2443526125719"><a name="p2443526125719"></a><a name="p2443526125719"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="15.15%" headers="mcps1.1.5.1.2 "><p id="p12443326145716"><a name="p12443326145716"></a><a name="p12443326145716"></a>输入/输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.3 "><p id="p4443526135714"><a name="p4443526135714"></a><a name="p4443526135714"></a>unsigned int *</p>
</td>
<td class="cellrowborder" valign="top" width="50.51%" headers="mcps1.1.5.1.4 "><p id="p3443102675716"><a name="p3443102675716"></a><a name="p3443102675716"></a>buf数组长度</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section1276192518575"></a>**

<a name="table154712012328"></a>
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

**异常处理<a name="section10284025165720"></a>**

无

**约束说明<a name="section961224610162"></a>**

**表 1**  sub\_cmd对应的buf格式

<a name="table192633010224"></a>
<table><thead align="left"><tr id="row3271230142220"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p13271130172213"><a name="p13271130172213"></a><a name="p13271130172213"></a>sub_cmd</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p72793011221"><a name="p72793011221"></a><a name="p72793011221"></a>buf对应的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p152673404312"><a name="p152673404312"></a><a name="p152673404312"></a>size</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p17643133310315"><a name="p17643133310315"></a><a name="p17643133310315"></a>参数说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row92715303227"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p142710306225"><a name="p142710306225"></a><a name="p142710306225"></a>DCMI_SOC_INFO_SUB_CMD_CUSTOM_OP</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1618115016210"><a name="p1618115016210"></a><a name="p1618115016210"></a>unsigned int</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p364642917314"><a name="p364642917314"></a><a name="p364642917314"></a>长度为：sizeof(unsigned int)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p06431333153111"><a name="p06431333153111"></a><a name="p06431333153111"></a>0：关闭AI CPU算子免权签功能</p>
<p id="p1313285923110"><a name="p1313285923110"></a><a name="p1313285923110"></a>1：开启AI CPU算子免权签功能</p>
</td>
</tr>
</tbody>
</table>

**表 2** 不同部署场景下的支持情况

<a name="table1891871242416"></a>
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

**调用示例<a name="section8297125115718"></a>**

```
    … 
    int ret = 0;
    int card_id = 0;
    int dev_id = 0;
    unsigned int enable_flag = 1;
    unsigned int size = (unsigned int)sizeof(int);
    ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_SOC_INFO, 
        DCMI_SOC_INFO_SUB_CMD_CUSTOM_OP, &enable_flag, &size);
    if (ret != DCMI_OK){
        //todo：记录日志
        return ret;
    }
    …
```


## DCMI\_MAIN\_CMD\_DEVICE\_SHARE命令说明<a name="ZH-CN_TOPIC_0000002485478778"></a>

**函数原型<a name="section782265716308"></a>**

**int dcmi\_get\_device\_info\(int card\_id, int device\_id, enum dcmi\_main\_cmd main\_cmd, unsigned int sub\_cmd, void \*buf,unsigned int \*size\)**

**功能说明<a name="section19822105710305"></a>**

查询指定芯片的容器共享使能标记。

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
<p id="p16638123531619"><a name="p16638123531619"></a><a name="p16638123531619"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
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
<p id="p1068613612160"><a name="p1068613612160"></a><a name="p1068613612160"></a></p>
<div class="note" id="note1218455521019"><a name="note1218455521019"></a><a name="note1218455521019"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="p14728105710819"><a name="p14728105710819"></a><a name="p14728105710819"></a>查询容器共享功能也可参见<a href="dcmi_get_device_share_enable.md">dcmi_get_device_share_enable</a>。</p>
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
<p id="p59671384166"><a name="p59671384166"></a><a name="p59671384166"></a></p>
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

<a name="table1985018574309"></a>
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

**异常处理<a name="section6438163417415"></a>**

无。

**约束说明<a name="section13645113118442"></a>**

-   设置容器共享模式后，重启系统后容器使能状态默认为禁用。
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

**调用示例<a name="section49611431655"></a>**

```
    … 
    int ret = 0;
    int card_id = 0;
    int dev_id = 0;
    unsigned int enable_flag = 1;
    unsigned int size = (unsigned int)sizeof(int);
    ret = dcmi_get_device_info(card_id, dev_id, DCMI_MAIN_CMD_DEVICE_SHARE, 
        DCMI_DEVICE_SHARE_SUB_CMD_COMMON, &enable_flag, &size);
    if (ret != DCMI_OK){
        //todo：记录日志
        return ret;
    }
    …
```


