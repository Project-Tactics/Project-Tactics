#include "DemoState.h"

#include <Libs/Rendering/GeometryBuilder.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Math.h>

#include <Engine/ECS/EcsSystem.h>
#include <Engine/ECS/Components/MeshComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/Camera.h>

#include <glm/glm.hpp>

namespace tactics {

FsmAction DemoState::enter() {
	using namespace components;
	_exitNextFrame = false;
	_exitNextFrameAlt = false;

	auto& ecs = getService<EcsSystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();

	auto plane = _createObject({0.0f, 0.0f, 0.0f}, "quad", "texturedUnlit");
	auto& planeTransform = ecs.get<Transform>(plane);
	planeTransform.setScale({200, 200, 200});
	planeTransform.setRotation(glm::radians(90.0f), Vector3::right);
	ecs.get<Mesh>(plane).material->set("u_Texture", resourceSystem.getResource<resource::Texture>("floor"));

	auto book = _createObject({0.0f, 0.0f, 0.0f}, "book", "texturedUnlit");
	auto& transform = ecs.get<Transform>(book);
	transform.setRotation(glm::radians(90.0f), Vector3::up);
	ecs.get<Mesh>(book).material->set("u_Texture", resourceSystem.getResource<resource::Texture>("colors"));

	auto crate = _createObject({40.0f, 5.0f, 0.0f}, "cube", "texturedUnlit");
	ecs.get<Transform>(crate).setScale({10, 10, 10});
	ecs.get<Mesh>(crate).material->set("u_Texture", resourceSystem.getResource<resource::Texture>("crate"));

	int width = 4;
	int height = 4;
	for (auto x = -width / 2; x < width / 2; ++x) {
		for (auto y = -height / 2; y < height / 2; ++y) {
			auto quad = _createObject({-40.0f + y * 20.f, 10.0f, x * 10.f}, "quad", "texturedUnlit");
			ecs.get<Transform>(quad).setScale({15, 15, 15});
			ecs.patch<Mesh>(quad, [&resourceSystem] (auto& mesh) {
				mesh.material->set("u_Texture", resourceSystem.getResource<resource::Texture>("tacticsIcon"));
			});
		}
	}

	// Example of how to create a custom resource programmatically ( in this case a mesh )
	// with a custom material/shader and add it to the resource system
	resourceSystem.createManualPack("CustomPack");
	auto geometryBuilder = GeometryBuilder({{3}, {2}});
	geometryBuilder.addVertex({-10.f, -10.f, 0.0f, 0, 0});
	geometryBuilder.addVertex({10.f, -10.f, 0.0f, 1, 0});
	geometryBuilder.addVertex({10, 10.0f, 0.0f, 1, 1});
	geometryBuilder.addVertex({-10, 10.0f, 0.0f, 0, 1});
	geometryBuilder.addIndices({0, 1, 2});
	geometryBuilder.addIndices({2, 3, 0});
	auto triangleMesh = geometryBuilder.build<resource::Mesh>("customQuadMesh");
	resourceSystem.loadExternalResource("CustomPack", triangleMesh);

	// We can also create a resource by simulating the usual pack loading
	nlohmann::json descriptor = {
		{"vertexShader", "shaders/main.vert"},
		{"fragmentShader", R"(
				#version 330 core
				layout(location = 0) out vec4 color;
				uniform vec4 u_Color;
				void main()
				{
					color = u_Color;
				}
			)"
	}
	};
	resourceSystem.loadExternalResource<resource::Shader>("CustomPack", "CustomShader", descriptor);

	auto material = std::make_shared<resource::Material>("colorOnly");
	material->shader = resourceSystem.getResource<resource::Shader>("CustomShader");
	material->set("u_Color", {0.204f, 0.608f, 0.922f, 1.0f});
	resourceSystem.loadExternalResource("CustomPack", material);

	// Now I can use the triangle mesh by applying the custom material with a specialized fragment shader
	_customQuadEntity = _createObject({0.0f, 40.0f, 0.0f}, "customQuadMesh", "colorOnly");

	return FsmAction::none();
}

void DemoState::exit() {
	using namespace tactics::components;

	auto& ecsSystem = getService<EcsSystem>();
	ecsSystem.view<Mesh, Transform>().each([&ecsSystem] (auto entity, auto&, auto&) {
		ecsSystem.destroy(entity);
	});

	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack("CustomPack");
	resourceSystem.unloadPack("mainPackage");
}

FsmAction DemoState::update() {
	if (_exitNextFrame) {
		return FsmAction::transition("exit");
	} else if (_exitNextFrameAlt) {
		return FsmAction::transition("alt");
	}

	auto& ecs = getService<EcsSystem>();
	ecs.get<components::Transform>(_customQuadEntity).rotate(glm::radians(1.0f), Vector3::forward);

	_rotateCamera();

	return FsmAction::none();
}

bool DemoState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_Scancode::SDL_SCANCODE_ESCAPE) {
		_exitNextFrame = true;
		return true;
	} else if (event.keysym.scancode == SDL_Scancode::SDL_SCANCODE_SPACE) {
		_exitNextFrameAlt = true;
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

entt::entity DemoState::_createObject(const glm::vec3& position, std::string_view meshName, std::string_view materialName) {
	auto& ecs = getService<EcsSystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();

	auto entity = ecs.createMeshEntity(
		position,
		Quaternion::identity,
		Vector3::one,
		resourceSystem.getResource<resource::Mesh>(meshName),
		resourceSystem.getResource<resource::Material>(materialName)
	);

	return entity;
}

}
