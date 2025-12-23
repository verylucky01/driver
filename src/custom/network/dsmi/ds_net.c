/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ds_net.h"
#include "securec.h"
#include "user_log.h"
#include <arpa/inet.h>
#include <unistd.h>
#include "dms_devdrv_info_comm.h"
#include "ascend_hal.h"
#include "hccn_comm.h"
#include "hccn_tool.h"
#include "dsmi_device_info.h"

int dsmi_inet_ntop_ipv4_address(const unsigned int *address)
{
    const char *str = NULL;
    char net_addr[MAX_IP_LEN] = {0};

    str = inet_ntop(AF_INET, address, net_addr, sizeof(net_addr));
    if (str == NULL) {
        roce_err("the value of gateway 0x%x is a invalid!", *address);
        return -EINVAL;
    }

    return 0;
}

int dsmi_inet_ntop_ip_address(struct ipv_addr *address)
{
    const char *result = NULL;
    char net_addr[MAX_IP_LEN] = {0};
    char net_addr_ipv6[MAX_IPV6_LEN] = {0};

    if (address == NULL) {
        roce_err("the address is invalid!");
        return -EINVAL;
    }

    if (address->ip_flag == 1) {
        result = inet_ntop(AF_INET6, address->ipv6, net_addr_ipv6, sizeof(net_addr_ipv6));
        if (result == NULL) {
            roce_err("the value of ipv6 address is a invalid!");
            return -EINVAL;
        }
    } else {
        result = inet_ntop(AF_INET, &(address->ipv4), net_addr, sizeof(net_addr));
        if (result == NULL) {
            roce_err("the value of address 0x%x is a invalid!", *address);
            return -EINVAL;
        }
    }

    return 0;
}

int dsmi_set_net_detect_ip_address(int logic_id, struct ipv_addr *ip_address)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    ret = dsmi_inet_ntop_ip_address(ip_address);
    if (ret) {
        roce_err("dsmi set net detected address fail: ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_NET_DETECT, (char*)ip_address, sizeof(struct ipv_addr), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set net detect ip address fail: ret[%d] logic_id[%d] ipv4 address_ip[0x%x], ipv6 address_ip[%s]",
            ret, logic_id, ip_address->ipv4, ip_address->ipv6);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set net detect ip error: result[%d] logic_id[%d] ipv4 address_ip[0x%x], ipv6 address_ip[%s]",
            trans_data.result, logic_id, ip_address->ipv4, ip_address->ipv6);
    }

    return trans_data.result;
}

int dsmi_get_net_detect_ip_address(int logic_id, int port_id, struct ipv_addr *ip_address)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((ip_address == NULL) || (logic_id < 0) || (logic_id > DS_MAX_LOGIC_ID)) {
        roce_err("invalid param, ip=%p, logic_id=%d", ip_address, logic_id);
        return -EINVAL;
    }

    size = sizeof(struct ipv_addr);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_NET_DETECT, (char*)ip_address,
                        sizeof(struct ipv_addr), (char *)ip_address, &size);
    ret = memcpy_s(trans_data.outbuf, sizeof(struct ipv_addr), ip_address, sizeof(struct ipv_addr));
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi_get_net_detect_ip_address fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi_get_net_detect_ip_address fail result[%d], logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_set_default_gateway_address(int logic_id, int port, struct ds_gateway_addr gateway_addr)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    /* before set new gateway, check the value of gateway is correct */
    ret = dsmi_inet_ntop_ipv4_address(&gateway_addr.address);
    if (ret) {
        roce_err("dsmi set default gateway address fail:invalid gateway 0x%x, ret[%d] logic_id[%d]",
                 gateway_addr.address, ret, logic_id);
        return ret;
    }

    ret = hccn_check_usr_identify();
    if (ret) {
        roce_err("Check usr identify failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    roce_info("dsmi set default gateway info: logic id[%d] port id[%d] gateway[0x%x]",
              logic_id, port, gateway_addr.address);

    gateway_addr.port = port;
    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_DEFAULT_GATEWAY, (char*)&gateway_addr, sizeof(gateway_addr), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set default gateway address fail ret[%d] logic_id[%d] gateway[0x%x]",
            ret, logic_id, gateway_addr.address);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set default gateway error result[%d] logic_id[%d] gateway[0x%x]",
            trans_data.result, logic_id, gateway_addr.address);
    }

    return trans_data.result;
}

int dsmi_del_default_gateway_address(int logic_id, int port, unsigned int gateway, bool is_gateway_on_eth)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    struct ds_gateway_addr gateway_addr = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    /* before deleting   target gateway, check the value of gateway is correct */
    ret = dsmi_inet_ntop_ipv4_address(&gateway);
    if (ret) {
        roce_err("dsmi delete default gateway address fail:invalid gateway 0x%x, ret[%d] logic_id[%d]",
                 gateway, ret, logic_id);
        return ret;
    }

    ret = hccn_check_usr_identify();
    if (ret) {
        roce_err("Check usr identify failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    roce_info("dsmi del default gateway info: logic id[%d] port id[%d] gateway[0x%x]",
              logic_id, port, gateway);

    gateway_addr.address = gateway;
    gateway_addr.port = port;
    gateway_addr.is_gateway_on_eth = is_gateway_on_eth;
    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_DEL_DEFAULT_GATEWAY, (char*)&gateway_addr, sizeof(gateway_addr), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi del default gateway address fail ret[%d] logic_id[%d] gateway[0x%x]",
            ret, logic_id, gateway);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi del default gateway error result[%d] logic_id[%d] gateway[0x%x]",
            trans_data.result, logic_id, gateway);
    }

    return trans_data.result;
}

int dsmi_get_default_gateway_address(int logic_id, int *port, unsigned int *gateway,
    unsigned int *default_phy_id, bool is_gateway_on_eth)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    struct ds_gateway_addr gateway_addr = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(port, -EINVAL);
    DSMI_CHECK_PTR_VALID_RETURN_VAL(gateway, -EINVAL);

    gateway_addr.is_gateway_on_eth = is_gateway_on_eth;
    size = sizeof(struct ds_gateway_addr);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_DEFAULT_GATEWAY, (char*)&gateway_addr, size, (char*)&gateway_addr, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get default gateway address fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        if (trans_data.result == -ENOENT) {
            roce_warn("logic_id[%d] without default gateway address.", logic_id);
        } else {
            roce_err("dsmi get default gateway error result[%d] logic_id[%d]", trans_data.result, logic_id);
        }
    } else {
        *port = gateway_addr.port;
        *gateway = gateway_addr.address;
        *default_phy_id = gateway_addr.default_phy_id;
    }

    return trans_data.result;
}

int dsmi_lldp_get_neighbor_info(int logic_id)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    char lldp_info[MAX_CMD_PAYLOAD_LEN] = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size = sizeof(lldp_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_NET_LLDP_NEIGHBOR_INFO, NULL, 0, lldp_info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get lldp neighbor info fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get lldp neighbor info result[%d] logic_id[%d]", trans_data.result, logic_id);
    } else {
        lldp_info[MAX_CMD_PAYLOAD_LEN - 1] = '\0';
        DSMI_PRINT_INFO("%s", lldp_info);
    }

    return trans_data.result;
}

