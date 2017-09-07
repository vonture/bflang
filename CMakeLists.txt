cmake_minimum_required(VERSION 3.7)

project(bflang)

set(BFLANG_BUILD_TESTS TRUE CACHE BOOL "Build test suite")

set(CMAKE_CXX_STANDARD 14)

set(BFLANG_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(BFLANG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(BFLANG_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

add_subdirectory(${BFLANG_SRC_DIR})

if (${BFLANG_BUILD_TESTS})
    add_subdirectory(${BFLANG_TEST_DIR})
endif()
