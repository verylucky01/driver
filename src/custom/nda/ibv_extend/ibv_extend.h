/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 *
 * File Name     : ibv_extend.h
 * Version       : v3.2.0
 * Created       : 2026/2/14
 * Updated       : 2026/4/24
 * Description   : The declaration invoked by the Application interface of RoCE NDA Function
 */

#ifndef IBV_EXTEND_H
#define IBV_EXTEND_H

#include <infiniband/verbs.h>
#include <stdint.h>

/*
 * 版本号说明：
 * - 主版本号(MAJOR): 不兼容的API变更
 * - 次版本号(MINOR): 后向兼容的功能新增
 * - 修订号(PATCH): 后向兼容的问题修复
 *
 * 兼容性规则：
 * 1. 驱动和业务编译时的头文件版本必须 <= 运行时库的版本
 * 2. 主版本号必须完全一致
 * 3. 次版本号：驱动 <= 库
 */
#define IBV_EXTEND_VERSION_MAJOR 3
#define IBV_EXTEND_VERSION_MINOR 2
#define IBV_EXTEND_VERSION_PATCH 0
#define IBV_EXTEND_VERSION_STRING "3.2.0"

/* RoCE队列内存的DMA寻址方式 */
enum queue_buf_dma_mode {
    QU_BUF_DMA_MODE_DEFAULT = 0, /* 采用Host所在总线的DMA, NIC - PCIe - NPU */
    QU_BUF_DMA_MODE_INDEP_UB,    /* 采用非Host的独立UB总线做DMA, NIC - UB - NPU */
    QU_BUF_DMA_MODE_MAX
};

/* Doorbel的地址映射方式 */
enum doorbell_map_mode {
    DB_MAP_MODE_HOST_VA = 0, /* 基于host VA映射 */
    DB_MAP_MODE_UB_RES,      /* 基于UB设备的资源描述映射 */
    DB_MAP_MODE_UB_MAX
};

/* 内存拷贝方向 */
enum memcpy_direction {
    MEMCPY_DIR_HOST_TO_HOST = 0,    // Host内复制
    MEMCPY_DIR_HOST_TO_DEVICE,      // Host到Device的内存复制
    MEMCPY_DIR_DEVICE_TO_HOST,      // Device到Host的内存复制
    MEMCPY_DIR_DEVICE_TO_DEVICE,    // Device内或Device间的内存复制
};

/* QP初始化扩展属性 */
enum ibv_qp_init_cap {
    QP_ENABLE_DIRECT_WQE   = 1 << 0,   /* SQ启用direct_wqe能力 */
};

/* 扩展设备属性 */
enum ibv_extend_device_cap {
    IBV_EXTEND_DEV_NDR  = 1 << 0,           // 网卡设备支持NDR
    IBV_EXTEND_DEV_NDA  = 1 << 1,           // 网卡设备支持NDA
};

/* 用于映射doorbell资源到设备侧 */
struct doorbell_map_desc {
    union {
        uint64_t hva;
        struct {
            uint64_t guid_l;
            uint64_t guid_h;
            struct {
                uint64_t resource_id : 4;
                uint64_t offset : 32;       /* 单位：字节 */
                uint64_t resv : 28;
            } bits;
        } ub_res;
    };
    uint64_t size; /* 单位：字节 */
    uint32_t type; /* doorbell映射的寻址模式, 对应doorbell_map_mode枚举变量 */

    uint32_t resv; /* 扩展专用，padding */
};

/* 回调函数集合 */
struct ibv_extend_ops {
    void *(*alloc)(size_t size);        /* 申请NPU内存，网卡创建QP时回调，返回内存指针 */
    void (*free)(void *ptr);            /* 释放内存 */

    void (*memset_s)(void *dst, int value, size_t count);    /* 初始化NPU内存内容 */
    /* 内存拷贝，需支持不同方向实现，direct类型为 memcpy_direction */
    int (*memcpy_s)(void *dst, size_t dst_max_size, void *src, size_t size, uint32_t direct);

    void *(*db_mmap)(struct doorbell_map_desc *desc); /* 返回映射的起始内存地址，需允许对相同desc的重复map */
    int (*db_unmap)(void *ptr, struct doorbell_map_desc *desc);   /* 解除地址映射，需对应的输入描述符传入 */
};

struct iov_addr_desc {
    void *iov_base;
    size_t iov_len;
};

/* 队列buffer信息 */
struct queue_buf {
    uint64_t base;       /* 队列的首地址 */
    uint32_t entry_cnt;  /* 队列中entry个数, 表示WQE/CQE个数 */
    uint32_t entry_size; /* 队列中entry大小, 表示WQE/CQE大小 */
    uint64_t resv[4];    /* 扩展专用 */
};

