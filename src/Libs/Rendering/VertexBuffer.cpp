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

void VertexBuffer::setData(const void* data, size_t size) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

}
