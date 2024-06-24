#pragma once

#include "../Resource.h"
#include "../Texture/Texture.h"

#include <Libs/Utility/Math.h>

namespace tactics::resource {

class SpriteSheet: public Resource<SpriteSheet> {
public:
	using Resource<SpriteSheet>::Resource;
	static const ResourceType TYPE = ResourceType::SpriteSheet;

	glm::vec2 getUVCoordinates(uint32_t spriteIndex) const;
	glm::vec2 getUVSpriteSize() const;
	unsigned int getSpriteCount() const;

	std::shared_ptr<Texture> texture;
	glm::vec2 spriteSize;
};

}
