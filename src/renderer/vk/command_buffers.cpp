#include "command_buffers.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>

CommandBuffers::CommandBuffers(const LogicalDevice& logical_device, const CommandPool& command_pool, uint32_t count) {
    device = logical_device.get_handle();
    pool = command_pool.get_handle();

    if (count == 0) {
        throw std::runtime_error("Command buffer count must be greater than zero.");
    }

    allocate_command_buffers(count);
}

CommandBuffers::~CommandBuffers() {
    if (!buffers.empty()) {
        vkFreeCommandBuffers(device, pool, static_cast<uint32_t>(buffers.size()), buffers.data());
    }
}

void CommandBuffers::allocate_command_buffers(uint32_t count) {
    buffers.resize(count, VK_NULL_HANDLE);

    VkCommandBufferAllocateInfo allocate_info{};

    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool = pool;
    allocate_info.commandBufferCount = count;

    VkResult result = vkAllocateCommandBuffers(device, &allocate_info, buffers.data());
    
    if (result != VK_SUCCESS) {
        buffers.clear();
        throw std::runtime_error("Failed to allocate commmand buffers: " + std::to_string(static_cast<int>(result)));
    }

    std::cout << "Allocated " << count << " command buffers.\n";
}

void CommandBuffers::begin(VkCommandBuffer command_buffer) {
    VkCommandBufferBeginInfo begin_info{};

    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkResult result = vkBeginCommandBuffer(command_buffer, &begin_info);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin command buffer: " + std::to_string(static_cast<int>(result)));
    }
}

void CommandBuffers::end(VkCommandBuffer command_buffer) {
    VkResult result = vkEndCommandBuffer(command_buffer);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to end command buffer: " + std::to_string(static_cast<int>(result)));
    }
}

void CommandBuffers::reset(VkCommandBuffer command_buffer) {
    VkResult result = vkResetCommandBuffer(command_buffer, 0);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to reset command buffer: " + std::to_string(static_cast<int>(result)));
    }
}

const std::vector<VkCommandBuffer>& CommandBuffers::get_buffers() const {
    return buffers;
}