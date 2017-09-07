add_executable(bflang
    main.cpp
)

target_include_directories(bflang PRIVATE ${BFLANG_INCLUDE_DIR})
target_link_libraries(bflang bflib)
