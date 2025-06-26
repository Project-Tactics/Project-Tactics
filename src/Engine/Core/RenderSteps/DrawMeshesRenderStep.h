#pragma once

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/QuadBuffers.h>
#include <Libs/Rendering/RenderStep/RenderStep.h>
#include <Libs/Rendering/VertexAttributes.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Resource/Material/Material.h>

#include <glm/glm.hpp>
#include <memory>

namespace tactics {
namespace component {
struct Camera;
struct Mesh;
struct ParticleEmitter;
struct Transform;
} // namespace component

namespace resource {
class Mesh;
class SubMesh;
} // namespace resource

class EntityComponentSystem;
class ParticleSystem;

} // namespace tactics

namespace tactics::renderstep {

enum class AlphaBlendedFlag {
	WithoutAlphaBlend,
	WithAlphaBlend
};

class DrawMeshes : public RenderStep {
public:
	DrawMeshes(EntityComponentSystem& ecs, ParticleSystem& particleSystem, AlphaBlendedFlag drawAlphaBlendedMeshes);
	void execute(RenderStepInfo& renderInfo) override;

private:
	void _drawMesh(const glm::mat4x4& viewProjection,
				   component::Transform& transform,
				   const component::Mesh& mesh,
				   bool filterTransparent);
	void _drawGeometry(const resource::SubMesh& mesh);
	void _drawOpaqueGeometry(const RenderStepInfo& renderInfo);
	void _drawAlphaBlendedGeometry(const RenderStepInfo& renderInfo);
	void _drawParticles(const RenderStepInfo& renderStepInfo,
						component::Transform& transform,
						component::ParticleEmitter& emitter);

	EntityComponentSystem& _ecs;
	AlphaBlendedFlag _alphaBlendedFlag{};
	ParticleSystem& _particleSystem;

	/*
	 * Quad variables for Particle System
	 */
	QuadBuffers _quadBuffers;
};

} // namespace tactics::renderstep
