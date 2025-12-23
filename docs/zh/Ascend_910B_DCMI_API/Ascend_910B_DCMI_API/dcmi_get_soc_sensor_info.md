# dcmi\_get\_soc\_sensor\_info<a name="ZH-CN_TOPIC_0000002485455430"></a>

**函数原型<a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_toc533412077"></a>**

**int dcmi\_get\_soc\_sensor\_info\(int card\_id, int device\_id, int sensor\_id, union tag\_sensor\_info \*sensor\_info\)**

**功能说明<a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_toc533412078"></a>**

获取设备的传感器信息。

**参数说明<a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_toc533412079"></a>**

<a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p10021890"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p10021890"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p6466753"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p6466753"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p54045009"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p54045009"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15569626"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15569626"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p36741947142813"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p36741947142813"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p96741747122818"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p96741747122818"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p46747472287"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p46747472287"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1467413474281"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1467413474281"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_num_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p7711145152918"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p7711145152918"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p671116522914"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p671116522914"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1771116572910"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1771116572910"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p11512051201615"><a name="p11512051201615"></a><a name="p11512051201615"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1762123612289"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1762123612289"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1762123612289"></a>sensor_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p46211236182811"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p46211236182811"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p46211236182811"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1662112368286"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1662112368286"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1662112368286"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p7852181031513"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p7852181031513"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p7852181031513"></a>enum dcmi_manager_sensor_id {</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1785221017151"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1785221017151"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1785221017151"></a>DCMI_CLUSTER_TEMP_ID = 0,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p38521010101510"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p38521010101510"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p38521010101510"></a>DCMI_PERI_TEMP_ID = 1,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p085261051512"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p085261051512"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p085261051512"></a>DCMI_AICORE0_TEMP_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1285241010157"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1285241010157"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1285241010157"></a>DCMI_AICORE1_TEMP_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p485201081517"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p485201081517"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p485201081517"></a>DCMI_AICORE_LIMIT_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1785221015156"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1785221015156"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1785221015156"></a>DCMI_AICORE_TOTAL_PER_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15852201014159"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15852201014159"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15852201014159"></a>DCMI_AICORE_ELIM_PER_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p185212107151"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p185212107151"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p185212107151"></a>DCMI_AICORE_BASE_FREQ_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p14852151041518"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p14852151041518"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p14852151041518"></a>DCMI_NPU_DDR_FREQ_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p18852141014159"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p18852141014159"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p18852141014159"></a>DCMI_THERMAL_THRESHOLD_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p11852810151518"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p11852810151518"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p11852810151518"></a>DCMI_NTC_TEMP_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p158521210141512"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p158521210141512"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p158521210141512"></a>DCMI_SOC_TEMP_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p198521110181519"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p198521110181519"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p198521110181519"></a>DCMI_FP_TEMP_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p208526102159"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p208526102159"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p208526102159"></a>DCMI_N_DIE_TEMP_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p16852131061516"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p16852131061516"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p16852131061516"></a>DCMI_HBM_TEMP_ID,</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15852161081518"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15852161081518"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p15852161081518"></a>DCMI_SENSOR_INVALID_ID = 255</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p17852121015152"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p17852121015152"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p17852121015152"></a>};</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p186975918015"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p186975918015"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p186975918015"></a>指定传感器索引，具体如下值：</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p198819103113"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p198819103113"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p198819103113"></a>0：DCMI_CLUSTER_TEMP_ID，表示CLUSTER温度；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1298861173110"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1298861173110"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1298861173110"></a>1：DCMI_PERI_TEMP_ID，表示PERI温度；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p12988151163118"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p12988151163118"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p12988151163118"></a>2：DCMI_AICORE0_TEMP_ID，表示AICORE0温度；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1498861173113"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1498861173113"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1498861173113"></a>3：DCMI_AICORE1_TEMP_ID，表示AICORE1温度；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p998810116317"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p998810116317"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p998810116317"></a>4：DCMI_AICORE_LIMIT_ID，AICORE限核状态返回结果是0，不限核返回结果是1；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p29884133112"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p29884133112"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p29884133112"></a>5：DCMI_AICORE_TOTAL_PER_ID，表示AICORE脉冲总周期；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p398841113120"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p398841113120"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p398841113120"></a>6：DCMI_AICORE_ELIM_PER_ID，表示AICORE可消除周期；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p119882193110"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p119882193110"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p119882193110"></a>7：DCMI_AICORE_BASE_FREQ_ID，表示AICORE基准频率MHz；返回值对应输出联合体中的ushort成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p19988131163113"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p19988131163113"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p19988131163113"></a>8：DCMI_NPU_DDR_FREQ_ID，表示DDR频率单位MHz；返回值对应输出联合体中的ushort成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p139886119313"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p139886119313"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p139886119313"></a>9：DCMI_THERMAL_THRESHOLD_ID，返回值对应输出联合体中的temp[2]成员；temp[0]为温饱限频温度，temp[1]为系统复位温度；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p169881914318"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p169881914318"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p169881914318"></a>10：DCMI_NTC_TEMP_ID，返回值对应输出联合体中的ntc_tmp[4]成员；ntc_tmp[0] ntc_tmp[1] ntc_tmp[2] ntc_tmp[3]分别对应四个热敏电阻温度。</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p99886118317"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p99886118317"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p99886118317"></a>11：DCMI_SOC_TEMP_ID，表示SOC最高温；返回值对应输出联合体中的uchar成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p5988016310"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p5988016310"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p5988016310"></a>12：DCMI_FP_TEMP_ID，表示光模块最高温度；返回值对应输出联合体中的signed int iint成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p698815115312"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p698815115312"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p698815115312"></a>13：DCMI_N_DIE_TEMP_ID，表示N_DIE温度；返回值对应输出联合体中的signed int iint成员；</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1898816193112"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1898816193112"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1898816193112"></a>14：DCMI_HBM_TEMP_ID，表示片上内存最高温度；返回值对应输出联合体中的signed int iint成员。</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p31089182199"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p31089182199"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p31089182199"></a>该场景支持11~14。</p>
<p id="p20346115351616"><a name="p20346115351616"></a><a name="p20346115351616"></a></p>
<div class="note" id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_note3722171515"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_note3722171515"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_note3722171515"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p44421721202418"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p44421721202418"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p44421721202418"></a>DCMI_FP_TEMP_ID:</p>
<a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_ul18432021162417"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_ul18432021162417"></a><ul id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_ul18432021162417"><li>不插光模块时，会返回-8008。</li><li>若插入光模块时，显示正常温度。</li><li>若插入铜缆时，不支持温度查询，返回0x7EFF。</li></ul>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_row677353102819"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p347295412284"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p347295412284"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p347295412284"></a>sensor_info</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1847275412814"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1847275412814"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1847275412814"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1472195462814"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1472195462814"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1472195462814"></a>union tag_sensor_info *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1747255462818"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1747255462818"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1747255462818"></a>返回温度传感器结构体信息：</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p647214545285"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p647214545285"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p647214545285"></a>union tag_sensor_info {</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1547245492816"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1547245492816"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p1547245492816"></a>unsigned char uchar;</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p144721454162810"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p144721454162810"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p144721454162810"></a>unsigned short ushort;</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p11472954122811"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p11472954122811"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p11472954122811"></a>unsigned int uint;</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p17472154202814"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p17472154202814"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p17472154202814"></a>signed int iint;</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p147214545286"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p147214545286"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p147214545286"></a>signed char temp[2];</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p347305472813"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p347305472813"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p347305472813"></a>signed int ntc_tmp[4];</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p14731754152814"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p14731754152814"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p14731754152814"></a>unsigned int data[16];</p>
<p id="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p19473105411283"><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p19473105411283"></a><a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_p19473105411283"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_toc533412082"></a>**

该接口在后续版本将会删除，推荐使用[dcmi\_get\_device\_sensor\_info](dcmi_get_device_sensor_info.md)。

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

**调用示例<a name="zh-cn_topic_0000001206147256_zh-cn_topic_0000001223172959_zh-cn_topic_0000001101799418_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 0x3;
int device_id = 0;
union tag_sensor_info sensor_info = {0};
int sensor_id = 1;
ret = dcmi_get_soc_sensor_info(card_id, device_id, sensor_id, &sensor_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

