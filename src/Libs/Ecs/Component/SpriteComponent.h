#pragma once

#include <Libs/Resource/Sprite/Sprite.h>
#include <Libs/Resource/Mesh/Mesh.h>

#include <memory>

namespace tactics::component {

struct Sprite {
	std::shared_ptr<resource::Sprite> sprite;
	std::shared_ptr<resource::Mesh> mesh;
	unsigned int spriteIndex;
};

struct SpriteAnimation {
	float duration;
	std::vector<unsigned int> spriteIndices;
};

}
