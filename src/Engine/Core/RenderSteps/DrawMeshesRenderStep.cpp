#include "DrawMeshesRenderStep.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/AlphaBlendedComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/SpriteComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Rendering/VertexAttributes.h>

#include <glad/gl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.inl>
#include <glm/gtc/matrix_transform.hpp>

namespace tactics::renderstep {

DrawMeshes::DrawMeshes(EntityComponentSystem& ecs, AlphaBlendedFlag alphaBlendedFlag)
	: _ecs(ecs)
	, _alphaBlendedFlag(alphaBlendedFlag) {
}

void DrawMeshes::execute(RenderStepInfo&) {
	using namespace component;

	auto& registry = _ecs.sceneRegistry();

	if (_alphaBlendedFlag == AlphaBlendedFlag::WithoutAlphaBlend) {
		registry.view<Camera, CurrentCamera>().each([this] (auto& camera) {
			auto viewProjectionMatrix = camera.projection * camera.view;
			_drawOpaqueGeometry(viewProjectionMatrix);
		});
	} else {
		registry.view<Camera, Transform, CurrentCamera>().each([this] (auto& camera, auto& transform) {
			auto viewProjectionMatrix = camera.projection * camera.view;
			_drawAlphaBlendedGeometry(viewProjectionMatrix, transform);
			_drawSprite(viewProjectionMatrix);
		});
	}

	// Unbind any shader
	glUseProgram(0);
}

void DrawMeshes::_drawOpaqueGeometry(const glm::mat4x4& viewProjection) {
	using namespace component;

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	auto view = _ecs.sceneRegistry().view<Transform, Mesh>(entt::exclude<FullyAlphaBlended>);
	for (auto&& [entity, transform, mesh] : view.each()) {
		_drawMesh(viewProjection, transform, mesh, false);
	}
}

/*
* TODO(Gerark) The way how Sprites are rendered is very inefficient. We're changing states for each of them
* No batching is currently involved and that should be one of the first steps to follow once we're ready to scale this up
*/
void DrawMeshes::_drawSprite(const glm::mat4x4& viewProjection) {
	using namespace component;

	auto view = _ecs.sceneRegistry().view<Transform, Sprite>();
	for (auto&& [entity, transform, sprite] : view.each()) {
		auto& subMesh = sprite.mesh->subMeshes[0];
		auto& material = sprite.material;
		auto& shader = material->parent->shader;

		subMesh.vertexAttributes->bind();

		shader->bind();
		material->updateShaderUniforms();

		glm::mat4 mvp = viewProjection * transform.getMatrix();
		shader->setUniform("u_ModelViewProjection", mvp);
		_drawGeometry(subMesh);
	}
}

void DrawMeshes::_drawAlphaBlendedGeometry(const glm::mat4x4& viewProjection, component::Transform& cameraTransform) {
	using namespace component;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	auto& registry = _ecs.sceneRegistry();
	registry.sort<AlphaBlended>([&registry, &cameraTransform] (const entt::entity lhs, const entt::entity rhs) {
		auto diff1 = registry.get<Transform>(lhs).getPosition() - cameraTransform.getPosition();
		auto diff2 = registry.get<Transform>(rhs).getPosition() - cameraTransform.getPosition();
		return glm::length2(diff1) > glm::length2(diff2);
	});
	auto view = registry.view<Transform, Mesh, AlphaBlended>();
	for (auto&& [entity, transform, mesh] : view.each()) {
		_drawMesh(viewProjection, transform, mesh, true);
	}
}

void DrawMeshes::_drawMesh(const glm::mat4x4& viewProjection, component::Transform& transform, const component::Mesh& inMesh, bool filterTransparent) {
	auto& materials = inMesh.materials;
	auto& mesh = inMesh.mesh;

	unsigned int materialIndex = 0;
	for (auto& subMesh : mesh->subMeshes) {
		auto& material = materials[materialIndex];
		if (filterTransparent != material->parent->hasAlphaBlend) {
			++materialIndex;
			continue;
		}

		auto& shader = material->parent->shader;

		subMesh.vertexAttributes->bind();

		// TODO(Gerark) Optimization: Group Shader/Materials instead of constantly binding and unbinding
		shader->bind();
		material->updateShaderUniforms();

		// TODO(Gerark) Need to add way more than just the ModelViewProjection matrix as standard uniform but we should
		// parse the shader to check what are the standard uniforms it's requiring
		glm::mat4 mvp = viewProjection * transform.getMatrix();
		shader->setUniform("u_ModelViewProjection", mvp);

		_drawGeometry(subMesh);

		++materialIndex;
	}
}

void DrawMeshes::_drawGeometry(const resource::SubMesh& mesh) {
	mesh.vertexBuffer->bind();
	if (mesh.indexBuffer->getSize() > 0) {
		mesh.indexBuffer->bind();
		glDrawElements(GL_TRIANGLES, mesh.indexBuffer->getSize(), GL_UNSIGNED_INT, nullptr);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertexBuffer->getSize());
	}
	mesh.vertexAttributes->unbind();
	mesh.vertexBuffer->unbind();
}

}
