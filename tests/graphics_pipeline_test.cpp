#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/surface.h"
#include "../src/renderer/vk/swapchain.h"
#include "../src/renderer/vk/swapchain_image_views.h"
#include "../src/renderer/vk/graphics_pipeline.h"
#include "../src/renderer/window/window.h"

#include <type_traits>
#include <memory>

class GraphicsPipelineTest : public ::testing::Test {
protected:
    std::shared_ptr<GlfwContext> context = std::make_shared<GlfwContext>();
    std::unique_ptr<Window> window;
    std::unique_ptr<VulkanInstance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physical_device;
    std::unique_ptr<LogicalDevice> logical_device;
    std::unique_ptr<Swapchain> swapchain;
    std::unique_ptr<SwapchainImageViews> image_views;
    std::unique_ptr<GraphicsPipeline> graphics_pipeline;

    void SetUp() override {
        instance = std::make_unique<VulkanInstance>();
        window = std::make_unique<Window>(context, 1280, 720, "Graphics Pipeline Test");
        surface = std::make_unique<Surface>(instance->get_handle(), window->get_handle());
        physical_device = std::make_unique<PhysicalDevice>(instance->get_handle(), surface->get_handle());
        logical_device = std::make_unique<LogicalDevice>(*physical_device);
        swapchain = std::make_unique<Swapchain>(*physical_device, *logical_device, *surface, *window);
        image_views = std::make_unique<SwapchainImageViews>(*logical_device, *swapchain);
        graphics_pipeline = std::make_unique<GraphicsPipeline>(*logical_device, *swapchain, "shaders/triangle.vert.spv", "shaders/triangle.frag.spv");
    }
};

TEST_F(GraphicsPipelineTest, CreatesGraphicsPipeline) {
    ASSERT_NE(graphics_pipeline->get_handle(), VK_NULL_HANDLE);
}

TEST_F(GraphicsPipelineTest, DestroysGraphicsPipeline) {
    ASSERT_NE(graphics_pipeline->get_handle(), VK_NULL_HANDLE);

    graphics_pipeline.reset();

    SUCCEED();
}

TEST(GraphicsPipelineTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<GraphicsPipeline>);
}

TEST(GraphicsPipelineTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<GraphicsPipeline>);
}