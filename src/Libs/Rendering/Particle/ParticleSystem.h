#pragma once

#include "Particles.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Resource/ParticleEffect/ParticleEffect.h>
#include <Libs/Utility/HashId.h>

namespace tactics {
namespace resource {
class ResourceProvider;
}

class ParticleSystem {
public:
	ParticleSystem(resource::ResourceProvider& resourceSystem, EntityComponentSystem& ecs);
	~ParticleSystem();

	void update(entt::registry& registry);

	firebolt::EffectId createEffect(const HashId& resourceId);
	firebolt::EffectId createEffect(resource::ParticleEffect::Ptr effectResource);
	void destroyEffect(firebolt::EffectId effectId);

	unsigned int getEffectsCount() const;
	[[nodiscard]] const firebolt::Effect& getEffectByIndex(unsigned int index) const;
	[[nodiscard]] const firebolt::Effect& getEffectById(firebolt::EffectId effectId) const;

	void updateEffectConfig(firebolt::EffectId effectId, const firebolt::EffectConfig& config);
	const firebolt::EffectConfig& getEffectConfig(firebolt::EffectId effectId) const;

private:
	void _updateEffectsPosition(entt::registry& registry);
	void _updateEffectsState(entt::registry& registry);

	void _onEmitterCreated(entt::registry& registry, entt::entity entity);
	void _onEmitterDestroyed(entt::registry& registry, entt::entity entity);

	std::vector<firebolt::EffectId> _effects;
	resource::ResourceProvider& _resourceSystem;
	EntityComponentSystem& _ecs;
};

} // namespace tactics
