#include "synchronization.h"

#include <stdexcept>
#include <string>
#include <iostream>

Synchronization::Synchronization(const LogicalDevice& logical_device) {
    device = logical_device.get_handle();

    create_semaphores();
    create_fence();
}

Synchronization::~Synchronization() {
    if (in_flight_fence != VK_NULL_HANDLE) {
        vkDestroyFence(device, in_flight_fence, nullptr);
    }

    if (render_finished_semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(device, render_finished_semaphore, nullptr);
    }

    if (image_available_semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(device, image_available_semaphore, nullptr);
    }
}

void Synchronization::create_semaphores() {
    VkSemaphoreCreateInfo create_info{};

    create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkResult result = vkCreateSemaphore(device, &create_info, nullptr, &image_available_semaphore);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image available semaphore: " + std::to_string(static_cast<int>(result)));
    }

    result = vkCreateSemaphore(device, &create_info, nullptr, &render_finished_semaphore);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create render finished semaphore: " + std::to_string(static_cast<int>(result)));
    }

    std::cout << "Vulkan sempahores created successfully.\n";
}

void Synchronization::create_fence() {
    VkFenceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkResult result = vkCreateFence(device, &create_info, nullptr, &in_flight_fence);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create in-flight fence: " + std::to_string(static_cast<int>(result)));
    }

    std::cout << "Vulkan in-flight fence created successfully.\n";
}

VkSemaphore Synchronization::get_image_available_semaphore() const {
    return image_available_semaphore;
}

VkSemaphore Synchronization::get_render_finished_semaphore() const {
    return render_finished_semaphore;
}

VkFence Synchronization::get_in_flight_fence() const {
    return in_flight_fence;
}