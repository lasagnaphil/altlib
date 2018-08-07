cmake_minimum_required(VERSION 3.11)
set(PROJECT_NAME altlib)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

add_library(altlib
        log.cpp
        StringPool.cpp
        math/Matrix4.cpp
        math/Rect.cpp
        math/Vector2.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
