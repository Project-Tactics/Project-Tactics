#include "MathJsonSerializer.h"

namespace nlohmann {

void adl_serializer<glm::vec4>::to_json(json& j, const glm::vec4& value) {
	j = nlohmann::json{
		{"x", value.x},
		{"y", value.y},
		{"z", value.z},
		{"w", value.w}
	};
}

void adl_serializer<glm::vec4>::from_json(const json& j, glm::vec4& value) {
	j.at("x").get_to(value.x);
	j.at("y").get_to(value.y);
	j.at("z").get_to(value.z);
	j.at("w").get_to(value.w);
}

void adl_serializer<glm::vec3>::to_json(json& j, const glm::vec3& value) {
	j = nlohmann::json{
		{"x", value.x},
		{"y", value.y},
		{"z", value.z}
	};
}

void adl_serializer<glm::vec3>::from_json(const json& j, glm::vec3& value) {
	j.at("x").get_to(value.x);
	j.at("y").get_to(value.y);
	j.at("z").get_to(value.z);
}

void adl_serializer<glm::vec2>::to_json(json& j, const glm::vec2& value) {
	j = nlohmann::json{
		{"x", value.x},
		{"y", value.y},
	};
}

void adl_serializer<glm::vec2>::from_json(const json& j, glm::vec2& value) {
	j.at("x").get_to(value.x);
	j.at("y").get_to(value.y);
}

}
