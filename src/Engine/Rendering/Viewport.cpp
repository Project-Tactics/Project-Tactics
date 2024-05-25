#include "Viewport.h"

#include "Camera.h"

#include <Libs/Utility/Math.h>
#include <Libs/Utility/Color.h>

namespace tactics {

Viewport::Viewport(Camera& camera)
	: _camera(camera)
	, _topLeft(Vector2::zero)
	, _size(Vector2::zero)
	, _clearColor(Color::black) {
}

void Viewport::setDimensions(const glm::ivec2& topLeft, const glm::ivec2& size) {
	_topLeft = topLeft;
	_size = size;
	_camera.setAspectRatio(static_cast<float>(_size.x) / static_cast<float>(_size.y));
}

const glm::ivec2& Viewport::getTopLeft() const {
	return _topLeft;
}

const glm::ivec2& Viewport::getSize() const {
	return _size;
}

void Viewport::setClearColor(const glm::vec4& color) {
	_clearColor = color;
}

const glm::vec4& Viewport::getClearColor() const {
	return _clearColor;
}

}
