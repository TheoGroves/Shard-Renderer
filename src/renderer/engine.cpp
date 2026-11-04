#include "engine.h"
#include "glfw_context.h"
#include <iostream>
#include <stdexcept>

Engine::Engine(uint32_t width, uint32_t height) {
    context = std::make_shared<GlfwContext>();
    main_window = std::make_unique<Window>(context, width, height, "ShardRenderer");
    instance = std::make_unique<VulkanInstance>();
    surface = std::make_unique<Surface>(instance->get_handle(), main_window->get_handle());
    physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
    logical_device = std::make_unique<LogicalDevice>(*physical_device);
    swapchain = std::make_unique<Swapchain>(*physical_device, *logical_device, *surface, *main_window);
    swapchain_image_views = std::make_unique<SwapchainImageViews>(*logical_device, *swapchain);
    command_pool = std::make_unique<CommandPool>(*logical_device);
    command_buffers = std::make_unique<CommandBuffers>(*logical_device, *command_pool, 1);
    synchronization = std::make_unique<Synchronization>(*logical_device);
}

Engine::~Engine() {
    std::cout << "Engine shutting down.\n";
}

void Engine::begin_frame() {
    if (frame_open) {
        throw std::runtime_error("Cannot open a frame before closing the previous one.");
    }

    VkFence fence = synchronization->get_in_flight_fence();

    VkResult result = vkWaitForFences(logical_device->get_handle(), 1, &fence, VK_TRUE, UINT64_MAX);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed waiting for in-flight fence.");
    }

    main_window->poll_events();

    acquire_next_image();

    frame_open = true;
}

void Engine::end_frame() {
    if (!frame_open) {
        throw std::runtime_error("Cannot close a frame before opening a frame.");
    }

    record_command_buffer();

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

void Engine::acquire_next_image() {
    VkResult result = vkAcquireNextImageKHR(
        logical_device->get_handle(),
        swapchain->get_handle(),
        UINT64_MAX,
        synchronization->get_image_available_semaphore(),
        VK_NULL_HANDLE,
        &current_image_index
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        throw std::runtime_error("Swapchain is out of date.");
        // TODO: implement resizing
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swapchain image: " + std::to_string(static_cast<int>(result)));
    }
}

void Engine::record_command_buffer() {
    VkCommandBuffer command_buffer = command_buffers->get_buffers()[0];

    command_buffers->reset(command_buffer);
    command_buffers->begin(command_buffer);

    // TODO: Dynamic rendering commands

    command_buffers->end(command_buffer);
}