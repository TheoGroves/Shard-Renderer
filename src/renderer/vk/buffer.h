#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.h"
#include "physical_device.h"

#include <cstdint>

class Buffer {
public:
    Buffer(const LogicalDevice& logical_device, const PhysicalDevice& physical_device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    void copy_data(const void* data, VkDeviceSize size);

    VkBuffer get_handle() const;
    VkDeviceSize get_size() const;

private:
    VkDevice device = VK_NULL_HANDLE;

    VkBuffer handle = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkDeviceSize size = 0;
    VkMemoryPropertyFlags memory_properties = 0;

    uint32_t find_memory_type(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties) const;
    
    void create_buffer(VkPhysicalDevice physical_device, VkBufferUsageFlags usage);
};