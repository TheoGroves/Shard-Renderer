#pragma once

#include <vulkan/vulkan.h>

#include "physical_device.h"

class LogicalDevice {
public:
    explicit LogicalDevice(const PhysicalDevice& physical_device);
    ~LogicalDevice();

    LogicalDevice(const LogicalDevice&) = delete;
    LogicalDevice& operator=(const LogicalDevice&) = delete;
    
    VkDevice get_handle() const;

    VkQueue get_graphics_queue() const;
    VkQueue get_present_queue() const;

private:
    VkDevice handle = VK_NULL_HANDLE;

    VkQueue graphics_queue = VK_NULL_HANDLE;
    VkQueue present_queue = VK_NULL_HANDLE;

    void create_logical_device(const PhysicalDevice& physical_device);
    void retrieve_queues(const PhysicalDevice& physical_device);
};