#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/renderer/window/window.h"

#include <type_traits>
#include <memory>

class PhysicalDeviceTest : public ::testing::Test {
protected:
    std::shared_ptr<GlfwContext> context = std::make_shared<GlfwContext>();
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Window> window;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;

    void SetUp() override {
        instance = std::make_unique<VulkanInstance>();
        window = std::make_unique<Window>(context, 1280, 720, "Physical Device Test");
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
    }
};

TEST_F(PhysicalDeviceTest, CreatesValidLogicalDevice) {
    ASSERT_NE(physical_device->get_handle(), VK_NULL_HANDLE);
}

TEST_F(PhysicalDeviceTest, ReturnsConsistentHandle) {
    VkPhysicalDevice first = physical_device->get_handle();
    VkPhysicalDevice second = physical_device->get_handle();

    EXPECT_EQ(first, second);
}

TEST(PhysicalDeviceTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<PhysicalDevice>);
}

TEST(PhysicalDeviceTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<PhysicalDevice>);
}