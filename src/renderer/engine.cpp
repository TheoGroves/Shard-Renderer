#include "engine.h"
#include "glfw_context.h"
#include <iostream>
#include <stdexcept>
#include <array>
#include <cstdint>

Engine::Engine(uint32_t width, uint32_t height) {
    context = std::make_shared<GlfwContext>();
    main_window = std::make_unique<Window>(context, width, height, "ShardRenderer");
    instance = std::make_unique<VulkanInstance>();
    surface = std::make_unique<Surface>(instance->get_handle(), main_window->get_handle());
    physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
    logical_device = std::make_unique<LogicalDevice>(*physical_device);
    swapchain = std::make_unique<Swapchain>(*physical_device, *logical_device, *surface, *main_window);
    swapchain_image_views = std::make_unique<SwapchainImageViews>(*logical_device, *swapchain);
    graphics_pipeline = std::make_unique<GraphicsPipeline>(*logical_device, *swapchain, "shaders/triangle.vert.spv", "shaders/triangle.frag.spv");
    command_pool = std::make_unique<CommandPool>(*logical_device);
    command_buffers = std::make_unique<CommandBuffers>(*logical_device, *command_pool, 1);
    synchronization = std::make_unique<Synchronization>(*logical_device, static_cast<uint32_t>(swapchain->get_images().size()));
}

Engine::~Engine() {
    std::cout << "Engine shutting down.\n";
}

void Engine::begin_frame() {
    if (frame_open) {
        throw std::runtime_error("Cannot open a frame before closing the previous one.");
    }

    VkFence fence = synchronization->get_in_flight_fence();

    VkResult result = vkWaitForFences(logical_device->get_handle(), 1, &fence, VK_TRUE, UINT64_MAX);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed waiting for in-flight fence.");
    }

    main_window->poll_events();

    while (!acquire_next_image()) {
        recreate_swapchain();
    }

    frame_open = true;
}

void Engine::end_frame() {
    if (!frame_open) {
        throw std::runtime_error("Cannot close a frame before opening a frame.");
    }

    record_command_buffer();
    submit_command_buffer();
    present_image();

    frame_open = false;

    if (recreate_after_present) {
        recreate_after_present = false;
        recreate_swapchain();
    }
}

bool Engine::should_close() const {
    return main_window->should_close();
}

GLFWwindow* Engine::get_window_handle() {
    return main_window->get_handle();
}

VkInstance Engine::get_vk_handle() {
    return instance->get_handle();
}

VkPhysicalDevice Engine::get_device_handle() {
    return physical_device->get_handle();
}

bool Engine::acquire_next_image() {
    VkResult result = vkAcquireNextImageKHR(
        logical_device->get_handle(),
        swapchain->get_handle(),
        UINT64_MAX,
        synchronization->get_image_available_semaphore(),
        VK_NULL_HANDLE,
        &current_image_index
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        return false;
    }

    if (result == VK_SUBOPTIMAL_KHR) {
        recreate_after_present = true;
        return true;
    }

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to acquire swapchain image: " + std::to_string(static_cast<int>(result)));
    }

    return true;
}

