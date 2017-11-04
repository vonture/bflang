add_executable(bflib_tests
    ast_tests.cpp
    interpreter_tests.cpp
    tokenizer_tests.cpp
)

target_link_libraries(bflib_tests
    bflib
    gtest
    gtest_main
)

target_include_directories(bflib_tests PRIVATE
    ${BFLANG_INCLUDE_DIR}
)
