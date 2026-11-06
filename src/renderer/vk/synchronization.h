#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.h"
#include <vector>
#include <cstdint>

class Synchronization {
public:
    explicit Synchronization(const LogicalDevice& logical_device, uint32_t frames_in_flight, uint32_t swapchain_image_count);
    ~Synchronization();

    Synchronization(const Synchronization&) = delete;
    Synchronization& operator=(const Synchronization&) = delete;

    VkSemaphore get_image_available_semaphore(uint32_t frame_index) const;
    VkSemaphore get_render_finished_semaphore(uint32_t image_index) const;
    VkFence get_in_flight_fence(uint32_t frame_index) const;

private:
    VkDevice device = VK_NULL_HANDLE;

    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;

    std::vector<VkFence> in_flight_fences;

    void create_semaphores(uint32_t frames_in_flight, uint32_t swapchain_image_count);
    void create_fences(uint32_t frames_in_flight);
};