cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(CAOS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64 -DFUSE2")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

find_package(FUSE REQUIRED)

include_directories(${FUSE_INCLUDE_DIR})
add_executable(main main2.c)
target_link_libraries(main ${FUSE_LIBRARIES})

