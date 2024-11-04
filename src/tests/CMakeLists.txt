cmake_minimum_required(VERSION 3.28.0)
project(Test)
add_executable(sensor_test sensor_test.cpp)
target_include_directories(sensor_test PRIVATE ${PROJECT_SOURCE_DIR}/include)