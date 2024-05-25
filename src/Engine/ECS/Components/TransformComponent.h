#pragma once

#include <Libs/Utility/Math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tactics::components {

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

	void setScale(const glm::vec3& newScale) {
		scale = newScale;
		_dirty = true;
		transformMatrix = glm::scale(transformMatrix, scale);
	}

	const glm::vec3& getPosition() const {
		return position;
	}

	const glm::quat& getRotation() const {
		return glm::quat_cast(transformMatrix);
	}

	const glm::mat4x4& computeMatrix() {
		if (_dirty) {
			_dirty = false;
			transformMatrix = glm::mat4_cast(rotation);
			transformMatrix = glm::translate(transformMatrix, position);
			transformMatrix = glm::scale(transformMatrix, scale);
		}
		return transformMatrix;
	}
};
}
