#pragma once

#include <Libs/Utility/Json/Json.h>
#include <Libs/Utility/Json/JsonSerialization.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace tactics {

namespace resource {
class Uniforms;
class ResourceProvider;
} // namespace resource

struct UniformsDescriptor {
	std::unordered_map<std::string, std::string> textures;
	std::unordered_map<std::string, int> ints;
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, glm::mat4> matrices;
	std::unordered_map<std::string, glm::vec2> vectors2;
	std::unordered_map<std::string, glm::vec3> vectors3;
	std::unordered_map<std::string, glm::vec4> vectors4;

	JSON_SERIALIZE(UniformsDescriptor, textures, vectors4, vectors3, vectors2, floats, ints);

	static void fillUniformsInstance(const UniformsDescriptor& descriptor,
									 resource::Uniforms& uniforms,
									 const resource::ResourceProvider& resourceProvider);
	static void fillUniformsInstanceWithBasicTypes(const UniformsDescriptor& descriptor, resource::Uniforms& uniforms);
};

} // namespace tactics
