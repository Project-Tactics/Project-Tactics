#include "Texture.h"

#include <glad/gl.h>

namespace tactics::resource {

Texture::~Texture() {
	glDeleteTextures(1, &rendererId);
}

void Texture::bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, rendererId);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
