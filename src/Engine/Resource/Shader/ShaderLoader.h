#pragma once

#include <string>

#include <glad/gl.h>

namespace tactics::resource {

using ShaderId = GLuint;
using ShaderProgramId = GLuint;
using ShaderType = GLenum;

class ShaderLoader {
public:
	static ShaderProgramId loadProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

private:
	static ShaderId _loadShader(ShaderType shaderType, const std::string& shaderCode);
};

}
