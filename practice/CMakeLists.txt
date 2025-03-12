cmake_minimum_required(VERSION 3.20)
project(CRAM VERSION 0.0.1)

find_package(LibPressio REQUIRED)

add_executable(LibPressio_Practice LibPressio_Practice.cpp)

target_link_libraries(LibPressio_Practice PRIVATE LibPressio::libpressio)
