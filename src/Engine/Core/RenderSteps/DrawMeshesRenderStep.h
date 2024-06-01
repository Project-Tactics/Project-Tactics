#pragma once

#include <Libs/Rendering/RenderStep/RenderStep.h>
#include <Libs/Resource/Material/Material.h>

#include <glm/glm.hpp>

#include <memory>

namespace tactics {
namespace component {
struct Camera;
struct Mesh;
struct Transform;
}

namespace resource {
class Mesh;
}

class EntityComponentSystem;
}

namespace tactics::renderstep {

enum class AlphaBlendedFlag {
	WithoutAlphaBlend,
	WithAlphaBlend
};

/**
* @brief The DrawMeshes class is a render step that draws all meshes in the scene.
* It uses the ECS system to find all entities with a Transform, Mesh and Material component.
* It also supports drawing alpha blended meshes. If the drawAlphaBlendedMeshes flag is set to WithAlphaBlend
* it will draw all meshes with an AlphaBlended component.
*/
class DrawMeshes: public RenderStep {
public:
	DrawMeshes(EntityComponentSystem& ecs, AlphaBlendedFlag drawAlphaBlendedMeshes);
	void execute(RenderStepInfo& renderInfo) override;

private:
	void _drawMesh(const glm::mat4x4& viewProjection, component::Transform& transform, const component::Mesh& mesh);
	void _drawGeometry(const resource::Mesh& mesh);
	void _drawOpaqueGeometry(const glm::mat4x4& viewProjection);
	void _drawAlphaBlendedGeometry(const glm::mat4x4& viewProjection, component::Transform& cameraTransform);

	EntityComponentSystem& _ecs;
	AlphaBlendedFlag _alphaBlendedFlag{};
};

}
