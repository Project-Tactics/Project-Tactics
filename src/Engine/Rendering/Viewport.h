#pragma once

#include <glm/glm.hpp>

namespace tactics {

class Camera;

class Viewport {
public:
	Viewport(Camera& camera);
	void setDimensions(const glm::ivec2& topLeft, const glm::ivec2& size);
	const glm::ivec2& getSize() const;
	const glm::ivec2& getTopLeft() const;
	void setClearColor(const glm::vec4& color);
	const glm::vec4& getClearColor() const;

private:
	glm::ivec2 _topLeft;
	glm::ivec2 _size;
	glm::vec4 _clearColor;

	Camera& _camera;
};

}
