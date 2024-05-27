#pragma once

#include "VertexAttributes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <memory>
#include <string_view>
#include <vector>

namespace tactics {

class GeometryBuilder {
public:
	struct Attributef {
		unsigned int size;
	};

	GeometryBuilder(std::vector<Attributef> attributes);
	void addVertex(std::vector<float> vertex);
	void addIndex(unsigned int index);
	void addIndices(std::vector<unsigned int> indices);

	template<typename TMesh>
	std::shared_ptr<TMesh> build(std::string_view name) {
		auto vbo = std::make_unique<VertexBuffer>(_vertices);
		auto ibo = std::make_unique<IndexBuffer>(_indices);
		vbo->bind();
		auto builder = VertexAttributes::Builder();
		for (auto& attribute : _attributes) {
			builder.attributef(attribute.size);
		}
		auto vao = builder.create();
		vbo->unbind();
		return std::make_shared<TMesh>(std::string(name), std::move(vbo), std::move(ibo), std::move(vao));
	}

private:
	std::vector<Attributef> _attributes;
	std::vector<float> _vertices;
	std::vector<unsigned int> _indices;
};

}
