#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/renderer/vk/swapchain.h"
#include "../src/renderer/vk/synchronization.h"
#include "../src/renderer/window/window.h"

#include <type_traits>
#include <memory>

class SynchronizationTest : public ::testing::Test {
protected:
    std::shared_ptr<GlfwContext> context = std::make_shared<GlfwContext>();
    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;
    std::unique_ptr<Swapchain> swapchain;
    std::unique_ptr<Synchronization> synchronization;

    void SetUp() override {
        instance = std::make_unique<VulkanInstance>();
        window = std::make_unique<Window>(context, 1280, 720, "Command Pool Test");
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
        logical_device = std::make_unique<LogicalDevice>(*physical_device);
        swapchain = std::make_unique<Swapchain>(*physical_device, *logical_device, *surface, *window);
        synchronization = std::make_unique<Synchronization>(*logical_device, static_cast<uint32_t>(swapchain->get_images().size()));
    }
};

TEST_F(SynchronizationTest, CreatesValidInFlightFence) {
    ASSERT_NE(synchronization->get_in_flight_fence(), VK_NULL_HANDLE);
}

TEST_F(SynchronizationTest, CreatesValidImageAvailableSemaphore) {
    ASSERT_NE(synchronization->get_image_available_semaphore(), VK_NULL_HANDLE);
}

TEST_F(SynchronizationTest, CreatesValidRenderFinishedSemaphore) {
    ASSERT_NE(synchronization->get_render_finished_semaphore(0), VK_NULL_HANDLE);
}

TEST(SynchronizationTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<Synchronization>);
}

TEST(SynchronizationTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<Synchronization>);
}