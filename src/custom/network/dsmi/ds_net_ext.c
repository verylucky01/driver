/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdint.h>
#include <unistd.h>
#include "ds_net.h"
#include "securec.h"
#include "user_log.h"
#include "dms_devdrv_info_comm.h"
#include "ascend_hal.h"
#include "hccn_comm.h"
#include "hccn_tool.h"

int dsmi_get_qp_context(int logic_id, int port_id, unsigned int qpn, char *context)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size_out = INFO_PAYLOAD_LEN;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid!  (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(context, -EINVAL);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_QP_CONTEXT, (char *)&qpn, sizeof(unsigned int), context, &size_out);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi channel get qp context failed. (ret=%d; logic_id=%d; port_id=%d)", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi channel get qp context failed. (result=%d)", trans_data.result);
    }

    return trans_data.result;
}

static int is_ipv6_v4_mapped(const struct in6_addr *address)
{
    return IN6_IS_ADDR_V4MAPPED(address);
}

static unsigned short reverse_8_bits(unsigned short value)
{
    unsigned char high = (value >> 0x8) & 0xFF;
    unsigned char low = value & 0xFF;
    return ((unsigned short)low << 0x8) | high;
}

static int dgid_to_ip(char *ip, int ip_address_len, unsigned int *qpc, int is_ipv4_flag)
{
    int ret;
    int dgid_offset = 7; //dgid 偏移地址

    int offset_1 = 1;
    int offset_2 = 2;
    int offset_3 = 3;
    int offset_8 = 8;
    int offset_16 = 16;
    int offset_24 = 24;

    if (is_ipv4_flag) {
        ret = sprintf_s(ip, ip_address_len, "%u.%u.%u.%u",
            *(qpc + dgid_offset + offset_3) & 0xFF,
            (*(qpc + dgid_offset + offset_3) >> offset_8) & 0xFF,
            (*(qpc + dgid_offset + offset_3) >> offset_16) & 0xFF,
            (*(qpc + dgid_offset + offset_3) >> offset_24) & 0xFF);
        if (ret <= 0) {
            roce_err("Dsmi sprintf ip_v4 failed. (ret=%d)\n", ret);
            return -ENOMEM;
        }
    } else {
        ret = sprintf_s(ip, ip_address_len, "%x:%x:%x:%x:%x:%x:%x:%x",
            reverse_8_bits(*(qpc + dgid_offset) & 0xFFFF),
            reverse_8_bits((*(qpc + dgid_offset) >> offset_16) & 0xFFFF),
            reverse_8_bits(*(qpc + dgid_offset + offset_1) & 0xFFFF),
            reverse_8_bits((*(qpc + dgid_offset + offset_1) >> offset_16) & 0xFFFF),
            reverse_8_bits(*(qpc + dgid_offset + offset_2) & 0xFFFF),
            reverse_8_bits((*(qpc + dgid_offset + offset_2) >> offset_16) & 0xFFFF),
            reverse_8_bits(*(qpc + dgid_offset + offset_3) & 0xFFFF),
            reverse_8_bits((*(qpc + dgid_offset + offset_3) >> offset_16) & 0xFFFF));
        if (ret <= 0) {
            roce_err("Dsmi sprintf ip_v6 failed. (ret=%d)\n", ret);
            return -ENOMEM;
        }
    }
    return 0;
}

