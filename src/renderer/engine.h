#pragma once

#include "vk/vulkan_instance.h"
#include "vk/physical_device.h"
#include "vk/surface.h"
#include "../window/window.h"
#include <memory>

class Engine {
public:
    Engine(uint32_t width, uint32_t height);
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void begin_frame();
    void end_frame();

    bool should_close() const;

    GLFWwindow* get_window_handle();
    VkInstance get_vk_handle();
    VkPhysicalDevice get_device_handle();
private:
    std::unique_ptr<Window> main_window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;

    bool frame_open = false;
};