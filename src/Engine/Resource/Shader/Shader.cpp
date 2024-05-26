#include "Shader.h"

#include <glad/gl.h>

namespace tactics::resource {

Shader::~Shader() {
	glDeleteProgram(rendererId);
}

}
