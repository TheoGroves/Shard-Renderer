#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include <stdexcept>
#include <type_traits>
#include <GLFW/glfw3.h>

class GlfwContextTest : public ::testing::Test {
protected:
    GlfwContext context;
};

TEST_F(GlfwContextTest, CreatesValidWindow) {
    GLFWwindow* window;
    ASSERT_NE(window = context.create_window(1280, 720, "GLFW Context Test"), nullptr);
    context.destroy_window(window);
}

TEST(GlfwContextTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<GlfwContext>);
}

TEST(GlfwContextTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<GlfwContext>);
}