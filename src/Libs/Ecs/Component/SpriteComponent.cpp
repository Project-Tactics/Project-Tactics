#include "SpriteComponent.h"

#include <Libs/Resource/ResourceProvider.h>

namespace tactics::component {

struct SpriteDescriptor {
	std::string sprite;
	std::string mesh;
	std::string material;
	unsigned int spriteIndex;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SpriteDescriptor, sprite, mesh, material, spriteIndex);
};

void Sprite::deserialize(const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData) {
	using namespace resource;
	SpriteDescriptor descriptor = jsonData;
	sprite = resourceProvider->getResource<resource::Sprite>(descriptor.sprite);
	mesh = resourceProvider->getResource<Mesh>(descriptor.mesh);
	auto parentMaterial = resourceProvider->getResource<Material>(descriptor.material);
	material = Material::createInstance(parentMaterial);
	spriteIndex = descriptor.spriteIndex;
}

void Sprite::defineReflection() {
	componentReflection<Sprite>("sprite");
}

void SpriteAnimation::defineReflection() {
	componentReflection<SpriteAnimation>("spriteAnimation")
		.data<&SpriteAnimation::duration>(hash("duration"))
		.data<&SpriteAnimation::spriteIndices>(hash("spriteIndices"));
}

}
