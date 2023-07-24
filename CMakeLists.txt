cmake_minimum_required(VERSION 3.15)
project(named_tuples)

include_directories(include)
find_package(fmt REQUIRED)
add_executable(${PROJECT_NAME} src/test.cpp)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
