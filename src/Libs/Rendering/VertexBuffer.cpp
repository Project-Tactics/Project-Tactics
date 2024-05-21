#include "VertexBuffer.h"

#include <glad/gl.h>

namespace tactics {

VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &_id);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &_id);
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const std::vector<float>& data) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	_size = static_cast<unsigned int>(data.size());
}

unsigned int VertexBuffer::getSize() const {
	return _size;
}

}
