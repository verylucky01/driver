get_filename_component(SVM_DBI_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if(${PRODUCT_SIDE} STREQUAL host)
    file(GLOB SVM_SVM_DBI_DIR_SRCS
        ${SVM_DBI_DIR}/*.c
    )

    list(APPEND SVM_SRC_FILES ${SVM_SVM_DBI_DIR_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_DBI_DIR}/
    )
endif ()