int dsmi_lldp_get_local_info(int logic_id)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    char lldp_info[MAX_CMD_PAYLOAD_LEN] = {0};
    unsigned int size;
 
    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
 
    size = sizeof(lldp_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_NET_LLDP_LOCAL_INFO, NULL, 0, lldp_info, &size);
 
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("Dsmi get lldp local info fail. (ret=%d, logic_id=%d)", ret, logic_id);
        return ret;
    }
 
    if (trans_data.result != 0) {
        roce_err("Dsmi get lldp local info. (result=%d, logic_id=%d)", trans_data.result, logic_id);
    } else {
        lldp_info[MAX_CMD_PAYLOAD_LEN - 1] = '\0';
        DSMI_PRINT_INFO("%s", lldp_info);
    }
 
    return trans_data.result;
}

int dsmi_set_lldp_port_id(int logic_id, int port_id_type)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;
 
    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    if (port_id_type < 0 || port_id_type > 1) {
        roce_err("The value of port_id_type is invalid. (port_id_type=%d)", port_id_type);
        return -EINVAL;
    }
    ret = hccn_check_usr_identify();
    if (ret) {
        roce_err("Check usr identify failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }
 
    roce_info("dsmi set lldp port id config info. (logic id=%d; port id cfg=%d)",
              logic_id, port_id_type);
 
    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_LLDP_PORT_TYPE, (char*)&port_id_type, sizeof(port_id_type), NULL, &size);
 
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set lldp port id fail. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }
 
    if (trans_data.result != 0) {
        roce_err("dsmi set netdev down. (result=%d; logic_id=%d)", trans_data.result, logic_id);
    }
 
    return trans_data.result;
}

int dsmi_get_mac_address(int logic_id, int port, unsigned char *mac_addr)
{
    int ret;
    unsigned int size;
    struct ds_trans_data trans_data = {0};

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    if (mac_addr == NULL) {
        roce_err("mac addr is NULL");
        return -EINVAL;
    }

    size = DS_MAC_ADDR_LEN;
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_MAC_ADDRESS, NULL, 0, (char *)mac_addr, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get mac address fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get mac address result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

static char *g_ds_mac_filter_list[] = {
    "09:00:2B:00:00:04", "09:00:2B:00:00:05", "03:00:00:00:00:08",
    "03:00:00:00:00:10", "03:00:00:00:00:40", "03:00:00:00:01:00",
    "03:00:00:00:02:00", "03:00:00:00:04:00", "03:00:00:00:08:00",
    "03:00:00:00:10:00", "03:00:00:00:20:00", "03:00:00:00:40:00",
    "03:00:00:40:00:00", "00:00:00:00:00:00", "FF:FF:FF:FF:FF:FF",
    "ff:ff:ff:ff:ff:ff", "09:00:2b:00:00:04", "09:00:2b:00:00:05"
};

int dsmi_mac_filter_check(const unsigned char *mac_addr)
{
    int ret;
    int index;
    char mac_pieces[DS_MAC_FILTER_SIZE] = {0};

    ret = snprintf_s(mac_pieces, DS_MAC_FILTER_SIZE, DS_MAC_FILTER_SIZE - 1, "%02x:%02x:%02x:%02x:%02x:%02x",
                     mac_addr[0x0], mac_addr[0x1], mac_addr[0x2],
                     mac_addr[0x3], mac_addr[0x4], mac_addr[0x5]);
    if (ret < 0) {
        roce_err("snprintf_s fail, ret:%d, dest_len:%d", ret, DS_MAC_FILTER_SIZE);
        return -EINVAL;
    }

    if (!strncmp(mac_pieces, DS_MAC_FILTER_PRE_UPPER, strlen(DS_MAC_FILTER_PRE_UPPER))) {
        roce_err("strncmp argv error, upper case check failed");
        return -EINVAL;
    }

    if (!strncmp(mac_pieces, DS_MAC_FILTER_PRE_LOWER, strlen(DS_MAC_FILTER_PRE_LOWER))) {
        roce_err("strncmp argv error, lower case check failed");
        return -EINVAL;
    }

    for (index = 0; index < DS_MAC_FILTER_SIZE; index++) {
        if (!strncmp(mac_pieces, g_ds_mac_filter_list[index], DS_MAC_STR_LEN)) {
            roce_err("strncmp argv error, mac_address[%s], index[%d]", mac_pieces, index);
            return -EINVAL;
        }
    }
    return 0;
}

int dsmi_set_mac_address(int logic_id, int port, const unsigned char *mac_addr)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    if (mac_addr == NULL) {
        roce_err("mac addr is null, logic_id[%d]", logic_id);
        return -EINVAL;
    }

    ret = dsmi_mac_filter_check(mac_addr);
    if (ret) {
        roce_err("invalid mac -- [%02x:%02x:%02x:%02x:%02x:%02x]",
                 mac_addr[0x0], mac_addr[0x1], mac_addr[0x2],
                 mac_addr[0x3], mac_addr[0x4], mac_addr[0x5]);
        return ret;
    }

    ret = hccn_check_usr_identify();
    if (ret) {
        roce_err("Check usr identify failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    roce_info("dsmi set mac addr info: logic id[%d] port id[%d] mac addr[%02x:%02x:%02x:%02x:%02x:%02x]",
              logic_id, port, mac_addr[0x0], mac_addr[0x1], mac_addr[0x2], mac_addr[0x3], mac_addr[0x4], mac_addr[0x5]);

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_MAC_ADDRESS, (char*)mac_addr, DS_MAC_ADDR_LEN, NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set mac address fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set mac address result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_set_netdev_link(int logic_id, int port, int enable_flag)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port);
        return -EINVAL;
    }
    if (enable_flag != ENABLE && enable_flag != DISABLE) {
        roce_err("The value of en is invalid. (enable=%d)", enable_flag);
        return -EINVAL;
    }
    ret = hccn_check_usr_identify();
    if (ret != 0) {
        roce_err("Check usr identify failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    roce_info("dsmi set netdev link info: logic id[%d] port id[%d] link enable[%d]",
              logic_id, port, enable_flag);

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_NETDEV_STATUS, (char*)&enable_flag, sizeof(enable_flag), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi set netdev down fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set netdev down result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_netdev_link(int logic_id, int port, int *link)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    if (link == NULL) {
        roce_err("link is null, logic_id[%d]", logic_id);
        return -EINVAL;
    }

    size = sizeof(int);
    DSMI_SET_TRANS_DATA(trans_data, DS_INSPECT_ETH_STATUS, NULL, 0, (char*)link, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi_get_netdev_link fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi_get_netdev_link result[%d]", trans_data.result);
    }

    return trans_data.result;
}

