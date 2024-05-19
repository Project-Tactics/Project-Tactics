#include "Camera.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <Libs/Utilities/Math.h>

namespace tactics {

Camera::Camera() {
	setProjection(45, 1280.f / 720.f, 0.1f, 100.0f);
	setView(Vector::zero, Vector::forward, Vector::up);
}

Camera::~Camera() {
}

void Camera::update() {
	if (_viewDirty) {
		_updateViewMatrix();
		_viewDirty = false;
	}
}

void Camera::setProjection(float fovAngle, float aspectRatio, float near, float far) {
	_fov = fovAngle;
	_aspectRatio = aspectRatio;
	_near = near;
	_far = far;
	_projection = glm::perspective(glm::radians(fovAngle), aspectRatio, near, far);
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

const glm::vec3& Camera::getLookAt() const {
	return _lookAt;
}

const glm::vec3& Camera::getUp() const {
	return _up;
}

float Camera::getFov() const {
	return _fov;
}

float Camera::getAspectRatio() const {
	return _aspectRatio;
}

float Camera::getNearPlane() const {
	return _near;
}

float Camera::getFarPlane() const {
	return _far;
}

}
