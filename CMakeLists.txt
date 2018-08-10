cmake_minimum_required(VERSION 3.11)
set(PROJECT_NAME altlib)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE PROJECT_SOURCES
        ${PROJECT_INCLUDE_DIR}/*.cpp
        ${PROJECT_INCLUDE_DIR}/*.c)

add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})

set(CATCH2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch2)
add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE ${CATCH2_DIR})

set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
file(GLOB_RECURSE TEST_SOURCES ${TEST_DIR}/*.cpp)
add_executable(tests ${TEST_SOURCES})
target_include_directories(tests PRIVATE ${PROJECT_INCLUDE_DIR})
target_link_libraries(tests altlib)
target_link_libraries(tests catch2)