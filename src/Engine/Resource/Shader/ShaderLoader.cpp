#include "ShaderLoader.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Utility/Exception.h>

#include <glad/gl.h>
#include <fstream>

namespace tactics::resource {

std::shared_ptr<Shader> ShaderLoader::load(const ShaderDescriptor& descriptor) {
	auto shader = std::make_shared<Shader>(descriptor.name);
	shader->rendererId = _loadProgram(descriptor.vertexShaderPath, descriptor.fragmentShaderPath);
	return shader;
}

std::string ShaderLoader::_loadFile(const std::string& filePath) {
	auto path = _makeAbsolutePath(filePath);
	std::ifstream file(path);
	if (!file.is_open()) {
		throw Exception("Error while trying to open shader file. Could not open file: {}", path);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

unsigned int ShaderLoader::_loadProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	auto programId = glCreateProgram();

	auto vertexShaderId = _loadShader(GL_VERTEX_SHADER, vertexShaderFilePath);
	auto fragmentShaderId = _loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilePath);

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// Cleanup intermediate shaders
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

unsigned int ShaderLoader::_loadShader(unsigned int shaderType, const std::string& shaderFilePath) {
	auto shaderCode = _loadFile(shaderFilePath);
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
		throw Exception("Shader Compiler error: {}", buffer.data());
	}

	return shaderId;
}

}
