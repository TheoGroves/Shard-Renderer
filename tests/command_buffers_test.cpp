#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/renderer/vk/command_pool.h"
#include "../src/renderer/vk/command_buffers.h"
#include "../src/renderer/window/window.h"

#include <type_traits>
#include <memory>
#include <cstdint>

class CommandBuffersTest : public ::testing::Test {
protected:
    std::shared_ptr<GlfwContext> context = std::make_shared<GlfwContext>();
    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;
    std::unique_ptr<CommandPool> command_pool;
    std::unique_ptr<CommandBuffers> command_buffers;

    void Initialize(uint32_t count) {
        instance = std::make_unique<VulkanInstance>();
        window = std::make_unique<Window>(context, 1280, 720, "Command Buffers Test");
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
        logical_device = std::make_unique<LogicalDevice>(*physical_device);
        command_pool = std::make_unique<CommandPool>(*logical_device);
        command_buffers = std::make_unique<CommandBuffers>(*logical_device, *command_pool, count);
    }
};

class SingleCommandBuffersTest : public CommandBuffersTest {
protected:
    void SetUp() override {
        Initialize(1);
    }
};

class MultipleCommandBuffersTest : public CommandBuffersTest {
protected:
    void SetUp() override {
        Initialize(2);
    }
};

TEST_F(SingleCommandBuffersTest, CreatesValidCommandBuffer) {
    ASSERT_FALSE(command_buffers->get_buffers().empty());
}

TEST_F(MultipleCommandBuffersTest, CreatesMultipleValidCommandBuffers) {
    EXPECT_EQ(command_buffers->get_buffers().size(), 2);
}

TEST_F(MultipleCommandBuffersTest, CreatesMultipleDifferentCommandBuffers) {
    const auto& buffers = command_buffers->get_buffers();

    ASSERT_EQ(buffers.size(), 2);
    EXPECT_NE(buffers[0], buffers[1]);
}

TEST(CommandBuffersTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<CommandBuffers>);
}

TEST(CommandBuffersTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<CommandBuffers>);
}