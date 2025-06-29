#include "DebugDrawingSystem.h"

#include "../Component/DebugDrawingComponent.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void DebugDrawingSystem::update(entt::registry& registry) {
	auto view = registry.view<DebugDrawingLifetime>();
	for (auto [entity, lifetime] : view.each()) {
		if (lifetime.lifetime <= 0.0f) {
			registry.destroy(entity);
		} else {
			lifetime.lifetime -= EngineTime::fixedDeltaTime<float>();
		}
	}
}

} // namespace tactics::component
