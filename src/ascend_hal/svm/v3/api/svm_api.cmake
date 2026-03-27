get_filename_component(SVM_API_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if(${PRODUCT_SIDE} STREQUAL host)
    file(GLOB SVM_API_HOST_SRCS
        ${SVM_API_DIR}/*.c
        ${SVM_API_DIR}/master/*.c
        ${SVM_API_DIR}/master/task_group/*.c
    )

    list(APPEND SVM_SRC_FILES ${SVM_API_HOST_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_API_DIR}/master/
        ${SVM_API_DIR}/master/task_group/
    )
else ()
    file(GLOB SVM_API_DEVICE_SRCS
        ${SVM_API_DIR}/*.c
        ${SVM_API_DIR}/agent/*.c
        ${SVM_API_DIR}/agent/task_group/*.c
        ${SVM_API_DIR}/agent/task_group_sp/*.c
    )

    list(APPEND SVM_SRC_FILES ${SVM_API_DEVICE_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_API_DIR}/
        ${SVM_API_DIR}/agent/
        ${SVM_API_DIR}/agent/task_group/
        ${SVM_API_DIR}/agent/task_group_sp/
    )
endif ()
