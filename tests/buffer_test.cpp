#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/vk/vulkan_instance.h"
#include "../src/renderer/vk/physical_device.h"
#include "../src/renderer/vk/logical_device.h"
#include "../src/renderer/vk/buffer.h"

#include <type_traits>
#include <memory>
#include <cstdint>
#include <array>

class BufferTest : public ::testing::Test {
protected:
    std::unique_ptr<Instance> instance;
    std::unique_ptr<LogicalDevice> device;
    std::unique_ptr<Buffer> buffer;

    void SetUp() override {
        instance = std::make_unique<Instance>();
        VkPhysicalDevice physical_device = instance->get_physical_device();

        device = std::make_unique<LogicalDevice>(physical_device);

        buffer = std::make_unique<Buffer>(
            *device,
            1024,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
    }
};

TEST_F(BufferTest, CreatesValidBuffer) {
    ASSERT_NE(buffer->get_handle(), VK_NULL_HANDLE);
}

TEST_F(BufferTest, ReturnsCorrectSize) {
    EXPECT_EQ(buffer->get_size(), 1024);
}

TEST_F(BufferTest, HasValidMemory) {
    EXPECT_NE(buffer->get_memory(), VK_NULL_HANDLE);
}

TEST_F(BufferTest, CanCopyData) {
    std::array<float, 4> data = {
        1.0f, 2.0f, 3.0f, 4.0f
    };

    EXPECT_NO_THROW(buffer->copy_data(data.data(), sizeof(data)));
}

TEST(BufferTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<Buffer>);
}

TEST(BufferTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<Buffer>);
}