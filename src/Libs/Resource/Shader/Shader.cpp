#include "Shader.h"

#include <Libs/Rendering/RenderCalls/RenderCalls.h>
#include <Libs/Utility/Exception.h>

namespace tactics::resource {

Shader::~Shader() {
	render::pipeline::deleteShaderProgram(rendererId);
}

void Shader::bind() const {
	render::pipeline::activateShaderProgram(rendererId);
}

void Shader::setUniform(std::string_view uniformName, int value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	render::pipeline::setShaderVar(uniformLocation, value);
}

void Shader::setUniform(std::string_view uniformName, float value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	render::pipeline::setShaderVar(uniformLocation, value);
}

void Shader::setUniform(std::string_view uniformName, const glm::vec2& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	render::pipeline::setShaderVar(uniformLocation, value);
}

void Shader::setUniform(std::string_view uniformName, const glm::vec3& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	render::pipeline::setShaderVar(uniformLocation, value);
}

void Shader::setUniform(std::string_view uniformName, const glm::vec4& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	render::pipeline::setShaderVar(uniformLocation, value);
}

void Shader::setUniform(std::string_view uniformName, const glm::mat4& value) {
	int uniformLocation = _getAndCacheUniform(uniformName);
	render::pipeline::setShaderVar(uniformLocation, value);
}

bool Shader::hasUniform(std::string_view uniformName) const {
	return _uniformsMapping.find(uniformName) != _uniformsMapping.end() ||
		   render::pipeline::getShaderVarLocation(rendererId, uniformName.data()) != -1;
}

int Shader::_getAndCacheUniform(std::string_view uniformName) {
	if (auto itr = _uniformsMapping.find(uniformName); itr != _uniformsMapping.end()) {
		return itr->second;
	}

	auto location = render::pipeline::getShaderVarLocation(rendererId, uniformName.data());
	if (location == -1) {
		throw TACTICS_EXCEPTION("Uniform '{}' not found in shader program.", uniformName);
	}
	_uniformsMapping[uniformName] = location;
	return location;
}

} // namespace tactics::resource
