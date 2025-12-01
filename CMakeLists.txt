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

find_program(GLSLC_EXECUTABLE glslc REQUIRED)

function(compile_shader SHADER_SRC SHADER_SPV)
    add_custom_command(
        OUTPUT ${SHADER_SPV}
        COMMAND ${GLSLC_EXECUTABLE} ${SHADER_SRC} -o ${SHADER_SPV}
        DEPENDS ${SHADER_SRC}
        COMMENT "Compiling ${SHADER_SRC} to SPIR-V"
    )
endfunction()

set(SHADERS
    shader.vert
    shader.frag
)

set(SPIRV_FILES "")
foreach(SHADER ${SHADERS})
    set(SRC ${CMAKE_SOURCE_DIR}/shaders/${SHADER})
    set(SPV ${CMAKE_BINARY_DIR}/bin/${SHADER}.spv)
    compile_shader(${SRC} ${SPV})
    list(APPEND SPIRV_FILES ${SPV})
endforeach()

add_custom_target(compile_shaders ALL DEPENDS ${SPIRV_FILES})
