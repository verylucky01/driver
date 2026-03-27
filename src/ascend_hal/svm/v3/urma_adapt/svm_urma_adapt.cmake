get_filename_component(SVM_URMA_ADAPT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if(${PRODUCT_SIDE} STREQUAL host)
    if(${ENABLE_UBE})
        file(GLOB SVM_URMA_ADAPT_HOST_SRCS
            ${SVM_URMA_ADAPT_DIR}/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_jetty/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_chan/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_seg_register_client/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_seg_mng/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_adapt_init/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_seg_local/*.c
        )

        list(APPEND SVM_SRC_FILES ${SVM_URMA_ADAPT_HOST_SRCS})
        list(APPEND SVM_SRC_INC_DIRS
            ${SVM_URMA_ADAPT_DIR}/
            ${SVM_URMA_ADAPT_DIR}/inc
            ${SVM_URMA_ADAPT_DIR}/urma_jetty/
            ${SVM_URMA_ADAPT_DIR}/urma_chan/
            ${SVM_URMA_ADAPT_DIR}/urma_seg_register_client/
            ${SVM_URMA_ADAPT_DIR}/urma_seg_mng/
            ${SVM_URMA_ADAPT_DIR}/urma_adapt_init/
            ${SVM_URMA_ADAPT_DIR}/urma_seg_local/
        )
    else ()
	    file(GLOB SVM_URMA_ADAPT_HOST_SRCS
            ${SVM_URMA_ADAPT_DIR}/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_seg_register_client/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_seg_mng/*.c
            ${SVM_URMA_ADAPT_DIR}/urma_adapt_init/*.c
        )

        list(APPEND SVM_SRC_FILES ${SVM_URMA_ADAPT_HOST_SRCS})
        list(APPEND SVM_SRC_INC_DIRS
            ${SVM_URMA_ADAPT_DIR}/
            ${SVM_URMA_ADAPT_DIR}/inc
            ${SVM_URMA_ADAPT_DIR}/urma_seg_register_client/
            ${SVM_URMA_ADAPT_DIR}/urma_seg_mng/
            ${SVM_URMA_ADAPT_DIR}/urma_adapt_init/
        )
    endif ()
else ()
    file(GLOB SVM_URMA_ADAPT_DEVICE_SRCS
        ${SVM_URMA_ADAPT_DIR}/*.c
        ${SVM_URMA_ADAPT_DIR}/urma_jetty/*.c
        ${SVM_URMA_ADAPT_DIR}/urma_chan_agent/*.c
        ${SVM_URMA_ADAPT_DIR}/urma_seg_register_agent/*.c
        ${SVM_URMA_ADAPT_DIR}/urma_seg_local/*.c
    )

    list(APPEND SVM_SRC_FILES ${SVM_URMA_ADAPT_DEVICE_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_URMA_ADAPT_DIR}/
        ${SVM_URMA_ADAPT_DIR}/inc
        ${SVM_URMA_ADAPT_DIR}/urma_jetty/
        ${SVM_URMA_ADAPT_DIR}/urma_chan_agent/
        ${SVM_URMA_ADAPT_DIR}/urma_seg_register_agent/
        ${SVM_URMA_ADAPT_DIR}/urma_seg_local/
    )
endif ()
