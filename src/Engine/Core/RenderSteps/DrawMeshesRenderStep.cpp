#include "DrawMeshesRenderStep.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/AlphaBlendedComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
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

	if (_alphaBlendedFlag == AlphaBlendedFlag::WithoutAlphaBlend) {
		_ecs.view<Camera, CurrentCamera>().each([this] (auto& camera) {
			auto viewProjectionMatrix = camera.projection * camera.view;
			_drawOpaqueGeometry(viewProjectionMatrix);
		});
	} else {
		_ecs.view<Camera, Transform, CurrentCamera>().each([this] (auto& camera, auto& transform) {
			auto viewProjectionMatrix = camera.projection * camera.view;
			_drawAlphaBlendedGeometry(viewProjectionMatrix, transform);
		});
	}

	// Unbind any shader
	glUseProgram(0);
}

void DrawMeshes::_drawOpaqueGeometry(const glm::mat4x4& viewProjection) {
	using namespace component;

	glDisable(GL_BLEND);

	auto view = _ecs.view<Transform, Mesh>(entt::exclude<AlphaBlended>);
	for (auto&& [entity, transform, mesh] : view.each()) {
		_drawMesh(viewProjection, transform, mesh);
	}
}

void DrawMeshes::_drawAlphaBlendedGeometry(const glm::mat4x4& viewProjection, component::Transform& cameraTransform) {
	using namespace component;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	_ecs.sort<AlphaBlended>([this, &cameraTransform] (const entt::entity lhs, const entt::entity rhs) {
		auto diff1 = _ecs.get<Transform>(lhs).getPosition() - cameraTransform.getPosition();
		auto diff2 = _ecs.get<Transform>(rhs).getPosition() - cameraTransform.getPosition();
		return glm::length2(diff1) > glm::length2(diff2);
	});
	auto view2 = _ecs.view<Transform, Mesh, AlphaBlended>();
	for (auto&& [entity, transform, mesh] : view2.each()) {
		_drawMesh(viewProjection, transform, mesh);
	}
}

void DrawMeshes::_drawMesh(const glm::mat4x4& viewProjection, component::Transform& transform, const component::Mesh& inMesh) {
	auto& material = inMesh.material;
	auto& mesh = inMesh.mesh;
	auto& shader = material->parent->shader;

	mesh->vertexAttributes->bind();

	// TODO(Gerark) Optimization: only bind the shader if it's different from the last one
	shader->bind();
	material->updateShaderUniforms();
	glm::mat4 mvp = viewProjection * transform.computeMatrix();
	shader->setUniform("u_ModelViewProjection", mvp);
	_drawGeometry(*mesh);
}

void DrawMeshes::_drawGeometry(const resource::Mesh& mesh) {
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
