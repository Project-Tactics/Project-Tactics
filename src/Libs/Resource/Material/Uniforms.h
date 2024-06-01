#pragma once


#include <Libs/Resource/Texture/Texture.h>
#include <Libs/Resource/Shader/Shader.h>
#include <Libs/Utility/TransparentTypes.h>

#include <string>
#include <string_view>
#include <memory>
#include <glm/glm.hpp>

namespace tactics::resource {

class Uniforms {
public:
	void set(std::string_view uniformName, std::shared_ptr<Texture> value);
	void set(std::string_view uniformName, int value);
	void set(std::string_view uniformName, float value);
	void set(std::string_view uniformName, const glm::vec2& value);
	void set(std::string_view uniformName, const glm::vec3& value);
	void set(std::string_view uniformName, const glm::vec4& value);
	void set(std::string_view uniformName, const glm::mat4& value);

	void set(const std::unordered_map<std::string, std::shared_ptr<Texture>>& textures);
	void set(const std::unordered_map<std::string, int>& ints);
	void set(const std::unordered_map<std::string, float>& floats);
	void set(const std::unordered_map<std::string, glm::vec2>& vectors2);
	void set(const std::unordered_map<std::string, glm::vec3>& vectors3);
	void set(const std::unordered_map<std::string, glm::vec4>& vectors4);
	void set(const std::unordered_map<std::string, glm::mat4>& matrices);

	const UnorderedStringMap<std::shared_ptr<Texture>>& getTextures() const;
	const UnorderedStringMap<int>& getInts() const;
	const UnorderedStringMap<float>& getFloats() const;
	const UnorderedStringMap<glm::vec2>& getVectors2() const;
	const UnorderedStringMap<glm::vec3>& getVectors3() const;
	const UnorderedStringMap<glm::vec4>& getVectors4() const;
	const UnorderedStringMap<glm::mat4>& getMatrices() const;

private:
	UnorderedStringMap<std::shared_ptr<Texture>> _textures;
	UnorderedStringMap<int> _ints;
	UnorderedStringMap<float> _floats;
	UnorderedStringMap<glm::vec2> _vectors2;
	UnorderedStringMap<glm::vec3> _vectors3;
	UnorderedStringMap<glm::vec4> _vectors4;
	UnorderedStringMap<glm::mat4> _matrices;
};

}
