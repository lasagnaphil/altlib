cmake_minimum_required(VERSION 3.11)
set(PROJECT_NAME altlib)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

option(altlib_build_tests "Build tests for altlib." ON)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE PROJECT_SOURCES
        ${PROJECT_INCLUDE_DIR}/*.cpp
        ${PROJECT_INCLUDE_DIR}/*.c)

add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})

if (altlib_build_tests)
    set(DOCTEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doctest)
    add_library(doctest INTERFACE)
    target_compile_definitions(doctest INTERFACE TLS_NO_DEBUG)
    target_include_directories(doctest INTERFACE ${DOCTEST_DIR})

    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    file(GLOB_RECURSE TEST_SOURCES ${TEST_DIR}/*.cpp)
    add_executable(altlib-tests ${TEST_SOURCES})
    target_include_directories(altlib-tests PRIVATE ${PROJECT_INCLUDE_DIR})
    target_link_libraries(altlib-tests altlib)
    target_link_libraries(altlib-tests doctest)
endif()