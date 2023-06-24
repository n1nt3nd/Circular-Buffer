cmake_minimum_required(VERSION 3.14)
project(labwork-8)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)
