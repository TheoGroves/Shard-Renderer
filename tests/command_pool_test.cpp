#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/renderer/vk/command_pool.h"
#include "../src/renderer/window/window.h"

#include <type_traits>
#include <memory>

class CommandPoolTest : public ::testing::Test {
protected:
    std::shared_ptr<GlfwContext> context = std::make_shared<GlfwContext>();
    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;
    std::unique_ptr<CommandPool> command_pool;

    void SetUp() override {
        instance = std::make_unique<VulkanInstance>();
        window = std::make_unique<Window>(context, 1280, 720, "Image Views Test");
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
        logical_device = std::make_unique<LogicalDevice>(*physical_device);
        command_pool = std::make_unique<CommandPool>(*logical_device);
    }
};

TEST_F(CommandPoolTest, CreatesValidCommandPool) {
    ASSERT_NE(command_pool->get_handle(), VK_NULL_HANDLE);
}

TEST(CommandPoolTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<CommandPool>);
}

TEST(CommandPoolTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<CommandPool>);
}