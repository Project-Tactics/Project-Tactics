#include "VertexAttributes.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include <glad/gl.h>

namespace tactics {

// Vertex Attributes Builder

void VertexAttributes::Builder::attributef(int count) {
	void* stride = reinterpret_cast<void*>(static_cast<intptr_t>(_stride));
	_attributes.push_back([this, count, index = _attributeIndex, pointer = stride]() {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, _stride, pointer);
	});
	++_attributeIndex;
	_stride += count * sizeof(float);
}

VertexAttributes VertexAttributes::Builder::create(VertexBuffer& vb, IndexBuffer& ib) {
	auto attributes = VertexAttributes();
	create(attributes, vb, ib);
	return attributes;
}

VertexAttributes VertexAttributes::Builder::create(VertexBuffer& vb) {
	auto attributes = VertexAttributes();
	create(attributes, vb);
	return attributes;
}

void VertexAttributes::Builder::create(VertexAttributes& va, VertexBuffer& vb, IndexBuffer& ib) {
	va.setComponentPerVertex(_stride / static_cast<unsigned int>(sizeof(float)));
	va.bind();
	vb.bind();
	ib.bind();
	_defineAttributes();
	vb.unbind();
	va.unbind();
}

void VertexAttributes::Builder::create(VertexAttributes& va, VertexBuffer& vb) {
	va.setComponentPerVertex(_stride / static_cast<unsigned int>(sizeof(float)));
	va.bind();
	vb.bind();
	_defineAttributes();
	vb.unbind();
	va.unbind();
}

void VertexAttributes::Builder::_defineAttributes() {
	for (auto& attribute : _attributes) {
		attribute();
	}
}

// Vertex Attributes

VertexAttributes::VertexAttributes() {
	glGenVertexArrays(1, &_vao);
}

VertexAttributes::~VertexAttributes() {
	if (_vao == 0) {
		return;
	}
	glDeleteVertexArrays(1, &_vao);
}

VertexAttributes::VertexAttributes(VertexAttributes&& other) noexcept {
	_vao = other._vao;
	_componentPerVertex = other._componentPerVertex;
	other._vao = 0;
	other._componentPerVertex = 0;
}

VertexAttributes& VertexAttributes::operator=(VertexAttributes&& other) noexcept {
	if (this != &other) {
		release();
		_vao = other._vao;
		_componentPerVertex = other._componentPerVertex;
		other._vao = 0;
		other._componentPerVertex = 0;
	}
	return *this;
}

void VertexAttributes::bind() const {
	glBindVertexArray(_vao);
}

void VertexAttributes::unbind() const {
	glBindVertexArray(0);
}

void VertexAttributes::release() {
	glDeleteVertexArrays(1, &_vao);
	_vao = 0;
}

bool VertexAttributes::isValid() const {
	return _vao != 0;
}

unsigned int VertexAttributes::getVerticesCount(const VertexBuffer& vbo) const {
	return vbo.getSize() / _componentPerVertex;
}

void VertexAttributes::setComponentPerVertex(unsigned int componentPerVertex) {
	_componentPerVertex = componentPerVertex;
}

} // namespace tactics
