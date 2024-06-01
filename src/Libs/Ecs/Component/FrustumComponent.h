#pragma once

namespace tactics::component {

struct Frustum {
	float fov = 60.0f;
	float near = 0.1f;
	float far = 1000.0f;
	float aspectRatio = 1.0f;
};

}
