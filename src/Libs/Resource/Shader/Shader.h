#pragma once

#include <Libs/Resource/Resource.h>
#include <Libs/Utility/TransparentTypes.h>

#include <glm/glm.hpp>

namespace tactics::resource {

class Shader : public Resource<Shader> {
public:
	static const ResourceType TYPE = ResourceType::Shader;
	using Resource<Shader>::Resource;
	~Shader();
	void bind() const;
	void setUniform(std::string_view uniformName, int value);
	void setUniform(std::string_view uniformName, float value);
	void setUniform(std::string_view uniformName, const glm::vec2& value);
	void setUniform(std::string_view uniformName, const glm::vec3& value);
	void setUniform(std::string_view uniformName, const glm::vec4& value);
	void setUniform(std::string_view uniformName, const glm::mat4& value);
	bool hasUniform(std::string_view uniformName) const;

	unsigned int rendererId;
	std::string fragmentSource;
	std::string vertexSource;

private:
	// TODO(Gerark) Instead of trying to get or lazily create the uniform location, we should cache the uniform location
	// when the shader is created by inspecting the shader source code and storing the uniform locations in a map. This
	// way we can avoid the overhead of calling this function every time we want to set a uniform.
	int _getAndCacheUniform(std::string_view uniformName);
	UnorderedStringMap<int> _uniformsMapping;
};

} // namespace tactics::resource
