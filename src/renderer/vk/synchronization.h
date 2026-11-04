#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.h"

class Synchronization {
public:
    explicit Synchronization(const LogicalDevice& logical_device);
    ~Synchronization();

    Synchronization(const Synchronization&) = delete;
    Synchronization& operator=(const Synchronization&) = delete;

    VkSemaphore get_image_available_semaphore() const;
    VkSemaphore get_render_finished_semaphore() const;
    VkFence get_in_flight_fence() const;

private:
    VkDevice device = VK_NULL_HANDLE;

    VkSemaphore image_available_semaphore = VK_NULL_HANDLE;
    VkSemaphore render_finished_semaphore = VK_NULL_HANDLE;

    VkFence in_flight_fence = VK_NULL_HANDLE;

    void create_semaphores();
    void create_fence();
};