cmake_minimum_required(VERSION 3.15)

project(openfile LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_TEST "Build the test" ON)

add_subdirectory(src)

if(BUILD_TEST)
    add_subdirectory(test)
endif()