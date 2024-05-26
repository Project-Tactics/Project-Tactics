#include "TextureManager.h"

#include <Libs/Resource/ResourcePathHelper.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/gl.h>

namespace tactics::resource {

struct TextureDescriptor {
	std::string name;
	std::string path;
	bool useTransparency = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TextureDescriptor, name, path, useTransparency);
};

ResourceId TextureManager::load(const nlohmann::json& descriptor) {
	auto textureDescriptor = descriptor.template get<TextureDescriptor>();
	auto texture = _loadTexture(
		textureDescriptor.name,
		_pathHelper.makeAbsolutePath(textureDescriptor.path),
		textureDescriptor.useTransparency);
	auto resourceId = texture->id;
	_registerResource(std::move(texture));
	return resourceId;
}

std::unique_ptr<Texture> TextureManager::_loadTexture(const std::string& name, const std::string& filename, bool useTransparency) {
	if (filename.empty()) {
		throw Exception("TextureManager::_loadTexture: filename is empty while trying to load texture [{}]", name);
	}

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


}
