#include "ShaderLoader.h"

#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Utility/Exception.h>

#include <filesystem>
#include <fstream>
#include <glad/gl.h>

namespace tactics::resource {

std::shared_ptr<Shader> ShaderLoader::load(const ShaderDescriptor& descriptor) {
	auto shader = std::make_shared<Shader>();
	auto [shaderProgramId, vertexSource, fragmentSource] =
		_loadProgram(descriptor.vertexShader, descriptor.fragmentShader);
	shader->rendererId = shaderProgramId;
	shader->vertexSource = vertexSource;
	shader->fragmentSource = fragmentSource;
	return shader;
}

std::tuple<unsigned int, std::string, std::string> ShaderLoader::_loadProgram(const std::string& vertexShader,
																			  const std::string& fragmentShader) {
	auto programId = glCreateProgram();

	auto [vertexShaderId, vertexSource] = _loadShader(GL_VERTEX_SHADER, vertexShader);
	auto [fragmentShaderId, fragmentSource] = _loadShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// Cleanup intermediate shaders
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return {programId, vertexSource, fragmentSource};
}

std::tuple<unsigned int, std::string> ShaderLoader::_loadShader(unsigned int shaderType,
																const std::string& shaderContent) {
	std::filesystem::path shaderPath = shaderContent;
	std::string shaderCode = shaderContent;
	if (shaderPath.extension() == ".vert" || shaderPath.extension() == ".frag") {
		auto fileHandle = _getFileSystem().createStringFileHandle(shaderContent);
		fileHandle->load();
		shaderCode = fileHandle->getContent();
	}
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
		TACTICS_EXCEPTION("Shader Compiler error: {}", buffer.data());
	}

	return {shaderId, shaderCode};
}

} // namespace tactics::resource