int dsmi_set_mtu(int logic_id, int port_id, unsigned int mtu)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    char mtu_s[DS_MTU_LEN] = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    if (mtu < DSMI_MIN_MTU_SIZE || mtu > DSMI_MAX_MTU_SIZE) {
        roce_err("invalid mtu size-- '%u', expect[%d]-[%d]", mtu, DSMI_MIN_MTU_SIZE, DSMI_MAX_MTU_SIZE);
        return -EINVAL;
    }

    ret = sprintf_s(mtu_s, DS_MTU_LEN, "%u", mtu);
    if (ret <= 0) {
        roce_err("sprintf ds_cmd.info mtu err ret %d", ret);
        return -ENOMEM;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_MTU, mtu_s, DS_MTU_LEN, NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set mtu fail ret[%d] logic_id[%d] mtu[%u] port[%d]", ret, logic_id, mtu, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("set mtu fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_mtu(int logic_id, int port_id, unsigned int *mtu)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    char mtu_s[DS_MTU_LEN] = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    if (mtu == NULL) {
        roce_err("null point mtu, logic_id[%d]", logic_id);
        return -EINVAL;
    }

    size = DS_MTU_LEN;
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_MTU, NULL, 0, (char*)mtu_s, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get mtu fail ret[%d] logic_id[%d] mtu[%u]", ret, logic_id, *mtu);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("get mtu fail result[%d], logic_id[%d]", trans_data.result, logic_id);
        return trans_data.result;
    }

    *mtu = strtol(mtu_s, NULL, NUMBER_BASE);

    return trans_data.result;
}

int dsmi_set_dscp_map(int logic_id, unsigned int port_id, unsigned char dscp_val, unsigned char tc_val)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    struct ds_cmd_tos tos_param;
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    if ((port_id) < 0 ||  (port_id) > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return (-EINVAL);
    }
    if (dscp_val > DSCP_MAX) {
        roce_err("dscp[%u] > [%d](DSCP_MAX) is invalid!", dscp_val, DSCP_MAX);
        return -EINVAL;
    }

    if (tc_val > TC_MAX) {
        roce_err("tc[%u] > [%d](TC_MAX) is invalid!", tc_val, TC_MAX);
        return -EINVAL;
    }
    tos_param.dscp = dscp_val;
    tos_param.tc = tc_val;
    tos_param.port_id = port_id;
    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_DSCP_MAP, (char*)&tos_param, sizeof(tos_param), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set dscp fail ret[%d] logic_id[%d] dscp[%u] tc[%u].",
            ret, logic_id, dscp_val, tc_val);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set dscp fail, result[%d] logic_id[%d] dscp[%u] tc[%u].",
            trans_data.result, logic_id, dscp_val, tc_val);
    }

    return trans_data.result;
}

int dsmi_get_dscp_map(int logic_id, unsigned int port_id, unsigned char dscp_val, unsigned char *tc_val)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned char tos_tc[DS_MAX_USER_TOS];
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    if ((port_id) < 0 ||  (port_id) > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return (-EINVAL);
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(tc_val, -EINVAL);
    if (dscp_val >= DS_MAX_USER_TOS) {
        roce_err("dsmi get dscp map fail, dscp_val beyond range, max value %d.", DS_MAX_USER_TOS);
        return -EINVAL;
    }
    size = sizeof(tos_tc);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_DSCP_MAP, (char *)&port_id, sizeof(port_id), (char*)&tos_tc, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get dscp to tc fail, ret[%d] logic_id[%d].", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get dscp to tc fail, result[%d] logic_id[%d].", trans_data.result, logic_id);
    } else {
        *tc_val = tos_tc[dscp_val];
    }

    return trans_data.result;
}

int dsmi_set_port_shaping(int logic_id, unsigned int port_id, int bw_limit)
{
    struct ds_cmd_shaping_para shaping_param = {0};
    struct ds_trans_data trans_data = {0};
    unsigned int size;
    int ret;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    shaping_param.port_id = port_id;
    shaping_param.bw_limit = bw_limit;
    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_TM_SHAPING_PORT, (char *)&shaping_param, sizeof(shaping_param), NULL, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi shaping port set fail ret[%d] logic_id[%d] port_id[%u] bw_limit[%d]",
                 ret, logic_id, port_id, bw_limit);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi shaping port set fail result[%d] logic_id[%d] port_id[%u] bw_limit[%d]",
                 trans_data.result, logic_id, port_id, bw_limit);
    }

    return trans_data.result;
}

int dsmi_get_route_table(int logic_id, int port_id, char *route, unsigned int len)
{
    int ret;
    struct ds_trans_data trans_data = {0};

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(route, -EINVAL);
    if (len == 0) {
        roce_err("len is invalid len[%d]", len);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ROUTE_TABLE, NULL, 0, route, &len);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get route fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get route fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }
    return trans_data.result;
}

