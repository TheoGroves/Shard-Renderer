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
    ASSERT_NO_THROW(window = context.create_window(1280, 720, "GLFW Context Test"));
    context.destroy_window(window);
}

TEST_F(GlfwContextTest, CreatesMultipleDistinctWindows) {
    GLFWwindow* first;
    GLFWwindow* second;
    ASSERT_NO_THROW(first = context.create_window(1280, 720, "GLFW Context Test Window 1"));
    ASSERT_NO_THROW(second = context.create_window(1280, 720, "GLFW Context Test Window 2"));

    ASSERT_NE(first, second);

    context.destroy_window(second);
    context.destroy_window(first);
}

TEST(GlfwContextTests, IsNotCopyConstructible) {
    EXPECT_FALSE(std::is_copy_constructible_v<GlfwContext>);
}

TEST(GlfwContextTests, IsNotCopyAssignable) {
    EXPECT_FALSE(std::is_copy_assignable_v<GlfwContext>);
}