struct queue_info {
    struct queue_buf qbuf;  /* 队列的buffer */
    struct iov_addr_desc dbr_pi_va; /* doorbell recode PI地址, SQ/RQ硬件使用, CQ软件使用 */
    struct iov_addr_desc dbr_ci_va; /* doorbell recode CI地址, SQ/RQ软件使用，CQ硬件使用 */
    struct iov_addr_desc db_hw_va;  /* map之后的doorbell地址 */
    uint64_t resv[4];               /* 扩展专用 */
};

/* QP输出参数 */
struct ibv_qp_extend {
    struct ibv_qp *qp;
    struct queue_info sq_info; /* 发送队列的buffer */
    struct queue_info rq_info; /* 接收队列的buffer */

    uint64_t resv[32];           /* 扩展专用 */
};

/* CQ输出参数 */
struct ibv_cq_extend {
    struct ibv_cq *cq;
    struct queue_info cq_info;

    uint64_t resv[32];           /* 扩展专用 */
};

/* SRQ输出参数 */
struct ibv_srq_extend {
    struct ibv_srq *srq;
    struct queue_info srq_info;

    uint64_t resv[32];          /* 扩展专用 */
};

/* QP输入参数 */
struct ibv_qp_init_attr_extend {
    struct ibv_pd *pd;            /* 默认参数 */
    struct ibv_qp_init_attr attr; /* 默认参数 */

    uint32_t qp_cap_flag;         /* qp 配置, ibv_qp_init_cap 类型 */
    enum queue_buf_dma_mode type; /* DMA mode */
    struct ibv_extend_ops *ops;   /* 通过入参传递 */

    uint64_t resv[8];             /* 扩展专用 */
};

/* CQ输入参数 */
struct ibv_cq_init_attr_extend {
    struct ibv_cq_init_attr_ex attr; /* 默认参数 */

    uint32_t cq_cap_flag;         /* cq 配置 */
    enum queue_buf_dma_mode type; /* DMA mode */
    struct ibv_extend_ops *ops;   /* 通过入参传递 */

    uint64_t resv[8];             /* 扩展专用 */
};

/* SRQ输入参数 */
struct ibv_srq_init_attr_extend {
    struct ibv_pd *pd;              /* 默认参数 */
    struct ibv_srq_init_attr attr;  /* 默认参数 */

    uint32_t comp_mask; /* compatibility mask */

    uint32_t srq_cap_flag;        /* srq 配置 */
    enum queue_buf_dma_mode type; /* DMA mode */
    struct ibv_extend_ops *ops;   /* 通过入参传递 */

    uint64_t resv[8];             /* 扩展专用 */
};

/* 扩展设备属性 */
struct ibv_device_attr_extend {
    uint32_t ext_cap;      /* ibv_extend_device_cap 类型 */

    uint32_t resv[32];     /* 扩展专用 */
};

struct ibv_context_extend {
    struct ibv_context *context;
    struct ibv_context_extend_ops *ops;
};

/* ==版本兼容性查询接口== */
/**
 * @brief 获取库版本号
 * @param major 主版本号输出参数（可为NULL）
 * @param minor 次版本号输出参数（可为NULL）
 * @param patch 修订号输出参数（可为NULL）
 * @return 版本字符串指针（静态字符串，无需释放）
 */
const char *ibv_extend_get_version(uint32_t *major, uint32_t *minor, uint32_t *patch);

/**
 * @brief 检查版本兼容性
 * @param driver_major 驱动编译时的主版本号
 * @param driver_minor 驱动编译时的次版本号
 * @param driver_patch 驱动编译时的修订号
 * @return 0-兼容，-1-不兼容
 *
 * 此函数在运行时检查驱动编译时使用的头文件版本与当前库版本是否兼容。
 * 建议在驱动和上层应用初始化时调用此函数进行版本检查。
 */
int ibv_extend_check_version(uint32_t driver_major, uint32_t driver_minor, uint32_t driver_patch);

/* 北向接口 */
/**
 * @brief 打开扩展上下文，初始化NDA扩展功能
 * @param context RDMA设备上下文指针
 * @return NULL-打开失败，Non-NULL-打开成功，返回扩展上下文指针
 */
struct ibv_context_extend *ibv_open_extend(struct ibv_context *context);

/**
 * @brief 关闭扩展上下文，释放NDA扩展资源
 * @param context 扩展上下文指针
 * @return 0-成功，其他值-失败
 */
int ibv_close_extend(struct ibv_context_extend *context);