int dsmi_add_route_table(int logic_id, int port_id, struct ds_route_table_value *route_param, char *outbuf,
    unsigned int len)
{
    int ret;
    struct ds_trans_data trans_data = {0};

    if (logic_id > DS_MAX_LOGIC_ID || logic_id < 0) {
        roce_err("device id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if (port_id < 0 ||  port_id > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return (-EINVAL);
    }

    if (outbuf == NULL) {
        roce_err("outbuf is NULL!");
        return -EINVAL;
    }

    if (len == 0) {
        roce_err("len is invalid len[%u]", len);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_ADD_ROUTE_TABLE, (char*)route_param,
        sizeof(struct ds_route_table_value), outbuf, &len);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi add route fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi add route fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_del_route_table(int logic_id, int port_id, struct ds_route_table_value *route_param, char *outbuf,
    unsigned int len)
{
    int ret;
    struct ds_trans_data trans_data = {0};

    if (logic_id > DS_MAX_LOGIC_ID || logic_id < 0) {
        roce_err("device id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if (port_id < 0 ||  port_id > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return (-EINVAL);
    }

    if (outbuf == NULL) {
        roce_err("outbuf is NULL!");
        return -EINVAL;
    }

    if (len == 0) {
        roce_err("len is invalid len[%u]", len);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_DELETE_ROUTE_TABLE, (char*)route_param,
        sizeof(struct ds_route_table_value), outbuf, &len);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi del route fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi del route fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }
    return trans_data.result;
}

int dsmi_get_pci_info(int logic_id, int port_id, char *pci, unsigned int len)
{
    int ret;
    struct ds_trans_data trans_data = {0};

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(pci, -EINVAL);
    if (len == 0) {
        roce_err("len is invalid len[%u]", len);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_PCI_INFO, NULL, 0, pci, &len);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get pci fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get pci fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_roce_context(int logic_id, int port_id,
    struct dsmi_roce_context_factors roce_context_factors, char *context)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    struct ds_cmd_context cmd_context;
    cmd_context.type = roce_context_factors.type;
    cmd_context.index = roce_context_factors.index;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(context, -EINVAL);

    if (roce_context_factors.len == 0) {
        roce_err("roce_context_factors len[%d] is invalid.", roce_context_factors.len);
        return -EINVAL;
    }

    if (roce_context_factors.type < DS_TYPE_QPC || roce_context_factors.type > DS_TYPE_MAX) {
        roce_err("roce_context_factors type[%d] is invalid , valid range[%d - %d]",
            roce_context_factors.type, DS_TYPE_QPC, DS_TYPE_MAX);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ROCE_CONTEXT, (char*)&cmd_context,
        sizeof(struct ds_cmd_context), context, &(roce_context_factors.len));

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get context fail ret[%d] logic_id[%d] port[%d] type[%d]",
                 ret, logic_id, port_id, roce_context_factors.type);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get roce context fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_eth_reg_info(int logic_id, int port_id, char *info, unsigned int len)
{
    int ret;
    struct ds_trans_data trans_data = {0};

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(info, -EINVAL);
    if (len == 0) {
        roce_err("len is invalid len[%d]", len);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ETH_REG_INFO, NULL, 0, info, &len);
    trans_data.pid = getpid();

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get eth reg fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get eth reg fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_network_register(int logic_id, int port_id, unsigned long long addr, unsigned long *value)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size = sizeof(unsigned int);

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(value, -EINVAL);

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_NETWORK_REGISTER, (char*)(&addr), sizeof(unsigned int), (char*)value, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi_get_network_register fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi_get_network_register fail result[%d] logic_id[%d] port[%d]",
            trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_optical_info(int logic_id, int port_id, struct ds_optical_info *info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(info, -EINVAL);

    size = sizeof(struct ds_optical_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_OPTICAL_INFO, NULL, 0, (char*)info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get optical info fail ret[%d] logic_id[%d] port_id[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get optical fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_netdev_stat(int logic_id, int port, struct ds_port_stat_info *stat)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(stat, -EINVAL);

    size = sizeof(struct ds_port_stat_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_PACKET_STATISTICS, NULL, 0, (char*)stat, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get stat fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get stat fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port);
    }

    return trans_data.result;
}


int dsmi_get_tls_cfg(int logic_id, int port_id, struct tls_cert_show_info show_info[], unsigned int num)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(show_info, -EINVAL);

    size = sizeof(struct tls_cert_show_info) * num;
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_TLS_CFG, NULL, 0, (char*)show_info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get tls cfg fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0 && trans_data.result != (-ENOENT)) {
        roce_err("dsmi get tls cfg fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_clear_netdev_stat(int logic_id, int port)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_CLEAR_PACKET_STATISTICS, NULL, 0, NULL, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi clear stat fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi clear stat fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port);
    }

    return trans_data.result;
}

int dsmi_set_tls_cfg(int logic_id, struct tls_cert_ky_crl_info *tls_cfg)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(tls_cfg, -EINVAL);

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_TLS_CFG, (char*)tls_cfg,
        sizeof(struct tls_cert_ky_crl_info), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set tls cfg fail, ret:%d, logic_id:%d", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set tls cfg fail result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_clear_tls_cfg(int logic_id, struct tls_clear_info *clear_info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(clear_info, -EINVAL);

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_CLEAR_TLS_CFG, (char*)clear_info,
        sizeof(struct tls_clear_info), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi clear tls cfg fail, ret:%d, logic_id:%d", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi clear tls cfg fail result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_set_tls_enable(int logic_id, struct tls_enable_info *enable_info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;
    unsigned int enable;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    if (enable_info == NULL) {
        roce_err("enable_info is NULL");
        return -EINVAL;
    }

    enable = enable_info->enable;
    if (enable != 0 && enable != 1) {
        roce_err("tls enable[%u] error", enable);
        return -EINVAL;
    }

    ret = dsmi_set_tls_machine_type(logic_id, enable_info);
    if (ret != 0) {
        roce_err("Dsmi_set_tls_machine_type failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    roce_info("dsmi set tls enable info: device id[%d] enable[%d]", logic_id, enable);

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_TLS_ENABLE, (char*)(enable_info),
        sizeof(struct tls_enable_info), NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set tls enable fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set tls enable result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_set_tls_alarm(int logic_id, struct tls_alarm_info *alarm_info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;
    unsigned int alarm;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }
    if (alarm_info == NULL) {
        roce_err("alarm_info is NULL");
        return -EINVAL;
    }

    alarm = alarm_info->alarm;
    if ((alarm != DSMI_TLS_ALARM_DISABLE) && (alarm < DSMI_TLS_ALARM_MIN_DAYS || alarm > DSMI_TLS_ALARM_MAX_DAYS)) {
        roce_err("tls alarm[%u] error", alarm);
        return -EINVAL;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_TLS_ALARM, (char*)(alarm_info),
        sizeof(struct tls_alarm_info), NULL, &size);

    roce_info("dsmi set tls alarm info: device id[%d] alarm[%u]", logic_id, alarm);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set tls alarm fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi set tls alarm result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_firmware_version(int logic_id, int port_id, char *version, unsigned int length)
{
    int ret;
    struct ds_trans_data trans_data = {0};

    if ((version == NULL) || (length == 0) || (logic_id < 0) || (logic_id > DS_MAX_LOGIC_ID)) {
        roce_err("invalid param, version=%p, length=%u logic_id=%d", version, length, logic_id);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_FIRMWARE_VERSION, NULL, 0, version, &length);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get firmware version fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("get firmware fail result[%d], logic_id[%d]", trans_data.result, logic_id);
        return trans_data.result;
    }
    roce_info("dsmi_get_firmware_version, logic_id[%d], version[%s], length[%u].", logic_id, version, length);
    return trans_data.result;
}

int dsmi_get_device_process(int logic_id, int port_id, int *found, unsigned int length)
{
    int ret;
    struct ds_trans_data trans_data = {0};

    if ((found == NULL) || (length == 0) || (logic_id < 0) || (logic_id > DS_MAX_LOGIC_ID)) {
        roce_err("invalid param, found=%p, length=%u logic_id=%d", found, length, logic_id);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_DEVICE_PROCESS, NULL, 0, (char *)found, &length);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get device process info fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("get device process info fail result[%d], logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

static int dsmi_network_transmission_channel_para_check(int logic_id, struct ds_trans_data *trans_data)
{
    if (trans_data->inbuf == NULL && trans_data->size_in != 0) {
        roce_err("invalid param, inbuf is NULL, size_in %u", trans_data->size_in);
        return -EINVAL;
    }

    if (trans_data->inbuf != NULL && trans_data->size_in == 0) {
        roce_err("invalid param, inbuf is %p, size_in 0", trans_data->inbuf);
        return -EINVAL;
    }

    if (trans_data->outbuf == NULL && trans_data->size_out == NULL) {
        roce_err("invalid param, outbuf is NULL, sizeout is NULL");
        return -EINVAL;
    }

    if (trans_data->outbuf == NULL && trans_data->size_out != NULL && *(trans_data->size_out) != 0) {
        roce_err("invalid param, outbuf is NULL, sizeout %u", *trans_data->size_out);
        return -EINVAL;
    }

    if (trans_data->outbuf != NULL && trans_data->size_out == NULL) {
        roce_err("invalid param, outbuf is %p, sizeout is NULL", trans_data->outbuf);
        return -EINVAL;
    }

    if (trans_data->outbuf != NULL && trans_data->size_out != NULL && *(trans_data->size_out) == 0) {
        roce_err("invalid param, outbuf is not NULL, sizeout is zero");
        return -EINVAL;
    }

    return 0;
}

static void dsmi_network_transmission_channel_cmd_init(int logic_id, struct ds_common_req_param *ds_common_cmd,
    struct ds_trans_data *trans_data)
{
    ds_common_cmd->req_head.logic_id = logic_id;
    ds_common_cmd->req_head.cmd = trans_data->cmd;
    ds_common_cmd->req_head.send_data_len = trans_data->size_in;
    ds_common_cmd->req_head.recv_data_len = *(trans_data->size_out);
    ds_common_cmd->req_head.host_tid = drvDeviceGetBareTgid();
    ds_common_cmd->req_head.pid = trans_data->pid;
}

static void dsmi_cmd_cleanup_info(struct ds_common_req_param *ds_common_cmd,
    struct ds_common_rsp_param *ds_common_rsp)
{
    int ret1, ret2;
    ret1 = memset_s(ds_common_cmd->info, MAX_CMD_PAYLOAD_LEN, 0, MAX_CMD_PAYLOAD_LEN);
    if (ret1) {
        roce_warn("memset_s ds_req.info fail ret[%d]", ret1);
    }

    ret2 = memset_s(ds_common_rsp->info, MAX_CMD_PAYLOAD_LEN, 0, MAX_CMD_PAYLOAD_LEN);
    if (ret2) {
        roce_warn("memset_s ds_rsp.info fail ret[%d]", ret2);
    }
}

static int dsmi_cmd_get_outbuf_from_every_pkt(struct ds_trans_data *trans_data,
    struct ds_common_rsp_param *ds_common_rsp, int *offset, unsigned int *recv_len, unsigned int orig_len)
{
    int ret;

    /* rdfx response pkt from middle or one pkt */
    if ((orig_len <= MAX_CMD_PAYLOAD_LEN && ds_common_rsp->rsp_head.data_frag != DS_ONE_PKT) ||
        (orig_len > MAX_CMD_PAYLOAD_LEN && ds_common_rsp->rsp_head.data_frag == DS_FIRST_PKT)) {
        roce_err("error rsp from rdfx, orig_len %u, data frag %d", orig_len, ds_common_rsp->rsp_head.data_frag);
        return -EINVAL;
    }

    if (ds_common_rsp->rsp_head.one_piece_len > 0 && trans_data->outbuf != NULL &&
        ds_common_rsp->rsp_head.one_piece_len <= MAX_CMD_PAYLOAD_LEN && ds_common_rsp->rsp_head.one_piece_len != 0) {
        /* get recv data from every pkt */
        ret = memcpy_s(trans_data->outbuf + *offset, *recv_len, ds_common_rsp->info,
            ds_common_rsp->rsp_head.one_piece_len);
        if (ret) {
            roce_err("dsmi memcpy outbuf fail ret[%d], dst_len[%u], src_len[%u]",
                ret, *recv_len, ds_common_rsp->rsp_head.one_piece_len);
            return ret;
        }

        if (ds_common_rsp->rsp_head.one_piece_len > *recv_len) {
            roce_err("one piece len %u is bigger than recv len %u", ds_common_rsp->rsp_head.one_piece_len, *recv_len);
            return -EINVAL;
        }

        roce_info("dsmi get one piece len %u, offset %d", ds_common_rsp->rsp_head.one_piece_len, *offset);
        *offset += ds_common_rsp->rsp_head.one_piece_len;
        *recv_len -= ds_common_rsp->rsp_head.one_piece_len;
    }

    return 0;
}

static int dsmi_set_send_cfg(struct ds_common_req_param *ds_cmd, int len, unsigned int *copy_size,
    const char *buf_tmp, int off_set)
{
    int ret;

    if (ds_cmd->req_head.send_data_len <= MAX_CMD_PAYLOAD_LEN) {
        ds_cmd->req_head.data_frag = DS_ONE_PKT;
        *copy_size = ds_cmd->req_head.send_data_len;
        ds_cmd->req_head.one_piece_len = *copy_size;
    } else {
        *copy_size = (unsigned int)MIN(MAX_CMD_PAYLOAD_LEN, len);
        ds_cmd->req_head.one_piece_len = *copy_size;
        roce_info("copy_size:%d", *copy_size);
        if (off_set == 0) {
            ds_cmd->req_head.data_frag = DS_FIRST_PKT;
        } else if (*copy_size == len) {
            ds_cmd->req_head.data_frag = DS_LAST_PKT;
        } else {
            ds_cmd->req_head.data_frag = DS_MIDDLE_PKT;
        }
    }

    /* in buf size could be zero */
    if (*copy_size != 0 && buf_tmp != NULL) {
        ret = memcpy_s(ds_cmd->info, MAX_CMD_PAYLOAD_LEN, buf_tmp + off_set, *copy_size);
        if (ret) {
            roce_err("dsmi memcpy data to info failed ret[%d] dst_len[%d], off_set[%d], src_len[%u]",
                ret, MAX_CMD_PAYLOAD_LEN, off_set, *copy_size);
            return ret;
        }
    }

    return 0;
}

static int dsmi_send(int logic_id, struct ds_trans_data *trans_data, struct ds_common_req_param *ds_common_cmd,
    struct ds_common_rsp_param *ds_common_rsp)
{
    int ret;
    unsigned int len = trans_data->size_in;
    unsigned int copy_size = 0;
    const char *buf_tmp = trans_data->inbuf;
    unsigned int off_set = 0;
    int result = 0;
    unsigned int size = sizeof(struct ds_common_rsp_param);

    ds_common_cmd->req_head.snd_rcv_op = DS_SEND_OP; /* set send flag to device */
    while (true) {
        ret = dsmi_set_send_cfg(ds_common_cmd, len, &copy_size, buf_tmp, (int)off_set);
        if (ret) {
            roce_err("dev %d set send cfg failed ret %d", logic_id, ret);
            goto out;
        }

        ret = dsmi_cmd_get_network_device_info(logic_id, (char *)ds_common_cmd, sizeof(struct ds_common_req_param),
            (char *)ds_common_rsp, &size);
        if (ret || size != sizeof(struct ds_common_rsp_param)) {
            roce_err("logic_id %d dsmi get network info failed ret %d, ret_size %u != rsp_size %u",
                logic_id, ret, size, sizeof(struct ds_common_rsp_param));
            // 权限不足时返回 -EACCES
            ret = (ret == DRV_ERROR_OPER_NOT_PERMITTED) ? (-EACCES) : (-EINVAL);
            goto out;
        }

        /* get result from the every rsp head result */
        result = ds_common_rsp->rsp_head.result;
        if (result) {
            roce_warn("logic_id %d, dsmi get result %d != 0", logic_id, result);
            break;
        }

        len -= copy_size;
        off_set += copy_size;

        if (ds_common_cmd->req_head.data_frag == DS_LAST_PKT || ds_common_cmd->req_head.data_frag == DS_ONE_PKT ||
            len == 0) {
            break;
        }
    }

    ret = 0;
out:
    trans_data->result = result;
    return ret;
}

static int dsmi_recv(int logic_id, struct ds_trans_data *trans_data, struct ds_common_req_param *ds_common_cmd,
    struct ds_common_rsp_param *ds_common_rsp)
{
    int ret;
    int offset = 0;
    int result = 0;
    unsigned int recv_len = *trans_data->size_out;
    unsigned int orig_len = *trans_data->size_out;
    unsigned int size = sizeof(struct ds_common_rsp_param);

    ds_common_cmd->req_head.snd_rcv_op = DS_RECV_OP; /* set recv flag to device */
    while (true) {
        ret = dsmi_cmd_get_outbuf_from_every_pkt(trans_data, ds_common_rsp, &offset, &recv_len, orig_len);
        if (ret) {
            roce_err("logic_id %d dsmi get outbuf failed ret %d", logic_id, ret);
            goto out;
        }

        /* sizeout is sum of rsp head piece len */
        *(trans_data->size_out) += ds_common_rsp->rsp_head.one_piece_len;

        /* get rsp flag from device */
        if (ds_common_rsp->rsp_head.data_frag == DS_ONE_PKT || ds_common_rsp->rsp_head.data_frag == DS_LAST_PKT) {
            break;
        }

        if (recv_len == 0 && ds_common_rsp->rsp_head.data_frag == DS_MIDDLE_PKT) {
            roce_err("recv len is zero but pkt is not last or one pkt");
            return -EINVAL;
        }

        ds_common_cmd->req_head.data_frag = ds_common_rsp->rsp_head.data_frag; /* reuse rsp flag */
        ret = dsmi_cmd_get_network_device_info(logic_id, (char *)ds_common_cmd, sizeof(struct ds_common_req_param),
            (char *)ds_common_rsp, &size);
        if (ret || size != sizeof(struct ds_common_rsp_param) || ds_common_rsp->rsp_head.one_piece_len == 0 ||
            ds_common_rsp->rsp_head.one_piece_len > MAX_CMD_PAYLOAD_LEN) {
            roce_err("logic_id %d dsmi get network info failed ret %d, ret_size is %u, rsp_head_len %u",
                logic_id, ret, size, ds_common_rsp->rsp_head.one_piece_len);
            ret = (ds_common_rsp->rsp_head.result == UDA_TOOL_SYS_BUSY_ERR) ? UDA_TOOL_SYS_BUSY_ERR : -EINVAL;
            goto out;
        }

        /* get result from the every rsp head result */
        result = ds_common_rsp->rsp_head.result;
        if (result) {
            roce_err("logic_id %d dsmi get result %d != 0", logic_id, result);
            break;
        }
    }

    ret = 0;
out:
    trans_data->result = result;
    return ret;
}

int dsmi_network_transmission_channel(int logic_id, struct ds_trans_data *trans_data)
{
    int ret;
    struct ds_common_req_param ds_common_cmd = { {0}, {0} };
    struct ds_common_rsp_param ds_common_rsp = { {0}, {0} };

    if ((logic_id < 0) || (logic_id > DS_MAX_LOGIC_ID) || (trans_data == NULL)) {
        roce_err("Invalid input param. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    ret = dsmi_network_transmission_channel_para_check(logic_id, trans_data);
    if (ret) {
        roce_err("invaild para input %d, logic_id %d", ret, logic_id);
        return ret;
    }

    dsmi_network_transmission_channel_cmd_init(logic_id, &ds_common_cmd, trans_data);

    ret = dsmi_send(logic_id, trans_data, &ds_common_cmd, &ds_common_rsp);
    if (ret) {
        roce_err("dsmi send failed %d, logic_id %d", ret, logic_id);
        goto out;
    }

    if (trans_data->result != 0) {
        goto out;
    }

    ret = dsmi_recv(logic_id, trans_data, &ds_common_cmd, &ds_common_rsp);
    if (ret) {
        roce_err("dsmi recv failed %d, logic_id %d", ret, logic_id);
    }

out:
    dsmi_cmd_cleanup_info(&ds_common_cmd, &ds_common_rsp);
    return ret;
}

int dsmi_get_vnic_status(int logic_id, int port, int *link)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    if (link == NULL) {
        roce_err("link is null, logic_id[%d]", logic_id);
        return -EINVAL;
    }

    size = sizeof(int);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_VNIC_STATUS, NULL, 0, (char*)link, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi_get_vnic_status fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi_get_vnic_status result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_tls_digital_envelope_pub_ky(int logic_id, struct envelope_pub_info *pub_info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if (logic_id < 0 || logic_id > DS_MAX_LOGIC_ID) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if (pub_info == NULL) {
        roce_err("pub_info is NULL!");
        return -EINVAL;
    }

    size = sizeof(struct envelope_pub_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ENVELOPE_PUB, NULL, 0, (char*)pub_info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get digital envelope pub ky fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0 && trans_data.result != (-ENOENT)) {
        roce_err("dsmi get digital envelope pub ky fail result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_set_roce_port(int logic_id, int port_id, unsigned int roce_port)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    char roce_port_s[DS_PORT_LEN] = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }
    if ((roce_port < DSMI_MIN_UDP_PORT || roce_port > DSMI_MAX_UDP_PORT) && roce_port != 0) {
        DSMI_PRINT_INFO("udp_port:%u is out of range, expect 4097- 65535", roce_port);
        roce_err("invalid roce_port -- '%u', expect 4097-65535", roce_port);
        return -EINVAL;
    }
    ret = sprintf_s(roce_port_s, DS_PORT_LEN, "%u", roce_port);
    if (ret <= 0) {
        roce_err("sprintf ds_cmd.info roce_port err ret %d", ret);
        return -ENOMEM;
    }

    size = 0;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_ROCE_PORT, roce_port_s, DS_PORT_LEN, NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set roce_port fail ret[%d] logic_id[%d] roce_port[%u] port[%d]",
            ret, logic_id, roce_port, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("set roce_port fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_roce_port(int logic_id, int port_id, int *roce_port)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    char roce_port_s[DS_PORT_LEN] = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }
    if (roce_port == NULL) {
        roce_err("roce_port is null, logic_id[%d]", logic_id);
        return -EINVAL;
    }

    size = DS_PORT_LEN;
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ROCE_PORT, NULL, 0, (char*)roce_port_s, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi_get_roce_port fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi_get_roce_port result[%d] logic_id[%d]", trans_data.result, logic_id);
        return trans_data.result;
    }
    *roce_port = (int)strtol(roce_port_s, NULL, NUMBER_BASE);
    return trans_data.result;
}

int dsmi_set_cfg_status(int logic_id, unsigned int status)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    struct ds_net_cfg_status cfg_status;
    unsigned int size_out;

    size_out = 0;
    cfg_status.status = status;
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_CFG_STATUS, (char *)(&cfg_status),
        sizeof(struct ds_net_cfg_status), NULL, &size_out);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set network config status fail, (ret=%d, logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("set network config status fail, (ret=%d)", trans_data.result);
    }

    return trans_data.result;
}

int dsmi_get_cfg_status(int logic_id, unsigned int *status)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    struct ds_net_cfg_status cfg_status;
    unsigned int size_out;

    size_out = sizeof(struct ds_net_cfg_status);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_CFG_STATUS, (char *)(&cfg_status),
        sizeof(struct ds_net_cfg_status), (char *)(&cfg_status), &size_out);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi set network config status fail, (ret=%d, logic_id=%d)", ret, logic_id);
        return ret;
    }

    if (trans_data.result == 0) {
        *status = cfg_status.status;
    } else {
        roce_err("set network config status fail, (ret=%d)", trans_data.result);
    }

    return trans_data.result;
}

int dsmi_get_netdev_status(int logic_id, int port_id, char *status, unsigned int len)
{
    struct ds_trans_data trans_data;
    int ret;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if ((port_id) < 0 ||  (port_id) > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return -EINVAL;
    }
    if (len == 0) {
        roce_err("len is invalid len[%d]", len);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_NETDEV_STATUS, NULL, 0, status, &len);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get netdev status fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get netdev status fail result[%d] logic_id[%d] port[%d]",
                 trans_data.result, logic_id, port_id);
    }
    return trans_data.result;
}

int dsmi_set_generic_info(int logic_id, char *inbuf, unsigned int inbuf_len)
{
    struct ds_trans_data trans_data = {0};
    unsigned int size = 0;
    int ret;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    if (inbuf == NULL) {
        roce_err("inbuf is NULL!");
        return -EINVAL;
    }
    DSMI_SET_TRANS_DATA(trans_data, DS_SET_GENERIC_INFO, inbuf, inbuf_len, NULL, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi trans info failed, ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi set generic info failed, result[%d] logic_id[%d]", trans_data.result, logic_id);
    }

    return trans_data.result;
}

int dsmi_get_link_his_stat(int logic_id, int port_id, struct ds_link_his_stat *stat)
{
    int ret;
    int port = port_id;
    struct ds_trans_data trans_data = {0};
    unsigned int size = (unsigned int)sizeof(struct ds_link_his_stat);

    if ((port < 0) || (port > MAX_PORT_ID)) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port);
        return -EINVAL;
    }

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(stat, -EINVAL);

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_LINK_HIS_DATA, (char *)&port, sizeof(port), (char *)stat, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get stat fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get stat fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port);
    }

    return trans_data.result;
}