static int dsmi_parse_qp_info(unsigned int qpn, char *context, struct ds_qp_info *qp_info)
{
    int ret;
    unsigned int *qpc;
    int dgid_offset = 7;
    char ip[IP_ADDRESS_LEN] = {0};
    qpc = (unsigned int *)context;

    unsigned char status = (*(qpc + 14) >>29) & 0x07;       //bypte_60_qpst bit 29-31
    unsigned char type = (*(qpc + 19) >> 24) & 0x01;        //byte_80_xrc_qp_type bit 24
    unsigned short src_port = (*(qpc + 12) >> 16) & 0xFFFF; //byte_52_updspn bit 16-31
    unsigned int dst_qpn = *(qpc + 13) & 0xFFFFFF;          //byte_56_dqpn bit 0-23
    unsigned int send_psn = (*(qpc + 42) >> 8) & 0xFFFFFF;  //byte_172_sq_cur_psn bit 8-31
    unsigned int recv_psn = (*(qpc + 26) >> 8) & 0xFFFFFF;  //byte_108_rap_psn bit 8-31

    if (is_ipv6_v4_mapped((struct in6_addr *)(qpc + dgid_offset))) {
        ret = dgid_to_ip(ip, sizeof(ip), qpc, 1);
        if (ret != 0) {
            roce_err("Dsmi parse ip_v4 address of qp info failed. (ret=%d)\n", ret);
            return -ENOMEM;
        }
    } else {
        ret = dgid_to_ip(ip, sizeof(ip), qpc, 0);
        if (ret != 0) {
            roce_err("Dsmi parse ip_v6 address of qp info failed. (ret=%d)\n", ret);
            return -ENOMEM;
        }
    }

    qp_info->src_qpn = qpn;
    qp_info->status = status;
    qp_info->type = type;
    qp_info->src_port = src_port;
    qp_info->dst_qpn = dst_qpn;
    qp_info->send_psn = send_psn;
    qp_info->recv_psn = recv_psn;

    ret = memcpy_s(qp_info->ip, IP_ADDRESS_LEN, ip, strlen(ip));
    if (ret != 0) {
        roce_err("Dsmi memcpy_s ip address of qp info failed. (ret=%d)\n", ret);
        return -ENOMEM;
    }
    return 0;
}

