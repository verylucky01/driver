# dcmi\_get\_device\_network\_health<a name="ZH-CN_TOPIC_0000002517535377"></a>

**函数原型<a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_toc533412077"></a>**

**int dcmi\_get\_device\_network\_health\(int card\_id, int device\_id, enum dcmi\_rdfx\_detect\_result \*result\)**

**功能说明<a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_toc533412078"></a>**

查询RoCE网卡的IP地址的连通状态。

**参数说明<a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_toc533412079"></a>**

<a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p10021890"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p10021890"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="15.02%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p6466753"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p6466753"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="17.98%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p54045009"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p54045009"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p15569626"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p15569626"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p36741947142813"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p36741947142813"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p96741747122818"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p96741747122818"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p46747472287"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p46747472287"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1467413474281"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1467413474281"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p7711145152918"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p7711145152918"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p671116522914"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p671116522914"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1771116572910"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1771116572910"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1233565810448"><a name="p1233565810448"></a><a name="p1233565810448"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p5522164215178"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p5522164215178"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p5522164215178"></a>result</p>
</td>
<td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p8522242101715"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p8522242101715"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p8522242101715"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="17.98%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p17522114220174"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p17522114220174"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p17522114220174"></a>enum dcmi_rdfx_detect_result *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1723415192311"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1723415192311"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1723415192311"></a>查询RoCE网卡的IP地址的连通状态，内容定义为：</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p18585114872215"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p18585114872215"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p18585114872215"></a>enum dcmi_rdfx_detect_result {</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p15585124832211"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p15585124832211"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p15585124832211"></a>DCMI_RDFX_DETECT_OK = 0, //网络健康状态正常</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p158564872216"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p158564872216"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p158564872216"></a>DCMI_RDFX_DETECT_SOCK_FAIL = 1, //网络套接字创建失败</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p18585134832215"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p18585134832215"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p18585134832215"></a>DCMI_RDFX_DETECT_RECV_TIMEOUT = 2, //网口收包超时</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p458564892211"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p458564892211"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p458564892211"></a>DCMI_RDFX_DETECT_UNREACH = 3, //侦测ip地址不可达</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p2585114832220"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p2585114832220"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p2585114832220"></a>DCMI_RDFX_DETECT_TIME_EXCEEDED = 4, //发送侦测报文执行超时</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p195855487228"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p195855487228"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p195855487228"></a>DCMI_RDFX_DETECT_FAULT = 5, //发送侦测报文失败</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p165853488222"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p165853488222"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p165853488222"></a>DCMI_RDFX_DETECT_INIT = 6, //侦测任务初始化中</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p16585134812224"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p16585134812224"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p16585134812224"></a>DCMI_RDFX_DETECT_THREAD_ERR = 7, //侦测任务创建失败</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p10585848112215"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p10585848112215"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p10585848112215"></a>DCMI_RDFX_DETECT_IP_SET = 8, //正在设置侦测ip地址</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1258564820224"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1258564820224"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1258564820224"></a>DCMI_RDFX_DETECT_MAX = 0xFF</p>
<p id="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1858511481225"><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1858511481225"></a><a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_p1858511481225"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table490921851817"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1723193692019"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p135791655203618"><a name="zh-cn_topic_0000002485295476_p135791655203618"></a><a name="zh-cn_topic_0000002485295476_p135791655203618"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p386754617313"><a name="zh-cn_topic_0000002485295476_p386754617313"></a><a name="zh-cn_topic_0000002485295476_p386754617313"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p11867346143119"><a name="zh-cn_topic_0000002485295476_p11867346143119"></a><a name="zh-cn_topic_0000002485295476_p11867346143119"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p178671246173119"><a name="zh-cn_topic_0000002485295476_p178671246173119"></a><a name="zh-cn_topic_0000002485295476_p178671246173119"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row187242365207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p11724836152019"><a name="zh-cn_topic_0000002485295476_p11724836152019"></a><a name="zh-cn_topic_0000002485295476_p11724836152019"></a><span id="zh-cn_topic_0000002485295476_ph20724336102017"><a name="zh-cn_topic_0000002485295476_ph20724336102017"></a><a name="zh-cn_topic_0000002485295476_ph20724336102017"></a><span id="zh-cn_topic_0000002485295476_text1672443611202"><a name="zh-cn_topic_0000002485295476_text1672443611202"></a><a name="zh-cn_topic_0000002485295476_text1672443611202"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p6724636152017"><a name="zh-cn_topic_0000002485295476_p6724636152017"></a><a name="zh-cn_topic_0000002485295476_p6724636152017"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p072493613204"><a name="zh-cn_topic_0000002485295476_p072493613204"></a><a name="zh-cn_topic_0000002485295476_p072493613204"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p15724113616209"><a name="zh-cn_topic_0000002485295476_p15724113616209"></a><a name="zh-cn_topic_0000002485295476_p15724113616209"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row0724133613207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p172433614201"><a name="zh-cn_topic_0000002485295476_p172433614201"></a><a name="zh-cn_topic_0000002485295476_p172433614201"></a><span id="zh-cn_topic_0000002485295476_text8724103642014"><a name="zh-cn_topic_0000002485295476_text8724103642014"></a><a name="zh-cn_topic_0000002485295476_text8724103642014"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p197241836152010"><a name="zh-cn_topic_0000002485295476_p197241836152010"></a><a name="zh-cn_topic_0000002485295476_p197241836152010"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p19724123672017"><a name="zh-cn_topic_0000002485295476_p19724123672017"></a><a name="zh-cn_topic_0000002485295476_p19724123672017"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1972453612205"><a name="zh-cn_topic_0000002485295476_p1972453612205"></a><a name="zh-cn_topic_0000002485295476_p1972453612205"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row77243362207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1472463611209"><a name="zh-cn_topic_0000002485295476_p1472463611209"></a><a name="zh-cn_topic_0000002485295476_p1472463611209"></a><span id="zh-cn_topic_0000002485295476_text1872453612205"><a name="zh-cn_topic_0000002485295476_text1872453612205"></a><a name="zh-cn_topic_0000002485295476_text1872453612205"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1972433616206"><a name="zh-cn_topic_0000002485295476_p1972433616206"></a><a name="zh-cn_topic_0000002485295476_p1972433616206"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p1072423612012"><a name="zh-cn_topic_0000002485295476_p1072423612012"></a><a name="zh-cn_topic_0000002485295476_p1072423612012"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1972463672019"><a name="zh-cn_topic_0000002485295476_p1972463672019"></a><a name="zh-cn_topic_0000002485295476_p1972463672019"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row17724133613204"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p67241036122013"><a name="zh-cn_topic_0000002485295476_p67241036122013"></a><a name="zh-cn_topic_0000002485295476_p67241036122013"></a><span id="zh-cn_topic_0000002485295476_text772414362202"><a name="zh-cn_topic_0000002485295476_text772414362202"></a><a name="zh-cn_topic_0000002485295476_text772414362202"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p772417367200"><a name="zh-cn_topic_0000002485295476_p772417367200"></a><a name="zh-cn_topic_0000002485295476_p772417367200"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p672443632016"><a name="zh-cn_topic_0000002485295476_p672443632016"></a><a name="zh-cn_topic_0000002485295476_p672443632016"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1872417364208"><a name="zh-cn_topic_0000002485295476_p1872417364208"></a><a name="zh-cn_topic_0000002485295476_p1872417364208"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1772493672010"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p272403610201"><a name="zh-cn_topic_0000002485295476_p272403610201"></a><a name="zh-cn_topic_0000002485295476_p272403610201"></a><span id="zh-cn_topic_0000002485295476_text12724153619206"><a name="zh-cn_topic_0000002485295476_text12724153619206"></a><a name="zh-cn_topic_0000002485295476_text12724153619206"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p127241736142014"><a name="zh-cn_topic_0000002485295476_p127241736142014"></a><a name="zh-cn_topic_0000002485295476_p127241736142014"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p5724636162015"><a name="zh-cn_topic_0000002485295476_p5724636162015"></a><a name="zh-cn_topic_0000002485295476_p5724636162015"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p6724143611201"><a name="zh-cn_topic_0000002485295476_p6724143611201"></a><a name="zh-cn_topic_0000002485295476_p6724143611201"></a>Y</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row47241236102018"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1272463632016"><a name="zh-cn_topic_0000002485295476_p1272463632016"></a><a name="zh-cn_topic_0000002485295476_p1272463632016"></a><span id="zh-cn_topic_0000002485295476_text13724123617201"><a name="zh-cn_topic_0000002485295476_text13724123617201"></a><a name="zh-cn_topic_0000002485295476_text13724123617201"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p43171355112010"><a name="zh-cn_topic_0000002485295476_p43171355112010"></a><a name="zh-cn_topic_0000002485295476_p43171355112010"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p0330115562011"><a name="zh-cn_topic_0000002485295476_p0330115562011"></a><a name="zh-cn_topic_0000002485295476_p0330115562011"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p193431559206"><a name="zh-cn_topic_0000002485295476_p193431559206"></a><a name="zh-cn_topic_0000002485295476_p193431559206"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row197254368207"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p6725133615205"><a name="zh-cn_topic_0000002485295476_p6725133615205"></a><a name="zh-cn_topic_0000002485295476_p6725133615205"></a><span id="zh-cn_topic_0000002485295476_text17256369202"><a name="zh-cn_topic_0000002485295476_text17256369202"></a><a name="zh-cn_topic_0000002485295476_text17256369202"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1735585510205"><a name="zh-cn_topic_0000002485295476_p1735585510205"></a><a name="zh-cn_topic_0000002485295476_p1735585510205"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p123731555122014"><a name="zh-cn_topic_0000002485295476_p123731555122014"></a><a name="zh-cn_topic_0000002485295476_p123731555122014"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p3391255152012"><a name="zh-cn_topic_0000002485295476_p3391255152012"></a><a name="zh-cn_topic_0000002485295476_p3391255152012"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row15725536162012"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p6725193618208"><a name="zh-cn_topic_0000002485295476_p6725193618208"></a><a name="zh-cn_topic_0000002485295476_p6725193618208"></a><span id="zh-cn_topic_0000002485295476_ph66771792553"><a name="zh-cn_topic_0000002485295476_ph66771792553"></a><a name="zh-cn_topic_0000002485295476_ph66771792553"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

