#include "VertexDefinition.h"

#include <glad/gl.h>

namespace tactics {

void VertexDefinition::Builder::attributef(int count) {
	void* stride = reinterpret_cast<void*>(static_cast<intptr_t>(_stride));
	_attributes.push_back([this, count, index = _attributeIndex, pointer = stride] () {
		glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, _stride, pointer);
		glEnableVertexAttribArray(index);
	});
	++_attributeIndex;
	_stride += count * sizeof(float);
}

VertexDefinition* VertexDefinition::Builder::create() {
	auto vd = new VertexDefinition();
	vd->bind();
	for (auto& attribute : _attributes) {
		attribute();
	}
	vd->unbind();
	return vd;
}

VertexDefinition::VertexDefinition() {
	glGenVertexArrays(1, &vao);
}

VertexDefinition::~VertexDefinition() {
	glDeleteVertexArrays(1, &vao);
}

void VertexDefinition::bind() {
	glBindVertexArray(vao);
}

void VertexDefinition::unbind() {
	glBindVertexArray(0);
}

}
