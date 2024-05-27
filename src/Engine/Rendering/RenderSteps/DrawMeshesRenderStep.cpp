#include "DrawMeshesRenderStep.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.inl>
#include <glm/gtc/matrix_transform.hpp>

#include "../Camera.h"

#include <Engine/ECS/EcsSystem.h>
#include <Engine/ECS/Components/MeshComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/AlphaBlendedComponent.h>

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Rendering/VertexAttributes.h>

namespace tactics::renderstep {

DrawMeshes::DrawMeshes(EcsSystem& ecsSystem, AlphaBlendedFlag alphaBlendedFlag)
	: _ecsSystem(ecsSystem)
	, _alphaBlendedFlag(alphaBlendedFlag) {
}

void DrawMeshes::execute(RenderStepInfo& renderInfo) {
	auto viewProjectionMatrix = renderInfo.camera.getProjection() * renderInfo.camera.getView();

	if (_alphaBlendedFlag == AlphaBlendedFlag::WithoutAlphaBlend) {
		_drawOpaqueGeometry(viewProjectionMatrix);
	} else {
		_drawAlphaBlendedGeometry(viewProjectionMatrix, renderInfo);
	}

	// Unbind any shader
	glUseProgram(0);
}

void DrawMeshes::_drawOpaqueGeometry(const glm::mat4x4& viewProjection) {
	using namespace components;

	glDisable(GL_BLEND);

	auto view = _ecsSystem.view<Transform, Mesh>(entt::exclude<AlphaBlended>);
	for (auto&& [entity, transform, mesh] : view.each()) {
		_drawMesh(viewProjection, transform, mesh);
	}
}

void DrawMeshes::_drawAlphaBlendedGeometry(const glm::mat4x4& viewProjection, RenderStepInfo& renderInfo) {
	using namespace components;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	_ecsSystem.sort<AlphaBlended>([this, &renderInfo] (const entt::entity lhs, const entt::entity rhs) {
		auto diff1 = _ecsSystem.get<Transform>(lhs).getPosition() - renderInfo.camera.getPosition();
		auto diff2 = _ecsSystem.get<Transform>(rhs).getPosition() - renderInfo.camera.getPosition();
		return glm::length2(diff1) > glm::length2(diff2);
	});
	auto view2 = _ecsSystem.view<Transform, Mesh, AlphaBlended>();
	for (auto&& [entity, transform, mesh] : view2.each()) {
		_drawMesh(viewProjection, transform, mesh);
	}
}

void DrawMeshes::_drawMesh(const glm::mat4x4& viewProjection, components::Transform& transform, const components::Mesh& inMesh) {
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
