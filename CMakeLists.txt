cmake_minimum_required(VERSION 3.20)
project(CRAM VERSION 0.0.1)

find_package(LibPressio REQUIRED)

add_executable(ArrayWrapper ArrayWrapper.cpp)

target_include_directories(ArrayWrapper PRIVATE ${SZ_INCLUDE_DIR})
target_link_libraries(ArrayWrapper PRIVATE LibPressio::libpressio)

set(CMAKE_BUILD_TYPE Debug)