int dsmi_clear_link_his_stat(int logic_id, int port_id)
{
    int ret;
    int port = port_id;
    unsigned int size = 0;
    struct ds_trans_data trans_data = {0};

    if ((port < 0) || (port > MAX_PORT_ID)) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port);
        return -EINVAL;
    }

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_CLEAR_LINK_HIS_DATA, (char *)&port, sizeof(port), NULL, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi clear stat fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi clear stat fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port);
    }

    return trans_data.result;
}

int dsmi_get_roce_dfx_len(int logic_id, int port_id, struct ds_roce_dfx *roce_dfx)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size = (unsigned int)sizeof(struct ds_roce_dfx);

    if ((logic_id) > (DS_MAX_LOGIC_ID) || (logic_id) < (0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return (-EINVAL);
    }
    if ((port_id) < 0 ||  (port_id) > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return (-EINVAL);
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(roce_dfx, -EINVAL);

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ROCE_DFX_LEN, NULL, 0, (char*)roce_dfx, &size);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get roce dfx len fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get roce dfx len fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_roce_dfx_data(int logic_id, int port_id, char *data, unsigned int len)
{
    int ret;
    unsigned int buf_len = len;
    struct ds_trans_data trans_data = {0};

    if ((logic_id) > (DS_MAX_LOGIC_ID) || (logic_id) < (0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return (-EINVAL);
    }
    if ((port_id) < 0 ||  (port_id) > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return (-EINVAL);
    }

    DSMI_CHECK_PTR_VALID_RETURN_VAL(data, -EINVAL);
    if (len == 0) {
        roce_err("len is zero");
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ROCE_DFX_DATA, (char *)&buf_len, sizeof(buf_len), data, &len);
    trans_data.pid = getpid();
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get roce dfx fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get roce dfx fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_eth_test_info(int logic_id, char mode)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size = ETH_SELF_TEST_OUTBUFF_LEN;
    char outbuf[ETH_SELF_TEST_OUTBUFF_LEN] = {0};
    int size_out;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_GET_ETH_TEST_INFO, &mode, sizeof(mode), outbuf, &size);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("dsmi get eth test fail ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get mode eth test fail result[%d] logic_id[%d]", trans_data.result, logic_id);
        return trans_data.result;
    }

    // when trans_data.result == 0, data in outbuf is supposed to be string
    // but just in case, set a \0 here to avoid strlen overbound
    outbuf[ETH_SELF_TEST_OUTBUFF_LEN - 1] = '\0';
    size_out = strlen(outbuf);
    while (size_out && (outbuf[size_out - 1] == '\n')) {
        outbuf[size_out - 1] = '\0';
        size_out = strlen(outbuf);
    }
    DSMI_PRINT_INFO("%s", outbuf);

    return trans_data.result;
}

