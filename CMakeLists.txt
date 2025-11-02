cmake_minimum_required(VERSION 3.16)

project(learn-vulkan VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(fmt REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(volk REQUIRED)

add_executable(learn_vulkan 
    src/main.cpp
    src/test_base.cpp
    src/test_base.h
)

target_compile_definitions(learn_vulkan PRIVATE 
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLFW_INCLUDE_VULKAN
    VK_NO_PROTOTYPES # Required by volk
)

target_link_libraries(learn_vulkan PRIVATE 
    fmt::fmt
    glfw
    glm::glm
    volk::volk
)
