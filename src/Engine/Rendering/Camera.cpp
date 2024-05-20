#include "Camera.h"

#include "Viewport.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <Libs/Utilities/Math.h>

namespace tactics {

Camera::Camera() {
	setAspectRatio(1.0f);
	setFov(45.0f);
	setNearPlane(0.1f);
	setFarPlane(100.0f);
	setView(Vector3::zero, Vector3::forward, Vector3::up);
}

Camera::~Camera() {
}

void Camera::update() {
	if (_viewDirty) {
		_updateViewMatrix();
		_viewDirty = false;
	}

	if (_projectionDirty) {
		_projection = glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
		_projectionDirty = false;
	}
}

void Camera::setFov(float fovAngle) {
	_fov = fovAngle;
	_projectionDirty = true;
}

void Camera::setAspectRatio(float aspectRatio) {
	_aspectRatio = aspectRatio;
	_projectionDirty = true;
}

void Camera::setNearPlane(float near) {
	_near = near;
	_projectionDirty = false;
}

void Camera::setFarPlane(float far) {
	_far = far;
	_projectionDirty = false;
}

void Camera::setPosition(const glm::vec3& position) {
	_position = position;
	_viewDirty = true;
}

void Camera::setView(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up) {
	_position = position;
	_lookAt = lookAt;
	_up = up;
	_viewDirty = true;
}

void Camera::_updateViewMatrix() {
	_view = glm::lookAt(_position, _lookAt, _up);
}

const glm::mat4& Camera::getProjection() const {
	return _projection;
}

const glm::mat4& Camera::getView() const {
	return _view;
}

const glm::vec3& Camera::getPosition() const {
	return _position;
}

float Camera::getAspectRatio() const {
	return _aspectRatio;
}

const glm::vec3& Camera::getLookAt() const {
	return _lookAt;
}

const glm::vec3& Camera::getUp() const {
	return _up;
}

float Camera::getFov() const {
	return _fov;
}

float Camera::getNearPlane() const {
	return _near;
}

float Camera::getFarPlane() const {
	return _far;
}

}
