#include "Math.h"

#include <glm/gtc/constants.hpp>

namespace tactics {

using namespace glm;

const vec3 Vector3::zero = vec3(0.0f, 0.0f, 0.0f);
const vec3 Vector3::one = vec3(1.0f, 1.0f, 1.0f);
const vec3 Vector3::up = vec3(0.0f, 1.0f, 0.0f);
const vec3 Vector3::down = vec3(0.0f, -1.0f, 0.0f);
const vec3 Vector3::left = vec3(-1.0f, 0.0f, 0.0f);
const vec3 Vector3::right = vec3(1.0f, 0.0f, 0.0f);
const vec3 Vector3::forward = vec3(0.0f, 0.0f, -1.0f);
const vec3 Vector3::backward = vec3(0.0f, 0.0f, 1.0f);

const vec2 Vector2::zero = vec2(0.0f, 0.0f);
const vec2 Vector2::one = vec2(1.0f, 1.0f);
const vec2 Vector2::up = vec2(0.0f, 1.0f);
const vec2 Vector2::down = vec2(0.0f, -1.0f);
const vec2 Vector2::left = vec2(-1.0f, 0.0f);
const vec2 Vector2::right = vec2(1.0f, 0.0f);

const float Math::PI = pi<float>();

const quat Quaternion::identity = quat(1.0f, 0.0f, 0.0f, 0.0f);

const mat4x4 Matrix4x4::identity = mat4(1.0f);

} // namespace tactics
