#include "MeshComponent.h"

#include <Libs/Ecs/Entity.h>
#include <Libs/Resource/ResourceProvider.h>
#include <Libs/Utility/Uniforms/UniformsDescriptor.h>

namespace tactics::component {

Mesh Mesh::clone() {
	Mesh clone;
	clone.mesh = mesh;
	for (auto& materialInstance : materials) {
		clone.materials.push_back(materialInstance->clone());
	}
	return clone;
}

} // namespace tactics::component
