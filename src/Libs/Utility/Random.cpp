#include "Random.h"

namespace tactics {

Random* Random::_instance = nullptr;

Random::Random() {
	std::random_device device;
	generator = std::mt19937(device());
}

void Random::setInstance(Random* instance) {
	_instance = instance;
}

float Random::range(float min, float max) {
	return min + (max - min) * random();
}

float Random::random() {
	using limits = std::numeric_limits<float>;
	auto distribution = std::uniform_real_distribution<float>(0, 1);
	return distribution(_instance->generator);
}

glm::vec2 Random::random2D() {
	return glm::vec2(random(), random());
}

glm::vec3 Random::random3D() {
	return glm::vec3(random(), random(), random());
}

glm::vec4 Random::random4D() {
	return glm::vec4(random(), random(), random(), random());
}

glm::vec2 Random::range2D(float min, float max) {
	return glm::vec2(range(min, max), range(min, max));
}

glm::vec3 Random::range3D(float min, float max) {
	return glm::vec3(range(min, max), range(min, max), range(min, max));
}

glm::vec4 Random::range4D(float min, float max) {
	return glm::vec4(range(min, max), range(min, max), range(min, max), range(min, max));
}

glm::vec2 Random::range2D(const glm::vec2& min, const glm::vec2& max) {
	return glm::vec2(range(min.x, max.x), range(min.y, max.y));
}

glm::vec3 Random::range3D(const glm::vec3& min, const glm::vec3& max) {
	return glm::vec3(range(min.x, max.x), range(min.y, max.y), range(min.z, max.z));
}

glm::vec4 Random::range4D(const glm::vec4& min, const glm::vec4& max) {
	return glm::vec4(range(min.x, max.x), range(min.y, max.y), range(min.z, max.z), range(min.w, max.w));
}

glm::vec2 Random::random2D(const glm::vec2& min, const glm::vec2& max) {
	return glm::vec2(range(min.x, max.x), range(min.y, max.y));
}

glm::vec3 Random::random3D(const glm::vec3& min, const glm::vec3& max) {
	return glm::vec3(range(min.x, max.x), range(min.y, max.y), range(min.z, max.z));
}

} // namespace tactics
