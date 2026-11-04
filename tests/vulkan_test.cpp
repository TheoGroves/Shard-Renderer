#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/vk/vulkan_instance.h"
#include <type_traits>
#include <memory>

class VulkanInstanceTest : public ::testing::Test {
protected:
    GlfwContext context; 
    VulkanInstance instance;
};

TEST_F(VulkanInstanceTest, CreatesValidInstance) {
    ASSERT_NE(instance.get_handle(), VK_NULL_HANDLE);
}

TEST_F(VulkanInstanceTest, ReturnsConsistentHandle) {
    VkInstance first = instance.get_handle();
    VkInstance second = instance.get_handle();

    EXPECT_EQ(first, second);
}

TEST(VulkanInstanceTests, CanCreateAndDestroyInstance) {
    GlfwContext context; 

    {
        VulkanInstance instance;

        ASSERT_NE(
            instance.get_handle(),
            VK_NULL_HANDLE
        );
    }

    SUCCEED();
}

TEST(VulkanInstanceTests, IsNotCopyConstructible) {
    EXPECT_FALSE(
        std::is_copy_constructible_v<VulkanInstance>
    );
}

TEST(VulkanInstanceTests, IsNotCopyAssignable) {
    EXPECT_FALSE(
        std::is_copy_assignable_v<VulkanInstance>
    );
}