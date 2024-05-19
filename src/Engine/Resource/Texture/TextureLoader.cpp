#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace tactics {

std::tuple<TextureId, TextureInfo> TextureLoader::loadTexture(std::string_view filename) {
	auto info = TextureInfo();
	unsigned char* textureData = stbi_load(filename.data(), &info.width, &info.height, &info.channelsCount, 0);
	GLuint textureId;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
	return {textureId, info};
}

void TextureLoader::unloadTexture(TextureId textureId) {
	glDeleteTextures(1, &textureId);
}

}