/**
 * @brief 查询设备支持的扩展能力
 * @param context RDMA设备上下文指针
 * @param ext_dev_attr 返回设备支持的扩展能力
 * @return 0-成功，其他值-失败
 */
int ibv_query_device_extend(struct ibv_context_extend *context, struct ibv_device_attr_extend *ext_dev_attr);

/**
 * @brief ibv_create_qp扩展接口，支持NDA场景创建qp
 * @param context 对应ibv设备的上下文
 * @param qp_init_attr 创建qp的属性
 * @attention qp_init_attr需要指定对应的内存类型和内存操作集，该信息要保存到qp_ctx后续使用。
 * @return NullPtr-创建失败，Non-NullPtr-创建成功，返回qp的句柄和内存信息
 */
struct ibv_qp_extend *ibv_create_qp_extend(struct ibv_context_extend *context,
                                           struct ibv_qp_init_attr_extend *qp_init_attr);

/**
 * @brief ibv_create_cq扩展接口，支持NDA场景创建cq
 * @param context 对应ibv设备的上下文
 * @param cq_init_attr 创建cq的属性
 * @attention cq_init_attr需要指定对应的内存类型和内存操作集，该信息要保存到cq_ctx后续使用。
 * @return NullPtr-创建失败，Non-NullPtr-创建成功，返回cq的句柄和内存信息
 */
struct ibv_cq_extend *ibv_create_cq_extend(struct ibv_context_extend *context,
                                           struct ibv_cq_init_attr_extend *cq_init_attr);

/* ibv_create_srq扩展接口，支持NDA场景创建srq */
/**
 * @brief ibv_create_srq扩展接口，支持NDA场景创建srq
 * @param context 对应ibv设备的上下文
 * @param srq_init_attr 创建srq的属性
 * @attention srq_init_attr需要指定对应的内存类型和内存操作集，该信息要保存到srq_ctx后续使用。
 * @return NullPtr-创建失败，Non-NullPtr-创建成功，返回srq的句柄和内存信息
 */
struct ibv_srq_extend *ibv_create_srq_extend(struct ibv_context_extend *context,
                                             struct ibv_srq_init_attr_extend *srq_init_attr);

/**
 * @brief ibv_destroy_qp扩展接口，支持NDA场景销毁qp
 * @param qp_extend 创建接口返回的qp句柄
 * @attention void
 * @return 接口返回值 0-success，other-fail
 */
int ibv_destroy_qp_extend(struct ibv_context_extend *context, struct ibv_qp_extend *qp_extend);

/**
 * @brief ibv_destroy_cq扩展接口，支持NDA场景销毁cq
 * @param cq_extend 创建接口返回的cq句柄
 * @attention void
 * @return 接口返回值 0-success，other-fail
 */
int ibv_destroy_cq_extend(struct ibv_context_extend *context, struct ibv_cq_extend *cq_extend);

/**
 * @brief ibv_destroy_srq扩展接口，支持NDA场景销毁srq
 * @param srq_extend 创建接口返回的srq句柄
 * @attention void
 * @return 接口返回值 0-success，other-fail
 */
int ibv_destroy_srq_extend(struct ibv_context_extend *context, struct ibv_srq_extend *srq_extend);

/* 南向接口 */
struct ibv_context_extend_ops {
    struct ibv_qp_extend *(*create_qp)(struct ibv_context *context,
                                       struct ibv_qp_init_attr_extend *qp_init_attr);
    struct ibv_cq_extend *(*create_cq)(struct ibv_context *context,
                                       struct ibv_cq_init_attr_extend *cq_init_attr);
    struct ibv_srq_extend *(*create_srq)(struct ibv_context *context,
                                         struct ibv_srq_init_attr_extend *srq_init_attr);

    int (*destroy_qp)(struct ibv_qp_extend *qp_extend);
    int (*destroy_cq)(struct ibv_cq_extend *cq_extend);
    int (*destroy_srq)(struct ibv_srq_extend *srq_extend);

    /* 提供设备的扩展能力查询 */
    int (*query_device)(struct ibv_context *context,
                        struct ibv_device_attr_extend *ext_dev_attr);
};

struct verbs_device_extend_ops {
    const char *name;   /* 与标准驱动匹配 */

    struct ibv_context_extend *(*alloc_context)(struct ibv_context *context);
    void (*free_context)(struct ibv_context_extend *context);
};

void verbs_register_driver_extend(const struct verbs_device_extend_ops *ops);

#define PROVIDER_EXTEND_DRIVER(drv)                                              \
    static __attribute__((constructor)) void drv##__register_extend_driver(void) \
    {                                                                            \
        verbs_register_driver_extend(&(drv));                                    \
    }

#endif