在调用**dcmi\_get\_device\_network\_health**接口前，需在Host侧以root用户执行如下命令设置IP地址：

-   配置RoCE网卡的IP地址、子网掩码

    ```
    hccn_tool -i devid -ip -s address %s netmask %s
    ```

    您需要根据实际情况，修改如下内容：

    -   _devid_  ：设置为设备ID。
    -   **address**后的_%s_：设置为RoCE网卡的IP地址。
    -   **netmask**后的_%s_：设置为子网掩码。

-   配置用于检测RoCE网卡的IP地址是否连通的IP地址

    ```
    hccn_tool -i devid -netdetect -s address %s
    ```

    您需要根据实际情况，修改如下内容：

    -   _devid_  ：设置为设备ID。
    -   **address**后的_%s_：设置为用于检测RoCE网卡的IP地址是否连通的IP地址，例如路由器的IP地址。

**调用示例<a name="zh-cn_topic_0000001206147228_zh-cn_topic_0000001178213206_zh-cn_topic_0000001101611602_toc533412083"></a>**

```
… 
int ret = 0;
enum dcmi_rdfx_detect_result health = DCMI_RDFX_DETECT_MAX;
int card_id = 0;
int device_id = 0;
ret = dcmi_get_device_network_health(card_id, device_id, &health);
…
```

