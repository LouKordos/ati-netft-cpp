cmake_minimum_required(VERSION 3.28.0)
add_executable(sensor_test sensor_test.cpp)
target_include_directories(sensor_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(sensor_test PUBLIC TracyClient)