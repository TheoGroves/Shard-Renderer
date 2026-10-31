#include <gtest/gtest.h>
#include "../src/window/window.h"
#include <GLFW/glfw3.h>
#include <type_traits>

class WindowTest : public ::testing::Test {
protected:
    Window* window = nullptr;

    void SetUp() override {
        ASSERT_EQ(glfwInit(), GLFW_TRUE);

        window = new Window(1280, 720, "Shard Renderer");
    }

    void TearDown() override {
        delete window;
        glfwTerminate();
    }
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
    EXPECT_THROW(Window(0, 720, "Shard Renderer"), std::runtime_error);
}

TEST(WindowTests, ThrowOnZeroHeight) {
    EXPECT_THROW(Window(1280, 0, "Shard Renderer"), std::runtime_error);
}

TEST(WindowTests, CanCreateAndDestroyWindow) {
    ASSERT_EQ(glfwInit(), GLFW_TRUE);

    {
        Window window(1280, 720, "Shard Renderer");

        ASSERT_NE(window.get_handle(), nullptr);
    }

    glfwTerminate();

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