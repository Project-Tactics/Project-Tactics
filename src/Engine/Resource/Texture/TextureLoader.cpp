#include "TextureLoader.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Utility/Exception.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/gl.h>

namespace tactics::resource {

std::shared_ptr<Texture> TextureLoader::load(const TextureDescriptor& descriptor) {
	auto texture = _loadTexture(
		descriptor.name,
		_makeAbsolutePath(descriptor.path),
		descriptor.useTransparency);
	return texture;
}

std::shared_ptr<Texture> TextureLoader::_loadTexture(const std::string& name, const std::string& filename, bool useTransparency) {
	if (filename.empty()) {
		throw Exception("Can't load texture. Filename is empty while trying to load texture [{}]", name);
	}

	auto texture = std::make_shared<Texture>(name);
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


}
