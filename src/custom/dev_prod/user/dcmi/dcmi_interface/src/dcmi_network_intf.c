/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <math.h>
#include <unistd.h>
 
#include "securec.h"
#include "dsmi_network_interface.h"
#include "dcmi_interface_api.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_common.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_npu_link_intf.h"
#include "dcmi_network_intf.h"

STATIC int dcmi_check_port_id_valid(int port_id)
{
    return ((port_id >= NETWORK_PORT_COUNT_DEFAULT) || (port_id < 0)) ? DCMI_ERR_CODE_INVALID_PARAMETER : DCMI_OK;
}
 
STATIC int dcmi_get_npu_rdma_bandwidth_info(int card_id, int device_id, int port_id, unsigned int prof_time,
    struct dcmi_network_rdma_bandwidth_info *network_rdma_bandwidth_info)
{
    int ret;
    int device_logic_id = 0;
    struct bandwidth_t rdma_bandwidth_info = {0};
 
    rdma_bandwidth_info.time_interval = prof_time;
 
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
 
    ret = dsmi_get_bandwidth(device_logic_id, port_id, &rdma_bandwidth_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_bandwidth failed. err is %d.", ret);
    }
 
    network_rdma_bandwidth_info->tx_bandwidth = rdma_bandwidth_info.tx_bandwidth / NETWORK_RDMA_BYTE_TO_MBYTE;
    network_rdma_bandwidth_info->rx_bandwidth = rdma_bandwidth_info.rx_bandwidth / NETWORK_RDMA_BYTE_TO_MBYTE;
 
    return dcmi_convert_error_code(ret);
}
 
