#pragma once

#include "../Resource.h"
#include "../Texture/Texture.h"

#include <Libs/Utility/HashId.h>
#include <Libs/Utility/Math.h>

#include <unordered_map>

namespace tactics::resource {

struct SpriteSheetAnimation {
	std::vector<uint32_t> frames;
	float duration;

	JSON_SERIALIZE(SpriteSheetAnimation, frames, duration)
};

class SpriteSheet : public Resource<SpriteSheet> {
public:
	using Resource<SpriteSheet>::Resource;
	static const ResourceType TYPE = ResourceType::SpriteSheet;

	glm::vec2 getUVCoordinates(uint32_t spriteIndex) const;
	glm::vec2 getUVSpriteSize() const;
	Rect getUVRect(uint32_t spriteIndex) const;
	unsigned int getSpriteCount() const;

	std::shared_ptr<Texture> texture;
	glm::vec2 spriteSize;
	std::unordered_map<HashId, SpriteSheetAnimation> animations;
};

} // namespace tactics::resource
