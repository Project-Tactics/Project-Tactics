#include "DemoPhysicsState.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Utility/Random.h>

namespace tactics {

DemoPhysicsState::DemoPhysicsState(ServiceLocator& serviceLocator) : SampleState(serviceLocator) {}

FsmAction DemoPhysicsState::enter() {
	auto& inputSystem = getService<InputSystem>();
	inputSystem.assignInputMap("PhysicsDemo");

	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("Camera"_id, "defaultCamera"_id);
	sceneSystem.createEntity("Plane"_id, "defaultPlane"_id);

	for (auto i = 0; i < 100; ++i) {
		auto entity = sceneSystem.createEntity(HashId("Cube_" + std::to_string(i)), "defaultCube"_id);
		entity.updateComponent<component::Transform>([i](component::Transform& transform) {
			transform.setPosition(Random::range3D(-10.f, 10.f) + glm::vec3{0, 10, 0});
			transform.setRotation(Random::random3D() * 90.f);
		});

		auto& mesh = entity.getComponent<component::Mesh>();
		mesh.materials[0]->set("u_Color", glm::vec4(Random::random3D(), 1.0f));
	}

	return FsmAction::none();
}

void DemoPhysicsState::exit() {
	auto& inputSystem = getService<InputSystem>();
	inputSystem.unassignInputMap("PhysicsDemo");
}

FsmAction DemoPhysicsState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	if (inputSystem.checkAction("resetRigidBodies")) {
		auto& sceneSystem = getService<SceneSystem>();
		for (auto i = 0; i < 100; ++i) {
			auto entity = sceneSystem.getEntityByName(HashId("Cube_" + std::to_string(i)));
			entity.updateComponent<component::Transform>([i](component::Transform& transform) {
				transform.setPosition(Random::range3D(-5.f, 5.f) + glm::vec3{0, 10, 0});
				transform.setRotation(Random::random3D() * 90.f);
			});
		}
	}

	return FsmAction::none();
}

} // namespace tactics
