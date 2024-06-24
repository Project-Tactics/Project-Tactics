#pragma once

#include <Libs/Utility/Math.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tactics::component {

struct Transform {
private:
	glm::mat4x4 transformMatrix = Matrix4x4::identity;
	glm::vec3 position = Vector3::zero;
	glm::quat rotation = Quaternion::identity;
	glm::vec3 scale = Vector3::one;
	bool _dirty{true};

public:
	void setPosition(const glm::vec3& newPosition);
	void setRotation(const glm::quat& newRotation);
	void setRotation(float eulerX, float eulerY, float eulerZ);
	void setRotation(const glm::vec3& eulerAngles);
	void setRotation(float radians, const glm::vec3& axis);
	void lookAt(const glm::vec3& target, const glm::vec3& up);
	void rotate(float radians, const glm::vec3& axis);
	void setScale(const glm::vec3& newScale);
	const glm::vec3& getPosition() const;
	const glm::quat& getRotation() const;
	const glm::mat4x4& computeMatrix();
	const glm::mat4x4& getMatrix() const;

	void operator=(const nlohmann::ordered_json& json);

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Transform, position, rotation, scale);
	static void defineReflection();
};
}
