#pragma once

#include <glm/glm.hpp>

namespace tactics {

class Camera {
public:
	Camera();
	~Camera();

	void update();

	void setProjection(float fovAngle, float aspectRatio, float near, float far);
	void setPosition(const glm::vec3& position);
	void setView(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

	const glm::mat4& getProjection() const;
	const glm::mat4& getView() const;

	const glm::vec3& getPosition() const;
	const glm::vec3& getLookAt() const;
	const glm::vec3& getUp() const;

	float getFov() const;
	float getAspectRatio() const;
	float getNearPlane() const;
	float getFarPlane() const;

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
};

}
