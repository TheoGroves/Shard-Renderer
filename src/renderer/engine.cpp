#include "engine.h"
#include <iostream>

Engine::Engine(uint32_t width, uint32_t height) {
    main_window = std::make_unique<Window>(width, height, "ShardRenderer");
    instance = std::make_unique<VulkanInstance>();
}

Engine::~Engine() {
    std::cout << "Engine shutting down.\n";
}

void Engine::begin_frame() {
    main_window->poll_events();
}

void Engine::end_frame() {}

bool Engine::should_close() const {
    return main_window->should_close();
}

GLFWwindow* Engine::get_window_handle() {
    return main_window->get_handle();
}

VkInstance Engine::get_vk_handle() {
    return instance->get_handle();
}