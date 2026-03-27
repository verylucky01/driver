get_filename_component(SVM_OP_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if(${PRODUCT_SIDE} STREQUAL host)
    file(GLOB SVM_OP_HOST_SRCS
        ${SVM_OP_DIR}/*.c
        ${SVM_OP_DIR}/access/*.c
        ${SVM_OP_DIR}/memcpy_local/*.c
        ${SVM_OP_DIR}/memcpy_local_client/*.c
        ${SVM_OP_DIR}/memcpy/*.c
        ${SVM_OP_DIR}/memset/*.c
        ${SVM_OP_DIR}/memset_client/*.c
    )

    if(${ENABLE_UBE})
        file(GLOB SVM_OP_PCI_ADAPT_SRCS
            ${SVM_OP_DIR}/pci_adapt/*.c
        )
        file(GLOB SVM_OP_UB_ADAPT_SRCS
            ${SVM_OP_DIR}/ub_adapt/*.c
        )

        list(APPEND SVM_SRC_FILES
            ${SVM_OP_PCI_ADAPT_SRCS}
            ${SVM_OP_UB_ADAPT_SRCS}
        )
        list(APPEND SVM_SRC_INC_DIRS
            ${SVM_OP_DIR}/pci_adapt/
            ${SVM_OP_DIR}/ub_adapt/
        )
    else ()
        file(GLOB SVM_OP_PCI_ADAPT_SRCS
            ${SVM_OP_DIR}/pci_adapt/*.c
        )

        list(APPEND SVM_SRC_FILES
            ${SVM_OP_PCI_ADAPT_SRCS}
        )
        list(APPEND SVM_SRC_INC_DIRS
            ${SVM_OP_DIR}/pci_adapt/
        )
    endif ()

    list(APPEND SVM_SRC_FILES ${SVM_OP_HOST_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_OP_DIR}/
        ${SVM_OP_DIR}/access/
        ${SVM_OP_DIR}/memcpy_local/
        ${SVM_OP_DIR}/memcpy_local_client/
        ${SVM_OP_DIR}/memcpy/
        ${SVM_OP_DIR}/memset/
        ${SVM_OP_DIR}/memset_client/
    )
else ()
    file(GLOB SVM_OP_DEVICE_SRCS
        ${SVM_OP_DIR}/*.c
        ${SVM_OP_DIR}/memcpy_local/*.c
        ${SVM_OP_DIR}/memcpy_local_agent/*.c
        ${SVM_OP_DIR}/memset/*.c
        ${SVM_OP_DIR}/memset_agent/*.c
    )

    list(APPEND SVM_SRC_FILES ${SVM_OP_DEVICE_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_OP_DIR}/
        ${SVM_OP_DIR}/memcpy_local/
        ${SVM_OP_DIR}/memcpy_local_agent/
        ${SVM_OP_DIR}/memset/
        ${SVM_OP_DIR}/memset_agent/
    )
endif ()
