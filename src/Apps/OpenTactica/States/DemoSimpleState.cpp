#include "DemoSimpleState.h"

#include "../Component/RotateItem.h"
#include "../Component/RotateAroundPoint.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Rendering/GeometryBuilder.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Math.h>

#include <glm/glm.hpp>

namespace tactics {

FsmAction DemoSimpleState::enter() {
	_createPlane();
	_createTeapot();
	_createCrate();
	_createQuads();
	_createExtraRotatingQuads();
	_createCustomQuadWithCustomResources();
	return FsmAction::none();
}

void DemoSimpleState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();

	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack("CustomPack");
}

FsmAction DemoSimpleState::update() {
	auto& ecs = getService<EntityComponentSystem>();
	component::RotateItemSystem::update(ecs.sceneRegistry().view<component::Transform, component::RotateItem>());
	component::RotateAroundPointSystem::update(ecs.sceneRegistry().view<component::Transform, component::RotateAroundPoint>());

	return FsmAction::none();
}

FsmEventAction DemoSimpleState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_Scancode::SDL_SCANCODE_ESCAPE) {
		return FsmEventAction::transition("exit");
	} else if (event.keysym.scancode == SDL_Scancode::SDL_SCANCODE_SPACE) {
		return FsmEventAction::transition("empty");
	}

	return FsmEventAction::none();
}

void DemoSimpleState::_createCrate() {
	auto& sceneSystem = getService<SceneSystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();

	auto crate = sceneSystem.createEntity({40.0f, 5.0f, 0.0f}, "cube", {"texturedUnlit"});
	crate.getComponent<component::Transform>().setScale({10, 10, 10});
	crate.updateComponent<component::Mesh>([&resourceSystem] (auto& mesh) {
		mesh.materials[0]->set("u_Texture", resourceSystem.getResource<resource::Texture>(hash("crate")));
	});
}

void DemoSimpleState::_createTeapot() {
	auto& sceneSystem = getService<SceneSystem>();

	auto teapot = sceneSystem.createEntity({0.0f, 0.0f, 0.0f}, "teapot", {"coloredUnlit"});
	auto& transform = teapot.getComponent<component::Transform>();
	transform.setRotation(glm::radians(90.0f), Vector3::up);
	transform.setScale({5, 5, 5});
	teapot.updateComponent<component::Mesh>([] (auto& mesh) {
		mesh.materials[0]->set("u_Color", Color::gray);
	});
}

void DemoSimpleState::_createPlane() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("plane", "plane");
}

void DemoSimpleState::_createQuads() {
	auto& sceneSystem = getService<SceneSystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();

	const int width = 4;
	const int height = 4;
	for (auto x = -width / 2; x < width / 2; ++x) {
		for (auto y = -height / 2; y < height / 2; ++y) {
			auto quad = sceneSystem.createEntity({-50.0f + y * 20.f, 10.0f, x * 10.f}, "quad", {"texturedUnlitWithAlpha"});
			quad.getComponent<component::Transform>().setScale({15, 15, 15});
			quad.getComponent<component::Mesh>().materials[0]->set("u_Texture",
				resourceSystem.getResource<resource::Texture>(hash("tacticsIcon")));
		}
	}
}

void DemoSimpleState::_createExtraRotatingQuads() {
	auto& sceneSystem = getService<SceneSystem>();

	const int width = 4;
	const int height = 4;
	const int distance = 3;
	glm::vec3 offset = {40, 25, 0};
	for (auto x = -width / 2; x < width / 2; ++x) {
		for (auto y = -height / 2; y < height / 2; ++y) {
			auto quad = sceneSystem.createEntity("rotatingQuad", "simpleRotatingQuad");
			auto position = glm::vec3{x * distance, y * distance, 0.0f};
			quad.getComponent<component::Transform>().setPosition(offset + position);
		}
	}

	offset = {52, 25, 0};
	for (auto x = -width / 2; x < width / 2; ++x) {
		for (auto y = -height / 2; y < height / 2; ++y) {
			auto quad = sceneSystem.createEntity("rotatingQuad", "simpleRotatingQuad");
			auto position = glm::vec3{x * distance, y * distance, 0.0f};
			quad.getComponent<component::Transform>().setPosition(offset + position);
			quad.getComponent<component::RotateItem>().axis = Vector3::up;
		}
	}

	offset = {28, 25, 0};
	for (auto x = -width / 2; x < width / 2; ++x) {
		for (auto y = -height / 2; y < height / 2; ++y) {
			auto quad = sceneSystem.createEntity("rotatingQuad", "simpleRotatingQuad");
			auto position = glm::vec3{x * distance, y * distance, 0.0f};
			quad.getComponent<component::Transform>().setPosition(offset + position);
			quad.getComponent<component::RotateItem>().axis = Vector3::forward;
		}
	}
}

void DemoSimpleState::_createCustomQuadWithCustomResources() {
	auto& sceneSystem = getService<SceneSystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();

	// Example of how to create a custom resource programmatically ( in this case a mesh )
	// with a custom material/shader and add it to the resource system
	resourceSystem.createManualPack("CustomPack");
	auto geometryBuilder = GeometryBuilder({{3}, {2}});
	geometryBuilder.beginSubMesh();
	geometryBuilder.addVertex({-10.f, -10.f, 0.0f, 0, 0});
	geometryBuilder.addVertex({10.f, -10.f, 0.0f, 1, 0});
	geometryBuilder.addVertex({10, 10.0f, 0.0f, 1, 1});
	geometryBuilder.addVertex({-10, 10.0f, 0.0f, 0, 1});
	geometryBuilder.addIndices({0, 1, 2});
	geometryBuilder.addIndices({2, 3, 0});
	geometryBuilder.endSubMesh();
	auto triangleMesh = geometryBuilder.build("customQuadMesh");
	resourceSystem.loadExternalResource("CustomPack", triangleMesh);

	// We can also create a resource by simulating the usual pack loading
	nlohmann::json descriptor = {
		{"vertexShader", "common/shaders/default.vert"},
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
	material->shader = resourceSystem.getResource<resource::Shader>(hash("CustomShader"));
	material->set("u_Color", {0.204f, 0.608f, 0.922f, 1.0f});
	resourceSystem.loadExternalResource("CustomPack", material);

	// Now I can use the quad mesh by applying the custom material with a specialized fragment shader
	auto customQuad = sceneSystem.createEntity({0.0f, 40.0f, 0.0f}, "customQuadMesh", {"colorOnly"});
	customQuad.addComponent<component::RotateItem>(5.f, Vector3::forward);
}

}
