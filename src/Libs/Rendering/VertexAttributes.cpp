#include "VertexAttributes.h"
#include "VertexBuffer.h"

#include <glad/gl.h>

namespace tactics {

void VertexAttributes::Builder::attributef(int count) {
	void* stride = reinterpret_cast<void*>(static_cast<intptr_t>(_stride));
	_attributes.push_back([this, count, index = _attributeIndex, pointer = stride] () {
		glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, _stride, pointer);
		glEnableVertexAttribArray(index);
	});
	++_attributeIndex;
	_stride += count * sizeof(float);
}

std::unique_ptr<VertexAttributes> VertexAttributes::Builder::create() {
	auto attributes = std::make_unique<VertexAttributes>(_stride / static_cast<unsigned int>(sizeof(float)));
	_defineAttributes(*attributes);
	return attributes;
}

VertexAttributes::VertexAttributes(unsigned int componentPerVertex): _componentPerVertex(componentPerVertex) {
	glGenVertexArrays(1, &_vao);
}

VertexAttributes::~VertexAttributes() {
	glDeleteVertexArrays(1, &_vao);
}

void VertexAttributes::bind() {
	glBindVertexArray(_vao);
}

void VertexAttributes::unbind() {
	glBindVertexArray(0);
}

void VertexAttributes::release() {
	glDeleteVertexArrays(1, &_vao);
}

void VertexAttributes::Builder::_defineAttributes(VertexAttributes& vertexAttribute) {
	vertexAttribute.bind();
	for (auto& attribute : _attributes) {
		attribute();
	}
	vertexAttribute.unbind();
}

unsigned int VertexAttributes::getVerticesCount(const VertexBuffer& vbo) const {
	return vbo.getSize() / _componentPerVertex;
}

}
