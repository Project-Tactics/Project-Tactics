#pragma once

#include <glm/glm.hpp>
#include <variant>

namespace firebolt {

struct Particle;

struct RandomRange {
	float min{0};
	float max{0};
};

struct FixedValue {
	float value{0};
};

/*
 * Updaters
 */

struct ColorOverLifetime {
	glm::vec4 startColor{1, 1, 1, 1};
	glm::vec4 endColor{1, 1, 1, 1};
};

struct SizeOverLifetime {
	float startSize{1};
	float endSize{1};
};

using ParticleUpdateAction = std::variant<ColorOverLifetime, SizeOverLifetime>;

void updateParticle(const ColorOverLifetime& modifier, Particle& particle, float lifetime);
void updateParticle(const SizeOverLifetime& modifier, Particle& particle, float lifetime);

/*
 * Value
 */

using Value = std::variant<FixedValue, RandomRange>;

float getConfigValue(const Value& value);
float getConfigValue(const FixedValue& fixedValue);
float getConfigValue(const RandomRange& randomRange);

/*
 * Spawn Position
 */

struct SphereEmitShape {
	glm::vec3 position{0, 0, 0};
	float radius{1};
};

struct ConeEmitShape {
	glm::vec3 position{0, 0, 0};
	float radius{20};
};

using EmitShape = std::variant<SphereEmitShape, ConeEmitShape>;

std::tuple<glm::vec3, glm::vec3> getEmitShapeInfo(const EmitShape& sphere);
std::tuple<glm::vec3, glm::vec3> getEmitShapeInfo(const SphereEmitShape& sphere);
std::tuple<glm::vec3, glm::vec3> getEmitShapeInfo(const ConeEmitShape& sphere);

} // namespace firebolt
