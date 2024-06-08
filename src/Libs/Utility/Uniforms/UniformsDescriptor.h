#pragma once

#include <Libs/Utility/Json/MathJsonSerializer.h>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace tactics {

namespace resource {
class Uniforms;
class ResourceProvider;
}

struct UniformsDescriptor {
	std::unordered_map<std::string, std::string> textures;
	std::unordered_map<std::string, int> ints;
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, glm::mat4> matrices;
	std::unordered_map<std::string, glm::vec2> vectors2;
	std::unordered_map<std::string, glm::vec3> vectors3;
	std::unordered_map<std::string, glm::vec4> vectors4;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(UniformsDescriptor, textures, vectors4, vectors3, vectors2, floats, ints);

	static void fillUniformsInstance(const UniformsDescriptor& descriptor, resource::Uniforms& uniforms, const resource::ResourceProvider& resourceProvider);
	static void fillUniformsInstanceWithBasicTypes(const UniformsDescriptor& descriptor, resource::Uniforms& uniforms);
};

}
