#pragma once

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

struct Frustum {
	float fov = 60.0f;
	float near = 0.1f;
	float far = 1000.0f;
	float aspectRatio = 1.0f;
	float orthoSize = 5.0f;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Frustum, fov, near, far, aspectRatio, orthoSize);
	static void defineReflection();
};

}
