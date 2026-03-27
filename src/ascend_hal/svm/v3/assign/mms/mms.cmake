get_filename_component(MMS_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
 
if(${PRODUCT_SIDE} STREQUAL host)
    file(GLOB MMS_DIR_SRCS
        ${MMS_DIR}/*.c
    )
 
    list(APPEND SVM_SRC_FILES ${MMS_DIR_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${MMS_DIR}/
    )
endif ()
