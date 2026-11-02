#include <gtest/gtest.h>

#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/window/window.h"

#include <GLFW/glfw3.h>
#include <type_traits>
#include <memory>

class LogicalDeviceTest : public ::testing::Test {
protected:
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Window> window;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;

    void SetUp() override {
        ASSERT_EQ(glfwInit(), GLFW_TRUE);

        window = std::make_unique<Window>(1280, 720, "Logical Device Test");

        instance = std::make_unique<VulkanInstance>();
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
        logical_device = std::make_unique<LogicalDevice>(*physical_device);
    }

    void TearDown() override {
        logical_device.reset();
        physical_device.reset();
        surface.reset();
        instance.reset();
        window.reset();

        glfwTerminate();
    }
};

TEST_F(LogicalDeviceTest, CreatesValidLogicalDevice) {
    ASSERT_NE(logical_device->get_handle(), VK_NULL_HANDLE);
}

TEST_F(LogicalDeviceTest, RetrievesValidGraphicsQueue) {
    ASSERT_NE(logical_device->get_graphics_queue(), VK_NULL_HANDLE);
}

TEST_F(LogicalDeviceTest, RetrievesValidPresentationQueue) {
    ASSERT_NE(logical_device->get_present_queue(), VK_NULL_HANDLE);
}

TEST(LogicalDeviceTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<LogicalDevice>);
}

TEST(LogicalDeviceTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<LogicalDevice>);
}