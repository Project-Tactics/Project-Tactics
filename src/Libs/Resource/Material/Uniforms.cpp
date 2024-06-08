#include "Uniforms.h"

namespace tactics::resource {

void Uniforms::set(std::string_view uniformName, int value) {
	_ints[std::string(uniformName)] = value;
}

void Uniforms::set(std::string_view uniformName, float value) {
	_floats[std::string(uniformName)] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::vec2& value) {
	_vectors2[std::string(uniformName)] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::vec3& value) {
	_vectors3[std::string(uniformName)] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::vec4& value) {
	_vectors4[std::string(uniformName)] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::mat4& value) {
	_matrices[std::string(uniformName)] = value;
}

void Uniforms::set(std::string_view uniformName, std::shared_ptr<resource::Texture> value) {
	_textures[std::string(uniformName)] = value;
}

const std::unordered_map<std::string, std::shared_ptr<resource::Texture>>& Uniforms::getTextures() const {
	return _textures;
}

const std::unordered_map<std::string, int>& Uniforms::getInts() const {
	return _ints;
}

const std::unordered_map<std::string, float>& Uniforms::getFloats() const {
	return _floats;
}

const std::unordered_map<std::string, glm::vec2>& Uniforms::getVectors2() const {
	return _vectors2;
}

const std::unordered_map<std::string, glm::vec3>& Uniforms::getVectors3() const {
	return _vectors3;
}

const std::unordered_map<std::string, glm::vec4>& Uniforms::getVectors4() const {
	return _vectors4;
}

const std::unordered_map<std::string, glm::mat4>& Uniforms::getMatrices() const {
	return _matrices;
}

void Uniforms::set(const std::unordered_map<std::string, std::shared_ptr<resource::Texture>>& textures) {
	_textures = textures;
}

void Uniforms::set(const std::unordered_map<std::string, int>& ints) {
	_ints = ints;
}

void Uniforms::set(const std::unordered_map<std::string, float>& floats) {
	_floats = floats;
}

void Uniforms::set(const std::unordered_map<std::string, glm::vec2>& vectors2) {
	_vectors2 = vectors2;
}

void Uniforms::set(const std::unordered_map<std::string, glm::vec3>& vectors3) {
	_vectors3 = vectors3;
}

void Uniforms::set(const std::unordered_map<std::string, glm::vec4>& vectors4) {
	_vectors4 = vectors4;
}

void Uniforms::set(const std::unordered_map<std::string, glm::mat4>& matrices) {
	_matrices = matrices;
}

}
