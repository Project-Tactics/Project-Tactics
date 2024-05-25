#include "Mesh.h"

#include <Libs/Utility/Exception.h>

namespace tactics::resource {

Mesh::Mesh(std::string_view name, std::unique_ptr<VertexBuffer> vb, std::unique_ptr<IndexBuffer> ib, std::unique_ptr<VertexAttributes> vao)
	: Resource(name)
	, vertexBuffer(std::move(vb))
	, indexBuffer(std::move(ib))
	, vertexAttributes(std::move(vao)) {
	if (vertexBuffer->getSize() == 0) {
		throw Exception("Vertex buffer must not be empty, Mesh: {}", name);
	}

	if (indexBuffer->getSize() == 0) {
		throw Exception("Index buffer must not be empty, Mesh: {}", name);
	}

	if (indexBuffer->getSize() % 3 != 0) {
		throw Exception("Index buffer size must be a multiple of 3, Mesh: {}", name);
	}
}

}
