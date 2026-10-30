#include "engine.h"
#include <iostream>

void Engine::init(uint32_t width, uint32_t height) {
    main_window = new Window(width, height, "ShardRenderer");

    instance = new VulkanInstance();
}

void Engine::cleanup() {
    std::cout << "Engine shutting down.\n";
    delete instance;
    delete main_window;
}

void Engine::begin_frame() {
    main_window->poll_events();
}

void Engine::end_frame() {}

bool Engine::should_close() const {
    return main_window->should_close();
}