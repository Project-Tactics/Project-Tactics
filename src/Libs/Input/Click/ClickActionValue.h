#pragma once

namespace click {

struct Axis3D {
	float x{};
	float y{};
	float z{};
};

struct Axis2D {
	float x{};
	float y{};
};

union ActionValue {
	float axis1D;
	Axis2D axis2D;
	Axis3D axis3D{};
};

} // namespace click
