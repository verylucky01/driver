# dcmi\_set\_power\_state<a name="ZH-CN_TOPIC_0000002517558695"></a>

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
<p id="p101061324171816"><a name="p101061324171816"></a><a name="p101061324171816"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
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
<p id="p49221225161714"><a name="p49221225161714"></a><a name="p49221225161714"></a>DCMI_POWER_STATE_SUSPEND,//暂停</p>
<p id="p7922192510177"><a name="p7922192510177"></a><a name="p7922192510177"></a>DCMI_POWER_STATE_POWEROFF,//下电</p>
<p id="p1392216255179"><a name="p1392216255179"></a><a name="p1392216255179"></a>DCMI_POWER_STATE_RESET,//复位</p>
<p id="p1892262581711"><a name="p1892262581711"></a><a name="p1892262581711"></a>DCMI_POWER_STATE_MAX,</p>
<p id="p49221125171711"><a name="p49221125171711"></a><a name="p49221125171711"></a>} DCMI_POWER_STATE;</p>
<p id="p69221425171710"><a name="p69221425171710"></a><a name="p69221425171710"></a></p>
<p id="p19336184841719"><a name="p19336184841719"></a><a name="p19336184841719"></a>typedef enum {</p>
<p id="p1192222517179"><a name="p1192222517179"></a><a name="p1192222517179"></a>DCMI_POWER_RESUME_MODE_BUTTON, //电源恢复模式按钮</p>
<p id="p9922112571710"><a name="p9922112571710"></a><a name="p9922112571710"></a>DCMI_POWER_RESUME_MODE_TIME, //系统按时间恢复</p>
<p id="p6922925191713"><a name="p6922925191713"></a><a name="p6922925191713"></a>DCMI_POWER_RESUME_MODE_MAX,</p>
<p id="p1492219258178"><a name="p1492219258178"></a><a name="p1492219258178"></a>} DCMI_LP_RESUME_MODE;</p>
<p id="p179222259171"><a name="p179222259171"></a><a name="p179222259171"></a></p>
<p id="p292216258173"><a name="p292216258173"></a><a name="p292216258173"></a>#define DCMI_POWER_INFO_RESERVE_LEN 8</p>
<p id="p59221125181714"><a name="p59221125181714"></a><a name="p59221125181714"></a>struct dcmi_power_state_info_stru {</p>
<p id="p59221325111718"><a name="p59221325111718"></a><a name="p59221325111718"></a>DCMI_POWER_STATE type; //休眠唤醒状态</p>
<p id="p10922142519172"><a name="p10922142519172"></a><a name="p10922142519172"></a>DCMI_LP_RESUME_MODE mode; //休眠唤醒模式</p>
<p id="p119229251172"><a name="p119229251172"></a><a name="p119229251172"></a>unsigned int value;</p>
<p id="p592216259177"><a name="p592216259177"></a><a name="p592216259177"></a>unsigned int reserve[DCMI_POWER_INFO_RESERVE_LEN];//保留</p>
<p id="p1092262511718"><a name="p1092262511718"></a><a name="p1092262511718"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001532160665_zh-cn_topic_0000001206307232_zh-cn_topic_0000001223414433_zh-cn_topic_0000001148284427_toc533412082"></a>**

命令执行后设备立即进入休眠状态，休眠时间结束后自动唤醒。

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