int dsmi_get_rdma_hw_stats(int logic_id, int port_id, struct ds_rdma_hw_stats *stats)
{
    struct ds_trans_data trans_data = {0};
    unsigned int size;
    int ret;

    if ((port_id) < 0 ||  (port_id) > MAX_PORT_ID) {
        roce_err("port id:%d is invalid! expect [0 - 7]", port_id);
        return (-EINVAL);
    }
    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("logic id:%d is invalid! expect [0]-[%d]", logic_id, DS_MAX_LOGIC_ID);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(stats, -EINVAL);

    size = (unsigned int)(stats->num_counters * sizeof(unsigned long long));
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_RDMA_HW_STATS_DATA, (char *)stats, sizeof(*stats),
        (char *)(stats->value), &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get stat fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get stat fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_acquire_dhcp_ip(int logic_id, int port_id, const struct udhcpc_param *udhcpc_param, char *outbuf,
                         unsigned int len)
{
    struct ds_trans_data trans_data = {0};
    int ret;

    if ((logic_id < 0) || (logic_id > DS_MAX_LOGIC_ID) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }

    if (udhcpc_param == NULL) {
        roce_err("udhcpc_param is NULL!");
        return -EINVAL;
    }

    if (outbuf == NULL) {
        roce_err("outbuf is NULL!");
        return -EINVAL;
    }

    if (len == 0) {
        roce_err("len is invalid. (len=%u)", len);
        return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_ACQUIRE_DHCP_IP, (char *)udhcpc_param,
        sizeof(struct udhcpc_param), outbuf, &len);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi acquire dhcp ip fail ret[%d] logic_id[%d] port_id[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi acquire dhcp ip result[%d] logic_id[%d] port_id[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_release_dhcp_ip(int logic_id, int port_id, const struct udhcpc_param *udhcpc_param, char *outbuf,
                         unsigned int len)
{
    struct ds_trans_data trans_data = {0};
    int ret;

    if ((logic_id < 0) || (logic_id > DS_MAX_LOGIC_ID) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }

    if (udhcpc_param == NULL) {
        roce_err("udhcpc_param is NULL!");
        return -EINVAL;
    }

    if (outbuf == NULL) {
        roce_err("outbuf is NULL!");
        return -EINVAL;
    }

    if (len == 0) {
            roce_err("len is invalid len[%u]", len);
            return -EINVAL;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_RELEASE_DHCP_IP, (char *)udhcpc_param, sizeof(struct udhcpc_param),
        outbuf, &len);
    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi release dhcp ip fail ret[%d] logic_id[%d] port_id[%d]", ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi release dhcp ip result[%d] logic_id[%d] port_id[%d]", trans_data.result, logic_id, port_id);
    }

    return trans_data.result;
}

int dsmi_get_dcqcn_info(int logic_id, struct ds_dcqcn_info *info)
{
    struct ds_trans_data trans_data = {0};
    unsigned int size_out;
    int ret;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    size_out = sizeof(struct ds_dcqcn_info);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_DCQCN_INFO, (char *)info, size_out, (char*)info, &size_out);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi get dcqcn info fail. ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get dcqcn info result fail. result[%d]", trans_data.result);
    }
    return trans_data.result;
}

