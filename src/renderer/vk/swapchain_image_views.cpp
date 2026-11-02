#include "swapchain_image_views.h"

#include <stdexcept>
#include <string>

SwapchainImageViews::SwapchainImageViews(const LogicalDevice& logical_device, const Swapchain& swapchain) {
    device = logical_device.get_handle();
    create_image_views(swapchain);
}

SwapchainImageViews::~SwapchainImageViews() {
    for (const auto& image_view : image_views) {
        if (image_view != VK_NULL_HANDLE) {
            vkDestroyImageView(device, image_view, nullptr);
        }
    }
}

void SwapchainImageViews::create_image_views(const Swapchain& swapchain) {
    const std::vector<VkImage>& images = swapchain.get_images();

    image_views.resize(images.size(), VK_NULL_HANDLE);

    for (size_t i = 0; i < images.size(); i++) {
        VkImageViewCreateInfo create_info{};

        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = images[i];

        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swapchain.get_image_format();

        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;

        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        VkResult result = vkCreateImageView(device, &create_info, nullptr, &image_views[i]);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swapchain image view: " + std::to_string(static_cast<int>(result)));
        }
    }
}

const std::vector<VkImageView>& SwapchainImageViews::get_image_views() const {
    return image_views;
}