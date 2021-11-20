cmake_minimum_required(VERSION 3.18)

project(compressPolylla)

add_subdirectory(sdsl-lite)
add_subdirectory(src)
include_directories(src)

add_executable(compressPolylla main.cpp)

target_link_libraries(compressPolylla PUBLIC voidfiles sdsl)