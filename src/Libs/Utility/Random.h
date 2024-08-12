#pragma once

#include <Libs/Utility/Math.h>

#include <glm/glm.hpp>
#include <random>

namespace tactics {

class Random {
public:
	Random();
	static void setInstance(Random* instance);
	static float range(float min, float max);
	static float random();
	static glm::vec2 random2D();
	static glm::vec3 random3D();
	static glm::vec4 random4D();
	static glm::vec2 range2D(float min, float max);
	static glm::vec3 range3D(float min, float max);
	static glm::vec4 range4D(float min, float max);
	static glm::vec2 range2D(const glm::vec2& min, const glm::vec2& max);
	static glm::vec3 range3D(const glm::vec3& min, const glm::vec3& max);
	static glm::vec4 range4D(const glm::vec4& min, const glm::vec4& max);
	static glm::vec2 random2D(const glm::vec2& min, const glm::vec2& max);
	static glm::vec3 random3D(const glm::vec3& min, const glm::vec3& max);

private:
	std::mt19937 generator;
	static Random* _instance;
};
} // namespace tactics
