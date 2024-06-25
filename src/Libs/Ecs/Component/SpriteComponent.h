#pragma once

#include <Libs/Resource/SpriteSheet/SpriteSheet.h>
#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Resource/Material/Material.h>
#include <Libs/Utility/Reflection.h>

#include <memory>

namespace tactics::component {

struct Sprite {
	std::shared_ptr<resource::SpriteSheet> spriteSheet;
	unsigned int spriteIndex;
	glm::vec2 uvFlip = Vector2::one;

	void deserialize(const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData);
	static void defineReflection();
};

struct SpriteAnimation {
	float currentTime = 0.0f;
	hash_string currentAnimation = none_hash();

	void startAnimation(const hash_string& animation);

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SpriteAnimation, currentTime, currentAnimation);
	static void defineReflection();
};

}
