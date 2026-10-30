cmake_minimum_required(VERSION 3.24)

project(ShardRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_executable(ShardRenderer
    src/main.cpp

    src/renderer/engine.cpp
    src/renderer/vulkan_instance.cpp
    
    src/window/window.cpp
)

target_include_directories(ShardRenderer PRIVATE
    "${VULKAN_INCLUDE_DIR}"
)

target_link_libraries(ShardRenderer PRIVATE
    glfw
    "${VULKAN_LIBRARY}"
)