void Engine::record_command_buffer() {
    VkCommandBuffer command_buffer = command_buffers->get_buffers()[0];

    command_buffers->reset(command_buffer);
    command_buffers->begin(command_buffer);

    VkImage image = swapchain->get_images()[current_image_index];

    transition_image_layout(command_buffer, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    begin_rendering(command_buffer);

    set_viewport_and_scissor(command_buffer);

    graphics_pipeline->bind(command_buffer);

    // Draw triangle
    vkCmdDraw(
        command_buffer,
        3,
        1,
        0,
        0
    );

    end_rendering(command_buffer);

    transition_image_layout(command_buffer, image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    command_buffers->end(command_buffer);
}

void Engine::transition_image_layout(VkCommandBuffer command_buffer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout) {
    VkImageMemoryBarrier barrier{};

    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;

    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags src_stage;
    VkPipelineStageFlags dst_stage;

    if (new_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dst_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (new_layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = 0;

        src_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dst_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    else {
        throw std::runtime_error("Unsupported image layout transition.");
    }

    vkCmdPipelineBarrier(command_buffer, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void Engine::begin_rendering(VkCommandBuffer command_buffer) {
    VkRenderingAttachmentInfo color_attachment{};
    color_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    color_attachment.imageView = swapchain_image_views->get_image_views()[current_image_index];
    color_attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.clearValue.color = {{ 0.05f, 0.05f, 0.05f, 1.0f }};

    VkRenderingInfo rendering_info{};
    rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    rendering_info.renderArea.offset = { 0, 0 };
    rendering_info.renderArea.extent = swapchain->get_extent();
    rendering_info.layerCount = 1;
    rendering_info.colorAttachmentCount = 1;
    rendering_info.pColorAttachments = &color_attachment;

    vkCmdBeginRendering(command_buffer, &rendering_info);
}

void Engine::end_rendering(VkCommandBuffer command_buffer) {
    vkCmdEndRendering(command_buffer);
}

void Engine::submit_command_buffer() {
    VkCommandBuffer command_buffer = command_buffers->get_buffers()[0];

    VkSemaphore wait_semaphore = synchronization->get_image_available_semaphore();
    VkSemaphore signal_semaphore = synchronization->get_render_finished_semaphore(current_image_index);
    
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info{};

    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &wait_semaphore;
    submit_info.pWaitDstStageMask = &wait_stage;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &signal_semaphore;

    VkFence fence = synchronization->get_in_flight_fence();

    VkResult result = vkResetFences(logical_device->get_handle(), 1, &fence);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to reset in-flight fence.");
    }

    result = vkQueueSubmit(logical_device->get_graphics_queue(), 1, &submit_info, fence);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit command buffer.");
    }
}

void Engine::present_image() {
    VkSemaphore wait_semaphore = synchronization->get_render_finished_semaphore(current_image_index);

    VkSwapchainKHR swapchain_handle = swapchain->get_handle();

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &wait_semaphore;

    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain_handle;

    present_info.pImageIndices = &current_image_index;

    VkResult result = vkQueuePresentKHR(logical_device->get_present_queue(), &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        recreate_after_present = true;
        return;
    }

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swapchain image.");
    }
}

void Engine::set_viewport_and_scissor(VkCommandBuffer command_buffer) {
    VkExtent2D extent = swapchain->get_extent();

    VkViewport viewport{};

    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;

    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
}

void Engine::recreate_swapchain() {
    int width = 0;
    int height = 0;

    while (width == 0 || height == 0) {
        context->get_framebuffer_size(main_window->get_handle(), &width, &height);

        if (width == 0 || height == 0) {
            glfwWaitEvents();
        }
    }

    VkResult result = vkDeviceWaitIdle(logical_device->get_handle());

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed waiting for device to become idle.");
    }

    VkSwapchainKHR old_swapchain = swapchain->get_handle();

    auto new_swapchain = std::make_unique<Swapchain>(*physical_device, *logical_device, *surface, *main_window, old_swapchain);
    auto new_swapchain_image_views = std::make_unique<SwapchainImageViews>(*logical_device, *new_swapchain);
    auto new_graphics_pipeline = std::make_unique<GraphicsPipeline>(*logical_device, *new_swapchain, "shaders/triangle.vert.spv", "shaders/triangle.frag.spv");
    auto new_synchronization = std::make_unique<Synchronization>(*logical_device, static_cast<uint32_t>(new_swapchain->get_images().size()));

    graphics_pipeline = std::move(new_graphics_pipeline);
    synchronization = std::move(new_synchronization);
    swapchain_image_views = std::move(new_swapchain_image_views);
    swapchain = std::move(new_swapchain);

    std::cout << "Swapchain recreated successfully.\n";
}