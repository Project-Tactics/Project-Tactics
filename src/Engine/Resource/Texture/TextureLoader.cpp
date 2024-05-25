#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace tactics::resource {

std::unique_ptr<Texture> TextureLoader::loadTexture(std::string_view name, std::string_view filename, bool useTransparency) {
	auto texture = std::make_unique<Texture>(name);
	texture->info.useTransparency = useTransparency;
	auto& info = texture->info;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* textureData = stbi_load(filename.data(), &info.width, &info.height, &info.channelsCount, 0);

	glGenTextures(1, &texture->rendererId);
	glBindTexture(GL_TEXTURE_2D, texture->rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, info.width, info.height, 0, info.channelsCount > 3 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return texture;
}

void TextureLoader::unloadTexture(TextureId textureId) {
	glDeleteTextures(1, &textureId);
}

}
