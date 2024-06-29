#include "SpriteSheetLoader.h"

namespace tactics::resource {

std::shared_ptr<SpriteSheet> SpriteSheetLoader::load(const SpriteSheetDescriptor& descriptor) {
	auto spriteSheet = std::make_shared<SpriteSheet>();
	spriteSheet->texture = _getResource<Texture>(hash(descriptor.texture));
	spriteSheet->spriteSize = descriptor.spriteSize;

	for (auto& [name, animation] : descriptor.animations) {
		spriteSheet->animations.insert({hash(name).value(), animation});
	}

	return spriteSheet;
}

}
