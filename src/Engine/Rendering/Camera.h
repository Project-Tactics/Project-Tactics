#pragma once

#include <glm/glm.hpp>

namespace tactics {

class Viewport;

class Camera {
public:
	Camera();
	~Camera();

	void update();

	void setPosition(const glm::vec3& position);
	void setView(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);
	void setAspectRatio(float aspectRatio);
	void setFov(float fov);
	void setNearPlane(float near);
	void setFarPlane(float far);

	const glm::mat4& getProjection() const;
	const glm::mat4& getView() const;

	const glm::vec3& getPosition() const;
	const glm::vec3& getLookAt() const;
	const glm::vec3& getUp() const;

	float getFov() const;
	float getNearPlane() const;
	float getFarPlane() const;
	float getAspectRatio() const;

private:
	void _updateViewMatrix();

	glm::mat4 _projection;
	glm::mat4 _view;

	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::vec3 _up;
	float _near{};
	float _far{};
	float _fov{};
	float _aspectRatio{};

	bool _viewDirty{};
	bool _projectionDirty{};
};

}
