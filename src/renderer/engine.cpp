#include "engine.h"
#include <iostream>
#include <stdexcept>

Engine::Engine(uint32_t width, uint32_t height) {
    main_window = std::make_unique<Window>(width, height, "ShardRenderer");
    instance = std::make_unique<VulkanInstance>();
    surface = std::make_unique<Surface>(instance->get_handle(), main_window->get_handle());
    physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
}

Engine::~Engine() {
    std::cout << "Engine shutting down.\n";
}

void Engine::begin_frame() {
    if (frame_open) {
        throw std::runtime_error("Cannot open a frame before closing the previous one.");
    }
    frame_open = true;
    main_window->poll_events();
}

void Engine::end_frame() {
    if (!frame_open) {
        throw std::runtime_error("Cannot close a frame before opening a frame.");
    }
    frame_open = false;
}

bool Engine::should_close() const {
    return main_window->should_close();
}

GLFWwindow* Engine::get_window_handle() {
    return main_window->get_handle();
}

VkInstance Engine::get_vk_handle() {
    return instance->get_handle();
}

VkPhysicalDevice Engine::get_device_handle() {
    return physical_device->get_handle();
}