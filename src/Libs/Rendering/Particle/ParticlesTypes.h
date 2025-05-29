#pragma once

#include "ParticlesUpdater.h"

#include <glm/glm.hpp>
#include <limits>
#include <variant>

namespace firebolt {

using EffectId = unsigned int;
inline EffectId InvalidEffectId = std::numeric_limits<unsigned int>::max();

struct EffectConfig {
	std::vector<ParticleUpdateAction> updaters;
	EmitShape emitShape;
	Value startLifetime;
	Value startSpeed;
	Value startSize;
	Value startRotation;
	glm::vec4 color{1, 1, 1, 1};
	float emitRate{0};
};

struct Particle {
	glm::vec3 position{0, 0, 0};
	glm::vec3 velocity{0, 0, 0};
	glm::vec3 acceleration{0, 0, 0};
	float rotation{0};
	float life{0};
	float maxLife{0};
	float size{1};
	glm::vec4 color{1, 1, 1, 1};
};

struct Effect {
	std::vector<Particle> particles;
	unsigned int nextParticle{0};
	float timeToNextEmit{0};
	glm::vec3 position{0, 0, 0};
};

struct ParticleSystemData {
	std::vector<Effect> effects;
	std::vector<EffectConfig> configs;
	std::vector<unsigned int> _freeParticleEffects;
	unsigned int maxParticleSize{0};
};

} // namespace firebolt
