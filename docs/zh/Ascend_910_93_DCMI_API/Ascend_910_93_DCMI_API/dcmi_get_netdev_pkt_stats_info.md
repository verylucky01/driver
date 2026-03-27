# dcmi\_get\_netdev\_pkt\_stats\_info<a name="ZH-CN_TOPIC_0000002485318754"></a>

**函数原型<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412077"></a>**

**int dcmi\_get\_netdev\_pkt\_stats\_info\(int card\_id, int device\_id, int port\_id, struct dcmi\_network\_pkt\_stats\_info \*network\_pkt\_stats\_info\)**

**功能说明<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412078"></a>**

查询NPU设备网口当前收发包数统计。

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
<tbody><tr id="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_row5908907"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p1290353951711"><a name="p1290353951711"></a><a name="p1290353951711"></a>card_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="p13903193920176"><a name="p13903193920176"></a><a name="p13903193920176"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="p09048390172"><a name="p09048390172"></a><a name="p09048390172"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p119041539161713"><a name="p119041539161713"></a><a name="p119041539161713"></a>设备ID，当前实际支持的ID通过dcmi_get_card_list接口获取。</p>
</td>
</tr>
<tr id="row1684271132614"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p11708131185016"><a name="p11708131185016"></a><a name="p11708131185016"></a>device_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="p1070811311504"><a name="p1070811311504"></a><a name="p1070811311504"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="p970973119509"><a name="p970973119509"></a><a name="p970973119509"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p870915316504"><a name="p870915316504"></a><a name="p870915316504"></a>芯片ID，通过dcmi_get_device_id_in_card接口获取。取值范围如下：</p>
<p id="p450968185915"><a name="p450968185915"></a><a name="p450968185915"></a>NPU芯片：[0, device_id_max-1]。</p>
<p id="p1329512573181"><a name="p1329512573181"></a><a name="p1329512573181"></a></p>
<div class="note" id="note102181227171418"><a name="note102181227171418"></a><a name="note102181227171418"></a><span class="notetitle"> 说明： </span><div class="notebody"><p id="zh-cn_topic_0000002517638669_p10218227151413"><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><a name="zh-cn_topic_0000002517638669_p10218227151413"></a><span id="zh-cn_topic_0000002517638669_ph833311293312"><a name="zh-cn_topic_0000002517638669_ph833311293312"></a><a name="zh-cn_topic_0000002517638669_ph833311293312"></a>device_id_max值为2，当device_id为0或1时表示NPU芯片；当device_id为2时表示MCU芯片。</span></p>
</div></div>
</td>
</tr>
<tr id="row5354192415172"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p1824103514419"><a name="p1824103514419"></a><a name="p1824103514419"></a>port_id</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="p424143514417"><a name="p424143514417"></a><a name="p424143514417"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="p1424112357410"><a name="p1424112357410"></a><a name="p1424112357410"></a>int</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1224183544119"><a name="p1224183544119"></a><a name="p1224183544119"></a>NPU设备的网口端口号，当前仅支持配置0。</p>
</td>
</tr>
<tr id="row1459681913429"><td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.5.1.1 "><p id="p4908193110424"><a name="p4908193110424"></a><a name="p4908193110424"></a>network_pkt_stats_info</p>
</td>
<td class="cellrowborder" valign="top" width="16.99%" headers="mcps1.1.5.1.2 "><p id="p7908931124212"><a name="p7908931124212"></a><a name="p7908931124212"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="16.009999999999998%" headers="mcps1.1.5.1.3 "><p id="p3908831194215"><a name="p3908831194215"></a><a name="p3908831194215"></a>dcmi_network_pkt_stats_info</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.5.1.4 "><p id="p1290823174218"><a name="p1290823174218"></a><a name="p1290823174218"></a>struct dcmi_network_pkt_stats_info {</p>
<p id="p17908133111429"><a name="p17908133111429"></a><a name="p17908133111429"></a>unsigned long long mac_tx_mac_pause_num;</p>
<p id="p8908163184219"><a name="p8908163184219"></a><a name="p8908163184219"></a>MAC发送的pause帧总报文数</p>
<p id="p11908183144213"><a name="p11908183144213"></a><a name="p11908183144213"></a>unsigned long long mac_rx_mac_pause_num;</p>
<p id="p18908931114213"><a name="p18908931114213"></a><a name="p18908931114213"></a>MAC接收的pause帧总报文数</p>
<p id="p99081531154210"><a name="p99081531154210"></a><a name="p99081531154210"></a>unsigned long long mac_tx_pfc_pkt_num;</p>
<p id="p19087311426"><a name="p19087311426"></a><a name="p19087311426"></a>MAC发送的PFC帧总报文数</p>
<p id="p29081731114212"><a name="p29081731114212"></a><a name="p29081731114212"></a>unsigned long long mac_tx_pfc_pri0_pkt_num;</p>
<p id="p1590863194214"><a name="p1590863194214"></a><a name="p1590863194214"></a>MAC 0号调度队列发送的PFC帧总报文数</p>
<p id="p1890810315427"><a name="p1890810315427"></a><a name="p1890810315427"></a>unsigned long long mac_tx_pfc_pri1_pkt_num;</p>
<p id="p17908193194218"><a name="p17908193194218"></a><a name="p17908193194218"></a>MAC 1号调度队列发送的PFC帧总报文数</p>
<p id="p20908173117422"><a name="p20908173117422"></a><a name="p20908173117422"></a>unsigned long long mac_tx_pfc_pri2_pkt_num;</p>
<p id="p18908203144210"><a name="p18908203144210"></a><a name="p18908203144210"></a>MAC 2号调度队列发送的PFC帧总报文数</p>
<p id="p1090819318427"><a name="p1090819318427"></a><a name="p1090819318427"></a>unsigned long long mac_tx_pfc_pri3_pkt_num;</p>
<p id="p990893174219"><a name="p990893174219"></a><a name="p990893174219"></a>MAC 3号调度队列发送的PFC帧总报文数</p>
<p id="p9908143117423"><a name="p9908143117423"></a><a name="p9908143117423"></a>unsigned long long mac_tx_pfc_pri4_pkt_num;</p>
<p id="p79081231194217"><a name="p79081231194217"></a><a name="p79081231194217"></a>MAC 4号调度队列发送的PFC帧总报文数</p>
<p id="p1190833119426"><a name="p1190833119426"></a><a name="p1190833119426"></a>unsigned long long mac_tx_pfc_pri5_pkt_num;</p>
<p id="p1490893113421"><a name="p1490893113421"></a><a name="p1490893113421"></a>MAC 5号调度队列发送的PFC帧总报文数</p>
<p id="p1490833117422"><a name="p1490833117422"></a><a name="p1490833117422"></a>unsigned long long mac_tx_pfc_pri6_pkt_num;</p>
<p id="p14908103104214"><a name="p14908103104214"></a><a name="p14908103104214"></a>MAC 6号调度队列发送的PFC帧总报文数</p>
<p id="p1790893117420"><a name="p1790893117420"></a><a name="p1790893117420"></a>unsigned long long mac_tx_pfc_pri7_pkt_num;</p>
<p id="p169081731144210"><a name="p169081731144210"></a><a name="p169081731144210"></a>MAC 7号调度队列发送的PFC帧总报文数</p>
<p id="p20908123154211"><a name="p20908123154211"></a><a name="p20908123154211"></a>unsigned long long mac_rx_pfc_pkt_num;</p>
<p id="p290843184213"><a name="p290843184213"></a><a name="p290843184213"></a>MAC接收的PFC帧总报文数</p>
<p id="p109081631104212"><a name="p109081631104212"></a><a name="p109081631104212"></a>unsigned long long mac_rx_pfc_pri0_pkt_num;</p>
<p id="p189082310429"><a name="p189082310429"></a><a name="p189082310429"></a>MAC 0号调度队列接收的PFC帧总报文数</p>
<p id="p7908631154220"><a name="p7908631154220"></a><a name="p7908631154220"></a>unsigned long long mac_rx_pfc_pri1_pkt_num;</p>
<p id="p590912317424"><a name="p590912317424"></a><a name="p590912317424"></a>MAC 1号调度队列接收的PFC帧总报文数</p>
<p id="p2909143116429"><a name="p2909143116429"></a><a name="p2909143116429"></a>unsigned long long mac_rx_pfc_pri2_pkt_num;</p>
<p id="p2090963164213"><a name="p2090963164213"></a><a name="p2090963164213"></a>MAC 2号调度队列接收的PFC帧总报文数</p>
<p id="p0909173112427"><a name="p0909173112427"></a><a name="p0909173112427"></a>unsigned long long mac_rx_pfc_pri3_pkt_num;</p>
<p id="p9909231204216"><a name="p9909231204216"></a><a name="p9909231204216"></a>MAC 3号调度队列接收的PFC帧总报文数</p>
<p id="p159091431144215"><a name="p159091431144215"></a><a name="p159091431144215"></a>unsigned long long mac_rx_pfc_pri4_pkt_num;</p>
<p id="p190933120421"><a name="p190933120421"></a><a name="p190933120421"></a>MAC 4号调度队列接收的PFC帧总报文数</p>
<p id="p1790917316421"><a name="p1790917316421"></a><a name="p1790917316421"></a>unsigned long long mac_rx_pfc_pri5_pkt_num;</p>
<p id="p8909431184217"><a name="p8909431184217"></a><a name="p8909431184217"></a>MAC 5号调度队列接收的PFC帧总报文数</p>
<p id="p690973124215"><a name="p690973124215"></a><a name="p690973124215"></a>unsigned long long mac_rx_pfc_pri6_pkt_num;</p>
<p id="p17909193104216"><a name="p17909193104216"></a><a name="p17909193104216"></a>MAC 6号调度队列接收的PFC帧总报文数</p>
<p id="p49097315422"><a name="p49097315422"></a><a name="p49097315422"></a>unsigned long long mac_rx_pfc_pri7_pkt_num;</p>
<p id="p69093313420"><a name="p69093313420"></a><a name="p69093313420"></a>MAC 7号调度队列接收的PFC帧总报文数</p>
<p id="p4909331174211"><a name="p4909331174211"></a><a name="p4909331174211"></a>unsigned long long mac_tx_total_pkt_num;</p>
<p id="p29095313426"><a name="p29095313426"></a><a name="p29095313426"></a>MAC发送的总报文数</p>
<p id="p1490933194218"><a name="p1490933194218"></a><a name="p1490933194218"></a>unsigned long long mac_tx_total_oct_num;</p>
<p id="p79091631184211"><a name="p79091631184211"></a><a name="p79091631184211"></a>MAC发送的总报文字节数</p>
<p id="p179094314425"><a name="p179094314425"></a><a name="p179094314425"></a>unsigned long long mac_tx_bad_pkt_num;</p>
<p id="p1590973124211"><a name="p1590973124211"></a><a name="p1590973124211"></a>MAC发送的坏包总报文数</p>
<p id="p8909231104214"><a name="p8909231104214"></a><a name="p8909231104214"></a>unsigned long long mac_tx_bad_oct_num;</p>
<p id="p69091231134219"><a name="p69091231134219"></a><a name="p69091231134219"></a>MAC发送的坏包总报文字节数</p>
<p id="p13909731154215"><a name="p13909731154215"></a><a name="p13909731154215"></a>unsigned long long mac_rx_total_pkt_num;</p>
<p id="p590943119423"><a name="p590943119423"></a><a name="p590943119423"></a>MAC接收的总报文数</p>
<p id="p5909331114215"><a name="p5909331114215"></a><a name="p5909331114215"></a>unsigned long long mac_rx_total_oct_num;</p>
<p id="p59091531104217"><a name="p59091531104217"></a><a name="p59091531104217"></a>MAC接收的总报文字节数</p>
<p id="p490973104214"><a name="p490973104214"></a><a name="p490973104214"></a>unsigned long long mac_rx_bad_pkt_num;</p>
<p id="p19909731104211"><a name="p19909731104211"></a><a name="p19909731104211"></a>MAC接收的坏包总报文数</p>
<p id="p990963110427"><a name="p990963110427"></a><a name="p990963110427"></a>unsigned long long mac_rx_bad_oct_num;</p>
<p id="p13909113124211"><a name="p13909113124211"></a><a name="p13909113124211"></a>MAC接收的坏包总报文字节数</p>
<p id="p18909193118427"><a name="p18909193118427"></a><a name="p18909193118427"></a>unsigned long long mac_rx_fcs_err_pkt_num;</p>
<p id="p7909113194210"><a name="p7909113194210"></a><a name="p7909113194210"></a>MAC接收的存在FCS错误的报文数</p>
<p id="p14909183114213"><a name="p14909183114213"></a><a name="p14909183114213"></a>unsigned long long roce_rx_rc_pkt_num;</p>
<p id="p19909103116423"><a name="p19909103116423"></a><a name="p19909103116423"></a>RoCEE接收的RC类型报文数</p>
<p id="p5909931174218"><a name="p5909931174218"></a><a name="p5909931174218"></a>unsigned long long roce_rx_all_pkt_num;</p>
<p id="p1690963194213"><a name="p1690963194213"></a><a name="p1690963194213"></a>RoCEE接收的总报文数</p>
<p id="p4909123113423"><a name="p4909123113423"></a><a name="p4909123113423"></a>unsigned long long roce_rx_err_pkt_num;</p>
<p id="p99091231184211"><a name="p99091231184211"></a><a name="p99091231184211"></a>RoCEE接收的坏包总报文数</p>
<p id="p490914317429"><a name="p490914317429"></a><a name="p490914317429"></a>unsigned long long roce_tx_rc_pkt_num;</p>
<p id="p2909531184212"><a name="p2909531184212"></a><a name="p2909531184212"></a>RoCEE发送的RC类型报文数</p>
<p id="p189099312429"><a name="p189099312429"></a><a name="p189099312429"></a>unsigned long long roce_tx_all_pkt_num;</p>
<p id="p6909531174211"><a name="p6909531174211"></a><a name="p6909531174211"></a>RoCEE发送的总报文数</p>
<p id="p4909103119426"><a name="p4909103119426"></a><a name="p4909103119426"></a>unsigned long long roce_tx_err_pkt_num;</p>
<p id="p7909103116422"><a name="p7909103116422"></a><a name="p7909103116422"></a>RoCEE发送的坏包总报文数</p>
<p id="p190983110426"><a name="p190983110426"></a><a name="p190983110426"></a>unsigned long long roce_cqe_num;</p>
<p id="p190943134215"><a name="p190943134215"></a><a name="p190943134215"></a>RoCEE任务完成的总元素个数</p>
<p id="p149093310426"><a name="p149093310426"></a><a name="p149093310426"></a>unsigned long long roce_rx_cnp_pkt_num;</p>
<p id="p18909531194213"><a name="p18909531194213"></a><a name="p18909531194213"></a>RoCEE接收的CNP类型报文数</p>
<p id="p8909153112428"><a name="p8909153112428"></a><a name="p8909153112428"></a>unsigned long long roce_tx_cnp_pkt_num;</p>
<p id="p3909143118429"><a name="p3909143118429"></a><a name="p3909143118429"></a>RoCEE发送的CNP类型报文数</p>
<p id="p49091131114215"><a name="p49091131114215"></a><a name="p49091131114215"></a>unsigned long long roce_err_ack_num;</p>
<p id="p0909173164220"><a name="p0909173164220"></a><a name="p0909173164220"></a>RoCEE接收的非预期ACK报文数，NPU做丢弃处理，不影响业务</p>
<p id="p3909143114213"><a name="p3909143114213"></a><a name="p3909143114213"></a>unsigned long long roce_err_psn_num;</p>
<p id="p6909231124210"><a name="p6909231124210"></a><a name="p6909231124210"></a>RoCEE接收的PSN&gt;预期PSN的报文，或重复PSN报文数。乱序或丢包，会触发重传</p>
<p id="p19091831134218"><a name="p19091831134218"></a><a name="p19091831134218"></a>unsigned long long roce_verification_err_num;</p>
<p id="p1090912314422"><a name="p1090912314422"></a><a name="p1090912314422"></a>RoCEE接收的域段校验错误的报文数，如：icrc、报文长度、目的端口号等校验失败</p>
<p id="p1290973144219"><a name="p1290973144219"></a><a name="p1290973144219"></a>unsigned long long roce_err_qp_status_num;</p>
<p id="p189092314421"><a name="p189092314421"></a><a name="p189092314421"></a>RoCEE接收的QP连接状态异常产生的报文数</p>
<p id="p49091031124213"><a name="p49091031124213"></a><a name="p49091031124213"></a>unsigned long long roce_new_pkt_rty_num;</p>
<p id="p15909731114217"><a name="p15909731114217"></a><a name="p15909731114217"></a>RoCEE发送的超次重传的数量统计</p>
<p id="p1890916310424"><a name="p1890916310424"></a><a name="p1890916310424"></a>unsigned long long roce_ecn_db_num;</p>
<p id="p490933117428"><a name="p490933117428"></a><a name="p490933117428"></a>RoCEE接收的存在ECN标记位的报文数</p>
<p id="p190973144218"><a name="p190973144218"></a><a name="p190973144218"></a>unsigned long long nic_tx_all_pkg_num;</p>
<p id="p10909631134212"><a name="p10909631134212"></a><a name="p10909631134212"></a>NIC发送的总报文数</p>
<p id="p590913311424"><a name="p590913311424"></a><a name="p590913311424"></a>unsigned long long nic_tx_all_oct_num;</p>
<p id="p890920316422"><a name="p890920316422"></a><a name="p890920316422"></a>NIC发送的总报文字节数</p>
<p id="p1990919315428"><a name="p1990919315428"></a><a name="p1990919315428"></a>unsigned long long nic_rx_all_pkg_num;</p>
<p id="p6910131114220"><a name="p6910131114220"></a><a name="p6910131114220"></a>NIC接收的总报文数</p>
<p id="p199101131114218"><a name="p199101131114218"></a><a name="p199101131114218"></a>unsigned long long nic_rx_all_oct_num;</p>
<p id="p891003119424"><a name="p891003119424"></a><a name="p891003119424"></a>NIC接收的总报文字节数</p>
<p id="p391093104213"><a name="p391093104213"></a><a name="p391093104213"></a>long tv_sec;</p>
<p id="p7910031144216"><a name="p7910031144216"></a><a name="p7910031144216"></a>查询发生时的当前系统时间（单位s）</p>
<p id="p18910103110426"><a name="p18910103110426"></a><a name="p18910103110426"></a>long tv_usec;</p>
<p id="p091023118421"><a name="p091023118421"></a><a name="p091023118421"></a>查询发生时的当前系统时间（单位us）</p>
<p id="p79104317427"><a name="p79104317427"></a><a name="p79104317427"></a>unsigned char reserved[64];</p>
<p id="p3910183194216"><a name="p3910183194216"></a><a name="p3910183194216"></a>};</p>
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

