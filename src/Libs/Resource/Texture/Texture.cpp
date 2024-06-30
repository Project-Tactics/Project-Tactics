#include "Texture.h"

#include <Libs/Rendering/RenderCalls/RenderCalls.h>

namespace tactics::resource {

Texture::~Texture() { render::pipeline::deleteTextures(1, &rendererId); }

void Texture::bind(unsigned int slot) const {
	using namespace render::pipeline;
	activeTexture(slot);
	bindTexture<TextureType::Texture2D>(rendererId);
}

void Texture::unbind() const {
	using namespace render::pipeline;
	unbindTexture<TextureType::Texture2D>();
}

std::shared_ptr<Texture> Texture::createNullTexture() {
	auto texture = std::make_shared<Texture>("_nullTexture"_id);
	texture->rendererId = 0;
	return texture;
}

} // namespace tactics::resource
