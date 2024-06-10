#pragma once

#include "Sprite.h"
#include "../ResourceLoader.h"

namespace tactics::resource {

struct SpriteDescriptor {
	std::string texture;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SpriteDescriptor, texture)
};

class SpriteLoader: public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<Sprite> load(const SpriteDescriptor& descriptor);
};

}
