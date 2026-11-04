#include "command_pool.h"

#include <stdexcept>
#include <string>
#include <iostream>

CommandPool::CommandPool(const LogicalDevice& logical_device) {
    device = logical_device.get_handle();

    create_command_pool(logical_device);
}

CommandPool::~CommandPool() {
    if (handle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(device, handle, nullptr);
    }
}

void CommandPool::create_command_pool(const LogicalDevice& logical_device) {
    const QueueFamilyIndices& indices = logical_device.get_queue_family_indices();

    VkCommandPoolCreateInfo create_info{};

    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    create_info.queueFamilyIndex = indices.graphics_family.value();
    
    VkResult result = vkCreateCommandPool(device, &create_info, nullptr, &handle);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool: " + std::to_string(static_cast<int>(result)));
    }

    std::cout << "Vulkan command pool created successfully.\n";
}

VkCommandPool CommandPool::get_handle() const {
    return handle;
}