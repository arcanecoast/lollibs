cmake_minimum_required(VERSION 3.1)

project(lollibs)

include_directories(include)

FILE(GLOB LOLLIBS_SOURCE_FILES src/*.cpp)
add_library(lollibs STATIC ${LOLLIBS_SOURCE_FILES})

install(TARGETS lollibs DESTINATION lib)

FILE(GLOB LOLLIBS_HEADERS_FILES include/lollibs/*.h)
install(FILES ${LOLLIBS_HEADERS_FILES} DESTINATION include/lollibs)