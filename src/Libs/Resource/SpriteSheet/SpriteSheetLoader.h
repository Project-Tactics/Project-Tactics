#pragma once

#include "../ResourceLoader.h"
#include "SpriteSheet.h"

#include <Libs/Utility/Json/JsonSerialization.h>

#include <glm/glm.hpp>

namespace tactics::resource {

struct SpriteSheetDescriptor {
	std::string texture;
	glm::vec2 spriteSize;
	std::unordered_map<std::string, SpriteSheetAnimation> animations;

	JSON_SERIALIZE(SpriteSheetDescriptor, texture, spriteSize, animations)
};

class SpriteSheetLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<SpriteSheet> load(const SpriteSheetDescriptor& descriptor);
};

} // namespace tactics::resource
