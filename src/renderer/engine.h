#pragma once

#include "engine.h"
#include "vulkan_instance.h"
#include "../window/window.h"

class Engine {
public:
    void init(uint32_t width, uint32_t height);
    void cleanup();

    void begin_frame();
    void end_frame();

    bool should_close() const;
private:
    Window* main_window = nullptr;
    VulkanInstance* instance = nullptr;
};