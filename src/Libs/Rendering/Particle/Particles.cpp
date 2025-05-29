#include "Particles.h"

#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/Random.h>

#include <iostream>

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
	effect.timeToNextEmit = 1 / config.emitRate;
	for (Particle& particle : effect.particles) {
		particle.life = 0;
	}
}

bool isEffectValid(EffectId effectId) {
	return effectId < data->effects.size();
}

const EffectConfig& getConfig(EffectId effectId) {
	return data->configs[effectId];
}

void updateConfig(EffectId effectId, const EffectConfig& config) {
	data->configs[effectId] = config;
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

void _updateParticle(Particle& particle, const EffectConfig& config, float deltaTime) {
	particle.position += particle.velocity * deltaTime;
	particle.velocity += particle.acceleration * deltaTime;
	particle.life -= deltaTime;

	// calculate normalized value of lifetime
	float lifetime = 1.0f - particle.life / particle.maxLife;
	for (auto& updater : config.updaters) {
		std::visit([&](auto&& arg) { updateParticle(arg, particle, lifetime); }, updater);
	}
}

void update(float deltaTime) {
	for (auto effectIndex = 0; effectIndex < data->effects.size(); ++effectIndex) {
		auto& config = data->configs[effectIndex];
		auto& effect = data->effects[effectIndex];
		for (Particle& particle : effect.particles) {
			if (particle.life > 0) {
				_updateParticle(particle, config, deltaTime);
			}
		}
	}

	for (auto effectIndex = 0; effectIndex < data->effects.size(); ++effectIndex) {
		auto& effect = data->effects[effectIndex];
		effect.timeToNextEmit -= deltaTime;
		if (effect.timeToNextEmit <= 0) {
			do {
				emitParticle(effect, data->configs[effectIndex]);
				effect.timeToNextEmit += 1 / data->configs[effectIndex].emitRate;
			} while (effect.timeToNextEmit <= 0);
		}
	}
}

void emitParticle(Effect& effect, const EffectConfig& config) {
	auto& particle = effect.particles[effect.nextParticle];

	auto [startPosition, startDirection] = getEmitShapeInfo(config.emitShape);
	particle.life = particle.maxLife = getConfigValue(config.startLifetime);
	particle.size = getConfigValue(config.startSize);

	particle.rotation = getConfigValue(config.startRotation);
	particle.position = startPosition;
	particle.velocity = startDirection * getConfigValue(config.startSpeed);
	particle.acceleration = glm::vec3(0, 0, 0);
	particle.color = config.color;
	_updateParticle(particle, config, 0);

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
