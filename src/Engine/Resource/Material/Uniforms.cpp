#include "Uniforms.h"

namespace tactics::resource {

void Uniforms::set(std::string_view uniformName, int value) {
	_ints[uniformName] = value;
}

void Uniforms::set(std::string_view uniformName, float value) {
	_floats[uniformName] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::vec2& value) {
	_vectors2[uniformName] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::vec3& value) {
	_vectors3[uniformName] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::vec4& value) {
	_vectors4[uniformName] = value;
}

void Uniforms::set(std::string_view uniformName, const glm::mat4& value) {
	_matrices[uniformName] = value;
}

void Uniforms::set(std::string_view uniformName, std::shared_ptr<resource::Texture> value) {
	_textures[uniformName] = value;
}

const UnorderedStringMap<std::shared_ptr<resource::Texture>>& Uniforms::getTextures() const {
	return _textures;
}

const UnorderedStringMap<int>& Uniforms::getInts() const {
	return _ints;
}

const UnorderedStringMap<float>& Uniforms::getFloats() const {
	return _floats;
}

const UnorderedStringMap<glm::vec2>& Uniforms::getVectors2() const {
	return _vectors2;
}

const UnorderedStringMap<glm::vec3>& Uniforms::getVectors3() const {
	return _vectors3;
}

const UnorderedStringMap<glm::vec4>& Uniforms::getVectors4() const {
	return _vectors4;
}

const UnorderedStringMap<glm::mat4>& Uniforms::getMatrices() const {
	return _matrices;
}

void Uniforms::set(const std::unordered_map<std::string, std::shared_ptr<resource::Texture>>& textures) {
	assign(textures, _textures);
}

void Uniforms::set(const std::unordered_map<std::string, int>& ints) {
	assign(ints, _ints);
}

void Uniforms::set(const std::unordered_map<std::string, float>& floats) {
	assign(floats, _floats);
}

void Uniforms::set(const std::unordered_map<std::string, glm::vec2>& vectors2) {
	assign(vectors2, _vectors2);
}

void Uniforms::set(const std::unordered_map<std::string, glm::vec3>& vectors3) {
	assign(vectors3, _vectors3);
}

void Uniforms::set(const std::unordered_map<std::string, glm::vec4>& vectors4) {
	assign(vectors4, _vectors4);
}

void Uniforms::set(const std::unordered_map<std::string, glm::mat4>& matrices) {
	assign(matrices, _matrices);
}

}
