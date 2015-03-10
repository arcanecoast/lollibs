cmake_minimum_required(VERSION 3.1)

project(lollibs)

# Library compilation instructions
# --------------------------------

include_directories(include)

FILE(GLOB LOLLIBS_SOURCE_FILES src/*.cpp)
add_library(lollibs STATIC ${LOLLIBS_SOURCE_FILES})

# Library testing instructions
# ----------------------------

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
    CXXTEST_ADD_TEST(unittest_fdt fdt_test.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/TestFDTSaveLoad.h)
    target_link_libraries(unittest_fdt lollibs)
#else()
#    message(SEND_ERROR "You must have CxxTest on your system to build lollibs" )
endif()

# Library installation instructions
# ---------------------------------

install(TARGETS lollibs DESTINATION lib)

FILE(GLOB LOLLIBS_HEADERS_FILES include/lollibs/*.h)
install(FILES ${LOLLIBS_HEADERS_FILES} DESTINATION include/lollibs)
