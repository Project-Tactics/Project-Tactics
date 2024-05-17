#pragma once

#include <string>

#include <glad/gl.h>

namespace tactics {

using ShaderId = GLuint;
using ShaderProgramId = GLuint;
using ShaderType = GLenum;

class ShaderLoader {
public:
	static ShaderProgramId loadProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
	static ShaderId loadShader(ShaderType shaderType, const std::string& shaderCode);
};

}
