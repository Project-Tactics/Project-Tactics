#include <Libs/Utility/Time/FrameTimer.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace tactics;

class FrameTimerTest : public testing::Test {
protected:
	FrameTimerTest() { _frameTimer.setFixedDeltaTime(1.0f); }

	FrameTimer _frameTimer;
};

TEST_F(FrameTimerTest, ExpectOneTick) {
	_frameTimer.setFixedDeltaTime(1.0f);
	_frameTimer.reset(1.0f);
	_frameTimer.update(2.0f);
	EXPECT_EQ(_frameTimer.getTicks(), 1);
}

TEST_F(FrameTimerTest, ExpectTwoTicks) {
	_frameTimer.setFixedDeltaTime(1.0f);
	_frameTimer.reset(1.0f);
	_frameTimer.update(3.0f);
	EXPECT_EQ(_frameTimer.getTicks(), 2);
}

TEST_F(FrameTimerTest, ExpectTwoTicksAfterLeftOverTime) {
	_frameTimer.setFixedDeltaTime(1.0f);

	_frameTimer.reset(1.0f);

	_frameTimer.update(2.1f);
	while (!_frameTimer.hasConsumedAllTicks()) { _frameTimer.consumeTick(); }
	_frameTimer.update(3.0f);
	EXPECT_EQ(_frameTimer.getTicks(), 2);
}