int dsmi_set_dcqcn_info(int logic_id, struct ds_dcqcn_info *info)
{
    struct ds_trans_data trans_data = {0};
    unsigned int size_out = 0;
    int ret;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0)) {
        roce_err("Logic id is invalid. (logic_id=%d)", logic_id);
        return -EINVAL;
    }

    ret = hccn_check_usr_identify();
    if (ret != 0) {
        roce_err("Check usr identify failed. (ret=%d; logic_id=%d)", ret, logic_id);
        return ret;
    }

    DSMI_SET_TRANS_DATA(trans_data, DS_SET_DCQCN_INFO, (char *)info, sizeof(struct ds_dcqcn_info), NULL, &size_out);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi set gratuitous arp info fail. ret[%d] logic_id[%d]", ret, logic_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get gratuitous arp info result fail. result[%d]", trans_data.result);
    }
    return trans_data.result;
}

int dsmi_get_bandwidth(int logic_id, int port, struct bandwidth_t *bandwidth_info)
{
    int ret;
    struct ds_trans_data trans_data = {0};
    unsigned int size;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port > MAX_PORT_ID) || (port < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id:%d; port_id:%d)", logic_id, port);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(bandwidth_info, -EINVAL);

    size = sizeof(struct bandwidth_t);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_BANDWIDTH, (char *)bandwidth_info, size, (char *)bandwidth_info, &size);

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret) {
        roce_err("dsmi get bandwidth fail ret[%d] logic_id[%d] port[%d]", ret, logic_id, port);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("dsmi get bandwidth fail result[%d] logic_id[%d] port[%d]", trans_data.result, logic_id, port);
    }

    return trans_data.result;
}

