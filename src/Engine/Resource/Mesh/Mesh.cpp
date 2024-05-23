#include "Mesh.h"

#include <Libs/Utilities/Exception.h>

namespace tactics {

Mesh::Mesh(std::string_view name, std::unique_ptr<VertexBuffer> vb, std::unique_ptr<IndexBuffer> ib):
	Resource(name), vertexBuffer(std::move(vb)), indexBuffer(std::move(ib)) {
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

Mesh::Mesh(std::string_view name, std::vector<float> vertexBuffer, std::vector<unsigned int> indexBuffer):
	Mesh(name, std::make_unique<VertexBuffer>(vertexBuffer), std::make_unique<IndexBuffer>(indexBuffer)) {
}

}
