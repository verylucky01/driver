# dcmi\_get\_device\_ecc\_info<a name="ZH-CN_TOPIC_0000002485318720"></a>

**函数原型<a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_toc533412077"></a>**

**int dcmi\_get\_device\_ecc\_info\(int card\_id, int device\_id, enum dcmi\_device\_type input\_type, struct dcmi\_ecc\_info \*device\_ecc\_info\)**

**功能说明<a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_toc533412078"></a>**

获取ECC信息。

**参数说明<a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_toc533412079"></a>**

<a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_table10480683"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_row7580267"><th class="cellrowborder" valign="top" width="17%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p10021890"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p10021890"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p10021890"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="16.99%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p6466753"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p6466753"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p6466753"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.009999999999998%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p54045009"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p54045009"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p54045009"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p15569626"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p15569626"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p15569626"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_row10560021192510"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p36741947142813"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p36741947142813"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p36741947142813"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p96741747122818"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p96741747122818"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p96741747122818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p46747472287"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p46747472287"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p46747472287"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1467413474281"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1467413474281"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1467413474281"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_row1155711494235"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p7711145152918"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p7711145152918"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p7711145152918"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p671116522914"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p671116522914"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p671116522914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1771116572910"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1771116572910"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1771116572910"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001148530297_p11747451997"><a name="zh-cn_topic_0000001148530297_p11747451997"></a><a name="zh-cn_topic_0000001148530297_p11747451997"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="zh-cn_topic_0000001148530297_p1377514432141"><a name="zh-cn_topic_0000001148530297_p1377514432141"></a><a name="zh-cn_topic_0000001148530297_p1377514432141"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p86201271359"><a name="p86201271359"></a><a name="p86201271359"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_row15462171542913"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p237363715296"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p237363715296"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p237363715296"></a>device_type</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1837312378299"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1837312378299"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1837312378299"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p637373712920"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p637373712920"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p637373712920"></a>enum dcmi_device_type</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1837317370296"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1837317370296"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1837317370296"></a>组件类型</p>
<p id="p36503314442"><a name="p36503314442"></a><a name="p36503314442"></a>enum dcmi_device_type {</p>
<p id="p14651233204415"><a name="p14651233204415"></a><a name="p14651233204415"></a>DCMI_DEVICE_TYPE_DDR,</p>
<p id="p176511336449"><a name="p176511336449"></a><a name="p176511336449"></a>DCMI_DEVICE_TYPE_SRAM,</p>
<p id="p36583324415"><a name="p36583324415"></a><a name="p36583324415"></a>DCMI_DEVICE_TYPE_HBM,</p>
<p id="p186573334416"><a name="p186573334416"></a><a name="p186573334416"></a>DCMI_DEVICE_TYPE_NPU,</p>
<p id="p065113334417"><a name="p065113334417"></a><a name="p065113334417"></a>DCMI_HBM_RECORDED_SINGLE_ADDR,</p>
<p id="p86513338448"><a name="p86513338448"></a><a name="p86513338448"></a>DCMI_HBM_RECORDED_MULTI_ADDR,</p>
<p id="p1065173364414"><a name="p1065173364414"></a><a name="p1065173364414"></a>DCMI_DEVICE_TYPE_NONE = 0xff</p>
<p id="p1165133174413"><a name="p1165133174413"></a><a name="p1165133174413"></a>};</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p795817279399"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p795817279399"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p795817279399"></a>仅支持DCMI_DEVICE_TYPE_HBM //片上内存类型内存。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_row1019163316298"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p53731537152910"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p53731537152910"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p53731537152910"></a>device_ecc_info</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p163731137102919"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p163731137102919"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p163731137102919"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p037353718298"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p037353718298"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p037353718298"></a>struct dcmi_ecc_info *</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p193731837202917"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p193731837202917"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p193731837202917"></a>返回ECC结构体信息：</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p5917537103018"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p5917537103018"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p5917537103018"></a>struct dcmi_ecc_info {</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1691723712309"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1691723712309"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1691723712309"></a>int enable_flag;</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p991753720308"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p991753720308"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p991753720308"></a>unsigned int single_bit_error_cnt; //单比特错误数量</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1591710377309"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1591710377309"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1591710377309"></a>unsigned int double_bit_error_cnt; //多比特错误数量</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p791718375301"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p791718375301"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p791718375301"></a>unsigned int total_single_bit_error_cnt; //生命周期内所有单比特ECC错误统计</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p5917337123010"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p5917337123010"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p5917337123010"></a>unsigned int total_double_bit_error_cnt; //生命周期内所有多比特ECC错误统计</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p491713733013"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p491713733013"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p491713733013"></a>unsigned int single_bit_isolated_pages_cnt; //单比特错误隔离内存页数量</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1291712371309"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1291712371309"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p1291712371309"></a>unsigned int double_bit_isolated_pages_cnt; //多比特错误隔离内存页数量</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p691743733013"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p691743733013"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p691743733013"></a>};</p>
<p id="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p6300629155519"><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p6300629155519"></a><a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_p6300629155519"></a>其中，enable_flag输出0，表示ECC检测未使能；enable_flag输出1，表示ECC检测使能。</p>
<p id="p145491101150"><a name="p145491101150"></a><a name="p145491101150"></a></p>
<div class="note" id="note82336295217"><a name="note82336295217"></a><a name="note82336295217"></a><span class="notetitle"> 说明： </span><div class="notebody"><a name="ul145976102198"></a><a name="ul145976102198"></a><ul id="ul145976102198"><li>当前支持单比特和多比特错误查询，如果查询到多比特错误，需要进行相关问题定位和处理。</li><li>该接口获取的是实时的数据，系统重启之后单比特错误数量和多比特错误数量清零。</li><li>single_bit_isolated_pages_cnt为预留参数，暂不使用，固定输出0。</li></ul>
</div></div>
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

**约束说明<a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_toc533412082"></a>**

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

**调用示例<a name="zh-cn_topic_0000001251427189_zh-cn_topic_0000001223414429_zh-cn_topic_0000001147924553_toc533412083"></a>**

```
… 
int ret = 0;
int card_id = 1;
int device_id = 0;
struct dcmi_ecc_info device_ecc_info = {0};
ret = dcmi_get_device_ecc_info(card_id, device_id, DCMI_DEVICE_TYPE_DDR, &device_ecc_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

