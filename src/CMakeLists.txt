file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.hpp)

add_library(voidfiles STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(voidfiles PUBLIC sdsl)