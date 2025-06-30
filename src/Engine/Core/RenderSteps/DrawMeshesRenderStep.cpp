#include "DrawMeshesRenderStep.h"

#include <Libs/Ecs/Component/AlphaBlendedComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/ParticleEmitterComponent.h>
#include <Libs/Ecs/Component/RenderableComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/Particle/ParticleSystem.h>
#include <Libs/Rendering/VertexAttributes.h>
#include <Libs/Rendering/VertexBuffer.h>

#include <glad/gl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.inl>

namespace tactics::renderstep {

DrawMeshes::DrawMeshes(EntityComponentSystem& ecs, ParticleSystem& particleSystem, AlphaBlendedFlag alphaBlendedFlag)
	: _ecs(ecs)
	, _particleSystem(particleSystem)
	, _alphaBlendedFlag(alphaBlendedFlag) {}

void DrawMeshes::execute(RenderStepInfo& info) {
	using namespace component;

	if (_alphaBlendedFlag == AlphaBlendedFlag::WithoutAlphaBlend) {
		_drawOpaqueGeometry(info);
	} else {
		_drawAlphaBlendedGeometry(info);
	}

	// Unbind any shader
	glUseProgram(0);
}

void DrawMeshes::_drawOpaqueGeometry(const RenderStepInfo& info) {
	using namespace component;

	glDepthMask(GL_TRUE);

	/*
	 * Opaque Geometry do not need to be sorted for now.
	 */
	auto view = _ecs.sceneRegistry().view<Transform, Mesh>(entt::exclude<FullyAlphaBlended>);
	for (auto&& [entity, transform, mesh] : view.each()) {
		_drawMesh(info, transform, mesh, false);
	}
}

void DrawMeshes::_drawAlphaBlendedGeometry(const RenderStepInfo& info) {
	using namespace component;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glDepthMask(GL_FALSE);

	/*
	 * Transparent Geometry need to be sorted back to front to be rendered properly.
	 * Sorting for alpha blended geometry is not trivial. Right now we're doing the most simple thing.
	 * We check distances from camera to the objects and sort them based on that.
	 */
	auto& registry = _ecs.sceneRegistry();
	registry.sort<AlphaBlended>([&registry, &info](const entt::entity lhs, const entt::entity rhs) {
		const auto& t1 = registry.get<Transform>(lhs);
		const auto& t2 = registry.get<Transform>(rhs);
		auto t1Lenght = glm::length2(t1.getPosition() - info.cameraPosition);
		auto t2Lenght = glm::length2(t2.getPosition() - info.cameraPosition);
		return t1Lenght > t2Lenght; // back to front
	});

	auto view = registry.view<AlphaBlended, Transform, Renderable>();
	for (auto&& [entity, transform, renderable] : view.each()) {
		switch (renderable.type) {
		case RenderType::Mesh: {
			auto& mesh = registry.get<Mesh>(entity);
			_drawMesh(info, transform, mesh, true);
			break;
		}
		case RenderType::Particle: {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			if (registry.all_of<ParticleEmitterPlaying>(entity)) {
				auto& emitter = registry.get<ParticleEmitter>(entity);
				_drawParticles(info, transform, emitter);
			}
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		}
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void DrawMeshes::_drawMesh(const RenderStepInfo& renderInfo,
						   component::Transform& transform,
						   const component::Mesh& inMesh,
						   bool filterTransparent) {
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

		subMesh.va().bind();

		// TODO(Gerark) Optimization: Group Shader/Materials instead of constantly binding and unbinding
		shader->bind();
		material->updateShaderUniforms();

		// TODO(Gerark) Need to add way more than just the ModelViewProjection matrix and Resolution as standard uniform
		// but we should parse the shader to check what are the standard uniforms it's requiring
		glm::mat4 mvp = renderInfo.viewProjection * transform.getMatrix();
		shader->setUniform("u_ModelViewProjection", mvp);
		if (shader->hasUniform("u_Resolution")) {
			shader->setUniform("u_Resolution", renderInfo.windowSize);
		}

		_drawGeometry(subMesh);

		++materialIndex;
	}
}

void DrawMeshes::_drawGeometry(const resource::SubMesh& mesh) {
	mesh.vb().bind();
	if (mesh.ib().getSize() > 0) {
		mesh.ib().bind();
		glDrawElements(GL_TRIANGLES, mesh.ib().getSize(), GL_UNSIGNED_INT, nullptr);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh.vb().getSize());
	}
	mesh.va().unbind();
	mesh.vb().unbind();
}

void DrawMeshes::_drawParticles(const RenderStepInfo& info,
								component::Transform& transform,
								component::ParticleEmitter& emitter) {
	auto& effect = _particleSystem.getEffectById(*emitter.maybeEffectId);

	_quadBuffers.bind();
	emitter.effectResource->texture->bind(0);

	auto& shader = emitter.effectResource->shader;
	shader->bind();

	// Billboard rotation: inverse of the camera's rotation part
	glm::mat4 viewRotationOnly = glm::mat4(glm::mat3(info.view));
	glm::mat4 billboardMatrix = glm::inverse(viewRotationOnly);

	// TODO(Gerark) Performance - This a quick solution just to put something in motion. We should
	// batch/gpu-instance or use point based particles on the shader side instead of doing all of this in the
	// CPU.
	for (auto i = effect.particles.size(); i > 0; --i) {
		auto& particle = effect.particles[i - 1];
		if (particle.life > 0) {
			// Transform particle.position to world space using effect.transform
			glm::vec4 worldPos = transform.getMatrix() * glm::vec4(particle.position, 1.0f);

			// Particle rotation around Z (e.g., spinning effect)
			glm::mat4 rotationMatrix =
				glm::rotate(glm::mat4(1.0f), glm::radians(particle.rotation), glm::vec3(0.0f, 0.0f, 1.0f));

			// Build final transform matrix
			glm::mat4 transformMatrix =
				glm::translate(glm::mat4(1.0f), glm::vec3(worldPos)) * billboardMatrix * rotationMatrix;

			glm::vec3 scale{glm::length(glm::vec3(transform.getMatrix()[0])),
							glm::length(glm::vec3(transform.getMatrix()[1])),
							glm::length(glm::vec3(transform.getMatrix()[2]))};

			transformMatrix = glm::scale(transformMatrix, glm::vec3(particle.size) * scale);

			// TODO(Gerark) for now we always assume the pivot for particles is always centered on the texture
			transformMatrix = glm::translate(transformMatrix, glm::vec3(-0.5f, -0.5f, 0.0f));

			// Final MVP
			glm::mat4 mvp = info.viewProjection * transformMatrix;

			shader->setUniform("u_ModelViewProjection", mvp);
			shader->setUniform("u_Color", particle.color);
			shader->setUniform("u_Texture", 0);

			glDrawElements(GL_TRIANGLES, _quadBuffers._indexBuffer.getSize(), GL_UNSIGNED_INT, nullptr);
		}
	}

	_quadBuffers.unbind();
}

} // namespace tactics::renderstep
