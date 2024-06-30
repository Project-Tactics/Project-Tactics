#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tactics {

class Vector3 {
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

class Vector2 {
public:
	static const glm::vec2 zero;
	static const glm::vec2 one;
	static const glm::vec2 up;
	static const glm::vec2 down;
	static const glm::vec2 left;
	static const glm::vec2 right;
};

class Math {
public:
	static const float PI;
};

class Quaternion {
public:
	static const glm::quat identity;
};

class Matrix4x4 {
public:
	static const glm::mat4x4 identity;
};

struct Rect {
	union {
		struct {
			glm::vec2 position;
			glm::vec2 size;
		} rect;

		struct {
			glm::vec2 min;
			glm::vec2 max;
		} bounds;
	};
};

} // namespace tactics
