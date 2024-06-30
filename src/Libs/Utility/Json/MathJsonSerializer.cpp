#include "MathJsonSerializer.h"

namespace nlohmann {

void adl_serializer<glm::vec4>::to_json(json& j, const glm::vec4& value) {
	j = nlohmann::json{{"x", value.x}, {"y", value.y}, {"z", value.z}, {"w", value.w}};
}

void adl_serializer<glm::vec4>::from_json(const json& j, glm::vec4& value) {
	j.at("x").get_to(value.x);
	j.at("y").get_to(value.y);
	j.at("z").get_to(value.z);
	j.at("w").get_to(value.w);
}

void adl_serializer<glm::vec3>::to_json(json& j, const glm::vec3& value) {
	j = nlohmann::json{{"x", value.x}, {"y", value.y}, {"z", value.z}};
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

void adl_serializer<glm::quat>::to_json(json& j, const glm::quat& value) {
	j = nlohmann::json{{"x", value.x}, {"y", value.y}, {"z", value.z}, {"w", value.w}};
}

void adl_serializer<glm::quat>::from_json(const json& j, glm::quat& value) {
	if (j.contains("fromAngleAxis")) {
		auto& jsonData = j["fromAngleAxis"];
		glm::vec3 axis = jsonData.at("axis");
		float angle = jsonData.at("angle");
		value = glm::angleAxis(glm::radians(angle), axis);
	} else if (j.contains("fromLookAt")) {
		auto& jsonData = j["fromLookAt"];
		glm::vec3 from = jsonData.at("from");
		glm::vec3 to = jsonData.at("to");
		value = glm::quatLookAt(glm::normalize(to - from), glm::vec3(0, 1, 0));
	} else {
		j.at("x").get_to(value.x);
		j.at("y").get_to(value.y);
		j.at("z").get_to(value.z);
		j.at("w").get_to(value.w);
	}
}

void adl_serializer<glm::mat4>::to_json(json& j, const glm::mat4& value) {
	j = nlohmann::json{{"m00", value[0][0]},
					   {"m01", value[0][1]},
					   {"m02", value[0][2]},
					   {"m03", value[0][3]},
					   {"m10", value[1][0]},
					   {"m11", value[1][1]},
					   {"m12", value[1][2]},
					   {"m13", value[1][3]},
					   {"m20", value[2][0]},
					   {"m21", value[2][1]},
					   {"m22", value[2][2]},
					   {"m23", value[2][3]},
					   {"m30", value[3][0]},
					   {"m31", value[3][1]},
					   {"m32", value[3][2]},
					   {"m33", value[3][3]}};
}

void adl_serializer<glm::mat4>::from_json(const json& j, glm::mat4& value) {
	j.at("m00").get_to(value[0][0]);
	j.at("m01").get_to(value[0][1]);
	j.at("m02").get_to(value[0][2]);
	j.at("m03").get_to(value[0][3]);
	j.at("m10").get_to(value[1][0]);
	j.at("m11").get_to(value[1][1]);
	j.at("m12").get_to(value[1][2]);
	j.at("m13").get_to(value[1][3]);
	j.at("m20").get_to(value[2][0]);
	j.at("m21").get_to(value[2][1]);
	j.at("m22").get_to(value[2][2]);
	j.at("m23").get_to(value[2][3]);
	j.at("m30").get_to(value[3][0]);
	j.at("m31").get_to(value[3][1]);
	j.at("m32").get_to(value[3][2]);
	j.at("m33").get_to(value[3][3]);
}

} // namespace nlohmann
