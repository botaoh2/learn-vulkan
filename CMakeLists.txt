cmake_minimum_required(VERSION 3.16)

project(learn-vulkan VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(fmt REQUIRED)

add_executable(learn_vulkan main.cpp)

target_link_libraries(learn_vulkan PRIVATE fmt::fmt)
