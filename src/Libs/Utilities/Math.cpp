#include "Math.h"

#include <glm/gtc/constants.hpp>

namespace tactics {

const glm::vec3 Vector::zero = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 Vector::one = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 Vector::up = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Vector::down = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 Vector::left = glm::vec3(-1.0f, 0.0f, 0.0f);
const glm::vec3 Vector::right = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Vector::forward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Vector::backward = glm::vec3(0.0f, 0.0f, 1.0f);

const float Math::PI = glm::pi<float>();

}
