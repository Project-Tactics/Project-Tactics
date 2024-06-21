#pragma once

#include <Libs/Resource/SpriteSheet/SpriteSheet.h>
#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Resource/Material/Material.h>
#include <Libs/Utility/Reflection.h>

#include <memory>

namespace tactics::component {

struct Sprite {
	std::shared_ptr<resource::SpriteSheet> spriteSheet;
	std::shared_ptr<resource::Mesh> mesh;
	std::shared_ptr<resource::Material::Instance> material;
	unsigned int spriteIndex;

	void deserialize(const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData);
	static void defineReflection();
};

struct SpriteAnimation {
	float duration;
	int i{};
	std::vector<unsigned int> spriteIndices;

	static void defineReflection();
};

}
