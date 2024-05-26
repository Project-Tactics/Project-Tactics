#include "Texture.h"

#include <glad/gl.h>

namespace tactics::resource {

Texture::~Texture() {
	glDeleteTextures(1, &rendererId);
}

}
