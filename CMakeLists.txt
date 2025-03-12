cmake_minimum_required(VERSION 3.20)
project(CRAM VERSION 0.0.1)

find_package(LibPressio REQUIRED)

add_executable(ArrayWrapper ArrayWrapper.cpp)

add_library(CRAM CRAM_Manager.cpp)

target_link_libraries(ArrayWrapper PRIVATE LibPressio::libpressio)
target_link_libraries(CRAM PUBLIC LibPressio::libpressio)
