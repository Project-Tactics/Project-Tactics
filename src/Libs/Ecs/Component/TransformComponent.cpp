#include "TransformComponent.h"

#include <Libs/Utility/Reflection.h>

#include <entt/entt.hpp>

namespace tactics::component {

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
	_dirty = true;
}

void Transform::setRotation(float radians, const glm::vec3& axis) {
	rotation = glm::angleAxis(radians, axis);
	_dirty = true;
}

void Transform::translate(const glm::vec3& translation) {
	position += translation;
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

void Transform::rotate(const glm::quat& amountToRotate) {
	rotation *= amountToRotate;
	_dirty = true;
}

void Transform::setScale(const glm::vec3& newScale) {
	scale = newScale;
	transformMatrix = glm::scale(transformMatrix, scale);
	_dirty = true;
}

void Transform::setScale(float newScale) {
	setScale({newScale, newScale, newScale});
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
		transformMatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation);
		transformMatrix = glm::scale(transformMatrix, scale);
	}
	return transformMatrix;
}

const glm::mat4x4& Transform::getMatrix() const {
	return transformMatrix;
}

} // namespace tactics::component
