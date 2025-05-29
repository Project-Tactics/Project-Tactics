#include "ParticleSystem.h"

#include <Libs/Ecs/Component/ParticleEmitterComponent.h>
#include <Libs/Ecs/Component/RenderableComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Resource/ParticleEffect/ParticleEffect.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics {

ParticleSystem::ParticleSystem(resource::ResourceProvider& resourceSystem, EntityComponentSystem& ecs)
	: _resourceSystem(resourceSystem)
	, _ecs(ecs) {
	firebolt::init(2000);
	ecs.sceneRegistry().on_construct<component::ParticleEmitter>().connect<&ParticleSystem::_onEmitterCreated>(this);
	ecs.sceneRegistry().on_destroy<component::ParticleEmitter>().connect<&ParticleSystem::_onEmitterDestroyed>(this);
}

ParticleSystem::~ParticleSystem() {
	_ecs.sceneRegistry().on_construct<component::ParticleEmitter>().disconnect<&ParticleSystem::_onEmitterCreated>(
		this);
	_ecs.sceneRegistry().on_destroy<component::ParticleEmitter>().disconnect<&ParticleSystem::_onEmitterDestroyed>(
		this);
	firebolt::shutdown();
}

void ParticleSystem::update(entt::registry& registry) {
	_updateEffectsState(registry);
	_updateEffectsPosition(registry);
	firebolt::update(EngineTime::fixedDeltaTime<float>());
}

void ParticleSystem::_onEmitterCreated(entt::registry& registry, entt::entity entity) {
	auto& emitter = registry.get<component::ParticleEmitter>(entity);
	emitter.maybeEffectId = createEffect(emitter.effectResource);
	registry.emplace<component::Renderable>(entity, component::RenderType::Particle);
}

void ParticleSystem::_onEmitterDestroyed(entt::registry& registry, entt::entity entity) {
	auto& emitter = registry.get<component::ParticleEmitter>(entity);
	destroyEffect(*emitter.maybeEffectId);
	registry.remove<component::Renderable>(entity);
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
			registry.emplace<ParticleEmitterPlaying>(entity);
		}
	}

	auto activeEmitters = registry.view<ParticleEmitter, ParticleEmitterPlaying>();
	for (auto&& [entity, emitter] : activeEmitters.each()) {
		if (!emitter.isEmittingParticles) {
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

void ParticleSystem::updateEffectConfig(firebolt::EffectId effectId, const firebolt::EffectConfig& config) {
	firebolt::updateConfig(effectId, config);
}

const firebolt::EffectConfig& ParticleSystem::getEffectConfig(firebolt::EffectId effectId) const {
	return firebolt::getConfig(effectId);
}

} // namespace tactics