**约束说明<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412082"></a>**

**表 1** 不同部署场景下的支持情况

<a name="table095520915213"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002485318818_row12818154935117"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002485318818_p181256193914"><a name="zh-cn_topic_0000002485318818_p181256193914"></a><a name="zh-cn_topic_0000002485318818_p181256193914"></a>产品形态</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002485318818_p181115613399"><a name="zh-cn_topic_0000002485318818_p181115613399"></a><a name="zh-cn_topic_0000002485318818_p181115613399"></a>物理机场景（裸机）root用户</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002485318818_p4145619392"><a name="zh-cn_topic_0000002485318818_p4145619392"></a><a name="zh-cn_topic_0000002485318818_p4145619392"></a>物理机场景（裸机）运行用户组（非root用户）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002485318818_p9165683914"><a name="zh-cn_topic_0000002485318818_p9165683914"></a><a name="zh-cn_topic_0000002485318818_p9165683914"></a>物理机+普通容器场景root用户</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002485318818_row1781874915512"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p1681824913518"><a name="zh-cn_topic_0000002485318818_p1681824913518"></a><a name="zh-cn_topic_0000002485318818_p1681824913518"></a><span id="zh-cn_topic_0000002485318818_text1381814492513"><a name="zh-cn_topic_0000002485318818_text1381814492513"></a><a name="zh-cn_topic_0000002485318818_text1381814492513"></a><span id="zh-cn_topic_0000002485318818_text1781824975120"><a name="zh-cn_topic_0000002485318818_text1781824975120"></a><a name="zh-cn_topic_0000002485318818_text1781824975120"></a>Atlas 900 A3 SuperPoD 超节点</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p788675755111"><a name="zh-cn_topic_0000002485318818_p788675755111"></a><a name="zh-cn_topic_0000002485318818_p788675755111"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p088675795113"><a name="zh-cn_topic_0000002485318818_p088675795113"></a><a name="zh-cn_topic_0000002485318818_p088675795113"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p688614575515"><a name="zh-cn_topic_0000002485318818_p688614575515"></a><a name="zh-cn_topic_0000002485318818_p688614575515"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row4818124912514"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p681824985116"><a name="zh-cn_topic_0000002485318818_p681824985116"></a><a name="zh-cn_topic_0000002485318818_p681824985116"></a><span id="zh-cn_topic_0000002485318818_text158181149145113"><a name="zh-cn_topic_0000002485318818_text158181149145113"></a><a name="zh-cn_topic_0000002485318818_text158181149145113"></a><span id="zh-cn_topic_0000002485318818_text081844985117"><a name="zh-cn_topic_0000002485318818_text081844985117"></a><a name="zh-cn_topic_0000002485318818_text081844985117"></a>Atlas 9000 A3 SuperPoD 集群算力系统</span></span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p168861157145118"><a name="zh-cn_topic_0000002485318818_p168861157145118"></a><a name="zh-cn_topic_0000002485318818_p168861157145118"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p188861657125114"><a name="zh-cn_topic_0000002485318818_p188861657125114"></a><a name="zh-cn_topic_0000002485318818_p188861657125114"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p688645745119"><a name="zh-cn_topic_0000002485318818_p688645745119"></a><a name="zh-cn_topic_0000002485318818_p688645745119"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row2202847205512"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p12706213161"><a name="zh-cn_topic_0000002485318818_p12706213161"></a><a name="zh-cn_topic_0000002485318818_p12706213161"></a><span id="zh-cn_topic_0000002485318818_text42718211164"><a name="zh-cn_topic_0000002485318818_text42718211164"></a><a name="zh-cn_topic_0000002485318818_text42718211164"></a>Atlas 800T A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p55253416563"><a name="zh-cn_topic_0000002485318818_p55253416563"></a><a name="zh-cn_topic_0000002485318818_p55253416563"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p05251540564"><a name="zh-cn_topic_0000002485318818_p05251540564"></a><a name="zh-cn_topic_0000002485318818_p05251540564"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p175250435618"><a name="zh-cn_topic_0000002485318818_p175250435618"></a><a name="zh-cn_topic_0000002485318818_p175250435618"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row1659123792914"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p365923772919"><a name="zh-cn_topic_0000002485318818_p365923772919"></a><a name="zh-cn_topic_0000002485318818_p365923772919"></a><span id="zh-cn_topic_0000002485318818_text7609428297"><a name="zh-cn_topic_0000002485318818_text7609428297"></a><a name="zh-cn_topic_0000002485318818_text7609428297"></a>Atlas 800I A3 超节点</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p0342154513291"><a name="zh-cn_topic_0000002485318818_p0342154513291"></a><a name="zh-cn_topic_0000002485318818_p0342154513291"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p11342134582916"><a name="zh-cn_topic_0000002485318818_p11342134582916"></a><a name="zh-cn_topic_0000002485318818_p11342134582916"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p10342345102917"><a name="zh-cn_topic_0000002485318818_p10342345102917"></a><a name="zh-cn_topic_0000002485318818_p10342345102917"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row179571851165511"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002485318818_p87312035620"><a name="zh-cn_topic_0000002485318818_p87312035620"></a><a name="zh-cn_topic_0000002485318818_p87312035620"></a><span id="zh-cn_topic_0000002485318818_text187313016562"><a name="zh-cn_topic_0000002485318818_text187313016562"></a><a name="zh-cn_topic_0000002485318818_text187313016562"></a>A200T A3 Box8 超节点服务器</span></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002485318818_p1667457567"><a name="zh-cn_topic_0000002485318818_p1667457567"></a><a name="zh-cn_topic_0000002485318818_p1667457567"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002485318818_p36715575611"><a name="zh-cn_topic_0000002485318818_p36715575611"></a><a name="zh-cn_topic_0000002485318818_p36715575611"></a>Y</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p4671505616"><a name="zh-cn_topic_0000002485318818_p4671505616"></a><a name="zh-cn_topic_0000002485318818_p4671505616"></a>NA</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002485318818_row5430115215816"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.5.1.1 mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002485318818_p84951356389"><a name="zh-cn_topic_0000002485318818_p84951356389"></a><a name="zh-cn_topic_0000002485318818_p84951356389"></a>注：Y表示支持；N表示不支持；NA表示不涉及，当前未规划此场景。</p>
</td>
</tr>
</tbody>
</table>

**调用示例<a name="zh-cn_topic_0000001251107199_zh-cn_topic_0000001223414423_zh-cn_topic_0000001146259777_toc533412083"></a>**

```
…
int ret = 0;
int card_id=0;
int device_id=0;
int port_id=0;
struct dcmi_network_pkt_stats_info network_pkt_stats_info = {0};
ret = dcmi_get_netdev_pkt_stats_info (card_id, device_id, port_id, &network_pkt_stats_info);
if (ret != 0){
    //todo：记录日志
    return ret;
}
…
```

