#include "IndexBuffer.h"

#include <glad/gl.h>

namespace tactics {

IndexBuffer::IndexBuffer() {
	glGenBuffers(1, &_id);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &_id);
}

void IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::setData(const std::vector<unsigned int>& data) {
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
	_size = static_cast<unsigned int>(data.size());
}

unsigned int IndexBuffer::getSize() const {
	return _size;
}

}
