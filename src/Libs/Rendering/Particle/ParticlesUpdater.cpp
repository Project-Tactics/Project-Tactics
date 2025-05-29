#include "ParticlesUpdater.h"

#include <Libs/Utility/Random.h>

#include "Particles.h"

namespace firebolt {

/*
* Updaters
*/
void updateParticle(const ColorOverLifetime& modifier, Particle& particle, float lifetime) {
	particle.color = glm::mix(modifier.startColor, modifier.endColor, lifetime);
}

void updateParticle(const SizeOverLifetime& modifier, Particle& particle, float lifetime) {
	particle.size = glm::mix(modifier.startSize, modifier.endSize, lifetime);
}

/*
* Value
*/

float getConfigValue(const Value& value) {
	return std::visit([&](auto&& arg) { return getConfigValue(arg); }, value);
}

float getConfigValue(const FixedValue& fixedValue) {
	return fixedValue.value;
}

float getConfigValue(const RandomRange& randomRange) {
	return tactics::Random::range(randomRange.min, randomRange.max);
}

/*
* Spawn Position
*/

std::tuple<glm::vec3, glm::vec3> getEmitShapeInfo(const EmitShape& shape) {
	return std::visit([](auto&& arg) { return getEmitShapeInfo(arg); }, shape);
}

std::tuple<glm::vec3, glm::vec3> getEmitShapeInfo(const SphereEmitShape& sphere) {
	auto direction = tactics::Random::range3D(-1, 1);
	direction = glm::normalize(direction);
	auto distanceFromCenter = tactics::Random::range(0, sphere.radius);
	return {sphere.position + direction * distanceFromCenter, direction};
}

std::tuple<glm::vec3, glm::vec3> getEmitShapeInfo(const ConeEmitShape& cone) {
	auto randomPointOnCircle = tactics::Random::randomPointOnCircle(cone.radius);
	auto randomPoint = cone.position + glm::vec3{randomPointOnCircle.x, 1, randomPointOnCircle.y};
	auto direction = randomPoint - cone.position;
	direction = glm::normalize(direction);
	return {cone.position, direction};
}

} // namespace firebolt