int dcmi_get_rdma_bandwidth_info(int card_id, int device_id, int port_id, unsigned int prof_time,
    struct dcmi_network_rdma_bandwidth_info *network_rdma_bandwidth_info)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
 
    if (network_rdma_bandwidth_info == NULL) {
        gplog(LOG_ERR, "network_rdma_bandwidth_info is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_check_port_id_valid(port_id) == DCMI_ERR_CODE_INVALID_PARAMETER) {
        gplog(LOG_ERR, "port_id is invalid. Input portid is %d.", port_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (prof_time > MAX_TIME_INTERVAL || prof_time < MIN_TIME_INTERVAL) {
        gplog(LOG_ERR, "Input prof time is invalid, prof time need to be 100-10000ms.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (!(dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93())) {
        gplog(LOG_OP, "This device does not support get rdma bandwidth info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get rdma bandwidth info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_rdma_bandwidth_info(card_id, device_id, port_id,
            prof_time, network_rdma_bandwidth_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
 
STATIC int dcmi_get_npu_netdev_pkt_stats_info(int card_id, int device_id, int port_id,
    struct dcmi_network_pkt_stats_info *network_pkt_stats_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
 
    ret = dsmi_get_netdev_stat(device_logic_id, port_id, (struct ds_port_stat_info*)network_pkt_stats_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_bandwidth failed. err is %d.", ret);
    }
 
    return dcmi_convert_error_code(ret);
}
 
int dcmi_get_netdev_pkt_stats_info(int card_id, int device_id, int port_id,
    struct dcmi_network_pkt_stats_info *network_pkt_stats_info)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    
    if (network_pkt_stats_info == NULL) {
        gplog(LOG_ERR, "network_pkt_stats_info is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_check_port_id_valid(port_id) == DCMI_ERR_CODE_INVALID_PARAMETER) {
        gplog(LOG_ERR, "port_id is invalid. Input portid is %d.", port_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_board_chip_type_is_ascend_910b() && !dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support get dcmi_get_netdev_pkt_stats_info info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get dcmi_get_netdev_pkt_stats_info info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_netdev_pkt_stats_info(card_id, device_id, port_id,
            network_pkt_stats_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_get_npu_netdev_tc_pkt_stats_info(int card_id, int device_id,
    struct dcmi_tc_stat_data *network_tc_stat_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_logic_id failed. (ret=%d)", ret);
        return ret;
    }

    // 将 struct dcmi_tc_stat_data * 转换为 struct ds_tc_stat_data *，并使用 void * 来避免类型冲突
    ret = dsmi_get_tc_stat(device_logic_id, (struct ds_tc_stat_data *)(void *)network_tc_stat_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "Call dsmi_get_tc_stat failed. (ret=%d)", ret);
    }
 
    return dcmi_convert_error_code(ret);
}

int dcmi_get_netdev_tc_stat_info(int card_id, int device_id,
    struct dcmi_tc_stat_data *network_tc_stat_info)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (network_tc_stat_info == NULL) {
        gplog(LOG_ERR, "The network_tc_stat_info is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!(dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93())) {
        gplog(LOG_OP, "This device does not support get traceroute info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get tc stat info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "The dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_netdev_tc_pkt_stats_info(card_id, device_id, network_tc_stat_info);
    } else {
        gplog(LOG_ERR, "The device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_set_npu_device_clear_tc_pkt_stats(int card_id, int device_id)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_logic_id failed.(ret=%d).", ret);
        return ret;
    }

    ret = dsmi_clear_tc_stat(device_logic_id);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "Call dsmi_clear_tc_stat failed. (ret=%d).", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_device_clear_tc_pkt_stats(int card_id, int device_id)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (!(dcmi_is_in_privileged_docker_root() || dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root())) {
    gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (!(dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93())) {
        gplog(LOG_OP, "The device does not support get traceroute info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support clear tc pkt stats.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "Get dcmi_get_device_type failed. (ret=%d).", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_clear_tc_pkt_stats(card_id, device_id);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "Clear tc_pkt_stats info failed. (card_id=%d, device_id=%d, err=%d).", card_id, device_id,
                err);
            return err;
        }

        gplog(LOG_OP, "Clear tc_pkt_stats info success. card_id=%d, device_id=%d\n", card_id, device_id);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "The device_type %d is not support.\n", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_traceroute_environment_check(int card_id, int device_id, int *logic_id)
{
    int ret;
    unsigned int phy_id = 0;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (!dcmi_is_in_phy_machine_root() && !dcmi_is_in_vm_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical or virtual machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (!dcmi_board_chip_type_is_ascend_910b() && !dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "This device does not support get traceroute info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get traceroute info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }

    if (device_type == NPU_TYPE) {
        ret = dcmi_get_device_logic_id(logic_id, card_id, device_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Call dcmi_get_device_logic_id failed. (ret=%d)", ret);
            return ret;
        }
        ret = dcmi_get_phyid_by_cardid_and_devid(card_id, device_id, &phy_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Get phyid failed. (card_id=%d; device_id=%d; ret=%d)", card_id, device_id, ret);
            return ret;
        }
        return DCMI_OK;
    } else {
        gplog(LOG_ERR, "Device_type is not support. (device_type=%d)", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_traceroute_reset(int logic_id)
{
    int ret;
    int troute_reset = 0;

    ret = dsmi_reset_traceroute(logic_id, &troute_reset);
    if (ret != 0 || troute_reset != 0) {
        gplog(LOG_ERR, "Dcmi reset traceroute failed. (ret=%d; logic_id=%d; troute_reset=%d)",
            ret, logic_id, troute_reset);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}

static int dcmi_traceroute_ip_param(unsigned int *dip, bool is_ipv6, char *ipaddr)
{
    struct in_addr addr;
    struct in6_addr addr6;
    int ret;
 
    if (is_ipv6) {
        ret = inet_pton(AF_INET6, ipaddr, &addr6);
        if (ret <= 0) {
            gplog(LOG_ERR, "IPv6 convert failed. (ip=%s; ret=%d)", ipaddr, ret);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        ret = memcpy_s(dip, DEVDRV_IPV6_ARRAY_LEN, addr6.s6_addr, DEVDRV_IPV6_ARRAY_LEN);
        if (ret != 0) {
            gplog(LOG_ERR, "Dcmi traceroute ip param memcpy failed. (ret=%d)", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    } else {
        ret = inet_pton(AF_INET, ipaddr, &addr);
        if (ret <= 0) {
            gplog(LOG_ERR, "IPv4 convert failed. (ip=%s; ret=%d)", ipaddr, ret);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        dip[0] = addr.s_addr;
    }
 
    return DCMI_OK;
}

STATIC int dcmi_traceroute_info_load(struct dsmi_traceroute_info *traceroute_info_send,
                                     struct dcmi_traceroute_info param_in)
{
    int ret, tmp_value;
    unsigned int index;
    bool is_ipv6 = param_in.ipv6_flag;
    int max_tc_or_tos = is_ipv6 ? TRACEROUTE_TC_MAX : TRACEROUTE_TOS_MAX;
    struct param_value param_info[] = {
        { "max_ttl", .input.int_value = param_in.max_ttl,
            .min.int_value = TRACEROUTE_TTL_MIN, .max.int_value = TRACEROUTE_TTL_MAX },
        { "TOS(ipv4) or TC(ipv6)", .input.int_value = param_in.tos,
            .min.int_value = TRACEROUTE_TOS_MIN, .max.int_value = max_tc_or_tos },
        { "waittime", .input.int_value = param_in.waittime,
            .min.int_value = TRACEROUTE_WAITTIME_MIN, .max.int_value = TRACEROUTE_WAITTIME_MAX },
        { "source_port", .input.int_value = param_in.source_port,
            .min.int_value = UDP_PORT_NUMBER_MIN, .max.int_value = UDP_PORT_NUMBER_MAX },
        { "dest_port", .input.int_value = param_in.dest_port,
            .min.int_value = UDP_PORT_NUMBER_MIN, .max.int_value = UDP_PORT_NUMBER_MAX },
    };

    for (index = 0; index < (sizeof(param_info) / sizeof(struct param_value)); index++) {
        tmp_value = param_info[index].input.int_value;
        if (tmp_value != TRACEROUTE_DEFAULT_VALUE &&
            (tmp_value < param_info[index].min.int_value || tmp_value > param_info[index].max.int_value)) {
            gplog(LOG_ERR, "The input value is invalid. (operation=%s, input_value=%d)",
                  param_info[index].operation, tmp_value);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
    }

    ret = memcpy_s(traceroute_info_send, sizeof(struct dsmi_traceroute_info),
                   &param_in, sizeof(struct dsmi_traceroute_info));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Dcmi set traceroute memcpy failed! (ret=%d)", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_traceroute_ip_param(traceroute_info_send->dip, is_ipv6, param_in.dest_ip);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_traceroute_ip_param failed. (ret=%d)", ret);
        return ret;
    }

    traceroute_info_send->ipv6_flag = is_ipv6;

    return DCMI_OK;
}

STATIC int dcmi_traceroute_start(int logic_id, struct dsmi_traceroute_info *traceroute_info_send)
{
    int ret;
    int troute_status = 0, troute_result = 0, troute_reset = 0;

    ret = dsmi_get_traceroute_status(logic_id, &troute_status);
    if (ret != 0) {
        gplog(LOG_ERR, "Dcmi_waiting_traceroute_finish get status failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return (ret < 0 ? DCMI_ERR_CODE_INVALID_PARAMETER : dcmi_convert_error_code(ret));
    }

    if (troute_status == TRACEROUTE_STATUS_RUNNING) {
        gplog(LOG_ERR, "Traceroute is running, please kill the running process and start a new one. \
              (troute_status=%d)", troute_status);
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    } else if (troute_status == TRACEROUTE_STATUS_ERROR) {
        ret = dsmi_reset_traceroute(logic_id, &troute_reset);
        if (ret != 0 || troute_reset != 0) {
            gplog(LOG_ERR, "Call dsmi_reset_traceroute failed. (ret=%d; logic_id=%d; troute_reset=%d)",
                  ret, logic_id, troute_reset);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }

    ret = dsmi_start_traceroute(logic_id, traceroute_info_send, &troute_result);
    if (ret != 0 || troute_result == TRACEROUTE_STATUS_ERROR) {
        gplog(LOG_ERR, "Call dsmi_start_traceroute failed. (ret=%d; logic_id=%d; troute_result=%d)",
              ret, logic_id, troute_result);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return ret;
}

STATIC int dcmi_traceroute_waiting_finish(int logic_id)
{
    int ret;
    int wait_num = MAX_TRACEROUTE_WAITING_TIMES, troute_status = 0;

    while (wait_num-- > 0) {
        ret = dsmi_get_traceroute_status(logic_id, &troute_status);
        if (ret != 0) {
            gplog(LOG_ERR, "Dcmi_waiting_traceroute_finish get status failed. \
                  ret=%d; logic_id=%d.", ret, logic_id);
            return (ret < 0 ? DCMI_ERR_CODE_INVALID_PARAMETER : dcmi_convert_error_code(ret));
        }

        if (troute_status == TRACEROUTE_STATUS_NOT_RUNNING) {
            return DCMI_OK;
        } else if (troute_status == TRACEROUTE_STATUS_ERROR) {
            gplog(LOG_ERR, "Dsmi got error traceroute status. (ret=%d; logic_id=%d; troute_status=%d)",
                  ret, logic_id, troute_status);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        sleep(1);
    }

    gplog(LOG_ERR, "Tool wait traceroute finish timeout. (logic_id=%d)", logic_id);
    return DCMI_ERR_CODE_TIME_OUT;
}

STATIC int dcmi_set_traceroute_main(int logic_id, struct dcmi_traceroute_info param_in,
    struct dcmi_network_node_info *ret_info, unsigned int ret_info_size)
{
    int ret, index;
    struct dsmi_traceroute_info traceroute_info_send = {0};

    // 1.将需要填充的结构体数据置0
    ret = memset_s(ret_info, ret_info_size, 0, ret_info_size);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Func dcmi_set_traceroute_main call memset_s failed. (ret=%d)", ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    // 2.数据填充至dsmi结构体
    ret = dcmi_traceroute_info_load(&traceroute_info_send, param_in);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_traceroute_info_load failed. (ret=%d)", ret);
        return ret;
    }
    // 3.执行指令
    ret = dcmi_traceroute_start(logic_id, &traceroute_info_send);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_traceroute_start failed. (ret=%d)", ret);
        return ret;
    }
    // 4.持续等待device侧指令执行结束
    ret = dcmi_traceroute_waiting_finish(logic_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_waiting_traceroute_finish failed. (ret=%d)", ret);
        return ret;
    }
    // 5.获取回显
    ret = dsmi_get_traceroute_info(logic_id, (char *)(void *)ret_info, ret_info_size, TRACEROUTE_DCMI_CMD);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "Call dsmi_get_traceroute_info failed. (ret=%d)", ret);
        return (ret < 0 ? DCMI_ERR_CODE_INVALID_PARAMETER : dcmi_convert_error_code(ret));
    }

    // 由于device侧开方函数无法使用，在host侧对stdev值开方
    for (index = 0; (size_t)index < (ret_info_size / sizeof(struct dcmi_network_node_info)); index++) {
        ret_info[index].stdev = sqrt(ret_info[index].stdev);
    }

    return DCMI_OK;
}

int dcmi_set_traceroute(int card_id, int device_id, struct dcmi_traceroute_info param_in,
    struct dcmi_network_node_info *ret_info, unsigned int ret_info_size)
{
    int ret;
    int logic_id = 0;

    if (ret_info == NULL) {
        gplog(LOG_ERR, "The ret_info is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (ret_info_size < sizeof(struct dcmi_network_node_info) ||
        ret_info_size > sizeof(struct dcmi_network_node_info) * (TRACEROUTE_TTL_MAX + 1)) {
        gplog(LOG_ERR, "The ret_info_size is invalid. (min_size=%u; max_size=%u; ret_info_size=%u)",
              sizeof(struct dcmi_network_node_info), sizeof(struct dcmi_network_node_info) * (TRACEROUTE_TTL_MAX + 1),
              ret_info_size);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_traceroute_environment_check(card_id, device_id, &logic_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_traceroute_environment_check failed. (ret=%d)", ret);
        return ret;
    }

    if (param_in.reset_flag) {
        ret = dcmi_traceroute_reset(logic_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Call dcmi_reset_traceroute failed. (ret=%d)", ret);
            return ret;
        }
        goto out;
    }
    ret = dcmi_set_traceroute_main(logic_id, param_in, ret_info, ret_info_size);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_set_traceroute_main failed. (ret=%d)", ret);
        return ret;
    }

out:
    gplog(LOG_OP, "Set traceroute success. (card_id=%d; device_id=%d)", card_id, device_id);
    return DCMI_OK;
}

STATIC bool dcmi_check_sdid_is_valid(unsigned int sdid)
{
    return (sdid != PING_SDID_INVALID_VALUE && SDID_GET_SERVERID(sdid) < A3_SUPERPOD_MAX_NUMS &&
        (SDID_GET_CHIPID(sdid) * CHIP_DIE_CNT + SDID_GET_DIEID(sdid) == SDID_GET_DEVICEID(sdid)));
}

STATIC bool dcmi_check_vnic_ip_is_valid(struct in_addr addr)
{
    return (IP_GET_HEADER(addr.s_addr) == VNIC_IP_HEADER);
}

STATIC int dcmi_check_and_get_valid_ping_info(struct dcmi_ping_operate_info *dcmi_ping,
                                              hccs_ping_operate_info *ping_info, int phy_id)
{
    struct in_addr addr;
 
    // dst_addr不为空时，以dst_addr作为目的地址
    if (strlen(dcmi_ping->dst_addr) > 0) {
        if (inet_pton(AF_INET, dcmi_ping->dst_addr, &addr) <= 0 || (dcmi_check_vnic_ip_is_valid(addr) == false)) {
            gplog(LOG_ERR, "dst_addr is invalid. (dst_addr=%s; errno=%d)", dcmi_ping->dst_addr, errno);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        ping_info->sdid = IP_CONVERT_SDID(addr.s_addr); // 将ip地址转为sdid
    } else {
        ping_info->sdid = dcmi_ping->sdid;
    }
 
    // 用户输入的ip地址、sdid均无效
    if (dcmi_check_sdid_is_valid(ping_info->sdid) == false) {
        gplog(LOG_ERR, "dst_addr and sdid are invalid. (dst_addr=%u; sdid=%u)", addr.s_addr, ping_info->sdid);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    // 校验参数输入是否符合约束条件（防止处理时间过长导致HDC断链）：send_num * interval <= 20000ms , timeout 参数无意义
    if ((dcmi_ping->packet_size < PING_PACKET_SIZE_MIN || dcmi_ping->packet_size > PING_PACKET_SIZE_MAX) ||
        (dcmi_ping->packet_send_num < PING_PACKET_NUM_MIN || dcmi_ping->packet_send_num > PING_PACKET_NUM_MAX) ||
        (dcmi_ping->packet_interval < PING_PACKET_INTERVAL_MIN ||
        dcmi_ping->packet_interval > PING_PACKET_INTERVAL_MAX) ||
        (dcmi_ping->timeout < PING_PACKET_TIMEOUT_MIN || dcmi_ping->timeout > PING_PACKET_TIMEOUT_MAX) ||
        (dcmi_ping->packet_send_num * dcmi_ping->packet_interval > PING_CONSTRAINT_VALUE)) {
        gplog(LOG_ERR, "input param is invalid. (packet_size=%u; packet_send_num=%u; packet_interval=%u)",
              dcmi_ping->packet_size, dcmi_ping->packet_send_num, dcmi_ping->packet_interval);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    ping_info->packet_size = dcmi_ping->packet_size;
    ping_info->packet_send_num = dcmi_ping->packet_send_num;
    ping_info->packet_interval = dcmi_ping->packet_interval;
    ping_info->phy_id = phy_id;
 
    return DCMI_OK;
}
 
STATIC void dcmi_get_ping_reply(hccs_ping_reply_info *reply_info, struct dcmi_ping_reply_info_v2 *dcmi_reply)
{
    int i;

    inet_ntop(AF_INET, &reply_info->dstip, dcmi_reply->info.dst_addr, IP_ADDR_LEN);
    for (i = 0; i < PING_PACKET_NUM_MAX; ++i) {
        dcmi_reply->info.ret[i] = reply_info->ret[i];
        dcmi_reply->info.start_tv_sec[i] = 0;
        dcmi_reply->info.start_tv_usec[i] = 0;
        dcmi_reply->info.end_tv_sec[i] = reply_info->time[i] / NS_CONVERT_TO_S;
        dcmi_reply->info.end_tv_usec[i] = reply_info->time[i] / NS_CONVERT_TO_US;
        dcmi_reply->L1_plane_check_res[i] = reply_info->ret_16k[i];
    }
    dcmi_reply->info.total_packet_send_num = reply_info->total_packet_send_num;
    dcmi_reply->info.total_packet_recv_num = reply_info->total_packet_recv_num;
}
 
STATIC int dcmi_get_ping_info_status(int card_id, int device_id, int port_id, struct dcmi_ping_operate_info *dcmi_ping,
                                     struct dcmi_ping_reply_info_v2 *dcmi_reply)
{
    int ret;
    int logic_id = 0;
    unsigned int phy_id = 0;
    hccs_ping_operate_info ping_info = {.is_ipv6 = 0,
                                        .sdid = PING_SDID_INVALID_VALUE};
    hccs_ping_reply_info *reply_info = (hccs_ping_reply_info *)calloc(1, sizeof(hccs_ping_reply_info));
    if (reply_info == NULL) {
        gplog(LOG_ERR, "calloc reply_info failed.");
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }
 
    ret = dcmi_check_port_id_valid(port_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "port_id is invalid. (portid=%d; ret=%d)", port_id, ret);
        goto PING_EXIT;
    }
 
    ret = dcmi_get_device_logic_id(&logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. (ret=%d)", ret);
        goto PING_EXIT;
    }
 
    ret = dcmi_get_device_phyid_from_logicid((unsigned int)logic_id, &phy_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_phyid_from_logicid failed. (ret=%d)", ret);
        goto PING_EXIT;
    }

    ret = dcmi_check_and_get_valid_ping_info(dcmi_ping, &ping_info, (int)phy_id);
    if (ret != DCMI_OK) {
        goto PING_EXIT;
    }
 
    ret = dsmi_hccs_ping(logic_id, 0, &ping_info, reply_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_hccs_ping failed. (ret=%d)", ret);
        ret = dcmi_convert_error_code(ret);
        goto PING_EXIT;
    }
 
    dcmi_get_ping_reply(reply_info, dcmi_reply);
PING_EXIT:
    free(reply_info);
    return ret;
}
 
int dcmi_get_ping_info_v2(int card_id, int device_id, int port_id, struct dcmi_ping_operate_info *dcmi_ping,
                          struct dcmi_ping_reply_info_v2 *dcmi_reply)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
 
    if (dcmi_check_run_not_root() ||
        !(dcmi_is_in_phy_machine() || dcmi_check_run_in_vm() || dcmi_is_in_privileged_docker_root())) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical/virtual machine or"
              " privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
 
    if (dcmi_ping == NULL || dcmi_reply == NULL) {
        gplog(LOG_ERR, "dcmi_ping is NULL or dcmi_reply is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (!dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support get ping info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        return dcmi_get_ping_info_status(card_id, device_id, port_id, dcmi_ping, dcmi_reply);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_ping_info(int card_id, int device_id, int port_id, struct dcmi_ping_operate_info *dcmi_ping,
                       struct dcmi_ping_reply_info *dcmi_reply)
{
    int ret;
    struct dcmi_ping_reply_info_v2 *info = calloc(1, sizeof(struct dcmi_ping_reply_info_v2));
    if (info == NULL) {
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }

    if (dcmi_reply == NULL) {
        gplog(LOG_ERR, "dcmi_reply is NULL.");
        free(info);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    ret = dcmi_get_ping_info_v2(card_id, device_id, port_id, dcmi_ping, info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_ping_info_v2 failed. (ret=%d)", ret);
        free(info);
        return ret;
    }

    memcpy_s(dcmi_reply, sizeof(struct dcmi_ping_reply_info), &info->info, sizeof(struct dcmi_ping_reply_info));
    free(info);
    return DCMI_OK;
}

STATIC int dcmi_check_sdid_valid(char *target)
{
    int sd_len, i;
 
    if (target == NULL) {
        gplog(LOG_ERR, "target is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    sd_len = strlen(target);
    if (sd_len > SDID_LEN_MAX) {
        gplog(LOG_ERR, "sdid is invalid. sd_len is tool long.sd_len=%d", sd_len);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    for (i = 0; i < sd_len; i++) {
        if (target[i] < '0' || target[i] > '9') {
            gplog(LOG_ERR, "sdid is invalid.");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
    }
 
    return DCMI_OK;
}

STATIC int dcmi_get_hccsping_mesh_addr(int *sdid, char *target, struct in_addr *addr,
                                       int isIp, int destnum)
{
    int ret;
 
    if (sdid == NULL || target == NULL || addr == NULL) {
        gplog(LOG_ERR, "input param is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (isIp == 0) {
        // 判断异常SDID
        ret = dcmi_check_sdid_valid(target);
        if (ret != 0) {
            gplog(LOG_ERR, "sdid is invalid");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }

        sdid[destnum] = (int)strtol(target, NULL, 0);
        return DCMI_OK;
    }
 
    if (inet_pton(AF_INET, target, addr) <= 0 || (dcmi_check_vnic_ip_is_valid(*addr) == false)) {
        gplog(LOG_ERR, "hccsping mesh ip is invalid!");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    sdid[destnum] = IP_CONVERT_SDID(addr->s_addr);
    return DCMI_OK;
}
 
static int dcmi_parse_hccsping_mesh_addr(struct dcmi_hccsping_mesh_operate *hccsping_mesh,
                                         int *sdid, int *destnum)
{
    int ret, valid_sdid_num = 0;
    struct in_addr addr;
    char *token;
    char *ptr;
    char seps[] = ",";
    char *context = NULL;

    if (destnum == NULL || sdid == NULL) {
        gplog(LOG_ERR, "destnum or dest_tmp is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    token = strtok_s(hccsping_mesh->dst_addr_list, seps, &context);
    if (token == NULL) {
        gplog(LOG_ERR, "dst addr list is invalid!");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ptr = strchr(token, '.');
    bool isIp = (ptr == NULL) ? 0 : 1;  // 判断用户输入是否全部为IP地址格式
    while (token != NULL) {
        ptr = strchr(token, '.');
        bool curIsip = (ptr == NULL) ? 0 : 1;
        if (curIsip != isIp) {
            gplog(LOG_ERR, "hccsping mesh addr is invalid!");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }

        ret = dcmi_get_hccsping_mesh_addr(sdid, token, &addr, isIp, valid_sdid_num);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_hccsping_mesh_addr failed. err is %d.", ret);
            return ret;
        }

        valid_sdid_num++;
        token = strtok_s(NULL, seps, &context);
    }

    *destnum = valid_sdid_num;
    return DCMI_OK;
}
static int dcmi_check_hccsping_mesh_addr(struct dcmi_hccsping_mesh_operate *hccsping_mesh,
                                         struct hccs_ping_mesh_operate *operate)
{
    int i, j, unrepeat_sdid_num = 0;
    int ret, destnum = 0;
    int sdid[HCCS_PING_MESH_MAX_NUM];
 
    if (operate == NULL || hccsping_mesh == NULL) {
        gplog(LOG_ERR, "operate is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    ret = dcmi_parse_hccsping_mesh_addr(hccsping_mesh, sdid, &destnum);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_parse_hccsping_mesh_addr is failed!");
        return ret;
    }
    
    // 进行地址去重
    for (i = 0; i < destnum; i++) {
        int exists = 0;
        for (j = 0; j < unrepeat_sdid_num; j++) {
            if (sdid[i] == sdid[j]) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            sdid[unrepeat_sdid_num] = sdid[i];
            unrepeat_sdid_num++;
        }
    }
 
    if (unrepeat_sdid_num > HCCS_PING_MESH_MAX_NUM) {
        gplog(LOG_ERR, "dest_num is too many!");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    operate->dest_num = unrepeat_sdid_num;
 
    for (i = 0; i < unrepeat_sdid_num; i++) {
        if (dcmi_check_sdid_is_valid((unsigned int)sdid[i]) == false) {
            gplog(LOG_ERR, "pingmesh sdid is invalid. (sdid=%u)", (unsigned int)sdid[i]);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        operate->sdid[i] = (unsigned int)sdid[i];
    }

    return DCMI_OK;
}
 
STATIC int parse_integer_param(int min, int max, int *value)
{
    if (value == NULL) {
        gplog(LOG_ERR, "value is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (*value < min || *value > max) {
        gplog(LOG_ERR, "value is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return DCMI_OK;
}
 
STATIC int parse_pkt_param(struct dcmi_hccsping_mesh_operate *operate, int min, int max)
{
    return parse_integer_param(min, max, &operate->pkt_size);
}
 
STATIC int parse_cnt_param(struct dcmi_hccsping_mesh_operate *operate, int min, int max)
{
    return parse_integer_param(min, max, &operate->pkt_send_num);
}
 
STATIC int parse_interval_param(struct dcmi_hccsping_mesh_operate *operate, int min, int max)
{
    return parse_integer_param(min, max, &operate->pkt_interval);
}
 
STATIC int parse_task_interval_param(struct dcmi_hccsping_mesh_operate *operate, int min, int max)
{
    return parse_integer_param(min, max, &operate->task_interval);
}
 
STATIC int parse_task_id_param(struct dcmi_hccsping_mesh_operate *operate, int min, int max)
{
    return parse_integer_param(min, max, &operate->task_id);
}

STATIC int parse_timeout_param(struct dcmi_hccsping_mesh_operate *operate, int min, int max)
{
    return parse_integer_param(min, max, &operate->timeout);
}
 
STATIC struct ping_mesh_parse_ext g_ping_mesh_parse[] = {
    {parse_pkt_param, MIN_PKT_SIZE, MAX_PKT_SIZE},
    {parse_cnt_param, 1, MAX_SEND_NUM},
    {parse_interval_param, 0, MAX_PKT_INTERVAL},
    {parse_task_interval_param, 1, MAX_TASK_INTERVAL},
    {parse_task_id_param, 0, MAX_TASK_ID},
    {parse_timeout_param, 0, INT_MAX}
};
 
static int dcmi_get_hccsping_mesh_param_proc(struct dcmi_hccsping_mesh_operate *hccsping_mesh)
{
    int ret, parse_index;
 
    if (hccsping_mesh == NULL) {
        gplog(LOG_ERR, "hccsping_mesh is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    for (parse_index = 0; parse_index < (sizeof(g_ping_mesh_parse) / sizeof(struct ping_mesh_parse_ext));
         parse_index++) {
        if (g_ping_mesh_parse[parse_index].parse_proc == NULL) {
            gplog(LOG_ERR, "parse_proc is NULL.");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        ret = g_ping_mesh_parse[parse_index].parse_proc(hccsping_mesh,
                                                        g_ping_mesh_parse[parse_index].min,
                                                        g_ping_mesh_parse[parse_index].max);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "parse_index = %d,parse failed.ret is %d.", parse_index, ret);
            return ret;
        }
    }
        
    return ret;
}
 
static int dcmi_get_hccsping_mesh_param(struct dcmi_hccsping_mesh_operate *hccsping_mesh,
                                        struct hccs_ping_mesh_operate *operate, int phy_id)
{
    int ret;
    // 校验各参数是否在约束范围内
    ret = dcmi_get_hccsping_mesh_param_proc(hccsping_mesh);
    if (ret != 0) {
        gplog(LOG_ERR, "dcmi_get_hccsping_mesh_param_proc failed, ret is %d.", ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    // 赋值到operate中
    operate->ping_task_period = hccsping_mesh->task_interval;
    operate->ping_pkt_size = hccsping_mesh->pkt_size;
    operate->ping_pkt_num = hccsping_mesh->pkt_send_num;
    operate->ping_pkt_interval = hccsping_mesh->pkt_interval;
    operate->task_id = hccsping_mesh->task_id;
    operate->phy_id = phy_id;
    return DCMI_OK;
}
 
static int dcmi_start_hccs_ping_mesh_proc(int card_id, int device_id, int port_id,
                                          struct dcmi_hccsping_mesh_operate *hccsping_mesh)
{
    int ret = 0;
    struct hccs_ping_mesh_operate operate = {0};
    int device_logic_id = 0;
    unsigned int phy_id = 0;
 
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dcmi_get_phyid_by_cardid_and_devid(card_id, device_id, &phy_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Get phyid failed. (card_id=%d; device_id=%d; ret=%d)", card_id, device_id, ret);
        return ret;
    }
    // 目的地址校验
    ret = dcmi_check_hccsping_mesh_addr(hccsping_mesh, &operate);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_check_hccsping_mesh_addr failed. err is %d.", ret);
        return ret;
    }

    // 配置参数校验
    ret = dcmi_get_hccsping_mesh_param(hccsping_mesh, &operate, phy_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_hccsping_mesh_param failed. err is %d.", ret);
        return ret;
    }
 
    ret = dsmi_start_hccs_ping_mesh(device_logic_id, &operate);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi start hccs ping mesh failed. (ret=%d)", ret);
        ret = dcmi_convert_error_code(ret);
    }
 
    return ret;
}
 
int dcmi_start_hccsping_mesh(int card_id, int device_id, int port_id,
                             struct dcmi_hccsping_mesh_operate *hccsping_mesh)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (dcmi_check_port_id_valid(port_id) == DCMI_ERR_CODE_INVALID_PARAMETER) {
        gplog(LOG_ERR, "port_id is invalid. Input portid is %d.", port_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (hccsping_mesh == NULL) {
        gplog(LOG_ERR, "hccsping_mesh is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root() || dcmi_is_in_privileged_docker_root())) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
 
    if (!dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support start hccs ping mesh.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        gplog(LOG_INFO, "HCCS PINGMESH start!");
        return dcmi_start_hccs_ping_mesh_proc(card_id, device_id, port_id, hccsping_mesh);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
 
int dcmi_stop_hccsping_mesh(int card_id, int device_id, int port_id, unsigned int task_id)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    int device_logic_id = 0;

    if (dcmi_check_port_id_valid(port_id) == DCMI_ERR_CODE_INVALID_PARAMETER) {
        gplog(LOG_ERR, "port_id is invalid. Input portid is %d.", port_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (task_id < 0 || task_id > MAX_TASK_ID) {
        gplog(LOG_ERR, "task_id %u is invalid.", task_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    
    if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root() || dcmi_is_in_privileged_docker_root())) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
    
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
    
    if (!dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support start hccs ping mesh.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        gplog(LOG_INFO, "HCCS PINGMESH stop!");
        ret = dsmi_stop_hccs_ping_mesh(device_logic_id, (int)task_id);
        if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
            gplog(LOG_ERR, "call dsmi stop hccs ping mesh failed. (ret=%d)", ret);
            ret = dcmi_convert_error_code(ret);
        }
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return ret;
}

STATIC int dcmi_get_hccs_ping_mesh_info_status(int device_logic_id, unsigned int task_id,
                                               struct dcmi_hccsping_mesh_info_v2 *hccsping_mesh_reply)
{
    int ret;
    struct ping_mesh_stat *reply_info = (struct ping_mesh_stat *)calloc(1, sizeof(struct ping_mesh_stat));
    if (reply_info == NULL) {
        gplog(LOG_ERR, "calloc reply_info failed.");
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }

    ret = dsmi_get_hccs_ping_mesh_info(device_logic_id, (int)task_id, reply_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi get hccs ping mesh info failed. (ret=%d)", ret);
        ret = dcmi_convert_error_code(ret);
        goto PING_EXIT;
    }

    // 两个结构体存在字节偏移，需分开赋值
    ret = memcpy_s(&hccsping_mesh_reply->info, sizeof(struct dcmi_hccsping_mesh_info),
                   reply_info, sizeof(struct dcmi_hccsping_mesh_info));
    ret += memcpy_s(&hccsping_mesh_reply->L1_plane_check_res, sizeof(hccsping_mesh_reply->L1_plane_check_res),
                    &reply_info->L1_plane_check_res, sizeof(reply_info->L1_plane_check_res));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Dcmi set dcmi_hccsping_mesh_info_v2 memcpy failed! (ret=%d)", ret);
        ret = DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

PING_EXIT:
    free(reply_info);
    return ret;
}

int dcmi_get_hccsping_mesh_info_v2(int card_id, int device_id, int port_id, unsigned int task_id,
                                   struct dcmi_hccsping_mesh_info_v2 *hccsping_mesh_reply)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    int device_logic_id = 0;

    if (dcmi_check_port_id_valid(port_id) == DCMI_ERR_CODE_INVALID_PARAMETER) {
        gplog(LOG_ERR, "port_id is invalid. Input portid is %d.", port_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (hccsping_mesh_reply == NULL) {
        gplog(LOG_ERR, "hccsping_mesh_reply is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (task_id < 0 || task_id > MAX_TASK_ID) {
        gplog(LOG_ERR, "task_id %u is invalid.", task_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root() || dcmi_is_in_privileged_docker_root())) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
 
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
 
    if (!dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support start hccs ping mesh.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        return dcmi_get_hccs_ping_mesh_info_status(device_logic_id, task_id, hccsping_mesh_reply);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return ret;
}

int dcmi_get_hccsping_mesh_info(int card_id, int device_id, int port_id, unsigned int task_id,
                                struct dcmi_hccsping_mesh_info *hccsping_mesh_reply)
{
    int ret;
    struct dcmi_hccsping_mesh_info_v2 info = {0};

    if (hccsping_mesh_reply == NULL) {
        gplog(LOG_ERR, "hccsping_mesh_reply is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_hccsping_mesh_info_v2(card_id, device_id, port_id, task_id, &info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_hccsping_mesh_info_v2 failed. err is %d.", ret);
        return ret;
    }

    memcpy_s(hccsping_mesh_reply, sizeof(struct dcmi_hccsping_mesh_info),
             &info.info, sizeof(struct dcmi_hccsping_mesh_info));

    return DCMI_OK;
}
 
int dcmi_get_hccsping_mesh_state(int card_id, int device_id, int port_id, unsigned int task_id, unsigned int *state)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    int device_logic_id = 0;

    if (dcmi_check_port_id_valid(port_id) == DCMI_ERR_CODE_INVALID_PARAMETER) {
        gplog(LOG_ERR, "port_id is invalid. Input portid is %d.", port_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (state == NULL) {
        gplog(LOG_ERR, "state is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (task_id < 0 || task_id > MAX_TASK_ID) {
        gplog(LOG_ERR, "task_id %u is invalid.", task_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root() || dcmi_is_in_privileged_docker_root())) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
 
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
 
    if (!dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support start hccs ping mesh.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        ret = dsmi_get_hccs_ping_mesh_state(device_logic_id, (int)task_id, state);
        if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
            gplog(LOG_ERR, "call dsmi get hccs ping state failed. (ret=%d)", ret);
            ret = dcmi_convert_error_code(ret);
        }
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return ret;
}

STATIC int dcmi_get_npu_pfc_duration_info(int card_id, int device_id,
	   struct dcmi_pfc_duration_info *pfc_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
    ret = dsmi_get_pfc_duration_info(device_logic_id, (struct pfc_duration_info *)pfc_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_pfc_duration_info failed. err is %d.", ret);
    }
 
    return dcmi_convert_error_code(ret);
}

STATIC int dcmi_clear_npu_pfc_duration(int card_id, int device_id)
{
    int ret;
    int device_logic_id = 0;
    int mode = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
 	
    ret = dsmi_clear_pfc_duration(device_logic_id, mode);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_clear_pfc_duration failed. err is %d.", ret);
    }
 
    return dcmi_convert_error_code(ret);
}

int dcmi_get_pfc_duration_info(int card_id, int device_id, struct dcmi_pfc_duration_info *pfc_info)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    unsigned int main_board_id;
	
    if (pfc_info == NULL) {
        gplog(LOG_ERR, "pfc_duration_info is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_board_chip_type_is_ascend_910b() && !dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support clear pfc duration.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get pfc duration.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_mainboard_id(card_id, 0, &main_board_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to query main board id of card. err is %d.", ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID1) ||
        (main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID2)) {
            gplog(LOG_OP, "This device does not support clear pfc duration.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_pfc_duration_info(card_id, device_id, pfc_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_clear_pfc_duration(int card_id, int device_id)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    unsigned int main_board_id;

    if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root() || dcmi_is_in_privileged_docker_root())) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if ((!dcmi_board_chip_type_is_ascend_910b() && !dcmi_board_chip_type_is_ascend_910_93())) {
        gplog(LOG_OP, "This device does not support clear pfc duration.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support clear pfc duration.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    
    ret = dcmi_get_mainboard_id(card_id, 0, &main_board_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to query main board id of card. err is %d", ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    // 天工不支持
    if ((main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID1) ||
        (main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID2)) {
            gplog(LOG_OP, "This device does not support clear pfc duration.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        return dcmi_clear_npu_pfc_duration(card_id, device_id);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}


STATIC int dcmi_get_qpn_list_status(int card_id, int device_id, int port_id, struct dcmi_qpn_list *list)
{
    int ret;
    int logic_id = 0;

    ret = dcmi_check_port_id_valid(port_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "The port_id is invalid. (portid=%d; ret=%d)", port_id, ret);
        return ret;
    }

    ret = dcmi_get_device_logic_id(&logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_logic_id failed. (ret=%d)", ret);
        return ret;
    }

    ret = dsmi_get_qpn_list(logic_id, port_id, (struct ds_qpn_list *)list);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "Call dsmi_get_qpn_list failed. (ret=%d)", ret);
    }
 
    return dcmi_convert_error_code(ret);
}

int dcmi_get_qpn_list(int card_id, int device_id, int port_id, struct dcmi_qpn_list *list)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (list == NULL) {
        gplog(LOG_ERR, "The list is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_OP, "This device does not support get qpn list.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (!(dcmi_board_chip_type_is_ascend_910_93() || dcmi_board_chip_type_is_ascend_910b())) {
        gplog(LOG_OP, "This device does not support get qpn list.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get qpn list.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_qpn_list_status(card_id, device_id, port_id, list);
    } else {
        gplog(LOG_ERR, "The device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_get_qp_info_status(int card_id, int device_id, int port_id,
                                   unsigned int qpn, struct dcmi_qp_info *qp_info)
{
    int ret;
    int logic_id = 0;

    ret = dcmi_check_port_id_valid(port_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "The port_id is invalid. (portid=%d; ret=%d)", port_id, ret);
        return ret;
    }

    ret = dcmi_get_device_logic_id(&logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_logic_id failed. (ret=%d)", ret);
        return ret;
    }

    ret = dsmi_get_qp_info(logic_id, port_id, qpn, (struct ds_qp_info *)qp_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "Call dsmi_get_qp_info failed. (ret=%d)", ret);
    }
    
    return dcmi_convert_error_code(ret);
}

int dcmi_get_qp_info(int card_id, int device_id, int port_id,
                     unsigned int qpn, struct dcmi_qp_info *qp_info)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    
    if (qp_info == NULL) {
        gplog(LOG_ERR, "The qp_info is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_OP, "This device does not support get qp context.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (!(dcmi_board_chip_type_is_ascend_910_93() || dcmi_board_chip_type_is_ascend_910b())) {
        gplog(LOG_OP, "This device does not support get qp context.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get qp context.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_qp_info_status(card_id, device_id, port_id, qpn, qp_info);
    } else {
        gplog(LOG_ERR, "The device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int covert_dcmi_extra_statistics_info(struct ds_extra_statistics_info *ds_info,
                                             struct dcmi_extra_statistics_info *info)
{
    if (ds_info == NULL || info == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    info->cw_total_cnt = ds_info->cw_total_cnt;
    info->cw_before_correct_cnt = ds_info->cw_before_correct_cnt;
    info->cw_correct_cnt = ds_info->cw_correct_cnt;
    info->cw_uncorrect_cnt = ds_info->cw_uncorrect_cnt;
    info->cw_bad_cnt = ds_info->cw_bad_cnt;
    info->trans_total_bit = ds_info->trans_total_bit;
    info->cw_total_correct_bit = ds_info->cw_total_correct_bit;
    info->rx_full_drop_cnt = ds_info->drop_num;  // drop_num 映射到 rx_full_drop_cnt
    info->pcs_err_cnt = ds_info->pcs_err_count;  // pcs_err_count 映射到 pcs_err_cnt
    info->rx_send_app_good_pkts = ds_info->rx_send_app_good_pkts;
    info->rx_send_app_bad_pkts = ds_info->rx_send_app_bad_pkts;

    // 计算校正比特率
    if (ds_info->trans_total_bit > 0) {
        info->correcting_bit_rate = (double)ds_info->cw_total_correct_bit / (double)ds_info->trans_total_bit;
    } else {
        info->correcting_bit_rate = 0.0;  // 防止除以零
    }

    return DCMI_OK;
}
                                

int dcmi_get_extra_statistics_info(int card_id, int device_id, int port_id,
                                   struct dcmi_extra_statistics_info *dcmi_info)
{
    int ret;
    int logic_id = 0;
    struct ds_extra_statistics_info ds_info = {0};
    enum dcmi_unit_type device_type = INVALID_TYPE;
 
    if (dcmi_info == NULL) {
        gplog(LOG_ERR, "The statistics info is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_check_port_id_valid(port_id) == DCMI_ERR_CODE_INVALID_PARAMETER) {
        gplog(LOG_ERR, "port_id is invalid. Input portid is %d.", port_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    if (!(dcmi_board_chip_type_is_ascend_910_93() || dcmi_board_chip_type_is_ascend_910b())) {
        gplog(LOG_OP, "This device does not support get extra statistics info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_OP, "This device does not support get extra statistics info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_type failed. (ret=%d)", ret);
        return ret;
    }
 
    ret = dcmi_get_device_logic_id(&logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_logic_id failed. (ret=%d)", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        ret = dsmi_get_extra_statistics_info(logic_id, port_id, &ds_info);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "Call dsmi_get_extra_statistics_info failed. (ret=%d)", ret);
            return dcmi_convert_error_code(ret);
        }
        covert_dcmi_extra_statistics_info(&ds_info, dcmi_info);
        return 0;
    } else {
        gplog(LOG_ERR, "The device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