// mode的值已在tool层中校验过
int dsmi_set_optical_loopback(int logic_id, int port_id, int mode, int is_write)
{
    struct ds_trans_data trans_data = {0};
    int ret;
    unsigned int size_out = 0;
    int cmd = mode;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }

    cmd += (is_write ? 0x100 : 0); // cmd的第九位标记读写

    // 权限校验
    ret = hccn_check_usr_identify();
    if (ret) {
        roce_err("Check usr identify failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_SET_OPTICAL_LOOPBACK, (char *)&cmd, sizeof(cmd), NULL, &size_out);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("Dsmi set optical loopback mode %d failed. (ret=%d; logic_id=%d)", mode, ret, logic_id);
        return ret;
    }

    if ((trans_data.result != 0) && (trans_data.result != DSMI_OPTICAL_LOOPBACK_RUNNING)) {
        roce_err("Dsmi set optical loopback mode %d failed. (result=%d; logic_id=%d)", mode,
            trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_mac_type(int logic_id, int port_id, unsigned int *mac_type)
{
    unsigned int size_out = sizeof(struct ds_mac_type);
    struct ds_mac_type mac_type_data = {0};
    struct ds_trans_data trans_data = {0};
    int ret;

    if (logic_id > DS_MAX_LOGIC_ID || logic_id < 0) {
        roce_err("Logic id is invalid! expect [0]-[%d]. (logic_id=%d)", DS_MAX_LOGIC_ID, logic_id);
        return -EINVAL;
    }
    if (port_id < 0 ||  port_id > MAX_PORT_ID) {
        roce_err("Port id is invalid! expect [0]-[%d]. (port_id=%d)", MAX_PORT_ID, port_id);
        return -EINVAL;
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(mac_type, -EINVAL);

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_MAC_TYPE, NULL, 0, (char *)&mac_type_data, &size_out);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("Dsmi get mac type fail. (ret=%d; logic_id=%d; port_id=%d)", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get mac type fail (result=%d; logic_id=%d; port_id=%d)", trans_data.result, logic_id, port_id);
    } else {
        *mac_type = mac_type_data.mac_type;
    }

    return trans_data.result;
}

int dsmi_set_tls_machine_type(int logic_id, struct tls_enable_info *enable_info)
{
    long int val;
    int ret;
 
    ret = halGetDeviceInfo(logic_id, MODULE_TYPE_SYSTEM, INFO_TYPE_RUN_MACH, &val);
    if (ret != 0) {
        roce_err("HalGetDeviceInfo failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }
    enable_info->machine_type = (int)val;
    return 0;
}

bool is_ATLAS_900_A3_SUPERPOD(unsigned int mainboard_id)
{
    if (mainboard_id == ATLAS_900_A3_MAINBOARD_ID_1 || mainboard_id == ATLAS_900_A3_MAINBOARD_ID_2 ||
        mainboard_id == ATLAS_900_A3_MAINBOARD_A_X  || mainboard_id == ATLAS_900_A3_MAINBOARD_A_K) {
        roce_info("Detect Atlas_900_a3_superpod success.(mainboard_id=%u)", mainboard_id);
        return true;
    }
    return false;
}

bool is_ATLAS_9000_A3_SUPERPOD(unsigned int mainboard_id)
{
    if (mainboard_id == ATLAS_9000_A3_MAINBOARD_ID || mainboard_id == ATLAS_9000_A3_MAINBOARD_ID_2) {
        roce_info("Detect Atlas_9000_a3_superpod success.(mainboard_id=%u)", mainboard_id);
        return true;
    }
    return false;
}

int dsmi_get_tls_ca_cfg(int logic_id, int port_id, struct tls_ca_new_certs *ca_cert_info, unsigned int num)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("GET tls ca, param is invalid. (logic_id=%d, port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }

    size = sizeof(struct tls_ca_new_certs) * num;
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_TLS_CA_CFG, NULL, 0, (char*)ca_cert_info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi get tls ca cfg fail. (ret=%d, logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0 && trans_data.result != (-ENOENT)) {
        roce_err("dsmi get tls ca cfg failed. (result=%d, logic_id=%d, port=%d)", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_set_tls_ca_cfg(int logic_id, int port_id, struct tls_ca_new_certs *ca_cert_info, unsigned int num)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("SET tls ca, param is invalid. (logic_id=%d, port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_TLS_CA_CFG, (char*)ca_cert_info, sizeof(struct tls_ca_new_certs) * num,
                        NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi set tls ca cfg fail. (ret=%d, logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0 && trans_data.result != (-ENOENT)) {
        roce_err("dsmi set tls ca cfg failed. (result=%d, logic_id=%d, port=%d)", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_clear_tls_ca_cfg(int logic_id, unsigned int save_mode)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_CLEAR_TLS_CA_CFG, (char*)&save_mode,
        sizeof(unsigned int), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi clear tls ca cfg fail. (ret=%d, logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0 && trans_data.result != (-ENOENT)) {
        roce_err("dsmi clear tls ca cfg failed. (result=%d, logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_port_shaping(int logic_id, unsigned int port_id, struct dsmi_shaping_info *shaping_info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(shaping_info, -EINVAL);

    size = sizeof(struct dsmi_shaping_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_TM_SHAPING_PORT, (char *)&port_id, sizeof(port_id),
                        (char*)shaping_info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi shaping port get fail fail ret[%d] logic_id[%d] port_id[%u]",
                 ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi shaping port get fail result[%d] logic_id[%d] port_id[%u]", trans_data.result, logic_id,
            port_id);
    }

    return trans_data.result;
}

int dsmi_get_hccs_ping_result(int logic_id, hccs_ping_operate_info *operate_info,
                   hccs_ping_reply_info_ext *reply_info_ext)
{
    int time_val, ret;
    unsigned int reply_len;
    struct ds_trans_data trans_data;
 
    operate_info->get_info = 1;
    for (time_val = 0; time_val < TASK_WAIT_MAX_TIME; time_val++) {
        sleep(1);

        ret = memset_s(&trans_data, sizeof(struct ds_trans_data), 0, sizeof(struct ds_trans_data));
        if (ret != 0) {
            roce_err("get hccs ping memset failed. (ret=[%d]; logic_id=[%d])", ret, logic_id);
            return ret;
        }

        reply_len = sizeof(hccs_ping_reply_info_ext);
        DSMI_SET_TRANS_DATA(trans_data, DS_HCCS_PING, (char *)operate_info, sizeof(hccs_ping_operate_info),
                            (char *)reply_info_ext, &reply_len);
        trans_data.pid = getpid();
        ret = dsmi_network_transmission_channel(logic_id, &trans_data);
        if (ret != 0) {
            roce_err("get hccs ping res trans info failed. (ret=[%d]; logic_id=[%d])", ret, logic_id);
            return ret;
        }

        ret = trans_data.result;
        if (ret != 0) {
            roce_err("get hccs ping res failed. (result=[%d]; logic_id=[%d])", ret, logic_id);
            return ret;
        }

        if (reply_info_ext->completed) {
            break;
        }
    }

    if (time_val >= TASK_WAIT_MAX_TIME) {
        roce_err("get hccs ping res timeout. (time_val=[%d]; result=[%d]; logic_id=[%d])", time_val, ret, logic_id);
    }

    return ret;
}

int dsmi_hccs_ping(int logic_id, int port_id, hccs_ping_operate_info *operate_info,
                   hccs_ping_reply_info *reply_info)
{
    int ret;
    unsigned int reply_len;
    struct ds_trans_data trans_data = {0};
    hccs_ping_reply_info_ext *reply_info_ext;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Ping_ext logic id is invalid. (logic_id=[%d])", logic_id);
        return -EINVAL;
    }

    reply_info_ext = calloc(1, sizeof(hccs_ping_reply_info_ext));
    if (reply_info_ext == NULL) {
        roce_err("calloc hccs ping reply failed.");
        return -ENOMEM;
    }

    operate_info->get_info = 0;
    ret = memcpy_s(&reply_info_ext->info, sizeof(hccs_ping_reply_info), reply_info, sizeof(hccs_ping_reply_info));
    if (ret != 0) {
        roce_err("hccs ping memcpy reply info ext failed. (ret=[%d])", ret);
        ret = -EINVAL;
        goto out;
    }

    reply_len = sizeof(hccs_ping_reply_info_ext);
    DSMI_SET_TRANS_DATA(trans_data, DS_HCCS_PING, (char *)operate_info, sizeof(hccs_ping_operate_info),
                        (char *)reply_info_ext, &reply_len);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Ping_ext dsmi trans info failed. (ret=[%d]; logic_id=[%d])", ret, logic_id);
        goto out;
    }

    ret = trans_data.result;
    if (ret != 0) {
        roce_err("Ping_ext failed. (result=[%d]; logic_id=[%d])", ret, logic_id);
        goto out;
    }

    // 接收底层hccsping结果
    ret = dsmi_get_hccs_ping_result(logic_id, operate_info, reply_info_ext);
    if (ret) {
        roce_err("dsmi_get_hccs_ping_result failed. (result=[%d]; logic_id=[%d])", ret, logic_id);
        goto out;
    }

    ret = memcpy_s(reply_info, sizeof(hccs_ping_reply_info), &reply_info_ext->info, sizeof(hccs_ping_reply_info));
out:
    free(reply_info_ext);
    return ret;
}

int dsmi_get_traceroute_status(int logic_id, int *troute_status)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size = ARGC_NUM_10;
    char status[ARGC_NUM_10];
    char *p_tmp = NULL;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id in dsmi get traceroute status is invalid! expect [0]-[%d]. (logic_id=%d)",
                 DS_MAX_LOGIC_ID, logic_id);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_TRACEROUTE_STATUS, NULL, 0, status, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi channel got traceroute status failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }
    *troute_status = strtol(status, &p_tmp, STR_TO_INT_TEN);
    if (*p_tmp != '\0') {
        roce_err("Strtol current status got invalid param. (status=%s)", status);
        return -EINVAL;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get traceroute status result wrong. (result=%d; logic_id=%d)", trans_data.result, logic_id);
        return trans_data.result;
    }

    return 0;
}
 
int dsmi_start_traceroute(int logic_id, struct dsmi_traceroute_info *traceroute_info_send,
    int *troute_start_result)
{
    int ret;
    unsigned int size = ARGC_NUM_10;
    struct ds_trans_data trans_data = {0};
    char result[ARGC_NUM_10];
    char *p_tmp = NULL;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id in dsmi start traceroute is invalid! expect [0]-[%d]. (logic_id=%d)",
                 DS_MAX_LOGIC_ID, logic_id);
        return -EINVAL;
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(traceroute_info_send, -EINVAL);
    DSMI_SET_TRANS_DATA(trans_data, DS_START_TRACEROUTE, (char *)(uintptr_t)traceroute_info_send,
        sizeof(struct dsmi_traceroute_info), result, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi channel start traceroute cmd failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }
    *troute_start_result = strtol(result, &p_tmp, STR_TO_INT_TEN);
    if (*p_tmp != '\0') {
        roce_err("Strtol traceroute result got invalid param. (result=%s)", result);
        return -EINVAL;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi start traceroute result wrong. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}
 
int dsmi_get_traceroute_info(int logic_id, char *troute_info_show, unsigned int info_size, int cmd_type)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id in dsmi get traceroute info is invalid! expect [0]-[%d]. (logic_id=%d)",
                 DS_MAX_LOGIC_ID, logic_id);
        return -EINVAL;
    }

    size = info_size;
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_TRACEROUTE_INFO, (char *)(uintptr_t)cmd_type,
                        sizeof(cmd_type), troute_info_show, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi channel got traceroute info failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get traceroute info result wrong. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}
 
int dsmi_reset_traceroute(int logic_id, int *troute_reset)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size = ARGC_NUM_10;
    char reset[ARGC_NUM_10] = {0};
    char *p_tmp = NULL;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id in dsmi reset traceroute is invalid! expect [0]-[%d]. (logic_id=%d)",
                 DS_MAX_LOGIC_ID, logic_id);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_RESET_TRACEROUTE, NULL, 0, reset, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi channel got traceroute reset failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }
    *troute_reset = strtol(reset, &p_tmp, STR_TO_INT_TEN);
    if (*p_tmp != '\0') {
        roce_err("Strtol traceroute reset got invalid param. (reset=%s)", reset);
        return -EINVAL;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi reset traceroute result was wrong. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_set_prbs_flag(int logic_id, int prbs_flag)
{
    struct ds_trans_data trans_data = {0};
    int ret;
    unsigned int size = 0;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    roce_info("in dsmi_set_prbs_flag, (logic_id=%d, prbs_flag=%d)", logic_id, prbs_flag);
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_NPU_PRBS_FLAG, (char *)&prbs_flag, sizeof(prbs_flag), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set prbs flag data failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set prbs flag data result failed. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_prbs_adapt_in_order(unsigned int mode, unsigned int logic_id, unsigned char master_flag)
{
    int ret = 0;
    unsigned int size = 0;
    struct ds_trans_data trans_data = {0};
    struct prbs_adapt_mode_info mode_info = {0};

    mode_info.mode = mode;
    mode_info.master_flag =master_flag;

    roce_info("prbs adapt in order start. (mode=%u, logic_id=%u, master_flag=%d)",
        mode, logic_id, master_flag);
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_PRBS_ADAPT_IN_ORDER,
                        (char *)&mode_info, sizeof(struct prbs_adapt_mode_info), NULL, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set prbs adapt in order failed. (mode=%u, ret=%d; logic_id=%d)", mode, ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set prbs adapt in order failed. (mode=%u, result=%d; logic_id=%d)",
            mode, trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_set_cdr_mode_cmd(int logic_id, int port, struct ds_cdr_mode_info *info)
{
    struct ds_trans_data trans_data = {0};
    unsigned int size_out = 0;
    int ret;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_SET_CDR_MODE_CMD, (char *)info, sizeof(struct ds_cdr_mode_info), NULL, &size_out);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("Dsmi set cdr mode fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi set cdr mode result failed. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_pfc_duration_info(int logic_id, struct pfc_duration_info *info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size_out;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if (info == NULL) {
        roce_err("input info is null. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size_out = sizeof(struct pfc_duration_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_PFC_D_INFO, NULL, 0, (char *)info, &size_out);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi trans fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi trans res error. (result=%d)", trans_data.result);
    }

    return trans_data.result;
}

int dsmi_get_tc_stat(int logic_id, struct ds_tc_stat_data *stat)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < DS_MIN_LOGIC_ID)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(stat, -EINVAL);
    size = sizeof(struct ds_tc_stat_data);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_TC_STAT, (char *)(&logic_id), sizeof(logic_id), (char *)stat, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("Dsmi get stat fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get stat fail. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_clear_tc_stat(int logic_id)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_CLEAR_TC_PACKET_STATISTICS, (char *)(&logic_id), sizeof(logic_id), NULL, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi clear tc stat fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi clear tc stat fail. (result=%d logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_clear_pfc_duration(int logic_id, int mode)
{
    struct ds_trans_data trans_data = {0};
    int ret;
    unsigned int size_out = 0;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    if (mode < 0 || mode > DS_CLEAR_PFC_MODE_MAX) {
        roce_err("duration mode is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_CLEAR_PFC_DURATION, (char *)&mode, sizeof(int), NULL, &size_out);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi clear pfc pause duration fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi set cdr mode result failed. (result=%d; logic_id=%d)", trans_data.result, logic_id);
        return trans_data.result;
    }

    return trans_data.result;
}

int dsmi_get_qpn_list(int logic_id, int port_id, struct ds_qpn_list *list)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size_out;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(list, -EINVAL);

    size_out = sizeof(struct ds_qpn_list);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_QPN_LIST, NULL, 0, (char *)list, &size_out);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi channel get qpn list failed. (ret=%d; logic_id=%d; port_id=%d)", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi channel get qpn list failed. (result=%d)", trans_data.result);
    }

    return trans_data.result;
}
 
