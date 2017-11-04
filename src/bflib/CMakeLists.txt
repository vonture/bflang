add_library(bflib STATIC
    ast.cpp
    ast.h
    bflib.cpp
    interpreter.cpp
    interpreter.h
    stream.cpp
    stream.h
    tokenizer.cpp
    tokenizer.h
    ${BFLANG_INCLUDE_DIR}/bflib.h
    ${BFLANG_INCLUDE_DIR}/bflib_raii.h
)

target_include_directories(bflib PRIVATE ${BFLANG_INCLUDE_DIR})
