#include <gtest/gtest.h>

#include <Libs/TestLib/TestLibFile.h>

TEST(MyTest, Test1) {
	TestLibFile test;
	EXPECT_EQ(13, test.sum(7, 6));
}
