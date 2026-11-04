#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.h"
#include <vector>

class Synchronization {
public:
    explicit Synchronization(const LogicalDevice& logical_device, uint32_t swapchain_image_count = 1);
    ~Synchronization();

    Synchronization(const Synchronization&) = delete;
    Synchronization& operator=(const Synchronization&) = delete;

    VkSemaphore get_image_available_semaphore() const;
    VkSemaphore get_render_finished_semaphore(uint32_t image_index) const;
    VkFence get_in_flight_fence() const;

private:
    VkDevice device = VK_NULL_HANDLE;

    VkSemaphore image_available_semaphore = VK_NULL_HANDLE;
    std::vector<VkSemaphore> render_finished_semaphores;

    VkFence in_flight_fence = VK_NULL_HANDLE;

    void create_semaphores(uint32_t swapchain_image_count);
    void create_fence();
};