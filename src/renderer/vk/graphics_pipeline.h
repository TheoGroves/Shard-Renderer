#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.h"
#include "swapchain.h"

#include <string>
#include <vector>
#include <cstdint>

class GraphicsPipeline {
public:
    GraphicsPipeline(const LogicalDevice& logical_device, const Swapchain& swapchain, const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    ~GraphicsPipeline();

    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

    void bind(VkCommandBuffer command_buffer) const;

    VkPipeline get_handle() const;

private:
    VkDevice device = VK_NULL_HANDLE;

    VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
    VkPipeline handle = VK_NULL_HANDLE;

    VkShaderModule create_shader_module(const std::vector<char>& code) const;
    std::vector<char> read_shader_file(const std::string& path) const;

    void create_pipeline(const Swapchain& swapchain, const std::string& vertex_shader_path, const std::string& fragment_shader_path);
};