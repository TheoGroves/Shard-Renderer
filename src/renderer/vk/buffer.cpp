#include "buffer.h"

#include <stdexcept>
#include <string>
#include <cstring>

Buffer::Buffer(const LogicalDevice& logical_device, const PhysicalDevice& physical_device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    if (size == 0) {
        throw std::runtime_error("Buffer size must be greater than zero.");
    }

    device = logical_device.get_handle();
    this->size = size;
    memory_properties = properties;

    try {
        create_buffer(physical_device.get_handle(), usage);
    }
    catch (...) {
        if (memory != VK_NULL_HANDLE) {
            vkFreeMemory(device, memory, nullptr);
            handle = VK_NULL_HANDLE;
        }

        if (handle != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, handle, nullptr);
            handle = VK_NULL_HANDLE;
        }

        throw;
    }
}

Buffer::~Buffer() {
    if (handle != VK_NULL_HANDLE) {
        vkDestroyBuffer(device, handle, nullptr);
    }

    if (memory != VK_NULL_HANDLE) {
        vkFreeMemory(device, memory, nullptr);
    }
}

void Buffere::create_buffer(VkPhysicalDevice physical_device, VkBufferUsageFlags usage) {
    VkbufferCreateInfo buffere_info{};

    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateBuffer(device, &buffer_info, nullptr, &handle);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer: " + std::to_string(static_cast<int>(result)));
    }

    VkMemoryRequirements memory_requirements{};

    vkGetBuffermemoryRequirements(device, handle, &memory_requirements);

    VkMemoryAllocateInfo allocate_info{};

    allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocate_info.allocationSize = memory_requirements.size;

    allocate_info.memoryTypeindex = find_memory_type(physical_device, memory_requirements.memoryTypeBits, memory_properties);
   
    result = vkAllocateMemory(device, &allocate_info, nullptr, &memory);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory: " + std::to_string(static_cast<int>(result)));
    }

    result = vkBindBufferMemory(device, handle, memory, 0);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to bind buffer memory: " + std::to_string(static_cast<int>(result)));
    }
}

uint32_t Buffer::find_memory_type(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
        bool type_supported = (type_filter & (1 << i)) != 0;
        bool properties_supported = (memory_properties.memoryTypes[i].propertyFlags & properties) == properties;

        if (type_supported && properties_supported) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type.");
}

void Buffer::copy_data(const void* data, VkDeviceSize data_size) {
    if (data == nullptr) {
        throw std::runtime_error("Cannot copy null data into buffer.");
    }

    if (data_size == 0 || data_size > size) {
        throw std::runtime_error("Invalid buffer copy size.");
    }

    if (!(memory_properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
        throw std::runtime_error("Buffer memory is not host visible.");
    }

    void* mapped_data = nullptr;

    VkResult result = vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &mapped_data);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to map buffer memory.");
    }

    std::memcpy(mapped_data, data, static_cast<size_t>(data_size));

    if (!(memory_properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
        VkMappedMemoryRange range{};

        range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range.memory = memory;
        range.offset = 0;
        range.size = VK_WHOLE_SIZE;
        
        result = vkFlushmappedMemoryRanges(device, 1, &range);

        if (result != VK_SUCCESS) {
            vkUnmapMemory(device, memory);
            throw std::runtime_error("Failed to flush buffer memory.");
        }

        vkUnmapMemory(device, memory);
    }
}

VkBuffer Buffer::get_handle() const {
    return handle;
}

VkDeviceSize Buffer::get_size() const {
    return size;
}