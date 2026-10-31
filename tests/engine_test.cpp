#include <gtest/gtest.h>
#include "../src/renderer/engine.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <type_traits>

class EngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_EQ(glfwInit(), GLFW_TRUE);
    }

    void TearDown() override {
        glfwTerminate();
    }
};

TEST_F(EngineTest, ThrowOnZeroWidth) {
    EXPECT_THROW(Engine(0, 720), std::runtime_error);
}

TEST_F(EngineTest, ThrowOnZeroHeight) {
    EXPECT_THROW(Engine(1280, 0), std::runtime_error);
}

TEST_F(EngineTest, ThrowOnNegativeWidth) {
    EXPECT_THROW(Engine(-1, 720), std::runtime_error);
}

TEST_F(EngineTest, ThrowOnNegativeHeight) {
    EXPECT_THROW(Engine(1280, -1), std::runtime_error);
}

TEST_F(EngineTest, CanCreateAndDestroyEngine) {
    {
        Engine engine(1280, 720);

        ASSERT_NE(
            engine.get_vk_handle(),
            VK_NULL_HANDLE
        );

        ASSERT_NE(
            engine.get_window_handle(),
            nullptr
        );
    }
}

TEST(EngineTests, IsNotCopyConstructible) {
    EXPECT_FALSE(
        std::is_copy_constructible_v<Engine>
    );
}

TEST(EngineTests, IsNotCopyAssignable) {
    EXPECT_FALSE(
        std::is_copy_assignable_v<Engine>
    );
}

TEST_F(EngineTest, ShouldNotCloseOnCreation) {
    Engine engine(1280, 720);
    EXPECT_FALSE(engine.should_close());
}

TEST_F(EngineTest, CanBeginAndEndFrame) {
    Engine engine(1280, 720);
    EXPECT_NO_THROW(engine.begin_frame());
    EXPECT_NO_THROW(engine.end_frame());
}