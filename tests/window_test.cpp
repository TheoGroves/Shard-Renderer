#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/window/window.h"
#include <type_traits>
#include <memory>

class WindowTest : public ::testing::Test {
protected:
    std::shared_ptr<GlfwContext> context = std::make_shared<GlfwContext>();
    std::unique_ptr<Window> window = std::make_unique<Window>(context, 1280, 720, "Window Test");
};

TEST_F(WindowTest, CreatesValidWindow) {
    ASSERT_NE(window->get_handle(), nullptr);
}

TEST_F(WindowTest, ReturnsConsistentWindow) {
    GLFWwindow* first = window->get_handle();
    GLFWwindow* second = window->get_handle();

    EXPECT_EQ(first, second);
}

TEST(WindowTests, ThrowOnZeroWidth) {
    auto context = std::make_shared<GlfwContext>();
    EXPECT_THROW(Window(context, 0, 720, "Window Test"), std::runtime_error);
}

TEST(WindowTests, ThrowOnZeroHeight) {
    auto context = std::make_shared<GlfwContext>();
    EXPECT_THROW(Window(context, 1280, 0, "Window Test"), std::runtime_error);
}

TEST(WindowTests, CanCreateAndDestroyWindow) {
    auto context = std::make_shared<GlfwContext>();
    {
        Window window(context, 1280, 720, "Window Test");

        ASSERT_NE(window.get_handle(), nullptr);
    }

    SUCCEED();
}

TEST(WindowTests, IsNotCopyConstructible) {
    EXPECT_FALSE(
        std::is_copy_constructible_v<Window>
    );
}

TEST(WindowTests, IsNotCopyAssignable) {
    EXPECT_FALSE(
        std::is_copy_assignable_v<Window>
    );
}