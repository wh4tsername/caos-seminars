project("task0")

cmake_minimum_required(VERSION 3.16)

find_package(PythonLibs 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#python_add_module(matrix 0.c)
python_add_module(matrix task0.c)

#add_executable(task0 task0.c)

#target_link_libraries(task0 ${PYTHON_LIBRARIES})

