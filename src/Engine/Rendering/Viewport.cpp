#include "Viewport.h"

namespace tactics {

Viewport::Viewport(const glm::ivec2& size): _size(size) {}

void Viewport::setSize(const glm::ivec2& size) {
	_size = size;
}

int Viewport::getWidth() const {
	return _size.x;
}

int Viewport::getHeight() const {
	return _size.y;
}

const glm::ivec2& Viewport::getSize() const {
	return _size;
}

}
