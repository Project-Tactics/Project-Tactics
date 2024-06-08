#include "TransformComponent.h"

#include "../EntityUtilities.h"
#include "../Entity.h"

#include <entt/entt.hpp>

namespace tactics::component {

void Transform::defineReflection() {
	entt::meta<Transform>()
		.type(hash("transform"))
		.func<&Entity::explicitAddComponent<Transform>>(hash("emplace"))
		.data<&Transform::position>(hash("position"))
		.data<&Transform::rotation>(hash("rotation"))
		.data<&Transform::scale>(hash("scale"));
}

void Transform::setPosition(const glm::vec3& newPosition) {
	position = newPosition;
	_dirty = true;
}

void Transform::setRotation(const glm::quat& newRotation) {
	rotation = newRotation;
	_dirty = true;
}

void Transform::setRotation(float eulerX, float eulerY, float eulerZ) {
	setRotation({eulerX, eulerY, eulerZ});
	_dirty = true;
}

void Transform::setRotation(const glm::vec3& eulerAngles) {
	rotation = glm::quat(eulerAngles);
}

void Transform::setRotation(float radians, const glm::vec3& axis) {
	rotation = glm::angleAxis(radians, axis);
	_dirty = true;
}

void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
	rotation = glm::quatLookAt(glm::normalize(target - position), up);
	_dirty = true;
}

void Transform::rotate(float radians, const glm::vec3& axis) {
	rotation = glm::angleAxis(radians, axis) * rotation;
	_dirty = true;
}

void Transform::setScale(const glm::vec3& newScale) {
	scale = newScale;
	_dirty = true;
	transformMatrix = glm::scale(transformMatrix, scale);
}

const glm::vec3& Transform::getPosition() const {
	return position;
}

const glm::quat& Transform::getRotation() const {
	return rotation;
}

const glm::mat4x4& Transform::computeMatrix() {
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

const glm::mat4x4& Transform::getMatrix() const {
	return transformMatrix;
}

}