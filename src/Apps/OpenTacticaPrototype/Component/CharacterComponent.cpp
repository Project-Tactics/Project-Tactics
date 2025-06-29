#include "CharacterComponent.h"

namespace tactics::component {

void BattleSystem::advanceTick(entt::registry& registry, float deltaTime) {
	std::array<entt::entity, 64> readyEntities;
	auto readyEntitiesIndex = 0;

	auto view = registry.view<ChargeTime>();
	for (auto [entity, state] : view.each()) {
		state.chargeTime += state.speed * deltaTime;
		if (state.chargeTime >= 100) {
			state.chargeTime = 0;
			readyEntities[readyEntitiesIndex++] = entity;
		}
	}

	for (auto i = 0; i < readyEntitiesIndex; ++i) {
		registry.emplace_or_replace<CharBattleReady>(readyEntities[i]);
	}
	readyEntitiesIndex = 0;
}

} // namespace tactics::component
