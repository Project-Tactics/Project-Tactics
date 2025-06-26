#pragma once

namespace jab {

struct Edges {
	Edges() = default;

	Edges(float all) : top(all), right(all), bottom(all), left(all) {}

	Edges(float vertical, float horizontal) : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}

	Edges(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}

	float top{};
	float right{};
	float bottom{};
	float left{};
};

struct Size {
	Size() = default;

	Size(float width, float height) : width(width), height(height) {}

	float width{};
	float height{};
};

struct Bounds {
	Bounds() = default;

	Bounds(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

	float x{};
	float y{};
	float width{};
	float height{};
};

struct Position {
	float x{};
	float y{};
};

enum class FlexDirection {
	Row,
	Column
};

enum class JustifyContent {
	Start,
	Center,
	End,
	SpaceBetween,
	SpaceAround
};

enum class AlignItems {
	Start,
	Center,
	End,
	Stretch
};

} // namespace jab
