#include "SpriteLoader.h"

namespace tactics::resource {

std::shared_ptr<Sprite> SpriteLoader::load(const SpriteDescriptor& descriptor) {
	auto sprite = std::make_shared<Sprite>();
	sprite->texture = _getResource<Texture>(descriptor.texture);
	return sprite;
}

}
