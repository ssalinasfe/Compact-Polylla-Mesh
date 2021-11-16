cmake_minimum_required(VERSION 3.18)

project(compressDelfin)

add_subdirectory(sdsl-lite)
add_subdirectory(src)
include_directories(src)

add_executable(compressDelfin main.cpp)

target_link_libraries(compressDelfin PUBLIC voidfiles sdsl)