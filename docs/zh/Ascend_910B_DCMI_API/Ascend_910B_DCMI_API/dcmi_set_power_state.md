# dcmi\_set\_power\_state<a name="ZH-CN_TOPIC_0000002517615409"></a>

**函数原型<a name="zh-cn_topic_0000001532160665_zh-cn_topic_0251903700_section204121018181415"></a>**

**int dcmi\_set\_power\_state\(int card\_id, int device\_id, struct dcmi\_power\_state\_info\_stru power\_info\)**

**功能说明<a name="zh-cn_topic_0000001532160665_zh-cn_topic_0251903700_section10486629111410"></a>**

设置系统模式。

**参数说明<a name="zh-cn_topic_0000001532160665_zh-cn_topic_0251903700_section1126553991413"></a>**

<a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.99%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.009999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row67890141240"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"></a><a name="zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p208971318134613"><a name="p208971318134613"></a><a name="p208971318134613"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001532160665_p9315182755815"><a name="zh-cn_topic_0000001532160665_p9315182755815"></a><a name="zh-cn_topic_0000001532160665_p9315182755815"></a>type</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p1817571618112"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"></a><a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001251427211_zh-cn_topic_0000001178373160_zh-cn_topic_0000001101290912_p20175191611119"></a>struct dcmi_power_state_info_stru</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p2680123615336"><a name="p2680123615336"></a><a name="p2680123615336"></a>设置系统状态参数。</p>
<p id="p7279113417175"><a name="p7279113417175"></a><a name="p7279113417175"></a>typedef enum {</p>
<p id="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p49221225161714"><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p49221225161714"></a><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p49221225161714"></a>DCMI_POWER_STATE_SUSPEND, //暂停</p>
<p id="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p7922192510177"><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p7922192510177"></a><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p7922192510177"></a>DCMI_POWER_STATE_POWEROFF, //下电</p>
<p id="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p1392216255179"><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p1392216255179"></a><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p1392216255179"></a>DCMI_POWER_STATE_RESET, //复位</p>
<p id="p1892262581711"><a name="p1892262581711"></a><a name="p1892262581711"></a>DCMI_POWER_STATE_MAX,</p>
<p id="p49221125171711"><a name="p49221125171711"></a><a name="p49221125171711"></a>} DCMI_POWER_STATE;</p>
<p id="p69221425171710"><a name="p69221425171710"></a><a name="p69221425171710"></a></p>
<p id="p19336184841719"><a name="p19336184841719"></a><a name="p19336184841719"></a>typedef enum {</p>
<p id="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p1192222517179"><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p1192222517179"></a><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p1192222517179"></a>DCMI_POWER_RESUME_MODE_BUTTON,  //电源恢复模式按钮</p>
<p id="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p9922112571710"><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p9922112571710"></a><a name="zh-cn_topic_0000001548668158_zh-cn_topic_0000001548828062_p9922112571710"></a>DCMI_POWER_RESUME_MODE_TIME,  //系统按时间恢复</p>
<p id="p6922925191713"><a name="p6922925191713"></a><a name="p6922925191713"></a>DCMI_POWER_RESUME_MODE_MAX,</p>
<p id="p1492219258178"><a name="p1492219258178"></a><a name="p1492219258178"></a>} DCMI_LP_RESUME_MODE;</p>
<p id="p179222259171"><a name="p179222259171"></a><a name="p179222259171"></a></p>
<p id="p292216258173"><a name="p292216258173"></a><a name="p292216258173"></a>#define DCMI_POWER_INFO_RESERVE_LEN 8</p>
<p id="p59221125181714"><a name="p59221125181714"></a><a name="p59221125181714"></a>struct dcmi_power_state_info_stru {</p>
<p id="p59221325111718"><a name="p59221325111718"></a><a name="p59221325111718"></a>DCMI_POWER_STATE type;//休眠唤醒状态</p>
<p id="p10922142519172"><a name="p10922142519172"></a><a name="p10922142519172"></a>DCMI_LP_RESUME_MODE mode;//休眠唤醒模式</p>
<p id="p119229251172"><a name="p119229251172"></a><a name="p119229251172"></a>unsigned int value;</p>
<p id="p592216259177"><a name="p592216259177"></a><a name="p592216259177"></a>unsigned int reserve[DCMI_POWER_INFO_RESERVE_LEN];//保留</p>
<p id="p1092262511718"><a name="p1092262511718"></a><a name="p1092262511718"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001206307232_zh-cn_topic_0000001223414433_zh-cn_topic_0000001148284427_toc533412082"></a>**

命令执行后设备立即进入休眠状态，休眠时间结束后自动唤醒。

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

**调用示例<a name="zh-cn_topic_0000001532160665_zh-cn_topic_0251903700_section1130783912517"></a>**

```
...
    int ret;
    int card_id = 0;
    int device_id = 0;
    struct dcmi_power_state_info_stru power_info = {0};

    power_info.type = DCMI_POWER_STATE_SUSPEND;
    power_info.mode = DCMI_POWER_RESUME_MODE_TIME;
    power_info.value = 300;// 300ms

    ret = dcmi_set_power_state(card_id, device_id, power_info);
    if (ret) {
        // todo
    }
    // todo
...
```

