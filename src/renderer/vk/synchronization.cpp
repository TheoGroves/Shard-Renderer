#include "synchronization.h"

#include <stdexcept>
#include <string>
#include <iostream>

Synchronization::Synchronization(const LogicalDevice& logical_device, uint32_t frames_in_flight, uint32_t swapchain_image_count) {
    device = logical_device.get_handle();

    if (frames_in_flight == 0 || swapchain_image_count == 0) {
        throw std::runtime_error("Synchronization counts must be greater than zero.");
    }

    create_semaphores(frames_in_flight, swapchain_image_count);
    create_fences(frames_in_flight);
}

Synchronization::~Synchronization() {
    for (VkFence fence : in_flight_fences) {
        if (fence != VK_NULL_HANDLE) {
            vkDestroyFence(device, fence, nullptr);
        }
    }

    for (VkSemaphore semaphore : image_available_semaphores) {
        if (semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, semaphore, nullptr);
        }
    }

    for (VkSemaphore semaphore : render_finished_semaphores) {
        if (semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, semaphore, nullptr);
        }
    }
}

void Synchronization::create_semaphores(uint32_t frames_in_flight, uint32_t swapchain_image_count) {
    VkSemaphoreCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    image_available_semaphores.resize(frames_in_flight, VK_NULL_HANDLE);
    render_finished_semaphores.resize(swapchain_image_count, VK_NULL_HANDLE);

    for (auto& semaphore : image_available_semaphores) {
        VkResult result = vkCreateSemaphore(device, &create_info, nullptr, &semaphore);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image available semaphore: " + std::to_string(static_cast<int>(result)));
        }
    }

    for (auto& semaphore : render_finished_semaphores) {
        VkResult result = vkCreateSemaphore(device, &create_info, nullptr, &semaphore);
    
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render finished semaphore: " + std::to_string(static_cast<int>(result)));
        }
    }

    std::cout << "Vulkan sempahores created successfully.\n";
}

void Synchronization::create_fences(uint32_t frames_in_flight) {
    VkFenceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    in_flight_fences.resize(frames_in_flight, VK_NULL_HANDLE);

    for (auto& fence : in_flight_fences) {
        VkResult result = vkCreateFence(device, &create_info, nullptr, &fence);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create in-flight fence: " + std::to_string(static_cast<int>(result)));
        }
    }

    std::cout << "Vulkan in-flight fences created successfully.\n";
}

VkSemaphore Synchronization::get_image_available_semaphore(uint32_t frame_index) const {
    if (frame_index >= render_finished_semaphores.size()) {
        throw std::runtime_error("Invalid frame index.");
    }

    return image_available_semaphores[frame_index];
}

VkSemaphore Synchronization::get_render_finished_semaphore(uint32_t image_index) const {
    if (image_index >= render_finished_semaphores.size()) {
        throw std::runtime_error("Invalid swapchain image index.");
    }
    
    return render_finished_semaphores[image_index];
}

VkFence Synchronization::get_in_flight_fence(uint32_t frame_index) const {
    if (frame_index >= render_finished_semaphores.size()) {
        throw std::runtime_error("Invalid frame index.");
    }
    
    return in_flight_fences[frame_index];
}