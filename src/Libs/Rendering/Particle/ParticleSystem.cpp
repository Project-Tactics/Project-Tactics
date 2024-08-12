#include "ParticleSystem.h"

#include <Libs/Ecs/Component/ParticleEmitterComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Resource/ParticleEffect/ParticleEffect.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics {

ParticleSystem::ParticleSystem(resource::ResourceProvider& resourceSystem, EntityComponentSystem& ecs)
	: _resourceSystem(resourceSystem)
	, _ecs(ecs) {
	firebolt::init(500);
}

ParticleSystem::~ParticleSystem() {
	firebolt::shutdown();
}

void ParticleSystem::update(entt::registry& registry) {
	_updateEffectsState(registry);
	_updateEffectsPosition(registry);
	firebolt::update(EngineTime::fixedDeltaTime<float>());
}

firebolt::EffectId ParticleSystem::createEffect(const HashId& resourceId) {
	return createEffect(_resourceSystem.getResource<resource::ParticleEffect>(resourceId));
}

firebolt::EffectId ParticleSystem::createEffect(resource::ParticleEffect::Ptr effectResource) {
	auto effectId = firebolt::createEffect(effectResource->config);
	_effects.push_back(effectId);
	return effectId;
}

void ParticleSystem::destroyEffect(firebolt::EffectId effectId) {
	firebolt::destroyEffect(effectId);
	std::erase_if(_effects, [effectId](firebolt::EffectId id) { return id == effectId; });
}

unsigned int ParticleSystem::getEffectsCount() const {
	return firebolt::getEffectsCount();
}

const firebolt::Effect& ParticleSystem::getEffectByIndex(unsigned int index) const {
	return firebolt::getEffect(_effects[index]);
}

const firebolt::Effect& ParticleSystem::getEffectById(firebolt::EffectId effectId) const {
	return firebolt::getEffect(effectId);
}

void ParticleSystem::_updateEffectsState(entt::registry& registry) {
	using namespace component;
	auto inactiveEmitters = registry.view<ParticleEmitter>(entt::exclude<ParticleEmitterPlaying>);
	for (auto&& [entity, emitter] : inactiveEmitters.each()) {
		if (emitter.isEmittingParticles) {
			if (!emitter.maybeEffectId.has_value()) {
				emitter.maybeEffectId = createEffect(emitter.effectResource);
				registry.emplace<ParticleEmitterPlaying>(entity);
			} else if (!emitter.maybeEffectId.value()) {
				registry.remove<ParticleEmitterPlaying>(entity);
			}
		}
	}

	auto activeEmitters = registry.view<ParticleEmitter, ParticleEmitterPlaying>();
	for (auto&& [entity, emitter] : activeEmitters.each()) {
		if (!emitter.isEmittingParticles) {
			destroyEffect(emitter.maybeEffectId.value());
			emitter.maybeEffectId = std::nullopt;
			registry.remove<ParticleEmitterPlaying>(entity);
		}
	}
}

void ParticleSystem::_updateEffectsPosition(entt::registry& registry) {
	using namespace component;
	auto view = registry.view<ParticleEmitter, Transform, ParticleEmitterPlaying>();
	for (auto&& [entity, emitter, transform] : view.each()) {
		auto& effect = firebolt::getEffect(*emitter.maybeEffectId);
		effect.position = transform.getPosition();
	}
}

} // namespace tactics
