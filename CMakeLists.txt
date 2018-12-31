cmake_minimum_required(VERSION 3.1)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project (elements_counting C CXX)
add_executable(${PROJECT_NAME} main.cpp)
