#include "SpriteSheet.h"

namespace tactics::resource {

glm::vec2 SpriteSheet::getUVCoordinates(uint32_t spriteIndex) const {
	auto spritesPerRow = std::lround(texture->info.width / (int)spriteSize.x);
	auto row = spriteIndex / spritesPerRow;
	auto column = spriteIndex % spritesPerRow;
	return glm::vec2((column * spriteSize.x) / texture->info.width, (row * spriteSize.y) / texture->info.height);
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
