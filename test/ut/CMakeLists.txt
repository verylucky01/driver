add_executable(npu_driver_utest
    common/main.cc
)

target_include_directories(npu_driver_utest PRIVATE

)

target_link_libraries(npu_driver_utest

)

target_compile_definitions(npu_driver_utest PRIVATE

)

target_compile_options(npu_driver_utest PRIVATE

)

target_link_options(npu_driver_utest PRIVATE

)

install(TARGETS npu_driver_utest
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
)
