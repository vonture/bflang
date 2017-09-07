add_library(bflib STATIC
    bflib.cpp
    error_stream.cpp
    error_stream.h
    tokenizer.cpp
    tokenizer.h
    ${BFLANG_INCLUDE_DIR}/bflib.h
)

target_include_directories(bflib PRIVATE ${BFLANG_INCLUDE_DIR})
