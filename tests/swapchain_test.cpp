#include <gtest/gtest.h>

#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/renderer/vk/swapchain.h"
#include "../src/window/window.h"

#include <GLFW/glfw3.h>
#include <type_traits>
#include <memory>

class SwapchainTest : public ::testing::Test {
protected:
    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;
    std::unique_ptr<Swapchain> swapchain;

    void SetUp() override {
        ASSERT_EQ(glfwInit(), GLFW_TRUE);

        window = std::make_unique<Window>(1280, 720, "Swapchain Test");

        instance = std::make_unique<VulkanInstance>();
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
        logical_device = std::make_unique<LogicalDevice>(*physical_device);
        swapchain = std::make_unique<Swapchain>(*physical_device, *logical_device, *surface, *window);
    }

    void TearDown() override {
        swapchain.reset();
        logical_device.reset();
        physical_device.reset();
        surface.reset();
        instance.reset();
        window.reset();

        glfwTerminate();
    }
};

TEST_F(SwapchainTest, CreatesValidSwapchain) {
    ASSERT_NE(swapchain->get_handle(), VK_NULL_HANDLE);
}

TEST_F(SwapchainTest, ReturnsConsistentHandle) {
    VkSwapchainKHR first = swapchain->get_handle();
    VkSwapchainKHR second = swapchain->get_handle();

    EXPECT_EQ(first, second);
}

TEST_F(SwapchainTest, CreatesSwapchainImages) {
    EXPECT_FALSE(swapchain->get_images().empty());
}

TEST_F(SwapchainTest, CreatesValidSwapchainImages) {
    for (const auto& image : swapchain->get_images()) {
        EXPECT_NE(image, VK_NULL_HANDLE);
    }
}

TEST_F(SwapchainTest, ReturnsConsistentImages) {
    const auto& first = swapchain->get_images();
    const auto& second = swapchain->get_images();

    ASSERT_EQ(first.size(), second.size());

    for (size_t i = 0; i < first.size(); i++) {
        EXPECT_EQ(first[i], second[i]);
    }
}

TEST_F(SwapchainTest, HasValidImageFormat) {
    EXPECT_NE(swapchain->get_image_format(), VK_FORMAT_UNDEFINED);
}

TEST_F(SwapchainTest, HasValidExtent) {
    VkExtent2D extent = swapchain->get_extent();

    EXPECT_GT(extent.width, 0);
    EXPECT_GT(extent.height, 0);
}

TEST(SwapchainTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<Swapchain>);
}

TEST(SwapchainTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<Swapchain>);
}