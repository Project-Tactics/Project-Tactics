#include "Particles.h"

#include <Libs/Utility/Random.h>

namespace firebolt {

ParticleSystemData* data = nullptr;

void init(unsigned int maxParticleSize) {
	data = new ParticleSystemData();
	data->maxParticleSize = maxParticleSize;
	data->effects.reserve(50);
}

void shutdown() {
	delete data;
	data = nullptr;
}

void _resetEffectData(Effect& effect, EffectConfig& config) {
	effect.nextParticle = 0;
	effect.timeToNextEmit = config.emitRate;
	for (Particle& particle : effect.particles) {
		particle.life = 0;
	}
}

EffectId createEffect(const EffectConfig& config) {
	EffectId effectId = 0;
	Effect* particleEffect{};
	EffectConfig* particleConfig{};
	if (data->_freeParticleEffects.empty()) {
		effectId = static_cast<EffectId>(data->effects.size());
		particleEffect = &data->effects.emplace_back();
		particleConfig = &data->configs.emplace_back(config);
		particleEffect->particles.resize(data->maxParticleSize);
	} else {
		effectId = data->_freeParticleEffects.back();
		particleEffect = &data->effects[effectId];
		particleConfig = &data->configs[effectId];
		data->_freeParticleEffects.pop_back();
	}

	_resetEffectData(*particleEffect, *particleConfig);

	return effectId;
}

void update(float deltaTime) {
	unsigned int effectIndex = 0;
	for (auto& effect : data->effects) {
		auto& config = data->configs[effectIndex];
		for (Particle& particle : effect.particles) {
			if (particle.life > 0) {
				particle.position += particle.velocity;
				particle.velocity += particle.acceleration;
				particle.life -= deltaTime;

				// calculate normalized value of lifetime
				float lifetime = 1.0f - particle.life / config.lifetime;
				for (auto& modifier : config.modifiers) {
					std::visit([&](auto&& arg) { updateParticle(arg, particle, lifetime); }, modifier);
				}
			}
		}
	}

	for (auto& effect : data->effects) {
		effect.timeToNextEmit -= deltaTime;
		if (effect.timeToNextEmit <= 0) {
			emitParticle(effect, data->configs[effectIndex]);
			effect.timeToNextEmit = data->configs[effectIndex].emitRate;
		}
	}
}

void emitParticle(Effect& effect, const EffectConfig& config) {
	auto& particle = effect.particles[effect.nextParticle];

	auto [startPosition, direction] = std::visit(
		[&particle](auto&& arg) -> std::tuple<glm::vec3, glm::vec3> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, PointSpawnPosition>) {
				return {arg.position, tactics::Vector3::zero};
			} else if constexpr (std::is_same_v<T, CircleSpawnPosition>) {
				auto direction = tactics::Random::range3D(-1, 1);
				direction = glm::normalize(direction);
				// direction *= tactics::Random::range(0, arg.radius);
				return {{0, 0, 0}, direction};
			} else if constexpr (std::is_same_v<T, ConeSpawnPosition>) {
				float angle = tactics::Random::range(0.0f, glm::two_pi<float>());
				float radius = tactics::Random::range(0.0f, arg.radius);
				float height = tactics::Random::range(0.0f, arg.radius * glm::tan(arg.angle));
				return {arg.position + glm::vec3(radius * glm::cos(angle), height, radius * glm::sin(angle)),
						tactics::Vector3::zero};
			}
		},
		config.spawnPosition);

	particle.position = effect.position + startPosition;
	particle.velocity = direction * config.speed;
	particle.acceleration = glm::vec3(0, 0, 0);
	particle.life = config.lifetime;
	particle.size = config.size;
	particle.color = config.color;

	effect.nextParticle = (effect.nextParticle + 1) % effect.particles.size();
}

void destroyEffect(EffectId effectId) {
	data->_freeParticleEffects.push_back(effectId);
	_resetEffectData(data->effects[effectId], data->configs[effectId]);
}

Effect& getEffect(EffectId effectId) {
	return data->effects[effectId];
}

unsigned int getEffectsCount() {
	return static_cast<unsigned int>(data->effects.size());
}

} // namespace firebolt
