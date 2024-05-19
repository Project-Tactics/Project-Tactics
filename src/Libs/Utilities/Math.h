#pragma once

#include <glm/glm.hpp>

namespace tactics {

class Vector {
public:
	static const glm::vec3 zero;
	static const glm::vec3 one;
	static const glm::vec3 up;
	static const glm::vec3 down;
	static const glm::vec3 left;
	static const glm::vec3 right;
	static const glm::vec3 forward;
	static const glm::vec3 backward;
};

class Math {
public:
	static const float PI;
};

}
