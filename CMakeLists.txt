project(elements_counting C CXX)
cmake_minimum_required(VERSION 3.1.0)


find_package(Boost 1.50.0 COMPONENTS program_options)


find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)


add_executable(${PROJECT_NAME} main.cpp)
