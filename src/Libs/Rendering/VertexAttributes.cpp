#include "VertexAttributes.h"

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
	auto attributes = std::make_unique<VertexAttributes>();
	_defineAttributes(*attributes);
	return attributes;
}

void VertexAttributes::Builder::create(VertexAttributes* outVertexAttribute) {
	outVertexAttribute->bind();
	for (auto& attribute : _attributes) {
		attribute();
	}
	outVertexAttribute->unbind();
}

VertexAttributes::VertexAttributes() {
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

}
