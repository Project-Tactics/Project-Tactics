#include "DrawMeshesRenderStep.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.inl>
#include <glm/gtc/matrix_transform.hpp>

#include "../Camera.h"

#include <Engine/ECS/EcsSystem.h>
#include <Engine/ECS/Components/MaterialComponent.h>
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

	using namespace components;
	auto& registry = _ecsSystem.getRegistry();

	if (_alphaBlendedFlag == AlphaBlendedFlag::WithoutAlphaBlend) {
		glDisable(GL_BLEND);
		auto view = registry.view<Transform, Mesh, Material>(entt::exclude<AlphaBlended>);
		for (auto&& [entity, transform, mesh, material] : view.each()) {
			_drawMesh(viewProjectionMatrix, transform, mesh, material);
		}
	} else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		registry.sort<AlphaBlended>([&registry, &renderInfo] (const entt::entity lhs, const entt::entity rhs) {
			auto diff1 = registry.get<Transform>(lhs).getPosition() - renderInfo.camera.getPosition();
			auto diff2 = registry.get<Transform>(rhs).getPosition() - renderInfo.camera.getPosition();
			return glm::length2(diff1) > glm::length2(diff2);
		});
		auto view2 = registry.view<Transform, Mesh, Material, AlphaBlended>();
		for (auto&& [entity, transform, mesh, material] : view2.each()) {
			_drawMesh(viewProjectionMatrix, transform, mesh, material);
		}
	}
}

void DrawMeshes::_drawMesh(const glm::mat4x4& viewProjection, components::Transform& transform, const components::Mesh& mesh, const components::Material& material) {
	mesh.mesh->vertexAttributes->bind();
	glUseProgram(material.shader->rendererId);
	glBindTexture(GL_TEXTURE_2D, material.texture->rendererId);
	glActiveTexture(GL_TEXTURE0);

	int mvpLocation = glGetUniformLocation(material.shader->rendererId, "u_ModelViewProjection");
	int texLocation = glGetUniformLocation(material.shader->rendererId, "u_Texture");

	glm::mat4 mvp = viewProjection * transform.computeMatrix();
	glUniform1i(texLocation, 0);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

	mesh.mesh->vertexBuffer->bind();

	if (mesh.mesh->indexBuffer->getSize() > 0) {
		mesh.mesh->indexBuffer->bind();
		glDrawElements(GL_TRIANGLES, mesh.mesh->indexBuffer->getSize(), GL_UNSIGNED_INT, nullptr);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh.mesh->vertexBuffer->getSize());
	}
	mesh.mesh->vertexAttributes->unbind();
	mesh.mesh->vertexBuffer->unbind();
}

}
