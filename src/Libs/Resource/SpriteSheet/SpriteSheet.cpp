#include "SpriteSheet.h"

namespace tactics::resource {

Rect SpriteSheet::getUVRect(uint32_t spriteIndex) const {
	auto coordinates = getUVCoordinates(spriteIndex);
	auto size = getUVSpriteSize();
	return {
		coordinates,
		{coordinates.x + size.x, coordinates.y - size.y}
	};
}

glm::vec2 SpriteSheet::getUVCoordinates(uint32_t spriteIndex) const {
	auto uvSpriteSize = getUVSpriteSize();
	auto spritePerRow = static_cast<int>(1.0f / uvSpriteSize.x);
	auto row = spriteIndex / spritePerRow;
	auto column = spriteIndex % spritePerRow;
	return glm::vec2(column * uvSpriteSize.x, 1 - (row * uvSpriteSize.y));
}

glm::vec2 SpriteSheet::getUVSpriteSize() const {
	return glm::vec2(spriteSize.x / texture->info.width, spriteSize.y / texture->info.height);
}

unsigned int SpriteSheet::getSpriteCount() const {
	auto spritesPerRow = std::lround(texture->info.width / (int)spriteSize.x);
	auto spritesPerColumn = std::lround(texture->info.height / (int)spriteSize.y);
	return spritesPerRow * spritesPerColumn;
}

}