int dsmi_get_qp_info(int logic_id, int port_id, unsigned int qpn, struct ds_qp_info *qp_info)
{
    int ret;
    char context[INFO_PAYLOAD_LEN] = {0};

    ret = dsmi_get_qp_context(logic_id, port_id, qpn, context);
    if (ret != 0) {
        roce_err("Dsmi get qp context failed. (ret=%d; logic_id=%d; port_id=%d)", ret, logic_id, port_id);
        return dsmi_analysis_dsmi_ret_to_uda(ret);
    }

    context[INFO_PAYLOAD_LEN - 1] = '\0';

    ret = dsmi_parse_qp_info(qpn, context, qp_info);
    if (ret != 0) {
        roce_err("Dsmi parse qp info failed. (ret=%d; logic_id=%d; port_id=%d)", ret, logic_id, port_id);
        return ret;
    }

    return 0;
}

int dsmi_start_hccs_ping_mesh(int logic_id, struct hccs_ping_mesh_operate *operate_info)
{
    struct ds_trans_data trans_data = {0};
    unsigned int size_in, size_out = 0;
    int ret;

    if (logic_id > DS_MAX_LOGIC_ID || logic_id < 0) {
        roce_err("logic id is invalid, expect [0]-[%d]. (logic_id=%d)", DS_MAX_LOGIC_ID, logic_id);
        return -EINVAL;
    }

    if (operate_info == NULL) {
        roce_err("operate_info is NULL. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size_in = sizeof(struct hccs_ping_mesh_operate);
    DSMI_SET_TRANS_DATA(trans_data, DS_START_PING_MESH_TASK, (char *)operate_info, size_in, NULL, &size_out);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi trans fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi start hccs ping mesh task error. (result=%d)", trans_data.result);
    }

    return trans_data.result;
}
int dsmi_get_hccs_ping_mesh_info(int logic_id, int task_id, struct ping_mesh_stat *info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size_out;

    if (logic_id > DS_MAX_LOGIC_ID || logic_id < 0) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if (info == NULL) {
        roce_err("input is null. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size_out = sizeof(struct ping_mesh_stat);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_PING_MESH_INFO, (char *)&task_id, sizeof(int), (char *)info, &size_out);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi get hccs ping mesh info trans fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get hccs ping mesh info trans res error. (result=%d)", trans_data.result);
    }
    return trans_data.result;
}
int dsmi_get_hccs_ping_mesh_state(int logic_id, int task_id, unsigned int *state)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size_out;

    if (logic_id > DS_MAX_LOGIC_ID || logic_id < 0) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if (!state) {
        roce_err("input is null. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size_out = sizeof(int);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_PING_MESH_STATE, (char *)&task_id, sizeof(int), (char *)state, &size_out);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi get hccs ping mesh state trans fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get hccs ping mesh state trans res error. (result=%d)", trans_data.result);
    }
    return trans_data.result;
}
int dsmi_stop_hccs_ping_mesh(int logic_id, int task_id)
{
    struct ds_trans_data trans_data = {0};
    int ret;
    unsigned int size_out = 0;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size_out = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_STOP_HCCS_PING_MESH, (char *)&task_id, sizeof(int), NULL, &size_out);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi stop hccs ping mesh fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi stop hccs ping mesh task error failed. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_extra_statistics_info(int logic_id, int port_id, struct ds_extra_statistics_info *info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(info, -EINVAL);

    size = sizeof(struct ds_extra_statistics_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_EXTRA_STAT_INFO, NULL, 0, (char *)info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("Dsmi get extra statistics info fail. (ret=%d; logic_id=%d; port_id=%d)", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get extra statistics fail. (result=%d; logic_id=%d; port=%d)",
                 trans_data.result, logic_id, port_id);
    }
    return trans_data.result;
}