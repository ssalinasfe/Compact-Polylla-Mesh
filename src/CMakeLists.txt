file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

add_library(voidfiles STATIC ${SOURCE_FILES} ${HEADER_FILES})