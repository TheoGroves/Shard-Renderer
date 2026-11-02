#include <gtest/gtest.h>

#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/renderer/vk/swapchain.h"
#include "../src/renderer/vk/swapchain_image_views.h"
#include "../src/window/window.h"

#include <GLFW/glfw3.h>
#include <type_traits>
#include <memory>

class SwapchainImageViewsTest : public ::testing::Test {
protected:
    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;
    std::unique_ptr<Swapchain> swapchain;
    std::unique_ptr<SwapchainImageViews> image_views;

    void SetUp() override {
        ASSERT_EQ(glfwInit(), GLFW_TRUE);

        window = std::make_unique<Window>(1280, 720, "Image Views Test");

        instance = std::make_unique<VulkanInstance>();
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
        logical_device = std::make_unique<LogicalDevice>(*physical_device);
        swapchain = std::make_unique<Swapchain>(*physical_device, *logical_device, *surface, *window);
        image_views = std::make_unique<SwapchainImageViews>(*logical_device, *swapchain);
    }

    void TearDown() override {
        image_views.reset();
        swapchain.reset();
        logical_device.reset();
        physical_device.reset();
        surface.reset();
        instance.reset();
        window.reset();

        glfwTerminate();
    }
};

TEST_F(SwapchainImageViewsTest, CreatesImageViews) {
    ASSERT_FALSE(image_views->get_image_views().empty());
}

TEST_F(SwapchainImageViewsTest, CreatesOneViewPerSwapchainImage) {
    EXPECT_EQ(image_views->get_image_views().size(), swapchain->get_images().size());
}

TEST_F(SwapchainImageViewsTest, CreatesValidImageViews) {
    for (const auto& image_view : image_views->get_image_views()) {
        EXPECT_NE(image_view, VK_NULL_HANDLE);
    }
}

TEST(SwapchainImageViewsTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<SwapchainImageViews>);
}

TEST(SwapchainImageViewsTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<SwapchainImageViews>);
}