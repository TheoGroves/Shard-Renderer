#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.h"

class CommandPool {
public:
    explicit CommandPool(const LogicalDevice& logical_device);
    ~CommandPool();

    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;

    VkCommandPool get_handle() const;

private:
    VkDevice device = VK_NULL_HANDLE;
    VkCommandPool handle = VK_NULL_HANDLE;

    void create_command_pool(const LogicalDevice& logical_device);
};