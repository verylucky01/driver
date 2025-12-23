# dcmi\_prbs\_operate<a name="ZH-CN_TOPIC_0000002485295436"></a>

**函数原型<a name="section428918818434"></a>**

**int dcmi\_prbs\_operate\(int card\_id, int device\_id, struct dcmi\_prbs\_operate\_param operate\_para, struct dcmi\_prbs\_operate\_result \*operate\_result\)**

**功能说明<a name="section162912812435"></a>**

对昇腾NPU芯片打流和获取打流结果。

**参数说明<a name="section529358154316"></a>**

<a name="table6390285439"></a>
<table><thead align="left"><tr id="row15504689437"><th class="cellrowborder" valign="top" width="17.169999999999998%" id="mcps1.1.5.1.1"><p id="p6504198204312"><a name="p6504198204312"></a><a name="p6504198204312"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="11.92%" id="mcps1.1.5.1.2"><p id="p45041818432"><a name="p45041818432"></a><a name="p45041818432"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="16.220000000000002%" id="mcps1.1.5.1.3"><p id="p1450419814310"><a name="p1450419814310"></a><a name="p1450419814310"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="54.690000000000005%" id="mcps1.1.5.1.4"><p id="p45046812433"><a name="p45046812433"></a><a name="p45046812433"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row185041987434"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p1750511894313"><a name="p1750511894313"></a><a name="p1750511894313"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="11.92%" headers="mcps1.1.5.1.2 "><p id="p65056816436"><a name="p65056816436"></a><a name="p65056816436"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.220000000000002%" headers="mcps1.1.5.1.3 "><p id="p125055834311"><a name="p125055834311"></a><a name="p125055834311"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.690000000000005%" headers="mcps1.1.5.1.4 "><p id="p14505788439"><a name="p14505788439"></a><a name="p14505788439"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row105051780432"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p2505581438"><a name="p2505581438"></a><a name="p2505581438"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="11.92%" headers="mcps1.1.5.1.2 "><p id="p6505198134310"><a name="p6505198134310"></a><a name="p6505198134310"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.220000000000002%" headers="mcps1.1.5.1.3 "><p id="p950510814437"><a name="p950510814437"></a><a name="p950510814437"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="54.690000000000005%" headers="mcps1.1.5.1.4 "><p id="p55058810433"><a name="p55058810433"></a><a name="p55058810433"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p155061289436"><a name="p155061289436"></a><a name="p155061289436"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p183711190476"><a name="p183711190476"></a><a name="p183711190476"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517535283_p10218227151413"><a name="zh-cn_topic_0000002517535283_p10218227151413"></a><a name="zh-cn_topic_0000002517535283_p10218227151413"></a>device_id_max值为1，当device_id为0时表示NPU芯片；当device_id为1时表示MCU芯片。</p>
</div></div>
</td>
</tr>
<tr id="row1450638194317"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p16506188124320"><a name="p16506188124320"></a><a name="p16506188124320"></a>operate_para</p>
</td>
<td class="cellrowborder" valign="top" width="11.92%" headers="mcps1.1.5.1.2 "><p id="p125061184431"><a name="p125061184431"></a><a name="p125061184431"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.220000000000002%" headers="mcps1.1.5.1.3 "><p id="p9506158134317"><a name="p9506158134317"></a><a name="p9506158134317"></a>DCMI_PRBS_OPERATE_PARAM</p>
</td>
<td class="cellrowborder" valign="top" width="54.690000000000005%" headers="mcps1.1.5.1.4 "><p id="p1250688114319"><a name="p1250688114319"></a><a name="p1250688114319"></a>prbs码流操作入参，通过设置以下结构体中的参数区分打流和查询打流结果。</p>
<p id="p101001532182"><a name="p101001532182"></a><a name="p101001532182"></a>typedef struct dcmi_serdes_prbs_param_base {</p>
<p id="p1910017313185"><a name="p1910017313185"></a><a name="p1910017313185"></a>unsigned int serdes_prbs_macro_id; //仅支持macro_0打流</p>
<p id="p181004341818"><a name="p181004341818"></a><a name="p181004341818"></a>unsigned int serdes_prbs_start_lane_id;//lane的开始ID，最小值为0，最大值为3</p>
<p id="p111006311183"><a name="p111006311183"></a><a name="p111006311183"></a>unsigned int serdes_prbs_lane_count;//lane的数量，最小值1，最大值为4</p>
<p id="p310033151818"><a name="p310033151818"></a><a name="p310033151818"></a>} DCMI_SERDES_PRBS_PARAM_BASE;</p>
<p id="p1510018391816"><a name="p1510018391816"></a><a name="p1510018391816"></a>typedef DCMI_SERDES_PRBS_PARAM_BASE DCMI_SERDES_PRBS_GET_PARAM;  // 获取打流结果时仅需传入基础信息param base，具体获取类型通过sub_cmd区分</p>
<p id="p110016316188"><a name="p110016316188"></a><a name="p110016316188"></a>typedef struct dcmi_serdes_prbs_set_param {</p>
<p id="p9100173131817"><a name="p9100173131817"></a><a name="p9100173131817"></a>DCMI_SERDES_PRBS_PARAM_BASE param_base;//打流设置基础参数</p>
<p id="p161001335180"><a name="p161001335180"></a><a name="p161001335180"></a>unsigned int serdes_prbs_type;//打流的码型，参考prbs码型枚举</p>
<p id="p15100734183"><a name="p15100734183"></a><a name="p15100734183"></a>unsigned int serdes_prbs_direction;//打流的方向，参考prbs打流方向枚举</p>
<p id="p1210063131814"><a name="p1210063131814"></a><a name="p1210063131814"></a>} DCMI_SERDES_PRBS_SET_PARAM;</p>
<p id="p1610073161819"><a name="p1610073161819"></a><a name="p1610073161819"></a>typedef struct dcmi_prbs_operate_param {</p>
<p id="p1810063111816"><a name="p1810063111816"></a><a name="p1810063111816"></a>unsigned int main_cmd;//打流的命令，参考主命令字枚举</p>
<p id="p1510017361810"><a name="p1510017361810"></a><a name="p1510017361810"></a>unsigned int sub_cmd; // 标识是设置打流命令还是查询打流结果命令</p>
<p id="p1610015371810"><a name="p1610015371810"></a><a name="p1610015371810"></a>union {</p>
<p id="p5100335189"><a name="p5100335189"></a><a name="p5100335189"></a>DCMI_SERDES_PRBS_SET_PARAM set_param;//设置打流参数</p>
<p id="p12100037189"><a name="p12100037189"></a><a name="p12100037189"></a>DCMI_SERDES_PRBS_GET_PARAM get_param;//查询打流结果</p>
<p id="p0100133171811"><a name="p0100133171811"></a><a name="p0100133171811"></a>} operate_para;</p>
<p id="p18100103121813"><a name="p18100103121813"></a><a name="p18100103121813"></a>} DCMI_PRBS_OPERATE_PARAM;</p>
<p id="p195049239555"><a name="p195049239555"></a><a name="p195049239555"></a>// 主命令字枚举</p>
<p id="p155041623115511"><a name="p155041623115511"></a><a name="p155041623115511"></a>enum dcmi_prbs_main_cmd_list {</p>
<p id="p850402375512"><a name="p850402375512"></a><a name="p850402375512"></a>DSMI_SERDES_CMD_PRBS = 0,//打流的命令</p>
<p id="p350432375520"><a name="p350432375520"></a><a name="p350432375520"></a>DSMI_SERDES_CMD_MAX</p>
<p id="p155044232552"><a name="p155044232552"></a><a name="p155044232552"></a>};</p>
<p id="p15504172316553"><a name="p15504172316553"></a><a name="p15504172316553"></a>// 子命令字枚举</p>
<p id="p115051623165515"><a name="p115051623165515"></a><a name="p115051623165515"></a>enum dcmi_prbs_sub_cmd_list {</p>
<p id="p1250592345518"><a name="p1250592345518"></a><a name="p1250592345518"></a>SERDES_PRBS_SET_CMD = 0,//设置打流的子命令</p>
<p id="p8505172365516"><a name="p8505172365516"></a><a name="p8505172365516"></a>SERDES_PRBS_GET_RESULT_CMD, // 查结果</p>
<p id="p10505112315511"><a name="p10505112315511"></a><a name="p10505112315511"></a>SERDES_PRBS_GET_STATUS_CMD, // 查码型</p>
<p id="p10505523185516"><a name="p10505523185516"></a><a name="p10505523185516"></a>SERDES_PRBS_SUB_CMD_MAX</p>
<p id="p185051723165511"><a name="p185051723165511"></a><a name="p185051723165511"></a>};</p>
<p id="p950562311553"><a name="p950562311553"></a><a name="p950562311553"></a>// prbs码型枚举</p>
<p id="p550511235552"><a name="p550511235552"></a><a name="p550511235552"></a>enum dcmi_serdes_prbs_type_list {</p>
<p id="p1450513238551"><a name="p1450513238551"></a><a name="p1450513238551"></a>SERDES_PRBS_TYPE_END = 0,</p>
<p id="p12505923195516"><a name="p12505923195516"></a><a name="p12505923195516"></a>SERDES_PRBS_TYPE_31 = 8,</p>
<p id="p750582313556"><a name="p750582313556"></a><a name="p750582313556"></a>SERDES_PRBS_TYPE_MAX</p>
<p id="p9505123105513"><a name="p9505123105513"></a><a name="p9505123105513"></a>};</p>
<p id="p0505112385510"><a name="p0505112385510"></a><a name="p0505112385510"></a>// prbs打流方向枚举</p>
<p id="p1350532345511"><a name="p1350532345511"></a><a name="p1350532345511"></a>enum dcmi_serdes_prbs_direction {</p>
<p id="p135051623195512"><a name="p135051623195512"></a><a name="p135051623195512"></a>SERDES_PRBS_DIRECTION_TX = 0,</p>
<p id="p15505423115519"><a name="p15505423115519"></a><a name="p15505423115519"></a>SERDES_PRBS_DIRECTION_RX,</p>
<p id="p3505152325510"><a name="p3505152325510"></a><a name="p3505152325510"></a>SERDES_PRBS_DIRECTION_TXRX,</p>
<p id="p45051523165511"><a name="p45051523165511"></a><a name="p45051523165511"></a>SERDES_PRBS_DIRECTION_MAX</p>
<p id="p1750552316555"><a name="p1750552316555"></a><a name="p1750552316555"></a>};</p>
</td>
</tr>
<tr id="row85061087433"><td class="cellrowborder" valign="top" width="17.169999999999998%" headers="mcps1.1.5.1.1 "><p id="p11507178174313"><a name="p11507178174313"></a><a name="p11507178174313"></a>operate_result</p>
</td>
<td class="cellrowborder" valign="top" width="11.92%" headers="mcps1.1.5.1.2 "><p id="p115071088431"><a name="p115071088431"></a><a name="p115071088431"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.220000000000002%" headers="mcps1.1.5.1.3 "><p id="p9297132912213"><a name="p9297132912213"></a><a name="p9297132912213"></a>DCMI_PRBS_OPERATE_RESULT</p>
</td>
<td class="cellrowborder" valign="top" width="54.690000000000005%" headers="mcps1.1.5.1.4 "><p id="p175071983435"><a name="p175071983435"></a><a name="p175071983435"></a>查询打流结果或查询prbs链路状态。</p>
<p id="p179734420424"><a name="p179734420424"></a><a name="p179734420424"></a>#define MAX_LANE_NUM 8</p>
<p id="p829314942119"><a name="p829314942119"></a><a name="p829314942119"></a>typedef struct dcmi_prbs_operate_result {</p>
<p id="p229339192111"><a name="p229339192111"></a><a name="p229339192111"></a>union {</p>
<p id="p32933992112"><a name="p32933992112"></a><a name="p32933992112"></a>SERDES_PRBS_STATUS_S result[MAX_LANE_NUM];//打流返回结果</p>
<p id="p329311913218"><a name="p329311913218"></a><a name="p329311913218"></a>DCMI_SERDES_PRBS_LANE_STATUS lane_status[MAX_LANE_NUM];//lane的状态</p>
<p id="p9294159182117"><a name="p9294159182117"></a><a name="p9294159182117"></a>} prbs_result;</p>
<p id="p529416942111"><a name="p529416942111"></a><a name="p529416942111"></a>} DCMI_PRBS_OPERATE_RESULT;</p>
<p id="p6766454154411"><a name="p6766454154411"></a><a name="p6766454154411"></a>struct dcmi_serdes_prbs_lane_status {</p>
<p id="p676675414449"><a name="p676675414449"></a><a name="p676675414449"></a>unsigned int lane_prbs_tx_status;//tx方向打流码型</p>
<p id="p87661054174413"><a name="p87661054174413"></a><a name="p87661054174413"></a>unsigned int lane_prbs_rx_status;//rx方向打流码型</p>
<p id="p8766195410445"><a name="p8766195410445"></a><a name="p8766195410445"></a>};</p>
<p id="p197666547445"><a name="p197666547445"></a><a name="p197666547445"></a>typedef struct {</p>
<p id="p37669545447"><a name="p37669545447"></a><a name="p37669545447"></a>unsigned int check_en;//查询打流是否打开</p>
<p id="p6766155434417"><a name="p6766155434417"></a><a name="p6766155434417"></a>unsigned int check_type;//查询打流码型</p>
<p id="p1576613547449"><a name="p1576613547449"></a><a name="p1576613547449"></a>unsigned int error_status;//查询打流状态</p>
<p id="p167667547444"><a name="p167667547444"></a><a name="p167667547444"></a>unsigned int error_cnt;//错误的码型数量统计</p>
<p id="p12766135417441"><a name="p12766135417441"></a><a name="p12766135417441"></a>unsigned long error_rate; //打流错误率的倒数，1/error_rate的值小于10<sup id="sup11750113155412"><a name="sup11750113155412"></a><a name="sup11750113155412"></a>-5</sup>为正常</p>
<p id="p18766454114416"><a name="p18766454114416"></a><a name="p18766454114416"></a>unsigned int alos_status; //输入的信号幅度，0表示正常，1表示过低</p>
<p id="p87662054174420"><a name="p87662054174420"></a><a name="p87662054174420"></a>unsigned long time_val;//设置打流和查询打流结果之间的时间间隔</p>
<p id="p167661554164418"><a name="p167661554164418"></a><a name="p167661554164418"></a>} SERDES_PRBS_STATUS_S;</p>
</td>
</tr>
</tbody>
</table>

