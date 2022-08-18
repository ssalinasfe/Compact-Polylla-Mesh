cmake_minimum_required(VERSION 3.10)

project(compactPolylla)

add_subdirectory(external)
add_subdirectory(src)
include_directories(src)

add_executable(compactPolylla main.cpp)

target_link_libraries(compactPolylla PUBLIC polyllafiles sdsl)