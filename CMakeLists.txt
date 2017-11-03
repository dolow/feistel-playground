cmake_minimum_required(VERSION 3.5)

project(fiestelxor CXX C)

option(ENABLE_STATIC "Build a static version of the libraries?" ON)
option(DISABLE_SHARED "Do not build a shared version of the libraries?" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_definitions(-DDEBUG_ARG=1)

# include directories
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

# cipher srcs
add_library(libfiestelxor STATIC src/main.cpp)
set_target_properties(libfiestelxor PROPERTIES OUTPUT_NAME "fiestelxor")

# googletest
add_subdirectory(test)
enable_testing()
add_test(NAME MainTest COMMAND Test)
