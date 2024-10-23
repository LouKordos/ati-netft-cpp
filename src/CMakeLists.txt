cmake_minimum_required(VERSION 3.28.0)
project(ATINetFTSensor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ati-netft STATIC ati-netft.cpp include/ati-netft.hpp)
target_include_directories(ati-netft PUBLIC include)

add_subdirectory(tests)