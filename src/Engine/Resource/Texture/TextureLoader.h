#pragma once

#include "Texture.h"

#include <memory>
#include <string_view>
#include <tuple>

#include <glad/gl.h>

namespace tactics::resource {

using TextureId = GLuint;

class TextureLoader {
public:
	static std::unique_ptr<Texture> loadTexture(std::string_view name, std::string_view filename, bool useTransparency);
};

}
