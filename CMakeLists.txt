cmake_minimum_required(VERSION 3.5)

project(feistelxor CXX C)

option(ENABLE_STATIC "Build a static version of the libraries?" ON)
option(DISABLE_SHARED "Do not build a shared version of the libraries?" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

# include directories
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

# cipher srcs
add_library(libfeistelxor STATIC src/main.cpp src/padding.cpp)
set_target_properties(libfeistelxor PROPERTIES OUTPUT_NAME "feistelxor")

# googletest
add_subdirectory(test)
enable_testing()
add_test(NAME MainTest COMMAND Test)
