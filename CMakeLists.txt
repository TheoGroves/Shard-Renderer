
cmake_minimum_required(VERSION 3.24)

project(ShardRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies

find_package(GTest REQUIRED PATHS
    "${CMAKE_SOURCE_DIR}/dependencies/GoogleTest/install"
    NO_DEFAULT_PATH
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/GLFW)

set(VULKAN_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/dependencies/Vulkan/include"
)

set(VULKAN_LOADER_DIR
    "${CMAKE_SOURCE_DIR}/dependencies/VulkanLoader/install"
)

find_library(VULKAN_LIBRARY
    NAMES vulkan-1
    PATHS "${VULKAN_LOADER_DIR}/lib"
    NO_DEFAULT_PATH
    REQUIRED
)

# Renderer library
add_library(Renderer STATIC
    src/renderer/engine.cpp
    src/renderer/vulkan_instance.cpp
    src/window/window.cpp
)

target_include_directories(Renderer PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${VULKAN_INCLUDE_DIR}"
)

target_link_libraries(Renderer PUBLIC
    glfw
    "${VULKAN_LIBRARY}"
)

# Main exe

add_executable(ShardRenderer
    src/main.cpp
)

target_link_libraries(ShardRenderer PRIVATE
    Renderer
)

# Tests
enable_testing()

add_executable(VulkanTests
    tests/vulkan_test.cpp
)

target_link_libraries(VulkanTests PRIVATE
    Renderer
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(VulkanTests)