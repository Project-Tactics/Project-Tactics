#include "Shader.h"

#include <glad/gl.h>

namespace tactics::resource {

Shader::~Shader() {
	glDeleteProgram(rendererId);
}

void Shader::bind() const {
	glUseProgram(rendererId);
}

void Shader::setUniform(std::string_view uniformName, int value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	glUniform1i(uniformLocation, value);
}

void Shader::setUniform(std::string_view uniformName, float value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	glUniform1f(uniformLocation, value);
}

void Shader::setUniform(std::string_view uniformName, const glm::vec2& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	glUniform2f(uniformLocation, value.x, value.y);
}

void Shader::setUniform(std::string_view uniformName, const glm::vec3& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Shader::setUniform(std::string_view uniformName, const glm::vec4& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
}

void Shader::setUniform(std::string_view uniformName, const glm::mat4& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &value[0][0]);
}

int Shader::_getAndCacheUniform(std::string_view uniformName) {
	if (auto itr = _uniformsMapping.find(uniformName); itr != _uniformsMapping.end()) {
		return itr->second;
	}

	auto location = glGetUniformLocation(rendererId, uniformName.data());
	_uniformsMapping[uniformName] = location;
	return location;
}

}
