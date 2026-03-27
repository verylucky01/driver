get_filename_component(SVM_ASSIGN_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if(${PRODUCT_SIDE} STREQUAL host)
    file(GLOB SVM_ASSIGN_HOST_SRCS
        ${SVM_ASSIGN_DIR}/*.c
        ${SVM_ASSIGN_DIR}/gen_allocator/*.c
        ${SVM_ASSIGN_DIR}/va_allocator/*.c
        ${SVM_ASSIGN_DIR}/normal_malloc/*.c
        ${SVM_ASSIGN_DIR}/cache_malloc/*.c
        ${SVM_ASSIGN_DIR}/malloc_mng/*.c
        ${SVM_ASSIGN_DIR}/mpl/*.c
        ${SVM_ASSIGN_DIR}/mpl_client/*.c
        ${SVM_ASSIGN_DIR}/mms/*.c
        ${SVM_ASSIGN_DIR}/gap/*.c
        ${SVM_ASSIGN_DIR}/pci_adapt/*.c
        ${SVM_ASSIGN_DIR}/madvise/*.c
        ${SVM_ASSIGN_DIR}/madvise_client/*.c
    )

    list(APPEND SVM_SRC_FILES ${SVM_ASSIGN_HOST_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_ASSIGN_DIR}/
        ${SVM_ASSIGN_DIR}/gen_allocator/
        ${SVM_ASSIGN_DIR}/va_allocator/
        ${SVM_ASSIGN_DIR}/normal_malloc/
        ${SVM_ASSIGN_DIR}/cache_malloc/
        ${SVM_ASSIGN_DIR}/malloc_mng/
        ${SVM_ASSIGN_DIR}/mpl/
        ${SVM_ASSIGN_DIR}/mpl_client/
        ${SVM_ASSIGN_DIR}/mms/
        ${SVM_ASSIGN_DIR}/gap/
        ${SVM_ASSIGN_DIR}/pci_adapt/
        ${SVM_ASSIGN_DIR}/madvise/
        ${SVM_ASSIGN_DIR}/madvise_client/
    )
else ()
    file(GLOB SVM_ASSIGN_DEVICE_SRCS
        ${SVM_ASSIGN_DIR}/*.c
        ${SVM_ASSIGN_DIR}/mpl/*.c
        ${SVM_ASSIGN_DIR}/mpl_agent/*.c
        ${SVM_ASSIGN_DIR}/va_allocator_agent/*.c
        ${SVM_ASSIGN_DIR}/gap_agent/*.c
        ${SVM_ASSIGN_DIR}/madvise/*.c
        ${SVM_ASSIGN_DIR}/madvise_agent/*.c
    )

    list(APPEND SVM_SRC_FILES ${SVM_ASSIGN_DEVICE_SRCS})
    list(APPEND SVM_SRC_INC_DIRS
        ${SVM_ASSIGN_DIR}/
        ${SVM_ASSIGN_DIR}/mpl/
        ${SVM_ASSIGN_DIR}/mpl_agent/
        ${SVM_ASSIGN_DIR}/va_allocator_agent/
        ${SVM_ASSIGN_DIR}/gap_agent/
        ${SVM_ASSIGN_DIR}/madvise/
        ${SVM_ASSIGN_DIR}/madvise_agent/
    )
endif ()
