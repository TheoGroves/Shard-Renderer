cmake_minimum_required(VERSION 3.15)

project(shard_render_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEPS_DIR ${CMAKE_SOURCE_DIR}/dependencies)

include_directories(
    ${DEPS_DIR}/include
)

link_directories(
    ${DEPS_DIR}/lib
)

execute_process(
    COMMAND python -c "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE PYBIND11_AUTO_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(PYBIND11_AUTO_DIR)
    set(pybind11_DIR "${PYBIND11_AUTO_DIR}")
endif()

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

add_library(renderer_core STATIC
    src/Core/Engine.cpp
    src/Rendering/Window.cpp
    src/Rendering/Shader.cpp
)

target_include_directories(renderer_core PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

pybind11_add_module(shard_render_engine
    src/Bindings/RendererBindings.cpp
)

target_link_libraries(shard_render_engine PRIVATE
    renderer_core
)

if (WIN32)
    target_link_libraries(renderer_core PRIVATE
        glfw3
        glew32s
        opengl32
        user32
        gdi32
        shell32
    )

    target_compile_definitions(renderer_core PRIVATE
        GLEW_STATIC
    )
endif()

set_target_properties(shard_render_engine PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/shard_renderer"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/shard_renderer"
)