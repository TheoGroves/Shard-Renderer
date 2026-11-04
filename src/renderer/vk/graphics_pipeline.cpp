#include "graphics_pipeline.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <iostream>

GraphicsPipeline::GraphicsPipeline(const LogicalDevice& logical_device, const Swapchain& swapchain, const std::string& vertex_shader_path, const std::string& fragment_shader_path) {
    device = logical_device.get_handle();

    create_pipeline(swapchain, vertex_shader_path, fragment_shader_path);
}

GraphicsPipeline::~GraphicsPipeline() {
    if (handle != VK_NULL_HANDLE) {
        vkDestroyPipeline(device, handle, nullptr);
    }

    if (pipeline_layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
    }
}

std::vector<char> GraphicsPipeline::read_shader_file(const std::string& path) const {
    std::ifstream file (path, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + path);
    }

    size_t file_size = static_cast<size_t>(file.tellg());

    if (file_size == 0 || file_size % sizeof(uint32_t) != 0) {
        throw std::runtime_error("Invalid SPIR-V shader file: " + path);
    }

    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(file_size));

    if (!file) {
        throw std::runtime_error("Failed to read shader file: " + path);
    }

    return buffer;
}

VkShaderModule GraphicsPipeline::create_shader_module(const std::vector<char>& code) const {
    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shader_module = VK_NULL_HANDLE;

    VkResult result = vkCreateShaderModule(device, &create_info, nullptr, &shader_module);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module: " + std::to_string(static_cast<int>(result)));
    }

    return shader_module;
}

void GraphicsPipeline::create_pipeline(const Swapchain& swapchain, const std::string& vertex_shader_path, const std::string& fragment_shader_path) {
    const std::vector<char> vertex_code = read_shader_file(vertex_shader_path);
    const std::vector<char> fragment_code = read_shader_file(fragment_shader_path);

    VkShaderModule vertex_shader = create_shader_module(vertex_code);
    VkShaderModule fragment_shader = VK_NULL_HANDLE;

    try {
        fragment_shader = create_shader_module(fragment_code);

        VkPipelineShaderStageCreateInfo vertex_stage{};
        vertex_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_stage.module = vertex_shader;
        vertex_stage.pName = "main";

        VkPipelineShaderStageCreateInfo fragment_stage{};
        fragment_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment_stage.module = fragment_shader;
        fragment_stage.pName = "main";

        VkPipelineShaderStageCreateInfo shader_stages[] = {
            vertex_stage, 
            fragment_stage
        };

        VkPipelineVertexInputStateCreateInfo vertex_input{};
        vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        
        VkPipelineInputAssemblyStateCreateInfo input_assembly{};
        input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewport_state{};
        viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state.viewportCount = 1;
        viewport_state.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_NONE;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo color_blending{};
        color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blending.logicOpEnable = VK_FALSE;
        color_blending.attachmentCount = 1;
        color_blending.pAttachments = &color_blend_attachment;

        const std::vector<VkDynamicState> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state{};
        dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
        dynamic_state.pDynamicStates = dynamic_states.data();

        VkPipelineLayoutCreateInfo layout_info{};
        layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        VkResult result = vkCreatePipelineLayout(device, &layout_info, nullptr, &pipeline_layout);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout: " + std::to_string(static_cast<int>(result)));
        }

        VkPipelineRenderingCreateInfo rendering_info{};
        rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        VkFormat color_format = swapchain.get_image_format();
        rendering_info.colorAttachmentCount = 1;
        rendering_info.pColorAttachmentFormats = &color_format;

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.pNext = &rendering_info;
        pipeline_info.stageCount = 2;
        pipeline_info.pStages = shader_stages;

        pipeline_info.pVertexInputState = &vertex_input;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer;
        pipeline_info.pMultisampleState = &multisampling;
        pipeline_info.pColorBlendState = &color_blending;
        pipeline_info.pDynamicState = &dynamic_state;

        pipeline_info.layout = pipeline_layout;
        pipeline_info.renderPass = VK_NULL_HANDLE;
    
        result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &handle);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics pipeline: " + std::to_string(static_cast<int>(result)));
        }

        std::cout << "Vulkan graphics pipeline created successfully.\n";
    }
    catch (...) {
        vkDestroyShaderModule(device, vertex_shader, nullptr);

        if (fragment_shader != VK_NULL_HANDLE) {
            vkDestroyShaderModule(device, fragment_shader, nullptr);
        }

        if (pipeline_layout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
            pipeline_layout = VK_NULL_HANDLE;
        }

        throw;
    }

    vkDestroyShaderModule(device, vertex_shader, nullptr);
    vkDestroyShaderModule(device, fragment_shader, nullptr);
}

void GraphicsPipeline::bind(VkCommandBuffer command_buffer) const {
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, handle);
}

VkPipeline GraphicsPipeline::get_handle() const {
    return handle;
}