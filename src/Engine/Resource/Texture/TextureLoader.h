#pragma once

#include "Texture.h"

#include <string_view>
#include <tuple>

#include <glad/gl.h>

namespace tactics {

using TextureId = GLuint;

class TextureLoader {
public:
	static std::tuple<TextureId, TextureInfo> loadTexture(std::string_view filename);
	static void unloadTexture(TextureId textureId);
};

}
