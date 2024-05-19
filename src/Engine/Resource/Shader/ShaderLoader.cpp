#include "ShaderLoader.h"

#include <exception>
#include <format>
#include <vector>

namespace tactics {

ShaderProgramId ShaderLoader::loadProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) {
	auto programId = glCreateProgram();

	auto vertexShaderId = _loadShader(GL_VERTEX_SHADER, vertexShaderCode);
	auto fragmentShaderId = _loadShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// Cleanup intermediate shaders
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

void ShaderLoader::unloadProgram(ShaderProgramId programId) {
	glDeleteProgram(programId);
}

ShaderId ShaderLoader::_loadShader(ShaderType shaderType, const std::string& shaderCode) {
	char* code = const_cast<char*>(shaderCode.c_str());

	auto shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &code, nullptr);
	glCompileShader(shaderId);

	// Let's do some error handling
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> buffer(length);
		glGetShaderInfoLog(shaderId, length, &length, buffer.data());
		glDeleteShader(shaderId);
		throw std::exception(std::format("Shader Compiler error: {}", buffer.data()).c_str());
	}

	return shaderId;
}

}
