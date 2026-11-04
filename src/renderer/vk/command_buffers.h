#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#include "logical_device.h"
#include "command_pool.h"

class CommandBuffers {
public:
    explicit CommandBuffers(const LogicalDevice& logical_device, const CommandPool& command_pool, uint32_t count);
    ~CommandBuffers();

    CommandBuffers(const CommandBuffers&) = delete;
    CommandBuffers& operator=(const CommandBuffers&) = delete;

    const std::vector<VkCommandBuffer>& get_buffers() const;

    void begin(VkCommandBuffer command_buffer);
    void end(VkCommandBuffer command_buffer);
    void reset(VkCommandBuffer command_buffer);

private:
    VkDevice device = VK_NULL_HANDLE;
    VkCommandPool pool = VK_NULL_HANDLE;

    std::vector<VkCommandBuffer> buffers;

    void allocate_command_buffers(uint32_t count);
};