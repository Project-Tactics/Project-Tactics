#include "DrawParticlesRenderStep.h"

#include <Libs/Ecs/Component/ParticleEmitterComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Rendering/Particle/ParticleSystem.h>

#include <glad/gl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.inl>

namespace tactics {

DrawParticles::DrawParticles(EntityComponentSystem& ecs, ParticleSystem& particleSystem)
	: _ecs(ecs)
	, _particleSystem(particleSystem) {
	_vertexBuffer.setData(
		{
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		},
		rp::StaticDraw::value);
	_indexBuffer.setData({0, 1, 2, 2, 3, 0}, rp::StaticDraw::value);
	_vertexBuffer.bind();

	VertexAttributes::Builder builder;
	builder.attributef(3);
	builder.attributef(2);
	_vertexAttributes = builder.create();

	_vertexBuffer.unbind();
}

void DrawParticles::execute(RenderStepInfo& renderInfo) {
	using namespace component;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glDepthMask(GL_FALSE);

	auto& registry = _ecs.sceneRegistry();
	auto view = registry.view<Transform, ParticleEmitter, ParticleEmitterPlaying>();
	registry.sort<ParticleEmitterPlaying>([&registry, &renderInfo](const entt::entity lhs, const entt::entity rhs) {
		auto diff1 = registry.get<Transform>(lhs).getPosition() - renderInfo.cameraPosition;
		auto diff2 = registry.get<Transform>(rhs).getPosition() - renderInfo.cameraPosition;
		return glm::length2(diff1) > glm::length2(diff2);
	});

	for (auto&& [entity, transform, emitter] : view.each()) {
		auto& effect = _particleSystem.getEffectById(*emitter.maybeEffectId);

		_vertexAttributes->bind();
		_vertexBuffer.bind();
		_indexBuffer.bind();
		emitter.effectResource->texture->bind(0);

		auto& shader = emitter.effectResource->shader;
		shader->bind();
		for (auto i = effect.particles.size(); i > 0; --i) {
			auto& particle = effect.particles[i - 1];
			if (particle.life > 0) {
				auto particleTransform = glm::translate(glm::mat4(1.0f), particle.position);
				particleTransform = glm::scale(particleTransform, {particle.size, particle.size, particle.size});
				glm::mat4 mvp = renderInfo.viewProjection * particleTransform;
				shader->setUniform("u_ModelViewProjection", mvp);
				shader->setUniform("u_Color", particle.color);
				shader->setUniform("u_Texture", 0);

				glDrawElements(GL_TRIANGLES, _indexBuffer.getSize(), GL_UNSIGNED_INT, nullptr);
			}
		}

		_indexBuffer.unbind();
		_vertexBuffer.unbind();
		_vertexAttributes->unbind();
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

} // namespace tactics
