#include <Libs/Utility/String/Math.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>

using namespace tactics;

TEST(StringTest, Vec2ToString) {
	auto str1 = toString(glm::vec2{10, 20});
	auto str2 = toString(glm::vec2{10.5, 20});
	auto str3 = toString(glm::vec2{10, 20.204});

	EXPECT_EQ(str1, "10, 20");
	EXPECT_EQ(str2, "10.5, 20");
	EXPECT_EQ(str3, "10, 20.204");
}

TEST(StringTest, Vec3ToString) {
	auto str1 = toString(glm::vec3{10, 20, 5000});
	auto str2 = toString(glm::vec3{10.5, 20, 5020});
	auto str3 = toString(glm::vec3{10, 20.204, 5010});

	EXPECT_EQ(str1, "10, 20, 5000");
	EXPECT_EQ(str2, "10.5, 20, 5020");
	EXPECT_EQ(str3, "10, 20.204, 5010");
}

TEST(StringTest, Vec4ToString) {
	auto str1 = toString(glm::vec4{10, 20, 5000, 0});
	auto str2 = toString(glm::vec4{10.5, 20, 5020, 10.000001});
	auto str3 = toString(glm::vec4{10, 20.204, 5010, 0});

	EXPECT_EQ(str1, "10, 20, 5000, 0");
	EXPECT_EQ(str2, "10.5, 20, 5020, 10.000001");
	EXPECT_EQ(str3, "10, 20.204, 5010, 0");
}

TEST(StringTest, Vec2FromString) {
	auto str1 = fromString<glm::vec2>("10, 20");
	auto str2 = fromString<glm::vec2>("10.5, 20");
	auto str3 = fromString<glm::vec2>("10, 20.204");

	EXPECT_EQ(str1, glm::vec2(10, 20));
	EXPECT_EQ(str2, glm::vec2(10.5, 20));
	EXPECT_EQ(str3, glm::vec2(10, 20.204));
}

TEST(StringTest, Vec3FromString) {
	auto str1 = fromString<glm::vec3>("10, 20, 5000");
	auto str2 = fromString<glm::vec3>("10.5, 20, 5020");
	auto str3 = fromString<glm::vec3>("10, 20.204, 5010");

	EXPECT_EQ(str1, glm::vec3(10, 20, 5000));
	EXPECT_EQ(str2, glm::vec3(10.5, 20, 5020));
	EXPECT_EQ(str3, glm::vec3(10, 20.204, 5010));
}

TEST(StringTest, Vec4FromString) {
	auto str1 = fromString<glm::vec4>("10, 20, 5000, 0");
	auto str2 = fromString<glm::vec4>("10.5, 20, 5020, 10.000001");
	auto str3 = fromString<glm::vec4>("10, 20.204, 5010, 0");

	EXPECT_EQ(str1, glm::vec4(10, 20, 5000, 0));
	EXPECT_EQ(str2, glm::vec4(10.5, 20, 5020, 10.000001));
	EXPECT_EQ(str3, glm::vec4(10, 20.204, 5010, 0));
}
