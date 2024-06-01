#pragma once

#include <Libs/Utility/Math.h>

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
	bool _dirty{};

public:
	void setPosition(const glm::vec3& newPosition) {
		position = newPosition;
		_dirty = true;
	}

	void setRotation(const glm::quat& newRotation) {
		rotation = newRotation;
		_dirty = true;
	}

	void setRotation(float eulerX, float eulerY, float eulerZ) {
		setRotation({eulerX, eulerY, eulerZ});
		_dirty = true;
	}

	void setRotation(const glm::vec3& eulerAngles) {
		rotation = glm::quat(eulerAngles);
	}

	void setRotation(float radians, const glm::vec3& axis) {
		rotation = glm::angleAxis(radians, axis);
		_dirty = true;
	}

	void lookAt(const glm::vec3& target, const glm::vec3& up) {
		rotation = glm::quatLookAt(glm::normalize(target - position), up);
		_dirty = true;
	}

	void rotate(float radians, const glm::vec3& axis) {
		rotation = glm::angleAxis(radians, axis) * rotation;
		_dirty = true;
	}

	void setScale(const glm::vec3& newScale) {
		scale = newScale;
		_dirty = true;
		transformMatrix = glm::scale(transformMatrix, scale);
	}

	const glm::vec3& getPosition() const {
		return position;
	}

	const glm::quat& getRotation() const {
		return rotation;
	}

	const glm::mat4x4& computeMatrix() {
		if (_dirty) {
			_dirty = false;
			glm::mat4 pivotTransform = glm::translate(glm::mat4(1.0f), position);
			glm::mat4 rotationTransform = glm::mat4_cast(rotation);
			glm::mat4 finalTransform = glm::translate(pivotTransform * rotationTransform, -position);
			transformMatrix = finalTransform;
			transformMatrix = glm::translate(transformMatrix, position);
			transformMatrix = glm::scale(transformMatrix, scale);
		}
		return transformMatrix;
	}
};
}
