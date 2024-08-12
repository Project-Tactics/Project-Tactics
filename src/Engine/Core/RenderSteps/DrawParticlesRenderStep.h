#pragma once

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/RenderStep/RenderStep.h>
#include <Libs/Rendering/VertexAttributes.h>
#include <Libs/Rendering/VertexBuffer.h>

namespace tactics {

class EntityComponentSystem;
class ParticleSystem;

class DrawParticles : public RenderStep {
public:
	DrawParticles(EntityComponentSystem& ecs, ParticleSystem& particleSystem);
	void execute(RenderStepInfo& renderInfo) override;

private:
	EntityComponentSystem& _ecs;
	ParticleSystem& _particleSystem;
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	std::unique_ptr<VertexAttributes> _vertexAttributes;
};

} // namespace tactics
