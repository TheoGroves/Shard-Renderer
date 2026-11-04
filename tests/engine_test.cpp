#include <gtest/gtest.h>

#include "../src/renderer/glfw_context.h"
#include "../src/renderer/engine.h"
#include <stdexcept>
#include <type_traits>

class EngineTest : public ::testing::Test {
protected:
    Engine engine = Engine(1280, 720);
};

TEST_F(EngineTest, ThrowOnZeroWidth) {
    EXPECT_THROW(Engine(0, 720), std::runtime_error);
}

TEST_F(EngineTest, ThrowOnZeroHeight) {
    EXPECT_THROW(Engine(1280, 0), std::runtime_error);
}

TEST(EngineTests, CanCreateAndDestroyEngine) {
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
    EXPECT_FALSE(engine.should_close());
}

TEST_F(EngineTest, CanBeginAndEndFrame) {
    EXPECT_NO_THROW(engine.begin_frame());
    EXPECT_NO_THROW(engine.end_frame());
}

TEST_F(EngineTest, ThrowOnOpeningFrameWhilePreviousFrameStillOpen) {
    EXPECT_NO_THROW(engine.begin_frame());
    EXPECT_THROW(engine.begin_frame(), std::runtime_error);
}

TEST_F(EngineTest, ThrowOnEndingFrameBeforeAnyFrameOpened) {
    EXPECT_THROW(engine.end_frame(), std::runtime_error);
}

TEST_F(EngineTest, ThrowOnEndingFrameTwice) {
    EXPECT_NO_THROW(engine.begin_frame());
    EXPECT_NO_THROW(engine.end_frame());
    EXPECT_THROW(engine.end_frame(), std::runtime_error);
}

TEST_F(EngineTest, CanBeginNewFrameAfterEndingPrevious) {
    engine.begin_frame();
    engine.end_frame();
    
    EXPECT_NO_THROW(engine.begin_frame());
    EXPECT_NO_THROW(engine.end_frame());
}