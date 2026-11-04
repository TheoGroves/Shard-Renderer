#pragma once

#include "vk/vulkan_instance.h"
#include "vk/physical_device.h"
#include "vk/logical_device.h"
#include "vk/surface.h"
#include "vk/swapchain.h"
#include "vk/swapchain_image_views.h"
#include "vk/command_pool.h"
#include "vk/command_buffers.h"
#include "vk/synchronization.h"
#include "vk/graphics_pipeline.h"
#include "window/window.h"
#include "glfw_context.h"
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
    std::shared_ptr<GlfwContext> context;
    std::unique_ptr<Window> main_window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;
    std::unique_ptr<Swapchain> swapchain;
    std::unique_ptr<SwapchainImageViews> swapchain_image_views;
    std::unique_ptr<CommandPool> command_pool;
    std::unique_ptr<CommandBuffers> command_buffers;
    std::unique_ptr<Synchronization> synchronization;
    std::unique_ptr<GraphicsPipeline> graphics_pipeline;

    bool frame_open = false;

    uint32_t current_image_index = 0;

    void acquire_next_image();
    void record_command_buffer();

    void transition_image_layout(VkCommandBuffer command_buffer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout);
    
    void begin_rendering(VkCommandBuffer command_buffer);
    void end_rendering(VkCommandBuffer command_buffer);

    void submit_command_buffer();
    void present_image();

    void set_viewport_and_scissor(VkCommandBuffer command_buffer);
};