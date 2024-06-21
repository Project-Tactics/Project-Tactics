#pragma once

#include "SpriteSheet.h"
#include "../ResourceLoader.h"

#include <Libs/Utility/Json/MathJsonSerializer.h>
#include <glm/glm.hpp>

namespace tactics::resource {

struct SpriteSheetDescriptor {
	std::string texture;
	glm::vec2 spriteSize;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SpriteSheetDescriptor, texture, spriteSize)
};

class SpriteSheetLoader: public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<SpriteSheet> load(const SpriteSheetDescriptor& descriptor);
};

}
