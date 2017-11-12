cmake_minimum_required(VERSION 3.5)

# set runtime directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Include files directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

# Library directory
link_directories(${CMAKE_CURRENT_LIST_DIR}/lib/)

# Generate executable file
add_executable(MainTest ${CMAKE_CURRENT_SOURCE_DIR}/src/test_main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/test_padding.cpp)

# Link libraries
target_link_libraries(MainTest libfeistelxor gtest gtest_main)
