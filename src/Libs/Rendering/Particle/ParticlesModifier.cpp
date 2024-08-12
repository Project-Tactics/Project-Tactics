#include "ParticlesModifier.h"

#include "Particles.h"

namespace firebolt {

void updateParticle(ColorOverLifetime& modifier, Particle& particle, float lifetime) {
	particle.color = glm::mix(modifier.startColor, modifier.endColor, lifetime);
}

void updateParticle(SizeOverLifetime& modifier, Particle& particle, float lifetime) {
	particle.size = glm::mix(modifier.startSize, modifier.endSize, lifetime);
}

} // namespace firebolt
