#include "SpriteSheetLoader.h"

namespace tactics::resource {

std::shared_ptr<SpriteSheet> SpriteSheetLoader::load(const SpriteSheetDescriptor& descriptor) {
	auto spriteSheet = std::make_shared<SpriteSheet>();
	spriteSheet->texture = _getResource<Texture>(descriptor.texture);
	spriteSheet->spriteSize = descriptor.spriteSize;
	return spriteSheet;
}

}