**返回值说明<a name="section3642631135017"></a>**

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

**异常处理<a name="section1464620315507"></a>**

无。

**约束说明<a name="section19396154863610"></a>**

-   打流前请关闭所有运行的业务。
-   对于Atlas 200T A2 Box16 异构子框、Atlas 800T A2 训练服务器、Atlas 800I A2 推理服务器、Atlas 900 A2 PoD 集群基础单元、A200I A2 Box 异构组件，该接口支持在物理机+特权容器场景下使用。
-   driver包安装或NPU复位之后需要等待120s之后才能做打流动作。
-   打流动作会使NPU网口down，会产生BMC告警；若后续不关闭打流动作，则网口无法up。
-   打流功能支持光模块环回（光模块需要有环回能力）、光模块外接自环头环回、CDR环回场景。
-   Atlas 900 A2 PoD 集群基础单元不支持prbs打流，Atlas 900 A2 PoDc 集群基础单元支持prbs打流。

**表 1** 不同部署场景下的支持情况

<a name="table6665182042413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485295476_row1514515016392"><th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485295476_p15286130203213"><a name="zh-cn_topic_0000002485295476_p15286130203213"></a><a name="zh-cn_topic_0000002485295476_p15286130203213"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="18.94%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485295476_p132861730113218"><a name="zh-cn_topic_0000002485295476_p132861730113218"></a><a name="zh-cn_topic_0000002485295476_p132861730113218"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485295476_p22861530183212"><a name="zh-cn_topic_0000002485295476_p22861530183212"></a><a name="zh-cn_topic_0000002485295476_p22861530183212"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485295476_p1928683003217"><a name="zh-cn_topic_0000002485295476_p1928683003217"></a><a name="zh-cn_topic_0000002485295476_p1928683003217"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485295476_row514512014397"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p14145100183919"><a name="zh-cn_topic_0000002485295476_p14145100183919"></a><a name="zh-cn_topic_0000002485295476_p14145100183919"></a><span id="zh-cn_topic_0000002485295476_ph15145206394"><a name="zh-cn_topic_0000002485295476_ph15145206394"></a><a name="zh-cn_topic_0000002485295476_ph15145206394"></a><span id="zh-cn_topic_0000002485295476_text141451204393"><a name="zh-cn_topic_0000002485295476_text141451204393"></a><a name="zh-cn_topic_0000002485295476_text141451204393"></a>Atlas 900 A2 PoD 集群基础单元</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p114519073913"><a name="zh-cn_topic_0000002485295476_p114519073913"></a><a name="zh-cn_topic_0000002485295476_p114519073913"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p672045395"><a name="zh-cn_topic_0000002485295476_p672045395"></a><a name="zh-cn_topic_0000002485295476_p672045395"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p14102046396"><a name="zh-cn_topic_0000002485295476_p14102046396"></a><a name="zh-cn_topic_0000002485295476_p14102046396"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1314590133918"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p914590143915"><a name="zh-cn_topic_0000002485295476_p914590143915"></a><a name="zh-cn_topic_0000002485295476_p914590143915"></a><span id="zh-cn_topic_0000002485295476_text151456017398"><a name="zh-cn_topic_0000002485295476_text151456017398"></a><a name="zh-cn_topic_0000002485295476_text151456017398"></a>Atlas 800T A2 训练服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p114512012392"><a name="zh-cn_topic_0000002485295476_p114512012392"></a><a name="zh-cn_topic_0000002485295476_p114512012392"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p161319419393"><a name="zh-cn_topic_0000002485295476_p161319419393"></a><a name="zh-cn_topic_0000002485295476_p161319419393"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p1019184143917"><a name="zh-cn_topic_0000002485295476_p1019184143917"></a><a name="zh-cn_topic_0000002485295476_p1019184143917"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1814514011392"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p414590193920"><a name="zh-cn_topic_0000002485295476_p414590193920"></a><a name="zh-cn_topic_0000002485295476_p414590193920"></a><span id="zh-cn_topic_0000002485295476_text161451704394"><a name="zh-cn_topic_0000002485295476_text161451704394"></a><a name="zh-cn_topic_0000002485295476_text161451704394"></a>Atlas 800I A2 推理服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1514516018392"><a name="zh-cn_topic_0000002485295476_p1514516018392"></a><a name="zh-cn_topic_0000002485295476_p1514516018392"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p18242403910"><a name="zh-cn_topic_0000002485295476_p18242403910"></a><a name="zh-cn_topic_0000002485295476_p18242403910"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p18307415390"><a name="zh-cn_topic_0000002485295476_p18307415390"></a><a name="zh-cn_topic_0000002485295476_p18307415390"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1114514093913"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1145407399"><a name="zh-cn_topic_0000002485295476_p1145407399"></a><a name="zh-cn_topic_0000002485295476_p1145407399"></a><span id="zh-cn_topic_0000002485295476_text161451007395"><a name="zh-cn_topic_0000002485295476_text161451007395"></a><a name="zh-cn_topic_0000002485295476_text161451007395"></a>Atlas 200T A2 Box16 异构子框</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p314518015391"><a name="zh-cn_topic_0000002485295476_p314518015391"></a><a name="zh-cn_topic_0000002485295476_p314518015391"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p13378413911"><a name="zh-cn_topic_0000002485295476_p13378413911"></a><a name="zh-cn_topic_0000002485295476_p13378413911"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p8431344398"><a name="zh-cn_topic_0000002485295476_p8431344398"></a><a name="zh-cn_topic_0000002485295476_p8431344398"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row1114514017397"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p1114510163910"><a name="zh-cn_topic_0000002485295476_p1114510163910"></a><a name="zh-cn_topic_0000002485295476_p1114510163910"></a><span id="zh-cn_topic_0000002485295476_text01466093919"><a name="zh-cn_topic_0000002485295476_text01466093919"></a><a name="zh-cn_topic_0000002485295476_text01466093919"></a>A200I A2 Box 异构组件</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p214613010391"><a name="zh-cn_topic_0000002485295476_p214613010391"></a><a name="zh-cn_topic_0000002485295476_p214613010391"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p15471840391"><a name="zh-cn_topic_0000002485295476_p15471840391"></a><a name="zh-cn_topic_0000002485295476_p15471840391"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p45014483920"><a name="zh-cn_topic_0000002485295476_p45014483920"></a><a name="zh-cn_topic_0000002485295476_p45014483920"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row3146140203913"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p0146160143914"><a name="zh-cn_topic_0000002485295476_p0146160143914"></a><a name="zh-cn_topic_0000002485295476_p0146160143914"></a><span id="zh-cn_topic_0000002485295476_text61462003390"><a name="zh-cn_topic_0000002485295476_text61462003390"></a><a name="zh-cn_topic_0000002485295476_text61462003390"></a>Atlas 300I A2 推理卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p141461506394"><a name="zh-cn_topic_0000002485295476_p141461506394"></a><a name="zh-cn_topic_0000002485295476_p141461506394"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p353114123912"><a name="zh-cn_topic_0000002485295476_p353114123912"></a><a name="zh-cn_topic_0000002485295476_p353114123912"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p135715415392"><a name="zh-cn_topic_0000002485295476_p135715415392"></a><a name="zh-cn_topic_0000002485295476_p135715415392"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row31461604397"><td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485295476_p714619015395"><a name="zh-cn_topic_0000002485295476_p714619015395"></a><a name="zh-cn_topic_0000002485295476_p714619015395"></a><span id="zh-cn_topic_0000002485295476_text111462014399"><a name="zh-cn_topic_0000002485295476_text111462014399"></a><a name="zh-cn_topic_0000002485295476_text111462014399"></a>Atlas 300T A2 训练卡</span></p>
</td>
<td class="cellrowborder" valign="top" width="18.94%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485295476_p1314610043913"><a name="zh-cn_topic_0000002485295476_p1314610043913"></a><a name="zh-cn_topic_0000002485295476_p1314610043913"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485295476_p8602483913"><a name="zh-cn_topic_0000002485295476_p8602483913"></a><a name="zh-cn_topic_0000002485295476_p8602483913"></a>N</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p11635413398"><a name="zh-cn_topic_0000002485295476_p11635413398"></a><a name="zh-cn_topic_0000002485295476_p11635413398"></a>N</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485295476_row31464013910"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485295476_p5146120153917"><a name="zh-cn_topic_0000002485295476_p5146120153917"></a><a name="zh-cn_topic_0000002485295476_p5146120153917"></a><span id="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a><a name="zh-cn_topic_0000002485295476_zh-cn_topic_0000002485295476_ph209063317554"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</span></p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="section537048144314"></a>**

```
… 
int ret = 0;
int card_id = 0;
int device_id = 0;
DCMI_PRBS_OPERATE_PARAM operate_para = {0};
operate_para.main_cmd = DSMI_SERDES_CMD_PRBS;
operate_para.sub_cmd = SERDES_PRBS_SET_CMD;
operate_para.operate_para.set_param.param_base = {0, 0, 1};
operate_para.operate_para.set_param.serdes_prbs_type = SERDES_PRBS_TYPE_7;
operate_para.operate_para.set_param.serdes_prbs_direction = SERDES_PRBS_DIRECTION_TXRX;
DCMI_PRBS_OPERATE_RESULT operate_result;
ret = dcmi_prbs_operate(card_id, device_id, operate_para, &operate_result);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

