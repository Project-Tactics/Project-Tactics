#include "SpriteComponent.h"

#include <Libs/Resource/ResourceProvider.h>
#include <Libs/Utility/Json/MathJsonSerializer.h>

namespace tactics::component {

struct SpriteDescriptor {
	std::string spriteSheet;
	unsigned int spriteIndex{0};
	glm::vec2 uvFlip = Vector2::one;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SpriteDescriptor, spriteSheet, spriteIndex, uvFlip);
};

void Sprite::deserialize(const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData) {
	using namespace resource;
	SpriteDescriptor descriptor = jsonData;
	spriteSheet = resourceProvider->getResource<resource::SpriteSheet>(HashId(descriptor.spriteSheet));
	spriteIndex = descriptor.spriteIndex;
	uvFlip = descriptor.uvFlip;
}

void Sprite::defineReflection() {
	componentReflection<Sprite>("sprite");
}

void SpriteAnimation::defineReflection() {
	componentReflection<SpriteAnimation>("spriteAnimation").data<&SpriteAnimation::currentTime>("spriteIndices"_id);
}

void SpriteAnimation::startAnimation(const HashId& animation) {
	currentAnimation = animation;
	currentTime = 0.0f;
}

} // namespace tactics::component
