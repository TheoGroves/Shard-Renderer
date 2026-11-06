
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
    src/renderer/glfw_context.cpp
    src/renderer/vk/vulkan_instance.cpp
    src/renderer/vk/physical_device.cpp
    src/renderer/vk/logical_device.cpp
    src/renderer/vk/surface.cpp
    src/renderer/vk/swapchain.cpp
    src/renderer/vk/swapchain_image_views.cpp
    src/renderer/vk/command_pool.cpp
    src/renderer/vk/command_buffers.cpp
    src/renderer/vk/synchronization.cpp
    src/renderer/vk/graphics_pipeline.cpp
    src/renderer/vk/buffer.cpp
    src/renderer/window/window.cpp
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

add_executable(Tests
    tests/glfw_context_test.cpp
    tests/vulkan_test.cpp
    tests/window_test.cpp
    tests/engine_test.cpp
    tests/logical_device_test.cpp
    tests/physical_device_test.cpp
    tests/swapchain_test.cpp
    tests/swapchain_image_views_test.cpp
    tests/command_pool_test.cpp
    tests/command_buffers_test.cpp
    tests/synchronization_test.cpp
    tests/graphics_pipeline_test.cpp
    tests/buffer_test.cpp
)

target_link_libraries(Tests PRIVATE
    Renderer
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(Tests
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:Tests>"
)

# Copy shaders to compiled directory
add_custom_command(TARGET ShardRenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:ShardRenderer>/shaders"
)

add_custom_command(TARGET Tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:Tests>/shaders"
)