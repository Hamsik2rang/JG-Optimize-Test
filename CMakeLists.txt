cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME JungleOptimizeTest)

project("${PROJECT_NAME}" VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)