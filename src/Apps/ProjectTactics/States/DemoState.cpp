#include "DemoState.h"

#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Overlay/ExampleOverlay.h>
#include <Libs/Overlay/MainOverlay.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Math.h>

#include <Engine/Core/Overlay/DebugOverlay.h>
#include <Engine/ECS/EcsSystem.h>
#include <Engine/ECS/Components/MaterialComponent.h>
#include <Engine/ECS/Components/MeshComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/Camera.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Shader/Shader.h>
#include <Engine/Resource/Texture/Texture.h>

#include <glm/glm.hpp>

namespace tactics {

FsmAction DemoState::enter() {
	_exitNextFrame = false;

	auto& overlaySystem = getService<OverlaySystem>();
	auto& renderSystem = getService<RenderSystem>();
	auto& ecsSystem = getService<EcsSystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();

	overlaySystem.addOverlay<MainOverlay>("Main", true, overlaySystem);
	overlaySystem.addOverlay<DebugOverlay>("Debug", false, renderSystem);
	overlaySystem.addOverlay<ExampleOverlay>("ImGui Demo", false);

	ecsSystem.createMeshEntity(
		{0.0f, 0.0f, 0.0f},
		glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		Vector3::one,
		resourceSystem.getResource<resource::Mesh>("book"),
		resourceSystem.getResource<resource::Shader>("main"),
		resourceSystem.getResource<resource::Texture>("colors")
	);

	ecsSystem.createMeshEntity(
		{-40.0f, 10.0f, 0.0f},
		Quaternion::identity,
		{15, 15, 15},
		resourceSystem.getResource<resource::Mesh>("quad"),
		resourceSystem.getResource<resource::Shader>("main"),
		resourceSystem.getResource<resource::Texture>("tactics-icon")
	);

	ecsSystem.createMeshEntity(
		{-40.0f, 10.0f, -10.0f},
		Quaternion::identity,
		{15, 15, 15},
		resourceSystem.getResource<resource::Mesh>("quad"),
		resourceSystem.getResource<resource::Shader>("main"),
		resourceSystem.getResource<resource::Texture>("tactics-icon")
	);

	ecsSystem.createMeshEntity(
		{-40.0f, 10.0f, 10.0f},
		Quaternion::identity,
		{15, 15, 15},
		resourceSystem.getResource<resource::Mesh>("quad"),
		resourceSystem.getResource<resource::Shader>("main"),
		resourceSystem.getResource<resource::Texture>("tactics-icon")
	);

	ecsSystem.createMeshEntity(
		{40.0f, 5.0f, 0.0f},
		Quaternion::identity,
		{10, 10, 10},
		resourceSystem.getResource<resource::Mesh>("cube"),
		resourceSystem.getResource<resource::Shader>("main"),
		resourceSystem.getResource<resource::Texture>("crate")
	);

	ecsSystem.createMeshEntity(
		{0.0f, 0.0f, 0.0f},
		glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		{200, 200, 200},
		resourceSystem.getResource<resource::Mesh>("quad"),
		resourceSystem.getResource<resource::Shader>("main"),
		resourceSystem.getResource<resource::Texture>("floor")
	);

	return FsmAction::none();
}

void DemoState::exit() {
	using namespace tactics::components;

	auto& ecsSystem = getService<EcsSystem>();
	ecsSystem.getRegistry().view<Mesh, Transform, Material>().each([&ecsSystem] (auto entity, auto&, auto&, auto&) {
		ecsSystem.getRegistry().destroy(entity);
	});

	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadResourcePack("mainPackage");

	auto& overlaySystem = getService<OverlaySystem>();
	overlaySystem.removeOverlay("Main");
	overlaySystem.removeOverlay("Debug");
	overlaySystem.removeOverlay("ImGui Demo");
}

FsmAction DemoState::update() {
	if (_exitNextFrame) {
		return FsmAction::transition("exit");
	}

	_rotateCamera();

	return FsmAction::none();
}

bool DemoState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_Scancode::SDL_SCANCODE_ESCAPE) {
		_exitNextFrame = true;
		return true;
	}

	return false;
}

void DemoState::_rotateCamera() {
	auto& renderSystem = getService<RenderSystem>();
	auto& camera = renderSystem.getCamera();
	_cameraAngleX += 0.005f;
	if (_cameraAngleX > 2 * Math::PI) {
		_cameraAngleX -= 2 * Math::PI;
	}

	float distance = 100;
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(distance * glm::cos(_cameraAngleX), 0.0f, distance * glm::sin(_cameraAngleX));

	camera.setPosition(position + glm::vec3(0, 10, 0));
	camera.setDirection(center - position);
}

}
