#include <gtest/gtest.h>
#include "../src/renderer/vulkan_instance.h"
#include <type_traits>
#include <GLFW/glfw3.h>

class VulkanInstanceTest : public ::testing::Test {
protected:
    VulkanInstance* instance = nullptr;

    void SetUp() override {
        ASSERT_EQ(glfwInit(), GLFW_TRUE);

        instance = new VulkanInstance();
    }

    void TearDown() override {
        delete instance;
        glfwTerminate();
    }
};

TEST_F(VulkanInstanceTest, CreatesValidInstance) {
    ASSERT_NE(instance->get_handle(), VK_NULL_HANDLE);
}

TEST_F(VulkanInstanceTest, ReturnsConsistentHandle) {
    VkInstance first = instance->get_handle();
    VkInstance second = instance->get_handle();

    EXPECT_EQ(first, second);
}

TEST(VulkanInstanceTests, CanCreateAndDestroyInstance) {
    ASSERT_EQ(glfwInit(), GLFW_TRUE);

    {
        VulkanInstance instance;

        ASSERT_NE(
            instance.get_handle(),
            VK_NULL_HANDLE
        );
    }

    glfwTerminate();

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