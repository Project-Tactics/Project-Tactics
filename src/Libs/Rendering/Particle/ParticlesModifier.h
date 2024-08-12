#pragma once

#include <glm/glm.hpp>
#include <variant>

namespace firebolt {

struct Particle;

struct ColorOverLifetime {
	glm::vec4 startColor{1, 1, 1, 1};
	glm::vec4 endColor{1, 1, 1, 1};
};

struct SizeOverLifetime {
	float startSize{1};
	float endSize{1};
};

using ParticleModifier = std::variant<ColorOverLifetime, SizeOverLifetime>;

void updateParticle(ColorOverLifetime& modifier, Particle& particle, float lifetime);
void updateParticle(SizeOverLifetime& modifier, Particle& particle, float lifetime);

} // namespace firebolt