int dsmi_get_link_cnt(int logic_id, int port_id, unsigned int *link_cnt)
{
    unsigned int size_out = sizeof(unsigned int);
    struct ds_trans_data trans_data = {0};
    unsigned int link_cnt_temp = 0;
    unsigned int mainboard_id = 0;
    unsigned int phy_id = 0;
    int ret;

    if ((logic_id > DS_MAX_LOGIC_ID) || (logic_id < 0) || (port_id > MAX_PORT_ID) || (port_id < 0)) {
        roce_err("Logic id or port id is invalid. (logic_id=%d; port_id=%d)", logic_id, port_id);
        return -EINVAL;
    }
    DSMI_CHECK_PTR_VALID_RETURN_VAL(link_cnt, -EINVAL);
    DSMI_SET_TRANS_DATA(trans_data, DS_GET_LINK_CNT, NULL, 0, (char *)&link_cnt_temp, &size_out);

    /* is_atlas_9000_a3 */
    ret = dsmi_get_phyid_from_logicid(logic_id, &phy_id);
    if (ret != 0) {
        roce_err("Call dsmi get phyid from logicid failed. (ret=%d, logic_id=%d)", ret, logic_id);
        return ret;
    }
    ret = dsmi_get_mainboard_id(phy_id, &mainboard_id);
    if (ret != 0) {
        roce_err("Call dsmi get mainboard id failed. (ret=%d, phy_id=%u)", ret, phy_id);
        return UDA_DSMI_EXE_ERR;
    }

    ret = dsmi_network_transmission_channel(logic_id, &trans_data);
    if (ret != 0) {
        roce_err("Dsmi network transmission channel failed. (ret=%d, logic_id=%d, port_id=%d)",
            ret, logic_id, port_id);
        return ret;
    }

    if (trans_data.result != 0) {
        roce_err("Dsmi get link count failed. (result=%d, logic_id=%d, port_id=%d)",
            trans_data.result, logic_id, port_id);
        return trans_data.result;
    }

    link_cnt_temp = strtoul(trans_data.outbuf, NULL, UNSIGNED_INT_RADIX);
    if (mainboard_id == ATLAS_9000_A3_MAINBOARD_ID || mainboard_id == ATLAS_9000_A3_MAINBOARD_ID_2) {
        *link_cnt = link_cnt_temp / CHIP_DIE_CNT;
    } else {
        *link_cnt = 1;
    }

    return trans_data.result;
}

int dsmi_analysis_dsmi_ret_to_uda(int ret)
{
    switch (ret) {
        case -ENOLINK:
            return UDA_DSMI_LINK_IS_DOWN_ERR;
        case -EACCES:
            return UDA_TOOL_SYS_NOT_ACCESS;
        case -EBUSY:
        case UDA_TOOL_SYS_BUSY_ERR:
            return UDA_TOOL_SYS_BUSY_ERR;
        case TLS_CERT_ILLEGAL_ERR:
            return UDA_DSMI_TLS_CER_ILLEGAL_ERR;
        case TLS_CERT_EXPIRED_ERR:
            return UDA_DSMI_TLS_CERT_EXPIRED_ERR;
        case TLS_CERT_VERIFY_ERR:
        case TLS_CERT_KYMATCH_ERR:
        case TLS_CERT_LOAD_ERR:
            return UDA_DSMI_TLS_CER_VERIFY_ERR;
        case HCCN_TOOL_GET_LOCK_FAIL:
            return UDA_DSMI_GET_LOCK_FAILED;
        case UDA_TOOL_CHECK_TC_QOS_CONFIG:
            return UDA_TOOL_CHECK_TC_QOS_CONFIG;
        case 0:
            return UDA_EXE_SUCCESS;
        default:
            return UDA_DSMI_EXE_ERR;
    }
}
